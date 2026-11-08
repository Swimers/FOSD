# Complete Classic variant: service adapter and Qt/VTK host composition.
add_library(fosd_classic_runtime STATIC
    runtime/WorkbenchRuntime.h runtime/WorkbenchRuntime.cpp
    runtime/WorkbenchConnection.cpp runtime/WorkbenchVisualization.cpp
    runtime/WorkbenchData.cpp runtime/WorkbenchMarkers.cpp
)
target_link_libraries(fosd_classic_runtime PUBLIC fosd_classic_contracts
    fosd_protocol fosd_calculation fosd_visualization)
target_compile_features(fosd_classic_runtime PUBLIC cxx_std_11)

add_library(fosd_classic_application STATIC
    ClassicWorkbench.h ClassicWorkbench.cpp
    ../features/visualization/views/WorkbenchRenderSurface.ui
)
target_link_libraries(fosd_classic_application PUBLIC fosd_classic_widgets
    fosd_classic_runtime fosd_visualization_qt)
set_target_properties(fosd_classic_application PROPERTIES
    AUTOUIC_SEARCH_PATHS "${CMAKE_CURRENT_SOURCE_DIR}/../features/visualization/views")
