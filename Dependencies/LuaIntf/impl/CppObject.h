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

template <typename T, int KIND = 0>
struct CppSignature
{
    /**
     * Get the signature id for type
     *
     * The id is unique in the process
     */
    static void* value()
    {
        static bool v = false;
        return &v;
    }
};

template <typename T>
using CppClassSignature = CppSignature<T, 1>;

template <typename T>
using CppConstSignature = CppSignature<T, 2>;

//--------------------------------------------------------------------------

/**
 * Because of Lua's dynamic typing and our improvised system of imposing C++
 * class structure, there is the possibility that executing scripts may
 * knowingly or unknowingly cause invalid data to get passed to the C functions
 * created by LuaIntf. To improve type security, we have the following policy:
 *
 *     1. Scripts cannot set the metatable on a userdata
 *     2. CppObject's metatable has been tagged by typeid
 *     3. CppObject's typeid is a unique pointer in the process.
 *     4. Access to CppObject class method will check for typeid, and raise error if not matched
 */
class CppObject
{
protected:
    CppObject() {}

    template <typename OBJ>
    static void* allocate(lua_State* L, void* class_id)
    {
        void* mem = lua_newuserdata(L, sizeof(OBJ));
        lua_rawgetp(L, LUA_REGISTRYINDEX, class_id);
        luaL_checktype(L, -1, LUA_TTABLE);
        lua_setmetatable(L, -2);
        return mem;
    }

public:
    virtual ~CppObject() {}

    CppObject(const CppObject&) = delete;
    CppObject& operator = (const CppObject&) = delete;

    /**
     * Whether the object is shared pointer
     */
    virtual bool isSharedPtr() const
    {
        return false;
    }

    /**
     * The object pointer
     */
    virtual void* objectPtr() = 0;

    /**
     * Get internal class id of the given class
     */
    template <typename T>
    static void* getClassID(bool is_const)
    {
        return is_const ? CppConstSignature<T>::value() : CppClassSignature<T>::value();
    }

    /**
     * Returns the CppObject* if the class on the Lua stack is exact the same class (not one of the subclass).
     * If the class does not match, a Lua error is raised.
     */
    template <typename T>
    static CppObject* getExactObject(lua_State* L, int index, bool is_const)
    {
        return getObject(L, index, getClassID<T>(is_const), is_const, true, true);
    }

    /**
     * Returns the CppObject* if the object on the Lua stack is an instance of the given class.
     * If the object is not the class or a subclass, a Lua error is raised.
     */
    template <typename T>
    static CppObject* getObject(lua_State* L, int index, bool is_const)
    {
        return getObject(L, index, getClassID<T>(is_const), is_const, false, true);
    }

    /**
     * Get a pointer to the class from the Lua stack.
     *
     * If the object is not the class or a subclass, return nullptr.
     */
    template <typename T>
    static T* cast(lua_State* L, int index, bool is_const)
    {
        CppObject* object = getObject(L, index, getClassID<T>(is_const), is_const, false, false);
        return object ? static_cast<T*>(object->objectPtr()) : nullptr;
    }

    /**
     * Get a pointer to the class from the Lua stack.
     *
     * If the object is not the class or a subclass, a Lua error is raised.
     */
    template <typename T>
    static T* get(lua_State* L, int index, bool is_const)
    {
        return static_cast<T*>(getObject<T>(L, index, is_const)->objectPtr());
    }

private:
    static void typeMismatchError(lua_State* L, int index);
    static CppObject* getObject(lua_State* L, int index, void* class_id,
        bool is_const, bool is_exact, bool raise_error);
};

//----------------------------------------------------------------------------

class CppAutoDowncast
{
#if LUAINTF_AUTO_DOWNCAST

private:
    template <typename T, typename SUPER, bool IS_CONST>
    static void* tryDowncast(SUPER* obj) {
        void* casted = dynamic_cast<T*>(obj);
        if (casted == obj) {
            return CppObject::getClassID<T>(IS_CONST);
        } else {
            return nullptr;
        }
    }

