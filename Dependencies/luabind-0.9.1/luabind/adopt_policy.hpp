// Copyright (c) 2003 Daniel Wallin and Arvid Norberg

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


#ifndef LUABIND_ADOPT_POLICY_HPP_INCLUDED
#define LUABIND_ADOPT_POLICY_HPP_INCLUDED

#include <luabind/config.hpp>
#include <luabind/wrapper_base.hpp>
#include <luabind/detail/policy.hpp>
#include <luabind/back_reference_fwd.hpp>
#include <luabind/wrapper_base.hpp>
#include <boost/type_traits/is_polymorphic.hpp>

namespace luabind { namespace detail 
{
    template <class T>
    void adjust_backref_ownership(T* ptr, mpl::true_)
    {
        if (wrap_base* p = dynamic_cast<wrap_base*>(ptr))
        {
            wrapped_self_t& wrapper = wrap_access::ref(*p);
            wrapper.get(wrapper.state());
            wrapper.m_strong_ref.set(wrapper.state());
        }
    }

    inline void adjust_backref_ownership(void*, mpl::false_)
    {}

	template<class Direction = lua_to_cpp>
    struct adopt_pointer : pointer_converter
	{
		typedef adopt_pointer type;

        int const consumed_args(...)
        {
            return 1;
        }

		template<class T>
		T* apply(lua_State* L, by_pointer<T>, int index)
		{
            T* ptr = pointer_converter::apply(
                L, LUABIND_DECORATE_TYPE(T*), index);

            object_rep* obj = static_cast<object_rep*>(
                lua_touserdata(L, index));
            obj->release();

            adjust_backref_ownership(ptr, boost::is_polymorphic<T>());

            return ptr;
		}

		template<class T>
		int match(lua_State* L, by_pointer<T>, int index)
		{
            return pointer_converter::match(
                L, LUABIND_DECORATE_TYPE(T*), index);
		}

		template<class T>
		void converter_postcall(lua_State*, T, int) {}
	};

	template<>
	struct adopt_pointer<cpp_to_lua>
	{
		typedef adopt_pointer type;

		template<class T>
		void apply(lua_State* L, T* ptr)
		{
			if (ptr == 0) 
			{
				lua_pushnil(L);
				return;
			}

			// if there is a back_reference, then the
			// ownership will be removed from the
			// back reference and put on the lua stack.
			if (luabind::move_back_reference(L, ptr))
				return;

            make_instance(L, std::auto_ptr<T>(ptr));
		}
	};

	template<int N>
//	struct adopt_policy : converter_policy_tag
	struct adopt_policy : conversion_policy<N>
	{
//		BOOST_STATIC_CONSTANT(int, index = N);

		static void precall(lua_State*, const index_map&) {}
		static void postcall(lua_State*, const index_map&) {}

		struct only_accepts_nonconst_pointers {};

		template<class T, class Direction>
		struct apply
		{
			typedef luabind::detail::is_nonconst_pointer<T> is_nonconst_p;
			typedef typename boost::mpl::if_<is_nonconst_p, adopt_pointer<Direction>, only_accepts_nonconst_pointers>::type type;
		};
	};

}}

namespace luabind
{
	template<int N>
	detail::policy_cons<detail::adopt_policy<N>, detail::null_type> 
	adopt(LUABIND_PLACEHOLDER_ARG(N))
	{ 
		return detail::policy_cons<detail::adopt_policy<N>, detail::null_type>(); 
	}
}

#endif // LUABIND_ADOPT_POLICY_HPP_INCLUDE

