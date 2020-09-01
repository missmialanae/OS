/****************************************************************************
** Meta object code from reading C++ file 'monitor_window.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "frontends/qmps/monitor_window.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'monitor_window.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MonitorWindow_t {
    QByteArrayData data[29];
    char stringdata0[428];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MonitorWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MonitorWindow_t qt_meta_stringdata_MonitorWindow = {
    {
QT_MOC_LITERAL(0, 0, 13), // "MonitorWindow"
QT_MOC_LITERAL(1, 14, 14), // "onCreateConfig"
QT_MOC_LITERAL(2, 29, 0), // ""
QT_MOC_LITERAL(3, 30, 12), // "onLoadConfig"
QT_MOC_LITERAL(4, 43, 12), // "clearConfigs"
QT_MOC_LITERAL(5, 56, 18), // "onLoadRecentConfig"
QT_MOC_LITERAL(6, 75, 10), // "editConfig"
QT_MOC_LITERAL(7, 86, 17), // "onStopMaskChanged"
QT_MOC_LITERAL(8, 104, 20), // "onSpeedActionChecked"
QT_MOC_LITERAL(9, 125, 14), // "onSpeedChanged"
QT_MOC_LITERAL(10, 140, 5), // "speed"
QT_MOC_LITERAL(11, 146, 13), // "increaseSpeed"
QT_MOC_LITERAL(12, 160, 13), // "decreaseSpeed"
QT_MOC_LITERAL(13, 174, 13), // "showCpuWindow"
QT_MOC_LITERAL(14, 188, 5), // "cpuId"
QT_MOC_LITERAL(15, 194, 18), // "onCpuItemActivated"
QT_MOC_LITERAL(16, 213, 11), // "QModelIndex"
QT_MOC_LITERAL(17, 225, 5), // "index"
QT_MOC_LITERAL(18, 231, 12), // "showTerminal"
QT_MOC_LITERAL(19, 244, 13), // "showAboutInfo"
QT_MOC_LITERAL(20, 258, 22), // "onMachineConfigChanged"
QT_MOC_LITERAL(21, 281, 16), // "onMachineStarted"
QT_MOC_LITERAL(22, 298, 15), // "onMachineHalted"
QT_MOC_LITERAL(23, 314, 33), // "updateStoppointActionsSensiti..."
QT_MOC_LITERAL(24, 348, 15), // "onAddBreakpoint"
QT_MOC_LITERAL(25, 364, 18), // "onRemoveBreakpoint"
QT_MOC_LITERAL(26, 383, 12), // "onAddSuspect"
QT_MOC_LITERAL(27, 396, 15), // "onRemoveSuspect"
QT_MOC_LITERAL(28, 412, 15) // "onAddTracepoint"

    },
    "MonitorWindow\0onCreateConfig\0\0"
    "onLoadConfig\0clearConfigs\0onLoadRecentConfig\0"
    "editConfig\0onStopMaskChanged\0"
    "onSpeedActionChecked\0onSpeedChanged\0"
    "speed\0increaseSpeed\0decreaseSpeed\0"
    "showCpuWindow\0cpuId\0onCpuItemActivated\0"
    "QModelIndex\0index\0showTerminal\0"
    "showAboutInfo\0onMachineConfigChanged\0"
    "onMachineStarted\0onMachineHalted\0"
    "updateStoppointActionsSensitivity\0"
    "onAddBreakpoint\0onRemoveBreakpoint\0"
    "onAddSuspect\0onRemoveSuspect\0"
    "onAddTracepoint"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MonitorWindow[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      23,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,  129,    2, 0x08 /* Private */,
       3,    0,  130,    2, 0x08 /* Private */,
       4,    0,  131,    2, 0x08 /* Private */,
       5,    0,  132,    2, 0x08 /* Private */,
       6,    0,  133,    2, 0x08 /* Private */,
       7,    0,  134,    2, 0x08 /* Private */,
       8,    0,  135,    2, 0x08 /* Private */,
       9,    1,  136,    2, 0x08 /* Private */,
      11,    0,  139,    2, 0x08 /* Private */,
      12,    0,  140,    2, 0x08 /* Private */,
      13,    1,  141,    2, 0x08 /* Private */,
      15,    1,  144,    2, 0x08 /* Private */,
      18,    0,  147,    2, 0x08 /* Private */,
      19,    0,  148,    2, 0x08 /* Private */,
      20,    0,  149,    2, 0x08 /* Private */,
      21,    0,  150,    2, 0x08 /* Private */,
      22,    0,  151,    2, 0x08 /* Private */,
      23,    0,  152,    2, 0x08 /* Private */,
      24,    0,  153,    2, 0x08 /* Private */,
      25,    0,  154,    2, 0x08 /* Private */,
      26,    0,  155,    2, 0x08 /* Private */,
      27,    0,  156,    2, 0x08 /* Private */,
      28,    0,  157,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   10,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   14,
    QMetaType::Void, 0x80000000 | 16,   17,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
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

void MonitorWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MonitorWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->onCreateConfig(); break;
        case 1: _t->onLoadConfig(); break;
        case 2: _t->clearConfigs(); break;
        case 3: _t->onLoadRecentConfig(); break;
        case 4: _t->editConfig(); break;
        case 5: _t->onStopMaskChanged(); break;
        case 6: _t->onSpeedActionChecked(); break;
        case 7: _t->onSpeedChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: _t->increaseSpeed(); break;
        case 9: _t->decreaseSpeed(); break;
        case 10: _t->showCpuWindow((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 11: _t->onCpuItemActivated((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        case 12: _t->showTerminal(); break;
        case 13: _t->showAboutInfo(); break;
        case 14: _t->onMachineConfigChanged(); break;
        case 15: _t->onMachineStarted(); break;
        case 16: _t->onMachineHalted(); break;
        case 17: _t->updateStoppointActionsSensitivity(); break;
        case 18: _t->onAddBreakpoint(); break;
        case 19: _t->onRemoveBreakpoint(); break;
        case 20: _t->onAddSuspect(); break;
        case 21: _t->onRemoveSuspect(); break;
        case 22: _t->onAddTracepoint(); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject MonitorWindow::staticMetaObject = { {
    &QMainWindow::staticMetaObject,
    qt_meta_stringdata_MonitorWindow.data,
    qt_meta_data_MonitorWindow,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *MonitorWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MonitorWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MonitorWindow.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int MonitorWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 23)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 23;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 23)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 23;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
