add_library(fosd_config STATIC
    IConfigStore.h IniConfigStore.h IniConfigStore.cpp
)
target_include_directories(fosd_config PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>)
target_link_libraries(fosd_config PUBLIC Qt5::Core)
target_compile_features(fosd_config PUBLIC cxx_std_11)
