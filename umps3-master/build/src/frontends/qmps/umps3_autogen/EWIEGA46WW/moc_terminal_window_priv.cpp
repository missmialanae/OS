/****************************************************************************
** Meta object code from reading C++ file 'terminal_window_priv.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "frontends/qmps/terminal_window_priv.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'terminal_window_priv.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_TerminalStatusWidget_t {
    QByteArrayData data[5];
    char stringdata0[85];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_TerminalStatusWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_TerminalStatusWidget_t qt_meta_stringdata_TerminalStatusWidget = {
    {
QT_MOC_LITERAL(0, 0, 20), // "TerminalStatusWidget"
QT_MOC_LITERAL(1, 21, 30), // "onHardwareFailureButtonClicked"
QT_MOC_LITERAL(2, 52, 0), // ""
QT_MOC_LITERAL(3, 53, 7), // "checked"
QT_MOC_LITERAL(4, 61, 23) // "onExpanderButtonClicked"

    },
    "TerminalStatusWidget\0"
    "onHardwareFailureButtonClicked\0\0checked\0"
    "onExpanderButtonClicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_TerminalStatusWidget[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   24,    2, 0x08 /* Private */,
       4,    0,   27,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, QMetaType::Bool,    3,
    QMetaType::Void,

       0        // eod
};

void TerminalStatusWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<TerminalStatusWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->onHardwareFailureButtonClicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: _t->onExpanderButtonClicked(); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject TerminalStatusWidget::staticMetaObject = { {
    &QWidget::staticMetaObject,
    qt_meta_stringdata_TerminalStatusWidget.data,
    qt_meta_data_TerminalStatusWidget,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *TerminalStatusWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TerminalStatusWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_TerminalStatusWidget.stringdata0))
        return static_cast<void*>(this);
    if (!strcmp(_clname, "sigc::trackable"))
        return static_cast< sigc::trackable*>(this);
    return QWidget::qt_metacast(_clname);
}

int TerminalStatusWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 2;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE