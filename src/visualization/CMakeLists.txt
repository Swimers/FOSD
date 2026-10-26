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
    marker/KeyPointCreator.h
    marker/KeyPointCreator.cpp
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
if(WIN32)
    set(FOSD_VISUALIZATION_VTK_RUNTIME_DIR
        "${FOSD_VISUALIZATION_VTK_ROOT}/bin"
        CACHE PATH "Directory containing the matching VTK runtime DLLs")

    function(fosd_deploy_vtk_runtime target)
        if(NOT IS_DIRECTORY "${FOSD_VISUALIZATION_VTK_RUNTIME_DIR}")
            message(FATAL_ERROR
                "VTK runtime directory does not exist: ${FOSD_VISUALIZATION_VTK_RUNTIME_DIR}")
        endif()
        file(GLOB _fosd_vtk_runtime_dlls
            "${FOSD_VISUALIZATION_VTK_RUNTIME_DIR}/*.dll")
        if(NOT _fosd_vtk_runtime_dlls)
            message(FATAL_ERROR
                "No VTK runtime DLLs found in: ${FOSD_VISUALIZATION_VTK_RUNTIME_DIR}")
        endif()
        add_custom_command(TARGET ${target} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
                "${FOSD_VISUALIZATION_VTK_RUNTIME_DIR}"
                "$<TARGET_FILE_DIR:${target}>"
            COMMENT "Deploying VTK runtime DLLs for ${target}"
            VERBATIM)
    endfunction()
endif()
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

if(FOSD_BUILD_WORKBENCH_APP OR FOSD_BUILD_VTK_NATIVE_TEST_APP)
    find_package(Qt5 5.12 REQUIRED COMPONENTS OpenGL)
    add_library(fosd_visualization_qt INTERFACE)
    find_library(FOSD_vtkGUISupportQt_LIBRARY_RELEASE NAMES vtkGUISupportQt-8.2
        PATHS "${FOSD_VISUALIZATION_VTK_ROOT}/lib" NO_DEFAULT_PATH)
    find_library(FOSD_vtkGUISupportQt_LIBRARY_DEBUG NAMES vtkGUISupportQt-8.2d vtkGUISupportQt-8.2D
        PATHS "${FOSD_VISUALIZATION_VTK_ROOT}/lib" NO_DEFAULT_PATH)
    select_library_configurations(FOSD_vtkGUISupportQt)
    if(NOT FOSD_vtkGUISupportQt_LIBRARY_RELEASE OR NOT FOSD_vtkGUISupportQt_LIBRARY_DEBUG)
        message(FATAL_ERROR "QVTKOpenGLNativeWidget requires matching Debug/Release vtkGUISupportQt libraries")
    endif()
    target_link_libraries(fosd_visualization_qt INTERFACE fosd_visualization_vtk
        Qt5::Widgets Qt5::OpenGL ${FOSD_vtkGUISupportQt_LIBRARY})
endif()
