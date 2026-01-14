/****************************************************************************
** Meta object code from reading C++ file 'udptask.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Communication/udptask.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'udptask.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_udptask_t {
    QByteArrayData data[18];
    char stringdata0[181];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_udptask_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_udptask_t qt_meta_stringdata_udptask = {
    {
QT_MOC_LITERAL(0, 0, 7), // "udptask"
QT_MOC_LITERAL(1, 8, 23), // "connectionStatusChanged"
QT_MOC_LITERAL(2, 32, 0), // ""
QT_MOC_LITERAL(3, 33, 9), // "connected"
QT_MOC_LITERAL(4, 43, 15), // "udpDataReceived"
QT_MOC_LITERAL(5, 59, 4), // "data"
QT_MOC_LITERAL(6, 64, 18), // "udpPocketsReceived"
QT_MOC_LITERAL(7, 83, 8), // "datagram"
QT_MOC_LITERAL(8, 92, 7), // "udpOpen"
QT_MOC_LITERAL(9, 100, 12), // "QHostAddress"
QT_MOC_LITERAL(10, 113, 7), // "address"
QT_MOC_LITERAL(11, 121, 4), // "port"
QT_MOC_LITERAL(12, 126, 8), // "udpClose"
QT_MOC_LITERAL(13, 135, 11), // "udpSendData"
QT_MOC_LITERAL(14, 147, 4), // "text"
QT_MOC_LITERAL(15, 152, 9), // "IPAddress"
QT_MOC_LITERAL(16, 162, 7), // "PortNum"
QT_MOC_LITERAL(17, 170, 10) // "udpRecData"

    },
    "udptask\0connectionStatusChanged\0\0"
    "connected\0udpDataReceived\0data\0"
    "udpPocketsReceived\0datagram\0udpOpen\0"
    "QHostAddress\0address\0port\0udpClose\0"
    "udpSendData\0text\0IPAddress\0PortNum\0"
    "udpRecData"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_udptask[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   49,    2, 0x06 /* Public */,
       4,    1,   52,    2, 0x06 /* Public */,
       6,    1,   55,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       8,    2,   58,    2, 0x0a /* Public */,
      12,    0,   63,    2, 0x0a /* Public */,
      13,    3,   64,    2, 0x0a /* Public */,
      17,    0,   71,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::Bool,    3,
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void, QMetaType::QByteArray,    7,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 9, QMetaType::UShort,   10,   11,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, 0x80000000 | 9, QMetaType::UShort,   14,   15,   16,
    QMetaType::Void,

       0        // eod
};

void udptask::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<udptask *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->connectionStatusChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: _t->udpDataReceived((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: _t->udpPocketsReceived((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 3: _t->udpOpen((*reinterpret_cast< const QHostAddress(*)>(_a[1])),(*reinterpret_cast< const quint16(*)>(_a[2]))); break;
        case 4: _t->udpClose(); break;
        case 5: _t->udpSendData((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QHostAddress(*)>(_a[2])),(*reinterpret_cast< const quint16(*)>(_a[3]))); break;
        case 6: _t->udpRecData(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (udptask::*)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&udptask::connectionStatusChanged)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (udptask::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&udptask::udpDataReceived)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (udptask::*)(const QByteArray );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&udptask::udpPocketsReceived)) {
                *result = 2;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject udptask::staticMetaObject = { {
    &QObject::staticMetaObject,
    qt_meta_stringdata_udptask.data,
    qt_meta_data_udptask,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *udptask::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *udptask::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_udptask.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int udptask::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void udptask::connectionStatusChanged(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void udptask::udpDataReceived(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void udptask::udpPocketsReceived(const QByteArray _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
