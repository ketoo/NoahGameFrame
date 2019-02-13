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

enum class LuaTypeID
{
    NONE = LUA_TNONE,
    NIL = LUA_TNIL,
    STRING = LUA_TSTRING,
    NUMBER = LUA_TNUMBER,
    THREAD = LUA_TTHREAD,
    BOOLEAN = LUA_TBOOLEAN,
    FUNCTION = LUA_TFUNCTION,
    TABLE = LUA_TTABLE,
    USERDATA = LUA_TUSERDATA,
    LIGHTUSERDATA = LUA_TLIGHTUSERDATA
};

template <typename T, typename ENABLED = void>
struct LuaTypeMapping;

template <typename T, bool IS_CONST, bool IS_REF>
struct LuaClassMapping;

//---------------------------------------------------------------------------

struct LuaTypeExists
{
    template <typename T, typename = decltype(T())>
    static std::true_type test(int);

    template <typename>
    static std::false_type test(...);
};

template <typename T>
struct LuaTypeMappingExists
{
    using Type = decltype(LuaTypeExists::test<LuaTypeMapping<T>>(0));
    static constexpr bool value = Type::value;
};

template <typename T>
struct LuaType
    : std::conditional<
        std::is_class<typename std::decay<T>::type>::value
            && !LuaTypeMappingExists<typename std::decay<T>::type>::value,
        LuaClassMapping<typename std::decay<T>::type,
            std::is_const<typename std::remove_reference<T>::type>::value,
            std::is_reference<T>::value>,
        LuaTypeMapping<typename std::decay<T>::type>
    >::type {};

//---------------------------------------------------------------------------

template <>
struct LuaTypeMapping <bool>
{
    static void push(lua_State* L, bool value)
    {
        lua_pushboolean(L, value);
    }

    static bool get(lua_State* L, int index)
    {
        return lua_toboolean(L, index) != 0;
    }

    static bool opt(lua_State* L, int index, bool def)
    {
        return lua_isnone(L, index) ? def : lua_toboolean(L, index) != 0;
    }
};

//---------------------------------------------------------------------------

template <typename T>
struct LuaIntegerTypeMapping
{
    static void push(lua_State* L, T value)
    {
        lua_pushinteger(L, static_cast<lua_Integer>(value));
    }

    static T get(lua_State* L, int index)
    {
        return static_cast<T>(luaL_checkinteger(L, index));
    }

    static T opt(lua_State* L, int index, T def)
    {
        return static_cast<T>(luaL_optinteger(L, index, static_cast<lua_Integer>(def)));
    }
};

template <>
struct LuaTypeMapping <signed char>
    : LuaIntegerTypeMapping <signed char> {};

template <>
struct LuaTypeMapping <short>
    : LuaIntegerTypeMapping <short> {};

template <>
struct LuaTypeMapping <int>
    : LuaIntegerTypeMapping <int> {};

template <>
struct LuaTypeMapping <long>
    : LuaIntegerTypeMapping <long> {};

//---------------------------------------------------------------------------

#if LUA_VERSION_NUM <= 502

template <typename T>
struct LuaUnsignedTypeMapping
{
    static void push(lua_State* L, T value)
    {
        lua_pushunsigned(L, static_cast<lua_Unsigned>(value));
    }

    static T get(lua_State* L, int index)
    {
        return static_cast<T>(luaL_checkunsigned(L, index));
    }

    static T opt(lua_State* L, int index, T def)
    {
        return static_cast<T>(luaL_optunsigned(L, index, static_cast<lua_Unsigned>(def)));
    }
};

#else

template <typename T>
using LuaUnsignedTypeMapping = LuaIntegerTypeMapping<T>;

#endif

template <>
struct LuaTypeMapping <unsigned char>
    : LuaUnsignedTypeMapping <unsigned char> {};

template <>
struct LuaTypeMapping <unsigned short>
    : LuaUnsignedTypeMapping <unsigned short> {};

template <>
struct LuaTypeMapping <unsigned int>
    : LuaUnsignedTypeMapping <unsigned int> {};

template <>
struct LuaTypeMapping <unsigned long>
    : LuaUnsignedTypeMapping <unsigned long> {};

//---------------------------------------------------------------------------

