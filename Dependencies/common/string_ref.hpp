#pragma once
#include <cstdint>
#include <string>
#include <iostream>

template<typename Char>
class basic_string_ref
{
public:
    using value_type = Char;
    using traits_type = std::char_traits<value_type>;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using const_pointer = const value_type*;
    using pointer = const_pointer;
    using const_reference = const value_type&;
    using reference = const_reference;
    using const_iterator = const value_type*;
    using iterator = const_iterator;
    static constexpr size_type npos = size_type(-1);

private:
    const value_type* data_;
    size_type size_;

public:
    constexpr basic_string_ref() noexcept
        : data_(nullptr)
        , size_(0)
    {
    }

    constexpr basic_string_ref(const_pointer s, size_type size) noexcept
        :data_(s)
        ,size_(size)
    {}

    constexpr basic_string_ref(const_pointer s) noexcept
        :data_(s)
        ,size_(std::char_traits<value_type>::length(s))
    {}

    constexpr basic_string_ref(const std::basic_string<value_type> &s) noexcept
        :data_(s.data())
        ,size_(s.size())
    {}

    constexpr const_pointer data() const noexcept
    {
        return data_;
    }

    constexpr size_type size() const noexcept
    {
        return size_;
    }

    constexpr size_type length() const noexcept
    {
        return size();
    }

    constexpr bool empty() const noexcept
    {
        return size_ == 0;
    }

    constexpr const_reference front() const noexcept
    {
        return *data_;
    }

    constexpr const_reference back() const noexcept
    {
        return data_[size_-1];
    }

    const_iterator begin() const noexcept
    {
        return data_;
    }

    const_iterator end() const noexcept
    {
        return data_ + size_;
    }

    int32_t compare(basic_string_ref other) const noexcept
    {
        size_type size = size_ < other.size_ ? size_ : other.size_;
        int result = std::char_traits<value_type>::compare(data_, other.data_, size);
        if (result == 0)
            result = size_ == other.size_ ? 0 : (size_ < other.size_ ? -1 : 1);
        return result;
    }

    int32_t compare(const size_type _Off, const size_type _N0,basic_string_ref other) const noexcept
    {
        auto sub = substr(_Off, _N0);
        return (substr(_Off, _N0).compare(other));
    }

    size_type find(const basic_string_ref& v, size_type pos = 0) const noexcept
    {
        if (v.size() == 0 && pos <= size_)
        {
            return pos;// null string always matches (if inside string)
        }

        size_type Nm;
        if (pos < size_ && v.size() <= (Nm = size_ - pos))
        {
            const_pointer Uptr, Vptr;
            for (Nm -= v.size() - 1, Vptr = data_ + pos;
                (Uptr = std::char_traits<value_type>::find(Vptr, Nm, v[0])) != 0;
                Nm -= Uptr - Vptr + 1, Vptr = Uptr + 1)
                if (std::char_traits<value_type>::compare(Uptr, v.data(), v.size()) == 0)
                    return (Uptr - data_);
        }
        return npos;
    }

    size_type find(const value_type v, size_type pos = 0) const noexcept
    {
        return find(basic_string_ref(&v, 1), pos);
    }

    basic_string_ref substr(size_t pos, size_t len) const
    {
        const size_type max_length = pos > size_ ? 0 : size_ - pos;
        return pos < size_ ? basic_string_ref(data_ + pos, len > max_length ? max_length : len) : throw std::out_of_range("Index out of range in basic_string_view::substr");
    }

    const_reference operator[](size_t pos) const noexcept
    {
        return data_[pos];
    }

    friend bool operator==(basic_string_ref lhs, basic_string_ref rhs)
    {
        return lhs.compare(rhs) == 0;
    }

    friend bool operator!=(basic_string_ref lhs, basic_string_ref rhs)
    {
        return lhs.compare(rhs) != 0;
    }

    friend bool operator<(basic_string_ref lhs, basic_string_ref rhs)
    {
        return lhs.compare(rhs) < 0;
    }

    friend bool operator<=(basic_string_ref lhs, basic_string_ref rhs)
    {
        return lhs.compare(rhs) <= 0;
    }

    friend bool operator>(basic_string_ref lhs, basic_string_ref rhs)
    {
        return lhs.compare(rhs) > 0;
    }

    friend bool operator>=(basic_string_ref lhs, basic_string_ref rhs)
    {
        return lhs.compare(rhs) >= 0;
    }

    friend std::basic_ostream<value_type>& operator<<(std::basic_ostream<value_type> &os, const basic_string_ref &str)
    {
        for (auto c : str)
        {
            os << c;
        }
        return os;
    }
};

using string_view_t = basic_string_ref<char>;
using wstring_view_t = basic_string_ref<wchar_t>;

