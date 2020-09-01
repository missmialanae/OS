/****************************************************************************
** Meta object code from reading C++ file 'debug_session.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "frontends/qmps/debug_session.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'debug_session.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_DebugSession_t {
    QByteArrayData data[25];
    char stringdata0[316];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_DebugSession_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_DebugSession_t qt_meta_stringdata_DebugSession = {
    {
QT_MOC_LITERAL(0, 0, 12), // "DebugSession"
QT_MOC_LITERAL(1, 13, 13), // "StatusChanged"
QT_MOC_LITERAL(2, 27, 0), // ""
QT_MOC_LITERAL(3, 28, 14), // "MachineStarted"
QT_MOC_LITERAL(4, 43, 14), // "MachineStopped"
QT_MOC_LITERAL(5, 58, 10), // "MachineRan"
QT_MOC_LITERAL(6, 69, 13), // "MachineHalted"
QT_MOC_LITERAL(7, 83, 12), // "MachineReset"
QT_MOC_LITERAL(8, 96, 23), // "DebugIterationCompleted"
QT_MOC_LITERAL(9, 120, 12), // "SpeedChanged"
QT_MOC_LITERAL(10, 133, 11), // "setStopMask"
QT_MOC_LITERAL(11, 145, 5), // "value"
QT_MOC_LITERAL(12, 151, 8), // "setSpeed"
QT_MOC_LITERAL(13, 160, 4), // "stop"
QT_MOC_LITERAL(14, 165, 22), // "onMachineConfigChanged"
QT_MOC_LITERAL(15, 188, 12), // "startMachine"
QT_MOC_LITERAL(16, 201, 13), // "onHaltMachine"
QT_MOC_LITERAL(17, 215, 13), // "toggleMachine"
QT_MOC_LITERAL(18, 229, 14), // "onResetMachine"
QT_MOC_LITERAL(19, 244, 10), // "onContinue"
QT_MOC_LITERAL(20, 255, 6), // "onStep"
QT_MOC_LITERAL(21, 262, 11), // "toggleDebug"
QT_MOC_LITERAL(22, 274, 23), // "updateActionSensitivity"
QT_MOC_LITERAL(23, 298, 12), // "runIteration"
QT_MOC_LITERAL(24, 311, 4) // "skip"

    },
    "DebugSession\0StatusChanged\0\0MachineStarted\0"
    "MachineStopped\0MachineRan\0MachineHalted\0"
    "MachineReset\0DebugIterationCompleted\0"
    "SpeedChanged\0setStopMask\0value\0setSpeed\0"
    "stop\0onMachineConfigChanged\0startMachine\0"
    "onHaltMachine\0toggleMachine\0onResetMachine\0"
    "onContinue\0onStep\0toggleDebug\0"
    "updateActionSensitivity\0runIteration\0"
    "skip"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_DebugSession[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      22,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       8,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,  124,    2, 0x06 /* Public */,
       3,    0,  125,    2, 0x06 /* Public */,
       4,    0,  126,    2, 0x06 /* Public */,
       5,    0,  127,    2, 0x06 /* Public */,
       6,    0,  128,    2, 0x06 /* Public */,
       7,    0,  129,    2, 0x06 /* Public */,
       8,    0,  130,    2, 0x06 /* Public */,
       9,    1,  131,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      10,    1,  134,    2, 0x0a /* Public */,
      12,    1,  137,    2, 0x0a /* Public */,
      13,    0,  140,    2, 0x0a /* Public */,
      14,    0,  141,    2, 0x08 /* Private */,
      15,    0,  142,    2, 0x08 /* Private */,
      16,    0,  143,    2, 0x08 /* Private */,
      17,    0,  144,    2, 0x08 /* Private */,
      18,    0,  145,    2, 0x08 /* Private */,
      19,    0,  146,    2, 0x08 /* Private */,
      20,    0,  147,    2, 0x08 /* Private */,
      21,    0,  148,    2, 0x08 /* Private */,
      22,    0,  149,    2, 0x08 /* Private */,
      23,    0,  150,    2, 0x08 /* Private */,
      24,    0,  151,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    2,

 // slots: parameters
    QMetaType::Void, QMetaType::UInt,   11,
    QMetaType::Void, QMetaType::Int,   11,
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
    QMetaType::Void,

       0        // eod
};

void DebugSession::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<DebugSession *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->StatusChanged(); break;
        case 1: _t->MachineStarted(); break;
        case 2: _t->MachineStopped(); break;
        case 3: _t->MachineRan(); break;
        case 4: _t->MachineHalted(); break;
        case 5: _t->MachineReset(); break;
        case 6: _t->DebugIterationCompleted(); break;
        case 7: _t->SpeedChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: _t->setStopMask((*reinterpret_cast< uint(*)>(_a[1]))); break;
        case 9: _t->setSpeed((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 10: _t->stop(); break;
        case 11: _t->onMachineConfigChanged(); break;
        case 12: _t->startMachine(); break;
        case 13: _t->onHaltMachine(); break;
        case 14: _t->toggleMachine(); break;
        case 15: _t->onResetMachine(); break;
        case 16: _t->onContinue(); break;
        case 17: _t->onStep(); break;
        case 18: _t->toggleDebug(); break;
        case 19: _t->updateActionSensitivity(); break;
        case 20: _t->runIteration(); break;
        case 21: _t->skip(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (DebugSession::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&DebugSession::StatusChanged)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (DebugSession::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&DebugSession::MachineStarted)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (DebugSession::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&DebugSession::MachineStopped)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (DebugSession::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&DebugSession::MachineRan)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (DebugSession::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&DebugSession::MachineHalted)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (DebugSession::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&DebugSession::MachineReset)) {
                *result = 5;
                return;
            }
        }
        {
            using _t = void (DebugSession::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&DebugSession::DebugIterationCompleted)) {
                *result = 6;
                return;
            }
        }
        {
            using _t = void (DebugSession::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&DebugSession::SpeedChanged)) {
                *result = 7;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject DebugSession::staticMetaObject = { {
    &QObject::staticMetaObject,
    qt_meta_stringdata_DebugSession.data,
    qt_meta_data_DebugSession,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *DebugSession::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *DebugSession::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_DebugSession.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int DebugSession::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 22)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 22;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 22)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 22;
    }
    return _id;
}

// SIGNAL 0
void DebugSession::StatusChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void DebugSession::MachineStarted()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void DebugSession::MachineStopped()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void DebugSession::MachineRan()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void DebugSession::MachineHalted()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}

// SIGNAL 5
void DebugSession::MachineReset()
{
    QMetaObject::activate(this, &staticMetaObject, 5, nullptr);
}

// SIGNAL 6
void DebugSession::DebugIterationCompleted()
{
    QMetaObject::activate(this, &staticMetaObject, 6, nullptr);
}

// SIGNAL 7
void DebugSession::SpeedChanged(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
