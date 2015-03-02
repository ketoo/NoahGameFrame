#include <iostream>
#include <ctime>

namespace std
{
	using ::clock_t;
//	using ::clock;
}

#define LUABIND_NO_ERROR_CHECKING
#define LUABIND_DONT_COPY_STRINGS
//#define LUABIND_NOT_THREADSAFE

extern "C"
{
	#include "lua.h"
	#include "lauxlib.h"
}

#include <luabind/luabind.hpp>

struct A {};

// luabind function
float f1(int a, float b, const char* str, A* c)
{
	return 3.14f;
}

// empty function
int f2(lua_State* L)
{
	return 0;
}


int main()
{
	const int num_calls = 100000;
	const int loops = 10;

	using namespace luabind;

	lua_State* L = luaL_newstate();
	open(L);
	
	class_<A>(L, "A")
		.def(constructor<>());

	function(L, "test1", &f1);

	lua_pushcclosure(L, &f2, 0);
	lua_setglobal(L, "test2");

	std::clock_t total1 = 0;
	std::clock_t total2 = 0;

	for (int i = 0; i < loops; ++i)
	{
		// benchmark luabind
		std::clock_t start1 = std::clock();
		lua_dostring(L, "a = A()\n"
									"for i = 1, 100000 do\n"
										"test1(5, 4.6, 'foo', a)\n"
									"end");

		std::clock_t end1 = std::clock();


		// benchmark empty binding
		std::clock_t start2 = std::clock();
		lua_dostring(L, "a = A()\n"
									"for i = 1, 100000 do\n"
										"test2(5, 4.6, 'foo', a)\n"
									"end");

		std::clock_t end2 = std::clock();
		total1 += end1 - start1;
		total2 += end2 - start2;
	}


	double time1 = double(total1) / (double)CLOCKS_PER_SEC;
	double time2 = double(total2) / (double)CLOCKS_PER_SEC;

#ifdef LUABIND_NO_ERROR_CHECKING
	std::cout << "without error-checking\n";
#endif
	std::cout << "luabind:\t" << time1 * 1000000 / num_calls / loops << " microseconds per call\n"
		<< "empty:\t" << time2 * 1000000 / num_calls / loops << " microseconds per call\n"
		<< "diff:\t" << ((time1 - time2) * 1000000 / num_calls / loops) << " microseconds\n\n";

	lua_close(L);
}

