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

struct CppBindConstant
{
    /**
     * lua_CFunction to get a constant
     *
     * The constant data is in the first upvalue.
     */
    static int call(lua_State* L)
    {
        lua_pushvalue(L, lua_upvalueindex(1));
        return 1;
    }
};

template <typename T, typename PT = T>
struct CppBindVariableGetter
{
    /**
     * lua_CFunction to get a variable
     *
     * This is used for global variables or class static data members.
     *
     * The pointer to the data is in the first upvalue.
     */
    static int call(lua_State* L)
    {
        try {
            assert(lua_islightuserdata(L, lua_upvalueindex(1)));
            auto ptr = static_cast<const T*>(lua_touserdata(L, lua_upvalueindex(1)));
            assert(ptr);

            LuaType<PT>::push(L, *ptr);
            return 1;
        } catch (std::exception& e) {
            return luaL_error(L, "%s", e.what());
        }
    }
};

template <typename T>
struct CppBindVariableSetter
{
    /**
     * lua_CFunction to set a variable.
     *
     * This is used for global variables or class static data members.
     *
     * The pointer to the data is in the first upvalue.
     */
    static int call(lua_State* L)
    {
        try {
            assert(lua_islightuserdata(L, lua_upvalueindex(1)));
            auto ptr = static_cast<T*>(lua_touserdata(L, lua_upvalueindex(1)));
            assert(ptr);

            *ptr = LuaType<T>::get(L, 1);
            return 0;
        } catch (std::exception& e) {
            return luaL_error(L, "%s", e.what());
        }
    }
};

//----------------------------------------------------------------------------

enum CppBindMethodCheck
{
    CHK_NORMAL,
    CHK_GETTER,
    CHK_SETTER,
    CHK_GETTER_INDEXED,
    CHK_SETTER_INDEXED
};

template <int CHK, typename FN, int IARG, typename R, typename... P>
struct CppBindMethodBase
{
    static_assert(CHK != CHK_GETTER || (!std::is_same<R, void>::value && sizeof...(P) == 0),
        "the specified function is not getter function");

    static_assert(CHK != CHK_SETTER || (std::is_same<R, void>::value && sizeof...(P) == 1),
        "the specified function is not setter function");

    /**
     * lua_CFunction to call a function
     *
     * The pointer to function object is in the first upvalue.
     */
    static int call(lua_State* L)
    {
        try {
            assert(lua_isuserdata(L, lua_upvalueindex(1)));
            const FN& fn = *reinterpret_cast<const FN*>(lua_touserdata(L, lua_upvalueindex(1)));
            assert(fn);

            CppArgTuple<P...> args;
            CppArgTupleInput<P...>::get(L, IARG, args);

            int n = CppInvokeMethod<FN, R, typename CppArg<P>::HolderType...>::push(L, fn, args);
            return n + CppArgTupleOutput<P...>::push(L, args);
        } catch (std::exception& e) {
            return luaL_error(L, "%s", e.what());
        }
    }

    template <typename PROC>
    static FN function(const PROC& fn)
    {
        return static_cast<FN>(fn);
    }
};

template <typename FN, typename ARGS = FN, int IARG = 1, int CHK = CHK_NORMAL, typename ENABLED = void>
struct CppBindMethod;

template <typename R, typename... P, int IARG, int CHK>
struct CppBindMethod <R(*)(P...), R(*)(P...), IARG, CHK>
    : CppBindMethodBase <CHK, R(*)(P...), IARG, R, P...> {};

template <typename R, typename... P, int IARG, int CHK>
struct CppBindMethod <std::function<R(P...)>, std::function<R(P...)>, IARG, CHK>
    : CppBindMethodBase <CHK, std::function<R(P...)>, IARG, R, P...> {};

template <typename R, typename... A, typename... P, int IARG, int CHK>
struct CppBindMethod <R(*)(A...), _arg(*)(P...), IARG, CHK>
    : CppBindMethodBase <CHK, R(*)(A...), IARG, R, P...>
{
    static_assert(sizeof...(A) == sizeof...(P),
        "the number of arguments and argument-specs do not match");
};

