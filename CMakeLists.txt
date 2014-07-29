project(fedora-plasma-first-login)

cmake_minimum_required(VERSION 2.8.0)

cmake_policy(SET CMP0005 NEW)

set(FPFL_VERSION_MAJOR   0)
set(FPFL_VERSION_MINOR   2)
set(FPFL_VERSION_RELEASE 0)
set(FPFL_VERSION_STRING "${FPFL_VERSION_MAJOR}.${FPFL_VERSION_MINOR}.${FPFL_VERSION_RELEASE}")
add_definitions(-DFPFL_VERSION="\\\"${FPFL_VERSION_STRING}\\\"")

find_package(ECM 0.0.11 REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR})
set(CMAKE_AUTOMOC ON)

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings)
include(ECMInstallIcons)
include(FeatureSummary)
include(CheckIncludeFiles)

find_package(Qt5 CONFIG REQUIRED COMPONENTS
    Core
    Gui
    Widgets # FIXME: QApplication
    Quick
    Script
)

find_package(KF5 CONFIG REQUIRED COMPONENTS
    Plasma
    I18n
    KDE4Support # required by Plasma
    Config
)

# http://copr.fedoraproject.org/coprs/dvratil/PackageKit-Qt5/
find_package(packagekitqt5 REQUIRED)

add_definitions(
    -DQT_NO_CAST_FROM_ASCII
    -DQT_NO_CAST_TO_ASCII
    -DQT_NO_KEYWORDS
    -DQT_USE_FAST_CONCATENATION
    -DQT_USE_FAST_OPERATOR_PLUS
    -DQT_STRICT_ITERATORS
    -DCMAKE_SOURCE_DIR="${CMAKE_SOURCE_DIR}"
)


if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_C_COMPILER MATCHES "icc" OR (CMAKE_CXX_COMPILER_ID MATCHES "Clang"))
  # more aggressive warnings and C++11 support
  set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS} -std=iso9899:1990 -Wall -Wextra")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -Wall -Wextra")
endif()


add_subdirectory(src)

install(DIRECTORY data/ DESTINATION ${DATA_INSTALL_DIR}/fedora-plasma-first-login)
