find_package(Qt5 5.12 REQUIRED COMPONENTS Widgets PrintSupport)

add_library(fosd_workbench_presenter STATIC
    contracts/UiTypes.h contracts/IPanelView.h contracts/IWorkbenchView.h contracts/IWorkbenchBackend.h
    presenters/PanelPresenter.h presenters/PanelPresenter.cpp
    presenters/WorkbenchPresenter.h presenters/WorkbenchPresenter.cpp
)
target_include_directories(fosd_workbench_presenter PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/..")
target_link_libraries(fosd_workbench_presenter PUBLIC Qt5::Core)

# Migrated drawing/switch controls; no legacy business/backend sources.
add_library(fosd_workbench_controls STATIC
    controls/qcustomplot.h controls/qcustomplot.cpp
    controls/switchcontrol.h controls/switchcontrol.cpp
    controls/renderhostwidget.h controls/renderhostwidget.cpp
)
target_include_directories(fosd_workbench_controls PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/controls")
target_link_libraries(fosd_workbench_controls PUBLIC Qt5::Widgets Qt5::PrintSupport)

set(_form_dirs connection disp2d disp3d disp3d/docks mainwindow user)
set(_forms
    connection/connectwindow.ui connection/connectdebug.ui
    disp2d/disp2dwindow.ui disp2d/disp2dpara.ui
    disp3d/disp3dwindow.ui disp3d/disp3dcolorpicker.ui
    disp3d/disp3dkeypointedit.ui disp3d/disp3dkeypointselect.ui disp3d/dispdata.ui
    disp3d/trackingpointmanager.ui disp3d/trackingpointdatatable.ui
    disp3d/docks/disp3dinfodockpanel.ui disp3d/docks/disp3dmodeldockpanel.ui disp3d/docks/disp3dviewdockpanel.ui
    mainwindow/oldmainwindow.ui mainwindow/rainbowmainwindow.ui
    user/login.ui user/userregister.ui user/userforget.ui user/usermanage.ui
)
set(_form_paths)
set(_search_paths)
foreach(_form IN LISTS _forms)
    list(APPEND _form_paths "forms/${_form}")
endforeach()
foreach(_dir IN LISTS _form_dirs)
    list(APPEND _search_paths "${CMAKE_CURRENT_SOURCE_DIR}/forms/${_dir}")
endforeach()
add_library(fosd_workbench_widgets STATIC
    views/LegacyViews.h views/LegacyViews.cpp
    views/FormBinding.h views/FormBinding.cpp
    views/WorkbenchView.h views/WorkbenchView.cpp
    resources/UiResources.h resources/UiResources.cpp resources/legacy_ui.qrc
    ${_form_paths}
)
set_target_properties(fosd_workbench_widgets PROPERTIES AUTOUIC_SEARCH_PATHS "${_search_paths}")
target_include_directories(fosd_workbench_widgets PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/..")
target_link_libraries(fosd_workbench_widgets PUBLIC fosd_workbench_presenter fosd_workbench_controls Qt5::Widgets)
target_compile_features(fosd_workbench_presenter PUBLIC cxx_std_11)
target_compile_features(fosd_workbench_controls PUBLIC cxx_std_11)
target_compile_features(fosd_workbench_widgets PUBLIC cxx_std_11)

if(FOSD_BUILD_TESTS OR FOSD_WORKBENCH_BUILD_TESTS)
    add_executable(test_workbench_widgets tests/WorkbenchTests.cpp)
    target_link_libraries(test_workbench_widgets PRIVATE fosd_workbench_widgets Qt5::Test)
    add_test(NAME test_workbench_widgets COMMAND test_workbench_widgets)
    set_tests_properties(test_workbench_widgets PROPERTIES ENVIRONMENT "QT_QPA_PLATFORM=offscreen" TIMEOUT 60)
    if(WIN32)
        set_property(TEST test_workbench_widgets APPEND PROPERTY ENVIRONMENT "QT_QPA_FONTDIR=$ENV{WINDIR}/Fonts")
    endif()
endif()
