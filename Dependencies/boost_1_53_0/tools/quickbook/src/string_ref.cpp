/*=============================================================================
    Copyright (c) 2011 Daniel James

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#include "string_ref.hpp"
#include <boost/range/algorithm/equal.hpp>
#include <boost/range/algorithm/lexicographical_compare.hpp>
#include <boost/utility/swap.hpp>
#include <ostream>

namespace quickbook
{
    void string_ref::swap(string_ref& x)
    {
        boost::swap(begin_, x.begin_);
        boost::swap(end_, x.end_);
    }

    bool operator==(string_ref const& x, string_ref const& y)
    {
        return boost::equal(x, y);
    }

    bool operator<(string_ref const& x, string_ref const& y)
    {
        return boost::lexicographical_compare(x, y);
    }

    std::ostream& operator<<(std::ostream& out, string_ref const& x)
    {
        return out.write(&*x.begin(), x.end() - x.begin());
    }
}
