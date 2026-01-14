/****************************************************************************
** Meta object code from reading C++ file 'demowindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../demo/demowindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'demowindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_DemoWindow_t {
    QByteArrayData data[26];
    char stringdata0[434];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_DemoWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_DemoWindow_t qt_meta_stringdata_DemoWindow = {
    {
QT_MOC_LITERAL(0, 0, 10), // "DemoWindow"
QT_MOC_LITERAL(1, 11, 10), // "close_main"
QT_MOC_LITERAL(2, 22, 0), // ""
QT_MOC_LITERAL(3, 23, 17), // "initialize_widget"
QT_MOC_LITERAL(4, 41, 16), // "Updatatabel_slot"
QT_MOC_LITERAL(5, 58, 25), // "on_Button_connect_clicked"
QT_MOC_LITERAL(6, 84, 31), // "on_Button_save_spectrum_clicked"
QT_MOC_LITERAL(7, 116, 23), // "on_Button_reset_clicked"
QT_MOC_LITERAL(8, 140, 30), // "on_Button_Save_dynamic_clicked"
QT_MOC_LITERAL(9, 171, 29), // "on_Button_Save_Static_clicked"
QT_MOC_LITERAL(10, 201, 13), // "set_strain_AB"
QT_MOC_LITERAL(11, 215, 3), // "m_A"
QT_MOC_LITERAL(12, 219, 3), // "m_B"
QT_MOC_LITERAL(13, 223, 6), // "setLED"
QT_MOC_LITERAL(14, 230, 7), // "QLabel*"
QT_MOC_LITERAL(15, 238, 5), // "label"
QT_MOC_LITERAL(16, 244, 5), // "color"
QT_MOC_LITERAL(17, 250, 4), // "size"
QT_MOC_LITERAL(18, 255, 28), // "on_Button_selectshow_clicked"
QT_MOC_LITERAL(19, 284, 31), // "on_Button_parameter_set_clicked"
QT_MOC_LITERAL(20, 316, 11), // "dataReceive"
QT_MOC_LITERAL(21, 328, 21), // "on_btnrefresh_clicked"
QT_MOC_LITERAL(22, 350, 18), // "on_btnsend_clicked"
QT_MOC_LITERAL(23, 369, 19), // "on_btnclear_clicked"
QT_MOC_LITERAL(24, 389, 19), // "on_btnclose_clicked"
QT_MOC_LITERAL(25, 409, 24) // "on_btn_diagnosis_clicked"

    },
    "DemoWindow\0close_main\0\0initialize_widget\0"
    "Updatatabel_slot\0on_Button_connect_clicked\0"
    "on_Button_save_spectrum_clicked\0"
    "on_Button_reset_clicked\0"
    "on_Button_Save_dynamic_clicked\0"
    "on_Button_Save_Static_clicked\0"
    "set_strain_AB\0m_A\0m_B\0setLED\0QLabel*\0"
    "label\0color\0size\0on_Button_selectshow_clicked\0"
    "on_Button_parameter_set_clicked\0"
    "dataReceive\0on_btnrefresh_clicked\0"
    "on_btnsend_clicked\0on_btnclear_clicked\0"
    "on_btnclose_clicked\0on_btn_diagnosis_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_DemoWindow[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      18,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,  104,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    0,  105,    2, 0x08 /* Private */,
       4,    0,  106,    2, 0x08 /* Private */,
       5,    0,  107,    2, 0x08 /* Private */,
       6,    0,  108,    2, 0x08 /* Private */,
       7,    0,  109,    2, 0x08 /* Private */,
       8,    0,  110,    2, 0x08 /* Private */,
       9,    0,  111,    2, 0x08 /* Private */,
      10,    2,  112,    2, 0x08 /* Private */,
      13,    3,  117,    2, 0x08 /* Private */,
      18,    0,  124,    2, 0x08 /* Private */,
      19,    0,  125,    2, 0x08 /* Private */,
      20,    0,  126,    2, 0x08 /* Private */,
      21,    0,  127,    2, 0x08 /* Private */,
      22,    0,  128,    2, 0x08 /* Private */,
      23,    0,  129,    2, 0x08 /* Private */,
      24,    0,  130,    2, 0x08 /* Private */,
      25,    0,  131,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Bool,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Float, QMetaType::Float,   11,   12,
    QMetaType::Void, 0x80000000 | 14, QMetaType::Int, QMetaType::Int,   15,   16,   17,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void DemoWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<DemoWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->close_main(); break;
        case 1: _t->initialize_widget(); break;
        case 2: _t->Updatatabel_slot(); break;
        case 3: _t->on_Button_connect_clicked(); break;
        case 4: { bool _r = _t->on_Button_save_spectrum_clicked();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 5: _t->on_Button_reset_clicked(); break;
        case 6: _t->on_Button_Save_dynamic_clicked(); break;
        case 7: _t->on_Button_Save_Static_clicked(); break;
        case 8: _t->set_strain_AB((*reinterpret_cast< float(*)>(_a[1])),(*reinterpret_cast< float(*)>(_a[2]))); break;
        case 9: _t->setLED((*reinterpret_cast< QLabel*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 10: _t->on_Button_selectshow_clicked(); break;
        case 11: _t->on_Button_parameter_set_clicked(); break;
        case 12: _t->dataReceive(); break;
        case 13: _t->on_btnrefresh_clicked(); break;
        case 14: _t->on_btnsend_clicked(); break;
        case 15: _t->on_btnclear_clicked(); break;
        case 16: _t->on_btnclose_clicked(); break;
        case 17: _t->on_btn_diagnosis_clicked(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 9:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QLabel* >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (DemoWindow::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&DemoWindow::close_main)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject DemoWindow::staticMetaObject = { {
    &QWidget::staticMetaObject,
    qt_meta_stringdata_DemoWindow.data,
    qt_meta_data_DemoWindow,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *DemoWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *DemoWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_DemoWindow.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int DemoWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 18)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 18;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 18)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 18;
    }
    return _id;
}

// SIGNAL 0
void DemoWindow::close_main()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