template <typename R, typename... A, typename... P, int IARG, int CHK>
struct CppBindMethod <std::function<R(A...)>, _arg(*)(P...), IARG, CHK>
    : CppBindMethodBase <CHK, std::function<R(A...)>, IARG, R, P...>
{
    static_assert(sizeof...(A) == sizeof...(P),
        "the number of arguments and argument-specs do not match");
};

template <typename FN, int IARG, int CHK>
struct CppBindMethod <FN, FN, IARG, CHK,
        typename std::enable_if<CppCouldBeLambda<FN>::value>::type>
    : CppBindMethod <typename CppLambdaTraits<FN>::FunctionType, typename CppLambdaTraits<FN>::FunctionType, IARG, CHK> {};

template <typename FN, typename... P, int IARG, int CHK>
struct CppBindMethod <FN, _arg(*)(P...), IARG, CHK,
        typename std::enable_if<CppCouldBeLambda<FN>::value>::type>
    : CppBindMethod <typename CppLambdaTraits<FN>::FunctionType, _arg(*)(P...), IARG, CHK> {};

template <typename FN, int IARG, int CHK>
struct CppBindMethod <FN, FN, IARG, CHK,
        typename std::enable_if<std::is_function<FN>::value>::type>
    : CppBindMethod <FN*, FN*, IARG, CHK> {};

template <typename FN, typename... P, int IARG, int CHK>
struct CppBindMethod <FN, _arg(*)(P...), IARG, CHK,
        typename std::enable_if<std::is_function<FN>::value>::type>
    : CppBindMethod <FN*, _arg(*)(P...), IARG, CHK> {};

//----------------------------------------------------------------------------

struct CppBindModuleMetaMethod
{
    /**
     * __index metamethod for module members.
     *
     * Retrieving global functions methods, stored in the metatable.
     * Reading global variable and properties, stored in the ___getters table.
     */
    static int index(lua_State* L);

    /**
     * __newindex metamethod for module members.
     *
     * The ___setters table stores proxy functions for assignment to
     * global variable and properties
     */
    static int newIndex(lua_State* L);

    /**
     * Forward __call to sub class or module.
     * The name of sub class/module is in upvalue(1)
     */
    static int forwardCall(lua_State* L);

    /**
     * lua_CFunction to report an error writing to a read-only value.
     *
     * The name of the variable is in the first upvalue.
     */
    static int errorReadOnly(lua_State* L);
};

//----------------------------------------------------------------------------

class CppBindModuleBase
{
    friend class CppBindClassBase;

protected:
    explicit CppBindModuleBase(const LuaRef& meta)
        : m_meta(meta)
    {
        m_meta.checkTable();
    }

    CppBindModuleBase(LuaRef& meta, const char* module);

    static std::string getFullName(const LuaRef& parent, const char* name);
    static std::string getMemberName(const LuaRef& parent, const char* name);

    void setGetter(const char* name, const LuaRef& getter);
    void setSetter(const char* name, const LuaRef& setter);
    void setReadOnly(const char* name);

public:
    /**
     * Copy constructor.
     */
    CppBindModuleBase(const CppBindModuleBase& that)
        : m_meta(that.m_meta)
        {}

    /**
     * Move constructor for temporaries.
     */
    CppBindModuleBase(CppBindModuleBase&& that)
        : m_meta(std::move(that.m_meta))
        {}

    /**
     * Copy assignment.
     */
    CppBindModuleBase& operator = (const CppBindModuleBase& that)
    {
        m_meta = that.m_meta;
        return *this;
    }

    /**
     * Move assignment for temporaries.
     */
    CppBindModuleBase& operator = (CppBindModuleBase&& that)
    {
        m_meta = std::move(that.m_meta);
        return *this;
    }

    /**
     * The underlying lua state.
     */
    lua_State* state() const
    {
        return m_meta.state();
    }

    /**
     * The underlying meta table.
     */
    LuaRef meta() const
    {
        return m_meta;
    }

protected:
    LuaRef m_meta;
};

