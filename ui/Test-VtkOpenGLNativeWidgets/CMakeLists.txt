find_package(Qt5 5.12 REQUIRED COMPONENTS Widgets OpenGL)

add_library(fosd_vtk_validation_presenter STATIC
    views/IVtkValidationView.h
    backend/IVtkValidationBackend.h
    presenters/VtkValidationPresenter.h
    presenters/VtkValidationPresenter.cpp)
target_include_directories(fosd_vtk_validation_presenter PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/..")
target_link_libraries(fosd_vtk_validation_presenter PUBLIC Qt5::Core)

add_library(fosd_vtk_validation_qt INTERFACE)
target_link_libraries(fosd_vtk_validation_qt INTERFACE fosd_visualization_qt)

add_library(fosd_vtk_validation_widgets STATIC
    views/VtkValidationWindow.h
    views/VtkValidationWindow.cpp
    views/VtkValidationWindow.ui)
target_link_libraries(fosd_vtk_validation_widgets PUBLIC fosd_vtk_validation_presenter fosd_vtk_validation_qt)
target_compile_features(fosd_vtk_validation_widgets PUBLIC cxx_std_11)

add_library(fosd_vtk_validation_backend STATIC
    backend/VtkValidationBackend.h
    backend/VtkValidationBackend.cpp)
target_link_libraries(fosd_vtk_validation_backend
    PUBLIC
        fosd_vtk_validation_presenter
        fosd_communication
        fosd_protocol
        fosd_calculation
        fosd_storage
        fosd_visualization)
target_compile_features(fosd_vtk_validation_backend PUBLIC cxx_std_11)

add_library(fosd_vtk_validation_smoke STATIC
    tests/SmokeCheck.h
    tests/SmokeCheck.cpp)
target_link_libraries(fosd_vtk_validation_smoke
    PUBLIC
        fosd_vtk_validation_widgets
        fosd_vtk_validation_backend)
target_compile_features(fosd_vtk_validation_smoke PUBLIC cxx_std_11)

if(FOSD_BUILD_TESTS OR FOSD_VTK_NATIVE_BUILD_TESTS)
    add_executable(test_vtk_validation_presenter tests/PresenterTests.cpp)
    target_link_libraries(test_vtk_validation_presenter PRIVATE fosd_vtk_validation_presenter Qt5::Test)
    add_test(NAME test_vtk_validation_presenter COMMAND test_vtk_validation_presenter)
endif()
