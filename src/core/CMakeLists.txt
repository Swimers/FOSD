add_library(fosd_core STATIC
    Datamodel/FrameData.h
    Datamodel/CalibratedData.h
    Datamodel/FrameRepository.h
    Datamodel/FrameRepository.cpp
    Datamodel/ModelData.h
    Datamodel/CalculationRepository.h
    Datamodel/CalculationRepository.cpp
)

# Only VTK's configured type headers are needed; no rendering libraries or DLLs.
# Override FOSD_VTK_TYPES_INCLUDE_DIR when using a different VTK installation.
if(WIN32)
    set(_fosd_vtk_types_default "${PROJECT_SOURCE_DIR}/lib/VTK/VTK-Win-release/include/vtk-8.2")
else()
    set(_fosd_vtk_types_default "${PROJECT_SOURCE_DIR}/lib/VTK/VTK-RK3588-Linux-debug/include/vtk-8.2")
endif()
find_path(FOSD_VTK_TYPES_INCLUDE_DIR vtkType.h HINTS "${_fosd_vtk_types_default}")
if(NOT FOSD_VTK_TYPES_INCLUDE_DIR)
    message(FATAL_ERROR "Set FOSD_VTK_TYPES_INCLUDE_DIR to the directory containing vtkType.h")
endif()
target_include_directories(fosd_core SYSTEM PUBLIC "${FOSD_VTK_TYPES_INCLUDE_DIR}")
target_include_directories(fosd_core PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>)
target_link_libraries(fosd_core PUBLIC Qt5::Core)