//----------------------------------------------------------------------------

template <typename T, typename PARENT>
class CppBindClass;

/**
 * Provides C++ to Lua registration capabilities.
 *
 * This class is not instantiated directly, call LuaBinding(L) to start
 * the registration process.
 */
template <typename PARENT>
class CppBindModule : public CppBindModuleBase
{
    friend class LuaBinding;
    template <typename T, typename P> friend class CppBindClass;
    template <typename P> friend class CppBindModule;

    explicit CppBindModule(const LuaRef& meta)
        : CppBindModuleBase(meta)
        {}

    CppBindModule(LuaRef& meta, const char* module)
        : CppBindModuleBase(meta, module)
        {}

public:
    /**
     * Copy constructor.
     */
    CppBindModule(const CppBindModule<PARENT>& that)
        : CppBindModuleBase(that)
        {}

    /**
     * Move constructor for temporaries.
     */
    CppBindModule(CppBindModule<PARENT>&& that)
        : CppBindModuleBase(std::move(that))
        {}

    /**
     * Copy assignment.
     */
    CppBindModule<PARENT>& operator = (const CppBindModule<PARENT>& that)
    {
        m_meta = that.m_meta;
        return *this;
    }

    /**
     * Move assignment for temporaries.
     */
    CppBindModule<PARENT>& operator = (CppBindModule<PARENT>&& that)
    {
        m_meta = std::move(that.m_meta);
        return *this;
    }

    /**
     * Add or replace a constant value.
     */
    template <typename V>
    CppBindModule<PARENT>& addConstant(const char* name, const V& v)
    {
        LuaRef r = LuaRef::fromValue(state(), v);
        if (r.isFunction()) {
            r = LuaRef::createFunctionWith(state(), &CppBindConstant::call, r);
        }
        setGetter(name, r);
        setReadOnly(name);
        return *this;
    }

    /**
     * Add or replace a non-const variable.
     * The value return to lua is pass-by-value, that will create a local copy in lua.
     * This is different from addVariableRef, which is pass-by-reference, and allow direct access to the variable.
     * This apply only to the class type, the primitive types are always pass-by-value.
     */
    template <typename V>
    CppBindModule<PARENT>& addVariable(const char* name, V* v, bool writable = true)
    {
        setGetter(name, LuaRef::createFunctionWithPtr(state(), &CppBindVariableGetter<V>::call, v));
        if (writable) {
            setSetter(name, LuaRef::createFunctionWithPtr(state(), &CppBindVariableSetter<V>::call, v));
        } else {
            setReadOnly(name);
        }
        return *this;
    }

    /**
     * Add or replace a const read-only variable.
     * The value return to lua is pass-by-value, that will create a local copy in lua.
     * This is different from addVariableRef, which is pass-by-reference, and allow direct access to the variable.
     * This apply only to the class type, the primitive types are always pass-by-value.
     */
    template <typename V>
    CppBindModule<PARENT>& addVariable(const char* name, const V* v)
    {
        setGetter(name, LuaRef::createFunctionWithPtr(state(), &CppBindVariableGetter<V>::call, v));
        setReadOnly(name);
        return *this;
    }

    /**
     * Add or replace a non-const variable.
     * The value return to lua is pass-by-reference, and allow direct access to the variable.
     * This is different from addVariable, which is pass-by-value, and will create a local copy upon access.
     * This apply only to the class type, the primitive types are always pass-by-value.
     */
    template <typename V>
    typename std::enable_if<std::is_copy_assignable<V>::value, CppBindModule<PARENT>&>::type
        addVariableRef(const char* name, V* v, bool writable = true)
    {
        setGetter(name, LuaRef::createFunctionWithPtr(state(), &CppBindVariableGetter<V, V&>::call, v));
        if (writable) {
            setSetter(name, LuaRef::createFunctionWithPtr(state(), &CppBindVariableSetter<V>::call, v));
        } else {
            setReadOnly(name);
        }
        return *this;
    }

