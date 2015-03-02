// Copyright Daniel Wallin 2005. Use, modification and distribution is
// subject to the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef LUABIND_EXCEPTION_HANDLER_050601_HPP
# define LUABIND_EXCEPTION_HANDLER_050601_HPP

# include <luabind/lua_include.hpp>
# include <luabind/config.hpp>
# include <boost/optional.hpp>
# include <boost/type.hpp>

# if BOOST_WORKAROUND(BOOST_MSVC, <= 1300)
#  include <boost/mpl/if.hpp>
#  include <boost/type_traits/is_pointer.hpp>
# endif

namespace luabind {

# ifndef LUABIND_NO_EXCEPTIONS

namespace detail
{

  struct LUABIND_API exception_handler_base
  {
      exception_handler_base()
        : next(0)
      {}

      virtual ~exception_handler_base() {}
      virtual void handle(lua_State*) const = 0;

      void try_next(lua_State*) const;

      exception_handler_base* next;
  };

  namespace mpl = boost::mpl;

  template<class E, class Handler>
  struct exception_handler : exception_handler_base
  {
#  if BOOST_WORKAROUND(BOOST_MSVC, <= 1300)
      typedef typename mpl::if_<
          boost::is_pointer<E>, E, E const&
      >::type argument;
#  else
      typedef E const& argument;
#  endif

      exception_handler(Handler handler)
        : handler(handler)
      {}

      void handle(lua_State* L) const
      {
          try
          {
              try_next(L);
          }
          catch (argument e)
          {
              handler(L, e);
          }
      }

      Handler handler;
  };

  LUABIND_API void handle_exception_aux(lua_State* L);
  LUABIND_API void register_exception_handler(exception_handler_base*);

} // namespace detail

# endif

template<class E, class Handler>
void register_exception_handler(Handler handler, boost::type<E>* = 0)
{
# ifndef LUABIND_NO_EXCEPTIONS
    detail::register_exception_handler(
        new detail::exception_handler<E, Handler>(handler)
    );
# endif
}

template<class R, class F>
boost::optional<R> handle_exceptions(lua_State* L, F fn, boost::type<R>* = 0)
{
# ifndef LUABIND_NO_EXCEPTIONS
    try
    {
        return fn();
    }
    catch (...)
    {
        detail::handle_exception_aux(L);
    }

    return boost::optional<R>();
# else
    return fn();
# endif
}

} // namespace luabind

#endif // LUABIND_EXCEPTION_HANDLER_050601_HPP

