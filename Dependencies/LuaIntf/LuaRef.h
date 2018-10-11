//
// https://github.com/SteveKChiu/lua-intf
//
// Copyright 2013, Steve Chiu <steve.k.chiu@gmail.com>
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

#ifndef LUAREF_H
#define LUAREF_H

//---------------------------------------------------------------------------

#include "LuaState.h"
#include <functional>
#include <tuple>
#include <memory>

namespace LuaIntf
{

class LuaRef;

//---------------------------------------------------------------------------

/**
 * Assignable and convertible result of bracket-style lookup.
 */
class LuaTableRef
{
public:
    /**
     * Create new index association.
     *
     * @param state the Lua state
     * @param table the table reference (no auto unref)
     * @param key the table key reference (it will be unref automatically)
     */
    LuaTableRef(lua_State* state, int table, int key)
        : L(state)
        , m_table(table)
        , m_key(key)
    {
        assert(L);
    }

    /**
     * Copy constructor for LuaTableRef.
     */
    LuaTableRef(const LuaTableRef& that)
        : L(that.L)
        , m_table(that.m_table)
    {
        assert(L);
        lua_rawgeti(L, LUA_REGISTRYINDEX, that.m_key);
        m_key = luaL_ref(L, LUA_REGISTRYINDEX);
    }

    /**
     * Move constructor for LuaTableRef.
     */
    LuaTableRef(LuaTableRef&& that)
        : L(that.L)
        , m_table(that.m_table)
        , m_key(that.m_key)
    {
        assert(L);
        that.m_key = LUA_NOREF;
    }

    /**
     * Destructor for LuaTableRef.
     */
    ~LuaTableRef()
    {
        luaL_unref(L, LUA_REGISTRYINDEX, m_key);
    }

    /**
     * Copy value from other table[key] reference.
     *
     * @param that reference from other table[key]
     */
    LuaTableRef& operator = (const LuaTableRef& that);

    /**
     * Assign value for table[key].
     *
     * @param value new value for table[key]
     * @throw LuaException if V are not convertible to Lua types
     */
    template <typename V>
    LuaTableRef& operator = (const V& value)
    {
        lua_rawgeti(L, LUA_REGISTRYINDEX, m_table);
        lua_rawgeti(L, LUA_REGISTRYINDEX, m_key);
        Lua::push(L, value);
        lua_settable(L, -3);
        lua_pop(L, 1);
        return *this;
    }

    /**
     * Get value of table[key].
     * This may raise Lua error or throw LuaException if value is not convertible.
     *
     * @return value of t[k] as type V
     * @throw LuaException if V are not convertible to Lua types
     */
    template <typename V = LuaRef>
    V value() const
    {
        lua_rawgeti(L, LUA_REGISTRYINDEX, m_table);
        lua_rawgeti(L, LUA_REGISTRYINDEX, m_key);
        lua_gettable(L, -2);
        V v = Lua::get<V>(L, -1);
        lua_pop(L, 2);
        return v;
    }

private:
    lua_State* L;
    int m_table;
    int m_key;
};

//---------------------------------------------------------------------------

/**
 * C++ style const iterator for this table
 */
class LuaTableIterator
{
public:
    /**
     * Create empty LuaTableIterator, it must be assigned to other LuaTableIterator before using.
     */
    constexpr LuaTableIterator()
        : L(nullptr)
        , m_table(LUA_NOREF)
        , m_key(LUA_NOREF)
        , m_value(LUA_NOREF)
        {}

    /**
     * Create LuaTableIterator for table.
     *
     * @param state the Lua state
     * @param table the Lua table reference (no auto unref)
     * @param fetch_next true if fetch next entry first
     */
    LuaTableIterator(lua_State* state, int table, bool fetch_next);

    /**
     * Copy constructor for LuaTableIterator.
     */
    LuaTableIterator(const LuaTableIterator& that);

    /**
     * Move constructor for LuaTableIterator.
     */
    LuaTableIterator(LuaTableIterator&& that);

    /**
     * Destructor for LuaTableIterator.
     */
    ~LuaTableIterator();

    /**
     * Get entry (for loop inerator compatibility).
     */
    const LuaTableIterator& operator * () const
    {
        return *this;
    }

    /**
     * Advance to next entry.
     */
    LuaTableIterator& operator ++ ()
    {
        next();
        return *this;
    }

