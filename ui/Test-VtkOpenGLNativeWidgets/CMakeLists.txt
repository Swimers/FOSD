find_package(Qt5 5.12 REQUIRED COMPONENTS Widgets OpenGL)

add_library(fosd_vtk_validation_presenter STATIC
    IVtkValidationView.h IVtkValidationBackend.h VtkValidationPresenter.h VtkValidationPresenter.cpp)
target_include_directories(fosd_vtk_validation_presenter PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/..")
target_link_libraries(fosd_vtk_validation_presenter PUBLIC Qt5::Core)

add_library(fosd_vtk_validation_qt INTERFACE)
target_link_libraries(fosd_vtk_validation_qt INTERFACE fosd_visualization_vtk Qt5::Widgets Qt5::OpenGL)
include(SelectLibraryConfigurations)
find_library(FOSD_vtkGUISupportQt_LIBRARY_RELEASE NAMES vtkGUISupportQt-8.2
    PATHS "${FOSD_VISUALIZATION_VTK_ROOT}/lib" NO_DEFAULT_PATH)
find_library(FOSD_vtkGUISupportQt_LIBRARY_DEBUG NAMES vtkGUISupportQt-8.2d vtkGUISupportQt-8.2D
    PATHS "${FOSD_VISUALIZATION_VTK_ROOT}/lib" NO_DEFAULT_PATH)
select_library_configurations(FOSD_vtkGUISupportQt)
if(NOT FOSD_vtkGUISupportQt_LIBRARY)
    message(FATAL_ERROR "QVTKOpenGLNativeWidget requires vtkGUISupportQt in FOSD_VISUALIZATION_VTK_ROOT/lib")
endif()
if(WIN32 AND (NOT FOSD_vtkGUISupportQt_LIBRARY_DEBUG OR NOT FOSD_vtkGUISupportQt_LIBRARY_RELEASE))
    message(FATAL_ERROR "vtkGUISupportQt requires matching Debug and Release import libraries")
endif()
target_link_libraries(fosd_vtk_validation_qt INTERFACE ${FOSD_vtkGUISupportQt_LIBRARY})

add_library(fosd_vtk_validation_widgets STATIC VtkValidationWindow.h VtkValidationWindow.cpp VtkValidationWindow.ui)
target_link_libraries(fosd_vtk_validation_widgets PUBLIC fosd_vtk_validation_presenter fosd_vtk_validation_qt)
target_compile_features(fosd_vtk_validation_widgets PUBLIC cxx_std_11)
if(FOSD_BUILD_TESTS OR FOSD_VTK_NATIVE_BUILD_TESTS)
    add_executable(test_vtk_validation_presenter tests/PresenterTests.cpp)
    target_link_libraries(test_vtk_validation_presenter PRIVATE fosd_vtk_validation_presenter Qt5::Test)
    add_test(NAME test_vtk_validation_presenter COMMAND test_vtk_validation_presenter)
endif()
