// Copyright Daniel Wallin 2008. Use, modification and distribution is
// subject to the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef LUABIND_FORMAT_SIGNATURE_081014_HPP
# define LUABIND_FORMAT_SIGNATURE_081014_HPP

# include <luabind/config.hpp>
# include <luabind/lua_include.hpp>
# include <luabind/typeid.hpp>

# include <boost/mpl/begin_end.hpp>
# include <boost/mpl/next.hpp>
# include <boost/mpl/size.hpp>

namespace luabind {

class object;
class argument;
template <class Base>
struct table;

} // namespace luabind

namespace luabind { namespace detail {

LUABIND_API std::string get_class_name(lua_State* L, type_id const& i);

template <class T>
struct type_to_string
{
    static void get(lua_State* L)
    {
        lua_pushstring(L, get_class_name(L, typeid(T)).c_str());
    }
};

template <class T>
struct type_to_string<T*>
{
    static void get(lua_State* L)
    {
        type_to_string<T>::get(L);
        lua_pushstring(L, "*");
        lua_concat(L, 2);
    }
};

template <class T>
struct type_to_string<T&>
{
    static void get(lua_State* L)
    {
        type_to_string<T>::get(L);
        lua_pushstring(L, "&");
        lua_concat(L, 2);
    }
};

template <class T>
struct type_to_string<T const>
{
    static void get(lua_State* L)
    {
        type_to_string<T>::get(L);
        lua_pushstring(L, " const");
        lua_concat(L, 2);
    }
};

# define LUABIND_TYPE_TO_STRING(x) \
    template <> \
    struct type_to_string<x> \
    { \
        static void get(lua_State* L) \
        { \
            lua_pushstring(L, #x); \
        } \
    };

# define LUABIND_INTEGRAL_TYPE_TO_STRING(x) \
    LUABIND_TYPE_TO_STRING(x) \
    LUABIND_TYPE_TO_STRING(unsigned x)

LUABIND_INTEGRAL_TYPE_TO_STRING(char)
LUABIND_INTEGRAL_TYPE_TO_STRING(short)
LUABIND_INTEGRAL_TYPE_TO_STRING(int)
LUABIND_INTEGRAL_TYPE_TO_STRING(long)

LUABIND_TYPE_TO_STRING(void)
LUABIND_TYPE_TO_STRING(bool)
LUABIND_TYPE_TO_STRING(std::string)
LUABIND_TYPE_TO_STRING(lua_State)

LUABIND_TYPE_TO_STRING(luabind::object)
LUABIND_TYPE_TO_STRING(luabind::argument)

# undef LUABIND_INTEGRAL_TYPE_TO_STRING
# undef LUABIND_TYPE_TO_STRING

template <class Base>
struct type_to_string<table<Base> >
{
    static void get(lua_State* L)
    {
        lua_pushstring(L, "table");
    }
};

template <class End>
void format_signature_aux(lua_State*, bool, End, End)
{}

template <class Iter, class End>
void format_signature_aux(lua_State* L, bool first, Iter, End end)
{
    if (!first)
        lua_pushstring(L, ",");
    type_to_string<typename Iter::type>::get(L);
    format_signature_aux(L, false, typename mpl::next<Iter>::type(), end);
}

template <class Signature>
void format_signature(lua_State* L, char const* function, Signature)
{
    typedef typename mpl::begin<Signature>::type first;

    type_to_string<typename first::type>::get(L);

    lua_pushstring(L, " ");
    lua_pushstring(L, function);

    lua_pushstring(L, "(");
    format_signature_aux(
        L
      , true
      , typename mpl::next<first>::type()
      , typename mpl::end<Signature>::type()
    );
    lua_pushstring(L, ")");

    lua_concat(L, static_cast<int>(mpl::size<Signature>()) * 2 + 2);
}

}} // namespace luabind::detail

#endif // LUABIND_FORMAT_SIGNATURE_081014_HPP

