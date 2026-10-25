cmake_minimum_required(VERSION 3.16)

project(FOSD VERSION 0.1.0 LANGUAGES CXX)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# ---------------------------------------------------------------------------
# Build options
# ---------------------------------------------------------------------------
option(FOSD_BUILD_TESTS "Build the unit test suite" ON) 
option(FOSD_BUILD_VISUALIZATION_TESTS "Build VTK runtime tests (requires matching runtime DLLs)" OFF)
if(FOSD_BUILD_VISUALIZATION_TESTS AND NOT FOSD_BUILD_TESTS)
    message(FATAL_ERROR "Visualization runtime tests require FOSD_BUILD_TESTS=ON")
endif()
option(FOSD_BUILD_MAIN_APP "Build the main FOSD application" ON)
option(FOSD_BUILD_WORKBENCH_APP "Build the standalone FOSD-Classic-QWidgets preview application" OFF)
option(FOSD_WORKBENCH_BUILD_TESTS "Build the standalone FOSD-Classic-QWidgets tests" OFF)
option(FOSD_BUILD_VTK_NATIVE_TEST_APP "Build the QVTKOpenGLNativeWidget validation application" OFF)
option(FOSD_VTK_NATIVE_BUILD_TESTS "Build the native VTK validation presenter tests" OFF)
option(FOSD_BUILD_COMMUNICATION_ASSISTANT_QML "Build the standalone QML communication assistant" ON)
option(FOSD_ASSISTANT_QML_BUILD_TESTS "Build the QML communication assistant tests" ON)
option(FOSD_BUILD_WIDGETS_ASSISTANT "Build the QWidgets communication assistant" OFF)
option(FOSD_WIDGETS_ASSISTANT_BUILD_TESTS "Build QWidgets assistant tests" OFF)

if(FOSD_WORKBENCH_BUILD_TESTS AND NOT (FOSD_BUILD_WORKBENCH_APP OR FOSD_BUILD_MAIN_APP OR FOSD_BUILD_TESTS))
    message(FATAL_ERROR "Workbench tests require FOSD_BUILD_WORKBENCH_APP, FOSD_BUILD_MAIN_APP or FOSD_BUILD_TESTS=ON")
endif()
if(FOSD_VTK_NATIVE_BUILD_TESTS AND NOT FOSD_BUILD_VTK_NATIVE_TEST_APP)
    message(FATAL_ERROR "Native VTK tests require FOSD_BUILD_VTK_NATIVE_TEST_APP=ON")
endif()

if(FOSD_WIDGETS_ASSISTANT_BUILD_TESTS AND NOT FOSD_BUILD_WIDGETS_ASSISTANT)
    message(FATAL_ERROR "Widgets assistant tests require FOSD_BUILD_WIDGETS_ASSISTANT=ON")
endif()

if(FOSD_ASSISTANT_QML_BUILD_TESTS AND NOT FOSD_BUILD_COMMUNICATION_ASSISTANT_QML)
    message(FATAL_ERROR "Assistant tests require FOSD_BUILD_COMMUNICATION_ASSISTANT_QML=ON")
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
find_package(Qt5 5.12 REQUIRED COMPONENTS Core)
if(FOSD_BUILD_MAIN_APP OR FOSD_BUILD_TESTS OR FOSD_BUILD_VTK_NATIVE_TEST_APP OR FOSD_BUILD_COMMUNICATION_ASSISTANT_QML OR FOSD_BUILD_WIDGETS_ASSISTANT)
    find_package(Qt5 5.12 REQUIRED COMPONENTS SerialPort Network)
endif()

if(FOSD_BUILD_MAIN_APP OR FOSD_BUILD_WORKBENCH_APP OR FOSD_BUILD_WIDGETS_ASSISTANT)
    find_package(Qt5 5.12 REQUIRED COMPONENTS Widgets)
endif()
if(FOSD_BUILD_MAIN_APP OR FOSD_BUILD_TESTS OR FOSD_BUILD_VTK_NATIVE_TEST_APP)
    find_package(Qt5 5.12 REQUIRED COMPONENTS Sql)
endif()
if(FOSD_BUILD_COMMUNICATION_ASSISTANT_QML)
    find_package(Qt5 5.12 REQUIRED COMPONENTS Qml Quick QuickControls2)
endif()
if(FOSD_BUILD_TESTS OR FOSD_ASSISTANT_QML_BUILD_TESTS OR FOSD_WIDGETS_ASSISTANT_BUILD_TESTS
        OR FOSD_WORKBENCH_BUILD_TESTS OR FOSD_VTK_NATIVE_BUILD_TESTS)
    find_package(Qt5 5.12 REQUIRED COMPONENTS Test)
    enable_testing()
endif()

# ---------------------------------------------------------------------------
# Targets
# ---------------------------------------------------------------------------
if(FOSD_BUILD_MAIN_APP OR FOSD_BUILD_TESTS OR FOSD_BUILD_VTK_NATIVE_TEST_APP
        OR FOSD_BUILD_COMMUNICATION_ASSISTANT_QML OR FOSD_BUILD_WIDGETS_ASSISTANT)
    add_subdirectory(src)
endif()
if(FOSD_BUILD_MAIN_APP OR FOSD_BUILD_TESTS OR FOSD_BUILD_WORKBENCH_APP)
    add_subdirectory(ui/FOSD-Classic-QWidgets)
endif()
if(FOSD_BUILD_WORKBENCH_APP)
    add_subdirectory(apps/FOSD-Classic-QWidgets)
endif()
if(FOSD_BUILD_MAIN_APP)
    add_subdirectory(apps/FOSD-Win)
endif()
if(FOSD_BUILD_VTK_NATIVE_TEST_APP)
    add_subdirectory(ui/Test-VtkOpenGLNativeWidgets)
    add_subdirectory(apps/Test-VtkOpenGLNativeWidgets)
endif()
if(FOSD_BUILD_COMMUNICATION_ASSISTANT_QML OR FOSD_BUILD_WIDGETS_ASSISTANT)
    add_subdirectory(ui/communication-assistant-QML)
endif()
if(FOSD_BUILD_COMMUNICATION_ASSISTANT_QML)
    add_subdirectory(apps/communication-assistant-QML)
endif()
if(FOSD_BUILD_WIDGETS_ASSISTANT)
    add_subdirectory(ui/CommunicationAssistantWidgets)
    add_subdirectory(apps/CommunicationAssistantWidgets)
endif()

if(FOSD_BUILD_TESTS)
    add_subdirectory(test)
endif()
