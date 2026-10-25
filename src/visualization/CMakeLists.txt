add_library(fosd_visualization STATIC
    VisualizationControl.h
    VisualizationControl.cpp
    VisualizationTypes.h
    mesh/MeshManager.h 
    mesh/MeshManager.cpp
    render/DeformationRenderer.h 
    render/DeformationRenderer.cpp
    render/ScalarColorMapper.h 
    render/ScalarColorMapper.cpp
    marker/KeyPointManager.h 
    marker/KeyPointManager.cpp
    interaction/ModelInteractorStyle.h 
    interaction/ModelInteractorStyle.cpp
    interaction/PointPicker.h 
    interaction/PointPicker.cpp
    scene/SceneEnvironment.h 
    scene/SceneEnvironment.cpp
)

# Public includes use visualization/..., with src/ as their root.
target_include_directories(fosd_visualization
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>
)

# The bundled VTK is a trimmed SDK: its VTKConfig/VTKTargets still reference
# removed modules and bin/ files. Resolve the required libraries individually
# instead of importing that incomplete package or modifying vendor files.
get_filename_component(_vtk_include_parent "${FOSD_VTK_TYPES_INCLUDE_DIR}" DIRECTORY)
get_filename_component(_vtk_sdk_default "${_vtk_include_parent}" DIRECTORY)
set(FOSD_VISUALIZATION_VTK_ROOT "${_vtk_sdk_default}" CACHE PATH "VTK 8.2 SDK for visualization (matching core's VTK headers)")
find_package(Qt5 5.12 REQUIRED COMPONENTS Widgets)

add_library(fosd_visualization_vtk INTERFACE)
target_include_directories(fosd_visualization_vtk SYSTEM INTERFACE "${FOSD_VTK_TYPES_INCLUDE_DIR}")
include(SelectLibraryConfigurations)
foreach(_module IN ITEMS
        vtkCommonCore vtkCommonDataModel vtkCommonExecutionModel
        vtkFiltersCore vtkFiltersSources vtkIOGeometry
        vtkRenderingCore vtkRenderingOpenGL2 vtkRenderingAnnotation
        vtkRenderingFreeType vtkInteractionStyle)
    find_library(FOSD_${_module}_LIBRARY_RELEASE NAMES ${_module}-8.2 ${_module}
        PATHS "${FOSD_VISUALIZATION_VTK_ROOT}/lib" NO_DEFAULT_PATH)
    find_library(FOSD_${_module}_LIBRARY_DEBUG NAMES ${_module}-8.2d ${_module}-8.2D ${_module}d
        PATHS "${FOSD_VISUALIZATION_VTK_ROOT}/lib" NO_DEFAULT_PATH)
    select_library_configurations(FOSD_${_module})
    if(NOT FOSD_${_module}_LIBRARY)
        message(FATAL_ERROR "Missing ${_module}: set FOSD_VISUALIZATION_VTK_ROOT to a matching VTK 8.2 SDK")
    endif()
    if(WIN32 AND (NOT FOSD_${_module}_LIBRARY_DEBUG OR NOT FOSD_${_module}_LIBRARY_RELEASE))
        message(FATAL_ERROR "${_module} requires both Debug and Release libraries in FOSD_VISUALIZATION_VTK_ROOT/lib")
    endif()
    target_link_libraries(fosd_visualization_vtk INTERFACE ${FOSD_${_module}_LIBRARY})
endforeach()

target_link_libraries(fosd_visualization PUBLIC fosd_core Qt5::Core Qt5::Widgets fosd_visualization_vtk)

target_compile_features(fosd_visualization PUBLIC cxx_std_11)
