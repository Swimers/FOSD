#-------------------------------------------------
#
# Project created by QtCreator 2025-06-26T11:13:38
#
#-------------------------------------------------

QT       += core gui
QT       += opengl

win32 {
    SYSTEM_NAME = windows
}
unix:!macx {
    QT += waylandcompositor
    LIBS += -lwayland-server -lxkbcommon
}
macx {
    SYSTEM_NAME = macos
}

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

INCLUDEPATH +=  src/main \
                src/test \
                src/app/conncect \
                src/app/disp2d \
                src/app/disp3d \
                src/app/disp3d/docks \
                src/app/mainwindow \
                src/app/user \
                src/communication \
                src/core/IFE \
                src/core/RainbowIFE \
                src/core/model \
                src/protocol \
                src/utils/database \
                src/utils/global \
                src/utils/QCustomPlot \
                src/utils/Timers \
                src/utils/Storage \
                src/utils/config \
                lib/CustomUi \


FORMS += \
    src/app/conncect/connectdebug.ui \
    src/app/conncect/connectwindow.ui \
    src/app/disp2d/disp2dpara.ui \
    src/app/disp2d/disp2dwindow.ui \
    src/app/disp3d/disp3dkeypointedit.ui \
    src/app/disp3d/disp3dkeypointselect.ui \
    src/app/disp3d/docks/disp3dinfodockpanel.ui \
    src/app/disp3d/docks/disp3dmodeldockpanel.ui \
    src/app/disp3d/docks/disp3dviewdockpanel.ui \
    src/app/disp3d/disp3dwindow.ui \
    src/app/mainwindow/oldmainwindow.ui \
    src/app/user/login.ui \
    src/app/disp3d/trackingpointmanager.ui \
    src/app/disp3d/trackingpointdatatable.ui \
    src/app/mainwindow/rainbowmainwindow.ui \
    src/app/disp3d/dispdata.ui

HEADERS += \
    src/app/conncect/connectdebug.h \
    src/app/conncect/connectwindow.h \
    src/app/disp2d/disp2dpara.h \
    src/app/disp2d/disp2dwindow.h \
    src/app/disp3d/disp3dkeypointedit.h \
    src/app/disp3d/disp3dkeypointselect.h \
    src/app/disp3d/docks/disp3ddockmanager.h \
    src/app/disp3d/docks/disp3ddockpanel.h \
    src/app/disp3d/docks/disp3dinfodockpanel.h \
    src/app/disp3d/docks/disp3dmodeldockpanel.h \
    src/app/disp3d/docks/disp3dviewdockpanel.h \
    src/app/disp3d/disp3dwindow.h \
    src/app/mainwindow/oldmainwindow.h \
    src/app/user/login.h \
    src/communication/serialtask.h \
    src/communication/tcptask.h \
    src/communication/udptask.h \
    src/core/IFE/IFE.h \
    src/protocol/analyzepackets.h \
    src/protocol/pocketsconfig.h \
    src/utils/database/database.h \
    src/utils/global/global.h \
    src/utils/QCustomPlot/qcpdocumentobject.h \
    src/utils/QCustomPlot/qcustomplot.h \
    src/utils/Timers/Timer.h \
    src/utils/config/AppConfig.h \
    src/utils/config/ConfigManager.h \
    lib/CustomUi/switchcontrol.h \
    src/core/model/disp3dbackground.h \
    src/core/model/disp3dfreshmodelthread.h \
    src/core/model/disp3dglobal.h \
    src/core/model/disp3dinteractor.h \
    src/core/model/disp3dmodel.h \
    src/core/model/trackingpoint.h \
    src/app/disp3d/trackingpointmanager.h \
    src/app/disp3d/trackingpointdatatable.h \
    src/core/RainbowIFE/rainbowife.h \
    src/core/RainbowIFE/rainbowloadcalibration.h \
    src/utils/Storage/DataStorageThread.h \
    src/app/mainwindow/rainbowmainwindow.h \
    src/test/teststorge.h \
    src/app/disp3d/dispdata.h \
    src/test/testdatainjector.h \
    src/test/testsavepointsdisp.h

