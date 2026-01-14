/****************************************************************************
** Meta object code from reading C++ file 'disp3dkeypointedit.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Disp3D/disp3dkeypointedit.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'disp3dkeypointedit.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_disp3dkeypointedit_t {
    QByteArrayData data[12];
    char stringdata0[258];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_disp3dkeypointedit_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_disp3dkeypointedit_t qt_meta_stringdata_disp3dkeypointedit = {
    {
QT_MOC_LITERAL(0, 0, 18), // "disp3dkeypointedit"
QT_MOC_LITERAL(1, 19, 13), // "SelectSuccess"
QT_MOC_LITERAL(2, 33, 0), // ""
QT_MOC_LITERAL(3, 34, 38), // "on_cbox_IDKeyPoint_currentInd..."
QT_MOC_LITERAL(4, 73, 4), // "arg1"
QT_MOC_LITERAL(5, 78, 28), // "on_pbtn_FindKeyPoint_clicked"
QT_MOC_LITERAL(6, 107, 25), // "on_pbtn_SelectAll_clicked"
QT_MOC_LITERAL(7, 133, 24), // "on_pbtn_ClearAll_clicked"
QT_MOC_LITERAL(8, 158, 24), // "on_pbtn_DispCube_clicked"
QT_MOC_LITERAL(9, 183, 26), // "on_pbtn_SelectCube_clicked"
QT_MOC_LITERAL(10, 210, 25), // "on_pbtn_ClearCube_clicked"
QT_MOC_LITERAL(11, 236, 21) // "on_pushButton_clicked"

    },
    "disp3dkeypointedit\0SelectSuccess\0\0"
    "on_cbox_IDKeyPoint_currentIndexChanged\0"
    "arg1\0on_pbtn_FindKeyPoint_clicked\0"
    "on_pbtn_SelectAll_clicked\0"
    "on_pbtn_ClearAll_clicked\0"
    "on_pbtn_DispCube_clicked\0"
    "on_pbtn_SelectCube_clicked\0"
    "on_pbtn_ClearCube_clicked\0"
    "on_pushButton_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_disp3dkeypointedit[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   59,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    1,   60,    2, 0x08 /* Private */,
       5,    0,   63,    2, 0x08 /* Private */,
       6,    0,   64,    2, 0x08 /* Private */,
       7,    0,   65,    2, 0x08 /* Private */,
       8,    0,   66,    2, 0x08 /* Private */,
       9,    0,   67,    2, 0x08 /* Private */,
      10,    0,   68,    2, 0x08 /* Private */,
      11,    0,   69,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,    4,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void disp3dkeypointedit::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<disp3dkeypointedit *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->SelectSuccess(); break;
        case 1: _t->on_cbox_IDKeyPoint_currentIndexChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: _t->on_pbtn_FindKeyPoint_clicked(); break;
        case 3: _t->on_pbtn_SelectAll_clicked(); break;
        case 4: _t->on_pbtn_ClearAll_clicked(); break;
        case 5: _t->on_pbtn_DispCube_clicked(); break;
        case 6: _t->on_pbtn_SelectCube_clicked(); break;
        case 7: _t->on_pbtn_ClearCube_clicked(); break;
        case 8: _t->on_pushButton_clicked(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (disp3dkeypointedit::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&disp3dkeypointedit::SelectSuccess)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject disp3dkeypointedit::staticMetaObject = { {
    &QWidget::staticMetaObject,
    qt_meta_stringdata_disp3dkeypointedit.data,
    qt_meta_data_disp3dkeypointedit,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *disp3dkeypointedit::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *disp3dkeypointedit::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_disp3dkeypointedit.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int disp3dkeypointedit::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
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
void disp3dkeypointedit::SelectSuccess()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