    /**
     * Copy assignment for LuaTableIterator.
     */
    LuaTableIterator& operator = (const LuaTableIterator& that);

    /**
     * Move assignment for LuaTableIterator.
     */
    LuaTableIterator& operator = (LuaTableIterator&& that);

    /**
     * Test whether the two iterator is at same position.
     */
    bool operator == (const LuaTableIterator& that) const;

    /**
     * Test whether the two iterator is not at same position.
     */
    bool operator != (const LuaTableIterator& that) const
    {
        return !operator == (that);
    }

    /**
     * Get the key of current entry.
     * This may raise Lua error or throw LuaException if key is not convertible.
     */
    template <typename K = LuaRef>
    K key() const
    {
        assert(L);
        lua_rawgeti(L, LUA_REGISTRYINDEX, m_key);
        return Lua::pop<K>(L);
    }

    /**
     * Get the value of current entry.
     * This may raise Lua error or throw LuaException if value is not convertible.
     */
    template <typename V = LuaRef>
    V value() const
    {
        assert(L);
        lua_rawgeti(L, LUA_REGISTRYINDEX, m_value);
        return Lua::pop<V>(L);
    }

private:
    void next();

private:
    lua_State* L;
    int m_table;
    int m_key;
    int m_value;
};

//---------------------------------------------------------------------------

/**
 * Lightweight reference to a Lua object.
 *
 * The reference is maintained for the lifetime of the C++ object.
 */
class LuaRef
{
public:
    /**
     * Create new userdata with requested size.
     *
     * @param L Lua state
     * @param userdata_size the size of userdata to allocate
     * @param out_userdata [out] the pointer to allocated userdata, can be nullptr if not needed
     */
    static LuaRef createUserData(lua_State* L, size_t userdata_size, void** out_userdata = nullptr)
    {
        void* userdata = lua_newuserdata(L, userdata_size);
        if (out_userdata) *out_userdata = userdata;
        return popFromStack(L);
    }

    /**
     * Create new userdata with specified object.
     * The object is copied to userdata (by using copy constructor).
     * If the given object has non trivial destructor, it will be called upon gc.
     *
     * @param L Lua state
     * @param cpp_obj the object to be copied
     */
    template <typename T>
    static LuaRef createUserDataFrom(lua_State* L, const T& cpp_obj)
    {
        pushUserDataFrom(L, cpp_obj);
        return popFromStack(L);
    }

    /**
     * Create new lua_CFunction with multiple upvalues.
     *
     * @param L Lua state
     * @param proc the lua_CFunction
     * @param upvalues as upvalues(n) to the lua_CFunction
     */
    template <typename... P>
    static LuaRef createFunctionWith(lua_State* L, lua_CFunction proc, P&&... upvalues)
    {
        pushArg(L, std::forward<P>(upvalues)...);
        lua_pushcclosure(L, proc, sizeof...(P));
        return popFromStack(L);
    }

    /**
     * Create new lua_CFunction with context (lightuserdata) as upvalue(1).
     *
     * @param L Lua state
     * @param proc the lua_CFunction
     * @param ptr the context pointer as upvalue(1)
     */
    static LuaRef createFunctionWithPtr(lua_State* L, lua_CFunction proc, const void* ptr)
    {
        lua_pushlightuserdata(L, const_cast<void*>(ptr));
        lua_pushcclosure(L, proc, 1);
        return popFromStack(L);
    }

    /**
     * Create new lua_CFunction with allocated object as upvalue(1).
     * The object is copied to upvalue(1) (by using copy constructor).
     * If the given object has non trivial destructor, it will be called upon gc.
     *
     * @param L Lua state
     * @param proc the lua_CFunction
     * @param cpp_obj the data to be copied as upvalue(1)
     */
    template <typename T>
    static typename std::enable_if<std::is_function<T>::value, LuaRef>::type
        createFunction(lua_State* L, lua_CFunction proc, const T& cpp_obj)
    {
        pushUserDataFrom<T*>(L, &cpp_obj);
        lua_pushcclosure(L, proc, 1);
        return popFromStack(L);
    }

