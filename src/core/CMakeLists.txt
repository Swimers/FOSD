add_library(fosd_core STATIC
    Datamodel/FrameData.h
    Datamodel/FrameRepository.h
    Datamodel/FrameRepository.cpp
)
target_include_directories(fosd_core PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>)
target_link_libraries(fosd_core PUBLIC Qt5::Core)
