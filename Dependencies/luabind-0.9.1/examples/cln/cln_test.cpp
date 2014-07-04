#include <cln/cln.h>

extern "C"
{
	#include "lua.h"
	#include "lualib.h"
	#include "lauxlib.h"
}

#include <luabind/luabind.hpp>

void bind_cln(lua_State* L)
{
	using namespace luabind;
	using namespace cln;

	module(L)
	[
		// real numbers
		class_<cl_R>("cl_R")
			.def(constructor<>())
			.def(constructor<const cl_I&>())
			.def(constructor<float>())
			.def(constructor<const char*>())
			.def(tostring(const_self))
			.def(-self)
			.def(const_self + const_self)
			.def(const_self - const_self)
			.def(const_self * const_self)
			.def(const_self / const_self)
			.def(const_self <= const_self)
			.def(const_self < const_self)
			.def(const_self == const_self)
			.def(other<int>() + const_self)
			.def(other<int>() - const_self)
			.def(other<int>() * const_self)
			.def(other<int>() / const_self)
			.def(other<int>() <= const_self)
			.def(other<int>() < const_self)
			.def(const_self + other<int>())
			.def(const_self - other<int>())
			.def(const_self * other<int>())
			.def(const_self / other<int>())
			.def(const_self <= other<int>())
			.def(const_self < other<int>())
			,
		
		// rational numbers
		class_<cl_RA, cl_R>("cl_RA")
			.def(constructor<>())
			.def(constructor<const cl_I&>())
			.def(constructor<int>())
			.def(constructor<const char*>())
			.def(tostring(const_self))
			.def(-self)
			.def(const_self + const_self)
			.def(const_self - const_self)
			.def(const_self * const_self)
			.def(const_self / const_self)
			.def(const_self <= const_self)
			.def(const_self < const_self)
			.def(const_self == const_self)
			.def(other<int>() + const_self)
			.def(other<int>() - const_self)
			.def(other<int>() * const_self)
			.def(other<int>() / const_self)
			.def(other<int>() <= const_self)
			.def(other<int>() < const_self)
			.def(const_self + other<int>())
			.def(const_self - other<int>())
			.def(const_self * other<int>())
			.def(const_self / other<int>())
			.def(const_self <= other<int>())
			.def(const_self < other<int>())
			,

		// integers
		class_<cl_I, cl_RA>("cl_I")
			.def(constructor<>())
			.def(constructor<const cl_I&>())
			.def(constructor<int>())
			.def(constructor<const char*>())
			.def(tostring(const_self))
			.def(-self)
			.def(const_self + const_self)
			.def(const_self - const_self)
			.def(const_self * const_self)
			.def(const_self <= const_self)
			.def(const_self < const_self)
			.def(const_self == const_self)
			.def(other<int>() + const_self)
			.def(other<int>() - const_self)
			.def(other<int>() * const_self)
			.def(other<int>() <= const_self)
			.def(other<int>() < const_self)
			.def(const_self + other<int>())
			.def(const_self - other<int>())
			.def(const_self * other<int>())
			.def(const_self <= other<int>())
			.def(const_self < other<int>())
			,
			
		def("factorial", &cln::factorial),
		def("sqrt", (const cl_R(*)(const cl_R&))&cln::sqrt)
	];
}

int main()
{
	lua_State* L = lua_open();
	lua_baselibopen(L);
	lua_mathlibopen(L);
	luabind::open(L);

	bind_cln(L);

	lua_dofile(L, "cln_test.lua");

	lua_close(L);
	return 0;
}

