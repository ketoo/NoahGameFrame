// Copyright Daniel Wallin 2008. Use, modification and distribution is
// subject to the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef LUABIND_COPY_POLICY_081021_HPP
# define LUABIND_COPY_POLICY_081021_HPP

# include <luabind/detail/policy.hpp>

namespace luabind {

namespace detail
{

  struct copy_converter
  {
      template <class T>
      void apply(lua_State* L, T const& x)
      {
          value_converter().apply(L, x);
      }

      template <class T>
      void apply(lua_State* L, T* x)
      {
          if (!x)
              lua_pushnil(L);
          else
              apply(L, *x);
      }
  };

  template <int N>
  struct copy_policy : conversion_policy<N>
  {
      static void precall(lua_State*, index_map const&)
      {}

      static void postcall(lua_State*, index_map const&)
      {}

      template <class T, class Direction>
      struct apply
      {
          typedef copy_converter type;
      };
  };

} // namespace detail

template <int N>
detail::policy_cons<detail::copy_policy<N>, detail::null_type>
copy(LUABIND_PLACEHOLDER_ARG(N))
{
    return detail::policy_cons<detail::copy_policy<N>, detail::null_type>();
}

} // namespace luabind

#endif // LUABIND_COPY_POLICY_081021_HPP

