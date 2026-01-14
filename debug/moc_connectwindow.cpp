/****************************************************************************
** Meta object code from reading C++ file 'connectwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Connect/connectwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'connectwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_connectwindow_t {
    QByteArrayData data[77];
    char stringdata0[1109];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_connectwindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_connectwindow_t qt_meta_stringdata_connectwindow = {
    {
QT_MOC_LITERAL(0, 0, 13), // "connectwindow"
QT_MOC_LITERAL(1, 14, 9), // "dataReady"
QT_MOC_LITERAL(2, 24, 0), // ""
QT_MOC_LITERAL(3, 25, 14), // "serialPortOpen"
QT_MOC_LITERAL(4, 40, 8), // "portName"
QT_MOC_LITERAL(5, 49, 8), // "baudRate"
QT_MOC_LITERAL(6, 58, 15), // "serialPortClose"
QT_MOC_LITERAL(7, 74, 14), // "serialDataSend"
QT_MOC_LITERAL(8, 89, 4), // "data"
QT_MOC_LITERAL(9, 94, 7), // "udpOpen"
QT_MOC_LITERAL(10, 102, 12), // "QHostAddress"
QT_MOC_LITERAL(11, 115, 7), // "address"
QT_MOC_LITERAL(12, 123, 4), // "port"
QT_MOC_LITERAL(13, 128, 8), // "udpClose"
QT_MOC_LITERAL(14, 137, 11), // "udpDataSend"
QT_MOC_LITERAL(15, 149, 4), // "text"
QT_MOC_LITERAL(16, 154, 9), // "IPAddress"
QT_MOC_LITERAL(17, 164, 7), // "PortNum"
QT_MOC_LITERAL(18, 172, 7), // "tcpOpen"
QT_MOC_LITERAL(19, 180, 8), // "tcpClose"
QT_MOC_LITERAL(20, 189, 11), // "tcpDataSend"
QT_MOC_LITERAL(21, 201, 11), // "dataFreshCH"
QT_MOC_LITERAL(22, 213, 10), // "const int*"
QT_MOC_LITERAL(23, 224, 4), // "Flag"
QT_MOC_LITERAL(24, 229, 9), // "dataReset"
QT_MOC_LITERAL(25, 239, 12), // "dataSaveAuto"
QT_MOC_LITERAL(26, 252, 13), // "AutoSaveCycle"
QT_MOC_LITERAL(27, 266, 14), // "dataSaveManual"
QT_MOC_LITERAL(28, 281, 13), // "set_strain_AB"
QT_MOC_LITERAL(29, 295, 3), // "m_A"
QT_MOC_LITERAL(30, 299, 3), // "m_B"
QT_MOC_LITERAL(31, 303, 11), // "handleWheel"
QT_MOC_LITERAL(32, 315, 12), // "QWheelEvent*"
QT_MOC_LITERAL(33, 328, 5), // "event"
QT_MOC_LITERAL(34, 334, 13), // "DispDiagnosis"
QT_MOC_LITERAL(35, 348, 4), // "temp"
QT_MOC_LITERAL(36, 353, 6), // "setLED"
QT_MOC_LITERAL(37, 360, 5), // "color"
QT_MOC_LITERAL(38, 366, 4), // "size"
QT_MOC_LITERAL(39, 371, 13), // "serialDataRec"
QT_MOC_LITERAL(40, 385, 18), // "serialStatusChange"
QT_MOC_LITERAL(41, 404, 5), // "State"
QT_MOC_LITERAL(42, 410, 10), // "udpDataRec"
QT_MOC_LITERAL(43, 421, 15), // "udpStatusChange"
QT_MOC_LITERAL(44, 437, 10), // "tcpDataRec"
QT_MOC_LITERAL(45, 448, 15), // "tcpStatusChange"
QT_MOC_LITERAL(46, 464, 25), // "tcptask::ConnectionStatus"
QT_MOC_LITERAL(47, 490, 6), // "status"
QT_MOC_LITERAL(48, 497, 11), // "UpDataTable"
QT_MOC_LITERAL(49, 509, 10), // "InitWindow"
QT_MOC_LITERAL(50, 520, 10), // "InitSerial"
QT_MOC_LITERAL(51, 531, 7), // "InitUdp"
QT_MOC_LITERAL(52, 539, 7), // "InitTcp"
QT_MOC_LITERAL(53, 547, 11), // "InitAnalyze"
QT_MOC_LITERAL(54, 559, 17), // "RefreshNetAddress"
QT_MOC_LITERAL(55, 577, 29), // "on_pbtn_RefreshSerial_clicked"
QT_MOC_LITERAL(56, 607, 26), // "on_pbtn_OpenSerial_clicked"
QT_MOC_LITERAL(57, 634, 26), // "on_pbtn_SerialSend_clicked"
QT_MOC_LITERAL(58, 661, 27), // "on_pbtn_SerialClear_clicked"
QT_MOC_LITERAL(59, 689, 25), // "on_pbtn_Diagnosis_clicked"
QT_MOC_LITERAL(60, 715, 23), // "on_pbtn_OpenNet_clicked"
QT_MOC_LITERAL(61, 739, 23), // "on_pbtn_UdpSend_clicked"
QT_MOC_LITERAL(62, 763, 24), // "on_pbtn_UdpClear_clicked"
QT_MOC_LITERAL(63, 788, 23), // "on_pbtn_ParaSet_clicked"
QT_MOC_LITERAL(64, 812, 27), // "on_pbtn_SelectAllCH_clicked"
QT_MOC_LITERAL(65, 840, 24), // "on_pbtn_SelectCH_clicked"
QT_MOC_LITERAL(66, 865, 24), // "on_pbtn_ParaRest_clicked"
QT_MOC_LITERAL(67, 890, 27), // "on_tabWidget_currentChanged"
QT_MOC_LITERAL(68, 918, 5), // "index"
QT_MOC_LITERAL(69, 924, 27), // "on_pbtn_SaveDynamic_clicked"
QT_MOC_LITERAL(70, 952, 26), // "on_pbtn_SaveStatic_clicked"
QT_MOC_LITERAL(71, 979, 28), // "on_pbtn_SaveSpectrum_clicked"
QT_MOC_LITERAL(72, 1008, 23), // "on_pbtn_TcpSend_clicked"
QT_MOC_LITERAL(73, 1032, 24), // "on_pbtn_TcpClear_clicked"
QT_MOC_LITERAL(74, 1057, 31), // "on_pbtn_WaveLengthReset_clicked"
QT_MOC_LITERAL(75, 1089, 9), // "SwitchToB"
QT_MOC_LITERAL(76, 1099, 9) // "SwitchToW"

    },
    "connectwindow\0dataReady\0\0serialPortOpen\0"
    "portName\0baudRate\0serialPortClose\0"
    "serialDataSend\0data\0udpOpen\0QHostAddress\0"
    "address\0port\0udpClose\0udpDataSend\0"
    "text\0IPAddress\0PortNum\0tcpOpen\0tcpClose\0"
    "tcpDataSend\0dataFreshCH\0const int*\0"
    "Flag\0dataReset\0dataSaveAuto\0AutoSaveCycle\0"
    "dataSaveManual\0set_strain_AB\0m_A\0m_B\0"
    "handleWheel\0QWheelEvent*\0event\0"
    "DispDiagnosis\0temp\0setLED\0color\0size\0"
    "serialDataRec\0serialStatusChange\0State\0"
    "udpDataRec\0udpStatusChange\0tcpDataRec\0"
    "tcpStatusChange\0tcptask::ConnectionStatus\0"
    "status\0UpDataTable\0InitWindow\0InitSerial\0"
    "InitUdp\0InitTcp\0InitAnalyze\0"
    "RefreshNetAddress\0on_pbtn_RefreshSerial_clicked\0"
    "on_pbtn_OpenSerial_clicked\0"
    "on_pbtn_SerialSend_clicked\0"
    "on_pbtn_SerialClear_clicked\0"
    "on_pbtn_Diagnosis_clicked\0"
    "on_pbtn_OpenNet_clicked\0on_pbtn_UdpSend_clicked\0"
    "on_pbtn_UdpClear_clicked\0"
    "on_pbtn_ParaSet_clicked\0"
    "on_pbtn_SelectAllCH_clicked\0"
    "on_pbtn_SelectCH_clicked\0"
    "on_pbtn_ParaRest_clicked\0"
    "on_tabWidget_currentChanged\0index\0"
    "on_pbtn_SaveDynamic_clicked\0"
    "on_pbtn_SaveStatic_clicked\0"
    "on_pbtn_SaveSpectrum_clicked\0"
    "on_pbtn_TcpSend_clicked\0"
    "on_pbtn_TcpClear_clicked\0"
    "on_pbtn_WaveLengthReset_clicked\0"
    "SwitchToB\0SwitchToW"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_connectwindow[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      52,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      14,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,  274,    2, 0x06 /* Public */,
       3,    2,  275,    2, 0x06 /* Public */,
       6,    0,  280,    2, 0x06 /* Public */,
       7,    1,  281,    2, 0x06 /* Public */,
       9,    2,  284,    2, 0x06 /* Public */,
      13,    0,  289,    2, 0x06 /* Public */,
      14,    3,  290,    2, 0x06 /* Public */,
      18,    2,  297,    2, 0x06 /* Public */,
      19,    0,  302,    2, 0x06 /* Public */,
      20,    1,  303,    2, 0x06 /* Public */,
      21,    1,  306,    2, 0x06 /* Public */,
      24,    0,  309,    2, 0x06 /* Public */,
      25,    1,  310,    2, 0x06 /* Public */,
      27,    0,  313,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      28,    2,  314,    2, 0x0a /* Public */,
      31,    1,  319,    2, 0x0a /* Public */,
      34,    1,  322,    2, 0x0a /* Public */,
      36,    2,  325,    2, 0x0a /* Public */,
      39,    1,  330,    2, 0x0a /* Public */,
      40,    1,  333,    2, 0x0a /* Public */,
      42,    1,  336,    2, 0x0a /* Public */,
      43,    1,  339,    2, 0x0a /* Public */,
      44,    1,  342,    2, 0x0a /* Public */,
      45,    1,  345,    2, 0x0a /* Public */,
      48,    0,  348,    2, 0x08 /* Private */,
      49,    0,  349,    2, 0x08 /* Private */,
      50,    0,  350,    2, 0x08 /* Private */,
      51,    0,  351,    2, 0x08 /* Private */,
      52,    0,  352,    2, 0x08 /* Private */,
      53,    0,  353,    2, 0x08 /* Private */,
      54,    0,  354,    2, 0x08 /* Private */,
      55,    0,  355,    2, 0x08 /* Private */,
      56,    0,  356,    2, 0x08 /* Private */,
      57,    0,  357,    2, 0x08 /* Private */,
      58,    0,  358,    2, 0x08 /* Private */,
      59,    0,  359,    2, 0x08 /* Private */,
      60,    0,  360,    2, 0x08 /* Private */,
      61,    0,  361,    2, 0x08 /* Private */,
      62,    0,  362,    2, 0x08 /* Private */,
      63,    0,  363,    2, 0x08 /* Private */,
      64,    0,  364,    2, 0x08 /* Private */,
      65,    0,  365,    2, 0x08 /* Private */,
      66,    0,  366,    2, 0x08 /* Private */,
      67,    1,  367,    2, 0x08 /* Private */,
      69,    0,  370,    2, 0x08 /* Private */,
      70,    0,  371,    2, 0x08 /* Private */,
      71,    0,  372,    2, 0x08 /* Private */,
      72,    0,  373,    2, 0x08 /* Private */,
      73,    0,  374,    2, 0x08 /* Private */,
      74,    0,  375,    2, 0x08 /* Private */,
      75,    0,  376,    2, 0x08 /* Private */,
      76,    0,  377,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::Int,    4,    5,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    8,
    QMetaType::Void, 0x80000000 | 10, QMetaType::UShort,   11,   12,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, 0x80000000 | 10, QMetaType::UShort,   15,   16,   17,
    QMetaType::Void, 0x80000000 | 10, QMetaType::UShort,   11,   12,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   15,
    QMetaType::Void, 0x80000000 | 22,   23,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   26,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::Float, QMetaType::Float,   29,   30,
    QMetaType::Void, 0x80000000 | 32,   33,
    QMetaType::Void, QMetaType::QString,   35,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,   37,   38,
    QMetaType::Void, QMetaType::QString,    8,
    QMetaType::Void, QMetaType::Bool,   41,
    QMetaType::Void, QMetaType::QString,    8,
    QMetaType::Void, QMetaType::Bool,   41,
    QMetaType::Void, QMetaType::QString,    8,
    QMetaType::Void, 0x80000000 | 46,   47,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   68,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Bool,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void connectwindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<connectwindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->dataReady(); break;
        case 1: _t->serialPortOpen((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< qint32(*)>(_a[2]))); break;
        case 2: _t->serialPortClose(); break;
        case 3: _t->serialDataSend((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 4: _t->udpOpen((*reinterpret_cast< const QHostAddress(*)>(_a[1])),(*reinterpret_cast< const quint16(*)>(_a[2]))); break;
        case 5: _t->udpClose(); break;
        case 6: _t->udpDataSend((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QHostAddress(*)>(_a[2])),(*reinterpret_cast< const quint16(*)>(_a[3]))); break;
        case 7: _t->tcpOpen((*reinterpret_cast< const QHostAddress(*)>(_a[1])),(*reinterpret_cast< const quint16(*)>(_a[2]))); break;
        case 8: _t->tcpClose(); break;
        case 9: _t->tcpDataSend((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 10: _t->dataFreshCH((*reinterpret_cast< const int*(*)>(_a[1]))); break;
        case 11: _t->dataReset(); break;
        case 12: _t->dataSaveAuto((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 13: _t->dataSaveManual(); break;
        case 14: _t->set_strain_AB((*reinterpret_cast< float(*)>(_a[1])),(*reinterpret_cast< float(*)>(_a[2]))); break;
        case 15: _t->handleWheel((*reinterpret_cast< QWheelEvent*(*)>(_a[1]))); break;
        case 16: _t->DispDiagnosis((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 17: _t->setLED((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 18: _t->serialDataRec((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 19: _t->serialStatusChange((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 20: _t->udpDataRec((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 21: _t->udpStatusChange((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 22: _t->tcpDataRec((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 23: _t->tcpStatusChange((*reinterpret_cast< tcptask::ConnectionStatus(*)>(_a[1]))); break;
        case 24: _t->UpDataTable(); break;
        case 25: _t->InitWindow(); break;
        case 26: _t->InitSerial(); break;
        case 27: _t->InitUdp(); break;
        case 28: _t->InitTcp(); break;
        case 29: _t->InitAnalyze(); break;
        case 30: _t->RefreshNetAddress(); break;
        case 31: _t->on_pbtn_RefreshSerial_clicked(); break;
        case 32: _t->on_pbtn_OpenSerial_clicked(); break;
        case 33: _t->on_pbtn_SerialSend_clicked(); break;
        case 34: _t->on_pbtn_SerialClear_clicked(); break;
        case 35: _t->on_pbtn_Diagnosis_clicked(); break;
        case 36: _t->on_pbtn_OpenNet_clicked(); break;
        case 37: _t->on_pbtn_UdpSend_clicked(); break;
        case 38: _t->on_pbtn_UdpClear_clicked(); break;
        case 39: _t->on_pbtn_ParaSet_clicked(); break;
        case 40: _t->on_pbtn_SelectAllCH_clicked(); break;
        case 41: _t->on_pbtn_SelectCH_clicked(); break;
        case 42: _t->on_pbtn_ParaRest_clicked(); break;
        case 43: _t->on_tabWidget_currentChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 44: _t->on_pbtn_SaveDynamic_clicked(); break;
        case 45: _t->on_pbtn_SaveStatic_clicked(); break;
        case 46: { bool _r = _t->on_pbtn_SaveSpectrum_clicked();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 47: _t->on_pbtn_TcpSend_clicked(); break;
        case 48: _t->on_pbtn_TcpClear_clicked(); break;
        case 49: _t->on_pbtn_WaveLengthReset_clicked(); break;
        case 50: _t->SwitchToB(); break;
        case 51: _t->SwitchToW(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (connectwindow::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&connectwindow::dataReady)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (connectwindow::*)(const QString & , qint32 );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&connectwindow::serialPortOpen)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (connectwindow::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&connectwindow::serialPortClose)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (connectwindow::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&connectwindow::serialDataSend)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (connectwindow::*)(const QHostAddress & , const quint16 & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&connectwindow::udpOpen)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (connectwindow::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&connectwindow::udpClose)) {
                *result = 5;
                return;
            }
        }
        {
            using _t = void (connectwindow::*)(const QString , const QHostAddress , const quint16 );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&connectwindow::udpDataSend)) {
                *result = 6;
                return;
            }
        }
        {
            using _t = void (connectwindow::*)(const QHostAddress & , const quint16 & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&connectwindow::tcpOpen)) {
                *result = 7;
                return;
            }
        }
        {
            using _t = void (connectwindow::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&connectwindow::tcpClose)) {
                *result = 8;
                return;
            }
        }
        {
            using _t = void (connectwindow::*)(const QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&connectwindow::tcpDataSend)) {
                *result = 9;
                return;
            }
        }
        {
            using _t = void (connectwindow::*)(const int * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&connectwindow::dataFreshCH)) {
                *result = 10;
                return;
            }
        }
        {
            using _t = void (connectwindow::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&connectwindow::dataReset)) {
                *result = 11;
                return;
            }
        }
        {
            using _t = void (connectwindow::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&connectwindow::dataSaveAuto)) {
                *result = 12;
                return;
            }
        }
        {
            using _t = void (connectwindow::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&connectwindow::dataSaveManual)) {
                *result = 13;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject connectwindow::staticMetaObject = { {
    &QWidget::staticMetaObject,
    qt_meta_stringdata_connectwindow.data,
    qt_meta_data_connectwindow,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *connectwindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *connectwindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_connectwindow.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int connectwindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 52)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 52;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 52)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 52;
    }
    return _id;
}

// SIGNAL 0
void connectwindow::dataReady()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void connectwindow::serialPortOpen(const QString & _t1, qint32 _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void connectwindow::serialPortClose()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void connectwindow::serialDataSend(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void connectwindow::udpOpen(const QHostAddress & _t1, const quint16 & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void connectwindow::udpClose()
{
    QMetaObject::activate(this, &staticMetaObject, 5, nullptr);
}

// SIGNAL 6
void connectwindow::udpDataSend(const QString _t1, const QHostAddress _t2, const quint16 _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void connectwindow::tcpOpen(const QHostAddress & _t1, const quint16 & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void connectwindow::tcpClose()
{
    QMetaObject::activate(this, &staticMetaObject, 8, nullptr);
}

// SIGNAL 9
void connectwindow::tcpDataSend(const QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 9, _a);
}

// SIGNAL 10
void connectwindow::dataFreshCH(const int * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 10, _a);
}

// SIGNAL 11
void connectwindow::dataReset()
{
    QMetaObject::activate(this, &staticMetaObject, 11, nullptr);
}

// SIGNAL 12
void connectwindow::dataSaveAuto(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 12, _a);
}

// SIGNAL 13
void connectwindow::dataSaveManual()
{
    QMetaObject::activate(this, &staticMetaObject, 13, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
