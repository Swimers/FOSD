cmake_minimum_required(VERSION 3.16)

project(FOSD VERSION 0.1.0 LANGUAGES CXX)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# ---------------------------------------------------------------------------
# Build options
# ---------------------------------------------------------------------------
option(FOSD_BUILD_TESTS "Build the unit test suite" ON)

# Qt5 kit directory. Override on the command line when the kit lives elsewhere:
#   cmake -DFOSD_QT_ROOT=<path-to-qt-kit> ..
set(FOSD_QT_ROOT "d:/AppData/Qt/Qt5.12.2/5.12.2/msvc2017_64"
    CACHE PATH "Qt5 kit directory")

if(FOSD_QT_ROOT)
    list(APPEND CMAKE_PREFIX_PATH "${FOSD_QT_ROOT}")
endif()

# ---------------------------------------------------------------------------
# Global C++ / Qt settings
# ---------------------------------------------------------------------------
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Group every artifact under build/bin and build/lib instead of scattering
# them per subdirectory.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

include(Windows)

# ---------------------------------------------------------------------------
# Dependencies
# ---------------------------------------------------------------------------
find_package(Qt5 5.12 REQUIRED COMPONENTS
    Core
    Widgets
    SerialPort
    Network
)

if(FOSD_BUILD_TESTS)
    find_package(Qt5 5.12 REQUIRED COMPONENTS Test)
    enable_testing()
endif()

# ---------------------------------------------------------------------------
# Targets
# ---------------------------------------------------------------------------
add_subdirectory(src)
add_subdirectory(apps/FOSD-Win)

if(FOSD_BUILD_TESTS)
    add_subdirectory(test)
endif()
