project(fedora-plasma-first-login)

cmake_minimum_required(VERSION 2.8.0)

cmake_policy(SET CMP0005 NEW)

set(FPFL_VERSION_MAJOR   0)
set(FPFL_VERSION_MINOR   1)
set(FPFL_VERSION_RELEASE 0)
set(FPFL_VERSION_STRING "${FPFL_VERSION_MAJOR}.${FPFL_VERSION_MINOR}.${FPFL_VERSION_RELEASE}")
add_definitions(-DFPFL_VERSION="\\\"${FPFL_VERSION_STRING}\\\"")

set (QT_MIN_VERSION "4.8.0")
find_package(KDE4 4.12.0 REQUIRED)
include(KDE4Defaults)
include(MacroLibrary)
include(KDEPlatformProfile)
include(FeatureSummary)

find_package(packagekit-qt2 REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_LINK_INTERFACE_LIBRARIES "")

include(GenerateExportHeader)
include(CMakeDependentOption)

add_definitions(-DQT_NO_CAST_FROM_ASCII
                -DQT_NO_CAST_TO_ASCII
                -DQT_NO_KEYWORDS
                -DQT_USE_FAST_CONCATENATION
                -DQT_USE_FAST_OPERATOR_PLUS
                -DQT_STRICT_ITERATORS
)

include_directories(${QT_INCLUDES}
                    ${KDE4_INCLUDES}
                    ${PACKAGEKIT_QT2_INCLUDE_DIR}
)

add_subdirectory(src)
