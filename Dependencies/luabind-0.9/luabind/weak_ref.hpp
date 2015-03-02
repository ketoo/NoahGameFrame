// Copyright (c) 2004 Daniel Wallin and Arvid Norberg

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

#ifndef WEAK_REF_040402_HPP
#define WEAK_REF_040402_HPP

#include <luabind/config.hpp>

struct lua_State;

namespace luabind {

    class LUABIND_API weak_ref
    {
    public:
        weak_ref();
        weak_ref(lua_State* main, lua_State* L, int index);
        weak_ref(weak_ref const&);
        ~weak_ref();

        weak_ref& operator=(weak_ref const&);

        void swap(weak_ref&);

		// returns a unique id that no
		// other weak ref will return
		int id() const;

        lua_State* state() const;
        void get(lua_State* L) const;

    private:
        struct impl;
        impl* m_impl;
    };

} // namespace luabind

#endif // WEAK_REF_040402_HPP