    /**
     * Create new lua_CFunction with allocated object as upvalue(1).
     * The object is copied to upvalue(1) (by using copy constructor).
     * If the given object has non trivial destructor, it will be called upon gc.
     *
     * @param L Lua state
     * @param proc the lua_CFunction
     * @param cpp_obj the data to be copied as upvalue(1)
     */
    template <typename T>
    static typename std::enable_if<!std::is_function<T>::value, LuaRef>::type
        createFunction(lua_State* L, lua_CFunction proc, const T& cpp_obj)
    {
        pushUserDataFrom(L, cpp_obj);
        lua_pushcclosure(L, proc, 1);
        return popFromStack(L);
    }

    /**
     * Create a new, empty table.
     *
     * @param L Lua state
     * @param narr pre-allocate space for this number of array elements
     * @param nrec pre-allocate space for this number of non-array elements
     * @return empty table
     */
    static LuaRef createTable(lua_State* L, int narr = 0, int nrec = 0)
    {
        lua_createtable(L, narr, nrec);
        return popFromStack(L);
    }

    /**
     * Create a new table and set the meta table, this behaves like an object.
     *
     * @param L Lua state
     * @param meta the name of meta table
     * @return empty table
     */
    static LuaRef createTableWithMeta(lua_State* L, const char* meta)
    {
        lua_newtable(L);
        Lua::pushGlobal(L, meta);
        lua_setmetatable(L, -2);
        return popFromStack(L);
    }

    /**
     * Get registry table.
     */
    static LuaRef registry(lua_State* L)
    {
        return LuaRef(L, LUA_REGISTRYINDEX);
    }

    /**
     * Get global table (_G).
     */
    static LuaRef globals(lua_State* L)
    {
        lua_pushglobaltable(L);
        return popFromStack(L);
    }

    /**
     * Create empty reference.
     */
    constexpr LuaRef()
        : L(nullptr)
        , m_ref(LUA_NOREF)
        {}

    /**
     * Create reference to Lua nil.
     */
    LuaRef(lua_State* state, std::nullptr_t)
        : L(state)
        , m_ref(LUA_REFNIL)
    {
        assert(L);
    }

    /**
     * Create reference to object on stack, stack is not changed.
     *
     * @param L Lua state
     * @param index position on stack
     */
    LuaRef(lua_State* state, int index)
        : L(state)
    {
        assert(L);
        lua_pushvalue(L, index);
        m_ref = luaL_ref(L, LUA_REGISTRYINDEX);
    }

    /**
     * Create reference to Lua global.
     *
     * @param L Lua state
     * @param name the global name, may contains '.' to access sub obejct
     */
    LuaRef(lua_State* state, const char* name)
        : L(state)
    {
        assert(L);
        Lua::pushGlobal(L, name);
        m_ref = luaL_ref(L, LUA_REGISTRYINDEX);
    }

    /**
     * Copy constructor, this still refer to the same Lua object.
     */
    LuaRef(const LuaRef& that);

    /**
     * Move constructor, this still refer to the same Lua object.
     */
    LuaRef(LuaRef&& that)
        : L(that.L)
        , m_ref(that.m_ref)
    {
        that.m_ref = LUA_NOREF;
    }

    /**
     * Type conversion for LuaTableRef.
     */
    LuaRef(const LuaTableRef& that)
        : LuaRef(that.value())
        {}

    /**
     * Destructor, release the reference.
     */
    ~LuaRef()
    {
        if (L) {
            luaL_unref(L, LUA_REGISTRYINDEX, m_ref);
        }
    }

    /**
     * Copy assignment, this only copy the reference, so both refer to the same object.
     */
    LuaRef& operator = (const LuaRef& that);

    /**
     * Move assignment, this only apply to the reference, the original object is not changed.
     */
    LuaRef& operator = (LuaRef&& that)
    {
        std::swap(L, that.L);
        std::swap(m_ref, that.m_ref);
        return *this;
    }

    /**
     * Reset to nil reference, the original object is not changed.
     */
    LuaRef& operator = (std::nullptr_t)
    {
        if (L) {
            luaL_unref(L, LUA_REGISTRYINDEX, m_ref);
            m_ref = LUA_REFNIL;
        }
        return *this;
    }

    /**
     * Get the underlying Lua state.
     */
    lua_State* state() const
    {
        return L;
    }

    /**
     * Check whether the reference is valid (has been bind to Lua).
     */
    bool isValid() const
    {
        return m_ref != LUA_NOREF;
    }

    /**
     * Test whether this ref is table.
     */
    bool isTable() const
    {
        return type() == LuaTypeID::TABLE;
    }

