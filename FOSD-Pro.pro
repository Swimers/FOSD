#-------------------------------------------------
#
# Project created by QtCreator 2025-06-26T11:13:38
#
#-------------------------------------------------

QT       += core gui
QT       += opengl

QT += waylandcompositor
LIBS += -lwayland-server -lxkbcommon

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport
                                  QT += printsupport
                                  QT += serialport
                                  QT += network
                                  QT += widgets

CODECFORSRC = UTF-8
CODECFORTR = UTF-8

msvc:QMAKE_CXXFLAGS += /utf-8
win32:QMAKE_CXXFLAGS += /utf-8

TARGET = FOSD-Pro
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

INCLUDEPATH +=  $$PWD/src/main \
                $$PWD/src/UserManage \
                $$PWD/src/Disp2D \
                $$PWD/src/Disp3D \
                $$PWD/src/Math \
                $$PWD/src/Connect \
                $$PWD/src/Dependence \
                $$PWD/src/Communication \
                $$PWD/lib/CustomUi \

SOURCES += \
        $$PWD/src/main/main.cpp \
        $$PWD/src/main/mainwindow.cpp \
        $$PWD/src/UserManage/login.cpp \
        $$PWD/src/UserManage/userregister.cpp \
        $$PWD/src/UserManage/usermanage.cpp \
        $$PWD/src/UserManage/userforget.cpp \
        $$PWD/src/Connect/connectdebug.cpp \
        $$PWD/src/Connect/qcustomplot.cpp \
        $$PWD/src/Connect/connectwindow.cpp \
        $$PWD/src/Connect/qcpdocumentobject.cpp \
        $$PWD/src/Dependence/global.cpp \
        $$PWD/src/Dependence/IFE.cpp \
        $$PWD/src/Dependence/database.cpp \
        $$PWD/src/Communication/serialtask.cpp \
        $$PWD/src/Communication/analyzepackets.cpp \
        $$PWD/src/Communication/udptask.cpp \
        $$PWD/src/Communication/tcptask.cpp \
        $$PWD/src/Disp2D/disp2dpara.cpp \
        $$PWD/src/Disp2D/disp2dwindow.cpp \
        $$PWD/src/Disp3D/disp3dbackground.cpp \
        $$PWD/src/Disp3D/disp3dfreshmodelthread.cpp \
        $$PWD/src/Disp3D/disp3dinteractor.cpp \
        $$PWD/src/Disp3D/disp3dkeypointedit.cpp \
        $$PWD/src/Disp3D/disp3dkeypointselect.cpp \
        $$PWD/src/Disp3D/disp3dmodel.cpp \
        $$PWD/src/Disp3D/disp3dwindow.cpp \
        $$PWD/lib/CustomUi/switchcontrol.cpp \

HEADERS += \
        $$PWD/src/main/mainwindow.h \
        $$PWD/src/UserManage/login.h \
        $$PWD/src/UserManage/userregister.h \
        $$PWD/src/UserManage/usermanage.h \
        $$PWD/src/UserManage/userforget.h \
        $$PWD/src/Disp2D/disp2dwindow.h \
        $$PWD/src/Disp2D/disp2dpara.h \
        $$PWD/src/Connect/connectdebug.h \
        $$PWD/src/Connect/qcustomplot.h \
        $$PWD/src/Connect/connectwindow.h \
        $$PWD/src/Connect/qcpdocumentobject.h \
        $$PWD/src/Dependence/Timer.h \
        $$PWD/src/Dependence/global.h \
        $$PWD/src/Dependence/IFE.h \
        $$PWD/src/Dependence/database.h \
        $$PWD/src/Communication/serialtask.h \
        $$PWD/src/Communication/analyzepackets.h \
        $$PWD/src/Communication/udptask.h \
        $$PWD/src/Communication/udptask.h \
        $$PWD/src/Communication/tcptask.h \
        $$PWD/src/Communication/pocketsconfig.h \
        $$PWD/src/Disp3D/disp3dbackground.h \
        $$PWD/src/Disp3D/disp3dfreshmodelthread.h \
        $$PWD/src/Disp3D/disp3dglobal.h \
        $$PWD/src/Disp3D/disp3dinteractor.h \
        $$PWD/src/Disp3D/disp3dkeypointedit.h \
        $$PWD/src/Disp3D/disp3dkeypointselect.h \
        $$PWD/src/Disp3D/disp3dmodel.h \
        $$PWD/src/Disp3D/disp3dwindow.h \
        $$PWD/lib/CustomUi/switchcontrol.h \

FORMS += \
        $$PWD/src/main/mainwindow.ui \
        $$PWD/src/UserManage/login.ui \
        $$PWD/src/UserManage/userregister.ui \
        $$PWD/src/UserManage/usermanage.ui \
        $$PWD/src/UserManage/userforget.ui \
        $$PWD/src/Connect/connectdebug.ui \
        $$PWD/src/Connect/connectwindow.ui \
        $$PWD/src/Disp2D/disp2dwindow.ui \
        $$PWD/src/Disp2D/disp2dpara.ui \
        $$PWD/src/Disp3D/disp3dkeypointedit.ui \
        $$PWD/src/Disp3D/disp3dkeypointselect.ui \
        $$PWD/src/Disp3D/disp3dwindow.ui

