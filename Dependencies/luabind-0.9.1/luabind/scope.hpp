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

#ifndef NEW_SCOPE_040211_HPP
#define NEW_SCOPE_040211_HPP

#include <luabind/prefix.hpp>
#include <luabind/config.hpp>
#include <luabind/lua_include.hpp>
#include <memory>

namespace luabind { 
    
    struct scope; 

} // namespace luabind

namespace luabind { namespace detail {

    struct LUABIND_API registration
    {
        registration();
        virtual ~registration();

    protected:
        virtual void register_(lua_State*) const = 0;

    private:
        friend struct ::luabind::scope;
        registration* m_next;
    };

}} // namespace luabind::detail

namespace luabind {

    struct LUABIND_API scope
    {
        scope();
        explicit scope(std::auto_ptr<detail::registration> reg);
        scope(scope const& other_);
        ~scope();

        scope& operator=(scope const& other_);

        scope& operator,(scope s);

        void register_(lua_State* L) const;

    private:
        detail::registration* m_chain;
    };

    class LUABIND_API namespace_ : public scope
    {
    public:
        explicit namespace_(char const* name);
        namespace_& operator[](scope s);

    private:
        struct registration_;
        registration_* m_registration;
    };

    class LUABIND_API module_
    {
    public:
        module_(lua_State* L_, char const* name);
        void operator[](scope s);

    private:
        lua_State* m_state;
        char const* m_name;
    };

    inline module_ module(lua_State* L, char const* name = 0)
    {
        return module_(L, name);
    }

} // namespace luabind

#endif // NEW_SCOPE_040211_HPP

