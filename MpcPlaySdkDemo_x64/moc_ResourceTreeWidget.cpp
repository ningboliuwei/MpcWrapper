/****************************************************************************
** Meta object code from reading C++ file 'ResourceTreeWidget.hpp'
**
** Created: Wed Jul 15 11:30:03 2015
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "ResourceTreeWidget.hpp"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ResourceTreeWidget.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CResourceTreeWidget[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      26,   20,   21,   20, 0x08,
      34,   20,   21,   20, 0x08,
      43,   20,   21,   20, 0x08,
      55,   20,   20,   20, 0x08,
      68,   20,   20,   20, 0x08,
      81,   20,   20,   20, 0x08,
      99,   20,   20,   20, 0x08,
     140,  128,   20,   20, 0x08,
     178,  128,   20,   20, 0x08,
     242,  223,   20,   20, 0x08,
     326,  316,   21,   20, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_CResourceTreeWidget[] = {
    "CResourceTreeWidget\0\0bool\0login()\0"
    "logout()\0modifyPwd()\0delCmsItem()\0"
    "AddCmsItem()\0propertyCmsItem()\0"
    "singleScheduleMoniterIndex()\0item,coloum\0"
    "slotItemClicked(QTreeWidgetItem*,int)\0"
    "slotItemDoubleCliecked(QTreeWidgetItem*,int)\0"
    "item,manageSession\0"
    "getManageSessionByChildItem(QTreeWidgetItem*,mpc::nsdk::IManageSession"
    "**)\0"
    "item,info\0"
    "getResourceInfoByChildItem(QTreeWidgetItem*,SResourceInfo&)\0"
};

void CResourceTreeWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CResourceTreeWidget *_t = static_cast<CResourceTreeWidget *>(_o);
        switch (_id) {
        case 0: { bool _r = _t->login();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 1: { bool _r = _t->logout();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 2: { bool _r = _t->modifyPwd();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 3: _t->delCmsItem(); break;
        case 4: _t->AddCmsItem(); break;
        case 5: _t->propertyCmsItem(); break;
        case 6: _t->singleScheduleMoniterIndex(); break;
        case 7: _t->slotItemClicked((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 8: _t->slotItemDoubleCliecked((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 9: _t->getManageSessionByChildItem((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< mpc::nsdk::IManageSession**(*)>(_a[2]))); break;
        case 10: { bool _r = _t->getResourceInfoByChildItem((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< SResourceInfo(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CResourceTreeWidget::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CResourceTreeWidget::staticMetaObject = {
    { &QTreeWidget::staticMetaObject, qt_meta_stringdata_CResourceTreeWidget,
      qt_meta_data_CResourceTreeWidget, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CResourceTreeWidget::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CResourceTreeWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CResourceTreeWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CResourceTreeWidget))
        return static_cast<void*>(const_cast< CResourceTreeWidget*>(this));
    if (!strcmp(_clname, "mpc::nsdk::IManageSessionNotify"))
        return static_cast< mpc::nsdk::IManageSessionNotify*>(const_cast< CResourceTreeWidget*>(this));
    return QTreeWidget::qt_metacast(_clname);
}

int CResourceTreeWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QTreeWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
