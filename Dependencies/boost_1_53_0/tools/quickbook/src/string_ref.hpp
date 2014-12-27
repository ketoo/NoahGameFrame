/*=============================================================================
    Copyright (c) 2011 Daniel James

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#if !defined(BOOST_QUICKBOOK_STRING_REF_HPP)
#define BOOST_QUICKBOOK_STRING_REF_HPP

#include <boost/operators.hpp>
#include <string>
#include <iosfwd>

namespace quickbook
{
    struct string_ref
        : boost::less_than_comparable<string_ref,
            boost::less_than_comparable<string_ref, std::string,
            boost::equality_comparable<string_ref,
            boost::equality_comparable<string_ref, std::string> > > >
    {
    public:
        typedef std::string::const_iterator iterator;
        typedef std::string::const_iterator const_iterator;

    private:
        iterator begin_, end_;

    public:
        string_ref() : begin_(), end_() {}

        explicit string_ref(iterator b, iterator e)
            : begin_(b), end_(e) {}

        explicit string_ref(std::string const& x)
            : begin_(x.begin()), end_(x.end()) {}

        void swap(string_ref&);

        void clear() {
            begin_ = end_ = iterator();
        }

        operator std::string() const {
            return std::string(begin_, end_);
        }

        iterator begin() const { return begin_; }
        iterator end() const { return end_; }

        std::size_t size() const
        {
            return static_cast<std::size_t>(end_ - begin_);
        }

        bool empty() const
        {
            return begin_ == end_;
        }
    };

    bool operator==(string_ref const& x, string_ref const& y);
    bool operator<(string_ref const& x, string_ref const& y);
    std::ostream& operator<<(std::ostream&, string_ref const& x);

    inline bool operator==(string_ref const& x, std::string const& y)
    {
        return x == string_ref(y);
    }

    inline bool operator<(string_ref const& x, std::string const& y)
    {
        return x < string_ref(y);
    }

    inline bool operator>(string_ref const& x, std::string const& y)
    {
        return x > string_ref(y);
    }

    inline void swap(string_ref& x, string_ref& y)
    {
        x.swap(y);
    }
}

#endif
