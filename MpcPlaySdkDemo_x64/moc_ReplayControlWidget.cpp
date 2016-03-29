/****************************************************************************
** Meta object code from reading C++ file 'ReplayControlWidget.hpp'
**
** Created: Wed Jul 15 11:30:01 2015
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "ReplayControlWidget.hpp"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ReplayControlWidget.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CReplayControlWidget[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      22,   21,   21,   21, 0x08,
      44,   38,   21,   21, 0x08,
      71,   65,   21,   21, 0x08,
      92,   65,   21,   21, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_CReplayControlWidget[] = {
    "CReplayControlWidget\0\0onButtonClick()\0"
    "state\0onCheckBoxMute(bool)\0value\0"
    "onVolumeChanged(int)\0onSpeedChanged(int)\0"
};

void CReplayControlWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CReplayControlWidget *_t = static_cast<CReplayControlWidget *>(_o);
        switch (_id) {
        case 0: _t->onButtonClick(); break;
        case 1: _t->onCheckBoxMute((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: _t->onVolumeChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->onSpeedChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CReplayControlWidget::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CReplayControlWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_CReplayControlWidget,
      qt_meta_data_CReplayControlWidget, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CReplayControlWidget::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CReplayControlWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CReplayControlWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CReplayControlWidget))
        return static_cast<void*>(const_cast< CReplayControlWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int CReplayControlWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
