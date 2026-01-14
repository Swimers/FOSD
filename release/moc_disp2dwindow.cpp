/****************************************************************************
** Meta object code from reading C++ file 'disp2dwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "D:/CommonFiles/Projects And Works/SummerProject/Qt_Software/StudyVersion/FOSD-mergeV/disp2d/disp2dwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'disp2dwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Disp2DWindow_t {
    QByteArrayData data[7];
    char stringdata0[165];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Disp2DWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Disp2DWindow_t qt_meta_stringdata_Disp2DWindow = {
    {
QT_MOC_LITERAL(0, 0, 12), // "Disp2DWindow"
QT_MOC_LITERAL(1, 13, 43), // "on_Button_ChangeOriginalCoord..."
QT_MOC_LITERAL(2, 57, 0), // ""
QT_MOC_LITERAL(3, 58, 34), // "on_Button_ChangeBackGround_cl..."
QT_MOC_LITERAL(4, 93, 25), // "on_Button_Observe_clicked"
QT_MOC_LITERAL(5, 119, 16), // "Updatatabel_slot"
QT_MOC_LITERAL(6, 136, 28) // "on_Button_ParaRevise_clicked"

    },
    "Disp2DWindow\0on_Button_ChangeOriginalCoordinates_clicked\0"
    "\0on_Button_ChangeBackGround_clicked\0"
    "on_Button_Observe_clicked\0Updatatabel_slot\0"
    "on_Button_ParaRevise_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Disp2DWindow[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   39,    2, 0x08 /* Private */,
       3,    0,   40,    2, 0x08 /* Private */,
       4,    0,   41,    2, 0x08 /* Private */,
       5,    0,   42,    2, 0x08 /* Private */,
       6,    0,   43,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void Disp2DWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<Disp2DWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->on_Button_ChangeOriginalCoordinates_clicked(); break;
        case 1: _t->on_Button_ChangeBackGround_clicked(); break;
        case 2: _t->on_Button_Observe_clicked(); break;
        case 3: _t->Updatatabel_slot(); break;
        case 4: _t->on_Button_ParaRevise_clicked(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject Disp2DWindow::staticMetaObject = { {
    &QWidget::staticMetaObject,
    qt_meta_stringdata_Disp2DWindow.data,
    qt_meta_data_Disp2DWindow,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *Disp2DWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Disp2DWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Disp2DWindow.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int Disp2DWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 5;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
