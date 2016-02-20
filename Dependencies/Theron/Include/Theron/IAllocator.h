// Copyright (C) by Ashton Mason. See LICENSE.txt for licensing information.
#ifndef THERON_IALLOCATOR_H
#define THERON_IALLOCATOR_H


/**
\file IAllocator.h
Allocator interface.
*/


#include <Theron/BasicTypes.h>


namespace Theron
{


/**
\brief Interface describing a general-purpose memory allocator.

This interface class defines the interface expected of allocators used by Theron.
In particular, it characterizes the type of allocator that can be assigned as
Theron's internal allocator with \ref AllocatorManager::SetAllocator.

By implementing the IAllocator interface on a custom allocator implementation
(or wrapping an existing allocator in a custom wrapper that implements IAllocator),
users can provide a custom allocator to be used by Theron. When set via
\ref AllocatorManager::SetAllocator, then the allocator replaces the \ref
DefaultAllocator that is used within Theron by default.

By replacing the default allocator, users can control, cache, or optimize the
allocation of memory within Theron. Theron guarantees that all of its internal
heap allocations are allocated via the allocator set with
\ref AllocatorManager::SetAllocator, as long as the allocator is set before
any Theron objects are constructed.
*/
class IAllocator
{
public:

    /**
    \brief Defines an integer type used for specifying sizes of memory allocations.
    */
    typedef uint32_t SizeType;

    /**
    \brief Default constructor
    */
    inline IAllocator()
    {
    }
    
    /**
    \brief Virtual destructor
    */
    inline virtual ~IAllocator()
    {
    }

    /**
    \brief Allocates a piece of contiguous memory.
    \param size The size of the memory to allocate, in bytes.
    \return A pointer to the allocated memory.
    */
    virtual void *Allocate(const SizeType size) = 0;

    /**
    \brief Allocates a piece of contiguous memory aligned to a given byte-multiple boundary.
    \param size The size of the memory to allocate, in bytes.
    \param alignment The alignment of the memory to allocate, in bytes.
    \return A pointer to the allocated memory.
    \note The default implementation, which implementors can override,
    simply calls \ref Allocate, ignoring the alignment. This provides backwards
    compatibility with legacy implementations that don't implement AllocateAligned.
    */
    inline virtual void *AllocateAligned(const SizeType size, const SizeType /*alignment*/)
    {
        return Allocate(size);
    }

    /**
    \brief Frees a previously allocated piece of contiguous memory.
    \param memory A pointer to the memory to be deallocated.
    */
    virtual void Free(void *const memory) = 0;

    /**
    \brief Frees a previously allocated block of contiguous memory of a known size.
    Knowing the size of the freed block allows some implementations to cache and reuse freed blocks.
    \param memory A pointer to the block of memory to be deallocated.
    \param size The size of the freed block.
    \note The default implementation, which implementors can override,
    simply calls \ref Free with no size, ignoring the size parameter. This provides backwards
    compatibility with legacy implementations that don't implement this overload of Free.
    */
    inline virtual void Free(void *const memory, const SizeType /*size*/)
    {
        Free(memory);
    }

private:

    IAllocator(const IAllocator &other);
    IAllocator &operator=(const IAllocator &other);
};


} // namespace Theron


#endif // THERON_IALLOCATOR_H
