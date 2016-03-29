/****************************************************************************
** Meta object code from reading C++ file 'AlarmTab.hpp'
**
** Created: Wed Jul 15 11:30:05 2015
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "AlarmTab.hpp"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'AlarmTab.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CAlarmTab[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      22,   11,   10,   10, 0x09,
      55,   10,   10,   10, 0x09,
      80,   10,   10,   10, 0x09,

       0        // eod
};

static const char qt_meta_stringdata_CAlarmTab[] = {
    "CAlarmTab\0\0row,column\0"
    "alarmResItemChangedSlot(int,int)\0"
    "onConfirmButtonClicked()\0"
    "onCleanButtonClicked()\0"
};

void CAlarmTab::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CAlarmTab *_t = static_cast<CAlarmTab *>(_o);
        switch (_id) {
        case 0: _t->alarmResItemChangedSlot((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 1: _t->onConfirmButtonClicked(); break;
        case 2: _t->onCleanButtonClicked(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CAlarmTab::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CAlarmTab::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_CAlarmTab,
      qt_meta_data_CAlarmTab, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CAlarmTab::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CAlarmTab::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CAlarmTab::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CAlarmTab))
        return static_cast<void*>(const_cast< CAlarmTab*>(this));
    return QWidget::qt_metacast(_clname);
}

int CAlarmTab::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