    /**
     * Add or replace a non-const variable.
     * The value return to lua is pass-by-reference, and allow direct access to the variable.
     * This is different from addVariable, which is pass-by-value, and will create a local copy upon access.
     * This apply only to the class type, the primitive types are always pass-by-value.
     */
    template <typename V>
    typename std::enable_if<!std::is_copy_assignable<V>::value, CppBindModule<PARENT>&>::type
        addVariableRef(const char* name, V* v)
    {
        setGetter(name, LuaRef::createFunctionWithPtr(state(), &CppBindVariableGetter<V, V&>::call, v));
        setReadOnly(name);
        return *this;
    }

    /**
     * Add or replace a const read-only variable.
     * The value return to lua is pass-by-reference, and allow direct access to the variable.
     * This is different from addVariable, which is pass-by-value, and will create a local copy upon access.
     * This apply only to the class type, the primitive types are always pass-by-value.
     */
    template <typename V>
    CppBindModule<PARENT>& addVariableRef(const char* name, const V* v)
    {
        setGetter(name, LuaRef::createFunctionWithPtr(state(), &CppBindVariableGetter<V, const V&>::call, v));
        setReadOnly(name);
        return *this;
    }

    /**
     * Add or replace a read-write property.
     */
    template <typename FG, typename FS>
    CppBindModule<PARENT>& addProperty(const char* name, const FG& get, const FS& set)
    {
        using CppGetter = CppBindMethod<FG, FG, 1, CHK_GETTER>;
        using CppSetter = CppBindMethod<FS, FS, 1, CHK_SETTER>;
        setGetter(name, LuaRef::createFunction(state(), &CppGetter::call, CppGetter::function(get)));
        setSetter(name, LuaRef::createFunction(state(), &CppSetter::call, CppSetter::function(set)));
        return *this;
    }

    /**
     * Add or replace a read-only property.
     */
    template <typename FN>
    CppBindModule<PARENT>& addProperty(const char* name, const FN& get)
    {
        using CppGetter = CppBindMethod<FN, FN, 1, CHK_GETTER>;
        setGetter(name, LuaRef::createFunction(state(), &CppGetter::call, CppGetter::function(get)));
        setReadOnly(name);
        return *this;
    }

    /**
     * Add or replace a function.
     */
    template <typename FN>
    CppBindModule<PARENT>& addFunction(const char* name, const FN& proc)
    {
        using CppProc = CppBindMethod<FN>;
        m_meta.rawset(name, LuaRef::createFunction(state(), &CppProc::call, CppProc::function(proc)));
        return *this;
    }

    /**
     * Add or replace a function, user can specify augument spec.
     */
    template <typename FN, typename ARGS>
    CppBindModule<PARENT>& addFunction(const char* name, const FN& proc, ARGS)
    {
        using CppProc = CppBindMethod<FN, ARGS>;
        m_meta.rawset(name, LuaRef::createFunction(state(), &CppProc::call, CppProc::function(proc)));
        return *this;
    }

    /**
     * Add or replace a factory function.
     */
    template <typename FN>
    CppBindModule<PARENT>& addFactory(const FN& proc)
    {
        using CppProc = CppBindMethod<FN, FN, 2>;
        m_meta.rawset("__call", LuaRef::createFunction(state(), &CppProc::call, CppProc::function(proc)));
        return *this;
    }

    /**
     * Add or replace a factory function, user can specify augument spec.
     */
    template <typename FN, typename ARGS>
    CppBindModule<PARENT>& addFactory(const FN& proc, ARGS)
    {
        using CppProc = CppBindMethod<FN, ARGS, 2>;
        m_meta.rawset("__call", LuaRef::createFunction(state(), &CppProc::call, CppProc::function(proc)));
        return *this;
    }

    /**
     * Add or replace a factory function, that forward call to sub module factory (or class constructor).
     */
    CppBindModule<PARENT>& addFactory(const char* name)
    {
        m_meta.rawset("__call", LuaRef::createFunctionWith(state(), &CppBindModuleMetaMethod::forwardCall, name));
        return *this;
    }

