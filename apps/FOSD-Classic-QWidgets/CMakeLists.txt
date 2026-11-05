# The complete application composition belongs to the Classic variant only.
add_library(fosd_classic_runtime STATIC
    backend/WorkbenchRuntime.h backend/WorkbenchRuntime.cpp
    backend/WorkbenchConnection.cpp backend/WorkbenchVisualization.cpp backend/WorkbenchData.cpp
    )
target_include_directories(fosd_classic_runtime PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_link_libraries(fosd_classic_runtime PUBLIC fosd_classic_contracts
    fosd_protocol fosd_calculation fosd_visualization)
target_compile_features(fosd_classic_runtime PUBLIC cxx_std_11)

add_executable(FOSD-Classic-QWidgets WIN32 main.cpp ClassicWorkbench.h ClassicWorkbench.cpp
    WorkbenchRenderSurface.ui)
target_link_libraries(FOSD-Classic-QWidgets PRIVATE fosd_classic_widgets
    fosd_classic_runtime fosd_visualization_qt)
if(WIN32)
    fosd_deploy_vtk_runtime(FOSD-Classic-QWidgets)
    set_target_properties(FOSD-Classic-QWidgets PROPERTIES
        VS_DEBUGGER_ENVIRONMENT
            "PATH=${FOSD_VISUALIZATION_VTK_RUNTIME_DIR};${FOSD_QT_ROOT}/bin;%PATH%")
endif()

if(FOSD_BUILD_TESTS OR FOSD_WORKBENCH_BUILD_TESTS)
    add_executable(test_workbench_runtime tests/workbench_runtime_tests.cpp)
    target_link_libraries(test_workbench_runtime PRIVATE fosd_classic_runtime fosd_classic_backends Qt5::Test)
    add_test(NAME test_workbench_runtime COMMAND test_workbench_runtime)

    add_executable(test_workbench_main tests/workbench_main_tests.cpp
        ClassicWorkbench.h ClassicWorkbench.cpp WorkbenchRenderSurface.ui)
    target_link_libraries(test_workbench_main PRIVATE fosd_classic_runtime
        fosd_classic_widgets fosd_visualization_qt Qt5::Test)
    add_test(NAME test_workbench_main COMMAND test_workbench_main)
endif()
