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

LUA_INLINE void CppObject::typeMismatchError(lua_State* L, int index)
{
    // <SP: index> = <obj>
    // <SP: -2> = <expected_mt>
    // <SP: -1> = <actual_mt>

    // now get the expected type -> <expected_mt> <actual_mt> <expected>
    lua_pushliteral(L, "___type");
    lua_rawget(L, -3);
    const char* expected = lua_tostring(L, -1);

    // now get the actual got type -> <expected_mt> <actual_mt> <expected> <actual>
    lua_pushliteral(L, "___type");
    lua_rawget(L, -3);
    const char* actual = lua_tostring(L, -1);
    if (actual == nullptr) {
        actual = lua_typename(L, lua_type(L, index));
    }

    // now create error msg, put it into bottom and pop all others -> <msg>
    luaL_where(L, 1);
    lua_pushfstring(L, "%s expected, got %s", expected, actual);
    lua_concat(L, 2);
    lua_error(L);
}

LUA_INLINE CppObject* CppObject::getObject(lua_State* L, int index, void* class_id,
    bool is_const, bool is_exact, bool raise_error)
{
    if (!lua_isuserdata(L, index)) {
        if (raise_error) {
            luaL_error(L, "expect userdata, got %s", lua_typename(L, lua_type(L, index)));
        }
        return nullptr;
    }

    // <SP: index> = <obj>
    index = lua_absindex(L, index);

    // get registry base class metatable -> <base_mt>
    lua_rawgetp(L, LUA_REGISTRYINDEX, class_id);

    // report error if no metatable
    if (!lua_istable(L, -1)) {
        if (raise_error) {
            luaL_error(L, "unknown class, you need to register this class with lua-intf first by using LuaBinding");
        } else {
            lua_pop(L, 1);
        }
        return nullptr;
    }

    // get the object metatable -> <base_mt> <obj_mt>
    lua_getmetatable(L, index);

    // use const metatable if needed
    if (is_const && !is_exact) {
        // get the const metatable -> <base_mt> <const_obj_mt>
        lua_pushliteral(L, "___const");
        lua_rawget(L, -2);
        lua_remove(L, -2);

        // report error if no const metatable
        if (!lua_istable(L, -1)) {
            if (raise_error) {
                luaL_error(L, "unknown class, you need to register this class with lua-intf first by using LuaBinding");
            } else {
                lua_pop(L, 2);
            }
            return nullptr;
        }
    }

    for (;;) {
        // check if <obj_mt> and <base_mt> are equal
        if (lua_rawequal(L, -1, -2)) {
            // matched, return this object
            lua_pop(L, 2);
            break;
        }

        // give up if exact match is needed
        if (is_exact) {
            if (raise_error) {
                typeMismatchError(L, index);
            } else {
                lua_pop(L, 2);
            }
            return nullptr;
        }

        // now try super class -> <base_mt> <obj_mt> <obj_super_mt>
        lua_pushliteral(L, "___super");
        lua_rawget(L, -2);

        if (lua_isnil(L, -1)) {
            // no super class
            if (raise_error) {
                lua_pop(L, 1); // pop nil
                typeMismatchError(L, index);
            } else {
                lua_pop(L, 3);
            }
            return nullptr;
        } else {
            // continue with <obj_super_mt> -> <base_mt> <obj_super_mt>
            lua_remove(L, -2);
        }
    }

    return static_cast<CppObject*>(lua_touserdata(L, index));
}

