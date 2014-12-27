// Copyright (c) 2004 Daniel Wallin

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

#ifndef CLASS_CACHE_040218_HPP
#define CLASS_CACHE_040218_HPP

#include <luabind/prefix.hpp>
#include <boost/type_traits/add_reference.hpp>
#include <boost/type_traits/add_const.hpp>

namespace luabind { namespace detail {

#ifdef LUABIND_NOT_THREADSAFE
    
    class class_rep;
    
    template<class T>
    struct class_cache_impl
    {
        static lua_State* state;
        static class_rep* class_;
    };

    template<class T>
    lua_State* class_cache_impl<T>::state = 0;

    template<class T>
    class_rep* class_cache_impl<T>::class_ = 0;

    template<class T>
    struct class_cache
        : class_cache_impl<
              typename boost::add_reference<
                  typename boost::add_const<
                      T
                  >::type
              >::type
          >
    {
    };
    
    template<class T>
    class_rep* get_class_rep(lua_State* L, void(*)(T*) = 0)
    {
        if (class_cache<T>::state != L)
        {
            class_cache<T>::state = L;

            class_registry* registry = class_registry::get_registry(L);
			class_cache<T>::class_ = registry->find_class(typeid(T));
        }

        return class_cache<T>::class_;
    }

#else

    template<class T>
    class_rep* get_class_rep(lua_State* L, void(*)(T*) = 0)
    {
        class_registry* registry = class_registry::get_registry(L);
        return registry->find_class(typeid(T));
    }

#endif

}} // namespace luabind::detail

#endif // CLASS_CACHE_040218_HPP