SOURCES += \
    src/app/conncect/connectdebug.cpp \
    src/app/conncect/connectwindow.cpp \
    src/app/disp2d/disp2dpara.cpp \
    src/app/disp2d/disp2dwindow.cpp \
    src/app/disp3d/disp3dkeypointedit.cpp \
    src/app/disp3d/disp3dkeypointselect.cpp \
    src/app/disp3d/docks/disp3ddockmanager.cpp \
    src/app/disp3d/docks/disp3dinfodockpanel.cpp \
    src/app/disp3d/docks/disp3dmodeldockpanel.cpp \
    src/app/disp3d/docks/disp3dviewdockpanel.cpp \
    src/app/disp3d/disp3dwindow.cpp \
    src/app/mainwindow/oldmainwindow.cpp \
    src/app/user/login.cpp \
    src/communication/serialtask.cpp \
    src/communication/tcptask.cpp \
    src/communication/udptask.cpp \
    src/core/IFE/IFE.cpp \
    src/protocol/analyzepackets.cpp \
    src/utils/database/database.cpp \
    src/utils/global/global.cpp \
    src/utils/QCustomPlot/qcpdocumentobject.cpp \
    src/utils/QCustomPlot/qcustomplot.cpp \
    src/utils/config/ConfigManager.cpp \
    src/main.cpp \
    lib/CustomUi/switchcontrol.cpp \
    src/core/model/disp3dbackground.cpp \
    src/core/model/disp3dfreshmodelthread.cpp \
    src/core/model/disp3dinteractor.cpp \
    src/core/model/disp3dmodel.cpp \
    src/core/model/trackingpoint.cpp \
    src/app/disp3d/trackingpointmanager.cpp \
    src/app/disp3d/trackingpointdatatable.cpp \
    src/core/RainbowIFE/rainbowife.cpp \
    src/core/RainbowIFE/rainbowloadcalibration.cpp \
    src/utils/Storage/DataStorageThread.cpp \
    src/app/mainwindow/rainbowmainwindow.cpp \
    src/fosdplugin.cpp \
    src/main.cpp \
    src/test/teststorge.cpp \
    src/app/disp3d/dispdata.cpp \
    src/test/testdatainjector.cpp \
    src/test/testsavepointsdisp.cpp