    /**
     * Test whether this ref is function.
     */
    bool isFunction() const
    {
        return type() == LuaTypeID::FUNCTION;
    }

    /**
     * Get the Lua type id.
     */
    LuaTypeID type() const;

    /**
     * Get the Lua type name.
     */
    const char* typeName() const
    {
        pushToStack();
        const char* s = luaL_typename(L, -1);
        lua_pop(L, 1);
        return s;
    }

    /**
     * Check whether the reference is table.
     * This may raise Lua error if type is not matched.
     */
    const LuaRef& checkTable() const
    {
        return checkType(LuaTypeID::TABLE);
    }

    /**
     * Check whether the reference is function.
     * This may raise Lua error if type is not matched.
     */
    const LuaRef& checkFunction() const
    {
        return checkType(LuaTypeID::FUNCTION);
    }

    /**
     * Check whether the reference is in given type.
     * This may raise Lua error if type is not matched.
     */
    const LuaRef& checkType(LuaTypeID type) const
    {
        pushToStack();
        luaL_checktype(L, -1, static_cast<int>(type));
        lua_pop(L, 1);
        return *this;
    }

    /**
     * Test whether this reference is identical to the other reference (not via any metatable).
     */
    bool isIdenticalTo(const LuaRef& r) const;

    /**
     * Compare this reference to the other reference.
     * @return 0 if equal; -1 if less than; 1 if greater than
     */
    int compareTo(const LuaRef& r) const;

    /**
     * Test whether this reference is equal to the other reference.
     */
    bool operator == (const LuaRef& r) const;

    /**
     * Test whether this reference is nil.
     */
    bool operator == (std::nullptr_t) const
    {
        return m_ref == LUA_REFNIL || m_ref == LUA_NOREF;
    }

    /**
     * Test whether this reference is not equal to the other reference.
     */
    bool operator != (const LuaRef& r) const
    {
        return !operator == (r);
    }

    /**
     * Test whether this is not nil.
     */
    bool operator != (std::nullptr_t r) const
    {
        return !operator == (r);
    }

    /**
     * Test whether this reference is less than the other reference.
     */
    bool operator < (const LuaRef& r) const;

    /**
     * Test whether this reference is less than or equal to the other reference.
     */
    bool operator <= (const LuaRef& r) const;

    /**
     * Test whether this reference is greater than the other reference.
     */
    bool operator > (const LuaRef& r) const
    {
        return !operator <= (r);
    }

    /**
     * Test whether this reference is greater than or equal to the other reference.
     */
    bool operator >= (const LuaRef& r) const
    {
        return !operator < (r);
    }

    /**
     * Test whether this reference is valid and not nil.
     */
    explicit operator bool () const
    {
        return m_ref != LUA_NOREF && m_ref != LUA_REFNIL;
    }

    /**
     * Push value of this reference to Lua stack.
     */
    void pushToStack() const
    {
        assert(L);
        lua_rawgeti(L, LUA_REGISTRYINDEX, m_ref);
    }

    /**
     * Create LuaRef from top of Lua stack, the top value is popped from stack.
     */
    static LuaRef popFromStack(lua_State* L)
    {
        return LuaRef(L);
    }

    /**
     * Cast to the given value type.
     * This may raise Lua error or throw LuaException if value is not convertible.
     */
    template <typename T>
    T toValue() const
    {
        pushToStack();
        return Lua::pop<T>(L);
    }

    /**
     * Create LuaRef from value.
     * This may raise Lua error or throw LuaException if value is not convertible.
     */
    template <typename T>
    static LuaRef fromValue(lua_State* L, const T& value)
    {
        Lua::push(L, value);
        return popFromStack(L);
    }

    /**
     * Cast to the data pointer, if this ref is not pointer, return nullptr.
     */
    void* toPtr() const
    {
        pushToStack();
        void* ptr = lua_touserdata(L, -1);
        lua_pop(L, 1);
        return ptr;
    }

    /**
     * Create LuaRef from data pointer, the data pointer life time is managed by C++.
     */
    static LuaRef fromPtr(lua_State* L, void* ptr)
    {
        lua_pushlightuserdata(L, ptr);
        return popFromStack(L);
    }

    /**
     * Call this function.
     * This may raise Lua error or throw LuaException if arguments are not convertible.
     *
     * @param args arguments to pass to function
     */
    template <typename... P>
    void operator () (P&&... args) const
    {
        assert(L);
        Call<void, P...>::invoke(L, *this, std::forward<P>(args)...);
    }

