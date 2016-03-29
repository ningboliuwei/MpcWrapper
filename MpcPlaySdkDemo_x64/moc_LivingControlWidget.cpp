/****************************************************************************
** Meta object code from reading C++ file 'LivingControlWidget.hpp'
**
** Created: Wed Jul 15 11:30:02 2015
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "LivingControlWidget.hpp"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'LivingControlWidget.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CLivingControlWidget[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      22,   21,   21,   21, 0x08,
      37,   21,   21,   21, 0x08,
      62,   56,   21,   21, 0x08,
      89,   83,   21,   21, 0x08,
     116,  110,   21,   21, 0x08,
     137,   21,   21,   21, 0x08,
     153,   21,   21,   21, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_CLivingControlWidget[] = {
    "CLivingControlWidget\0\0onButtonSnap()\0"
    "onButtonSnapPath()\0state\0onCheckBoxMute(bool)\0"
    "value\0onVolumeChanged(int)\0index\0"
    "onComBoxChanged(int)\0onButtonClose()\0"
    "onButtonCloseAll()\0"
};

void CLivingControlWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CLivingControlWidget *_t = static_cast<CLivingControlWidget *>(_o);
        switch (_id) {
        case 0: _t->onButtonSnap(); break;
        case 1: _t->onButtonSnapPath(); break;
        case 2: _t->onCheckBoxMute((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: _t->onVolumeChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->onComBoxChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->onButtonClose(); break;
        case 6: _t->onButtonCloseAll(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CLivingControlWidget::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CLivingControlWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_CLivingControlWidget,
      qt_meta_data_CLivingControlWidget, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CLivingControlWidget::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CLivingControlWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CLivingControlWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CLivingControlWidget))
        return static_cast<void*>(const_cast< CLivingControlWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int CLivingControlWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
