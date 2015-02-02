// Copyright (c) 2004 Daniel Wallin and Arvid Norberg

// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF
// ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED
// TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
// PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT
// SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR
// ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
// ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE
// OR OTHER DEALINGS IN THE SOFTWARE.

#ifndef LUABIND_BACK_REFERENCE_040510_HPP
#define LUABIND_BACK_REFERENCE_040510_HPP

#include <luabind/lua_include.hpp>
#include <luabind/wrapper_base.hpp>
#include <luabind/detail/has_get_pointer.hpp>
#include <luabind/get_pointer.hpp>
#include <boost/type_traits/is_polymorphic.hpp>
#include <boost/type_traits/is_const.hpp>
#include <boost/mpl/if.hpp>

namespace luabind {

namespace detail
{
  namespace mpl = boost::mpl;
 
  template<class T>
  wrap_base const* get_back_reference_aux0(T const* p, mpl::true_)
  {
      return dynamic_cast<wrap_base const*>(p);
  }

  template<class T>
  wrap_base const* get_back_reference_aux0(T const*, mpl::false_)
  {
      return 0;
  }

  template<class T>
  wrap_base const* get_back_reference_aux1(T const* p)
  {
      return get_back_reference_aux0(p, boost::is_polymorphic<T>());
  }

  template<class T>
  wrap_base const* get_back_reference_aux2(T const& x, mpl::true_)
  {
      return get_back_reference_aux1(get_pointer(x));
  }

  template<class T>
  wrap_base const* get_back_reference_aux2(T const& x, mpl::false_)
  {
      return get_back_reference_aux1(&x);
  }

  template<class T>
  wrap_base const* get_back_reference(T const& x)
  {
      return detail::get_back_reference_aux2(
          x
        , has_get_pointer<T>()
      );
  }
  
} // namespace detail

template<class T>
bool get_back_reference(lua_State* L, T const& x)
{
#ifndef LUABIND_NO_RTTI
    if (wrap_base const* w = detail::get_back_reference(x))
    {
        detail::wrap_access::ref(*w).get(L);
        return true;
    }
#endif
    return false;
}

template<class T>
bool move_back_reference(lua_State* L, T const& x)
{
#ifndef LUABIND_NO_RTTI
    if (wrap_base* w = const_cast<wrap_base*>(detail::get_back_reference(x)))
    {
        assert(detail::wrap_access::ref(*w).m_strong_ref.is_valid());
        detail::wrap_access::ref(*w).get(L);
        detail::wrap_access::ref(*w).m_strong_ref.reset();
        return true;
    }
#endif
    return false;
}

} // namespace luabind

#endif // LUABIND_BACK_REFERENCE_040510_HPP