    /**
     * Call this function and get return value(s).
     * This may raise Lua error or throw LuaException if result or arguments are not convertible.
     *
     * To return multiple values from Lua function, use std::tuple as return type:
     *
     * int a, b;
     * std::tie(a, b) = ref.call<std::tuple<int, int>>(arg1, arg2);
     *
     * The above code works like the following in Lua:
     *
     * local a, b = ref(arg1, arg2)
     *
     * @param args arguments to pass to function
     * @return values of function
     */
    template <typename R = void, typename... P>
    R call(P&&... args) const
    {
        assert(L);
        return Call<R, P...>::invoke(L, *this, std::forward<P>(args)...);
    }

    /**
     * Call the member function and get return value(s), the member function would
     * expect the first argument as object (table) itself, much like calling function using
     * ':' syntax in Lua.
     * This may raise Lua error or throw LuaException if result or arguments are not convertible.
     *
     * To return multiple values from Lua function, use std::tuple as return type:
     *
     * int a, b;
     * std::tie(a, b) = ref.dispatch<std::tuple<int, int>>("memberFunction", arg1, arg2);
     *
     * The above code works like the following in Lua, please note the ':' syntax:
     *
     * local a, b = ref:memberFunction(arg1, arg2)
     *
     * @param func the name of member function
     * @param args arguments to pass to function
     * @return values of function
     */
    template <typename R = void, typename... P>
    R dispatch(const char* func, P&&... args) const
    {
        assert(L);
        return Call<R, const LuaRef&, P...>::invoke(L, get(func), *this, std::forward<P>(args)...);
    }

    /**
     * Call the member function and get return value(s), much like calling function using
     * '.' syntax in Lua.
     * This may raise Lua error or throw LuaException if result or arguments are not convertible.
     *
     * To return multiple values from Lua function, use std::tuple as return type:
     *
     * int a, b;
     * std::tie(a, b) = ref.dispatchStatic<std::tuple<int, int>>("staticFunction", arg1, arg2);
     *
     * The above code works like the following in Lua, please note the '.' syntax:
     *
     * local a, b = ref.staticFunction(arg1, arg2)
     *
     * @param func the name of static function
     * @param args arguments to pass to function
     * @return values of function
     */
    template <typename R = void, typename... P>
    R dispatchStatic(const char* func, P&&... args) const
    {
        assert(L);
        return Call<R, P...>::invoke(L, get(func), std::forward<P>(args)...);
    }

    /**
     * Get this table's metatable.
     *
     * @return this table's metatable or nil if none
     */
    LuaRef getMetaTable() const;

    /**
     * Set this table's metatable.
     *
     * @param meta new metatable
     */
    void setMetaTable(const LuaRef& meta);

    /**
     * Look up field in table in raw mode (not via metatable).
     * This may raise Lua error or throw LuaException if K or V is not convertible.
     *
     * @param key field key
     * @return field value
     */
    template <typename V = LuaRef, typename K>
    V rawget(const K& key) const
    {
        pushToStack();
        Lua::push(L, key);
        lua_rawget(L, -2);
        V v = Lua::get<V>(L, -1);
        lua_pop(L, 2);
        return v;
    }

    /**
     * Look up field in table in raw mode (not via metatable).
     * This may raise Lua error or throw LuaException if K or V is not convertible.
     *
     * @param key field key
     * @param def default value if the field is missing
     * @return field value
     */
    template <typename V, typename K>
    V rawget(const K& key, const V& def) const
    {
        pushToStack();
        Lua::push(L, key);
        lua_rawget(L, -2);
        V v = Lua::opt<V>(L, -1, def);
        lua_pop(L, 2);
        return v;
    }

    /**
     * Set field in table in raw mode (not via metatable).
     * This may raise Lua error or throw LuaException if K or V is not convertible.
     *
     * @param key field key
     * @param value field value
     */
    template <typename K, typename V>
    void rawset(const K& key, const V& value)
    {
        pushToStack();
        Lua::push(L, key);
        Lua::push(L, value);
        lua_rawset(L, -3);
        lua_pop(L, 1);
    }

