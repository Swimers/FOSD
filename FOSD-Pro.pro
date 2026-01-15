#-------------------------------------------------
#
# Project created by QtCreator 2025-06-26T11:13:38
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport
                                  QT += printsupport
                                  QT += serialport
                                  QT += network

CODECFORSRC = UTF-8
CODECFORTR = UTF-8

QMAKE_CXXFLAGS += /utf-8
QMAKE_CFLAGS += /utf-8

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
                $$PWD/src/Dependence/CustomUi \
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
        $$PWD/src/Connect/connectdebug.cpp \
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
        lib/CustomUi/switchcontrol.cpp

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
        $$PWD/src/Connect/connectdebug.h \
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
        lib/CustomUi/switchcontrol.h

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

# ===================== 定义统一的构建根目录 =====================
# $$PWD 表示.pro文件所在的当前目录（项目根目录）
# 所有构建产物都集中到 项目根目录/build 下，便于统一管理
BUILD_ROOT = $$PWD/build

win32:CONFIG(release, debug|release):{
    BUILD_MODE = release
}
else:win32:CONFIG(debug, debug|release):{
    BUILD_MODE = debug
}

DESTDIR = $$BUILD_ROOT/bin/$$BUILD_MODE
OBJECTS_DIR = $$BUILD_ROOT/obj/$$BUILD_MODE
MOC_DIR = $$BUILD_ROOT/generated/moc/$$BUILD_MODE
UI_DIR = $$BUILD_ROOT/generated/ui/$$BUILD_MODE
RCC_DIR = $$BUILD_ROOT/generated/rcc/$$BUILD_MODE

# 指定编译器输出的警告/错误日志
QMAKE_CXXFLAGS += -Wall
# 确保目录不存在时自动创建
!exists($$BUILD_ROOT) {
    system(mkdir -p $$BUILD_ROOT)  # Linux/Mac
    system(mkdir $$BUILD_ROOT)     # Windows（qmake会自动兼容）
}

