find_package(Qt5 5.12 REQUIRED COMPONENTS Widgets PrintSupport)

add_library(fosd_classic_contracts STATIC
    common/UiTypes.h common/UiTypes.cpp common/IView.h common/ViewEvents.h common/IBackend.h
    common/WorkbenchServices.h
)
target_include_directories(fosd_classic_contracts PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/..")
target_link_libraries(fosd_classic_contracts PUBLIC Qt5::Core)

add_library(fosd_classic_backends STATIC
    common/FeatureBackend.h common/FeatureBackend.cpp
    apps/WorkbenchBackend.h apps/WorkbenchBackend.cpp
    apps/mainwindow/backends/MainWindowBackend.h apps/mainwindow/backends/MainWindowBackend.cpp
    apps/connection/backends/ConnectionBackend.h apps/connection/backends/ConnectionBackend.cpp
    apps/visualization/backends/VisualizationBackend.h apps/visualization/backends/VisualizationBackend.cpp
    apps/model/backends/ModelBackend.h apps/model/backends/ModelBackend.cpp
    apps/tracking/backends/TrackingBackend.h apps/tracking/backends/TrackingBackend.cpp
)
target_link_libraries(fosd_classic_backends PUBLIC fosd_classic_contracts)

add_library(fosd_classic_presenters STATIC
    common/PanelPresenter.h common/PanelPresenter.cpp
    apps/IWorkbenchView.h apps/WorkbenchPresenter.h apps/WorkbenchPresenter.cpp
    apps/mainwindow/presenters/MainWindowPresenter.h apps/mainwindow/presenters/MainWindowPresenter.cpp
    apps/connection/presenters/ConnectionPresenter.h apps/connection/presenters/ConnectionPresenter.cpp
    apps/visualization/presenters/VisualizationPresenter.h apps/visualization/presenters/VisualizationPresenter.cpp
    apps/visualization/presenters/sceneColorDialog.ui
    apps/model/presenters/ModelPresenter.h apps/model/presenters/ModelPresenter.cpp
    apps/tracking/presenters/TrackingPresenter.h apps/tracking/presenters/TrackingPresenter.cpp
)
target_link_libraries(fosd_classic_presenters PUBLIC fosd_classic_backends Qt5::Widgets)

add_library(fosd_classic_controls STATIC
    controls/qcustomplot.h controls/qcustomplot.cpp
    controls/switchcontrol.h controls/switchcontrol.cpp
    controls/renderhostwidget.h controls/renderhostwidget.cpp
)
target_include_directories(fosd_classic_controls PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/controls")
target_link_libraries(fosd_classic_controls PUBLIC Qt5::Widgets Qt5::PrintSupport)

add_library(fosd_classic_widgets STATIC
    common/FormBinding.h common/FormBinding.cpp common/ViewFactory.h common/ViewFactory.cpp
    apps/WorkbenchView.h apps/WorkbenchView.cpp
    resources/UiResources.h resources/UiResources.cpp resources/legacy_ui.qrc
    apps/connection/views/connectwindow.h
    apps/connection/views/connectwindow.cpp
    apps/connection/views/connectwindow.ui
    apps/connection/views/channlCalibration.h
    apps/connection/views/channlCalibration.cpp
    apps/connection/views/channlCalibration.ui
    apps/visualization/views/disp3dwindow.h
    apps/visualization/views/disp3dwindow.cpp
    apps/visualization/views/disp3dwindow.ui
    apps/visualization/views/docks/disp3dinfodockpanel.h
    apps/visualization/views/docks/disp3dinfodockpanel.cpp
    apps/visualization/views/docks/disp3dinfodockpanel.ui
    apps/visualization/views/docks/disp3dviewdockpanel.h
    apps/visualization/views/docks/disp3dviewdockpanel.cpp
    apps/visualization/views/docks/disp3dviewdockpanel.ui
    apps/visualization/views/docks/disp3dmodeldockpanel.h
    apps/visualization/views/docks/disp3dmodeldockpanel.cpp
    apps/visualization/views/docks/disp3dmodeldockpanel.ui
    apps/model/views/disp3dkeypointedit.h
    apps/model/views/disp3dkeypointedit.cpp
    apps/model/views/disp3dkeypointedit.ui
    apps/model/views/disp3dkeypointselect.h
    apps/model/views/disp3dkeypointselect.cpp
    apps/model/views/disp3dkeypointselect.ui
    apps/tracking/views/dispdata.h
    apps/tracking/views/dispdata.cpp
    apps/tracking/views/dispdata.ui
    apps/mainwindow/views/oldmainwindow.h
    apps/mainwindow/views/oldmainwindow.cpp
    apps/mainwindow/views/oldmainwindow.ui
)
target_link_libraries(fosd_classic_widgets PUBLIC fosd_classic_presenters fosd_classic_controls Qt5::Widgets)

if(FOSD_BUILD_TESTS OR FOSD_WORKBENCH_BUILD_TESTS)
    add_executable(test_classic_widgets tests/WorkbenchTests.cpp)
    target_link_libraries(test_classic_widgets PRIVATE fosd_classic_widgets Qt5::Test)
    add_test(NAME test_classic_widgets COMMAND test_classic_widgets)
    set_tests_properties(test_classic_widgets PROPERTIES ENVIRONMENT "QT_QPA_PLATFORM=offscreen" TIMEOUT 60)
    if(WIN32)
        set_property(TEST test_classic_widgets APPEND PROPERTY ENVIRONMENT "QT_QPA_FONTDIR=$ENV{WINDIR}/Fonts")
    endif()
endif()
