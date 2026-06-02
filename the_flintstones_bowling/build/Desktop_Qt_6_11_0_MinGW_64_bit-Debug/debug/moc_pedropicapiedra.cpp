/****************************************************************************
** Meta object code from reading C++ file 'pedropicapiedra.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.11.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../pedropicapiedra.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'pedropicapiedra.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.11.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {
struct qt_meta_tag_ZN15PedropicapiedraE_t {};
} // unnamed namespace

template <> constexpr inline auto Pedropicapiedra::qt_create_metaobjectdata<qt_meta_tag_ZN15PedropicapiedraE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "Pedropicapiedra",
        "avanzarFrame",
        "",
        "onAnimacionTerminada"
    };

    QtMocHelpers::UintData qt_methods {
        // Slot 'avanzarFrame'
        QtMocHelpers::SlotData<void()>(1, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onAnimacionTerminada'
        QtMocHelpers::SlotData<void()>(3, 2, QMC::AccessPrivate, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<Pedropicapiedra, qt_meta_tag_ZN15PedropicapiedraE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject Pedropicapiedra::staticMetaObject = { {
    QMetaObject::SuperData::link<Personaje::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN15PedropicapiedraE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN15PedropicapiedraE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN15PedropicapiedraE_t>.metaTypes,
    nullptr
} };

void Pedropicapiedra::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<Pedropicapiedra *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->avanzarFrame(); break;
        case 1: _t->onAnimacionTerminada(); break;
        default: ;
        }
    }
    (void)_a;
}

const QMetaObject *Pedropicapiedra::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Pedropicapiedra::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN15PedropicapiedraE_t>.strings))
        return static_cast<void*>(this);
    return Personaje::qt_metacast(_clname);
}

int Pedropicapiedra::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = Personaje::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 2;
    }
    return _id;
}
QT_WARNING_POP
