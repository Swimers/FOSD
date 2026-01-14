/****************************************************************************
** Meta object code from reading C++ file 'login.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../UserManage/login.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'login.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_loginwindow_t {
    QByteArrayData data[16];
    char stringdata0[260];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_loginwindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_loginwindow_t qt_meta_stringdata_loginwindow = {
    {
QT_MOC_LITERAL(0, 0, 11), // "loginwindow"
QT_MOC_LITERAL(1, 12, 5), // "login"
QT_MOC_LITERAL(2, 18, 0), // ""
QT_MOC_LITERAL(3, 19, 12), // "close_window"
QT_MOC_LITERAL(4, 32, 18), // "on_btn_Min_clicked"
QT_MOC_LITERAL(5, 51, 20), // "on_btn_Close_clicked"
QT_MOC_LITERAL(6, 72, 20), // "on_btn_Login_clicked"
QT_MOC_LITERAL(7, 93, 20), // "on_btn_Clear_clicked"
QT_MOC_LITERAL(8, 114, 23), // "on_btn_Register_clicked"
QT_MOC_LITERAL(9, 138, 21), // "on_btn_Forget_clicked"
QT_MOC_LITERAL(10, 160, 21), // "on_comboBox_activated"
QT_MOC_LITERAL(11, 182, 4), // "arg1"
QT_MOC_LITERAL(12, 187, 24), // "on_check_RemPass_clicked"
QT_MOC_LITERAL(13, 212, 31), // "on_comboBox_currentIndexChanged"
QT_MOC_LITERAL(14, 244, 5), // "index"
QT_MOC_LITERAL(15, 250, 9) // "LoginInit"

    },
    "loginwindow\0login\0\0close_window\0"
    "on_btn_Min_clicked\0on_btn_Close_clicked\0"
    "on_btn_Login_clicked\0on_btn_Clear_clicked\0"
    "on_btn_Register_clicked\0on_btn_Forget_clicked\0"
    "on_comboBox_activated\0arg1\0"
    "on_check_RemPass_clicked\0"
    "on_comboBox_currentIndexChanged\0index\0"
    "LoginInit"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_loginwindow[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   74,    2, 0x06 /* Public */,
       3,    0,   75,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    0,   76,    2, 0x08 /* Private */,
       5,    0,   77,    2, 0x08 /* Private */,
       6,    0,   78,    2, 0x08 /* Private */,
       7,    0,   79,    2, 0x08 /* Private */,
       8,    0,   80,    2, 0x08 /* Private */,
       9,    0,   81,    2, 0x08 /* Private */,
      10,    1,   82,    2, 0x08 /* Private */,
      12,    0,   85,    2, 0x08 /* Private */,
      13,    1,   86,    2, 0x08 /* Private */,
      15,    0,   89,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   11,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   14,
    QMetaType::Void,

       0        // eod
};

void loginwindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<loginwindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->login(); break;
        case 1: _t->close_window(); break;
        case 2: _t->on_btn_Min_clicked(); break;
        case 3: _t->on_btn_Close_clicked(); break;
        case 4: _t->on_btn_Login_clicked(); break;
        case 5: _t->on_btn_Clear_clicked(); break;
        case 6: _t->on_btn_Register_clicked(); break;
        case 7: _t->on_btn_Forget_clicked(); break;
        case 8: _t->on_comboBox_activated((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 9: _t->on_check_RemPass_clicked(); break;
        case 10: _t->on_comboBox_currentIndexChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 11: _t->LoginInit(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (loginwindow::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&loginwindow::login)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (loginwindow::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&loginwindow::close_window)) {
                *result = 1;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject loginwindow::staticMetaObject = { {
    &QMainWindow::staticMetaObject,
    qt_meta_stringdata_loginwindow.data,
    qt_meta_data_loginwindow,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *loginwindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *loginwindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_loginwindow.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int loginwindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 12)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 12;
    }
    return _id;
}

// SIGNAL 0
void loginwindow::login()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void loginwindow::close_window()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
