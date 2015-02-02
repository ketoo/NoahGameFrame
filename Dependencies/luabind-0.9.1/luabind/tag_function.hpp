// Copyright Daniel Wallin 2008. Use, modification and distribution is
// subject to the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if !BOOST_PP_IS_ITERATING

# ifndef LUABIND_TAG_FUNCTION_081129_HPP
#  define LUABIND_TAG_FUNCTION_081129_HPP

#  if LUABIND_MAX_ARITY <= 8
#   include <boost/mpl/vector/vector10.hpp>
#  else
#   include <boost/mpl/vector/vector50.hpp>
#  endif
#  include <boost/preprocessor/cat.hpp>
#  include <boost/preprocessor/iterate.hpp>
#  include <boost/preprocessor/repetition/enum_params.hpp>
#  include <boost/preprocessor/repetition/enum_trailing_params.hpp>

namespace luabind {

namespace detail
{

  template <class Signature, class F>
  struct tagged_function
  {
      tagged_function(F f)
        : f(f)
      {}

      F f;
  };

  template <class Signature, class F>
  Signature deduce_signature(tagged_function<Signature, F> const&, ...)
  {
      return Signature();
  }

  template <class Signature, class F, class Policies>
  int invoke(
      lua_State* L, function_object const& self, invoke_context& ctx
    , tagged_function<Signature, F> const& tagged
    , Signature, Policies const& policies)
  {
      return invoke(L, self, ctx, tagged.f, Signature(), policies);
  }

  template <class Function>
  struct signature_from_function;

#  define BOOST_PP_ITERATION_PARAMS_1 \
  (3, (0, LUABIND_MAX_ARITY, <luabind/tag_function.hpp>))
#  include BOOST_PP_ITERATE()

} // namespace detail

template <class Signature, class F>
detail::tagged_function<
    typename detail::signature_from_function<Signature>::type
  , F
>
tag_function(F f)
{
    return f;
}

} // namespace luabind

# endif // LUABIND_TAG_FUNCTION_081129_HPP

#else // BOOST_PP_IS_ITERATING

# define N BOOST_PP_ITERATION()
# define NPLUS1 BOOST_PP_INC(N)

template <class R BOOST_PP_ENUM_TRAILING_PARAMS(N, class A)>
struct signature_from_function<R(BOOST_PP_ENUM_PARAMS(N, A))>
{
    typedef BOOST_PP_CAT(mpl::vector, NPLUS1)<
        R BOOST_PP_ENUM_TRAILING_PARAMS(N, A)
    > type;
};

#endif // BOOST_PP_IS_ITERATING


