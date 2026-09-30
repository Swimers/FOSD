#-------------------------------------------------
#
# Project created by QtCreator 2025-06-26T11:13:38
#
#-------------------------------------------------

QT       += core gui
QT       += opengl

# Invoke the Buildroot qmake with CONFIG+=rk3588_arm_linux.
contains(CONFIG, rk3588_arm_linux) {
    !unix: error(rk3588_arm_linux requires the RK3588 Linux cross qmake)
    CONFIG -= debug
    CONFIG += release
    DEFINES += FOSD_RK3588_ARM_LINUX
}

win32 {
    SYSTEM_NAME = windows
}
unix:!macx:!contains(CONFIG, rk3588_arm_linux) {
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
                src/Communication \
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
    src/app/disp3d/disp3dcolorpicker.ui \
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
    src/app/disp3d/disp3dcolorpicker.h \
    src/app/disp3d/docks/disp3ddockmanager.h \
    src/app/disp3d/docks/disp3ddockpanel.h \
    src/app/disp3d/docks/disp3dinfodockpanel.h \
    src/app/disp3d/docks/disp3dmodeldockpanel.h \
    src/app/disp3d/docks/disp3dviewdockpanel.h \
    src/app/disp3d/disp3dwindow.h \
    src/app/mainwindow/oldmainwindow.h \
    src/app/user/login.h \
    src/Communication/serialtask.h \
    src/Communication/tcptask.h \
    src/Communication/udptask.h \
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
    src/app/disp3d/disp3dcolorpicker.cpp \
    src/app/disp3d/docks/disp3ddockmanager.cpp \
    src/app/disp3d/docks/disp3dinfodockpanel.cpp \
    src/app/disp3d/docks/disp3dmodeldockpanel.cpp \
    src/app/disp3d/docks/disp3dviewdockpanel.cpp \
    src/app/disp3d/disp3dwindow.cpp \
    src/app/mainwindow/oldmainwindow.cpp \
    src/app/user/login.cpp \
    src/Communication/serialtask.cpp \
    src/Communication/tcptask.cpp \
    src/Communication/udptask.cpp \
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

# =============================================================================
# VTK 8.2 依赖库 — 仅保留项目实际使用的模块及必要的传递依赖
# 清理日期: 2026-07-10
#
# 实际使用的VTK头文件 → 对应模块:
#   vtkSmartPointer/vtkDoubleArray/vtkAutoInit/vtkOutputWindow/vtkCallbackCommand/vtkLookupTable → vtkCommonCore
#   vtkPolyData/vtkPoints/vtkPointData/vtkCellArray/vtkIdType → vtkCommonDataModel
#   vtkRenderer/vtkRenderWindow/vtkActor/vtkPolyDataMapper/vtkCamera/vtkTextActor/vtkProperty/
#   vtkPointPicker/vtkRenderWindowInteractor/vtkRenderWidget → vtkRenderingCore
#   vtkSTLReader/vtkSTLWriter → vtkIOGeometry
#   vtkSphereSource/vtkCubeSource/vtkArrowSource/vtkLineSource/vtkPlaneSource → vtkFiltersSources
#   vtkAppendPolyData/vtkCleanPolyData → vtkFiltersCore
#   vtkTextureMapToSphere → vtkFiltersGeneral
#   vtkInteractorStyleTrackballCamera → vtkInteractionStyle
#   vtkPointPicker/vtkInteractorObserver → vtkInteractionWidgets (传递依赖)
#   vtkAxesActor/vtkCaptionActor2D → vtkRenderingAnnotation
#   vtkTextProperty → vtkRenderingFreeType
#   vtkJPEGReader/vtkPNGReader → vtkIOImage
#   vtkTexture → vtkRenderingCore
#   QVTKOpenGLNativeWidget → vtkGUISupportQt
#   vtkGenericOpenGLRenderWindow → vtkRenderingOpenGL2
#   VTK_MODULE_INIT(vtkRenderingFreeType) → vtkRenderingFreeType
# =============================================================================

win32:CONFIG(release, debug|release):{
VTK_DIR = $$clean_path($$PWD/lib/VTK/VTK-release)
INCLUDEPATH += $$VTK_DIR/include/vtk-8.2
INCLUDEPATH += $$VTK_DIR/include/vtk-8.2/vtkhdf5
LIBS += -L$$VTK_DIR/lib
# ---- Common 核心 ----
LIBS += $$VTK_DIR/lib/vtkCommonColor-8.2.lib
LIBS += $$VTK_DIR/lib/vtkCommonComputationalGeometry-8.2.lib
LIBS += $$VTK_DIR/lib/vtkCommonCore-8.2.lib
LIBS += $$VTK_DIR/lib/vtkCommonDataModel-8.2.lib
LIBS += $$VTK_DIR/lib/vtkCommonExecutionModel-8.2.lib
LIBS += $$VTK_DIR/lib/vtkCommonMath-8.2.lib
LIBS += $$VTK_DIR/lib/vtkCommonMisc-8.2.lib
LIBS += $$VTK_DIR/lib/vtkCommonSystem-8.2.lib
LIBS += $$VTK_DIR/lib/vtkCommonTransforms-8.2.lib
# ---- Filters 过滤器 ----
LIBS += $$VTK_DIR/lib/vtkFiltersCore-8.2.lib
LIBS += $$VTK_DIR/lib/vtkFiltersGeneral-8.2.lib
LIBS += $$VTK_DIR/lib/vtkFiltersModeling-8.2.lib
LIBS += $$VTK_DIR/lib/vtkFiltersTexture-8.2.lib
LIBS += $$VTK_DIR/lib/vtkFiltersSources-8.2.lib
# ---- Rendering 渲染 ----
LIBS += $$VTK_DIR/lib/vtkRenderingAnnotation-8.2.lib
LIBS += $$VTK_DIR/lib/vtkRenderingCore-8.2.lib
LIBS += $$VTK_DIR/lib/vtkRenderingFreeType-8.2.lib
LIBS += $$VTK_DIR/lib/vtkRenderingOpenGL2-8.2.lib
LIBS += $$VTK_DIR/lib/vtkRenderingQt-8.2.lib
# ---- Interaction 交互 ----
LIBS += $$VTK_DIR/lib/vtkInteractionStyle-8.2.lib
LIBS += $$VTK_DIR/lib/vtkInteractionWidgets-8.2.lib
# ---- IO 输入输出 ----
LIBS += $$VTK_DIR/lib/vtkIOCore-8.2.lib
LIBS += $$VTK_DIR/lib/vtkIOGeometry-8.2.lib
LIBS += $$VTK_DIR/lib/vtkIOImage-8.2.lib
LIBS += $$VTK_DIR/lib/vtkIOLegacy-8.2.lib
# ---- GUI Qt集成 ----
LIBS += $$VTK_DIR/lib/vtkGUISupportQt-8.2.lib
# ---- ThirdParty 第三方依赖（VTK内部传递依赖） ----
LIBS += $$VTK_DIR/lib/vtkfreetype-8.2.lib
LIBS += $$VTK_DIR/lib/vtkglew-8.2.lib
LIBS += $$VTK_DIR/lib/vtkgl2ps-8.2.lib
LIBS += $$VTK_DIR/lib/vtkjpeg-8.2.lib
LIBS += $$VTK_DIR/lib/vtkjsoncpp-8.2.lib
LIBS += $$VTK_DIR/lib/vtklz4-8.2.lib
LIBS += $$VTK_DIR/lib/vtklzma-8.2.lib
LIBS += $$VTK_DIR/lib/vtkmetaio-8.2.lib
LIBS += $$VTK_DIR/lib/vtkpng-8.2.lib
LIBS += $$VTK_DIR/lib/vtkpugixml-8.2.lib
LIBS += $$VTK_DIR/lib/vtksqlite-8.2.lib
LIBS += $$VTK_DIR/lib/vtksys-8.2.lib
LIBS += $$VTK_DIR/lib/vtktiff-8.2.lib
LIBS += $$VTK_DIR/lib/vtkzlib-8.2.lib
}
else:win32:CONFIG(debug, debug|release):{
VTK_DIR = $$clean_path($$PWD/lib/VTK/VTK-debug)
INCLUDEPATH += $$VTK_DIR/include/vtk-8.2
INCLUDEPATH += $$VTK_DIR/include/vtk-8.2/vtkhdf5
LIBS += -L$$VTK_DIR/lib
# ---- Common 核心 ----
LIBS +=$$VTK_DIR/lib/vtkCommonColor-8.2D.lib
LIBS +=$$VTK_DIR/lib/vtkCommonComputationalGeometry-8.2D.lib
LIBS +=$$VTK_DIR/lib/vtkCommonCore-8.2D.lib
LIBS +=$$VTK_DIR/lib/vtkCommonDataModel-8.2D.lib
LIBS +=$$VTK_DIR/lib/vtkCommonExecutionModel-8.2D.lib
LIBS +=$$VTK_DIR/lib/vtkCommonMath-8.2D.lib
LIBS +=$$VTK_DIR/lib/vtkCommonMisc-8.2D.lib
LIBS +=$$VTK_DIR/lib/vtkCommonSystem-8.2D.lib
LIBS +=$$VTK_DIR/lib/vtkCommonTransforms-8.2D.lib
# ---- Filters 过滤器 ----
LIBS +=$$VTK_DIR/lib/vtkFiltersCore-8.2D.lib
LIBS +=$$VTK_DIR/lib/vtkFiltersGeneral-8.2D.lib
LIBS +=$$VTK_DIR/lib/vtkFiltersModeling-8.2D.lib
LIBS +=$$VTK_DIR/lib/vtkFiltersTexture-8.2D.lib
LIBS +=$$VTK_DIR/lib/vtkFiltersSources-8.2D.lib
# ---- Rendering 渲染 ----
LIBS +=$$VTK_DIR/lib/vtkRenderingAnnotation-8.2D.lib
LIBS +=$$VTK_DIR/lib/vtkRenderingCore-8.2D.lib
LIBS +=$$VTK_DIR/lib/vtkRenderingFreeType-8.2D.lib
LIBS +=$$VTK_DIR/lib/vtkRenderingOpenGL2-8.2D.lib
LIBS +=$$VTK_DIR/lib/vtkRenderingQt-8.2D.lib
# ---- Interaction 交互 ----
LIBS +=$$VTK_DIR/lib/vtkInteractionStyle-8.2D.lib
LIBS +=$$VTK_DIR/lib/vtkInteractionWidgets-8.2D.lib
# ---- IO 输入输出 ----
LIBS +=$$VTK_DIR/lib/vtkIOCore-8.2D.lib
LIBS +=$$VTK_DIR/lib/vtkIOGeometry-8.2D.lib
LIBS +=$$VTK_DIR/lib/vtkIOImage-8.2D.lib
LIBS +=$$VTK_DIR/lib/vtkIOLegacy-8.2D.lib
# ---- GUI Qt集成 ----
LIBS +=$$VTK_DIR/lib/vtkGUISupportQt-8.2D.lib
# ---- ThirdParty 第三方依赖（VTK内部传递依赖） ----
LIBS +=$$VTK_DIR/lib/vtkfreetype-8.2D.lib
LIBS +=$$VTK_DIR/lib/vtkglew-8.2D.lib
LIBS +=$$VTK_DIR/lib/vtkgl2ps-8.2D.lib
LIBS +=$$VTK_DIR/lib/vtkjpeg-8.2D.lib
LIBS +=$$VTK_DIR/lib/vtkjsoncpp-8.2D.lib
LIBS +=$$VTK_DIR/lib/vtklz4-8.2D.lib
LIBS +=$$VTK_DIR/lib/vtklzma-8.2D.lib
LIBS +=$$VTK_DIR/lib/vtkmetaio-8.2D.lib
LIBS +=$$VTK_DIR/lib/vtkpng-8.2D.lib
LIBS +=$$VTK_DIR/lib/vtkpugixml-8.2D.lib
LIBS +=$$VTK_DIR/lib/vtksqlite-8.2D.lib
LIBS +=$$VTK_DIR/lib/vtksys-8.2D.lib
LIBS +=$$VTK_DIR/lib/vtktiff-8.2D.lib
LIBS +=$$VTK_DIR/lib/vtkzlib-8.2D.lib
}
else:unix {
contains(CONFIG, rk3588_arm_linux) {
    VTK_ROOT = $$clean_path($$PWD/lib/VTK/VTK-Linux-release)
    !exists($$VTK_ROOT/include/vtk-8.2/vtkVersion.h): error(RK3588 VTK Release headers are missing: $$VTK_ROOT)
    !exists($$VTK_ROOT/lib/libvtkGUISupportQt-8.2.so): error(RK3588 VTK Release libraries are missing: $$VTK_ROOT)
    INCLUDEPATH += $$VTK_ROOT/include/vtk-8.2
    LIBS += -L$$VTK_ROOT/lib
    QMAKE_RPATHLINKDIR += $$VTK_ROOT/lib
    LIBS += -lvtkGUISupportQt-8.2 \
            -lvtkRenderingOpenGL2-8.2 \
            -lvtkRenderingFreeType-8.2 \
            -lvtkRenderingAnnotation-8.2 \
            -lvtkInteractionStyle-8.2 \
            -lvtkInteractionWidgets-8.2 \
            -lvtkIOGeometry-8.2 \
            -lvtkIOImage-8.2 \
            -lvtkIOLegacy-8.2 \
            -lvtkIOCore-8.2 \
            -lvtkFiltersSources-8.2 \
            -lvtkFiltersTexture-8.2 \
            -lvtkFiltersModeling-8.2 \
            -lvtkFiltersGeneral-8.2 \
            -lvtkFiltersCore-8.2 \
            -lvtkRenderingCore-8.2 \
            -lvtkCommonExecutionModel-8.2 \
            -lvtkCommonDataModel-8.2 \
            -lvtkCommonCore-8.2
} else {
VTK_ROOT = $$PWD/lib/VTK/VTK-Linux-debug
VTK_INCLUDE_DIR = $$VTK_ROOT/include/vtk-8.2
VTK_LIB_DIR     = $$VTK_ROOT/lib
INCLUDEPATH += $$VTK_INCLUDE_DIR
DEPENDPATH += $$VTK_INCLUDE_DIR
LIBS += -L$$VTK_LIB_DIR
# ---- Common 核心 ----
LIBS += -lvtkCommonColor-8.2
LIBS += -lvtkCommonComputationalGeometry-8.2
LIBS += -lvtkCommonCore-8.2
LIBS += -lvtkCommonDataModel-8.2
LIBS += -lvtkCommonExecutionModel-8.2
LIBS += -lvtkCommonMath-8.2
LIBS += -lvtkCommonMisc-8.2
LIBS += -lvtkCommonSystem-8.2
LIBS += -lvtkCommonTransforms-8.2
# ---- Filters 过滤器 ----
LIBS += -lvtkFiltersCore-8.2
LIBS += -lvtkFiltersGeneral-8.2
LIBS += -lvtkFiltersModeling-8.2
LIBS += -lvtkFiltersTexture-8.2
LIBS += -lvtkFiltersSources-8.2
# ---- Rendering 渲染 ----
LIBS += -lvtkRenderingAnnotation-8.2
LIBS += -lvtkRenderingCore-8.2
LIBS += -lvtkRenderingFreeType-8.2
LIBS += -lvtkRenderingOpenGL2-8.2
LIBS += -lvtkRenderingQt-8.2
# ---- Interaction 交互 ----
LIBS += -lvtkInteractionStyle-8.2
LIBS += -lvtkInteractionWidgets-8.2
# ---- IO 输入输出 ----
LIBS += -lvtkIOCore-8.2
LIBS += -lvtkIOGeometry-8.2
LIBS += -lvtkIOImage-8.2
LIBS += -lvtkIOLegacy-8.2
# ---- GUI Qt集成 ----
LIBS += -lvtkGUISupportQt-8.2
# ---- ThirdParty 第三方依赖 ----
LIBS += -lvtkfreetype-8.2
LIBS += -lvtkglew-8.2
LIBS += -lvtkgl2ps-8.2
LIBS += -lvtkjpeg-8.2
LIBS += -lvtkjsoncpp-8.2
LIBS += -lvtklz4-8.2
LIBS += -lvtklzma-8.2
LIBS += -lvtkmetaio-8.2
LIBS += -lvtkpng-8.2
LIBS += -lvtkpugixml-8.2
LIBS += -lvtksqlite-8.2
LIBS += -lvtksys-8.2
LIBS += -lvtktiff-8.2
LIBS += -lvtkzlib-8.2
}
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
contains(CONFIG, rk3588_arm_linux): BUILD_ROOT = $$PWD/build/Qmake/RK3588-ARM-Linux

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
