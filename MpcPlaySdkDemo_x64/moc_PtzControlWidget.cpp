/****************************************************************************
** Meta object code from reading C++ file 'PtzControlWidget.hpp'
**
** Created: Wed Jul 15 11:30:00 2015
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "PtzControlWidget.hpp"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'PtzControlWidget.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CPtzControlWidget[] = {

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
      19,   18,   18,   18, 0x08,
      35,   18,   18,   18, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_CPtzControlWidget[] = {
    "CPtzControlWidget\0\0onButtonClick()\0"
    "onComboBoxActived()\0"
};

void CPtzControlWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CPtzControlWidget *_t = static_cast<CPtzControlWidget *>(_o);
        switch (_id) {
        case 0: _t->onButtonClick(); break;
        case 1: _t->onComboBoxActived(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData CPtzControlWidget::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CPtzControlWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_CPtzControlWidget,
      qt_meta_data_CPtzControlWidget, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CPtzControlWidget::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CPtzControlWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CPtzControlWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CPtzControlWidget))
        return static_cast<void*>(const_cast< CPtzControlWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int CPtzControlWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
