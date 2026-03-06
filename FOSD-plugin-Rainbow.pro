#-------------------------------------------------
#
# Project: FOSD-16CH
# Type   : Qt Plugin Library
# Notes  : 已整理依赖、输出目录、源码分组及注释
#
#-------------------------------------------------

#==================================================
# 1. Qt 模块配置
#==================================================
QT += core gui

greaterThan(QT_MAJOR_VERSION, 4) {
    QT += widgets printsupport
    QT += printsupport
    QT += serialport
    QT += network
    QT += widgets
    QT += core gui
    QT += opengl
}

#源码/翻译编码（按需启用）
CODECFORSRC = UTF-8
CODECFORTR  = UTF-8

# Windows 下 MSVC 可启用 UTF-8 编译选项（按需启用）
# QMAKE_CXXFLAGS += /utf-8
# QMAKE_CFLAGS   += /utf-8


#==================================================
# 2. 工程基本配置
#==================================================
CONFIG += c++11 plugin
TEMPLATE = lib
TARGET = FOSD-16CH

# 启用 Qt 废弃 API 警告
DEFINES += QT_DEPRECATED_WARNINGS

# 如需禁止 Qt 6.0 以前废弃接口，可打开下面宏
# DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000

win32 {         SYSTEM_NAME = windows}
unix:!macx {    QT += waylandcompositor
                LIBS += -lwayland-server -lxkbcommon}
macx {          SYSTEM_NAME = macos}

#==================================================
# 3. 工程内头文件搜索路径
#==================================================
INCLUDEPATH +=  src/main \
                src/app/conncect \
                src/app/disp2d \
                src/app/disp3d \
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
                src/test \
                lib/CustomUi \

#==================================================
# 4. 源文件
#==================================================
SOURCES += \
    src/app/conncect/connectdebug.cpp \
    src/app/conncect/connectwindow.cpp \
    src/app/disp2d/disp2dpara.cpp \
    src/app/disp2d/disp2dwindow.cpp \
    src/app/disp3d/disp3dkeypointedit.cpp \
    src/app/disp3d/disp3dkeypointselect.cpp \
    src/app/disp3d/disp3dwindow.cpp \
    src/app/disp3d/dispdata.cpp \
    src/app/mainwindow/mainwindow.cpp \
    src/app/user/login.cpp \
    src/app/user/userforget.cpp \
    src/app/user/usermanage.cpp \
    src/app/user/userregister.cpp \
    src/Communication/serialtask.cpp \
    src/Communication/tcptask.cpp \
    src/Communication/udptask.cpp \
    src/core/IFE/IFE.cpp \
    src/protocol/analyzepackets.cpp \
    src/protocol/data.cpp \
    src/rainbowfosdplugin.cpp \
    src/test/teststorge.cpp \
    src/utils/database/database.cpp \
    src/utils/global/global.cpp \
    src/utils/QCustomPlot/qcpdocumentobject.cpp \
    src/utils/QCustomPlot/qcustomplot.cpp \
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
    src/app/mainwindow/rainbowmainwindow.cpp


#==================================================
# 5. 头文件
#==================================================
HEADERS += \
    src/app/conncect/connectdebug.h \
    src/app/conncect/connectwindow.h \
    src/app/disp2d/disp2dpara.h \
    src/app/disp2d/disp2dwindow.h \
    src/app/disp3d/disp3dkeypointedit.h \
    src/app/disp3d/disp3dkeypointselect.h \
    src/app/disp3d/disp3dwindow.h \
    src/app/disp3d/dispdata.h \
    src/app/mainwindow/mainwindow.h \
    src/app/user/login.h \
    src/app/user/userforget.h \
    src/app/user/usermanage.h \
    src/app/user/userregister.h \
    src/Communication/serialtask.h \
    src/Communication/tcptask.h \
    src/Communication/udptask.h \
    src/core/IFE/IFE.h \
    src/protocol/analyzepackets.h \
    src/protocol/data.h \
    src/protocol/pocketsconfig.h \
    src/test/teststorge.h \
    src/utils/database/database.h \
    src/utils/global/global.h \
    src/utils/QCustomPlot/qcpdocumentobject.h \
    src/utils/QCustomPlot/qcustomplot.h \
    src/utils/Timers/Timer.h \
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
    src/app/mainwindow/rainbowmainwindow.h



