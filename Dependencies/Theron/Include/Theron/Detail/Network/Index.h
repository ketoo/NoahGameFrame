// Copyright (C) by Ashton Mason. See LICENSE.txt for licensing information.
#ifndef THERON_DETAIL_NETWORK_INDEX_H
#define THERON_DETAIL_NETWORK_INDEX_H


#include <Theron/Assert.h>
#include <Theron/BasicTypes.h>
#include <Theron/Defines.h>


namespace Theron
{
namespace Detail
{


/**
\brief Union that combines a framework index and a mailbox index.
*/
union Index
{
    THERON_FORCEINLINE Index() : mUInt32(0)
    {
    }

    THERON_FORCEINLINE Index(const uint32_t framework, const uint32_t index) : mUInt32(0)
    {
        mComponents.mFramework = framework;
        mComponents.mIndex = index;
    }

    THERON_FORCEINLINE Index(const Index &other) : mUInt32(other.mUInt32)
    {
    }

    THERON_FORCEINLINE Index &operator=(const Index &other)
    {
        mUInt32 = other.mUInt32;
        return *this;
    }

    THERON_FORCEINLINE bool operator==(const Index &other) const
    {
        return (mUInt32 == other.mUInt32);
    }

    THERON_FORCEINLINE bool operator!=(const Index &other) const
    {
        return (mUInt32 != other.mUInt32);
    }

    THERON_FORCEINLINE bool operator<(const Index &other) const
    {
        return (mUInt32 < other.mUInt32);
    }

    uint32_t mUInt32;               ///< Unsigned 32-bit value.

    struct
    {
        uint32_t mFramework : 12;  ///< Integer index identifying the framework within the local process (zero indicates a receiver).
        uint32_t mIndex : 20;      ///< Integer index of the actor within the framework (or receiver within the process).

    } mComponents;
};


} // namespace Detail
} // namespace Theron


#endif // THERON_DETAIL_NETWORK_INDEX_H
