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

#ifndef LUABIND_LINK_COMPATIBILITY_HPP_INCLUDED
#define LUABIND_LINK_COMPATIBILITY_HPP_INCLUDED

#include <luabind/config.hpp>

namespace luabind { namespace detail
{

#ifdef LUABIND_NOT_THREADSAFE
	LUABIND_API void not_threadsafe_defined_conflict();
#else
	LUABIND_API void not_threadsafe_not_defined_conflict();
#endif

#ifdef LUABIND_NO_ERROR_CHECKING
	LUABIND_API void no_error_checking_defined_conflict();
#else
	LUABIND_API void no_error_checking_not_defined_conflict();
#endif

	inline void check_link_compatibility()
	{
	#ifdef LUABIND_NOT_THREADSAFE
		not_threadsafe_defined_conflict();
	#else
		not_threadsafe_not_defined_conflict();
	#endif

	#ifdef LUABIND_NO_ERROR_CHECKING
		no_error_checking_defined_conflict();
	#else
		no_error_checking_not_defined_conflict();
	#endif
	}

}}

#endif
