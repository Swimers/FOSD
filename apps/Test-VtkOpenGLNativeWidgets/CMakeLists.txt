add_executable(Test-VtkOpenGLNativeWidgets WIN32 main.cpp)
target_link_libraries(Test-VtkOpenGLNativeWidgets
    PRIVATE
        fosd_vtk_validation_widgets
        fosd_vtk_validation_backend
        fosd_vtk_validation_smoke)
target_compile_features(Test-VtkOpenGLNativeWidgets PRIVATE cxx_std_11)
set(FOSD_VTK_NATIVE_RUNTIME_DIR "" CACHE PATH "Directory containing matching VTK runtime DLLs for the native validation app")
if(WIN32)
    set_target_properties(Test-VtkOpenGLNativeWidgets PROPERTIES
        VS_DEBUGGER_ENVIRONMENT "PATH=${FOSD_VTK_NATIVE_RUNTIME_DIR};${FOSD_QT_ROOT}/bin;%PATH%")
endif()
