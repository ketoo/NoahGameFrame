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

LUA_INLINE void Lua::pushGlobal(lua_State* L, const char* name)
{
    const char* p = strchr(name, '.');
    if (p) {
        lua_pushglobaltable(L);                 // <table>
        while (p) {
            lua_pushlstring(L, name, p - name); // <table> <key>
            lua_gettable(L, -2);                // <table> <table_value>
            lua_remove(L, -2);                  // <table_value>
            if (lua_isnoneornil(L, -1)) return;
            name = p + 1;
            p = strchr(name, '.');
        }
        lua_pushstring(L, name);                // <last_table> <key>
        lua_gettable(L, -2);                    // <last_table> <table_value>
        lua_remove(L, -2);                      // <table_value>
    } else {
        lua_getglobal(L, name);
    }
}

LUA_INLINE void Lua::popToGlobal(lua_State* L, const char* name)
{
    const char* p = strchr(name, '.');
    if (p) {
        lua_pushglobaltable(L);                 // <value> <table>
        while (p) {
            lua_pushlstring(L, name, p - name); // <value> <table> <key>
            lua_gettable(L, -2);                // <value> <table> <table_value>
            lua_remove(L, -2);                  // <value> <table_value>
            name = p + 1;
            p = strchr(name, '.');
        }
        lua_pushstring(L, name);                // <value> <last_table> <name>
        lua_pushvalue(L, -3);                   // <value> <last_table> <name> <value>
        lua_settable(L, -3);                    // <value> <last_table>
        lua_pop(L, 2);
    } else {
        lua_setglobal(L, name);
    }
}

LUA_INLINE void Lua::exec(lua_State* L, const char* lua_expr, int num_results)
{
    lua_pushcfunction(L, &LuaException::traceback);

    int err = luaL_loadstring(L, lua_expr);

    if (err == LUA_OK) {
        err = lua_pcall(L, 0, num_results, -2);
    }

    if (err != LUA_OK) {
        lua_remove(L, -2);
        throw LuaException(L);
    }

    lua_remove(L, -(num_results + 1));
}

LUA_INLINE const char* LuaState::pushf(const char* fmt, ...) const
{
    va_list argp;
    va_start(argp, fmt);
    const char* ret = lua_pushvfstring(L, fmt, argp);
    va_end(argp);
    return ret;
}

LUA_INLINE int LuaState::error(const char* fmt, ...) const
{
    va_list argp;
    va_start(argp, fmt);
    lua_pushvfstring(L, fmt, argp);
    va_end(argp);
    return lua_error(L);
}
