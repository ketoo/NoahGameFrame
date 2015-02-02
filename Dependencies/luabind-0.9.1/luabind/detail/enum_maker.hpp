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


#ifndef LUABIND_ENUM_MAKER_HPP_INCLUDED
#define LUABIND_ENUM_MAKER_HPP_INCLUDED

#include <vector>
#include <string>

#include <luabind/config.hpp>
#include <luabind/detail/class_rep.hpp>

namespace luabind
{
	struct value;

	struct value_vector : public std::vector<value>
	{
		// a bug in intel's compiler forces us to declare these constructors explicitly.
		value_vector();
		virtual ~value_vector();
		value_vector(const value_vector& v);
		value_vector& operator,(const value& rhs);
	};

	struct value
	{
	friend class std::vector<value>;
		template<class T>
		value(const char* name, T v)
			: name_(name)
			, val_(v)
		{}

		const char* name_;
		int val_;

		value_vector operator,(const value& rhs) const
		{
			value_vector v;

			v.push_back(*this);
			v.push_back(rhs);

			return v;
		}

	private: 

		value() {}
	};

	inline value_vector::value_vector()
		: std::vector<value>()
	{
	}

	inline value_vector::~value_vector() {}

	inline value_vector::value_vector(const value_vector& rhs)
		: std::vector<value>(rhs)
	{
	}

	inline value_vector& value_vector::operator,(const value& rhs)
	{
		push_back(rhs);
		return *this;
	}

	namespace detail
	{
		template<class From>
		struct enum_maker
		{
			explicit enum_maker(From& from): from_(from) {}

			From& operator[](const value& val)
			{
				from_.add_static_constant(val.name_, val.val_);
				return from_;
			}
			
			From& operator[](const value_vector& values)
			{
				for (value_vector::const_iterator i = values.begin(); i != values.end(); ++i)
				{
					from_.add_static_constant(i->name_, i->val_);
				}

				return from_;
			}

			From& from_;

		private:
            void operator=(enum_maker const&); // C4512, assignment operator could not be generated
			template<class T> void operator,(T const&) const;
		};
	}
}

#endif // LUABIND_ENUM_MAKER_HPP_INCLUDED
