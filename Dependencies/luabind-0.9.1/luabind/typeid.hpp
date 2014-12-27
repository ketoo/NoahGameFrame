// Copyright Daniel Wallin 2008. Use, modification and distribution is
// subject to the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef LUABIND_TYPEID_081227_HPP
# define LUABIND_TYPEID_081227_HPP

# include <boost/operators.hpp>
# include <typeinfo>
# include <luabind/detail/primitives.hpp>

namespace luabind {

# ifdef BOOST_MSVC
#  pragma warning(push)
// std::type_info::before() returns int, rather than bool.
// At least on MSVC7.1, this is true for the comparison
// operators as well.
#  pragma warning(disable:4800)
# endif

class type_id
  : public boost::less_than_comparable<type_id>
{
public:
    type_id()
      : id(&typeid(detail::null_type))
    {}

    type_id(std::type_info const& id)
      : id(&id)
    {}

    bool operator!=(type_id const& other) const
    {
        return *id != *other.id;
    }

    bool operator==(type_id const& other) const
    {
        return *id == *other.id;
    }

    bool operator<(type_id const& other) const
    {
        return id->before(*other.id);
    }

    char const* name() const
    {
        return id->name();
    }

private:
    std::type_info const* id;
};

# ifdef BOOST_MSVC
#  pragma warning(pop)
# endif

} // namespace luabind

#endif // LUABIND_TYPEID_081227_HPP

