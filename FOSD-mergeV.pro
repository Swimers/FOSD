#-------------------------------------------------
#
# Project created by QtCreator 2024-03-11T20:40:57
#
#-------------------------------------------------

QT       += core gui network
QT += serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = FOSD-mergeV
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

INCLUDEPATH += ./demo \
               ./disp2d \
               ./disp3d \
               ./TEST \
               ./MATH \
               ./bin

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    demo/demowindow.cpp \
    disp2d/disp2dwindow.cpp \
    logwidget.cpp \
    demo/qcustomplot.cpp \
    demo/comthread.cpp \
    demo/qcpdocumentobject.cpp \
    demo/recedatethread.cpp \
    TEST/Timer.cpp \
    MATH/IFE.cpp \
    disp3d/disp3dmainwindow.cpp \
    disp3d/displaymodel.cpp \
    disp3d/corddisplayset.cpp \
    disp3d/selectdisplaynode.cpp \
    disp3d/picker.cpp \
    disp3d/renderdermanager.cpp \
    disp3d/modelcalculationthread.cpp \
    global.cpp \
    usermanangerwidget.cpp \
    demo/pipemonitorwindow.cpp \
    disp2d/pararevisewindow.cpp

HEADERS += \
        mainwindow.h \
    demo/demowindow.h \
    disp2d/disp2dwindow.h \
    logwidget.h \
    demo/qcustomplot.h \
    demo/comthread.h \
    demo/qcpdocumentobject.h \
    demo/recedatethread.h \
    TEST/Timer.h \
    MATH/IFE.h \
    disp3d/selfdefinedata.h \
    disp3d/disp3dmainwindow.h \
    disp3d/displaymodel.h \
    disp3d/corddisplayset.h \
    disp3d/selectdisplaynode.h \
    disp3d/picker.h \
    disp3d/renderdermanager.h \
    disp3d/modelcalculationthread.h \
    global.h \
    usermanangerwidget.h \
    demo/pipemonitorwindow.h \
    disp2d/pararevisewindow.h

FORMS += \
        mainwindow.ui \
    demo/demowindow.ui \
    disp2d/disp2dwindow.ui \
    logwidget.ui \
    disp3d/disp3dmainwindow.ui \
    disp3d/corddisplayset.ui \
    disp3d/selectdisplaynode.ui \
    usermanangerwidget.ui \
    demo/pipemonitorwindow.ui \
    disp2d/pararevisewindow.ui



#LIBS += C:\ProgramFiles(x86)\WindowsKits\10\Lib\10.0.17763.0\um\x64\User32.Lib

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    images.qrc

