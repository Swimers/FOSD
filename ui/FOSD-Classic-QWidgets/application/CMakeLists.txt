add_library(fosd_classic_application STATIC
    ClassicWorkbench.h ClassicWorkbench.cpp
    ../features/visualization/views/WorkbenchRenderSurface.ui
)
target_link_libraries(fosd_classic_application PUBLIC fosd_classic_widgets
    fosd_classic_presenters fosd_classic_model fosd_visualization_qt)
set_target_properties(fosd_classic_application PROPERTIES
    AUTOUIC_SEARCH_PATHS "${CMAKE_CURRENT_SOURCE_DIR}/../features/visualization/views")
