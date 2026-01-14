/****************************************************************************
** Meta object code from reading C++ file 'disp3dpoint.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../Test3-8CH/Disp3D/disp3dpoint.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'disp3dpoint.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_disp3dpoint_t {
    QByteArrayData data[15];
    char stringdata0[183];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_disp3dpoint_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_disp3dpoint_t qt_meta_stringdata_disp3dpoint = {
    {
QT_MOC_LITERAL(0, 0, 11), // "disp3dpoint"
QT_MOC_LITERAL(1, 12, 11), // "delete_node"
QT_MOC_LITERAL(2, 24, 0), // ""
QT_MOC_LITERAL(3, 25, 6), // "nodeNo"
QT_MOC_LITERAL(4, 32, 15), // "deal_selectNode"
QT_MOC_LITERAL(5, 48, 3), // "num"
QT_MOC_LITERAL(6, 52, 1), // "x"
QT_MOC_LITERAL(7, 54, 1), // "y"
QT_MOC_LITERAL(8, 56, 1), // "z"
QT_MOC_LITERAL(9, 58, 13), // "deal_editNode"
QT_MOC_LITERAL(10, 72, 3), // "ini"
QT_MOC_LITERAL(11, 76, 24), // "on_pbtn_AddPoint_clicked"
QT_MOC_LITERAL(12, 101, 27), // "on_pbtn_DelectPoint_clicked"
QT_MOC_LITERAL(13, 129, 25), // "on_pbtn_EditPoint_clicked"
QT_MOC_LITERAL(14, 155, 27) // "on_pbtn_EditPoint_2_clicked"

    },
    "disp3dpoint\0delete_node\0\0nodeNo\0"
    "deal_selectNode\0num\0x\0y\0z\0deal_editNode\0"
    "ini\0on_pbtn_AddPoint_clicked\0"
    "on_pbtn_DelectPoint_clicked\0"
    "on_pbtn_EditPoint_clicked\0"
    "on_pbtn_EditPoint_2_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_disp3dpoint[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   49,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    4,   52,    2, 0x08 /* Private */,
       9,    5,   61,    2, 0x08 /* Private */,
      11,    0,   72,    2, 0x08 /* Private */,
      12,    0,   73,    2, 0x08 /* Private */,
      13,    0,   74,    2, 0x08 /* Private */,
      14,    0,   75,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int,    3,

 // slots: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::Double, QMetaType::Double, QMetaType::Double,    5,    6,    7,    8,
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Double, QMetaType::Double, QMetaType::Double,   10,    5,    6,    7,    8,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void disp3dpoint::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<disp3dpoint *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->delete_node((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->deal_selectNode((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2])),(*reinterpret_cast< double(*)>(_a[3])),(*reinterpret_cast< double(*)>(_a[4]))); break;
        case 2: _t->deal_editNode((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< double(*)>(_a[3])),(*reinterpret_cast< double(*)>(_a[4])),(*reinterpret_cast< double(*)>(_a[5]))); break;
        case 3: _t->on_pbtn_AddPoint_clicked(); break;
        case 4: _t->on_pbtn_DelectPoint_clicked(); break;
        case 5: _t->on_pbtn_EditPoint_clicked(); break;
        case 6: _t->on_pbtn_EditPoint_2_clicked(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (disp3dpoint::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&disp3dpoint::delete_node)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject disp3dpoint::staticMetaObject = { {
    &QDialog::staticMetaObject,
    qt_meta_stringdata_disp3dpoint.data,
    qt_meta_data_disp3dpoint,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *disp3dpoint::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *disp3dpoint::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_disp3dpoint.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int disp3dpoint::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
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
void disp3dpoint::delete_node(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
