/****************************************************************************
** Meta object code from reading C++ file 'TimeSegmentWidget.hpp'
**
** Created: Wed Jul 15 11:30:01 2015
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "TimeSegmentWidget.hpp"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'TimeSegmentWidget.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CTimeSegmentWidget[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      20,   19,   19,   19, 0x08,
      45,   40,   19,   19, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_CTimeSegmentWidget[] = {
    "CTimeSegmentWidget\0\0OnButtonRetrieval()\0"
    "item\0OnitemDoubleClicked(QListWidgetItem*)\0"
};

void CTimeSegmentWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CTimeSegmentWidget *_t = static_cast<CTimeSegmentWidget *>(_o);
        switch (_id) {
        case 0: _t->OnButtonRetrieval(); break;
        case 1: _t->OnitemDoubleClicked((*reinterpret_cast< QListWidgetItem*(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CTimeSegmentWidget::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CTimeSegmentWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_CTimeSegmentWidget,
      qt_meta_data_CTimeSegmentWidget, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CTimeSegmentWidget::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CTimeSegmentWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CTimeSegmentWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CTimeSegmentWidget))
        return static_cast<void*>(const_cast< CTimeSegmentWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int CTimeSegmentWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
