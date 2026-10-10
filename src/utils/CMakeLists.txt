# Each utility owns its sources, include path, and direct Qt dependency.
add_library(fosd_plot STATIC
    QCustomPlot/qcpdocumentobject.cpp
    QCustomPlot/qcpdocumentobject.h
    QCustomPlot/qcustomplot.cpp
    QCustomPlot/qcustomplot.h
)
target_include_directories(fosd_plot PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/QCustomPlot)
target_link_libraries(fosd_plot
    PUBLIC Qt5::Widgets Qt5::PrintSupport
    PRIVATE fosd_build_options
)

add_library(fosd_storage STATIC
    Storage/DataStorageThread.cpp
    Storage/DataStorageThread.h
)
target_include_directories(fosd_storage PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Storage)
target_link_libraries(fosd_storage
    PUBLIC Qt5::Core
    PRIVATE fosd_build_options
)

add_library(fosd_config STATIC
    config/AppConfig.h
    config/ConfigManager.cpp
    config/ConfigManager.h
)
target_include_directories(fosd_config PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/config)
target_link_libraries(fosd_config
    PUBLIC Qt5::Core
    PRIVATE fosd_build_options
)

add_library(fosd_database STATIC
    database/database.cpp
    database/database.h
)
target_include_directories(fosd_database PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/database)
target_link_libraries(fosd_database PRIVATE fosd_build_options)

add_library(fosd_global STATIC
    global/global.cpp
    global/global.h
)
target_include_directories(fosd_global PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/global)
target_link_libraries(fosd_global
    PUBLIC Qt5::Core
    PRIVATE fosd_build_options
)

# Timers has only a header, so an INTERFACE target carries its include path.
add_library(fosd_timers INTERFACE)
target_sources(fosd_timers INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/Timers/Timer.h)
target_include_directories(fosd_timers INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/Timers)
target_link_libraries(fosd_timers INTERFACE Qt5::Core fosd_build_options)
