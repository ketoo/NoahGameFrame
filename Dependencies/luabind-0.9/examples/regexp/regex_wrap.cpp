#include <boost/cregex.hpp> 

extern "C"
{
	#include "lua.h"
	#include "lauxlib.h"
	#include "lualib.h"
}

#include <luabind/luabind.hpp>

namespace
{
	bool match(boost::RegEx& r, const char* s)
	{
		return r.Match(s);
	}

	bool search(boost::RegEx& r, const char* s)
	{
		return r.Search(s);
	}
} // namespace unnamed


void wrap_regex(lua_State* L)
{
	using boost::RegEx;
	using namespace luabind;

	module(L)
	[
		class_<RegEx>("regex")
			.def(constructor<const char*>())
			.def(constructor<const char*, bool>())
			.def("match", match)
			.def("search", search)
			.def("what", &RegEx::What)
			.def("matched", &RegEx::Matched)
			.def("length", &RegEx::Length)
			.def("position", &RegEx::Position)
	];
}

int main()
{
	lua_State* L = luaL_newstate();
	lua_baselibopen(L);
	lua_strlibopen(L);
	luabind::open(L);
	
	wrap_regex(L);
	
	lua_dofile(L, "regex.lua");
	
	lua_close(L);
}

