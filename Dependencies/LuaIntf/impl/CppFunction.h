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

/**
 * A template for temporately object created as lua function (with proper gc)
 * This is usually used for iterator function.
 *
 * To use this, user need to inherit CppFunctor, and override run method and optional descructor.
 * Then call make or pushToStack to create the functor object on lua stack.
 */
class CppFunctor
{
public:
    /**
     * Override destructor if you need to perform any cleanup action
     */
    virtual ~CppFunctor() {}

    /**
     * Override this method to perform lua function
     */
    virtual int run(lua_State* L) = 0;

    /**
     * Create the specified functor as callable lua object on stack
     */
    static int pushToStack(lua_State* L, CppFunctor* f);

    /**
     * Create the specified functor as callable lua object on stack,
     * the functor is created inside userdata. This is different from
     * pushToStack, which only keep the functor pointer. This reduces the
     * overhead of extra pointer and memory allocation.
     */
    template <typename FUNCTOR, typename... ARGS>
    static int make(lua_State* L, ARGS&&... args)
    {
        void* mem = lua_newuserdata(L, sizeof(FUNCTOR));
        ::new (mem) FUNCTOR(std::forward<ARGS>(args)...);
        return bind(L, &call, &gc);
    }

private:
    static int call(lua_State* L);
    static int gc(lua_State* L);

    static int callp(lua_State* L);
    static int gcp(lua_State* L);

    static int bind(lua_State* L, lua_CFunction call, lua_CFunction gc);
};

//---------------------------------------------------------------------------

template <>
struct LuaTypeMapping <lua_CFunction>
{
    static void push(lua_State* L, lua_CFunction f)
    {
        lua_pushcfunction(L, f);
    }

    static lua_CFunction get(lua_State* L, int index)
    {
        return lua_tocfunction(L, index);
    }

    static lua_CFunction opt(lua_State* L, int index, lua_CFunction def)
    {
        return lua_isnoneornil(L, index) ? def : lua_tocfunction(L, index);
    }
};

//----------------------------------------------------------------------------

template <typename FN>
struct LuaCppFunctionWrapper
{
    [[noreturn]]
    static const FN& get(lua_State* L, int)
    {
        luaL_error(L, "invalid c++ function reference");
    }
};

#if LUAINTF_STD_FUNCTION_WRAPPER

template <typename R, typename... P>
struct LuaCppFunctionWrapper <std::function<R(P...)>>
{
    static std::function<R(P...)> get(lua_State* L, int index)
    {
        LuaRef ref(L, index);
        return [ref] (P&&... arg) mutable {
            return ref.call<R>(std::forward<P>(arg)...);
        };
    }
};

#endif

//----------------------------------------------------------------------------

/**
 * Lua conversion for std::function type
 */
template <typename FN>
struct LuaCppFunction
{
    using ReturnType = typename std::conditional<LUAINTF_STD_FUNCTION_WRAPPER, FN, const FN&>::type;

    static void push(lua_State* L, const FN& proc)
    {
        using CppProc = CppBindMethod<FN>;
        LuaRef ref = LuaRef::createUserDataFrom(L, proc);
        ref.pushToStack();
        lua_pushlightuserdata(L, CppSignature<FN>::value());
        lua_pushcclosure(L, &CppProc::call, 2);
    }

    static ReturnType get(lua_State* L, int index)
    {
        index = lua_absindex(L, index);
        if (lua_iscfunction(L, index)) {
            const char* name = lua_getupvalue(L, index, 1);
            if (name && lua_isuserdata(L, -1)) {
                name = lua_getupvalue(L, index, 2);
                if (name && lua_touserdata(L, -1) == CppSignature<FN>::value()) {
                    const FN& func = *reinterpret_cast<const FN*>(lua_touserdata(L, -2));
                    assert(func);
                    lua_pop(L, 2);
                    return func;
                }
                lua_pop(L, 1);
            }
            lua_pop(L, 1);
        }

        return LuaCppFunctionWrapper<FN>::get(L, index);
    }

    static ReturnType opt(lua_State* L, int index, const FN& def)
    {
        if (lua_isnoneornil(L, index)) {
            return def;
        } else {
            return get(L, index);
        }
    }
};

template <typename R, typename... P>
struct LuaTypeMapping <R(*)(P...)>
    : LuaCppFunction <R(*)(P...)> {};

template <typename R, typename... P>
struct LuaTypeMapping <std::function<R(P...)>>
    : LuaCppFunction <std::function<R(P...)>> {};

