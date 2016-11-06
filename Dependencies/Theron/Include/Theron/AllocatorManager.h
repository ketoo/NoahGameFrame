// Copyright (C) by Ashton Mason. See LICENSE.txt for licensing information.
#ifndef THERON_ALLOCATORMANAGER_H
#define THERON_ALLOCATORMANAGER_H


/**
\file AllocatorManager.h
Static manager class for allocators used within Theron.
*/


#include <Theron/Assert.h>
#include <Theron/DefaultAllocator.h>
#include <Theron/Defines.h>
#include <Theron/IAllocator.h>

#include <Theron/Detail/Allocators/CachingAllocator.h>
#include <Theron/Detail/Threading/SpinLock.h>


#ifdef _MSC_VER
#pragma warning(push)
#pragma warning (disable:4324)  // structure was padded due to __declspec(align())
#endif //_MSC_VER


namespace Theron
{


/**
\brief Static class that manages allocators for use by Theron.

Static \ref SetAllocator and \ref GetAllocator methods on the class
allow the allocator used by Theron to be set and retrieved. Setting the allocator
replaces the \ref DefaultAllocator, which is used if no custom allocator is explicitly
set. The \ref GetAllocator method returns a pointer to the currently set allocator,
which is either the allocator set previously using \ref SetAllocator or an instance
of \ref DefaultAllocator, if none has been set.

\code
class MyAllocator : public Theron::IAllocator
{
public:

    MyAllocator();
    virtual ~MyAllocator();

    virtual void *Allocate(const SizeType size);
    virtual void *AllocateAligned(const SizeType size, const SizeType alignment);
    virtual void Free(void *const memory);
    virtual void Free(void *const memory, const SizeType size);
};

MyAllocator allocator;
Theron::AllocatorManager::SetAllocator(&allocator);
\endcode

\note The \ref SetAllocator method can be called at most once, at application start.
If the \ref DefaultAllocator is replaced with a custom allocator then it must be
replaced at application start, before any Theron objects (\ref EndPoint "endpoints",
\ref Framework "frameworks", \ref Actor "actors" or \ref Receiver "receivers") are constructed.
\ref GetAllocator can be called any number of times after \ref SetAllocator is called.
*/
class AllocatorManager
{
public:

    /**
    \brief Sets the allocator used for internal allocations, replacing the default allocator.

    Calling this method allows applications to provide custom allocators and hence to control
    how and where memory is allocated by Theron. This is useful, for example, in embedded systems
    where memory is scarce or of different available types. The allocator provided to this method
    must implement IAllocator and can be a wrapper around another existing allocator implementation.
    The provided allocator is used for all internal heap allocations, including the allocation of
    instantiated actors. If this method is not called by user code then a default DefaultAllocator
    is used, which is a simple wrapper around global new and delete.

    \code
    MyAllocator allocator;
    Theron::AllocatorManager::SetAllocator(&allocator);
    \endcode
    
    \note This method can't be called at static construction time.

    \see GetAllocator
    */
    static void SetAllocator(IAllocator *const allocator);

    /**
    \brief Gets a pointer to the general allocator currently in use by Theron.

    \code
    Theron::IAllocator *const allocator = Theron::AllocatorManager::GetAllocator();
    Theron::DefaultAllocator *const defaultAllocator = dynamic_cast<Theron::DefaultAllocator *>(allocator);

    if (defaultAllocator)
    {
        printf("Default allocator has %d bytes currently allocated\n", defaultAllocator->GetBytesAllocated());
    }
    \endcode

    \see SetAllocator
    */
    THERON_FORCEINLINE static IAllocator *GetAllocator()
    {
        return smCache.GetAllocator();
    }

    /**
    \brief Gets a pointer to the caching allocator that wraps the general allocator.

    Theron caches allocations internally using a caching allocator that wraps the low-level general allocator.
    The caching can't be replaced.

    \note Although this method is public, calling it is not recommended since it is really
    an implementation detail and may change in future releases. It would be made private if
    that weren't made difficult by the need to forward-declare loads of other classes in
    order to declare them friends of AllocatorManager.
    */
    THERON_FORCEINLINE static IAllocator *GetCache()
    {
        return &smCache;
    }

private:

    struct CacheTraits
    {
        typedef Detail::SpinLock LockType;

        struct THERON_PREALIGN(THERON_CACHELINE_ALIGNMENT) AlignType
        {
        } THERON_POSTALIGN(THERON_CACHELINE_ALIGNMENT);

        static const uint32_t MAX_POOLS = 8;
        static const uint32_t MAX_BLOCKS = 16;
    };

    typedef Detail::CachingAllocator<CacheTraits> CacheType;

    THERON_FORCEINLINE AllocatorManager()
    {
    }

    AllocatorManager(const AllocatorManager &other);
    AllocatorManager &operator=(const AllocatorManager &other);

    static DefaultAllocator smDefaultAllocator;     ///< Default allocator used if no user allocator is set.
    static CacheType smCache;                       ///< Cache that caches allocations from the actual allocator.
};


} // namespace Theron


#ifdef _MSC_VER
#pragma warning(pop)
#endif //_MSC_VER


#endif // THERON_ALLOCATORMANAGER_H
