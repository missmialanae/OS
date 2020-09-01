/****************************************************************************
** Meta object code from reading C++ file 'trace_browser_priv.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "frontends/qmps/trace_browser_priv.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'trace_browser_priv.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_TracepointListModel_t {
    QByteArrayData data[1];
    char stringdata0[20];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_TracepointListModel_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_TracepointListModel_t qt_meta_stringdata_TracepointListModel = {
    {
QT_MOC_LITERAL(0, 0, 19) // "TracepointListModel"

    },
    "TracepointListModel"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_TracepointListModel[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

void TracepointListModel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject TracepointListModel::staticMetaObject = { {
    &BaseStoppointListModel::staticMetaObject,
    qt_meta_stringdata_TracepointListModel.data,
    qt_meta_data_TracepointListModel,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *TracepointListModel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TracepointListModel::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_TracepointListModel.stringdata0))
        return static_cast<void*>(this);
    if (!strcmp(_clname, "TrackableMixin"))
        return static_cast< TrackableMixin*>(this);
    return BaseStoppointListModel::qt_metacast(_clname);
}

int TracepointListModel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = BaseStoppointListModel::qt_metacall(_c, _id, _a);
    return _id;
}
struct qt_meta_stringdata_AsciiView_t {
    QByteArrayData data[1];
    char stringdata0[10];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_AsciiView_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_AsciiView_t qt_meta_stringdata_AsciiView = {
    {
QT_MOC_LITERAL(0, 0, 9) // "AsciiView"

    },
    "AsciiView"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_AsciiView[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

void AsciiView::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject AsciiView::staticMetaObject = { {
    &QPlainTextEdit::staticMetaObject,
    qt_meta_stringdata_AsciiView.data,
    qt_meta_data_AsciiView,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *AsciiView::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AsciiView::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_AsciiView.stringdata0))
        return static_cast<void*>(this);
    if (!strcmp(_clname, "MemoryViewDelegate"))
        return static_cast< MemoryViewDelegate*>(this);
    return QPlainTextEdit::qt_metacast(_clname);
}

int AsciiView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QPlainTextEdit::qt_metacall(_c, _id, _a);
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
