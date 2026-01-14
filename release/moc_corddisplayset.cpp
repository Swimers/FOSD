/****************************************************************************
** Meta object code from reading C++ file 'corddisplayset.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "D:/CommonFiles/Projects And Works/SummerProject/Qt_Software/StudyVersion/FOSD-mergeV/disp3d/corddisplayset.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'corddisplayset.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_CordDisplaySet_t {
    QByteArrayData data[14];
    char stringdata0[138];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CordDisplaySet_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CordDisplaySet_t qt_meta_stringdata_CordDisplaySet = {
    {
QT_MOC_LITERAL(0, 0, 14), // "CordDisplaySet"
QT_MOC_LITERAL(1, 15, 11), // "delete_node"
QT_MOC_LITERAL(2, 27, 0), // ""
QT_MOC_LITERAL(3, 28, 6), // "nodeNo"
QT_MOC_LITERAL(4, 35, 18), // "on_AddCord_clicked"
QT_MOC_LITERAL(5, 54, 21), // "on_DeleteCord_clicked"
QT_MOC_LITERAL(6, 76, 19), // "on_EditCord_clicked"
QT_MOC_LITERAL(7, 96, 13), // "deal_seleNode"
QT_MOC_LITERAL(8, 110, 3), // "num"
QT_MOC_LITERAL(9, 114, 1), // "x"
QT_MOC_LITERAL(10, 116, 1), // "y"
QT_MOC_LITERAL(11, 118, 1), // "z"
QT_MOC_LITERAL(12, 120, 13), // "deal_editNode"
QT_MOC_LITERAL(13, 134, 3) // "ini"

    },
    "CordDisplaySet\0delete_node\0\0nodeNo\0"
    "on_AddCord_clicked\0on_DeleteCord_clicked\0"
    "on_EditCord_clicked\0deal_seleNode\0num\0"
    "x\0y\0z\0deal_editNode\0ini"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CordDisplaySet[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   44,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    0,   47,    2, 0x08 /* Private */,
       5,    0,   48,    2, 0x08 /* Private */,
       6,    0,   49,    2, 0x08 /* Private */,
       7,    4,   50,    2, 0x08 /* Private */,
      12,    5,   59,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int,    3,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::Double, QMetaType::Double, QMetaType::Double,    8,    9,   10,   11,
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Double, QMetaType::Double, QMetaType::Double,   13,    8,    9,   10,   11,

       0        // eod
};

void CordDisplaySet::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<CordDisplaySet *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->delete_node((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->on_AddCord_clicked(); break;
        case 2: _t->on_DeleteCord_clicked(); break;
        case 3: _t->on_EditCord_clicked(); break;
        case 4: _t->deal_seleNode((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2])),(*reinterpret_cast< double(*)>(_a[3])),(*reinterpret_cast< double(*)>(_a[4]))); break;
        case 5: _t->deal_editNode((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< double(*)>(_a[3])),(*reinterpret_cast< double(*)>(_a[4])),(*reinterpret_cast< double(*)>(_a[5]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (CordDisplaySet::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CordDisplaySet::delete_node)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject CordDisplaySet::staticMetaObject = { {
    &QDialog::staticMetaObject,
    qt_meta_stringdata_CordDisplaySet.data,
    qt_meta_data_CordDisplaySet,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *CordDisplaySet::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CordDisplaySet::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CordDisplaySet.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int CordDisplaySet::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void CordDisplaySet::delete_node(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
