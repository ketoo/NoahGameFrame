// Copyright (C) by Ashton Mason. See LICENSE.txt for licensing information.
#ifndef THERON_DETAIL_STRINGS_STRING_H
#define THERON_DETAIL_STRINGS_STRING_H


#include <Theron/Assert.h>
#include <Theron/BasicTypes.h>
#include <Theron/Defines.h>

#include <Theron/Detail/Strings/StringPool.h>


namespace Theron
{
namespace Detail
{


/**
A copyable string type that is a lightweight wrapper around a pooled string.
*/
class String
{
public:

    /**
    Default constructor.
    Constructs a null string with no value.
    */
    THERON_FORCEINLINE String() : mValue(0)
    {
    }

    /**
    Explicit constructor.
    */
    THERON_FORCEINLINE explicit String(const char *const str) : mValue(0)
    {
        if (str)
        {
            mValue = StringPool::Get(str);
        }
    }

    /**
    Returns true if the string has no value.
    */
    THERON_FORCEINLINE bool IsNull() const
    {
        return (mValue == 0);
    }

    /**
    Gets the value of the string.
    \note Returns a null pointer if the string is null.
    */
    THERON_FORCEINLINE const char *GetValue() const
    {
        return mValue;
    }

    /**
    Equality operator.
    */
    THERON_FORCEINLINE bool operator==(const String &other) const
    {
        // Pooled strings are unique so we can compare their addresses.
        // This works for null strings too, whose value pointers are zero.
        return (mValue == other.mValue);
    }

    /**
    Inequality operator.
    */
    THERON_FORCEINLINE bool operator!=(const String &other) const
    {
        return !operator==(other);
    }

    /**
    Less-than operator, mainly for sorted containers.
    Defines an arbitrary, non-alphabetical partial ordering.
    */
    THERON_FORCEINLINE bool operator<(const String &other) const
    {
        // Arbitrary less-than based on cheap pointer comparison.
        // This works for null strings too, whose value pointers are zero.
        return (mValue < other.mValue);
    }

private:

    const char *mValue;
};


} // namespace Detail
} // namespace Theron


#endif // THERON_DETAIL_STRINGS_STRING_H
