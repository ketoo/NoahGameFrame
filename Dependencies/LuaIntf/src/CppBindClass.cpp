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

LUA_INLINE int CppBindClassMetaMethod::index(lua_State* L)
{
    // <SP:1> -> table or userdata
    // <SP:2> -> key

    // get signature metatable -> <mt> <sign_mt>
    lua_getmetatable(L, 1);
    lua_rawgetp(L, -1, CppSignature<CppObject>::value());
    lua_rawget(L, LUA_REGISTRYINDEX);

    // check if both are equal
    if (!lua_rawequal(L, -1, -2)) {
        // not match, panic now -> <error>
        lua_pushliteral(L, "___type");
        lua_rawget(L, -3);
        return luaL_error(L, "invalid meta table found when try to get property '%s.%s'",
            luaL_optstring(L, -1, "<unknown>"), lua_tostring(L, 2));
    } else {
        // matched, pop <sign_mt> -> <mt>
        lua_pop(L, 1);
    }

    for (;;) {
        // push metatable[key] -> <mt> <mt[key]>
        lua_pushvalue(L, 2);
        lua_rawget(L, -2);

        if (!lua_isnil(L, -1)) {
            // value is found
            break;
        }

        
        // get metatable.getters -> <mt> <getters>
        lua_pop(L, 1);                  // pop nil

        if (lua_isnumber(L, 2)) {
            lua_pushliteral(L, "___get_indexed");
            lua_rawget(L, -2);

            if (!lua_isnil(L, -1)) {
                assert(lua_iscfunction(L, -1));
                lua_pushvalue(L, 1);
                lua_pushvalue(L, 2);
                lua_call(L, 2, 1);
                break;
            } else {
                lua_pop(L, 1);
            }
        }

        lua_pushliteral(L, "___getters");
        lua_rawget(L, -2);
        assert(lua_istable(L, -1));

        // get metatable.getters[key] -> <mt> <getters> <getters[key]>
        lua_pushvalue(L, 2);            // push key
        lua_rawget(L, -2);              // lookup key in getters

        // call the getter if it is function, or leave it as value
        if (!lua_isnil(L, -1)) {
            if (lua_iscfunction(L, -1)) {
                if (lua_isuserdata(L, 1)) {
                    // if it is userdata, that means instance
                    lua_pushvalue(L, 1);    // push userdata as object param for member function
                    lua_call(L, 1, 1);
                } else {
                    // otherwise, it is static (class getters)
                    assert(lua_istable(L, 1));
                    lua_call(L, 0, 1);
                }
            }
            break;
        }

        // now try super metatable -> <mt> <super_mt>
        lua_pop(L, 2);                  // pop <getters> <getters[key]>
        lua_pushliteral(L, "___super");
        lua_rawget(L, -2);

        if (lua_isnil(L, -1)) {

#if LUAINTF_EXTRA_LUA_FIELDS
            if (lua_isuserdata(L, 1)) {
                lua_getuservalue(L, 1);
                if (!lua_isnil(L, -1)) {
                    // get extra_fields[key] -> <mt> <nil> <extra_fields> <extra_fields[key]>
                    lua_pushvalue(L, 2);    // push key
                    lua_rawget(L, -2);      // lookup key in extra fields
                }
            }
#endif

            // leave value on top -> <nil> or <extra_fields[key]>
            break;
        }

        // yes, now continue with <super_mt>
        assert(lua_istable(L, -1));
        lua_remove(L, -2);              // pop <mt>
    }

    return 1;
}

