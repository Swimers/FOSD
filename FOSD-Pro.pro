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

INCLUDEPATH += ./UserManage \
               ./Disp2D \
               ./Disp3D \
               ./Math \
               ./Connect \
               ./Dependence \
               ./Communication \
               ./Dependence/CustomUi \


SOURCES += \
        main.cpp \
        UserManage\login.cpp \
    mainwindow.cpp \
    UserManage/userregister.cpp \
    UserManage/usermanage.cpp \
    UserManage/userforget.cpp \
    Connect/connectdebug.cpp \
    Disp2D/disp2dpara.cpp \
    Disp2D/disp2dwindow.cpp \
    Connect/qcustomplot.cpp \
    Connect/connectwindow.cpp \
    Connect/connectdebug.cpp \
    Connect/connectwindow.cpp \
    Connect/qcpdocumentobject.cpp \
    Connect/qcustomplot.cpp \
    Dependence/global.cpp \
    Dependence/IFE.cpp \
    Dependence/database.cpp \
    Communication/serialtask.cpp \
    Communication/analyzepackets.cpp \
    Communication/udptask.cpp \
    Communication/tcptask.cpp \
    Dependence/CustomUi/switchcontrol.cpp \
    Disp3D/disp3dbackground.cpp \
    Disp3D/disp3dfreshmodelthread.cpp \
    Disp3D/disp3dinteractor.cpp \
    Disp3D/disp3dkeypointedit.cpp \
    Disp3D/disp3dkeypointselect.cpp \
    Disp3D/disp3dmodel.cpp \
    Disp3D/disp3dwindow.cpp

HEADERS += \
        UserManage\login.h \
    mainwindow.h \
    UserManage/userregister.h \
    UserManage/usermanage.h \
    UserManage/userforget.h \
    Connect/connectdebug.h \
    Disp2D/disp2dwindow.h \
    Disp2D/disp2dpara.h \
    Connect/qcustomplot.h \
    Connect/connectwindow.h \
    Connect/connectdebug.h \
    Connect/connectwindow.h \
    Connect/qcpdocumentobject.h \
    Connect/qcustomplot.h \
    Dependence/Timer.h \
    Dependence/global.h \
    Dependence/IFE.h \
    Dependence/database.h \
    Communication/serialtask.h \
    Communication/analyzepackets.h \
    Communication/udptask.h \
    Communication/udptask.h \
    Communication/tcptask.h \
    Communication/pocketsconfig.h \
    Dependence/CustomUi/switchcontrol.h \
    Disp3D/disp3dbackground.h \
    Disp3D/disp3dfreshmodelthread.h \
    Disp3D/disp3dglobal.h \
    Disp3D/disp3dinteractor.h \
    Disp3D/disp3dkeypointedit.h \
    Disp3D/disp3dkeypointselect.h \
    Disp3D/disp3dmodel.h \
    Disp3D/disp3dwindow.h

FORMS += \
        UserManage\login.ui \
    mainwindow.ui \
    UserManage/userregister.ui \
    UserManage/usermanage.ui \
    UserManage/userforget.ui \
    Connect/connectdebug.ui \
    Disp2D/disp2dwindow.ui \
    Disp2D/disp2dpara.ui \
    Connect/connectwindow.ui \
    Disp3D/disp3dkeypointedit.ui \
    Disp3D/disp3dkeypointselect.ui \
    Disp3D/disp3dwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resource.qrc

win32:CONFIG(release, debug|release):{

VTK_DIR = $$clean_path($$PWD/VTK-release)
INCLUDEPATH += $$VTK_DIR/include/vtk-8.2
INCLUDEPATH += $$VTK_DIR/include/vtk-8.2/vtkhdf5
LIBS += -L$$VTK_DIR/lib

# 下面库名照你原来的继续加（建议逐步精简）
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
VTK_DIR = $$clean_path($$PWD/VTK-debug)
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