    template <typename T, typename SUPER, bool IS_CONST>
    static void addDowncast(LuaRef super) {
        lua_State* L = super.state();
        LuaRef downcast = LuaRef::createUserDataFrom(L, &tryDowncast<T, SUPER, IS_CONST>);

        void* class_id = CppObject::getClassID<SUPER>(IS_CONST);
        bool* class_may_downcast = static_cast<bool*>(class_id);
        *class_may_downcast = true;

        LuaRef list = super.rawget("__downcast");
        if (list == nullptr) {
            list = LuaRef::createTable(L);
            super.rawset("__downcast", list);
        }
        list.rawset(list.rawlen() + 1, downcast);
    }

    static void* findClassID(lua_State* L, void* obj, void* class_id) {
        bool class_may_downcast = *static_cast<bool*>(class_id);
        if (!class_may_downcast) return class_id;

        // <class_meta>
        lua_rawgetp(L, LUA_REGISTRYINDEX, class_id);
        luaL_checktype(L, -1, LUA_TTABLE);

        // <class_meta> <downcast>
        lua_pushliteral(L, "__downcast");
        lua_rawget(L, -2);

        if (!lua_isnil(L, -1)) {
            int len = int(lua_rawlen(L, -1));
            for (int i = 1; i <= len; i++) {
                // <class_meta> <downcast> <cast_func>
                lua_rawgeti(L, -1, i);
                auto downcast = *reinterpret_cast<void*(**)(void*)>(lua_touserdata(L, -1));

                void* cast_class_id = downcast(obj);
                if (cast_class_id) {
                    lua_pop(L, 3);
                    return findClassID(L, obj, cast_class_id);
                }

                lua_pop(L, 1);
            }
        }

        lua_pop(L, 2);
        return class_id;
    }

public:
    template <typename T, typename SUPER>
    static void add(lua_State* L) {
        LuaRef registry(L, LUA_REGISTRYINDEX);
        LuaRef super = registry.rawgetp(CppSignature<SUPER>::value());
        addDowncast<T, SUPER, false>(super.rawget("___class"));
        addDowncast<T, SUPER, true>(super.rawget("___const"));
    }

    template <typename T>
    static void* getClassID(lua_State* L, T* obj, bool is_const) {
        void* class_id = CppObject::getClassID<T>(is_const);
        return findClassID(L, obj, class_id);
    }

#else

public:
    template <typename T>
    static void* getClassID(lua_State*, T*, bool is_const) {
        return CppObject::getClassID<T>(is_const);
    }

#endif
};

//----------------------------------------------------------------------------

/**
 * Wraps a class object stored in a Lua userdata.
 *
 * The lifetime of the object is managed by Lua. The object is constructed
 * inside the userdata using placement new.
 */
template <typename T>
class CppObjectValue : public CppObject
{
private:
    CppObjectValue()
    {
        if (MAX_PADDING > 0) {
            uintptr_t offset = reinterpret_cast<uintptr_t>(&m_data[0]) % alignof(T);
            if (offset > 0) offset = alignof(T) - offset;
            assert(offset < MAX_PADDING);
            m_data[sizeof(m_data) - 1] = static_cast<unsigned char>(offset);
        }
    }

public:
    virtual ~CppObjectValue()
    {
        T* obj = static_cast<T*>(objectPtr());
        obj->~T();
    }

    virtual void* objectPtr() override
    {
        if (MAX_PADDING == 0) {
            return &m_data[0];
        } else {
            return &m_data[0] + m_data[sizeof(m_data) - 1];
        }
    }

    template <typename... P>
    static void pushToStack(lua_State* L, bool is_const, P&&... args)
    {
        void* mem = allocate<CppObjectValue<T>>(L, getClassID<T>(is_const));
        CppObjectValue<T>* v = ::new (mem) CppObjectValue<T>();
        ::new (v->objectPtr()) T(std::forward<P>(args)...);
    }

    template <typename... P>
    static void pushToStack(lua_State* L, std::tuple<P...>& args, bool is_const)
    {
        void* mem = allocate<CppObjectValue<T>>(L, getClassID<T>(is_const));
        CppObjectValue<T>* v = ::new (mem) CppObjectValue<T>();
        CppInvokeClassConstructor<T>::call(v->objectPtr(), args);
    }

