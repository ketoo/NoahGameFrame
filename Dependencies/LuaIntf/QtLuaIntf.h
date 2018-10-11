//
// https://github.com/SteveKChiu/lua-intf
//
// Copyright 2014, Steve K. Chiu <steve.k.chiu@gmail.com>
//
// The MIT License (http://www.opensource.org/licenses/mit-license.php)
//
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//

#ifndef QTLUAINTF_H
#define QTLUAINTF_H

//---------------------------------------------------------------------------

#include "LuaIntf.h"
#include <QVariant>

namespace LuaIntf
{

//---------------------------------------------------------------------------

namespace Lua
{
    void pushVariant(lua_State* L, const QVariant& v);
    QVariant getVariant(lua_State* L, int index);

    /**
     * Push QMap as Lua table onto Lua stack.
     */
    template <typename K, typename V>
    inline void pushMap(lua_State* L, const QMap<K, V>& map)
    {
        lua_newtable(L);
        for (auto it = map.begin(); it != map.end(); ++it) {
            push(L, it.key());
            push(L, it.value());
            lua_settable(L, -3);
        }
    }

    /**
     * Push QHash as Lua table onto Lua stack.
     */
    template <typename K, typename V>
    inline void pushMap(lua_State* L, const QHash<K, V>& map)
    {
        lua_newtable(L);
        for (auto it = map.begin(); it != map.end(); ++it) {
            push(L, it.key());
            push(L, it.value());
            lua_settable(L, -3);
        }
    }
}

//---------------------------------------------------------------------------

template <>
struct LuaTypeMapping <QByteArray>
{
    static void push(lua_State* L, const QByteArray& str)
    {
        if (str.isEmpty()) {
            lua_pushliteral(L, "");
        } else {
            lua_pushlstring(L, str.data(), str.length());
        }
    }

    static QByteArray get(lua_State* L, int index)
    {
        size_t len;
        const char* p = luaL_checklstring(L, index, &len);
        return QByteArray(p, len);
    }

    static QByteArray opt(lua_State* L, int index, const QByteArray& def)
    {
        return lua_isnoneornil(L, index) ? def : get(L, index);
    }
};

//---------------------------------------------------------------------------

template <>
struct LuaTypeMapping <QString>
{
    static void push(lua_State* L, const QString& str)
    {
        if (str.isEmpty()) {
            lua_pushliteral(L, "");
        } else {
            QByteArray buf = str.toUtf8();
            lua_pushlstring(L, buf.data(), buf.length());
        }
    }

    static QString get(lua_State* L, int index)
    {
        size_t len;
        const char* p = luaL_checklstring(L, index, &len);
        return QString::fromUtf8(p, len);
    }

    static QString opt(lua_State* L, int index, const QString& def)
    {
        return lua_isnoneornil(L, index) ? def : get(L, index);
    }
};

//---------------------------------------------------------------------------

template <>
struct LuaTypeMapping <QVariant>
{
    static void push(lua_State* L, const QVariant& v)
    {
        Lua::pushVariant(L, v);
    }

    static QVariant get(lua_State* L, int index)
    {
        return Lua::getVariant(L, index);
    }

    static QVariant opt(lua_State* L, int index, const QVariant& def)
    {
        return lua_isnoneornil(L, index) ? def : Lua::getVariant(L, index);
    }
};

//---------------------------------------------------------------------------

LUA_USING_LIST_TYPE_X(QStringList)
LUA_USING_LIST_TYPE_X(QVariantList)
LUA_USING_MAP_TYPE_X(QVariantMap)

//---------------------------------------------------------------------------

#if LUAINTF_HEADERS_ONLY
#include "src/QtLuaIntf.cpp"
#endif

//---------------------------------------------------------------------------

}

#endif