LUA_INLINE int CppBindClassMetaMethod::newIndex(lua_State* L)
{
    // <SP:1> -> table or userdata
    // <SP:2> -> key
    // <SP:3> -> value

    // get signature metatable -> <mt> <sign_mt>
    lua_getmetatable(L, 1);
    lua_rawgetp(L, -1, CppSignature<CppObject>::value());
    lua_rawget(L, LUA_REGISTRYINDEX);

    // check if both are equal
    if (!lua_rawequal(L, -1, -2)) {
        // not match, panic now -> <error>
        lua_pushliteral(L, "___type");
        lua_rawget(L, -3);
        return luaL_error(L, "invalid meta table found when try to set property '%s.%s'",
            luaL_optstring(L, -1, "<unknown>"), lua_tostring(L, 2));
    } else {
        // matched, pop <sign_mt> -> <mt>
        lua_pop(L, 1);
    }

    for (;;) {
        if (lua_isnumber(L, 2)) {
            lua_pushliteral(L, "___set_indexed");
            lua_rawget(L, -2);

            if (!lua_isnil(L, -1)) {
                assert(lua_iscfunction(L, -1));
                lua_pushvalue(L, 1);
                lua_pushvalue(L, 2);
                lua_pushvalue(L, 3);
                lua_call(L, 3, 0);
                break;
            } else {
                lua_pop(L, 1);
            }
        }

        // get setters subtable of metatable -> <mt> <setters>
        lua_pushliteral(L, "___setters");
        lua_rawget(L, -2);              // get __setters table
        assert(lua_istable(L, -1));

        // get setters[key] -> <mt> <setters> <setters[key]>
        lua_pushvalue(L, 2);            // push key arg2
        lua_rawget(L, -2);              // lookup key in setters

        if (lua_iscfunction(L, -1)) {
            // setter function found, now need to test whether it is object (== userdata)
            int n = 1;
            if (lua_isuserdata(L, 1)) {
                lua_pushvalue(L, 1);    // push userdata as object param for member function
                n++;
            } else {
                assert(lua_istable(L, 1));
            }

            lua_pushvalue(L, 3);        // push new value as arg
            lua_call(L, n, 0);
            break;
        }

        // now try super metatable -> <mt> <super_mt>
        assert(lua_isnil(L, -1));
        lua_pop(L, 2);                  // pop <setters> <setters[key]>
        lua_pushliteral(L, "___super");
        lua_rawget(L, -2);

        // check if there is one
        if (lua_isnil(L, -1)) {

#if LUAINTF_EXTRA_LUA_FIELDS
            if (lua_isuserdata(L, 1)) {
                // set instance fields
                lua_pushliteral(L, "___const");
                lua_rawget(L, -3);
                if (!lua_rawequal(L, -1, -3)) {
                    // set field only if not const
                    lua_getuservalue(L, 1);
                    if (lua_isnil(L, -1)) {
                        lua_newtable(L);
                        lua_pushvalue(L, 2);
                        lua_pushvalue(L, 3);
                        lua_rawset(L, -3);
                        lua_setuservalue(L, 1);
                    } else {
                        lua_pushvalue(L, 2);
                        lua_pushvalue(L, 3);
                        lua_rawset(L, -3);
                    }
                    break;
                }
                lua_pop(L, 1);
            } else {
                // set class fields
                lua_pushvalue(L, 2);
                lua_pushvalue(L, 3);
                lua_rawset(L, 1);
                break;
            }
#endif

            // give up
            lua_pushliteral(L, "___type");
            lua_rawget(L, -3);
            return luaL_error(L, "property '%s.%s' is not found or not writable",
                luaL_optstring(L, -1, "<unknown>"), lua_tostring(L, 2));
        }

        // yes, now continue with <super_mt>
        assert(lua_istable(L, -1));
        lua_remove(L, -2);              // pop <mt>
    }

    return 0;
}

LUA_INLINE int CppBindClassMetaMethod::errorReadOnly(lua_State* L)
{
    return luaL_error(L, "property '%s' is read-only",
        lua_tostring(L, lua_upvalueindex(1)));
}

LUA_INLINE int CppBindClassMetaMethod::errorConstMismatch(lua_State* L)
{
    return luaL_error(L, "member function '%s' can not be access by const object",
        lua_tostring(L, lua_upvalueindex(1)));
}

//---------------------------------------------------------------------------

LUA_INLINE bool CppBindClassBase::buildMetaTable(LuaRef& meta, LuaRef& parent, const char* name,
    void* static_id, void* clazz_id, void* const_id)
{
    LuaRef ref = parent.rawget(name);
    if (ref != nullptr) {
        meta = ref;
        return false;
    }

    auto L = parent.state();
    std::string type_name = "class<" + CppBindModuleBase::getFullName(parent, name) + ">";

    LuaRef type_const = LuaRef::fromPtr(L, const_id);
    LuaRef type_clazz = LuaRef::fromPtr(L, clazz_id);
    LuaRef type_static = LuaRef::fromPtr(L, static_id);

    LuaRef clazz_const = LuaRef::createTable(L);
    clazz_const.setMetaTable(clazz_const);
    clazz_const.rawset("__index", &CppBindClassMetaMethod::index);
    clazz_const.rawset("__newindex", &CppBindClassMetaMethod::newIndex);
    clazz_const.rawset("___getters", LuaRef::createTable(L));
    clazz_const.rawset("___setters", LuaRef::createTable(L));
    clazz_const.rawset("___type", "const_" + type_name);
    clazz_const.rawset("___const", clazz_const);
    clazz_const.rawsetp(CppSignature<CppObject>::value(), type_const);

    LuaRef clazz = LuaRef::createTable(L);
    clazz.setMetaTable(clazz);
    clazz.rawset("__index", &CppBindClassMetaMethod::index);
    clazz.rawset("__newindex", &CppBindClassMetaMethod::newIndex);
    clazz.rawset("___getters", LuaRef::createTable(L));
    clazz.rawset("___setters", LuaRef::createTable(L));
    clazz.rawset("___type", type_name);
    clazz.rawset("___const", clazz_const);
    clazz.rawsetp(CppSignature<CppObject>::value(), type_clazz);

    LuaRef clazz_static = LuaRef::createTable(L);
    clazz_static.setMetaTable(clazz_static);
    clazz_static.rawset("__index", &CppBindClassMetaMethod::index);
    clazz_static.rawset("__newindex", &CppBindClassMetaMethod::newIndex);
    clazz_static.rawset("___getters", LuaRef::createTable(L));
    clazz_static.rawset("___setters", LuaRef::createTable(L));
    clazz_static.rawset("___type", "static_" + type_name);
    clazz_static.rawset("___class", clazz);
    clazz_static.rawset("___const", clazz_const);
    clazz_static.rawset("___parent", parent);
    clazz_static.rawsetp(CppSignature<CppObject>::value(), type_static);

    clazz_const.rawset("class", clazz_static);
    clazz.rawset("class", clazz_static);

    LuaRef registry(L, LUA_REGISTRYINDEX);
    registry.rawset(type_clazz, clazz);
    registry.rawset(type_const, clazz_const);
    registry.rawset(type_static, clazz_static);
    parent.rawset(name, clazz_static);

    meta = clazz_static;
    return true;
}

