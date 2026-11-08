find_package(Qt5 5.12 REQUIRED COMPONENTS Widgets PrintSupport)

add_library(fosd_classic_contracts STATIC
    common/UiTypes.h common/IView.h common/ViewEvents.h
)
target_include_directories(fosd_classic_contracts PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/..")
target_link_libraries(fosd_classic_contracts PUBLIC Qt5::Core)

add_library(fosd_classic_model STATIC model/WorkbenchModel.h model/WorkbenchModel.cpp model/WorkbenchMarkers.cpp)
target_include_directories(fosd_classic_model PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/..")
target_link_libraries(fosd_classic_model PUBLIC fosd_protocol fosd_calculation fosd_visualization)
set_target_properties(fosd_classic_contracts PROPERTIES LINKER_LANGUAGE CXX)

add_library(fosd_classic_presenters STATIC
    common/PanelPresenter.h common/PanelPresenter.cpp
    application/IWorkbenchView.h application/WorkbenchPresenter.h application/WorkbenchPresenter.cpp
    features/mainwindow/presenters/MainWindowPresenter.h features/mainwindow/presenters/MainWindowPresenter.cpp
    features/connection/presenters/ConnectionPresenter.h features/connection/presenters/ConnectionPresenter.cpp
    features/visualization/presenters/VisualizationPresenter.h features/visualization/presenters/VisualizationPresenter.cpp
    features/visualization/presenters/ModelPanelPresenter.h features/visualization/presenters/ModelPanelPresenter.cpp
    features/visualization/views/dialogs/sceneColorDialog.ui
    features/data/presenters/HardwareDataPresenter.h features/data/presenters/HardwareDataPresenter.cpp
)
target_link_libraries(fosd_classic_presenters PUBLIC fosd_classic_contracts fosd_classic_model Qt5::Widgets)
set_target_properties(fosd_classic_presenters PROPERTIES
    AUTOUIC_SEARCH_PATHS "${CMAKE_CURRENT_SOURCE_DIR}/features/visualization/views/dialogs")

add_library(fosd_classic_controls STATIC
    controls/switchcontrol.h controls/switchcontrol.cpp
    controls/renderhostwidget.h controls/renderhostwidget.cpp
    third_party/qcustomplot/qcustomplot.h third_party/qcustomplot/qcustomplot.cpp
)
target_include_directories(fosd_classic_controls PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/.."
    "${CMAKE_CURRENT_SOURCE_DIR}/controls"
    "${CMAKE_CURRENT_SOURCE_DIR}/third_party/qcustomplot")
target_link_libraries(fosd_classic_controls PUBLIC Qt5::Widgets Qt5::PrintSupport)

add_library(fosd_classic_widgets STATIC
    common/FormBinding.h common/FormBinding.cpp common/ViewFactory.h common/ViewFactory.cpp
    common/CoordinateText.h common/UiResources.h common/UiResources.cpp resources/classic.qrc
    application/WorkbenchView.h application/WorkbenchView.cpp
    features/connection/views/ConnectionRenderers.h features/connection/views/ConnectionRenderers.cpp
    features/connection/views/connectwindow.h features/connection/views/connectwindow.cpp features/connection/views/connectwindow.ui
    features/connection/views/channlCalibration.h features/connection/views/channlCalibration.cpp features/connection/views/channlCalibration.ui
    features/visualization/views/disp3dwindow.h features/visualization/views/disp3dwindow.cpp features/visualization/views/disp3dwindow.ui
    features/visualization/views/docks/disp3dinfodockpanel.h features/visualization/views/docks/disp3dinfodockpanel.cpp features/visualization/views/docks/disp3dinfodockpanel.ui
    features/visualization/views/docks/disp3dviewdockpanel.h features/visualization/views/docks/disp3dviewdockpanel.cpp features/visualization/views/docks/disp3dviewdockpanel.ui
    features/visualization/views/docks/disp3dmodeldockpanel.h features/visualization/views/docks/disp3dmodeldockpanel.cpp features/visualization/views/docks/disp3dmodeldockpanel.ui
    features/visualization/views/keypoints/disp3dkeypointedit.h features/visualization/views/keypoints/disp3dkeypointedit.cpp features/visualization/views/keypoints/disp3dkeypointedit.ui
    features/visualization/views/keypoints/disp3dkeypointselect.h features/visualization/views/keypoints/disp3dkeypointselect.cpp features/visualization/views/keypoints/disp3dkeypointselect.ui
    features/data/views/dispdata.h features/data/views/dispdata.cpp features/data/views/dispdata.ui
    features/mainwindow/views/oldmainwindow.h features/mainwindow/views/oldmainwindow.cpp features/mainwindow/views/oldmainwindow.ui
)
target_link_libraries(fosd_classic_widgets PUBLIC fosd_classic_contracts fosd_classic_controls Qt5::Widgets)

if(FOSD_BUILD_WORKBENCH_APP)
    add_subdirectory(application)
endif()
if(FOSD_BUILD_TESTS OR FOSD_WORKBENCH_BUILD_TESTS)
    add_subdirectory(tests)
endif()
