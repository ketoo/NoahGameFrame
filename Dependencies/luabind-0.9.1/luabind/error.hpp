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

#ifndef LUABIND_ERROR_HPP_INCLUDED
#define LUABIND_ERROR_HPP_INCLUDED

#include <luabind/prefix.hpp>
#include <exception>
#include <luabind/config.hpp>
#include <luabind/typeid.hpp>

struct lua_State;

namespace luabind
{

#ifndef LUABIND_NO_EXCEPTIONS

	// this exception usually means that the lua function you called
	// from C++ failed with an error code. You will have to
	// read the error code from the top of the lua stack
	// the reason why this exception class doesn't contain
	// the message itself is that std::string's copy constructor
	// may throw, if the copy constructor of an exception that is
	// being thrown throws another exception, terminate will be called
	// and the entire application is killed.
	class LUABIND_API error : public std::exception
	{
	public:
		explicit error(lua_State* L): m_L(L) {}
		lua_State* state() const throw() { return m_L; }
		virtual const char* what() const throw()
		{
			return "lua runtime error";
		}
	private:
		lua_State* m_L;
	};

	// if an object_cast<>() fails, this is thrown
	// it is also thrown if the return value of
	// a lua function cannot be converted
	class LUABIND_API cast_failed : public std::exception
	{
	public:
		cast_failed(lua_State* L, type_id const& i): m_L(L), m_info(i) {}
		lua_State* state() const throw() { return m_L; }
		type_id info() const throw() { return m_info; }
		virtual const char* what() const throw() { return "unable to make cast"; }
	private:
		lua_State* m_L;
		type_id m_info;
	};

#else

	typedef void(*error_callback_fun)(lua_State*);
	typedef void(*cast_failed_callback_fun)(lua_State*, type_id const&);

	LUABIND_API void set_error_callback(error_callback_fun e);
	LUABIND_API void set_cast_failed_callback(cast_failed_callback_fun c);
	LUABIND_API error_callback_fun get_error_callback();
	LUABIND_API cast_failed_callback_fun get_cast_failed_callback();

#endif

	typedef int(*pcall_callback_fun)(lua_State*);
	LUABIND_API void set_pcall_callback(pcall_callback_fun e);
	LUABIND_API pcall_callback_fun get_pcall_callback();

}

#endif // LUABIND_ERROR_HPP_INCLUDED

