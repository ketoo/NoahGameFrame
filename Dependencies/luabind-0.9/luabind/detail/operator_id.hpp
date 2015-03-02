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


#ifndef LUABIND_OPERATOR_ID_HPP_INCLUDED
#define LUABIND_OPERATOR_ID_HPP_INCLUDED

#include <luabind/config.hpp>

namespace luabind { namespace detail {

	enum operator_id
	{
		op_add = 0,
		op_sub,
		op_mul,
		op_div,
		op_pow,
		op_lt,
		op_le,
		op_eq,
		op_call,
		op_unm,
		op_tostring,
        op_concat,
		op_len,

		number_of_operators
	};

	inline const char* get_operator_name(int i)
	{
		static const char* a[number_of_operators] = {
            "__add", "__sub", "__mul", "__div", "__pow", 
            "__lt", "__le", "__eq", "__call", "__unm", 
            "__tostring", "__concat", "__len" };
		return a[i];
	}

	inline const char* get_operator_symbol(int i)
	{
		static const char* a[number_of_operators] = {
            "+", "-", "*", "/", "^", "<", 
            "<=", "==", "()", "- (unary)", 
            "tostring", "..", "#" };
		return a[i];
	}

	inline bool is_unary(int i)
	{
		// the reason why unary minus is not considered a unary operator here is
		// that it always is given two parameters, where the second parameter always
		// is nil.
		return i == op_tostring;
	}


}}

#endif // LUABIND_OPERATOR_ID_HPP_INCLUDED
