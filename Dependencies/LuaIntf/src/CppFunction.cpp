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

LUA_INLINE int CppFunctor::call(lua_State* L)
{
    try {
        CppFunctor* f = static_cast<CppFunctor*>(lua_touserdata(L, 1));
        return f->run(L);
    } catch (std::exception& e) {
        return luaL_error(L, "%s", e.what());
    }
}

LUA_INLINE int CppFunctor::gc(lua_State* L)
{
    try {
        CppFunctor* f = static_cast<CppFunctor*>(lua_touserdata(L, 1));
        f->~CppFunctor();
        return 0;
    } catch (std::exception& e) {
        return luaL_error(L, "%s", e.what());
    }
}

LUA_INLINE int CppFunctor::callp(lua_State* L)
{
    try {
        CppFunctor* f = *static_cast<CppFunctor**>(lua_touserdata(L, 1));
        return f->run(L);
    } catch (std::exception& e) {
        return luaL_error(L, "%s", e.what());
    }
}

LUA_INLINE int CppFunctor::gcp(lua_State* L)
{
    try {
        CppFunctor* f = *static_cast<CppFunctor**>(lua_touserdata(L, 1));
        delete f;
        return 0;
    } catch (std::exception& e) {
        return luaL_error(L, "%s", e.what());
    }
}

LUA_INLINE int CppFunctor::bind(lua_State* L, lua_CFunction call, lua_CFunction gc)
{
    lua_newtable(L);
    lua_pushcfunction(L, call);
    lua_setfield(L, -2, "__call");
    lua_pushcfunction(L, gc);
    lua_setfield(L, -2, "__gc");
    lua_setmetatable(L, -2);
    return 1;
}

LUA_INLINE int CppFunctor::pushToStack(lua_State* L, CppFunctor* f)
{
    // need to create userdata, lightuserdata can't be gc
    CppFunctor** p = static_cast<CppFunctor**>(lua_newuserdata(L, sizeof(CppFunctor*)));
    *p = f;
    return bind(L, &callp, &gcp);
}