    /**
     * Look up field in table in raw mode (not via metatable).
     * This may raise Lua error or throw LuaException if V is not convertible.
     *
     * @param p field key
     * @return field value
     */
    template <typename V = LuaRef>
    V rawgetp(void* p) const
    {
        pushToStack();
        lua_rawgetp(L, -1, p);
        V v = Lua::get<V>(L, -1);
        lua_pop(L, 2);
        return v;
    }

    /**
     * Look up field in table in raw mode (not via metatable).
     * This may raise Lua error or throw LuaException if V is not convertible.
     * This is the same as rawgetp, just for convienence.
     *
     * @param p field key
     * @return field value
     */
    template <typename V = LuaRef>
    V rawget(void* p) const
    {
        return rawgetp<V>(p);
    }

    /**
     * Look up field in table in raw mode (not via metatable).
     * This may raise Lua error or throw LuaException if V is not convertible.
     *
     * @param p field key
     * @param def default value if the field is missing
     * @return field value
     */
    template <typename V>
    V rawgetp(void* p, const V& def) const
    {
        pushToStack();
        lua_rawgetp(L, -1, p);
        V v = Lua::opt<V>(L, -1, def);
        lua_pop(L, 2);
        return v;
    }

    /**
     * Look up field in table in raw mode (not via metatable).
     * This may raise Lua error or throw LuaException if V is not convertible.
     * This is the same as rawgetp, just for convienence.
     *
     * @param p field key
     * @param def default value if the field is missing
     * @return field value
     */
    template <typename V>
    V rawget(void* p, const V& def) const
    {
        return rawgetp(p, def);
    }

    /**
     * Set field in table in raw mode (not via metatable).
     * This may raise Lua error or throw LuaException if V is not convertible.
     *
     * @param p field key
     * @param value field value
     */
    template <typename V>
    void rawsetp(void* p, const V& value)
    {
        pushToStack();
        Lua::push(L, value);
        lua_rawsetp(L, -2, p);
        lua_pop(L, 1);
    }

    /**
     * Set field in table in raw mode (not via metatable).
     * This may raise Lua error or throw LuaException if V is not convertible.
     * This is the same as rawsetp, just for convienence.
     *
     * @param p field key
     * @param value field value
     */
    template <typename V>
    void rawset(void* p, const V& value)
    {
        rawsetp(p, value);
    }

    /**
     * Look up field in table in raw mode (not via metatable).
     * This may raise Lua error or throw LuaException if V is not convertible.
     *
     * @param i field key
     * @return field value
     */
    template <typename V = LuaRef>
    V rawget(int i) const
    {
        pushToStack();
        lua_rawgeti(L, -1, i);
        V v = Lua::get<V>(L, -1);
        lua_pop(L, 2);
        return v;
    }

    /**
     * Look up field in table in raw mode (not via metatable).
     * This may raise Lua error or throw LuaException if V is not convertible.
     *
     * @param i field key
     * @param def default value if the field is missing
     * @return field value
     */
    template <typename V>
    V rawget(int i, const V& def) const
    {
        pushToStack();
        lua_rawgeti(L, -1, i);
        V v = Lua::opt<V>(L, -1, def);
        lua_pop(L, 2);
        return v;
    }

    /**
     * Set field in table in raw mode (not via metatable).
     * This may raise Lua error or throw LuaException if V is not convertible.
     *
     * @param i field key
     * @param value field value
     */
    template <typename V>
    void rawset(int i, const V& value)
    {
        pushToStack();
        Lua::push(L, value);
        lua_rawseti(L, -2, i);
        lua_pop(L, 1);
    }

    /**
     * Get the length of this table (the same as # operator of Lua, but not via metatable).
     */
    int rawlen() const
    {
        pushToStack();
        int n = int(lua_rawlen(L, -1));
        lua_pop(L, 1);
        return n;
    }

    /**
     * Test whether the field is in this table.
     * This may raise Lua error or throw LuaException if K is not convertible.
     *
     * @param key field key
     * @return true if field is available
     */
    template <typename K>
    bool has(const K& key) const
    {
        pushToStack();
        Lua::push(L, key);
        lua_gettable(L, -2);
        bool ok = !lua_isnoneornil(L, -1);
        lua_pop(L, 2);
        return ok;
    }

