#-------------------------------------------------
#
# Project created by QtCreator 2025-09-21T17:03:13
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport
                                  QT += printsupport
                                  QT += serialport
                                  QT += network

TARGET = VTK_Test
TEMPLATE = app

CODECFORSRC = UTF-8
CODECFORTR = UTF-8

QMAKE_CXXFLAGS += /utf-8
QMAKE_CFLAGS += /utf-8

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

INCLUDEPATH += ./Disp3D \
            += ./Dependence \

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    Disp3D/disp3dwindow.cpp \
    Disp3D/disp3dmodel.cpp \
    Disp3D/disp3dbackground.cpp \
    Disp3D/disp3dinteractor.cpp \
    Disp3D/disp3dkeypointedit.cpp \
    Disp3D/disp3dkeypointselect.cpp \
    Disp3D/disp3dfreshmodelthread.cpp \
    Dependence/IFE.cpp \
    Dependence/global.cpp \
    Dependence/IFE.cpp

HEADERS += \
        mainwindow.h \
    Disp3D/disp3dwindow.h \
    Disp3D/disp3dmodel.h \
    Disp3D/disp3dbackground.h \
    Disp3D/disp3dinteractor.h \
    Disp3D/disp3dkeypointedit.h \
    Disp3D/disp3dkeypointselect.h \
    Disp3D/disp3dglobal.h \
    Disp3D/disp3dfreshmodelthread.h \
    Dependence/IFE.h \
    Dependence/global.h \
    Dependence/IFE.h