template <typename T>
struct LuaNumberTypeMapping
{
    static void push(lua_State* L, T value)
    {
        lua_pushnumber(L, static_cast<lua_Number>(value));
    }

    static T get(lua_State* L, int index)
    {
        return static_cast<T>(luaL_checknumber(L, index));
    }

    static T opt(lua_State* L, int index, T def)
    {
        return static_cast<T>(luaL_optnumber(L, index, static_cast<lua_Number>(def)));
    }
};

template <>
struct LuaTypeMapping <float>
    : LuaNumberTypeMapping <float> {};

template <>
struct LuaTypeMapping <double>
    : LuaNumberTypeMapping <double> {};

template <>
struct LuaTypeMapping <long double>
    : LuaNumberTypeMapping <long double> {};

//---------------------------------------------------------------------------

#if LUAINTF_UNSAFE_INT64 && (LUA_VERSION_NUM <= 502 || defined(LUA_32BITS))

template <typename T>
struct LuaUnsafeInt64TypeMapping
{
    static void push(lua_State* L, T value)
    {
        lua_Number f = static_cast<lua_Number>(value);
#if LUAINTF_UNSAFE_INT64_CHECK
        T verify = static_cast<T>(f);
        if (value != verify) {
            luaL_error(L, "unsafe cast from 64-bit int");
        }
#endif
        lua_pushnumber(L, f);
    }

    static T get(lua_State* L, int index)
    {
        return static_cast<T>(luaL_checknumber(L, index));
    }

    static T opt(lua_State* L, int index, T def)
    {
        return lua_isnoneornil(L, index) ? def : static_cast<T>(luaL_checknumber(L, index));
    }
};

template <>
struct LuaTypeMapping <long long>
    : LuaUnsafeInt64TypeMapping <long long> {};

template <>
struct LuaTypeMapping <unsigned long long>
    : LuaUnsafeInt64TypeMapping <unsigned long long> {};

#elif LUA_VERSION_NUM >= 503

template <>
struct LuaTypeMapping <long long>
    : LuaIntegerTypeMapping <long long> {};

template <>
struct LuaTypeMapping <unsigned long long>
    : LuaIntegerTypeMapping <unsigned long long> {};

#endif

//---------------------------------------------------------------------------

template <>
struct LuaTypeMapping <char>
{
    static void push(lua_State* L, char value)
    {
        char str[] = { value, 0 };
        lua_pushstring(L, str);
    }

    static char get(lua_State* L, int index)
    {
        return luaL_checkstring(L, index)[0];
    }

    static char opt(lua_State* L, int index, char def)
    {
        return lua_isnoneornil(L, index) ? def : get(L, index);
    }
};

//---------------------------------------------------------------------------

template <>
struct LuaTypeMapping <const char*>
{
    static void push(lua_State* L, const char* str)
    {
        lua_pushstring(L, str);
    }

    static const char* get(lua_State* L, int index)
    {
        return luaL_checkstring(L, index);
    }

    static const char* opt(lua_State* L, int index, const char* def)
    {
        return luaL_optstring(L, index, def);
    }
};

//---------------------------------------------------------------------------

template <>
struct LuaTypeMapping <char*>
{
    static void push(lua_State* L, const char* str)
    {
        lua_pushstring(L, str);
    }
};

//---------------------------------------------------------------------------

template <>
struct LuaTypeMapping <std::string>
{
    static void push(lua_State* L, const std::string& str)
    {
        lua_pushlstring(L, str.data(), str.length());
    }

    static std::string get(lua_State* L, int index)
    {
        size_t len;
        const char* p = luaL_checklstring(L, index, &len);
        return std::string(p, len);
    }

    static std::string opt(lua_State* L, int index, const std::string& def)
    {
        return lua_isnoneornil(L, index) ? def : get(L, index);
    }
};

//---------------------------------------------------------------------------

#if LUAINTF_STD_WIDE_STRING

template <typename CH>
struct LuaTypeMapping <std::basic_string<CH>>
{
    using WString = std::basic_string<CH>;
    using WStringConvert = std::wstring_convert<std::codecvt_utf8<CH>, CH>;

    static void push(lua_State* L, const WString& str)
    {
        if (str.empty()) {
            lua_pushliteral(L, "");
        } else {
            WStringConvert conv;
            std::string buf = conv.to_bytes(str);
            lua_pushlstring(L, buf.data(), buf.length());
        }
    }

