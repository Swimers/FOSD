cmake_minimum_required(VERSION 3.16)

project(FOSD VERSION 0.1.0 LANGUAGES CXX)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# ---------------------------------------------------------------------------
# Build options
# ---------------------------------------------------------------------------
option(FOSD_BUILD_TESTS "Build the unit test suite" ON) 
option(FOSD_BUILD_MAIN_APP "Build the main FOSD application" ON)
option(FOSD_BUILD_COMMUNICATION_ASSISTANT "Build the standalone raw communication assistant" ON)
option(FOSD_ASSISTANT_BUILD_TESTS "Build the communication assistant tests" ON)

if(FOSD_ASSISTANT_BUILD_TESTS AND NOT FOSD_BUILD_COMMUNICATION_ASSISTANT)
    message(FATAL_ERROR "Assistant tests require FOSD_BUILD_COMMUNICATION_ASSISTANT=ON")
endif()

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
    SerialPort
    Network
)

if(FOSD_BUILD_MAIN_APP)
    find_package(Qt5 5.12 REQUIRED COMPONENTS Widgets)
endif()
if(FOSD_BUILD_COMMUNICATION_ASSISTANT)
    find_package(Qt5 5.12 REQUIRED COMPONENTS Qml Quick QuickControls2)
endif()
if(FOSD_BUILD_TESTS OR FOSD_ASSISTANT_BUILD_TESTS)
    find_package(Qt5 5.12 REQUIRED COMPONENTS Test)
    enable_testing()
endif()

# ---------------------------------------------------------------------------
# Targets
# ---------------------------------------------------------------------------
add_subdirectory(src)
if(FOSD_BUILD_MAIN_APP)
    add_subdirectory(apps/FOSD-Win)
endif()
if(FOSD_BUILD_COMMUNICATION_ASSISTANT)
    add_subdirectory(ui/CommunicationAssistant)
    add_subdirectory(apps/CommunicationAssistant)
endif()

if(FOSD_BUILD_TESTS)
    add_subdirectory(test)
endif()
