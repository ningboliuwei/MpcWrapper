/****************************************************************************
** Meta object code from reading C++ file 'LoginWnd.hpp'
**
** Created: Wed Jul 15 11:30:02 2015
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "LoginWnd.hpp"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'LoginWnd.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CLoginWnd[] = {

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
      11,   10,   10,   10, 0x08,
      34,   10,   10,   10, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_CLoginWnd[] = {
    "CLoginWnd\0\0onLoginButtonClicked()\0"
    "onCancelButtonClicked()\0"
};

void CLoginWnd::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CLoginWnd *_t = static_cast<CLoginWnd *>(_o);
        switch (_id) {
        case 0: _t->onLoginButtonClicked(); break;
        case 1: _t->onCancelButtonClicked(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData CLoginWnd::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CLoginWnd::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_CLoginWnd,
      qt_meta_data_CLoginWnd, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CLoginWnd::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CLoginWnd::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CLoginWnd::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CLoginWnd))
        return static_cast<void*>(const_cast< CLoginWnd*>(this));
    return QDialog::qt_metacast(_clname);
}

int CLoginWnd::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