    /**
     * Look up field in this table.
     * This may raise Lua error or throw LuaException if K or V is not convertible.
     *
     * @param key field key
     * @return field value
     */
    template <typename V = LuaRef, typename K>
    V get(const K& key) const
    {
        pushToStack();
        Lua::push(L, key);
        lua_gettable(L, -2);
        V t = Lua::get<V>(L, -1);
        lua_pop(L, 2);
        return t;
    }

    /**
     * Look up field in this table.
     * This may raise Lua error or throw LuaException if K or V is not convertible.
     *
     * @param key field name
     * @param def default value if the field is missing
     * @return field value
     */
    template <typename V, typename K>
    V get(const K& key, const V& def) const
    {
        pushToStack();
        Lua::push(L, key);
        lua_gettable(L, -2);
        V t = Lua::opt<V>(L, -1, def);
        lua_pop(L, 2);
        return t;
    }

    /**
     * Set field in this table.
     * This may raise Lua error or throw LuaException if K or V is not convertible.
     *
     * @param key field key
     * @param value field value
     */
    template <typename K, typename V>
    void set(const K& key, const V& value)
    {
        pushToStack();
        Lua::push(L, key);
        Lua::push(L, value);
        lua_settable(L, -3);
        lua_pop(L, 1);
    }

    /**
     * Remove field in table.
     * This may raise Lua error or throw LuaException if K is not convertible.
     *
     * @param key field key
     */
    template <typename K>
    void remove(const K& key)
    {
        pushToStack();
        Lua::push(L, key);
        lua_pushnil(L);
        lua_settable(L, -3);
        lua_pop(L, 1);
    }

    /**
     * Get the length of this table (the same as # operator of Lua).
     */
    int len() const
    {
        pushToStack();
        int n = int(luaL_len(L, -1));
        lua_pop(L, 1);
        return n;
    }

    /**
     * Get or set field in this table with bracket-style syntax.
     * This may raise Lua error or throw LuaException if K is not convertible.
     *
     * @param key field key
     * @return assignable and convertible handle for specified key in this table
     */
    template <typename K>
    LuaTableRef operator [] (const K& key)
    {
        assert(L);
        Lua::push(L, key);
        return LuaTableRef(L, m_ref, luaL_ref(L, LUA_REGISTRYINDEX));
    }

    /**
     * Get field in table with bracket-style syntax.
     * This may raise Lua error or throw LuaException if K is not convertible.
     *
     * @param key field key
     * @return convertible handle for specified key in this table
     */
    template <typename K>
    const LuaTableRef operator [] (const K& key) const
    {
        assert(L);
        Lua::push(L, key);
        return LuaTableRef(L, m_ref, luaL_ref(L, LUA_REGISTRYINDEX));
    }

    /**
     * Get the C++ style const iterator.
     */
    LuaTableIterator begin() const
    {
        return LuaTableIterator(L, m_ref, true);
    }

    /**
     * Get the C++ style const iterator.
     */
    LuaTableIterator end() const
    {
        return LuaTableIterator(L, m_ref, false);
    }

private:
    /**
     * Special constructor for popFromStack.
     */
    explicit LuaRef(lua_State* state)
        : L(state)
    {
        assert(L);
        m_ref = luaL_ref(state, LUA_REGISTRYINDEX);
    }

    template <typename T>
    static typename std::enable_if<!std::is_destructible<T>::value || std::is_trivially_destructible<T>::value>::type
        pushUserDataFrom(lua_State* L, const T& cpp_obj)
    {
        void* userdata = lua_newuserdata(L, sizeof(T));
        ::new (userdata) T(cpp_obj);
    }

    template <typename T>
    static typename std::enable_if<std::is_destructible<T>::value && !std::is_trivially_destructible<T>::value>::type
        pushUserDataFrom(lua_State* L, const T& cpp_obj)
    {
        void* userdata = lua_newuserdata(L, sizeof(T));
        ::new (userdata) T(cpp_obj);
        lua_newtable(L);
        lua_pushcfunction(L, &destructUserData<T>);
        lua_setfield(L, -2, "__gc");
        lua_setmetatable(L, -2);
    }

    template <typename T>
    static int destructUserData(lua_State* L)
    {
        try {
            T* obj = static_cast<T*>(lua_touserdata(L, 1));
            obj->~T();
            return 0;
        } catch (std::exception& e) {
            return luaL_error(L, "%s", e.what());
        }
    }