    static WString get(lua_State* L, int index)
    {
        size_t len;
        const char* p = luaL_checklstring(L, index, &len);
        WStringConvert conv;
        return conv.from_bytes(p, p + len);
    }

    static WString opt(lua_State* L, int index, const WString& def)
    {
        return lua_isnoneornil(L, index) ? def : get(L, index);
    }
};

#endif

//---------------------------------------------------------------------------

/**
 * Transitient string type without copying underlying char values, use with caution.
 * It works like const char* with length field.
 */
struct LuaString
{
    constexpr LuaString()
        : data(nullptr)
        , size(0)
        {}

    LuaString(const std::string& str)
        : data(str.data())
        , size(str.size())
        {}

    LuaString(const char* str)
        : data(str)
        , size(std::strlen(str))
        {}

    LuaString(const char* str, size_t len)
        : data(str)
        , size(len)
        {}

    LuaString(lua_State* L, int index)
    {
        data = luaL_checklstring(L, index, &size);
    }

    explicit operator bool () const
    {
        return data != nullptr;
    }

    const char* data;
    size_t size;
};

template <>
struct LuaTypeMapping <LuaString>
{
    static void push(lua_State* L, const LuaString& str)
    {
        if (str.data) {
            lua_pushlstring(L, str.data, str.size);
        } else {
            lua_pushnil(L);
        }
    }

    static LuaString get(lua_State* L, int index)
    {
        return LuaString(L, index);
    }

    static LuaString opt(lua_State* L, int index, const LuaString& def)
    {
        return lua_isnoneornil(L, index) ? def : LuaString(L, index);
    }
};

//---------------------------------------------------------------------------

/**
 * Default type mapping to catch all enum conversion
 */
#if LUA_VERSION_NUM <= 502

template <typename T>
struct LuaTypeMapping <T, typename std::enable_if<std::is_enum<T>::value>::type>
    : std::conditional<
        std::is_unsigned<typename std::underlying_type<T>::type>::value,
        LuaUnsignedTypeMapping<T>,
        LuaIntegerTypeMapping<T>
    >::type {};

#else

template <typename T>
struct LuaTypeMapping <T, typename std::enable_if<std::is_enum<T>::value>::type>
    : LuaIntegerTypeMapping<T> {};

#endif

//---------------------------------------------------------------------------

/**
 * Template for list container type
 */
#define LUA_USING_LIST_TYPE_X(LIST, ...) \
    template <__VA_ARGS__> \
    struct LuaTypeMapping <LIST> \
    { \
        static void push(lua_State* L, const LIST& v) \
        { \
            if (v.empty()) { \
                lua_newtable(L); \
            } else { \
                Lua::pushList(L, v); \
            } \
        } \
    \
        static LIST get(lua_State* L, int index) \
        { \
            return lua_isnoneornil(L, index) ? LIST() : Lua::getList<LIST>(L, index); \
        } \
    \
        static LIST opt(lua_State* L, int index, const LIST& def) \
        { \
            return lua_isnoneornil(L, index) ? def : Lua::getList<LIST>(L, index); \
        } \
    };

#define LUA_USING_LIST_TYPE(LIST) \
    LUA_USING_LIST_TYPE_X(LIST<T>, typename T)

//---------------------------------------------------------------------------

/**
 * Template for map container type
 */
#define LUA_USING_MAP_TYPE_X(MAP, ...) \
    template <__VA_ARGS__> \
    struct LuaTypeMapping <MAP> \
    { \
        static void push(lua_State* L, const MAP& v) \
        { \
            if (v.empty()) { \
                lua_newtable(L); \
            } else { \
                Lua::pushMap(L, v); \
            } \
        } \
    \
        static MAP get(lua_State* L, int index) \
        { \
            return lua_isnoneornil(L, index) ? MAP() : Lua::getMap<MAP>(L, index); \
        } \
    \
        static MAP opt(lua_State* L, int index, const MAP& def) \
        { \
            return lua_isnoneornil(L, index) ? def : Lua::getMap<MAP>(L, index); \
        } \
    };

#define LUA_COMMA ,

#define LUA_USING_MAP_TYPE(MAP) \
    LUA_USING_MAP_TYPE_X(MAP<K LUA_COMMA V>, typename K, typename V)
