cmake_minimum_required(VERSION 3.16)

project(FOSD-Pro VERSION 1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

include(GNUInstallDirs)

set(FOSD_CMAKE_OUTPUT_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/build/CMake")
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
file(GLOB FOSD_VTK_DEBUG_LIBS "${FOSD_VTK_DEBUG_ROOT}/lib/*-8.2D.lib")

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

set(FOSD_SOURCES
    lib/CustomUi/switchcontrol.cpp
    lib/CustomUi/switchcontrol.h
    src/app/conncect/connectdebug.cpp
    src/app/conncect/connectdebug.h
    src/app/conncect/connectdebug.ui
    src/app/conncect/connectwindow.cpp
    src/app/conncect/connectwindow.h
    src/app/conncect/connectwindow.ui
    src/app/disp2d/disp2dpara.cpp
    src/app/disp2d/disp2dpara.h
    src/app/disp2d/disp2dpara.ui
    src/app/disp2d/disp2dwindow.cpp
    src/app/disp2d/disp2dwindow.h
    src/app/disp2d/disp2dwindow.ui
    src/app/disp3d/disp3dkeypointedit.cpp
    src/app/disp3d/disp3dkeypointedit.h
    src/app/disp3d/disp3dkeypointedit.ui
    src/app/disp3d/disp3dkeypointselect.cpp
    src/app/disp3d/disp3dkeypointselect.h
    src/app/disp3d/disp3dkeypointselect.ui
    src/app/disp3d/docks/disp3ddockmanager.cpp
    src/app/disp3d/docks/disp3ddockmanager.h
    src/app/disp3d/docks/disp3ddockpanel.h
    src/app/disp3d/docks/disp3dinfodockpanel.cpp
    src/app/disp3d/docks/disp3dinfodockpanel.h
    src/app/disp3d/docks/disp3dinfodockpanel.ui
    src/app/disp3d/docks/disp3dmodeldockpanel.cpp
    src/app/disp3d/docks/disp3dmodeldockpanel.h
    src/app/disp3d/docks/disp3dmodeldockpanel.ui
    src/app/disp3d/docks/disp3dviewdockpanel.cpp
    src/app/disp3d/docks/disp3dviewdockpanel.h
    src/app/disp3d/docks/disp3dviewdockpanel.ui
    src/app/disp3d/disp3dwindow.cpp
    src/app/disp3d/disp3dwindow.h
    src/app/disp3d/disp3dwindow.ui
    src/app/disp3d/dispdata.cpp
    src/app/disp3d/dispdata.h
    src/app/disp3d/dispdata.ui
    src/app/disp3d/trackingpointdatatable.cpp
    src/app/disp3d/trackingpointdatatable.h
    src/app/disp3d/trackingpointdatatable.ui
    src/app/disp3d/trackingpointmanager.cpp
    src/app/disp3d/trackingpointmanager.h
    src/app/disp3d/trackingpointmanager.ui
    src/app/mainwindow/oldmainwindow.cpp
    src/app/mainwindow/oldmainwindow.h
    src/app/mainwindow/oldmainwindow.ui
    src/app/mainwindow/rainbowmainwindow.cpp
    src/app/mainwindow/rainbowmainwindow.h
    src/app/mainwindow/rainbowmainwindow.ui
    src/app/user/login.cpp
    src/app/user/login.h
    src/app/user/login.ui
    src/Communication/serialtask.cpp
    src/Communication/serialtask.h
    src/Communication/tcptask.cpp
    src/Communication/tcptask.h
    src/Communication/udptask.cpp
    src/Communication/udptask.h
    src/core/IFE/IFE.cpp
    src/core/IFE/IFE.h
    src/core/RainbowIFE/rainbowife.cpp
    src/core/RainbowIFE/rainbowife.h
    src/core/RainbowIFE/rainbowloadcalibration.cpp
    src/core/RainbowIFE/rainbowloadcalibration.h
    src/core/model/disp3dbackground.cpp
    src/core/model/disp3dbackground.h
    src/core/model/disp3dfreshmodelthread.cpp
    src/core/model/disp3dfreshmodelthread.h
    src/core/model/disp3dglobal.h
    src/core/model/disp3dinteractor.cpp
    src/core/model/disp3dinteractor.h
    src/core/model/disp3dmodel.cpp
    src/core/model/disp3dmodel.h
    src/core/model/trackingpoint.cpp
    src/core/model/trackingpoint.h
    src/fosdplugin.cpp
    src/main.cpp
    src/protocol/analyzepackets.cpp
    src/protocol/analyzepackets.h
    src/protocol/pocketsconfig.h
    src/test/testdatainjector.cpp
    src/test/testdatainjector.h
    src/test/testsavepointsdisp.cpp
    src/test/testsavepointsdisp.h
    src/test/teststorge.cpp
    src/test/teststorge.h
    src/utils/QCustomPlot/qcpdocumentobject.cpp
    src/utils/QCustomPlot/qcpdocumentobject.h
    src/utils/QCustomPlot/qcustomplot.cpp
    src/utils/QCustomPlot/qcustomplot.h
    src/utils/Storage/DataStorageThread.cpp
    src/utils/Storage/DataStorageThread.h
    src/utils/Timers/Timer.h
    src/utils/database/database.cpp
    src/utils/database/database.h
    src/utils/global/global.cpp
    src/utils/global/global.h
)

qt5_add_resources(FOSD_RESOURCES res/resource.qrc)

add_executable(FOSD-Pro WIN32
    ${FOSD_SOURCES}
    ${FOSD_RESOURCES}
)

set_target_properties(FOSD-Pro PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${FOSD_CMAKE_OUTPUT_ROOT}/bin"
    LIBRARY_OUTPUT_DIRECTORY "${FOSD_CMAKE_OUTPUT_ROOT}/lib"
    ARCHIVE_OUTPUT_DIRECTORY "${FOSD_CMAKE_OUTPUT_ROOT}/lib"
    PDB_OUTPUT_DIRECTORY "${FOSD_CMAKE_OUTPUT_ROOT}/pdb"
    RUNTIME_OUTPUT_DIRECTORY_DEBUG "${FOSD_CMAKE_OUTPUT_ROOT}/bin/debug"
    LIBRARY_OUTPUT_DIRECTORY_DEBUG "${FOSD_CMAKE_OUTPUT_ROOT}/lib/debug"
    ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${FOSD_CMAKE_OUTPUT_ROOT}/lib/debug"
    PDB_OUTPUT_DIRECTORY_DEBUG "${FOSD_CMAKE_OUTPUT_ROOT}/pdb/debug"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${FOSD_CMAKE_OUTPUT_ROOT}/bin/release"
    LIBRARY_OUTPUT_DIRECTORY_RELEASE "${FOSD_CMAKE_OUTPUT_ROOT}/lib/release"
    ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${FOSD_CMAKE_OUTPUT_ROOT}/lib/release"
    PDB_OUTPUT_DIRECTORY_RELEASE "${FOSD_CMAKE_OUTPUT_ROOT}/pdb/release"
    RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${FOSD_CMAKE_OUTPUT_ROOT}/bin/relwithdebinfo"
    LIBRARY_OUTPUT_DIRECTORY_RELWITHDEBINFO "${FOSD_CMAKE_OUTPUT_ROOT}/lib/relwithdebinfo"
    ARCHIVE_OUTPUT_DIRECTORY_RELWITHDEBINFO "${FOSD_CMAKE_OUTPUT_ROOT}/lib/relwithdebinfo"
    PDB_OUTPUT_DIRECTORY_RELWITHDEBINFO "${FOSD_CMAKE_OUTPUT_ROOT}/pdb/relwithdebinfo"
    RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL "${FOSD_CMAKE_OUTPUT_ROOT}/bin/minsizerel"
    LIBRARY_OUTPUT_DIRECTORY_MINSIZEREL "${FOSD_CMAKE_OUTPUT_ROOT}/lib/minsizerel"
    ARCHIVE_OUTPUT_DIRECTORY_MINSIZEREL "${FOSD_CMAKE_OUTPUT_ROOT}/lib/minsizerel"
    PDB_OUTPUT_DIRECTORY_MINSIZEREL "${FOSD_CMAKE_OUTPUT_ROOT}/pdb/minsizerel"
)

target_include_directories(FOSD-Pro PRIVATE
    src
    lib/CustomUi
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
    src/utils/QCustomPlot
    src/utils/Storage
    src/utils/Timers
    src/utils/database
    src/utils/global
    "${FOSD_VTK_RELEASE_ROOT}/include/vtk-8.2"
    "${FOSD_VTK_RELEASE_ROOT}/include/vtk-8.2/vtkhdf5"
)

target_compile_definitions(FOSD-Pro PRIVATE
    QT_DEPRECATED_WARNINGS
)

if(MSVC)
    target_compile_options(FOSD-Pro PRIVATE /utf-8)
else()
    target_compile_options(FOSD-Pro PRIVATE -Wall)
endif()

target_link_libraries(FOSD-Pro PRIVATE
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
