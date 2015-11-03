/***************************************************************************
**
** Copyright (C) 2015 Jochen Becher
** Contact: http://www.qt.io/licensing
**
** This file is part of Qt Creator.
**
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company.  For licensing terms and
** conditions see http://www.qt.io/terms-conditions.  For further information
** use the contact form at http://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 or version 3 as published by the Free
** Software Foundation and appearing in the file LICENSE.LGPLv21 and
** LICENSE.LGPLv3 included in the packaging of this file.  Please review the
** following information to ensure the GNU Lesser General Public License
** requirements will be met: https://www.gnu.org/licenses/lgpl.html and
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, The Qt Company gives you certain additional
** rights.  These rights are described in The Qt Company LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
****************************************************************************/

#ifndef QARK_ATTRIBUTE_H
#define QARK_ATTRIBUTE_H

#include "parameters.h"

#include <QString>

namespace qark {

template<typename T>
class Attr {
public:
    Attr(const QString &qualified_name, T *value)
        : m_qualifiedName(qualified_name),
          m_value(value)
    {
    }

    Attr(const QString &qualified_name, T *value, const Parameters &parameters)
        : m_qualifiedName(qualified_name),
          m_value(value),
          m_parameters(parameters)
    {
    }

    const QString &getQualifiedName() const { return m_qualifiedName; }

    T *getValue() const { return m_value; }

    Parameters getParameters() const { return m_parameters; }

private:
    QString m_qualifiedName;
    T *m_value;
    Parameters m_parameters;
};

template<typename T>
Attr<T * const> attr(const QString &qualified_name, T * const &value)
{
    return Attr<T * const>(qualified_name, &value);
}

template<typename T>
Attr<T * const> attr(const QString &qualified_name, T * const &value, const Parameters &parameters)
{
    return Attr<T * const>(qualified_name, &value, parameters);
}

template<typename T>
Attr<T> attr(const QString &qualified_name, T &value)
{
    return Attr<T>(qualified_name, &value);
}

template<typename T>
Attr<T> attr(const QString &qualified_name, T &value, const Parameters &parameters)
{
    return Attr<T>(qualified_name, &value, parameters);
}


template<class U, typename T>
class GetterAttr {
public:
    GetterAttr(const QString &qualified_name, const U &u, T (U::*getter)() const)
        : m_qualifiedName(qualified_name),
          m_u(u),
          m_getter(getter)
    {
    }

    GetterAttr(const QString &qualified_name, const U &u, T (U::*getter)() const, const Parameters &parameters)
        : m_qualifiedName(qualified_name),
          m_u(u),
          m_getter(getter),
          m_parameters(parameters)
    {
    }

    const QString &getQualifiedName() const { return m_qualifiedName; }

    const U &getObject() const { return m_u; }

    T (U::*getGetter() const)() const { return m_getter; }

    Parameters getParameters() const { return m_parameters; }

private:
    QString m_qualifiedName;
    const U &m_u;
    T (U::*m_getter)() const;
    Parameters m_parameters;
};

template<class U, typename T>
GetterAttr<U, T> attr(const QString &qualified_name, const U &u, T (U::*getter)() const)
{
    return GetterAttr<U, T>(qualified_name, u, getter);
}

template<class U, typename T>
GetterAttr<U, T> attr(const QString &qualified_name, const U &u, T (U::*getter)() const, const Parameters &parameters)
{
    return GetterAttr<U, T>(qualified_name, u, getter, parameters);
}


template<class U, typename T>
class SetterAttr {
public:
    SetterAttr(const QString &qualified_name, U &u, void (U::*setter)(T))
        : m_qualifiedName(qualified_name),
          m_u(u),
          m_setter(setter)
    {
    }

    SetterAttr(const QString &qualified_name, U &u, void (U::*setter)(T), const Parameters &parameters)
        : m_qualifiedName(qualified_name),
          m_u(u),
          m_setter(setter),
          m_parameters(parameters)
    {
    }

    const QString &getQualifiedName() const { return m_qualifiedName; }

    U &getObject() const { return m_u; }

    void (U::*getSetter() const)(T) { return m_setter; }

    Parameters getParameters() const { return m_parameters; }

private:
    QString m_qualifiedName;
    U &m_u;
    void (U::*m_setter)(T);
    Parameters m_parameters;
};

template<class U, typename T>
SetterAttr<U, T> attr(const QString &qualified_name, U &u, void (U::*setter)(T))
{
    return SetterAttr<U, T>(qualified_name, u, setter);
}

template<class U, typename T>
SetterAttr<U, T> attr(const QString &qualified_name, U &u, void (U::*setter)(T), const Parameters &parameters)
{
    return SetterAttr<U, T>(qualified_name, u, setter, parameters);
}


template<class U, typename T, typename V>
class GetterSetterAttr {
public:
    GetterSetterAttr(const QString &qualified_name, U &u, T (U::*getter)() const, void (U::*setter)(V))
        : m_qualifiedName(qualified_name),
          m_u(u),
          m_getter(getter),
          m_setter(setter)
    {
    }

    GetterSetterAttr(const QString &qualified_name, U &u, T (U::*getter)() const, void (U::*setter)(V), const Parameters &parameters)
        : m_qualifiedName(qualified_name),
          m_u(u),
          m_getter(getter),
          m_setter(setter),
          m_parameters(parameters)
    {
    }

    const QString &getQualifiedName() const { return m_qualifiedName; }

    U &getObject() const { return m_u; }

