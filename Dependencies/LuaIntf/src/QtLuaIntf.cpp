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
    #include "LuaIntf/QtLuaIntf.h"
    using namespace LuaIntf;
#endif

//---------------------------------------------------------------------------

LUA_INLINE void Lua::pushVariant(lua_State* L, const QVariant& v)
{
    switch (v.type()) {
    case QVariant::Invalid:
        lua_pushnil(L);
        break;
    case QVariant::Bool:
        lua_pushboolean(L, v.toBool());
        break;
    case QVariant::Int:
        lua_pushinteger(L, v.toInt());
        break;
    case QVariant::UInt:
        lua_pushunsigned(L, v.toUInt());
        break;
    case QVariant::LongLong:
        push(L, v.toLongLong());
        break;
    case QVariant::ULongLong:
        push(L, v.toULongLong());
        break;
    case QVariant::Double:
        lua_pushnumber(L, v.toDouble());
        break;
    case QVariant::Map:
        pushMap(L, v.toMap());
        break;
    case QVariant::List:
        pushList(L, v.toList());
        break;
    case QVariant::StringList:
        pushList(L, v.toStringList());
        break;
    case QVariant::ByteArray:
        push(L, v.toByteArray());
        break;
    default:
        push(L, v.toString());
        break;
    }
}

LUA_INLINE QVariant Lua::getVariant(lua_State* L, int idx)
{
    int type = lua_type(L, idx);
    if (type == LUA_TNIL || type == LUA_TNONE) {
        return QVariant();
    } else if (type == LUA_TNUMBER) {
        return lua_tonumber(L, idx);
    } else if (type == LUA_TBOOLEAN) {
        return lua_toboolean(L, idx) != 0;
    } else if (type == LUA_TSTRING) {
        size_t l;
        const char* s = lua_tolstring(L, idx, &l);
        return QByteArray(s, l);
    } else if (luaL_callmeta(L, idx, "__tostring")) {
        size_t l;
        const char* s = lua_tolstring(L, -1, &l);
        if (!s) {
            luaL_error(L, "'tostring' must return a string");
            return QVariant();
        }
        QByteArray buf(s, l);
        lua_pop(L, 1);
        return buf;
    } else if (type == LUA_TTABLE) {
        if (luaL_len(L, idx) > 0) {
            return getList<QVariantList>(L, idx);
        } else {
            return getMap<QVariantMap>(L, idx);
        }
    } else {
        luaL_error(L, "unknown type, can not convert to variant");
        return QVariant();
    }
}