    static void pushToStack(lua_State* L, const T& obj, bool is_const)
    {
        void* mem = allocate<CppObjectValue<T>>(L, getClassID<T>(is_const));
        CppObjectValue<T>* v = ::new (mem) CppObjectValue<T>();
        ::new (v->objectPtr()) T(obj);
    }

private:
    using AlignType = typename std::conditional<alignof(T) <= alignof(double), T, void*>::type;
    static constexpr int MAX_PADDING = alignof(T) <= alignof(AlignType) ? 0 : alignof(T) - alignof(AlignType) + 1;
    alignas(AlignType) unsigned char m_data[sizeof(T) + MAX_PADDING];
};

//----------------------------------------------------------------------------

/**
 * Wraps a pointer to a class object inside a Lua userdata.
 *
 * The lifetime of the object is managed by C++.
 */
class CppObjectPtr : public CppObject
{
private:
    explicit CppObjectPtr(void* obj)
        : m_ptr(obj)
    {
        assert(obj != nullptr);
    }

public:
    virtual void* objectPtr() override
    {
        return m_ptr;
    }

    template <typename T>
    static void pushToStack(lua_State* L, T* obj, bool is_const)
    {
        void* mem = allocate<CppObjectPtr>(L, CppAutoDowncast::getClassID(L, obj, is_const));
        ::new (mem) CppObjectPtr(obj);
    }

private:
    void* m_ptr;
};

//----------------------------------------------------------------------------

/**
 * Wraps a shared ptr that references a class object.
 *
 * The template argument SP is the smart pointer type
 */
template <typename SP, typename T>
class CppObjectSharedPtr : public CppObject
{
private:
    explicit CppObjectSharedPtr(T* obj)
        : m_sp(obj)
        {}

    explicit CppObjectSharedPtr(const SP& sp)
        : m_sp(sp)
        {}

public:
    virtual bool isSharedPtr() const override
    {
        return true;
    }

    virtual void* objectPtr() override
    {
        return const_cast<T*>(&*m_sp);
    }

    SP& sharedPtr()
    {
        return m_sp;
    }

    static void pushToStack(lua_State* L, T* obj, bool is_const)
    {
        void* mem = allocate<CppObjectSharedPtr<SP, T>>(L,
            CppAutoDowncast::getClassID(L, obj, is_const));
        ::new (mem) CppObjectSharedPtr<SP, T>(obj);
    }

    static void pushToStack(lua_State* L, const SP& sp, bool is_const)
    {
        void* mem = allocate<CppObjectSharedPtr<SP, T>>(L,
            CppAutoDowncast::getClassID(L, const_cast<T*>(&*sp), is_const));
        ::new (mem) CppObjectSharedPtr<SP, T>(sp);
    }

private:
    SP m_sp;
};

//----------------------------------------------------------------------------

template <typename T>
struct CppObjectTraits
{
    using ObjectType = T;

    static constexpr bool isSharedPtr = false;
    static constexpr bool isSharedConst = false;
};

#define LUA_USING_SHARED_PTR_TYPE(SP) \
    template <typename T> \
    struct CppObjectTraits <SP<T>> \
    { \
        using ObjectType = typename std::remove_cv<T>::type; \
        \
        static constexpr bool isSharedPtr = true; \
        static constexpr bool isSharedConst = std::is_const<T>::value; \
    };

//---------------------------------------------------------------------------

template <typename SP, typename OBJ, bool IS_SHARED, bool IS_REF>
struct LuaCppObjectFactory;

template <typename T>
struct LuaCppObjectFactory <T, T, false, false>
{
    static void push(lua_State* L, const T& obj, bool is_const)
    {
        CppObjectValue<T>::pushToStack(L, obj, is_const);
    }

    static T& cast(lua_State*, CppObject* obj)
    {
        return *static_cast<T*>(obj->objectPtr());
    }
};

template <typename T>
struct LuaCppObjectFactory <T, T, false, true>
{
    static void push(lua_State* L, const T& obj, bool is_const)
    {
        CppObjectPtr::pushToStack(L, const_cast<T*>(&obj), is_const);
    }