#==================================================
# 6. UI 界面文件
#==================================================
FORMS += \
    src/app/conncect/connectdebug.ui \
    src/app/conncect/connectwindow.ui \
    src/app/disp2d/disp2dpara.ui \
    src/app/disp2d/disp2dwindow.ui \
    src/app/disp3d/disp3dkeypointedit.ui \
    src/app/disp3d/disp3dkeypointselect.ui \
    src/app/disp3d/disp3dwindow.ui \
    src/app/disp3d/dispdata.ui \
    src/app/mainwindow/mainwindow.ui \
    src/app/user/login.ui \
    src/app/user/userforget.ui \
    src/app/user/usermanage.ui \
    src/app/user/userregister.ui \
    src/app/disp3d/trackingpointmanager.ui \
    src/app/disp3d/trackingpointdatatable.ui \
    src/app/mainwindow/rainbowmainwindow.ui


#==================================================
# 7. Qt 资源文件
#==================================================
RESOURCES += \
    $$PWD/res/resource.qrc \


#==================================================
# 8. 输出目录配置
#==================================================
# 插件编译输出目录：
# 编译生成的 libFOSD-16CH.so（或对应平台动态库）
# 将输出到 MPCIF 客户端插件目录下，便于宿主程序直接加载
HOMEDIR = $$(HOME)
MPCIF_PATH = $$HOMEDIR/mpcif

DESTDIR = $$MPCIF_PATH/clients/FOSD_XD/lib

message([INFO] HOME       = $$HOMEDIR)
message([INFO] MPCIF_PATH = $$MPCIF_PATH)
message([INFO] DESTDIR    = $$DESTDIR)


#==================================================
# 9. 全局宏定义
#==================================================
DEFINES += \
    Linux \
    RTI_UNIX \
    TIXML_USEL


#==================================================
# 10. 本地 DDS/FOSD 专用库
#==================================================
# FOSD 自有 DDS 接口头文件及库
INCLUDEPATH += src/utils/DDS \
               src/utils/DDS/include \
               src/utils/DDS/include/dds

LIBS += src/utils/DDS/lib/libFOSD.so

message([INFO] Local FOSD LIBS = $$LIBS)


#==================================================
# 11. RTI DDS / DSE 平台相关依赖
#==================================================
MDS_DIR = /usr/local/rtidds

# RTI DDS 头文件
INCLUDEPATH += \
    $${MDS_DIR}/include \
    $${MDS_DIR}/include/ndds \
    /usr/local/dseapp/include

# DSE 平台相关库
LIBS += -L/usr/local/dseapp/lib \
    -lXmlParser \
    -lDsePrometheus \
    -lSimpleThreadServer \
    -lServerBase \
    -lEsInsert \
    -lDseDdsManagerRti \
    -lDseAuth

# RTI DDS 核心库
LIBS += -L$${MDS_DIR}/lib \
    -lnddscpp \
    -lnddsc \
    -lnddscore


#==================================================
# 12. VTK 图形显示相关依赖
#==================================================
VTK_PREFIX = /home/cring/FOSD/vtk8.2.0

# VTK 头文件
INCLUDEPATH += $$VTK_PREFIX/include/vtk-8.2