RESOURCES += \
    $$PWD/res/resource.qrc \

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32:CONFIG(release, debug|release):{
VTK_DIR = $$clean_path($$PWD/lib/VTK/VTK-release)
INCLUDEPATH += $$VTK_DIR/include/vtk-8.2
INCLUDEPATH += $$VTK_DIR/include/vtk-8.2/vtkhdf5
LIBS += -L$$VTK_DIR/lib
LIBS += $$VTK_DIR/lib/vtkCommonColor-8.2.lib
LIBS += $$VTK_DIR/lib/vtkCommonComputationalGeometry-8.2.lib
LIBS += $$VTK_DIR/lib/vtkCommonCore-8.2.lib
LIBS += $$VTK_DIR/lib/vtkCommonColor-8.2.lib
LIBS += $$VTK_DIR/lib/vtkCommonComputationalGeometry-8.2.lib
LIBS += $$VTK_DIR/lib/vtkCommonCore-8.2.lib
LIBS += $$VTK_DIR/lib/vtkCommonDataModel-8.2.lib
LIBS += $$VTK_DIR/lib/vtkCommonExecutionModel-8.2.lib
LIBS += $$VTK_DIR/lib/vtkCommonMath-8.2.lib
LIBS += $$VTK_DIR/lib/vtkCommonMisc-8.2.lib
LIBS += $$VTK_DIR/lib/vtkCommonSystem-8.2.lib
LIBS += $$VTK_DIR/lib/vtkCommonTransforms-8.2.lib
LIBS += $$VTK_DIR/lib/vtkDICOMParser-8.2.lib
LIBS += $$VTK_DIR/lib/vtkDomainsChemistry-8.2.lib
LIBS += $$VTK_DIR/lib/vtkDomainsChemistryOpenGL2-8.2.lib
LIBS += $$VTK_DIR/lib/vtkdoubleconversion-8.2.lib
LIBS += $$VTK_DIR/lib/vtkexodusII-8.2.lib
LIBS += $$VTK_DIR/lib/vtkexpat-8.2.lib
LIBS += $$VTK_DIR/lib/vtkFiltersAMR-8.2.lib
LIBS += $$VTK_DIR/lib/vtkFiltersCore-8.2.lib
LIBS += $$VTK_DIR/lib/vtkFiltersExtraction-8.2.lib
LIBS += $$VTK_DIR/lib/vtkFiltersFlowPaths-8.2.lib
LIBS += $$VTK_DIR/lib/vtkFiltersGeneral-8.2.lib
LIBS += $$VTK_DIR/lib/vtkFiltersGeneric-8.2.lib
LIBS += $$VTK_DIR/lib/vtkFiltersGeometry-8.2.lib
LIBS += $$VTK_DIR/lib/vtkFiltersHybrid-8.2.lib
LIBS += $$VTK_DIR/lib/vtkFiltersHyperTree-8.2.lib
LIBS += $$VTK_DIR/lib/vtkFiltersImaging-8.2.lib
LIBS += $$VTK_DIR/lib/vtkFiltersModeling-8.2.lib
LIBS += $$VTK_DIR/lib/vtkFiltersParallel-8.2.lib
LIBS += $$VTK_DIR/lib/vtkFiltersParallelImaging-8.2.lib
LIBS += $$VTK_DIR/lib/vtkFiltersPoints-8.2.lib
LIBS += $$VTK_DIR/lib/vtkFiltersProgrammable-8.2.lib
LIBS += $$VTK_DIR/lib/vtkFiltersSelection-8.2.lib
LIBS += $$VTK_DIR/lib/vtkFiltersSMP-8.2.lib
LIBS += $$VTK_DIR/lib/vtkFiltersSources-8.2.lib
LIBS += $$VTK_DIR/lib/vtkFiltersStatistics-8.2.lib
LIBS += $$VTK_DIR/lib/vtkFiltersTexture-8.2.lib
LIBS += $$VTK_DIR/lib/vtkFiltersTopology-8.2.lib
LIBS += $$VTK_DIR/lib/vtkFiltersVerdict-8.2.lib
LIBS += $$VTK_DIR/lib/vtkfreetype-8.2.lib
LIBS += $$VTK_DIR/lib/vtkGeovisCore-8.2.lib
LIBS += $$VTK_DIR/lib/vtkgl2ps-8.2.lib
LIBS += $$VTK_DIR/lib/vtkglew-8.2.lib
LIBS += $$VTK_DIR/lib/vtkGUISupportQt-8.2.lib
LIBS += $$VTK_DIR/lib/vtkGUISupportQtSQL-8.2.lib
LIBS += $$VTK_DIR/lib/vtkhdf5-8.2.lib
LIBS += $$VTK_DIR/lib/vtkhdf5_hl-8.2.lib
LIBS += $$VTK_DIR/lib/vtkImagingColor-8.2.lib
LIBS += $$VTK_DIR/lib/vtkImagingCore-8.2.lib
LIBS += $$VTK_DIR/lib/vtkImagingFourier-8.2.lib
LIBS += $$VTK_DIR/lib/vtkImagingGeneral-8.2.lib
LIBS += $$VTK_DIR/lib/vtkImagingHybrid-8.2.lib
LIBS += $$VTK_DIR/lib/vtkImagingMath-8.2.lib
LIBS += $$VTK_DIR/lib/vtkImagingMorphological-8.2.lib
LIBS += $$VTK_DIR/lib/vtkImagingSources-8.2.lib
LIBS += $$VTK_DIR/lib/vtkImagingStatistics-8.2.lib
LIBS += $$VTK_DIR/lib/vtkImagingStencil-8.2.lib
LIBS += $$VTK_DIR/lib/vtkInfovisCore-8.2.lib
LIBS += $$VTK_DIR/lib/vtkInfovisLayout-8.2.lib
LIBS += $$VTK_DIR/lib/vtkInteractionImage-8.2.lib
LIBS += $$VTK_DIR/lib/vtkInteractionStyle-8.2.lib
LIBS += $$VTK_DIR/lib/vtkInteractionWidgets-8.2.lib
LIBS += $$VTK_DIR/lib/vtkIOAMR-8.2.lib
LIBS += $$VTK_DIR/lib/vtkIOAsynchronous-8.2.lib
LIBS += $$VTK_DIR/lib/vtkIOCityGML-8.2.lib
LIBS += $$VTK_DIR/lib/vtkIOCore-8.2.lib
LIBS += $$VTK_DIR/lib/vtkIOEnSight-8.2.lib
LIBS += $$VTK_DIR/lib/vtkIOExodus-8.2.lib
LIBS += $$VTK_DIR/lib/vtkIOExport-8.2.lib
LIBS += $$VTK_DIR/lib/vtkIOExportOpenGL2-8.2.lib
LIBS += $$VTK_DIR/lib/vtkIOExportPDF-8.2.lib
LIBS += $$VTK_DIR/lib/vtkIOGeometry-8.2.lib
LIBS += $$VTK_DIR/lib/vtkIOImage-8.2.lib
LIBS += $$VTK_DIR/lib/vtkIOImport-8.2.lib
LIBS += $$VTK_DIR/lib/vtkIOInfovis-8.2.lib
LIBS += $$VTK_DIR/lib/vtkIOLegacy-8.2.lib
LIBS += $$VTK_DIR/lib/vtkIOLSDyna-8.2.lib
LIBS += $$VTK_DIR/lib/vtkIOMINC-8.2.lib
LIBS += $$VTK_DIR/lib/vtkIOMovie-8.2.lib
LIBS += $$VTK_DIR/lib/vtkIONetCDF-8.2.lib
LIBS += $$VTK_DIR/lib/vtkIOParallel-8.2.lib
LIBS += $$VTK_DIR/lib/vtkIOParallelXML-8.2.lib
LIBS += $$VTK_DIR/lib/vtkIOPLY-8.2.lib
LIBS += $$VTK_DIR/lib/vtkIOSegY-8.2.lib
LIBS += $$VTK_DIR/lib/vtkIOSQL-8.2.lib
LIBS += $$VTK_DIR/lib/vtkIOTecplotTable-8.2.lib
LIBS += $$VTK_DIR/lib/vtkIOVeraOut-8.2.lib
LIBS += $$VTK_DIR/lib/vtkIOVideo-8.2.lib
LIBS += $$VTK_DIR/lib/vtkIOXML-8.2.lib
LIBS += $$VTK_DIR/lib/vtkIOXMLParser-8.2.lib
LIBS += $$VTK_DIR/lib/vtkjpeg-8.2.lib
LIBS += $$VTK_DIR/lib/vtkjsoncpp-8.2.lib
LIBS += $$VTK_DIR/lib/vtklibharu-8.2.lib
LIBS += $$VTK_DIR/lib/vtklibxml2-8.2.lib
LIBS += $$VTK_DIR/lib/vtklz4-8.2.lib
LIBS += $$VTK_DIR/lib/vtklzma-8.2.lib
LIBS += $$VTK_DIR/lib/vtkmetaio-8.2.lib
LIBS += $$VTK_DIR/lib/vtkNetCDF-8.2.lib
LIBS += $$VTK_DIR/lib/vtkogg-8.2.lib
LIBS += $$VTK_DIR/lib/vtkParallelCore-8.2.lib
LIBS += $$VTK_DIR/lib/vtkpng-8.2.lib
LIBS += $$VTK_DIR/lib/vtkproj-8.2.lib
LIBS += $$VTK_DIR/lib/vtkpugixml-8.2.lib
LIBS += $$VTK_DIR/lib/vtkRenderingAnnotation-8.2.lib
LIBS += $$VTK_DIR/lib/vtkRenderingContext2D-8.2.lib
LIBS += $$VTK_DIR/lib/vtkRenderingContextOpenGL2-8.2.lib
LIBS += $$VTK_DIR/lib/vtkRenderingCore-8.2.lib
LIBS += $$VTK_DIR/lib/vtkRenderingFreeType-8.2.lib
LIBS += $$VTK_DIR/lib/vtkRenderingGL2PSOpenGL2-8.2.lib
LIBS += $$VTK_DIR/lib/vtkRenderingImage-8.2.lib
LIBS += $$VTK_DIR/lib/vtkRenderingLabel-8.2.lib
LIBS += $$VTK_DIR/lib/vtkRenderingLOD-8.2.lib
LIBS += $$VTK_DIR/lib/vtkRenderingOpenGL2-8.2.lib
LIBS += $$VTK_DIR/lib/vtkRenderingQt-8.2.lib
LIBS += $$VTK_DIR/lib/vtkRenderingVolume-8.2.lib
LIBS += $$VTK_DIR/lib/vtkRenderingVolumeOpenGL2-8.2.lib
LIBS += $$VTK_DIR/lib/vtksqlite-8.2.lib
LIBS += $$VTK_DIR/lib/vtksys-8.2.lib
LIBS += $$VTK_DIR/lib/vtktheora-8.2.lib
LIBS += $$VTK_DIR/lib/vtktiff-8.2.lib
LIBS += $$VTK_DIR/lib/vtkverdict-8.2.lib
LIBS += $$VTK_DIR/lib/vtkViewsContext2D-8.2.lib
LIBS += $$VTK_DIR/lib/vtkViewsCore-8.2.lib
LIBS += $$VTK_DIR/lib/vtkViewsInfovis-8.2.lib
LIBS += $$VTK_DIR/lib/vtkViewsQt-8.2.lib
LIBS += $$VTK_DIR/lib/vtkzlib-8.2.lib
}
else:win32:CONFIG(debug, debug|release):{
VTK_DIR = $$clean_path($$PWD/lib/VTK/VTK-debug)
INCLUDEPATH += $$VTK_DIR/include/vtk-8.2
INCLUDEPATH += $$VTK_DIR/include/vtk-8.2/vtkhdf5
LIBS += -L$$VTK_DIR/lib
LIBS +=$$VTK_DIR/lib/vtkChartsCore-8.2D.lib
LIBS +=$$VTK_DIR/lib/vtkCommonColor-8.2D.lib
LIBS +=$$VTK_DIR/lib/vtkCommonComputationalGeometry-8.2D.lib
LIBS +=$$VTK_DIR/lib/vtkCommonCore-8.2D.lib
LIBS +=$$VTK_DIR/lib/vtkCommonDataModel-8.2D.lib
LIBS +=$$VTK_DIR/lib/vtkCommonExecutionModel-8.2D.lib
LIBS +=$$VTK_DIR/lib/vtkCommonMath-8.2D.lib
LIBS +=$$VTK_DIR/lib/vtkCommonMisc-8.2D.lib
LIBS +=$$VTK_DIR/lib/vtkCommonSystem-8.2D.lib
LIBS +=$$VTK_DIR/lib/vtkCommonTransforms-8.2D.lib
LIBS +=$$VTK_DIR/lib/vtkDICOMParser-8.2D.lib
LIBS +=$$VTK_DIR/lib/vtkDomainsChemistry-8.2D.lib
LIBS +=$$VTK_DIR/lib/vtkDomainsChemistryOpenGL2-8.2D.lib
LIBS +=$$VTK_DIR/lib/vtkdoubleconversion-8.2D.lib
LIBS +=$$VTK_DIR/lib/vtkexodusII-8.2D.lib
LIBS +=$$VTK_DIR/lib/vtkexpat-8.2D.lib
LIBS +=$$VTK_DIR/lib/vtkFiltersAMR-8.2D.lib
LIBS +=$$VTK_DIR/lib/vtkFiltersCore-8.2D.lib
LIBS +=$$VTK_DIR/lib/vtkFiltersExtraction-8.2D.lib
LIBS +=$$VTK_DIR/lib/vtkFiltersFlowPaths-8.2D.lib
LIBS +=$$VTK_DIR/lib/vtkFiltersGeneral-8.2D.lib
LIBS +=$$VTK_DIR/lib/vtkFiltersGeneric-8.2D.lib
LIBS +=$$VTK_DIR/lib/vtkFiltersGeometry-8.2D.lib
LIBS +=$$VTK_DIR/lib/vtkFiltersHybrid-8.2D.lib
LIBS +=$$VTK_DIR/lib/vtkFiltersHyperTree-8.2D.lib
LIBS +=$$VTK_DIR/lib/vtkFiltersImaging-8.2D.lib
LIBS +=$$VTK_DIR/lib/vtkFiltersModeling-8.2D.lib
LIBS +=$$VTK_DIR/lib/vtkFiltersParallel-8.2D.lib
LIBS +=$$VTK_DIR/lib/vtkFiltersParallelImaging-8.2D.lib
LIBS +=$$VTK_DIR/lib/vtkFiltersPoints-8.2D.lib
LIBS +=$$VTK_DIR/lib/vtkFiltersProgrammable-8.2D.lib
LIBS +=$$VTK_DIR/lib/vtkFiltersSelection-8.2D.lib
LIBS +=$$VTK_DIR/lib/vtkFiltersSMP-8.2D.lib
LIBS +=$$VTK_DIR/lib/vtkFiltersSources-8.2D.lib
LIBS +=$$VTK_DIR/lib/vtkFiltersStatistics-8.2D.lib
LIBS +=$$VTK_DIR/lib/vtkFiltersTexture-8.2D.lib
LIBS +=$$VTK_DIR/lib/vtkFiltersTopology-8.2D.lib
LIBS +=$$VTK_DIR/lib/vtkFiltersVerdict-8.2D.lib
LIBS +=$$VTK_DIR/lib/vtkfreetype-8.2D.lib
LIBS +=$$VTK_DIR/lib/vtkGeovisCore-8.2D.lib
LIBS +=$$VTK_DIR/lib/vtkgl2ps-8.2D.lib
LIBS +=$$VTK_DIR/lib/vtkglew-8.2D.lib
LIBS +=$$VTK_DIR/lib/vtkGUISupportQt-8.2D.lib
LIBS +=$$VTK_DIR/lib/vtkGUISupportQtSQL-8.2D.lib
LIBS +=$$VTK_DIR/lib/vtkhdf5-8.2D.lib
LIBS +=$$VTK_DIR/lib/vtkhdf5_hl-8.2D.lib
LIBS +=$$VTK_DIR/lib/vtkImagingColor-8.2D.lib
LIBS +=$$VTK_DIR/lib/vtkImagingCore-8.2D.lib
LIBS +=$$VTK_DIR/lib/vtkImagingFourier-8.2D.lib
LIBS +=$$VTK_DIR/lib/vtkImagingGeneral-8.2D.lib
LIBS +=$$VTK_DIR/lib/vtkImagingHybrid-8.2D.lib
LIBS +=$$VTK_DIR/lib/vtkImagingMath-8.2D.lib
LIBS +=$$VTK_DIR/lib/vtkImagingMorphological-8.2D.lib
LIBS +=$$VTK_DIR/lib/vtkImagingSources-8.2D.lib
LIBS +=$$VTK_DIR/lib/vtkImagingStatistics-8.2D.lib
LIBS +=$$VTK_DIR/lib/vtkImagingStencil-8.2D.lib
LIBS +=$$VTK_DIR/lib/vtkInfovisCore-8.2D.lib
LIBS +=$$VTK_DIR/lib/vtkInfovisLayout-8.2D.lib
LIBS +=$$VTK_DIR/lib/vtkInteractionImage-8.2D.lib
LIBS +=$$VTK_DIR/lib/vtkInteractionStyle-8.2D.lib
LIBS +=$$VTK_DIR/lib/vtkInteractionWidgets-8.2D.lib
LIBS +=$$VTK_DIR/lib/vtkIOAMR-8.2D.lib
LIBS +=$$VTK_DIR/lib/vtkIOAsynchronous-8.2D.lib
LIBS +=$$VTK_DIR/lib/vtkIOCityGML-8.2D.lib
LIBS +=$$VTK_DIR/lib/vtkIOCore-8.2D.lib
LIBS +=$$VTK_DIR/lib/vtkIOEnSight-8.2D.lib
LIBS +=$$VTK_DIR/lib/vtkIOExodus-8.2D.lib
LIBS +=$$VTK_DIR/lib/vtkIOExport-8.2D.lib
LIBS +=$$VTK_DIR/lib/vtkIOExportOpenGL2-8.2D.lib
LIBS +=$$VTK_DIR/lib/vtkIOExportPDF-8.2D.lib
LIBS +=$$VTK_DIR/lib/vtkIOGeometry-8.2D.lib
LIBS +=$$VTK_DIR/lib/vtkIOImage-8.2D.lib
LIBS +=$$VTK_DIR/lib/vtkIOImport-8.2D.lib
LIBS +=$$VTK_DIR/lib/vtkIOInfovis-8.2D.lib
LIBS +=$$VTK_DIR/lib/vtkIOLegacy-8.2D.lib
LIBS +=$$VTK_DIR/lib/vtkIOLSDyna-8.2D.lib
LIBS +=$$VTK_DIR/lib/vtkIOMINC-8.2D.lib
LIBS +=$$VTK_DIR/lib/vtkIOMovie-8.2D.lib
LIBS +=$$VTK_DIR/lib/vtkIONetCDF-8.2D.lib
LIBS +=$$VTK_DIR/lib/vtkIOParallel-8.2D.lib
LIBS +=$$VTK_DIR/lib/vtkIOParallelXML-8.2D.lib
LIBS +=$$VTK_DIR/lib/vtkIOPLY-8.2D.lib
LIBS +=$$VTK_DIR/lib/vtkIOSegY-8.2D.lib
LIBS +=$$VTK_DIR/lib/vtkIOSQL-8.2D.lib
LIBS +=$$VTK_DIR/lib/vtkIOTecplotTable-8.2D.lib
LIBS +=$$VTK_DIR/lib/vtkIOVeraOut-8.2D.lib
LIBS +=$$VTK_DIR/lib/vtkIOVideo-8.2D.lib
LIBS +=$$VTK_DIR/lib/vtkIOXML-8.2D.lib
LIBS +=$$VTK_DIR/lib/vtkIOXMLParser-8.2D.lib
LIBS +=$$VTK_DIR/lib/vtkjpeg-8.2D.lib
LIBS +=$$VTK_DIR/lib/vtkjsoncpp-8.2D.lib
LIBS +=$$VTK_DIR/lib/vtklibharu-8.2D.lib
LIBS +=$$VTK_DIR/lib/vtklibxml2-8.2D.lib
LIBS +=$$VTK_DIR/lib/vtklz4-8.2D.lib
LIBS +=$$VTK_DIR/lib/vtklzma-8.2D.lib
LIBS +=$$VTK_DIR/lib/vtkmetaio-8.2D.lib
LIBS +=$$VTK_DIR/lib/vtkNetCDF-8.2D.lib
LIBS +=$$VTK_DIR/lib/vtkogg-8.2D.lib
LIBS +=$$VTK_DIR/lib/vtkParallelCore-8.2D.lib
LIBS +=$$VTK_DIR/lib/vtkpng-8.2D.lib
LIBS +=$$VTK_DIR/lib/vtkproj-8.2D.lib
LIBS +=$$VTK_DIR/lib/vtkpugixml-8.2D.lib
LIBS +=$$VTK_DIR/lib/vtkRenderingAnnotation-8.2D.lib
LIBS +=$$VTK_DIR/lib/vtkRenderingContext2D-8.2D.lib
LIBS +=$$VTK_DIR/lib/vtkRenderingContextOpenGL2-8.2D.lib
LIBS +=$$VTK_DIR/lib/vtkRenderingCore-8.2D.lib
LIBS +=$$VTK_DIR/lib/vtkRenderingFreeType-8.2D.lib
LIBS +=$$VTK_DIR/lib/vtkRenderingGL2PSOpenGL2-8.2D.lib
LIBS +=$$VTK_DIR/lib/vtkRenderingImage-8.2D.lib
LIBS +=$$VTK_DIR/lib/vtkRenderingLabel-8.2D.lib
LIBS +=$$VTK_DIR/lib/vtkRenderingLOD-8.2D.lib
LIBS +=$$VTK_DIR/lib/vtkRenderingOpenGL2-8.2D.lib
LIBS +=$$VTK_DIR/lib/vtkRenderingQt-8.2D.lib
LIBS +=$$VTK_DIR/lib/vtkRenderingVolume-8.2D.lib
LIBS +=$$VTK_DIR/lib/vtkRenderingVolumeOpenGL2-8.2D.lib
LIBS +=$$VTK_DIR/lib/vtksqlite-8.2D.lib
LIBS +=$$VTK_DIR/lib/vtksys-8.2D.lib
LIBS +=$$VTK_DIR/lib/vtktheora-8.2D.lib
LIBS +=$$VTK_DIR/lib/vtktiff-8.2D.lib
LIBS +=$$VTK_DIR/lib/vtkverdict-8.2D.lib
LIBS +=$$VTK_DIR/lib/vtkViewsContext2D-8.2D.lib
LIBS +=$$VTK_DIR/lib/vtkViewsCore-8.2D.lib
LIBS +=$$VTK_DIR/lib/vtkViewsInfovis-8.2D.lib
LIBS +=$$VTK_DIR/lib/vtkViewsQt-8.2D.lib
LIBS +=$$VTK_DIR/lib/vtkzlib-8.2D.lib
}
else:unix {
VTK_ROOT = $$PWD/lib/VTK/VTK-Linux-debug
VTK_INCLUDE_DIR = $$VTK_ROOT/include/vtk-9.5
VTK_LIB_DIR     = $$VTK_ROOT/lib/vtk-9.5
INCLUDEPATH += $$VTK_INCLUDE_DIR
DEPENDPATH += $$VTK_INCLUDE_DIR
LIBS += -L$$VTK_LIB_DIR
LIBS += -lvtkChartsCore-9.5
LIBS += -lvtkCommonColor-9.5
LIBS += -lvtkCommonComputationalGeometry-9.5
LIBS += -lvtkCommonCore-9.5
LIBS += -lvtkCommonDataModel-9.5
LIBS += -lvtkCommonExecutionModel-9.5
LIBS += -lvtkCommonMath-9.5
LIBS += -lvtkCommonMisc-9.5
LIBS += -lvtkCommonSystem-9.5
LIBS += -lvtkCommonTransforms-9.5
LIBS += -lvtkDICOMParser-9.5
LIBS += -lvtkDomainsChemistry-9.5
LIBS += -lvtkDomainsChemistryOpenGL2-9.5
LIBS += -lvtkFiltersAMR-9.5
LIBS += -lvtkFiltersCellGrid-9.5
LIBS += -lvtkFiltersCore-9.5
LIBS += -lvtkFiltersExtraction-9.5
LIBS += -lvtkFiltersFlowPaths-9.5
LIBS += -lvtkFiltersGeneral-9.5
LIBS += -lvtkFiltersGeneric-9.5
LIBS += -lvtkFiltersGeometry-9.5
LIBS += -lvtkFiltersGeometryPreview-9.5
LIBS += -lvtkFiltersHybrid-9.5
LIBS += -lvtkFiltersHyperTree-9.5
LIBS += -lvtkFiltersImaging-9.5
LIBS += -lvtkFiltersModeling-9.5
LIBS += -lvtkFiltersParallel-9.5
LIBS += -lvtkFiltersParallelImaging-9.5
LIBS += -lvtkFiltersPoints-9.5
LIBS += -lvtkFiltersProgrammable-9.5
LIBS += -lvtkFiltersReduction-9.5
LIBS += -lvtkFiltersSMP-9.5
LIBS += -lvtkFiltersSelection-9.5
LIBS += -lvtkFiltersSources-9.5
LIBS += -lvtkFiltersStatistics-9.5
LIBS += -lvtkFiltersTemporal-9.5
LIBS += -lvtkFiltersTensor-9.5
LIBS += -lvtkFiltersTexture-9.5
LIBS += -lvtkFiltersTopology-9.5
LIBS += -lvtkFiltersVerdict-9.5
LIBS += -lvtkGUISupportQt-9.5
LIBS += -lvtkGUISupportQtQuick-9.5
LIBS += -lvtkGUISupportQtSQL-9.5
LIBS += -lvtkGeovisCore-9.5
LIBS += -lvtkIOAMR-9.5
LIBS += -lvtkIOAsynchronous-9.5
LIBS += -lvtkIOCGNSReader-9.5
LIBS += -lvtkIOCONVERGECFD-9.5
LIBS += -lvtkIOCellGrid-9.5
LIBS += -lvtkIOCesium3DTiles-9.5
LIBS += -lvtkIOChemistry-9.5
LIBS += -lvtkIOCityGML-9.5
LIBS += -lvtkIOCore-9.5
LIBS += -lvtkIOERF-9.5
LIBS += -lvtkIOEnSight-9.5
LIBS += -lvtkIOEngys-9.5
LIBS += -lvtkIOExodus-9.5
LIBS += -lvtkIOExport-9.5
LIBS += -lvtkIOExportGL2PS-9.5
LIBS += -lvtkIOExportPDF-9.5
LIBS += -lvtkIOFDS-9.5
LIBS += -lvtkIOFLUENTCFF-9.5
LIBS += -lvtkIOGeometry-9.5
LIBS += -lvtkIOHDF-9.5
LIBS += -lvtkIOIOSS-9.5
LIBS += -lvtkIOImage-9.5
LIBS += -lvtkIOImport-9.5
LIBS += -lvtkIOInfovis-9.5
LIBS += -lvtkIOLANLX3D-9.5
LIBS += -lvtkIOLSDyna-9.5
LIBS += -lvtkIOLegacy-9.5
LIBS += -lvtkIOMINC-9.5
LIBS += -lvtkIOMotionFX-9.5
LIBS += -lvtkIOMovie-9.5
LIBS += -lvtkIONetCDF-9.5
LIBS += -lvtkIOOggTheora-9.5
LIBS += -lvtkIOPLY-9.5
LIBS += -lvtkIOParallel-9.5
LIBS += -lvtkIOParallelXML-9.5
LIBS += -lvtkIOSQL-9.5
LIBS += -lvtkIOSegY-9.5
LIBS += -lvtkIOTecplotTable-9.5
LIBS += -lvtkIOVeraOut-9.5
LIBS += -lvtkIOVideo-9.5
LIBS += -lvtkIOXML-9.5
LIBS += -lvtkIOXMLParser-9.5
LIBS += -lvtkImagingColor-9.5
LIBS += -lvtkImagingCore-9.5
LIBS += -lvtkImagingFourier-9.5
LIBS += -lvtkImagingGeneral-9.5
LIBS += -lvtkImagingHybrid-9.5
LIBS += -lvtkImagingMath-9.5
LIBS += -lvtkImagingMorphological-9.5
LIBS += -lvtkImagingSources-9.5
LIBS += -lvtkImagingStatistics-9.5
LIBS += -lvtkImagingStencil-9.5
LIBS += -lvtkInfovisCore-9.5
LIBS += -lvtkInfovisLayout-9.5
LIBS += -lvtkInteractionImage-9.5
LIBS += -lvtkInteractionStyle-9.5
LIBS += -lvtkInteractionWidgets-9.5
LIBS += -lvtkParallelCore-9.5
LIBS += -lvtkParallelDIY-9.5
LIBS += -lvtkRenderingAnnotation-9.5
LIBS += -lvtkRenderingCellGrid-9.5
LIBS += -lvtkRenderingContext2D-9.5
LIBS += -lvtkRenderingContextOpenGL2-9.5
LIBS += -lvtkRenderingCore-9.5
LIBS += -lvtkRenderingFreeType-9.5
LIBS += -lvtkRenderingGL2PSOpenGL2-9.5
LIBS += -lvtkRenderingGridAxes-9.5
LIBS += -lvtkRenderingHyperTreeGrid-9.5
LIBS += -lvtkRenderingImage-9.5
LIBS += -lvtkRenderingLICOpenGL2-9.5
LIBS += -lvtkRenderingLOD-9.5
LIBS += -lvtkRenderingLabel-9.5
LIBS += -lvtkRenderingOpenGL2-9.5
LIBS += -lvtkRenderingQt-9.5
LIBS += -lvtkRenderingSceneGraph-9.5
LIBS += -lvtkRenderingUI-9.5
LIBS += -lvtkRenderingVolume-9.5
LIBS += -lvtkRenderingVolumeOpenGL2-9.5
LIBS += -lvtkRenderingVtkJS-9.5
LIBS += -lvtkTestingCore-9.5
LIBS += -lvtkTestingRendering-9.5
LIBS += -lvtkViewsContext2D-9.5
LIBS += -lvtkViewsCore-9.5
LIBS += -lvtkViewsInfovis-9.5
LIBS += -lvtkViewsQt-9.5
LIBS += -lvtkWrappingTools-9.5
LIBS += -lvtkcgns-9.5
LIBS += -lvtkdoubleconversion-9.5
LIBS += -lvtkexodusII-9.5
LIBS += -lvtkexpat-9.5
LIBS += -lvtkfmt-9.5
LIBS += -lvtkfreetype-9.5
LIBS += -lvtkgl2ps-9.5
LIBS += -lvtkglad-9.5
LIBS += -lvtkhdf5-9.5
LIBS += -lvtkhdf5_hl-9.5
LIBS += -lvtkioss-9.5
LIBS += -lvtkjpeg-9.5
LIBS += -lvtkjsoncpp-9.5
LIBS += -lvtkkissfft-9.5
LIBS += -lvtklibharu-9.5
LIBS += -lvtklibproj-9.5
LIBS += -lvtklibxml2-9.5
LIBS += -lvtkloguru-9.5
LIBS += -lvtklz4-9.5
LIBS += -lvtklzma-9.5
LIBS += -lvtkmetaio-9.5
LIBS += -lvtknetcdf-9.5
LIBS += -lvtkogg-9.5
LIBS += -lvtkpng-9.5
LIBS += -lvtkpugixml-9.5
LIBS += -lvtksqlite-9.5
LIBS += -lvtksys-9.5
LIBS += -lvtktheora-9.5
LIBS += -lvtktiff-9.5
LIBS += -lvtktoken-9.5
LIBS += -lvtkverdict-9.5
LIBS += -lvtkzlib-9.5
}


