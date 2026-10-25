add_executable(Test-VtkOpenGLNativeWidgets WIN32 main.cpp)
target_link_libraries(Test-VtkOpenGLNativeWidgets
    PRIVATE
        fosd_vtk_validation_widgets
        fosd_vtk_validation_backend
        fosd_vtk_validation_smoke)
target_compile_features(Test-VtkOpenGLNativeWidgets PRIVATE cxx_std_11)
add_custom_target(fosd_plate_model_assets
    COMMAND ${CMAKE_COMMAND} -E make_directory
        "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIG>/tools/plate-simulation/models"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${PROJECT_SOURCE_DIR}/tools/plate-simulation/models/plate.stl"
        "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIG>/tools/plate-simulation/models/plate.stl"
    VERBATIM)
add_dependencies(Test-VtkOpenGLNativeWidgets fosd_plate_model_assets)
set(FOSD_VTK_NATIVE_RUNTIME_DIR "" CACHE PATH "Directory containing matching VTK runtime DLLs for the native validation app")
if(WIN32)
    set_target_properties(Test-VtkOpenGLNativeWidgets PROPERTIES
        VS_DEBUGGER_ENVIRONMENT "PATH=${FOSD_VTK_NATIVE_RUNTIME_DIR};${FOSD_QT_ROOT}/bin;%PATH%")
endif()