FORMS += \
        mainwindow.ui \
    Disp3D/disp3dwindow.ui \
    Disp3D/disp3dkeypointedit.ui \
    Disp3D/disp3dkeypointselect.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32:CONFIG(release, debug|release):{
#D:\VTK\VTK-release
INCLUDEPATH += D:\VTK\VTK-release\include\vtk-8.2
INCLUDEPATH += D:\VTK\VTK-release\include\vtk-8.2\vtkhdf5
DEPENDPATH += $$PWD/LIB/release/include
//LIBS += D:\VTK\VTK-release\lib\vtkChartsCore-8.2.lib
LIBS += D:\VTK\VTK-release\lib\vtkCommonColor-8.2.lib
LIBS += D:\VTK\VTK-release\lib\vtkCommonComputationalGeometry-8.2.lib
LIBS += D:\VTK\VTK-release\lib\vtkCommonCore-8.2.lib
LIBS += D:\VTK\VTK-release\lib\vtkCommonDataModel-8.2.lib
LIBS += D:\VTK\VTK-release\lib\vtkCommonExecutionModel-8.2.lib
LIBS += D:\VTK\VTK-release\lib\vtkCommonMath-8.2.lib
LIBS += D:\VTK\VTK-release\lib\vtkCommonMisc-8.2.lib
LIBS += D:\VTK\VTK-release\lib\vtkCommonSystem-8.2.lib
LIBS += D:\VTK\VTK-release\lib\vtkCommonTransforms-8.2.lib
LIBS += D:\VTK\VTK-release\lib\vtkDICOMParser-8.2.lib
LIBS += D:\VTK\VTK-release\lib\vtkDomainsChemistry-8.2.lib
LIBS += D:\VTK\VTK-release\lib\vtkDomainsChemistryOpenGL2-8.2.lib
LIBS += D:\VTK\VTK-release\lib\vtkdoubleconversion-8.2.lib
LIBS += D:\VTK\VTK-release\lib\vtkexodusII-8.2.lib
LIBS += D:\VTK\VTK-release\lib\vtkexpat-8.2.lib
LIBS += D:\VTK\VTK-release\lib\vtkFiltersAMR-8.2.lib
LIBS += D:\VTK\VTK-release\lib\vtkFiltersCore-8.2.lib
LIBS += D:\VTK\VTK-release\lib\vtkFiltersExtraction-8.2.lib
LIBS += D:\VTK\VTK-release\lib\vtkFiltersFlowPaths-8.2.lib
LIBS += D:\VTK\VTK-release\lib\vtkFiltersGeneral-8.2.lib
LIBS += D:\VTK\VTK-release\lib\vtkFiltersGeneric-8.2.lib
LIBS += D:\VTK\VTK-release\lib\vtkFiltersGeometry-8.2.lib
LIBS += D:\VTK\VTK-release\lib\vtkFiltersHybrid-8.2.lib
LIBS += D:\VTK\VTK-release\lib\vtkFiltersHyperTree-8.2.lib
LIBS += D:\VTK\VTK-release\lib\vtkFiltersImaging-8.2.lib
LIBS += D:\VTK\VTK-release\lib\vtkFiltersModeling-8.2.lib
LIBS += D:\VTK\VTK-release\lib\vtkFiltersParallel-8.2.lib
LIBS += D:\VTK\VTK-release\lib\vtkFiltersParallelImaging-8.2.lib
LIBS += D:\VTK\VTK-release\lib\vtkFiltersPoints-8.2.lib
LIBS += D:\VTK\VTK-release\lib\vtkFiltersProgrammable-8.2.lib
LIBS += D:\VTK\VTK-release\lib\vtkFiltersSelection-8.2.lib
LIBS += D:\VTK\VTK-release\lib\vtkFiltersSMP-8.2.lib
LIBS += D:\VTK\VTK-release\lib\vtkFiltersSources-8.2.lib
LIBS += D:\VTK\VTK-release\lib\vtkFiltersStatistics-8.2.lib
LIBS += D:\VTK\VTK-release\lib\vtkFiltersTexture-8.2.lib
LIBS += D:\VTK\VTK-release\lib\vtkFiltersTopology-8.2.lib
LIBS += D:\VTK\VTK-release\lib\vtkFiltersVerdict-8.2.lib
LIBS += D:\VTK\VTK-release\lib\vtkfreetype-8.2.lib
LIBS += D:\VTK\VTK-release\lib\vtkGeovisCore-8.2.lib
LIBS += D:\VTK\VTK-release\lib\vtkgl2ps-8.2.lib
LIBS += D:\VTK\VTK-release\lib\vtkglew-8.2.lib
LIBS += D:\VTK\VTK-release\lib\vtkGUISupportQt-8.2.lib
LIBS += D:\VTK\VTK-release\lib\vtkGUISupportQtSQL-8.2.lib
LIBS += D:\VTK\VTK-release\lib\vtkhdf5-8.2.lib
LIBS += D:\VTK\VTK-release\lib\vtkhdf5_hl-8.2.lib
LIBS += D:\VTK\VTK-release\lib\vtkImagingColor-8.2.lib
LIBS += D:\VTK\VTK-release\lib\vtkImagingCore-8.2.lib
LIBS += D:\VTK\VTK-release\lib\vtkImagingFourier-8.2.lib
LIBS += D:\VTK\VTK-release\lib\vtkImagingGeneral-8.2.lib
LIBS += D:\VTK\VTK-release\lib\vtkImagingHybrid-8.2.lib
LIBS += D:\VTK\VTK-release\lib\vtkImagingMath-8.2.lib
LIBS += D:\VTK\VTK-release\lib\vtkImagingMorphological-8.2.lib
LIBS += D:\VTK\VTK-release\lib\vtkImagingSources-8.2.lib
LIBS += D:\VTK\VTK-release\lib\vtkImagingStatistics-8.2.lib
LIBS += D:\VTK\VTK-release\lib\vtkImagingStencil-8.2.lib
LIBS += D:\VTK\VTK-release\lib\vtkInfovisCore-8.2.lib
LIBS += D:\VTK\VTK-release\lib\vtkInfovisLayout-8.2.lib
LIBS += D:\VTK\VTK-release\lib\vtkInteractionImage-8.2.lib
LIBS += D:\VTK\VTK-release\lib\vtkInteractionStyle-8.2.lib
LIBS += D:\VTK\VTK-release\lib\vtkInteractionWidgets-8.2.lib
LIBS += D:\VTK\VTK-release\lib\vtkIOAMR-8.2.lib
LIBS += D:\VTK\VTK-release\lib\vtkIOAsynchronous-8.2.lib
LIBS += D:\VTK\VTK-release\lib\vtkIOCityGML-8.2.lib
LIBS += D:\VTK\VTK-release\lib\vtkIOCore-8.2.lib
LIBS += D:\VTK\VTK-release\lib\vtkIOEnSight-8.2.lib
LIBS += D:\VTK\VTK-release\lib\vtkIOExodus-8.2.lib
LIBS += D:\VTK\VTK-release\lib\vtkIOExport-8.2.lib
LIBS += D:\VTK\VTK-release\lib\vtkIOExportOpenGL2-8.2.lib
LIBS += D:\VTK\VTK-release\lib\vtkIOExportPDF-8.2.lib
LIBS += D:\VTK\VTK-release\lib\vtkIOGeometry-8.2.lib
LIBS += D:\VTK\VTK-release\lib\vtkIOImage-8.2.lib
LIBS += D:\VTK\VTK-release\lib\vtkIOImport-8.2.lib
LIBS += D:\VTK\VTK-release\lib\vtkIOInfovis-8.2.lib
LIBS += D:\VTK\VTK-release\lib\vtkIOLegacy-8.2.lib
LIBS += D:\VTK\VTK-release\lib\vtkIOLSDyna-8.2.lib
LIBS += D:\VTK\VTK-release\lib\vtkIOMINC-8.2.lib
LIBS += D:\VTK\VTK-release\lib\vtkIOMovie-8.2.lib
LIBS += D:\VTK\VTK-release\lib\vtkIONetCDF-8.2.lib
LIBS += D:\VTK\VTK-release\lib\vtkIOParallel-8.2.lib
LIBS += D:\VTK\VTK-release\lib\vtkIOParallelXML-8.2.lib
LIBS += D:\VTK\VTK-release\lib\vtkIOPLY-8.2.lib
LIBS += D:\VTK\VTK-release\lib\vtkIOSegY-8.2.lib
LIBS += D:\VTK\VTK-release\lib\vtkIOSQL-8.2.lib
LIBS += D:\VTK\VTK-release\lib\vtkIOTecplotTable-8.2.lib
LIBS += D:\VTK\VTK-release\lib\vtkIOVeraOut-8.2.lib
LIBS += D:\VTK\VTK-release\lib\vtkIOVideo-8.2.lib
LIBS += D:\VTK\VTK-release\lib\vtkIOXML-8.2.lib
LIBS += D:\VTK\VTK-release\lib\vtkIOXMLParser-8.2.lib
LIBS += D:\VTK\VTK-release\lib\vtkjpeg-8.2.lib
LIBS += D:\VTK\VTK-release\lib\vtkjsoncpp-8.2.lib
LIBS += D:\VTK\VTK-release\lib\vtklibharu-8.2.lib
LIBS += D:\VTK\VTK-release\lib\vtklibxml2-8.2.lib
LIBS += D:\VTK\VTK-release\lib\vtklz4-8.2.lib
LIBS += D:\VTK\VTK-release\lib\vtklzma-8.2.lib
LIBS += D:\VTK\VTK-release\lib\vtkmetaio-8.2.lib
LIBS += D:\VTK\VTK-release\lib\vtkNetCDF-8.2.lib
LIBS += D:\VTK\VTK-release\lib\vtkogg-8.2.lib
LIBS += D:\VTK\VTK-release\lib\vtkParallelCore-8.2.lib
LIBS += D:\VTK\VTK-release\lib\vtkpng-8.2.lib
LIBS += D:\VTK\VTK-release\lib\vtkproj-8.2.lib
LIBS += D:\VTK\VTK-release\lib\vtkpugixml-8.2.lib
LIBS += D:\VTK\VTK-release\lib\vtkRenderingAnnotation-8.2.lib
LIBS += D:\VTK\VTK-release\lib\vtkRenderingContext2D-8.2.lib
LIBS += D:\VTK\VTK-release\lib\vtkRenderingContextOpenGL2-8.2.lib
LIBS += D:\VTK\VTK-release\lib\vtkRenderingCore-8.2.lib
LIBS += D:\VTK\VTK-release\lib\vtkRenderingFreeType-8.2.lib
LIBS += D:\VTK\VTK-release\lib\vtkRenderingGL2PSOpenGL2-8.2.lib
LIBS += D:\VTK\VTK-release\lib\vtkRenderingImage-8.2.lib
LIBS += D:\VTK\VTK-release\lib\vtkRenderingLabel-8.2.lib
LIBS += D:\VTK\VTK-release\lib\vtkRenderingLOD-8.2.lib
LIBS += D:\VTK\VTK-release\lib\vtkRenderingOpenGL2-8.2.lib
LIBS += D:\VTK\VTK-release\lib\vtkRenderingQt-8.2.lib
LIBS += D:\VTK\VTK-release\lib\vtkRenderingVolume-8.2.lib
LIBS += D:\VTK\VTK-release\lib\vtkRenderingVolumeOpenGL2-8.2.lib
LIBS += D:\VTK\VTK-release\lib\vtksqlite-8.2.lib
LIBS += D:\VTK\VTK-release\lib\vtksys-8.2.lib
LIBS += D:\VTK\VTK-release\lib\vtktheora-8.2.lib
LIBS += D:\VTK\VTK-release\lib\vtktiff-8.2.lib
LIBS += D:\VTK\VTK-release\lib\vtkverdict-8.2.lib
LIBS += D:\VTK\VTK-release\lib\vtkViewsContext2D-8.2.lib
LIBS += D:\VTK\VTK-release\lib\vtkViewsCore-8.2.lib
LIBS += D:\VTK\VTK-release\lib\vtkViewsInfovis-8.2.lib
LIBS += D:\VTK\VTK-release\lib\vtkViewsQt-8.2.lib
LIBS += D:\VTK\VTK-release\lib\vtkzlib-8.2.lib
}
else:win32:CONFIG(debug, debug|release):{
INCLUDEPATH += D:\VTK\VTK-debug\include\vtk-8.2
INCLUDEPATH += D:\VTK\VTK-debug\include\vtk-8.2\vtkhdf5
DEPENDPATH += D:\VTK\VTK-debug\include
LIBS += D:\VTK\VTK-debug\lib\vtkChartsCore-8.2D.lib
LIBS += D:\VTK\VTK-debug\lib\vtkCommonColor-8.2D.lib
LIBS += D:\VTK\VTK-debug\lib\vtkCommonComputationalGeometry-8.2D.lib
LIBS += D:\VTK\VTK-debug\lib\vtkCommonCore-8.2D.lib
LIBS += D:\VTK\VTK-debug\lib\vtkCommonDataModel-8.2D.lib
LIBS += D:\VTK\VTK-debug\lib\vtkCommonExecutionModel-8.2D.lib
LIBS += D:\VTK\VTK-debug\lib\vtkCommonMath-8.2D.lib
LIBS += D:\VTK\VTK-debug\lib\vtkCommonMisc-8.2D.lib
LIBS += D:\VTK\VTK-debug\lib\vtkCommonSystem-8.2D.lib
LIBS += D:\VTK\VTK-debug\lib\vtkCommonTransforms-8.2D.lib
LIBS += D:\VTK\VTK-debug\lib\vtkDICOMParser-8.2D.lib
LIBS += D:\VTK\VTK-debug\lib\vtkDomainsChemistry-8.2D.lib
LIBS += D:\VTK\VTK-debug\lib\vtkDomainsChemistryOpenGL2-8.2D.lib
LIBS += D:\VTK\VTK-debug\lib\vtkdoubleconversion-8.2D.lib
LIBS += D:\VTK\VTK-debug\lib\vtkexodusII-8.2D.lib
LIBS += D:\VTK\VTK-debug\lib\vtkexpat-8.2D.lib
LIBS += D:\VTK\VTK-debug\lib\vtkFiltersAMR-8.2D.lib
LIBS += D:\VTK\VTK-debug\lib\vtkFiltersCore-8.2D.lib
LIBS += D:\VTK\VTK-debug\lib\vtkFiltersExtraction-8.2D.lib
LIBS += D:\VTK\VTK-debug\lib\vtkFiltersFlowPaths-8.2D.lib
LIBS += D:\VTK\VTK-debug\lib\vtkFiltersGeneral-8.2D.lib
LIBS += D:\VTK\VTK-debug\lib\vtkFiltersGeneric-8.2D.lib
LIBS += D:\VTK\VTK-debug\lib\vtkFiltersGeometry-8.2D.lib
LIBS += D:\VTK\VTK-debug\lib\vtkFiltersHybrid-8.2D.lib
LIBS += D:\VTK\VTK-debug\lib\vtkFiltersHyperTree-8.2D.lib
LIBS += D:\VTK\VTK-debug\lib\vtkFiltersImaging-8.2D.lib
LIBS += D:\VTK\VTK-debug\lib\vtkFiltersModeling-8.2D.lib
LIBS += D:\VTK\VTK-debug\lib\vtkFiltersParallel-8.2D.lib
LIBS += D:\VTK\VTK-debug\lib\vtkFiltersParallelImaging-8.2D.lib
LIBS += D:\VTK\VTK-debug\lib\vtkFiltersPoints-8.2D.lib
LIBS += D:\VTK\VTK-debug\lib\vtkFiltersProgrammable-8.2D.lib
LIBS += D:\VTK\VTK-debug\lib\vtkFiltersSelection-8.2D.lib
LIBS += D:\VTK\VTK-debug\lib\vtkFiltersSMP-8.2D.lib
LIBS += D:\VTK\VTK-debug\lib\vtkFiltersSources-8.2D.lib
LIBS += D:\VTK\VTK-debug\lib\vtkFiltersStatistics-8.2D.lib
LIBS += D:\VTK\VTK-debug\lib\vtkFiltersTexture-8.2D.lib
LIBS += D:\VTK\VTK-debug\lib\vtkFiltersTopology-8.2D.lib
LIBS += D:\VTK\VTK-debug\lib\vtkFiltersVerdict-8.2D.lib
LIBS += D:\VTK\VTK-debug\lib\vtkfreetype-8.2D.lib
LIBS += D:\VTK\VTK-debug\lib\vtkGeovisCore-8.2D.lib
LIBS += D:\VTK\VTK-debug\lib\vtkgl2ps-8.2D.lib
LIBS += D:\VTK\VTK-debug\lib\vtkglew-8.2D.lib
LIBS += D:\VTK\VTK-debug\lib\vtkGUISupportQt-8.2D.lib
LIBS += D:\VTK\VTK-debug\lib\vtkGUISupportQtSQL-8.2D.lib
LIBS += D:\VTK\VTK-debug\lib\vtkhdf5-8.2D.lib
LIBS += D:\VTK\VTK-debug\lib\vtkhdf5_hl-8.2D.lib
LIBS += D:\VTK\VTK-debug\lib\vtkImagingColor-8.2D.lib
LIBS += D:\VTK\VTK-debug\lib\vtkImagingCore-8.2D.lib
LIBS += D:\VTK\VTK-debug\lib\vtkImagingFourier-8.2D.lib
LIBS += D:\VTK\VTK-debug\lib\vtkImagingGeneral-8.2D.lib
LIBS += D:\VTK\VTK-debug\lib\vtkImagingHybrid-8.2D.lib
LIBS += D:\VTK\VTK-debug\lib\vtkImagingMath-8.2D.lib
LIBS += D:\VTK\VTK-debug\lib\vtkImagingMorphological-8.2D.lib
LIBS += D:\VTK\VTK-debug\lib\vtkImagingSources-8.2D.lib
LIBS += D:\VTK\VTK-debug\lib\vtkImagingStatistics-8.2D.lib
LIBS += D:\VTK\VTK-debug\lib\vtkImagingStencil-8.2D.lib
LIBS += D:\VTK\VTK-debug\lib\vtkInfovisCore-8.2D.lib
LIBS += D:\VTK\VTK-debug\lib\vtkInfovisLayout-8.2D.lib
LIBS += D:\VTK\VTK-debug\lib\vtkInteractionImage-8.2D.lib
LIBS += D:\VTK\VTK-debug\lib\vtkInteractionStyle-8.2D.lib
LIBS += D:\VTK\VTK-debug\lib\vtkInteractionWidgets-8.2D.lib
LIBS += D:\VTK\VTK-debug\lib\vtkIOAMR-8.2D.lib
LIBS += D:\VTK\VTK-debug\lib\vtkIOAsynchronous-8.2D.lib
LIBS += D:\VTK\VTK-debug\lib\vtkIOCityGML-8.2D.lib
LIBS += D:\VTK\VTK-debug\lib\vtkIOCore-8.2D.lib
LIBS += D:\VTK\VTK-debug\lib\vtkIOEnSight-8.2D.lib
LIBS += D:\VTK\VTK-debug\lib\vtkIOExodus-8.2D.lib
LIBS += D:\VTK\VTK-debug\lib\vtkIOExport-8.2D.lib
LIBS += D:\VTK\VTK-debug\lib\vtkIOExportOpenGL2-8.2D.lib
LIBS += D:\VTK\VTK-debug\lib\vtkIOExportPDF-8.2D.lib
LIBS += D:\VTK\VTK-debug\lib\vtkIOGeometry-8.2D.lib
LIBS += D:\VTK\VTK-debug\lib\vtkIOImage-8.2D.lib
LIBS += D:\VTK\VTK-debug\lib\vtkIOImport-8.2D.lib
LIBS += D:\VTK\VTK-debug\lib\vtkIOInfovis-8.2D.lib
LIBS += D:\VTK\VTK-debug\lib\vtkIOLegacy-8.2D.lib
LIBS += D:\VTK\VTK-debug\lib\vtkIOLSDyna-8.2D.lib
LIBS += D:\VTK\VTK-debug\lib\vtkIOMINC-8.2D.lib
LIBS += D:\VTK\VTK-debug\lib\vtkIOMovie-8.2D.lib
LIBS += D:\VTK\VTK-debug\lib\vtkIONetCDF-8.2D.lib
LIBS += D:\VTK\VTK-debug\lib\vtkIOParallel-8.2D.lib
LIBS += D:\VTK\VTK-debug\lib\vtkIOParallelXML-8.2D.lib
LIBS += D:\VTK\VTK-debug\lib\vtkIOPLY-8.2D.lib
LIBS += D:\VTK\VTK-debug\lib\vtkIOSegY-8.2D.lib
LIBS += D:\VTK\VTK-debug\lib\vtkIOSQL-8.2D.lib
LIBS += D:\VTK\VTK-debug\lib\vtkIOTecplotTable-8.2D.lib
LIBS += D:\VTK\VTK-debug\lib\vtkIOVeraOut-8.2D.lib
LIBS += D:\VTK\VTK-debug\lib\vtkIOVideo-8.2D.lib
LIBS += D:\VTK\VTK-debug\lib\vtkIOXML-8.2D.lib
LIBS += D:\VTK\VTK-debug\lib\vtkIOXMLParser-8.2D.lib
LIBS += D:\VTK\VTK-debug\lib\vtkjpeg-8.2D.lib
LIBS += D:\VTK\VTK-debug\lib\vtkjsoncpp-8.2D.lib
LIBS += D:\VTK\VTK-debug\lib\vtklibharu-8.2D.lib
LIBS += D:\VTK\VTK-debug\lib\vtklibxml2-8.2D.lib
LIBS += D:\VTK\VTK-debug\lib\vtklz4-8.2D.lib
LIBS += D:\VTK\VTK-debug\lib\vtklzma-8.2D.lib
LIBS += D:\VTK\VTK-debug\lib\vtkmetaio-8.2D.lib
LIBS += D:\VTK\VTK-debug\lib\vtkNetCDF-8.2D.lib
LIBS += D:\VTK\VTK-debug\lib\vtkogg-8.2D.lib
LIBS += D:\VTK\VTK-debug\lib\vtkParallelCore-8.2D.lib
LIBS += D:\VTK\VTK-debug\lib\vtkpng-8.2D.lib
LIBS += D:\VTK\VTK-debug\lib\vtkproj-8.2D.lib
LIBS += D:\VTK\VTK-debug\lib\vtkpugixml-8.2D.lib
LIBS += D:\VTK\VTK-debug\lib\vtkRenderingAnnotation-8.2D.lib
LIBS += D:\VTK\VTK-debug\lib\vtkRenderingContext2D-8.2D.lib
LIBS += D:\VTK\VTK-debug\lib\vtkRenderingContextOpenGL2-8.2D.lib
LIBS += D:\VTK\VTK-debug\lib\vtkRenderingCore-8.2D.lib
LIBS += D:\VTK\VTK-debug\lib\vtkRenderingFreeType-8.2D.lib
LIBS += D:\VTK\VTK-debug\lib\vtkRenderingGL2PSOpenGL2-8.2D.lib
LIBS += D:\VTK\VTK-debug\lib\vtkRenderingImage-8.2D.lib
LIBS += D:\VTK\VTK-debug\lib\vtkRenderingLabel-8.2D.lib
LIBS += D:\VTK\VTK-debug\lib\vtkRenderingLOD-8.2D.lib
LIBS += D:\VTK\VTK-debug\lib\vtkRenderingOpenGL2-8.2D.lib
LIBS += D:\VTK\VTK-debug\lib\vtkRenderingQt-8.2D.lib
LIBS += D:\VTK\VTK-debug\lib\vtkRenderingVolume-8.2D.lib
LIBS += D:\VTK\VTK-debug\lib\vtkRenderingVolumeOpenGL2-8.2D.lib
LIBS += D:\VTK\VTK-debug\lib\vtksqlite-8.2D.lib
LIBS += D:\VTK\VTK-debug\lib\vtksys-8.2D.lib
LIBS += D:\VTK\VTK-debug\lib\vtktheora-8.2D.lib
LIBS += D:\VTK\VTK-debug\lib\vtktiff-8.2D.lib
LIBS += D:\VTK\VTK-debug\lib\vtkverdict-8.2D.lib
LIBS += D:\VTK\VTK-debug\lib\vtkViewsContext2D-8.2D.lib
LIBS += D:\VTK\VTK-debug\lib\vtkViewsCore-8.2D.lib
LIBS += D:\VTK\VTK-debug\lib\vtkViewsInfovis-8.2D.lib
LIBS += D:\VTK\VTK-debug\lib\vtkViewsQt-8.2D.lib
LIBS += D:\VTK\VTK-debug\lib\vtkzlib-8.2D.lib
}

RESOURCES += \
    resource.qrc