win32 {         SYSTEM_NAME = windows}
unix:!macx {    SYSTEM_NAME = linux}
macx {          SYSTEM_NAME = macos}

BUILD_ROOT = $$PWD/build/$$SYSTEM_NAME

CONFIG(release, debug|release) {
    BUILD_MODE = release
} else:CONFIG(debug, debug|release) {
    BUILD_MODE = debug
} else {
    BUILD_MODE = debug
}

DESTDIR = $$BUILD_ROOT/bin/$$BUILD_MODE
OBJECTS_DIR = $$BUILD_ROOT/obj/$$BUILD_MODE
MOC_DIR = $$BUILD_ROOT/generated/moc/$$BUILD_MODE
UI_DIR = $$BUILD_ROOT/generated/ui/$$BUILD_MODE
RCC_DIR = $$BUILD_ROOT/generated/rcc/$$BUILD_MODE

QMAKE_CXXFLAGS += -Wall

BUILD_DIRS = $$BUILD_ROOT \
             $$DESTDIR \
             $$OBJECTS_DIR \
             $$MOC_DIR \
             $$UI_DIR \
             $$RCC_DIR
for(dir, BUILD_DIRS) {
    !exists($$dir) {
        win32 {
            system(md $$shell_path($$dir))
        }
        unix {
            system(mkdir -p $$shell_path($$dir))
        }
    }
}
