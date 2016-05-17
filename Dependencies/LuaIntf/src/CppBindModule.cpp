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

LUA_INLINE int CppBindModuleMetaMethod::index(lua_State* L)
{
    // <SP:1> -> table
    // <SP:2> -> key

    // push metatable of table -> <mt>
    lua_getmetatable(L, 1);

    // push metatable[key] -> <mt> <mt[key]>
    lua_pushvalue(L, 2);
    lua_rawget(L, -2);

    if (lua_isnil(L, -1)) {
        // get metatable.getters -> <mt> <getters>
        lua_pop(L, 1);          // pop nil
        lua_pushliteral(L, "___getters");
        lua_rawget(L, -2);      // get getters table
        assert(lua_istable(L, -1));

        // get metatable.getters[key] -> <mt> <getters> <getters[key]>
        lua_pushvalue(L, 2);    // push key
        lua_rawget(L, -2);      // lookup key in getters

        if (lua_iscfunction(L, -1)) {
            // getter function found
            lua_call(L, 0, 1);
        }
    }

    return 1;
}

LUA_INLINE int CppBindModuleMetaMethod::newIndex(lua_State* L)
{
    // <SP:1> -> table
    // <SP:2> -> key
    // <SP:3> -> value

    // push metatable of table -> <mt>
    lua_getmetatable(L, 1);

    // get setters subtable of metatable -> <mt> <setters>
    lua_pushliteral(L, "___setters");
    lua_rawget(L, -2);          // get __setters table
    assert(lua_istable(L, -1));

    // get setters[key] -> <mt> <setters> <setters[key]>
    lua_pushvalue(L, 2);        // push key arg2
    lua_rawget(L, -2);          // lookup key in setters

    if (lua_iscfunction(L, -1)) {
        // setter function found
        lua_pushvalue(L, 3);    // push new value as arg
        lua_call(L, 1, 0);
    } else {
        // no setter found, just set the table field
        assert(lua_isnil(L, -1));
        lua_pop(L, 3);
        lua_rawset(L, 1);
    }
    return 0;
}

LUA_INLINE int CppBindModuleMetaMethod::forwardCall(lua_State* L)
{
    // <SP:1> -> table (table's metatable is set to itself)
    // <SP:2> ~ <SP:top> -> args
    int top = lua_gettop(L);

    lua_pushvalue(L, lua_upvalueindex(1));
    lua_rawget(L, 1);           // get sub module metatable -> <sub_mt>

    lua_pushliteral(L, "__call");
    lua_rawget(L, -2);          // get call function -> <sub_mt> <call>

    lua_insert(L, 1);           // move <call> to top -> now <mt> is at index 2
    lua_replace(L, 2);          // replace original <mt> with <sub_mt>
    lua_call(L, top, 1);
    return 1;
}

LUA_INLINE int CppBindModuleMetaMethod::errorReadOnly(lua_State* L)
{
    return luaL_error(L, "property '%s' is read-only", lua_tostring(L, lua_upvalueindex(1)));
}

//---------------------------------------------------------------------------

LUA_INLINE CppBindModuleBase::CppBindModuleBase(LuaRef& meta, const char* name)
{
    LuaRef ref = meta.rawget(name);
    if (ref != nullptr) {
        m_meta = ref;
        return;
    }

    lua_State* L = meta.state();
    std::string type_name = "module<" + getFullName(meta, name) + ">";
    LuaRef module = LuaRef::createTable(L);
    module.setMetaTable(module);

    module.rawset("__index", &CppBindModuleMetaMethod::index);
    module.rawset("__newindex", &CppBindModuleMetaMethod::newIndex);
    module.rawset("___getters", LuaRef::createTable(L));
    module.rawset("___setters", LuaRef::createTable(L));
    module.rawset("___type", type_name);
    module.rawset("___parent", meta);
    meta.rawset(name, module);
    m_meta = module;
}

LUA_INLINE std::string CppBindModuleBase::getFullName(const LuaRef& parent, const char* name)
{
    std::string full_name = parent.get<const char*>("___type", "");
    if (!full_name.empty()) {
        size_t pos = full_name.find('<');
        if (pos != std::string::npos) full_name.erase(0, pos + 1);
        pos = full_name.rfind('>');
        if (pos != std::string::npos) full_name.erase(pos);
        full_name += '.';
    }
    full_name += name;
    return full_name;
}

LUA_INLINE std::string CppBindModuleBase::getMemberName(const LuaRef& parent, const char* name)
{
    std::string full_name = parent.rawget<const char*>("___type", "<unknown>");
    full_name += '.';
    full_name += name;
    return full_name;
}

LUA_INLINE void CppBindModuleBase::setGetter(const char* name, const LuaRef& getter)
{
    m_meta.rawget("___getters").rawset(name, getter);
}

LUA_INLINE void CppBindModuleBase::setSetter(const char* name, const LuaRef& setter)
{
    m_meta.rawget("___setters").rawset(name, setter);
}

LUA_INLINE void CppBindModuleBase::setReadOnly(const char* name)
{
    std::string full_name = getMemberName(m_meta, name);
    setSetter(name, LuaRef::createFunctionWith(state(), &CppBindModuleMetaMethod::errorReadOnly, full_name));
}
