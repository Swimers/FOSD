add_executable(FOSDWidgets WIN32 main.cpp)
target_link_libraries(FOSDWidgets PRIVATE fosd_workbench_widgets)
if(WIN32)
    set_target_properties(FOSDWidgets PROPERTIES
        VS_DEBUGGER_ENVIRONMENT "PATH=${FOSD_QT_ROOT}/bin;%PATH%")
endif()
