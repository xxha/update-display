/****************************************************************************
** Meta object code from reading C++ file 'update_soft.h'
**
** Created: Mon Apr 22 13:17:06 2013
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "update_soft.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'update_soft.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_update_soft[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      13,   12,   12,   12, 0x08,
      34,   12,   12,   12, 0x08,
      55,   12,   12,   12, 0x08,
      78,   12,   12,   12, 0x08,
      99,   12,   12,   12, 0x08,
     120,   12,   12,   12, 0x08,
     142,   12,   12,   12, 0x08,
     167,   12,   12,   12, 0x08,
     190,   12,   12,   12, 0x08,
     214,   12,   12,   12, 0x08,
     239,   12,   12,   12, 0x08,
     255,   12,   12,   12, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_update_soft[] = {
    "update_soft\0\0m_10gCheckBox_slot()\0"
    "m_40gCheckBox_slot()\0m_100geCheckBox_slot()\0"
    "m_16gCheckBox_slot()\0m_1geCheckBox_slot()\0"
    "m_2_5gCheckBox_slot()\0m_common_CheckBox_slot()\0"
    "m_cancel_button_slot()\0m_upgrade_button_slot()\0"
    "m_rootfs_CheckBox_slot()\0timer_timeout()\0"
    "load_version_timer()\0"
};

const QMetaObject update_soft::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_update_soft,
      qt_meta_data_update_soft, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &update_soft::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *update_soft::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *update_soft::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_update_soft))
        return static_cast<void*>(const_cast< update_soft*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int update_soft::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: m_10gCheckBox_slot(); break;
        case 1: m_40gCheckBox_slot(); break;
        case 2: m_100geCheckBox_slot(); break;
        case 3: m_16gCheckBox_slot(); break;
        case 4: m_1geCheckBox_slot(); break;
        case 5: m_2_5gCheckBox_slot(); break;
        case 6: m_common_CheckBox_slot(); break;
        case 7: m_cancel_button_slot(); break;
        case 8: m_upgrade_button_slot(); break;
        case 9: m_rootfs_CheckBox_slot(); break;
        case 10: timer_timeout(); break;
        case 11: load_version_timer(); break;
        default: ;
        }
        _id -= 12;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
