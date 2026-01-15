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
                                  QT += widgets opengl

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
else:unix {
VTK_ROOT = $$PWD/lib/VTK/VTK-Linux-debug
VTK_INCLUDE_DIR = $$VTK_ROOT/include/vtk-8.2
VTK_LIB_DIR     = $$VTK_ROOT/lib
INCLUDEPATH += $$VTK_INCLUDE_DIR
DEPENDPATH += $$VTK_INCLUDE_DIR
LIBS += -L$$VTK_LIB_DIR
LIBS += -lvtkChartsCore-8.2
LIBS += -lvtkCommonCore-8.2
LIBS += -lvtkCommonDataModel-8.2
LIBS += -lvtkChartsCore-8.2
LIBS += -lvtkCommonColor-8.2
LIBS += -lvtkCommonComputationalGeometry-8.2
LIBS += -lvtkCommonCore-8.2
LIBS += -lvtkCommonDataModel-8.2
LIBS += -lvtkCommonExecutionModel-8.2
LIBS += -lvtkCommonMath-8.2
LIBS += -lvtkCommonMisc-8.2
LIBS += -lvtkCommonSystem-8.2
LIBS += -lvtkCommonTransforms-8.2
LIBS += -lvtkDICOMParser-8.2
LIBS += -lvtkDomainsChemistry-8.2
LIBS += -lvtkDomainsChemistryOpenGL2-8.2
LIBS += -lvtkdoubleconversion-8.2
LIBS += -lvtkexodusII-8.2
LIBS += -lvtkexpat-8.2
LIBS += -lvtkFiltersAMR-8.2
LIBS += -lvtkFiltersCore-8.2
LIBS += -lvtkFiltersExtraction-8.2
LIBS += -lvtkFiltersFlowPaths-8.2
LIBS += -lvtkFiltersGeneral-8.2
LIBS += -lvtkFiltersGeneric-8.2
LIBS += -lvtkFiltersGeometry-8.2
LIBS += -lvtkFiltersHybrid-8.2
LIBS += -lvtkFiltersHyperTree-8.2
LIBS += -lvtkFiltersImaging-8.2
LIBS += -lvtkFiltersModeling-8.2
LIBS += -lvtkFiltersParallel-8.2
LIBS += -lvtkFiltersParallelImaging-8.2
LIBS += -lvtkFiltersPoints-8.2
LIBS += -lvtkFiltersProgrammable-8.2
LIBS += -lvtkFiltersSelection-8.2
LIBS += -lvtkFiltersSMP-8.2
LIBS += -lvtkFiltersSources-8.2
LIBS += -lvtkFiltersStatistics-8.2
LIBS += -lvtkFiltersTexture-8.2
LIBS += -lvtkFiltersTopology-8.2
LIBS += -lvtkFiltersVerdict-8.2
LIBS += -lvtkfreetype-8.2
LIBS += -lvtkGeovisCore-8.2
LIBS += -lvtkgl2ps-8.2
LIBS += -lvtkglew-8.2
LIBS += -lvtkGUISupportQt-8.2
LIBS += -lvtkGUISupportQtSQL-8.2
LIBS += -lvtkhdf5-8.2
LIBS += -lvtkhdf5_hl-8.2
LIBS += -lvtkImagingColor-8.2
LIBS += -lvtkImagingCore-8.2
LIBS += -lvtkImagingFourier-8.2
LIBS += -lvtkImagingGeneral-8.2
LIBS += -lvtkImagingHybrid-8.2
LIBS += -lvtkImagingMath-8.2
LIBS += -lvtkImagingMorphological-8.2
LIBS += -lvtkImagingSources-8.2
LIBS += -lvtkImagingStatistics-8.2
LIBS += -lvtkImagingStencil-8.2
LIBS += -lvtkInfovisCore-8.2
LIBS += -lvtkInfovisLayout-8.2
LIBS += -lvtkInteractionImage-8.2
LIBS += -lvtkInteractionStyle-8.2
LIBS += -lvtkInteractionWidgets-8.2
LIBS += -lvtkIOAMR-8.2
LIBS += -lvtkIOAsynchronous-8.2
LIBS += -lvtkIOCityGML-8.2
LIBS += -lvtkIOCore-8.2
LIBS += -lvtkIOEnSight-8.2
LIBS += -lvtkIOExodus-8.2
LIBS += -lvtkIOExport-8.2
LIBS += -lvtkIOExportOpenGL2-8.2
LIBS += -lvtkIOExportPDF-8.2
LIBS += -lvtkIOGeometry-8.2
LIBS += -lvtkIOImage-8.2
LIBS += -lvtkIOImport-8.2
LIBS += -lvtkIOInfovis-8.2
LIBS += -lvtkIOLegacy-8.2
LIBS += -lvtkIOLSDyna-8.2
LIBS += -lvtkIOMINC-8.2
LIBS += -lvtkIOMovie-8.2
LIBS += -lvtkIONetCDF-8.2
LIBS += -lvtkIOParallel-8.2
LIBS += -lvtkIOParallelXML-8.2
LIBS += -lvtkIOPLY-8.2
LIBS += -lvtkIOSegY-8.2
LIBS += -lvtkIOSQL-8.2
LIBS += -lvtkIOTecplotTable-8.2
LIBS += -lvtkIOVeraOut-8.2
LIBS += -lvtkIOVideo-8.2
LIBS += -lvtkIOXML-8.2
LIBS += -lvtkIOXMLParser-8.2
LIBS += -lvtkjpeg-8.2
LIBS += -lvtkjsoncpp-8.2
LIBS += -lvtklibharu-8.2
LIBS += -lvtklz4-8.2
LIBS += -lvtklzma-8.2
LIBS += -lvtkmetaio-8.2
LIBS += -lvtkNetCDF-8.2
LIBS += -lvtkogg-8.2
LIBS += -lvtkParallelCore-8.2
LIBS += -lvtkpng-8.2
LIBS += -lvtkproj-8.2
LIBS += -lvtkpugixml-8.2
LIBS += -lvtkRenderingAnnotation-8.2
LIBS += -lvtkRenderingContext2D-8.2
LIBS += -lvtkRenderingContextOpenGL2-8.2
LIBS += -lvtkRenderingCore-8.2
LIBS += -lvtkRenderingFreeType-8.2
LIBS += -lvtkRenderingGL2PSOpenGL2-8.2
LIBS += -lvtkRenderingImage-8.2
LIBS += -lvtkRenderingLabel-8.2
LIBS += -lvtkRenderingLOD-8.2
LIBS += -lvtkRenderingOpenGL2-8.2
LIBS += -lvtkRenderingQt-8.2
LIBS += -lvtkRenderingVolume-8.2
LIBS += -lvtkRenderingVolumeOpenGL2-8.2
LIBS += -lvtksqlite-8.2
LIBS += -lvtksys-8.2
LIBS += -lvtktheora-8.2
LIBS += -lvtktiff-8.2
LIBS += -lvtkverdict-8.2
LIBS += -lvtkViewsContext2D-8.2
LIBS += -lvtkViewsCore-8.2
LIBS += -lvtkViewsInfovis-8.2
LIBS += -lvtkViewsQt-8.2
LIBS += -lvtkzlib-8.2
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


