/****************************************************************************
** Meta object code from reading C++ file 'PwdModifyWnd.hpp'
**
** Created: Wed Jul 15 11:30:04 2015
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "PwdModifyWnd.hpp"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'PwdModifyWnd.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CPwdModifyWnd[] = {

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
      15,   14,   14,   14, 0x08,
      35,   14,   14,   14, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_CPwdModifyWnd[] = {
    "CPwdModifyWnd\0\0onOkbuttonClicked()\0"
    "onCancelbuttonClicked()\0"
};

void CPwdModifyWnd::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CPwdModifyWnd *_t = static_cast<CPwdModifyWnd *>(_o);
        switch (_id) {
        case 0: _t->onOkbuttonClicked(); break;
        case 1: _t->onCancelbuttonClicked(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData CPwdModifyWnd::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CPwdModifyWnd::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_CPwdModifyWnd,
      qt_meta_data_CPwdModifyWnd, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CPwdModifyWnd::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CPwdModifyWnd::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CPwdModifyWnd::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CPwdModifyWnd))
        return static_cast<void*>(const_cast< CPwdModifyWnd*>(this));
    return QDialog::qt_metacast(_clname);
}

int CPwdModifyWnd::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
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