    /**
     * Open a new or existing CppBindModule for registrations.
     */
    CppBindModule<CppBindModule<PARENT>> beginModule(const char* name)
    {
        return CppBindModule<CppBindModule<PARENT>>(m_meta, name);
    }

    /**
     * Continue CppBindModule registration in the parent.
     */
    PARENT endModule()
    {
        return PARENT(m_meta.rawget("___parent"));
    }

    /**
     * Open a new or existing class for registrations.
     */
    template <typename T>
    CppBindClass<T, CppBindModule<PARENT>> beginClass(const char* name)
    {
        return CppBindClass<T, CppBindModule<PARENT>>::bind(m_meta, name);
    }

    /**
     * Open a new class to extend the base class.
     */
    template <typename T, typename SUPER>
    CppBindClass<T, CppBindModule<PARENT>> beginExtendClass(const char* name)
    {
        return CppBindClass<T, CppBindModule<PARENT>>::template extend<SUPER>(m_meta, name);
    }
};

//---------------------------------------------------------------------------

class LuaBinding
{
public:
    /**
     * Module binds to a global variable (lua 5.0 style)
     *
     * It is recommended to put your module inside the global, and
     * then add your classes and functions to it, rather than adding many classes
     * and functions directly to the global.
     */
    explicit LuaBinding(lua_State* L)
        : m_meta(LuaRef::globals(L))
        {}

    /**
     * Module binds to a local LuaRef value.
     *
     * This can be used to implement Lua 5.1 module style:
     *
     *     extern "C" int luaopen_modname(lua_State* L)
     *     {
     *         LuaRef mod = LuaRef::createTable(L);
     *         LuaBinding(mod)
     *             ...;
     *         mod.pushToStack();
     *         return 1;
     *     }
     */
    explicit LuaBinding(const LuaRef& mod)
        : m_meta(mod)
        {}

    /**
     * Copy constructor.
     */
    LuaBinding(const LuaBinding& that)
        : m_meta(that.m_meta)
        {}

    /**
     * Move constructor for temporaries.
     */
    LuaBinding(LuaBinding&& that)
        : m_meta(std::move(that.m_meta))
        {}

    /**
     * Copy assignment.
     */
    LuaBinding& operator = (const LuaBinding& that)
    {
        m_meta = that.m_meta;
        return *this;
    }

    /**
     * Move assignment for temporaries.
     */
    LuaBinding& operator = (LuaBinding&& that)
    {
        m_meta = std::move(that.m_meta);
        return *this;
    }

    /**
     * The underlying lua state.
     */
    lua_State* state() const
    {
        return m_meta.state();
    }

    /**
     * Add or replace a function.
     */
    template <typename FN>
    LuaBinding& addFunction(const char* name, const FN& proc)
    {
        using CppProc = CppBindMethod<FN>;
        m_meta.rawset(name, LuaRef::createFunction(state(), &CppProc::call, CppProc::function(proc)));
        return *this;
    }

    /**
     * Add or replace a function, user can specify augument spec.
     */
    template <typename FN, typename ARGS>
    LuaBinding& addFunction(const char* name, const FN& proc, ARGS)
    {
        using CppProc = CppBindMethod<FN, ARGS>;
        m_meta.rawset(name, LuaRef::createFunction(state(), &CppProc::call, CppProc::function(proc)));
        return *this;
    }

    /**
     * Open a new or existing CppBindModule for registrations.
     */
    CppBindModule<LuaBinding> beginModule(const char* name)
    {
        return CppBindModule<LuaBinding>(m_meta, name);
    }

    /**
     * Open a new or existing class for registrations.
     */
    template <typename T>
    CppBindClass<T, LuaBinding> beginClass(const char* name)
    {
        return CppBindClass<T, LuaBinding>::bind(m_meta, name);
    }

    /**
     * Open a new class to extend the base class.
     */
    template <typename T, typename SUPER>
    CppBindClass<T, LuaBinding> beginExtendClass(const char* name)
    {
        return CppBindClass<T, LuaBinding>::template extend<SUPER>(m_meta, name);
    }

private:
    LuaRef m_meta;
};
