// Copyright Daniel Wallin 2008. Use, modification and distribution is
// subject to the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef LUABIND_CONVERSION_STORAGE_080930_HPP
# define LUABIND_CONVERSION_STORAGE_080930_HPP

# include <luabind/config.hpp>
# include <boost/aligned_storage.hpp>

namespace luabind { namespace detail {

typedef void(*destruction_function)(void*);

// This is used by the converters in policy.hpp, and
// class_rep::convert_to as temporary storage when constructing
// holders.

struct conversion_storage
{
    conversion_storage()
      : destructor(0)
    {}

    ~conversion_storage()
    {
        if (destructor)
            destructor(&data);
    }

    // Unfortunately the converters currently doesn't have access to
    // the actual type being converted when this is instantiated, so
    // we have to guess a max size.
    boost::aligned_storage<128> data;
    destruction_function destructor;
};

}} // namespace luabind::detail

#endif // LUABIND_CONVERSION_STORAGE_080930_HPP

