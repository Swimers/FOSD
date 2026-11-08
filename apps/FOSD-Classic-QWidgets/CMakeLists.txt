# Executable entry only; UI composition is provided by ui/.
add_executable(FOSD-Classic-QWidgets WIN32 main.cpp)
target_link_libraries(FOSD-Classic-QWidgets PRIVATE fosd_classic_application)
if(WIN32)
    fosd_deploy_vtk_runtime(FOSD-Classic-QWidgets)
    set_target_properties(FOSD-Classic-QWidgets PROPERTIES
        VS_DEBUGGER_ENVIRONMENT
            "PATH=${FOSD_VISUALIZATION_VTK_RUNTIME_DIR};${FOSD_QT_ROOT}/bin;%PATH%")
endif()
