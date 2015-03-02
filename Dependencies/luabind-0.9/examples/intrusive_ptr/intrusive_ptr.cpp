#include <iostream>

extern "C"
{
	#include "lua.h"
	#include "lauxlib.h"
	#include "lualib.h"
}

bool dostring(lua_State* L, const char* str)
{
	if (luaL_loadbuffer(L, str, std::strlen(str), str) || lua_pcall(L, 0, 0, 0))
	{
		const char* a = lua_tostring(L, -1);
		std::cout << a << "\n";
		lua_pop(L, 1);
		return true;
	}
	return false;
}

#include <luabind/luabind.hpp>
#include <boost/intrusive_ptr.hpp>

namespace luabind
{
	namespace converters
	{
		// tell luabind that there is a converter for boost::intrusive_ptr<T>
		template<class T>
		yes_t is_user_defined(by_value<boost::intrusive_ptr<T> >);

		template<class T>
		yes_t is_user_defined(by_const_reference<boost::intrusive_ptr<T> >);

		// function used to destruct the object in lua
		template<class T>
		struct decrement_ref_count
		{
			static void apply(void* ptr)
			{
				T* p = static_cast<T*>(ptr);
				intrusive_ptr_release(p);
			}
		};

		template<class T>
		void convert_cpp_to_lua(lua_State* L, const boost::intrusive_ptr<T>& ptr)
		{
			if (!ptr)
			{
				lua_pushnil(L);
				return;
			}

			T* raw_ptr = ptr.get();
			// add reference to object, this will be released when the object is collected
			intrusive_ptr_add_ref(raw_ptr);

			detail::class_registry* registry = luabind::detail::class_registry::get_registry(L);
			detail::class_rep* crep = registry->find_class(LUABIND_TYPEID(T));

			assert(crep != 0 && "You are trying to convert an unregistered type");

			// create the struct to hold the object
			void* obj = lua_newuserdata(L, sizeof(detail::object_rep));
			// we send 0 as destructor since we know it will never be called
			new(obj) luabind::detail::object_rep(raw_ptr, crep, detail::object_rep::owner, decrement_ref_count<T>::apply);

			// set the meta table
			detail::getref(L, crep->metatable_ref());
			lua_setmetatable(L, -2);
		}

		template<class T>
		boost::intrusive_ptr<T>
		convert_lua_to_cpp(lua_State* L, by_const_reference<boost::intrusive_ptr<T> >, int index)
		{
			typename detail::default_policy::template generate_converter<T*, detail::lua_to_cpp>::type converter;
			T* ptr = converter.apply(L, LUABIND_DECORATE_TYPE(T*), index);
			return boost::intrusive_ptr<T>(ptr);
		}

		template<class T>
		boost::intrusive_ptr<T> convert_lua_to_cpp(lua_State* L, by_value<boost::intrusive_ptr<T> >, int index)
		{
			return convert_lua_to_cpp(L, by_const_reference<boost::intrusive_ptr<T> >(), index);
		}

		template<class T>
		int match_lua_to_cpp(lua_State* L, by_value<boost::intrusive_ptr<T> >, int index)
		{
			typedef typename detail::default_policy::template generate_converter<T*, detail::lua_to_cpp>::type converter_t;
			return converter_t::match(L, LUABIND_DECORATE_TYPE(T*), index);
		}

		template<class T>
		int match_lua_to_cpp(lua_State* L, by_const_reference<boost::intrusive_ptr<T> >, int index)
		{
			typedef typename detail::default_policy::template generate_converter<T*, detail::lua_to_cpp>::type converter_t;
			return converter_t::match(L, LUABIND_DECORATE_TYPE(T*), index);
		}

	}
}

struct A
{
	A()
	: cnt(0) 
	{}

	~A() { std::cout << "free memory\n"; }
	
	int cnt;
};

void intrusive_ptr_add_ref(A* ptr)
{
	++ptr->cnt;
	std::cout << "add ref\n";
}

void intrusive_ptr_release(A* ptr)
{
	--ptr->cnt;
	std::cout << "release\n";
	if (ptr->cnt == 0) delete ptr;
}

void f(boost::intrusive_ptr<A> ptr)
{
	std::cout << "count: " << ptr->cnt << "\n";
}

boost::intrusive_ptr<A> factory()
{
	return boost::intrusive_ptr<A>(new A());
}

int main()
{
	lua_State* L = luaL_newstate();
	lua_baselibopen(L);

	luabind::open(L);
	
	using namespace luabind;

	module(L)
	[
		class_<A>("A")
			.def_readonly("cnt", &A::cnt),

		def("factory", &factory),
		def("f", &f)
	];

	dostring(L, "a = factory()");
	dostring(L, "print('lua count: ' .. a.cnt)");
	dostring(L, "f(a)");

	lua_close(L);
	return 0;
}