    T (U::*getGetter() const)() const { return m_getter; }

    void (U::*getSetter() const)(V) { return m_setter; }

    Parameters getParameters() const { return m_parameters; }

private:
    QString m_qualifiedName;
    U &m_u;
    T (U::*m_getter)() const;
    void (U::*m_setter)(V);
    Parameters m_parameters;
};

template<class U, typename T, typename V>
GetterSetterAttr<U, T, V> attr(const QString &qualified_name, U &u, T (U::*getter)() const, void (U::*setter)(V))
{
    return GetterSetterAttr<U, T, V>(qualified_name, u, getter, setter);
}

template<class U, typename T, typename V>
GetterSetterAttr<U, T, V> attr(const QString &qualified_name, U &u, T (U::*getter)() const, void (U::*setter)(V), const Parameters &parameters)
{
    return GetterSetterAttr<U, T, V>(qualified_name, u, getter, setter, parameters);
}


template<class U, typename T>
class GetFuncAttr {
public:
    GetFuncAttr(const QString &qualified_name, U &u, T (*get_func)(const U &))
        : m_qualifiedName(qualified_name),
          m_u(u),
          m_getFunc(get_func)
    {
    }

    GetFuncAttr(const QString &qualified_name, U &u, T (*get_func)(const U &), const Parameters &parameters)
        : m_qualifiedName(qualified_name),
          m_u(u),
          m_getFunc(get_func),
          m_parameters(parameters)
    {
    }

    const QString &getQualifiedName() const { return m_qualifiedName; }

    U &getObject() const { return m_u; }

    T (*getGetFunc() const)(const U &) { return m_getFunc; }

    Parameters getParameters() const { return m_parameters; }

private:
    QString m_qualifiedName;
    U &m_u;
    T (*m_getFunc)(const U &);
    Parameters m_parameters;
};

template<class U, typename T>
GetFuncAttr<U, T> attr(const QString &qualified_name, const U &u, T (*get_func)(const U &))
{
    return GetFuncAttr<U, T>(qualified_name, u, get_func);
}

template<class U, typename T>
GetFuncAttr<U, T> attr(const QString &qualified_name, const U &u, T (*get_func)(const U &), const Parameters &parameters)
{
    return GetFuncAttr<U, T>(qualified_name, u, get_func, parameters);
}


template<class U, typename T>
class SetFuncAttr {
public:
    SetFuncAttr(const QString &qualified_name, U &u, void (*set_func)(U &, T))
        : m_qualifiedName(qualified_name),
          m_u(u),
          m_setFunc(set_func)
    {
    }

    SetFuncAttr(const QString &qualified_name, U &u, void (*set_func)(U &, T), const Parameters &parameters)
        : m_qualifiedName(qualified_name),
          m_u(u),
          m_setFunc(set_func),
          m_parameters(parameters)
    {
    }

    const QString &getQualifiedName() const { return m_qualifiedName; }

    U &getObject() const { return m_u; }

    void (*getSetFunc() const)(U &, T) { return m_setFunc; }

    Parameters getParameters() const { return m_parameters; }

private:
    QString m_qualifiedName;
    U &m_u;
    void (*m_setFunc)(U &, T);
    Parameters m_parameters;
};

template<class U, typename T>
SetFuncAttr<U, T> attr(const QString &qualified_name, U &u, void (*set_func)(U &, T))
{
    return SetFuncAttr<U, T>(qualified_name, u, set_func);
}

template<class U, typename T>
SetFuncAttr<U, T> attr(const QString &qualified_name, U &u, void (*set_func)(U &, T), const Parameters &parameters)
{
    return SetFuncAttr<U, T>(qualified_name, u, set_func, parameters);
}


template<class U, typename T, typename V>
class GetSetFuncAttr {
public:
    GetSetFuncAttr(const QString &qualified_name, U &u, T (*get_func)(const U &), void (*set_func)(U &, V))
        : m_qualifiedName(qualified_name),
          m_u(u),
          m_getFunc(get_func),
          m_setFunc(set_func)
    {
    }

    GetSetFuncAttr(const QString &qualified_name, U &u, T (*get_func)(const U &), void (*set_func)(U &, V), const Parameters &parameters)
        : m_qualifiedName(qualified_name),
          m_u(u),
          m_getFunc(get_func),
          m_setFunc(set_func),
          m_parameters(parameters)
    {
    }

    const QString &getQualifiedName() const { return m_qualifiedName; }

    U &getObject() const { return m_u; }

    T (*getGetFunc() const)(const U &) { return m_getFunc; }

    void (*getSetFunc() const)(U &, V) { return m_setFunc; }

    Parameters getParameters() const { return m_parameters; }

private:
    QString m_qualifiedName;
    U &m_u;
    T (*m_getFunc)(const U &);
    void (*m_setFunc)(U &, V);
    Parameters m_parameters;
};

template<class U, typename T, typename V>
GetSetFuncAttr<U, T, V> attr(const QString &qualified_name, U &u, T (*get_func)(const U &), void (*set_func)(U &, V))
{
    return GetSetFuncAttr<U, T, V>(qualified_name, u, get_func, set_func);
}

template<class U, typename T, typename V>
GetSetFuncAttr<U, T, V> attr(const QString &qualified_name, U &u, T (*get_func)(const U &), void (*set_func)(U &, V), const Parameters &parameters)
{
    return GetSetFuncAttr<U, T, V>(qualified_name, u, get_func, set_func, parameters);
}

}

#endif // QARK_ATTRIBUTE_H
