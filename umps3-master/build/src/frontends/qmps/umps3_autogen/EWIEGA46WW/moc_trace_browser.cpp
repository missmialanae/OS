/****************************************************************************
** Meta object code from reading C++ file 'trace_browser.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "frontends/qmps/trace_browser.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'trace_browser.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_TraceBrowser_t {
    QByteArrayData data[11];
    char stringdata0[156];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_TraceBrowser_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_TraceBrowser_t qt_meta_stringdata_TraceBrowser = {
    {
QT_MOC_LITERAL(0, 0, 12), // "TraceBrowser"
QT_MOC_LITERAL(1, 13, 16), // "onMachineStarted"
QT_MOC_LITERAL(2, 30, 0), // ""
QT_MOC_LITERAL(3, 31, 15), // "onMachineHalted"
QT_MOC_LITERAL(4, 47, 17), // "onTracepointAdded"
QT_MOC_LITERAL(5, 65, 16), // "removeTracepoint"
QT_MOC_LITERAL(6, 82, 18), // "onSelectionChanged"
QT_MOC_LITERAL(7, 101, 14), // "QItemSelection"
QT_MOC_LITERAL(8, 116, 21), // "onDelegateTypeChanged"
QT_MOC_LITERAL(9, 138, 5), // "index"
QT_MOC_LITERAL(10, 144, 11) // "refreshView"

    },
    "TraceBrowser\0onMachineStarted\0\0"
    "onMachineHalted\0onTracepointAdded\0"
    "removeTracepoint\0onSelectionChanged\0"
    "QItemSelection\0onDelegateTypeChanged\0"
    "index\0refreshView"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_TraceBrowser[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   49,    2, 0x08 /* Private */,
       3,    0,   50,    2, 0x08 /* Private */,
       4,    0,   51,    2, 0x08 /* Private */,
       5,    0,   52,    2, 0x08 /* Private */,
       6,    1,   53,    2, 0x08 /* Private */,
       8,    1,   56,    2, 0x08 /* Private */,
      10,    0,   59,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 7,    2,
    QMetaType::Void, QMetaType::Int,    9,
    QMetaType::Void,

       0        // eod
};

void TraceBrowser::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<TraceBrowser *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->onMachineStarted(); break;
        case 1: _t->onMachineHalted(); break;
        case 2: _t->onTracepointAdded(); break;
        case 3: _t->removeTracepoint(); break;
        case 4: _t->onSelectionChanged((*reinterpret_cast< const QItemSelection(*)>(_a[1]))); break;
        case 5: _t->onDelegateTypeChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->refreshView(); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject TraceBrowser::staticMetaObject = { {
    &QWidget::staticMetaObject,
    qt_meta_stringdata_TraceBrowser.data,
    qt_meta_data_TraceBrowser,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *TraceBrowser::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TraceBrowser::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_TraceBrowser.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int TraceBrowser::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
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
QT_WARNING_POP
QT_END_MOC_NAMESPACE
