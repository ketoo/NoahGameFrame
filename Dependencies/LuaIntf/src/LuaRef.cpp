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

#ifndef LUAINTF_H
    #include "LuaIntf/LuaIntf.h"
    using namespace LuaIntf;
#endif

//---------------------------------------------------------------------------

LUA_INLINE LuaTableRef& LuaTableRef::operator = (const LuaTableRef& that)
{
    assert(L == that.L);
    lua_rawgeti(L, LUA_REGISTRYINDEX, that.m_table);
    lua_rawgeti(L, LUA_REGISTRYINDEX, m_table);
    lua_rawgeti(L, LUA_REGISTRYINDEX, m_key);
    lua_rawgeti(L, LUA_REGISTRYINDEX, that.m_key);
    lua_gettable(L, -4);
    lua_settable(L, -3);
    lua_pop(L, 2);
    return *this;
}

LUA_INLINE LuaTableIterator::LuaTableIterator(lua_State* state, int table, bool fetch_next)
    : L(state)
    , m_table(table)
    , m_key(LUA_NOREF)
    , m_value(LUA_NOREF)
{
    assert(L);
    if (fetch_next) {
        next();
    }
}

LUA_INLINE LuaTableIterator::LuaTableIterator(const LuaTableIterator& that)
    : L(that.L)
    , m_table(that.m_table)
{
    if (L) {
        lua_rawgeti(L, LUA_REGISTRYINDEX, that.m_key);
        m_key = luaL_ref(L, LUA_REGISTRYINDEX);
        lua_rawgeti(L, LUA_REGISTRYINDEX, that.m_value);
        m_value = luaL_ref(L, LUA_REGISTRYINDEX);
    } else {
        m_key = LUA_NOREF;
        m_value = LUA_NOREF;
    }
}

LUA_INLINE LuaTableIterator::LuaTableIterator(LuaTableIterator&& that)
    : L(that.L)
    , m_table(that.m_table)
    , m_key(that.m_key)
    , m_value(that.m_table)
{
    that.m_key = LUA_NOREF;
    that.m_value = LUA_NOREF;
}

LUA_INLINE LuaTableIterator::~LuaTableIterator()
{
    if (L) {
        luaL_unref(L, LUA_REGISTRYINDEX, m_key);
        luaL_unref(L, LUA_REGISTRYINDEX, m_value);
    }
}

LUA_INLINE LuaTableIterator& LuaTableIterator::operator = (const LuaTableIterator& that)
{
    if (this != &that) {
        if (L) {
            luaL_unref(L, LUA_REGISTRYINDEX, m_key);
            luaL_unref(L, LUA_REGISTRYINDEX, m_value);
        }
        L = that.L;
        m_table = that.m_table;
        if (L) {
            lua_rawgeti(L, LUA_REGISTRYINDEX, that.m_key);
            m_key = luaL_ref(L, LUA_REGISTRYINDEX);
            lua_rawgeti(L, LUA_REGISTRYINDEX, that.m_value);
            m_value = luaL_ref(L, LUA_REGISTRYINDEX);
        } else {
            m_key = LUA_NOREF;
            m_value = LUA_NOREF;
        }
    }
    return *this;
}

LUA_INLINE LuaTableIterator& LuaTableIterator::operator = (LuaTableIterator&& that)
{
    std::swap(L, that.L);
    std::swap(m_table, that.m_table);
    std::swap(m_key, that.m_key);
    std::swap(m_value, that.m_value);
    return *this;
}

LUA_INLINE bool LuaTableIterator::operator == (const LuaTableIterator& that) const
{
    if (L != that.L || m_table != that.m_table) {
        return false;
    } else if (m_key == that.m_key) {
        return true;
    } else {
        lua_rawgeti(L, LUA_REGISTRYINDEX, m_key);
        lua_rawgeti(L, LUA_REGISTRYINDEX, that.m_key);
        bool ok = lua_rawequal(L, -1, -2) != 0;
        lua_pop(L, 2);
        return ok;
    }
}

