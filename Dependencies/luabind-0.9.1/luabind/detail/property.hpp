// Copyright Daniel Wallin 2008. Use, modification and distribution is
// subject to the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef LUABIND_PROPERTY_081020_HPP
# define LUABIND_PROPERTY_081020_HPP

namespace luabind { namespace detail {

template <class Class, class T, class Result = T>
struct access_member_ptr
{
    access_member_ptr(T Class::* mem_ptr)
      : mem_ptr(mem_ptr)
    {}

    Result operator()(Class const& x) const
    {
        return const_cast<Class&>(x).*mem_ptr;
    }

    void operator()(Class& x, T const& value) const
    {
        x.*mem_ptr = value;
    }

    T Class::* mem_ptr;
};

}} // namespace luabind::detail

#endif // LUABIND_PROPERTY_081020_HPP