    static T& cast(lua_State*, CppObject* obj)
    {
        return *static_cast<T*>(obj->objectPtr());
    }
};

template <typename SP, typename T>
struct LuaCppObjectFactory <SP, T, true, true>
{
    static void push(lua_State* L, const SP& sp, bool is_const)
    {
        if (!sp) {
            lua_pushnil(L);
        } else {
            CppObjectSharedPtr<SP, T>::pushToStack(L, sp, is_const);
        }
    }

    static SP& cast(lua_State* L, CppObject* obj)
    {
        if (!obj->isSharedPtr()) {
            luaL_error(L, "is not shared object");
        }
        return static_cast<CppObjectSharedPtr<SP, T>*>(obj)->sharedPtr();
    }
};

//---------------------------------------------------------------------------

/**
 * Lua conversion for reference or value to the class type,
 * this will catch all C++ class unless LuaTypeMapping<> exists.
 */
template <typename T, bool IS_CONST, bool IS_REF>
struct LuaClassMapping
{
    using ObjectType = typename CppObjectTraits<T>::ObjectType;

    static constexpr bool isShared = CppObjectTraits<T>::isSharedPtr;
    static constexpr bool isRef = isShared ? true : IS_REF;
    static constexpr bool isConst = isShared ? CppObjectTraits<T>::isSharedConst : IS_CONST;

    static void push(lua_State* L, const T& t)
    {
        LuaCppObjectFactory<T, ObjectType, isShared, isRef>::push(L, t, isConst);
    }

    static T& get(lua_State* L, int index)
    {
        CppObject* obj = CppObject::getObject<ObjectType>(L, index, isConst);
        return LuaCppObjectFactory<T, ObjectType, isShared, isRef>::cast(L, obj);
    }

    static const T& opt(lua_State* L, int index, const T& def)
    {
        if (lua_isnoneornil(L, index)) {
            return def;
        } else {
            return get(L, index);
        }
    }
};

//----------------------------------------------------------------------------

/**
 * Lua conversion for pointer to the class type
 */
template <typename T>
struct LuaTypeMapping <T*>
{
    using Type = typename std::decay<T>::type;
    using PtrType = T*;

    static constexpr bool isConst = std::is_const<T>::value;

    static void push(lua_State* L, const Type* p)
    {
        if (p == nullptr) {
            lua_pushnil(L);
        } else {
            CppObjectPtr::pushToStack(L, const_cast<Type*>(p), isConst);
        }
    }

    static PtrType get(lua_State* L, int index)
    {
        return CppObject::get<Type>(L, index, isConst);
    }

    static PtrType opt(lua_State* L, int index, PtrType def)
    {
        if (lua_isnoneornil(L, index)) {
            return def;
        } else {
            return CppObject::get<Type>(L, index, isConst);
        }
    }
};

//----------------------------------------------------------------------------

namespace Lua
{
    /**
     * Create new object inside userdata directly and push onto Lua stack.
     */
    template <typename T, typename... P>
    inline void pushNew(lua_State* L, P&&... args)
    {
        CppObjectValue<typename std::remove_cv<T>::type>::pushToStack(L, std::is_const<T>::value, std::forward<P>(args)...);
    }

    /**
     * Cast stack value at the index to the given class, return nullptr if it is not.
     */
    template <typename T>
    inline T* objectCast(lua_State* L, int index)
    {
        return CppObject::cast<typename std::remove_cv<T>::type>(L, index, std::is_const<T>::value);
    }

    /**
     * Cast LuaRef to the given class, return nullptr if it is not.
     */
    template <typename T>
    inline T* objectCast(const LuaRef& ref)
    {
        ref.pushToStack();
        T* object = objectCast<T>(ref.state(), -1);
        lua_pop(ref.state(), 1);
        return object;
    }
}

/**
 * Create LuaRef for cpp class, the object is created inside userdata directly.
 */
template <typename T, typename... P>
inline LuaRef LuaRefObject(lua_State* L, P&&... args)
{
    Lua::pushNew<T>(L, std::forward<P>(args)...);
    return LuaRef::popFromStack(L);
}