LUA_INLINE void LuaTableIterator::next()
{
    assert(L);
    lua_rawgeti(L, LUA_REGISTRYINDEX, m_table);
    lua_rawgeti(L, LUA_REGISTRYINDEX, m_key);
    luaL_unref(L, LUA_REGISTRYINDEX, m_key);
    luaL_unref(L, LUA_REGISTRYINDEX, m_value);
    if (lua_next(L, -2)) {
        m_value = luaL_ref(L, LUA_REGISTRYINDEX);
        m_key = luaL_ref(L, LUA_REGISTRYINDEX);
    } else {
        m_value = LUA_NOREF;
        m_key = LUA_NOREF;
    }
    lua_pop(L, 1);
}

LUA_INLINE LuaRef::LuaRef(const LuaRef& that)
    : L(that.L)
{
    if (L) {
        lua_rawgeti(L, LUA_REGISTRYINDEX, that.m_ref);
        m_ref = luaL_ref(L, LUA_REGISTRYINDEX);
    } else {
        m_ref = LUA_NOREF;
    }
}

LUA_INLINE LuaRef& LuaRef::operator = (const LuaRef& that)
{
    if (this != &that) {
        if (L) {
            luaL_unref(L, LUA_REGISTRYINDEX, m_ref);
        }
        L = that.L;
        if (L) {
            lua_rawgeti(L, LUA_REGISTRYINDEX, that.m_ref);
            m_ref = luaL_ref(L, LUA_REGISTRYINDEX);
        } else {
            m_ref = LUA_NOREF;
        }
    }
    return *this;
}

LUA_INLINE LuaTypeID LuaRef::type() const
{
    if (m_ref == LUA_NOREF) {
        return LuaTypeID::NONE;
    } else if (m_ref == LUA_REFNIL) {
        return LuaTypeID::NIL;
    } else {
        pushToStack();
        int t = lua_type(L, -1);
        lua_pop(L, 1);
        return static_cast<LuaTypeID>(t);
    }
}

LUA_INLINE bool LuaRef::isIdenticalTo(const LuaRef& r) const
{
    pushToStack();
    r.pushToStack();
    bool b = lua_rawequal(L, -2, -1) != 0;
    lua_pop(L, 2);
    return b;
}

LUA_INLINE int LuaRef::compareTo(const LuaRef& r) const
{
    pushToStack();
    r.pushToStack();
    int d = lua_compare(L, -2, -1, LUA_OPEQ)
        ? 0
        : (lua_compare(L, -2, -1, LUA_OPLT) ? -1 : 1);
    lua_pop(L, 2);
    return d;
}

LUA_INLINE bool LuaRef::operator == (const LuaRef& r) const
{
    pushToStack();
    r.pushToStack();
    bool b = lua_compare(L, -2, -1, LUA_OPEQ) != 0;
    lua_pop(L, 2);
    return b;
}

LUA_INLINE bool LuaRef::operator < (const LuaRef& r) const
{
    pushToStack();
    r.pushToStack();
    bool b = lua_compare(L, -2, -1, LUA_OPLT) != 0;
    lua_pop(L, 2);
    return b;
}

LUA_INLINE bool LuaRef::operator <= (const LuaRef& r) const
{
    pushToStack();
    r.pushToStack();
    bool b = lua_compare(L, -2, -1, LUA_OPLE) != 0;
    lua_pop(L, 2);
    return b;
}

LUA_INLINE LuaRef LuaRef::getMetaTable() const
{
    LuaRef meta;
    pushToStack();
    if (lua_getmetatable(L, -1)) {
        meta = popFromStack(L);
    }
    lua_pop(L, 1);
    return meta;
}

LUA_INLINE void LuaRef::setMetaTable(const LuaRef& meta)
{
    pushToStack();
    meta.pushToStack();
    lua_setmetatable(L, -2);
    lua_pop(L, 1);
}
