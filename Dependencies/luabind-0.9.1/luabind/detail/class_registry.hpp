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


#ifndef LUABIND_CLASS_REGISTRY_HPP_INCLUDED
#define LUABIND_CLASS_REGISTRY_HPP_INCLUDED

#include <map>

#include <luabind/config.hpp>
#include <luabind/open.hpp>
#include <luabind/typeid.hpp>

namespace luabind { namespace detail
{
	class class_rep;

	struct LUABIND_API class_registry
	{
		class_registry(lua_State* L);

		static class_registry* get_registry(lua_State* L);

		int cpp_instance() const { return m_instance_metatable; }
		int cpp_class() const { return m_cpp_class_metatable; }

		int lua_instance() const { return m_instance_metatable; }
		int lua_class() const { return m_lua_class_metatable; }
		int lua_function() const { return m_lua_function_metatable; }

		void add_class(type_id const& info, class_rep* crep);

		class_rep* find_class(type_id const& info) const;

        std::map<type_id, class_rep*> const& get_classes() const
        {
            return m_classes;
        }

	private:

		std::map<type_id, class_rep*> m_classes;

		// this is a lua reference that points to the lua table
		// that is to be used as meta table for all C++ class 
		// instances. It is a kind of v-table.
		int m_instance_metatable;

		// this is a lua reference to the metatable to be used
		// for all classes defined in C++.
		int m_cpp_class_metatable;

		// this is a lua reference to the metatable to be used
		// for all classes defined in lua
		int m_lua_class_metatable;

		// this metatable only contains a destructor
		// for luabind::Detail::free_functions::function_rep
		int m_lua_function_metatable;

	};

}}

#endif // LUABIND_CLASS_REGISTRY_HPP_INCLUDED

