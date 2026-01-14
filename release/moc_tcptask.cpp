/****************************************************************************
** Meta object code from reading C++ file 'tcptask.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../Test3-8CH/Communication/tcptask.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'tcptask.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_tcptask_t {
    QByteArrayData data[19];
    char stringdata0[231];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_tcptask_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_tcptask_t qt_meta_stringdata_tcptask = {
    {
QT_MOC_LITERAL(0, 0, 7), // "tcptask"
QT_MOC_LITERAL(1, 8, 23), // "connectionStatusChanged"
QT_MOC_LITERAL(2, 32, 0), // ""
QT_MOC_LITERAL(3, 33, 25), // "tcptask::ConnectionStatus"
QT_MOC_LITERAL(4, 59, 6), // "status"
QT_MOC_LITERAL(5, 66, 15), // "tcpDataReceived"
QT_MOC_LITERAL(6, 82, 4), // "data"
QT_MOC_LITERAL(7, 87, 18), // "tcpPocketsReceived"
QT_MOC_LITERAL(8, 106, 8), // "datagram"
QT_MOC_LITERAL(9, 115, 7), // "tcpOpen"
QT_MOC_LITERAL(10, 123, 12), // "QHostAddress"
QT_MOC_LITERAL(11, 136, 7), // "address"
QT_MOC_LITERAL(12, 144, 4), // "port"
QT_MOC_LITERAL(13, 149, 8), // "tcpClose"
QT_MOC_LITERAL(14, 158, 11), // "tcpSendData"
QT_MOC_LITERAL(15, 170, 4), // "text"
QT_MOC_LITERAL(16, 175, 10), // "tcpRecData"
QT_MOC_LITERAL(17, 186, 19), // "handleNewConnection"
QT_MOC_LITERAL(18, 206, 24) // "handleClientDisconnected"

    },
    "tcptask\0connectionStatusChanged\0\0"
    "tcptask::ConnectionStatus\0status\0"
    "tcpDataReceived\0data\0tcpPocketsReceived\0"
    "datagram\0tcpOpen\0QHostAddress\0address\0"
    "port\0tcpClose\0tcpSendData\0text\0"
    "tcpRecData\0handleNewConnection\0"
    "handleClientDisconnected"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_tcptask[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   59,    2, 0x06 /* Public */,
       5,    1,   62,    2, 0x06 /* Public */,
       7,    1,   65,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       9,    2,   68,    2, 0x0a /* Public */,
      13,    0,   73,    2, 0x0a /* Public */,
      14,    1,   74,    2, 0x0a /* Public */,
      16,    0,   77,    2, 0x08 /* Private */,
      17,    0,   78,    2, 0x08 /* Private */,
      18,    0,   79,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, QMetaType::QString,    6,
    QMetaType::Void, QMetaType::QByteArray,    8,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 10, QMetaType::UShort,   11,   12,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   15,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void tcptask::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<tcptask *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->connectionStatusChanged((*reinterpret_cast< tcptask::ConnectionStatus(*)>(_a[1]))); break;
        case 1: _t->tcpDataReceived((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: _t->tcpPocketsReceived((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 3: _t->tcpOpen((*reinterpret_cast< const QHostAddress(*)>(_a[1])),(*reinterpret_cast< const quint16(*)>(_a[2]))); break;
        case 4: _t->tcpClose(); break;
        case 5: _t->tcpSendData((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 6: _t->tcpRecData(); break;
        case 7: _t->handleNewConnection(); break;
        case 8: _t->handleClientDisconnected(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (tcptask::*)(tcptask::ConnectionStatus );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&tcptask::connectionStatusChanged)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (tcptask::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&tcptask::tcpDataReceived)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (tcptask::*)(const QByteArray );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&tcptask::tcpPocketsReceived)) {
                *result = 2;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject tcptask::staticMetaObject = { {
    &QObject::staticMetaObject,
    qt_meta_stringdata_tcptask.data,
    qt_meta_data_tcptask,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *tcptask::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *tcptask::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_tcptask.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int tcptask::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void tcptask::connectionStatusChanged(tcptask::ConnectionStatus _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void tcptask::tcpDataReceived(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void tcptask::tcpPocketsReceived(const QByteArray _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