LUA_INLINE bool CppBindClassBase::buildMetaTable(LuaRef& meta, LuaRef& parent, const char* name,
    void* static_id, void* clazz_id, void* const_id, void* super_static_id)
{
    if (buildMetaTable(meta, parent, name, static_id, clazz_id, const_id)) {
        LuaRef registry(parent.state(), LUA_REGISTRYINDEX);
        LuaRef super = registry.rawgetp(super_static_id);
        meta.rawset("___super", super);
        meta.rawget("___class").rawset("___super", super.rawget("___class"));
        meta.rawget("___const").rawset("___super", super.rawget("___const"));
        return true;
    }
    return false;
}

LUA_INLINE void CppBindClassBase::setStaticGetter(const char* name, const LuaRef& getter)
{
    m_meta.rawget("___getters").rawset(name, getter);
}

LUA_INLINE void CppBindClassBase::setStaticSetter(const char* name, const LuaRef& setter)
{
    m_meta.rawget("___setters").rawset(name, setter);
}

LUA_INLINE void CppBindClassBase::setStaticReadOnly(const char* name)
{
    std::string full_name = CppBindModuleBase::getMemberName(m_meta, name);
    setStaticSetter(name, LuaRef::createFunctionWith(state(), &CppBindClassMetaMethod::errorReadOnly, full_name));
}

LUA_INLINE void CppBindClassBase::setMemberGetter(const char* name, const LuaRef& getter, const LuaRef& getter_const)
{
    m_meta.rawget("___class").rawget("___getters").rawset(name, getter);
    m_meta.rawget("___const").rawget("___getters").rawset(name, getter_const);
}

LUA_INLINE void CppBindClassBase::setMemberGetter(const char* name, const LuaRef& getter)
{
    setMemberGetter(name, getter, getter);
}

LUA_INLINE void CppBindClassBase::setMemberSetter(const char* name, const LuaRef& setter)
{
    LuaRef meta_class = m_meta.rawget("___class");
    LuaRef meta_const = m_meta.rawget("___const");
    std::string full_name = CppBindModuleBase::getMemberName(meta_class, name);
    LuaRef err = LuaRef::createFunctionWith(state(), &CppBindClassMetaMethod::errorConstMismatch, full_name);
    meta_class.rawget("___setters").rawset(name, setter);
    meta_const.rawget("___setters").rawset(name, err);
}

LUA_INLINE void CppBindClassBase::setMemberReadOnly(const char* name)
{
    LuaRef meta_class = m_meta.rawget("___class");
    LuaRef meta_const = m_meta.rawget("___const");
    std::string full_name = CppBindModuleBase::getMemberName(meta_class, name);
    LuaRef err = LuaRef::createFunctionWith(state(), &CppBindClassMetaMethod::errorReadOnly, full_name);
    meta_class.rawget("___setters").rawset(name, err);
    meta_const.rawget("___setters").rawset(name, err);
}

LUA_INLINE void CppBindClassBase::setMemberFunction(const char* name, const LuaRef& proc, bool is_const)
{
    LuaRef meta_class = m_meta.rawget("___class");
    LuaRef meta_const = m_meta.rawget("___const");
    meta_class.rawset(name, proc);
    if (is_const) {
        meta_const.rawset(name, proc);
    } else {
        std::string full_name = CppBindModuleBase::getMemberName(meta_class, name);
        LuaRef err = LuaRef::createFunctionWith(state(), &CppBindClassMetaMethod::errorConstMismatch, full_name);
        meta_const.rawset(name, err);
    }
}