win32:CONFIG(release, debug|release):{
#D:\VTK\VTK-release
INCLUDEPATH += D:\VTK\VTK-release\include\vtk-8.2
INCLUDEPATH += D:\VTK\VTK-release\include\vtk-8.2\vtkhdf5
DEPENDPATH += $$PWD/LIB/release/include
LIBS += D:\VTK\VTK-release\lib\vtkChartsCore-8.2.lib
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
INCLUDEPATH += $$PWD/LIB/debug/include
DEPENDPATH += $$PWD/LIB/debug/include
LIBS += $$PWD\LIB\debug\lib\vtkChartsCore-8.2.lib
LIBS += $$PWD\LIB\debug\lib\vtkCommonColor-8.2.lib
LIBS += $$PWD\LIB\debug\lib\vtkCommonComputationalGeometry-8.2.lib
LIBS += $$PWD\LIB\debug\lib\vtkCommonCore-8.2.lib
LIBS += $$PWD\LIB\debug\lib\vtkCommonDataModel-8.2.lib
LIBS += $$PWD\LIB\debug\lib\vtkCommonExecutionModel-8.2.lib
LIBS += $$PWD\LIB\debug\lib\vtkCommonMath-8.2.lib
LIBS += $$PWD\LIB\debug\lib\vtkCommonMisc-8.2.lib
LIBS += $$PWD\LIB\debug\lib\vtkCommonSystem-8.2.lib
LIBS += $$PWD\LIB\debug\lib\vtkCommonTransforms-8.2.lib
LIBS += $$PWD\LIB\debug\lib\vtkDICOMParser-8.2.lib
LIBS += $$PWD\LIB\debug\lib\vtkDomainsChemistry-8.2.lib
LIBS += $$PWD\LIB\debug\lib\vtkDomainsChemistryOpenGL2-8.2.lib
LIBS += $$PWD\LIB\debug\lib\vtkdoubleconversion-8.2.lib
LIBS += $$PWD\LIB\debug\lib\vtkexodusII-8.2.lib
LIBS += $$PWD\LIB\debug\lib\vtkexpat-8.2.lib
LIBS += $$PWD\LIB\debug\lib\vtkFiltersAMR-8.2.lib
LIBS += $$PWD\LIB\debug\lib\vtkFiltersCore-8.2.lib
LIBS += $$PWD\LIB\debug\lib\vtkFiltersExtraction-8.2.lib
LIBS += $$PWD\LIB\debug\lib\vtkFiltersFlowPaths-8.2.lib
LIBS += $$PWD\LIB\debug\lib\vtkFiltersGeneral-8.2.lib
LIBS += $$PWD\LIB\debug\lib\vtkFiltersGeneric-8.2.lib
LIBS += $$PWD\LIB\debug\lib\vtkFiltersGeometry-8.2.lib
LIBS += $$PWD\LIB\debug\lib\vtkFiltersHybrid-8.2.lib
LIBS += $$PWD\LIB\debug\lib\vtkFiltersHyperTree-8.2.lib
LIBS += $$PWD\LIB\debug\lib\vtkFiltersImaging-8.2.lib
LIBS += $$PWD\LIB\debug\lib\vtkFiltersModeling-8.2.lib
LIBS += $$PWD\LIB\debug\lib\vtkFiltersParallel-8.2.lib
LIBS += $$PWD\LIB\debug\lib\vtkFiltersParallelImaging-8.2.lib
LIBS += $$PWD\LIB\debug\lib\vtkFiltersPoints-8.2.lib
LIBS += $$PWD\LIB\debug\lib\vtkFiltersProgrammable-8.2.lib
LIBS += $$PWD\LIB\debug\lib\vtkFiltersSelection-8.2.lib
LIBS += $$PWD\LIB\debug\lib\vtkFiltersSMP-8.2.lib
LIBS += $$PWD\LIB\debug\lib\vtkFiltersSources-8.2.lib
LIBS += $$PWD\LIB\debug\lib\vtkFiltersStatistics-8.2.lib
LIBS += $$PWD\LIB\debug\lib\vtkFiltersTexture-8.2.lib
LIBS += $$PWD\LIB\debug\lib\vtkFiltersTopology-8.2.lib
LIBS += $$PWD\LIB\debug\lib\vtkFiltersVerdict-8.2.lib
LIBS += $$PWD\LIB\debug\lib\vtkfreetype-8.2.lib
LIBS += $$PWD\LIB\debug\lib\vtkGeovisCore-8.2.lib
LIBS += $$PWD\LIB\debug\lib\vtkgl2ps-8.2.lib
LIBS += $$PWD\LIB\debug\lib\vtkglew-8.2.lib
LIBS += $$PWD\LIB\debug\lib\vtkGUISupportQt-8.2.lib
LIBS += $$PWD\LIB\debug\lib\vtkGUISupportQtSQL-8.2.lib
LIBS += $$PWD\LIB\debug\lib\vtkhdf5-8.2.lib
LIBS += $$PWD\LIB\debug\lib\vtkhdf5_hl-8.2.lib
LIBS += $$PWD\LIB\debug\lib\vtkImagingColor-8.2.lib
LIBS += $$PWD\LIB\debug\lib\vtkImagingCore-8.2.lib
LIBS += $$PWD\LIB\debug\lib\vtkImagingFourier-8.2.lib
LIBS += $$PWD\LIB\debug\lib\vtkImagingGeneral-8.2.lib
LIBS += $$PWD\LIB\debug\lib\vtkImagingHybrid-8.2.lib
LIBS += $$PWD\LIB\debug\lib\vtkImagingMath-8.2.lib
LIBS += $$PWD\LIB\debug\lib\vtkImagingMorphological-8.2.lib
LIBS += $$PWD\LIB\debug\lib\vtkImagingSources-8.2.lib
LIBS += $$PWD\LIB\debug\lib\vtkImagingStatistics-8.2.lib
LIBS += $$PWD\LIB\debug\lib\vtkImagingStencil-8.2.lib
LIBS += $$PWD\LIB\debug\lib\vtkInfovisCore-8.2.lib
LIBS += $$PWD\LIB\debug\lib\vtkInfovisLayout-8.2.lib
LIBS += $$PWD\LIB\debug\lib\vtkInteractionImage-8.2.lib
LIBS += $$PWD\LIB\debug\lib\vtkInteractionStyle-8.2.lib
LIBS += $$PWD\LIB\debug\lib\vtkInteractionWidgets-8.2.lib
LIBS += $$PWD\LIB\debug\lib\vtkIOAMR-8.2.lib
LIBS += $$PWD\LIB\debug\lib\vtkIOAsynchronous-8.2.lib
LIBS += $$PWD\LIB\debug\lib\vtkIOCityGML-8.2.lib
LIBS += $$PWD\LIB\debug\lib\vtkIOCore-8.2.lib
LIBS += $$PWD\LIB\debug\lib\vtkIOEnSight-8.2.lib
LIBS += $$PWD\LIB\debug\lib\vtkIOExodus-8.2.lib
LIBS += $$PWD\LIB\debug\lib\vtkIOExport-8.2.lib
LIBS += $$PWD\LIB\debug\lib\vtkIOExportOpenGL2-8.2.lib
LIBS += $$PWD\LIB\debug\lib\vtkIOExportPDF-8.2.lib
LIBS += $$PWD\LIB\debug\lib\vtkIOGeometry-8.2.lib
LIBS += $$PWD\LIB\debug\lib\vtkIOImage-8.2.lib
LIBS += $$PWD\LIB\debug\lib\vtkIOImport-8.2.lib
LIBS += $$PWD\LIB\debug\lib\vtkIOInfovis-8.2.lib
LIBS += $$PWD\LIB\debug\lib\vtkIOLegacy-8.2.lib
LIBS += $$PWD\LIB\debug\lib\vtkIOLSDyna-8.2.lib
LIBS += $$PWD\LIB\debug\lib\vtkIOMINC-8.2.lib
LIBS += $$PWD\LIB\debug\lib\vtkIOMovie-8.2.lib
LIBS += $$PWD\LIB\debug\lib\vtkIONetCDF-8.2.lib
LIBS += $$PWD\LIB\debug\lib\vtkIOParallel-8.2.lib
LIBS += $$PWD\LIB\debug\lib\vtkIOParallelXML-8.2.lib
LIBS += $$PWD\LIB\debug\lib\vtkIOPLY-8.2.lib
LIBS += $$PWD\LIB\debug\lib\vtkIOSegY-8.2.lib
LIBS += $$PWD\LIB\debug\lib\vtkIOSQL-8.2.lib
LIBS += $$PWD\LIB\debug\lib\vtkIOTecplotTable-8.2.lib
LIBS += $$PWD\LIB\debug\lib\vtkIOVeraOut-8.2.lib
LIBS += $$PWD\LIB\debug\lib\vtkIOVideo-8.2.lib
LIBS += $$PWD\LIB\debug\lib\vtkIOXML-8.2.lib
LIBS += $$PWD\LIB\debug\lib\vtkIOXMLParser-8.2.lib
LIBS += $$PWD\LIB\debug\lib\vtkjpeg-8.2.lib
LIBS += $$PWD\LIB\debug\lib\vtkjsoncpp-8.2.lib
LIBS += $$PWD\LIB\debug\lib\vtklibharu-8.2.lib
LIBS += $$PWD\LIB\debug\lib\vtklibxml2-8.2.lib
LIBS += $$PWD\LIB\debug\lib\vtklz4-8.2.lib
LIBS += $$PWD\LIB\debug\lib\vtklzma-8.2.lib
LIBS += $$PWD\LIB\debug\lib\vtkmetaio-8.2.lib
LIBS += $$PWD\LIB\debug\lib\vtkNetCDF-8.2.lib
LIBS += $$PWD\LIB\debug\lib\vtkogg-8.2.lib
LIBS += $$PWD\LIB\debug\lib\vtkParallelCore-8.2.lib
LIBS += $$PWD\LIB\debug\lib\vtkpng-8.2.lib
LIBS += $$PWD\LIB\debug\lib\vtkproj-8.2.lib
LIBS += $$PWD\LIB\debug\lib\vtkpugixml-8.2.lib
LIBS += $$PWD\LIB\debug\lib\vtkRenderingAnnotation-8.2.lib
LIBS += $$PWD\LIB\debug\lib\vtkRenderingContext2D-8.2.lib
LIBS += $$PWD\LIB\debug\lib\vtkRenderingContextOpenGL2-8.2.lib
LIBS += $$PWD\LIB\debug\lib\vtkRenderingCore-8.2.lib
LIBS += $$PWD\LIB\debug\lib\vtkRenderingFreeType-8.2.lib
LIBS += $$PWD\LIB\debug\lib\vtkRenderingGL2PSOpenGL2-8.2.lib
LIBS += $$PWD\LIB\debug\lib\vtkRenderingImage-8.2.lib
LIBS += $$PWD\LIB\debug\lib\vtkRenderingLabel-8.2.lib
LIBS += $$PWD\LIB\debug\lib\vtkRenderingLOD-8.2.lib
LIBS += $$PWD\LIB\debug\lib\vtkRenderingOpenGL2-8.2.lib
LIBS += $$PWD\LIB\debug\lib\vtkRenderingQt-8.2.lib
LIBS += $$PWD\LIB\debug\lib\vtkRenderingVolume-8.2.lib
LIBS += $$PWD\LIB\debug\lib\vtkRenderingVolumeOpenGL2-8.2.lib
LIBS += $$PWD\LIB\debug\lib\vtksqlite-8.2.lib
LIBS += $$PWD\LIB\debug\lib\vtksys-8.2.lib
LIBS += $$PWD\LIB\debug\lib\vtktheora-8.2.lib
LIBS += $$PWD\LIB\debug\lib\vtktiff-8.2.lib
LIBS += $$PWD\LIB\debug\lib\vtkverdict-8.2.lib
LIBS += $$PWD\LIB\debug\lib\vtkViewsContext2D-8.2.lib
LIBS += $$PWD\LIB\debug\lib\vtkViewsCore-8.2.lib
LIBS += $$PWD\LIB\debug\lib\vtkViewsInfovis-8.2.lib
LIBS += $$PWD\LIB\debug\lib\vtkViewsQt-8.2.lib
LIBS += $$PWD\LIB\debug\lib\vtkzlib-8.2.lib

}


