// Copyright Daniel Wallin 2005. Use, modification and distribution is
// subject to the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#define LUABIND_BUILDING

#include <luabind/config.hpp>
#include <luabind/exception_handler.hpp>
#include <luabind/error.hpp>
#include <stdexcept>

#ifndef LUABIND_NO_EXCEPTIONS

namespace luabind { namespace detail {

namespace
{
  exception_handler_base* handler_chain = 0;

  void push_exception_string(lua_State* L, char const* exception, char const* what)
  {
      lua_pushstring(L, exception);
      lua_pushstring(L, ": '");
      lua_pushstring(L, what);
      lua_pushstring(L, "'");
      lua_concat(L, 4);
  }
}

void exception_handler_base::try_next(lua_State* L) const
{
    if (next)
        next->handle(L);
    else
        throw;
}

LUABIND_API void handle_exception_aux(lua_State* L)
{
    try
    {
        if (handler_chain)
            handler_chain->handle(L);
        else
            throw;
    }
    catch (error const&)
    {}
    catch (std::logic_error const& e)
    {
        push_exception_string(L, "std::logic_error", e.what());
    }
    catch (std::runtime_error const& e)
    {
        push_exception_string(L, "std::runtime_error", e.what());
    }
    catch (std::exception const& e)
    {
        push_exception_string(L, "std::exception", e.what());
    }
    catch (char const* str)
    {
        push_exception_string(L, "c-string", str);
    }
    catch (...)
    {
        lua_pushstring(L, "Unknown C++ exception");
    }
}

LUABIND_API void register_exception_handler(exception_handler_base* handler)
{
    if (!handler_chain) handler_chain = handler;
    else
    {
        exception_handler_base* p = handler_chain;

        for (; p->next; p = p->next);

        handler->next = 0;
        p->next = handler;
    }
}

}} // namespace luabind::detail

#endif // LUABIND_NO_EXCEPTIONS
