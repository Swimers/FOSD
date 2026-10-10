add_library(fosd_custom_ui STATIC
    switchcontrol.cpp
    switchcontrol.h
)
target_include_directories(fosd_custom_ui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(fosd_custom_ui
    PUBLIC Qt5::Widgets
    PRIVATE fosd_build_options
)