# VTK 库
LIBS += -L$$VTK_PREFIX/lib \
    -lvtkCommonCore-8.2 \
    -lvtkCommonDataModel-8.2 \
    -lvtkCommonExecutionModel-8.2 \
    -lvtkCommonMath-8.2 \
    -lvtkCommonMisc-8.2 \
    -lvtkCommonSystem-8.2 \
    -lvtkCommonTransforms-8.2 \
    -lvtkCommonComputationalGeometry-8.2 \
    -lvtkCommonColor-8.2 \
    -lvtkFiltersCore-8.2 \
    -lvtkFiltersGeneral-8.2 \
    -lvtkFiltersGeometry-8.2 \
    -lvtkFiltersSources-8.2 \
    -lvtkFiltersExtraction-8.2 \
    -lvtkFiltersSelection-8.2 \
    -lvtkFiltersStatistics-8.2 \
    -lvtkRenderingCore-8.2 \
    -lvtkRenderingAnnotation-8.2 \
    -lvtkRenderingOpenGL2-8.2 \
    -lvtkRenderingFreeType-8.2 \
    -lvtkRenderingQt-8.2 \
    -lvtkGUISupportQt-8.2 \
    -lvtkInteractionStyle-8.2 \
    -lvtkInteractionWidgets-8.2 \
    -lvtkInfovisCore-8.2 \
    -lvtkViewsCore-8.2 \
    -lvtkIOCore-8.2 \
    -lvtkIOGeometry-8.2 \
    -lvtkIOImage-8.2 \
    -lvtkIOLegacy-8.2 \
    -lvtkIOXML-8.2 \
    -lvtkIOXMLParser-8.2 \
    -lvtkImagingCore-8.2 \
    -lvtkImagingFourier-8.2 \
    -lvtkDICOMParser-8.2 \
    -lvtkmetaio-8.2 \
    -lvtksys-8.2 \
    -lvtkfreetype-8.2 \
    -lvtkzlib-8.2 \
    -lvtklzma-8.2 \
    -lvtklz4-8.2 \
    -lvtkexpat-8.2 \
    -lvtkjpeg-8.2 \
    -lvtktiff-8.2 \
    -lvtkdoubleconversion-8.2 \
    -lvtkglew-8.2 \
    -lGL \
    -lX11 \
    -lXt \
    -lXext

#test
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


#==================================================
# 13. MPCIF 平台头文件路径
#==================================================
INCLUDEPATH += \
    $$MPCIF_PATH/include/_3rdDepends/proj/include \
    $$MPCIF_PATH/include/OWS \
    $$MPCIF_PATH/include/cgis \
    $$MPCIF_PATH/include/cgis/attr \
    $$MPCIF_PATH/include/cgis/dmfmap \
    $$MPCIF_PATH/include/cgis/global \
    $$MPCIF_PATH/include/cgis/global/mapdata \
    $$MPCIF_PATH/include/cgis/item \
    $$MPCIF_PATH/include/cgis/view \
    $$MPCIF_PATH/include/basics/csstub \
    $$MPCIF_PATH/include/basics/ctoolset/ipf \
    $$MPCIF_PATH/include/basics/ctoolset/openthreads \
    $$MPCIF_PATH/include/basics/ctoolset/sqlite \
    $$MPCIF_PATH/include/ui/uimgr \
    $$MPCIF_PATH/include/ui/remotedialog \
    $$MPCIF_PATH/include/procm \
    $$MPCIF_PATH/include/dock \
    $$MPCIF_PATH/include/rcmenu \
    $$MPCIF_PATH/include/dds \
    $$MPCIF_PATH/include/remotedialog \
    $$MPCIF_PATH/include/toppanel \
    $$MPCIF_PATH/include/ui/ica \
    $$MPCIF_PATH/include/plot \
    $$MPCIF_PATH/include/plotinterface \
    $$MPCIF_PATH/include/basics/remotenotifier \
    $$MPCIF_PATH/include/utils/MQManager \
    $$MPCIF_PATH/include/utils/MQUtil \
    $$MPCIF_PATH/include

#==================================================
# 14. MPCIF 平台链接库
#==================================================
LIBS += -L$$HOMEDIR/mpcif/lib \
    -lUIMgrd \
    -lCommonToold \
    -lGISEngined \
    -lclient_stubd \
    -lipfopenthreadsd \
    -lipfsqlited \
    -lserver_stubd \
    -lICAlibd \
    -lPlotApid \
    -lPlotInterface \
    -lRemoteDialog_Clientd \
    -lMQManager \
    -lMQUtils


#==================================================
# 15. 部署说明（保留模板）
#==================================================
## Default rules for deployment.
# qnx: target.path = /tmp/$${TARGET}/bin
# else: unix:!android: target.path = /opt/$${TARGET}/bin
# !isEmpty(target.path): INSTALLS += target


#==================================================
# 16. 编译中间产物存放地址
#==================================================
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
