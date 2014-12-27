#include <iostream>

extern "C"
{
	#include "lua.h"
	#include "lauxlib.h"
	#include "lualib.h"
}

#include <luabind/luabind.hpp>
#include <luabind/operator.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>

#include "directory_iterator.hpp"

const boost::filesystem::path& 
identity(const boost::filesystem::path& x)
{
	return x;
}

void bind_filesystem(lua_State* L)
{
	using namespace luabind;
	using namespace boost::filesystem;

	namespace fs = boost::filesystem;
	
	module(L, "filesystem")
	[
		class_<fs::path>("path")
			.def(constructor<>())
			.def(constructor<const char*>())
			.def("string", &fs::path::string)
			.def("native_file_string", &fs::path::native_file_string)
			.def("native_directory_string", &fs::path::native_directory_string)
			.def("root_path", &fs::path::root_path)
			.def("root_name", &fs::path::root_name)
			.def("root_directory", &fs::path::root_directory)
			.def("relative_path", &fs::path::relative_path)
			.def("leaf", &fs::path::leaf)
			.def("branch_path", &fs::path::branch_path)

			.def("empty", &fs::path::empty)
			.def("is_complete", &fs::path::is_complete)
			.def("is_directory", &fs::is_directory)
			.def("is_empty", &fs::is_empty)
			.def("has_root_path", &fs::path::has_root_path)
			.def("has_root_name", &fs::path::has_root_name)
			.def("has_root_directory", &fs::path::has_root_directory)
			.def("has_relative_path", &fs::path::has_relative_path)
			.def("has_leaf", &fs::path::has_leaf)
			.def("has_branch_path", &fs::path::has_branch_path)

			.def(const_self / const_self)
			.def(other<const char*>() / const_self)
			.def(const_self / other<const char*>())

//			.property("contents", &identity, return_directory_iterator)
			,

		def("exists", &fs::exists),
		def("is_directory", &fs::is_directory),
		def("is_empty", &fs::is_empty),
		def("create_directory", &fs::create_directory),
		def("remove", &fs::remove),
		def("remove_all", &fs::remove_all),
		def("rename", &fs::rename),
		def("copy_file", &fs::copy_file),

		def("initial_path", &fs::initial_path),
		def("current_path", &fs::current_path),
		def("complete", &fs::complete),
		def("system_complete", &fs::system_complete)
	];
}

int main(int argc, const char* argv[])
{
	lua_State* L = lua_open();
	luaopen_base(L);
	luaopen_string(L);
	luaopen_table(L);
	luaopen_math(L);
	luaopen_io(L);
	luaopen_debug(L);

	if (argc < 2)
	{
		std::cout << "This example will bind parts of the boost.Filesystem library\n"
			"and then run the given lua file.\n\n"
			"usage: filesystem filename [args]\n";
		return 1;
	}
	
	using namespace luabind;

	open(L);

	bind_filesystem(L);
	
	object args = newtable(L);

	for (int i = 0; i < argc; ++i)
	{
		args[i + 1] = argv[i];
	}

	args["n"] = argc;
	globals(L)["args"] = args;

	luaL_dofile(L, argv[1]);
}

