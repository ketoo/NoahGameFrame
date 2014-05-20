// Copyright (C) by Ashton Mason. See LICENSE.txt for licensing information.
#ifndef THERON_DETAIL_MESSAGES_MESSAGESIZE_H
#define THERON_DETAIL_MESSAGES_MESSAGESIZE_H


#include <Theron/BasicTypes.h>
#include <Theron/Defines.h>


namespace Theron
{
namespace Detail
{


/**
\brief Helper that tells us the allocated size of a message value type.
The point really is to do the job of rounding up 'empty' allocations consistently in one place.
*/
template <class ValueType>
class MessageSize
{
public:

    THERON_FORCEINLINE static uint32_t GetSize()
    {
        uint32_t valueSize(sizeof(ValueType));
        const uint32_t minimumAllocationSize(4);

        // Empty structs passed as message values have a size of one byte, which we don't like.
        // To be on the safe side we round every allocation up to at least four bytes.
        // If we don't then the data that follows won't be word-aligned.
        if (valueSize < minimumAllocationSize)
        {
            valueSize = minimumAllocationSize;
        }

        return valueSize;
    }

private:

	MessageSize();
    MessageSize(const MessageSize &other);
    MessageSize &operator=(const MessageSize &other);
};


} // namespace Detail
} // namespace Theron


#endif // THERON_DETAIL_MESSAGES_MESSAGESIZE_H

