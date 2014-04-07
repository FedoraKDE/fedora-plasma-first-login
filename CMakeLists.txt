project(fedora-plasma-first-login)

cmake_minimum_required(VERSION 2.8.0)

cmake_policy(SET CMP0005 NEW)

set(FPFL_VERSION_MAJOR   0)
set(FPFL_VERSION_MINOR   1)
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
)

find_package(KF5 CONFIG REQUIRED COMPONENTS
    Plasma
    I18n
    Service     # required by Plasma
    KDE4Support # required by Plasma
)

# FIXME: Need Qt 5 version!
#find_package(packagekit-qt2 REQUIRED)

add_definitions(
    -DQT_NO_CAST_FROM_ASCII
    -DQT_NO_CAST_TO_ASCII
    -DQT_NO_KEYWORDS
    -DQT_USE_FAST_CONCATENATION
    -DQT_USE_FAST_OPERATOR_PLUS
    -DQT_STRICT_ITERATORS
    -DCMAKE_SOURCE_DIR="${CMAKE_SOURCE_DIR}"
)

include_directories(${PACKAGEKIT_QT2_INCLUDE_DIR})

add_subdirectory(src)

install(DIRECTORY data/ DESTINATION ${DATA_INSTALL_DIR}/fedora-plasma-first-login)