    template <typename R, typename... P>
    struct Call
    {
        static R invoke(lua_State* L, const LuaRef& f, P&&... args)
        {
            lua_pushcfunction(L, &LuaException::traceback);
            f.pushToStack();
            pushArg(L, std::forward<P>(args)...);
            if (lua_pcall(L, sizeof...(P), 1, -int(sizeof...(P) + 2)) != LUA_OK) {
                lua_remove(L, -2);
                throw LuaException(L);
            }
            R v = Lua::get<R>(L, -1);
            lua_pop(L, 2);
            return v;
        }
    };

    template <typename... P>
    struct Call <void, P...>
    {
        static void invoke(lua_State* L, const LuaRef& f, P&&... args)
        {
            lua_pushcfunction(L, &LuaException::traceback);
            f.pushToStack();
            pushArg(L, std::forward<P>(args)...);
            if (lua_pcall(L, sizeof...(P), 0, -int(sizeof...(P) + 2)) != LUA_OK) {
                lua_remove(L, -2);
                throw LuaException(L);
            }
            lua_pop(L, 1);
        }
    };

    template <typename... R, typename... P>
    struct Call <std::tuple<R...>, P...>
    {
        static std::tuple<R...> invoke(lua_State* L, const LuaRef& f, P&&... args)
        {
            lua_pushcfunction(L, &LuaException::traceback);
            f.pushToStack();
            pushArg(L, std::forward<P>(args)...);
            if (lua_pcall(L, sizeof...(P), sizeof...(R), -int(sizeof...(P) + 2)) != LUA_OK) {
                lua_remove(L, -2);
                throw LuaException(L);
            }
            std::tuple<R...> ret;
            TupleResult<sizeof...(R), R...>::fill(L, ret);
            lua_pop(L, int(sizeof...(R) + 1));
            return ret;
        }
    };

    template <size_t N, typename... P>
    struct TupleResult
    {
        static void fill(lua_State* L, std::tuple<P...>& args)
        {
            using T = typename std::tuple_element<sizeof...(P) - N, std::tuple<P...>>::type;
            std::get<sizeof...(P) - N>(args) = Lua::get<T>(L, -int(N));
            TupleResult<N - 1, P...>::fill(L, args);
        }
    };

    template <typename... P>
    struct TupleResult <0, P...>
    {
        static void fill(lua_State*, std::tuple<P...>&)
        {
            // template terminate function
        }
    };

    template <typename P0, typename... P>
    static void pushArg(lua_State* L, P0&& p0, P&&... p)
    {
        Lua::push(L, std::forward<P0>(p0));
        pushArg(L, std::forward<P>(p)...);
    }

    static void pushArg(lua_State*)
    {
        // template terminate function
    }

private:
    lua_State* L;
    int m_ref;
};

//---------------------------------------------------------------------------

template <>
struct LuaTypeMapping <LuaRef>
{
    static void push(lua_State* L, const LuaRef& r)
    {
        if (r.isValid()) {
            r.pushToStack();
        } else {
            lua_pushnil(L);
        }
    }

    static LuaRef get(lua_State* L, int index)
    {
        return lua_isnone(L, index) ? LuaRef() : LuaRef(L, index);
    }

    static LuaRef opt(lua_State* L, int index, const LuaRef& def)
    {
        return lua_isnone(L, index) ? def : LuaRef(L, index);
    }
};

//---------------------------------------------------------------------------

namespace Lua
{
    template <typename LIST>
    inline LIST getList(const LuaRef& table)
    {
        table.pushToStack();
        LIST list = getList<LIST>(table.state(), -1);
        lua_pop(table.state(), 1);
        return list;
    }

    template <typename MAP>
    inline MAP getMap(const LuaRef& table)
    {
        table.pushToStack();
        MAP map = getMap<MAP>(table.state(), -1);
        lua_pop(table.state(), 1);
        return map;
    }
}

/**
 * Create LuaRef from value.
 */
template <typename T>
inline LuaRef LuaRefValue(lua_State* L, T&& v)
{
    Lua::push(L, std::forward<T>(v));
    return LuaRef::popFromStack(L);
}

/**
 * Create LuaRef from nullptr.
 */
inline LuaRef LuaRefValue(lua_State* L, std::nullptr_t)
{
    return LuaRef(L, nullptr);
}

#if LUAINTF_HEADERS_ONLY
#include "src/LuaRef.cpp"
#endif

//---------------------------------------------------------------------------

}

#endif
