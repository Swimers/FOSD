cmake_minimum_required(VERSION 3.16)

project(FOSD-Pro VERSION 1.0 LANGUAGES C CXX)

include(CTest)
if(BUILD_TESTING)
    add_subdirectory(tests)
endif()

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

include(GNUInstallDirs)

set(FOSD_CMAKE_OUTPUT_ROOT "${CMAKE_BINARY_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${FOSD_CMAKE_OUTPUT_ROOT}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${FOSD_CMAKE_OUTPUT_ROOT}/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${FOSD_CMAKE_OUTPUT_ROOT}/lib")
set(CMAKE_PDB_OUTPUT_DIRECTORY "${FOSD_CMAKE_OUTPUT_ROOT}/pdb")

foreach(config DEBUG RELEASE RELWITHDEBINFO MINSIZEREL)
    string(TOLOWER "${config}" config_lower)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${config} "${FOSD_CMAKE_OUTPUT_ROOT}/bin/${config_lower}")
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${config} "${FOSD_CMAKE_OUTPUT_ROOT}/lib/${config_lower}")
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${config} "${FOSD_CMAKE_OUTPUT_ROOT}/lib/${config_lower}")
    set(CMAKE_PDB_OUTPUT_DIRECTORY_${config} "${FOSD_CMAKE_OUTPUT_ROOT}/pdb/${config_lower}")
endforeach()

if(EXISTS "D:/Qt/5.12.2/msvc2017_64/lib/cmake/Qt5/Qt5Config.cmake")
    list(PREPEND CMAKE_PREFIX_PATH "D:/Qt/5.12.2/msvc2017_64")
endif()

find_package(Qt5 5.12 REQUIRED COMPONENTS
    Core
    Gui
    Network
    OpenGL
    PrintSupport
    SerialPort
    Widgets
)

set(FOSD_VTK_RELEASE_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/lib/VTK/VTK-release")
set(FOSD_VTK_DEBUG_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/lib/VTK/VTK-debug")

file(GLOB FOSD_VTK_RELEASE_LIBS "${FOSD_VTK_RELEASE_ROOT}/lib/*-8.2.lib")

set(FOSD_VTK_LIBRARIES)
foreach(release_lib IN LISTS FOSD_VTK_RELEASE_LIBS)
    get_filename_component(release_name "${release_lib}" NAME)
    string(REPLACE "-8.2.lib" "-8.2D.lib" debug_name "${release_name}")
    set(debug_lib "${FOSD_VTK_DEBUG_ROOT}/lib/${debug_name}")
    if(EXISTS "${debug_lib}")
        list(APPEND FOSD_VTK_LIBRARIES
            optimized "${release_lib}"
            debug "${debug_lib}"
        )
    else()
        list(APPEND FOSD_VTK_LIBRARIES "${release_lib}")
    endif()
endforeach()

qt5_add_resources(FOSD_RESOURCES res/resource.qrc)

# Shared compile requirements for each project target.
add_library(fosd_build_options INTERFACE)
target_compile_features(fosd_build_options INTERFACE cxx_std_11)
target_compile_definitions(fosd_build_options INTERFACE QT_DEPRECATED_WARNINGS)
if(MSVC)
    target_compile_options(fosd_build_options INTERFACE /utf-8)
else()
    target_compile_options(fosd_build_options INTERFACE -Wall)
endif()

add_executable(FOSD-Pro WIN32
    src/main.cpp
    src/fosdplugin.cpp
    ${FOSD_RESOURCES}
)

add_subdirectory(lib/CustomUi)
add_subdirectory(src/app)
add_subdirectory(src/Communication)
add_subdirectory(src/core)
add_subdirectory(src/protocol)
add_subdirectory(src/test)
add_subdirectory(src/utils)

target_include_directories(FOSD-Pro PRIVATE
    src
    src/app/conncect
    src/app/disp2d
    src/app/disp3d
    src/app/disp3d/docks
    src/app/mainwindow
    src/app/user
    src/Communication
    src/core/IFE
    src/core/RainbowIFE
    src/core/model
    src/main
    src/protocol
    src/test
    "${FOSD_VTK_RELEASE_ROOT}/include/vtk-8.2"
    "${FOSD_VTK_RELEASE_ROOT}/include/vtk-8.2/vtkhdf5"
)

target_link_libraries(FOSD-Pro PRIVATE
    fosd_build_options
    fosd_custom_ui
    fosd_plot
    fosd_storage
    fosd_config
    fosd_database
    fosd_global
    fosd_timers
    Qt5::Core
    Qt5::Gui
    Qt5::Network
    Qt5::OpenGL
    Qt5::PrintSupport
    Qt5::SerialPort
    Qt5::Widgets
    ${FOSD_VTK_LIBRARIES}
)

add_custom_command(TARGET FOSD-Pro POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_CURRENT_SOURCE_DIR}/data"
            "$<TARGET_FILE_DIR:FOSD-Pro>/data"
)

install(TARGETS FOSD-Pro
    BUNDLE DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