RESOURCES += \
    $$PWD/res/resource.qrc \

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32:CONFIG(release, debug|release):{
VTK_DIR = $$clean_path($$PWD/lib/VTK/VTK-release)
INCLUDEPATH += $$VTK_DIR/include/vtk-8.2
LIBS += -L$$VTK_DIR/lib
# ---- Common ----
LIBS += $$VTK_DIR/lib/vtkCommonCore-8.2.lib
LIBS += $$VTK_DIR/lib/vtkCommonDataModel-8.2.lib
LIBS += $$VTK_DIR/lib/vtkCommonMath-8.2.lib
LIBS += $$VTK_DIR/lib/vtkCommonMisc-8.2.lib
LIBS += $$VTK_DIR/lib/vtkCommonSystem-8.2.lib
LIBS += $$VTK_DIR/lib/vtkCommonTransforms-8.2.lib
LIBS += $$VTK_DIR/lib/vtksys-8.2.lib
# ---- Filters ----
LIBS += $$VTK_DIR/lib/vtkFiltersCore-8.2.lib
LIBS += $$VTK_DIR/lib/vtkFiltersGeneral-8.2.lib
LIBS += $$VTK_DIR/lib/vtkFiltersSources-8.2.lib
# ---- Rendering ----
LIBS += $$VTK_DIR/lib/vtkRenderingCore-8.2.lib
LIBS += $$VTK_DIR/lib/vtkRenderingOpenGL2-8.2.lib
LIBS += $$VTK_DIR/lib/vtkRenderingFreeType-8.2.lib
LIBS += $$VTK_DIR/lib/vtkRenderingAnnotation-8.2.lib
LIBS += $$VTK_DIR/lib/vtkRenderingQt-8.2.lib
LIBS += $$VTK_DIR/lib/vtkfreetype-8.2.lib
LIBS += $$VTK_DIR/lib/vtkglew-8.2.lib
# ---- Interaction ----
LIBS += $$VTK_DIR/lib/vtkInteractionStyle-8.2.lib
LIBS += $$VTK_DIR/lib/vtkInteractionWidgets-8.2.lib
# ---- IO ----
LIBS += $$VTK_DIR/lib/vtkIOCore-8.2.lib
LIBS += $$VTK_DIR/lib/vtkIOGeometry-8.2.lib
LIBS += $$VTK_DIR/lib/vtkIOImage-8.2.lib
LIBS += $$VTK_DIR/lib/vtkImagingCore-8.2.lib
LIBS += $$VTK_DIR/lib/vtkjpeg-8.2.lib
LIBS += $$VTK_DIR/lib/vtkpng-8.2.lib
LIBS += $$VTK_DIR/lib/vtktiff-8.2.lib
LIBS += $$VTK_DIR/lib/vtkmetaio-8.2.lib
LIBS += $$VTK_DIR/lib/vtkDICOMParser-8.2.lib
LIBS += $$VTK_DIR/lib/vtklz4-8.2.lib
LIBS += $$VTK_DIR/lib/vtklzma-8.2.lib
LIBS += $$VTK_DIR/lib/vtkzlib-8.2.lib
# ---- Qt Integration ----
LIBS += $$VTK_DIR/lib/vtkGUISupportQt-8.2.lib
}
else:win32:CONFIG(debug, debug|release):{
VTK_DIR = $$clean_path($$PWD/lib/VTK/VTK-debug)
INCLUDEPATH += $$VTK_DIR/include/vtk-8.2
LIBS += -L$$VTK_DIR/lib
# ---- Common ----
LIBS += $$VTK_DIR/lib/vtkCommonCore-8.2D.lib
LIBS += $$VTK_DIR/lib/vtkCommonDataModel-8.2D.lib
LIBS += $$VTK_DIR/lib/vtkCommonMath-8.2D.lib
LIBS += $$VTK_DIR/lib/vtkCommonMisc-8.2D.lib
LIBS += $$VTK_DIR/lib/vtkCommonSystem-8.2D.lib
LIBS += $$VTK_DIR/lib/vtkCommonTransforms-8.2D.lib
LIBS += $$VTK_DIR/lib/vtksys-8.2D.lib
# ---- Filters ----
LIBS += $$VTK_DIR/lib/vtkFiltersCore-8.2D.lib
LIBS += $$VTK_DIR/lib/vtkFiltersGeneral-8.2D.lib
LIBS += $$VTK_DIR/lib/vtkFiltersSources-8.2D.lib
# ---- Rendering ----
LIBS += $$VTK_DIR/lib/vtkRenderingCore-8.2D.lib
LIBS += $$VTK_DIR/lib/vtkRenderingOpenGL2-8.2D.lib
LIBS += $$VTK_DIR/lib/vtkRenderingFreeType-8.2D.lib
LIBS += $$VTK_DIR/lib/vtkRenderingAnnotation-8.2D.lib
LIBS += $$VTK_DIR/lib/vtkRenderingQt-8.2D.lib
LIBS += $$VTK_DIR/lib/vtkfreetype-8.2D.lib
LIBS += $$VTK_DIR/lib/vtkglew-8.2D.lib
# ---- Interaction ----
LIBS += $$VTK_DIR/lib/vtkInteractionStyle-8.2D.lib
LIBS += $$VTK_DIR/lib/vtkInteractionWidgets-8.2D.lib
# ---- IO ----
LIBS += $$VTK_DIR/lib/vtkIOCore-8.2D.lib
LIBS += $$VTK_DIR/lib/vtkIOGeometry-8.2D.lib
LIBS += $$VTK_DIR/lib/vtkIOImage-8.2D.lib
LIBS += $$VTK_DIR/lib/vtkImagingCore-8.2D.lib
LIBS += $$VTK_DIR/lib/vtkjpeg-8.2D.lib
LIBS += $$VTK_DIR/lib/vtkpng-8.2D.lib
LIBS += $$VTK_DIR/lib/vtktiff-8.2D.lib
LIBS += $$VTK_DIR/lib/vtkmetaio-8.2D.lib
LIBS += $$VTK_DIR/lib/vtkDICOMParser-8.2D.lib
LIBS += $$VTK_DIR/lib/vtklz4-8.2D.lib
LIBS += $$VTK_DIR/lib/vtklzma-8.2D.lib
LIBS += $$VTK_DIR/lib/vtkzlib-8.2D.lib
# ---- Qt Integration ----
LIBS += $$VTK_DIR/lib/vtkGUISupportQt-8.2D.lib
}
else:unix {
VTK_ROOT = $$PWD/lib/VTK/VTK-Linux-debug
VTK_INCLUDE_DIR = $$VTK_ROOT/include/vtk-8.2
VTK_LIB_DIR     = $$VTK_ROOT/lib
INCLUDEPATH += $$VTK_INCLUDE_DIR
DEPENDPATH += $$VTK_INCLUDE_DIR
LIBS += -L$$VTK_LIB_DIR
# ---- Common ----
LIBS += -lvtkCommonCore-8.2
LIBS += -lvtkCommonDataModel-8.2
LIBS += -lvtkCommonMath-8.2
LIBS += -lvtkCommonMisc-8.2
LIBS += -lvtkCommonSystem-8.2
LIBS += -lvtkCommonTransforms-8.2
LIBS += -lvtksys-8.2
# ---- Filters ----
LIBS += -lvtkFiltersCore-8.2
LIBS += -lvtkFiltersGeneral-8.2
LIBS += -lvtkFiltersSources-8.2
# ---- Rendering ----
LIBS += -lvtkRenderingCore-8.2
LIBS += -lvtkRenderingOpenGL2-8.2
LIBS += -lvtkRenderingFreeType-8.2
LIBS += -lvtkRenderingAnnotation-8.2
LIBS += -lvtkRenderingQt-8.2
LIBS += -lvtkfreetype-8.2
LIBS += -lvtkglew-8.2
# ---- Interaction ----
LIBS += -lvtkInteractionStyle-8.2
LIBS += -lvtkInteractionWidgets-8.2
# ---- IO ----
LIBS += -lvtkIOCore-8.2
LIBS += -lvtkIOGeometry-8.2
LIBS += -lvtkIOImage-8.2
LIBS += -lvtkImagingCore-8.2
LIBS += -lvtkjpeg-8.2
LIBS += -lvtkpng-8.2
LIBS += -lvtktiff-8.2
LIBS += -lvtkmetaio-8.2
LIBS += -lvtkDICOMParser-8.2
LIBS += -lvtklz4-8.2
LIBS += -lvtklzma-8.2
LIBS += -lvtkzlib-8.2
# ---- Qt Integration ----
LIBS += -lvtkGUISupportQt-8.2
}


win32 {
    SYSTEM_NAME = windows
}
unix:!macx {
    SYSTEM_NAME = linux
}
macx {
    SYSTEM_NAME = macos
}

BUILD_ROOT = $$PWD/build/QMake/$$SYSTEM_NAME

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
