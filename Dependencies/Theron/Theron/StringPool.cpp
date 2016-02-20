// Copyright (C) by Ashton Mason. See LICENSE.txt for licensing information.


#include <Theron/Assert.h>
#include <Theron/Defines.h>

#include <Theron/Detail/Strings/StringPool.h>
#include <Theron/Detail/Threading/Lock.h>


#ifdef _MSC_VER
#pragma warning(push)
#pragma warning (disable:4996)  // function or variable may be unsafe.
#endif //_MSC_VER


namespace Theron
{
namespace Detail
{


StringPool *StringPool::smInstance = 0;
Mutex StringPool::smReferenceMutex;
uint32_t StringPool::smReferenceCount = 0;


void StringPool::Reference()
{
    Lock lock(smReferenceMutex);

    // Create the singleton instance if this is the first reference.
    if (smReferenceCount++ == 0)
    {
        IAllocator *const allocator(AllocatorManager::GetCache());
        void *const memory(allocator->AllocateAligned(sizeof(StringPool), THERON_CACHELINE_ALIGNMENT));
        smInstance = new (memory) StringPool();
    }
}


void StringPool::Dereference()
{
    Lock lock(smReferenceMutex);

    // Destroy the singleton instance if this was the last reference.
    if (--smReferenceCount == 0)
    {
        IAllocator *const allocator(AllocatorManager::GetCache());
        smInstance->~StringPool();
        allocator->Free(smInstance, sizeof(StringPool));
    }
}


StringPool::StringPool()
{
}


StringPool::~StringPool()
{
}


const char *StringPool::Lookup(const char *const str)
{
    // Hash the string value to a bucket index.
    const uint32_t index(Hash(str));
    THERON_ASSERT(index < BUCKET_COUNT);
    Bucket &bucket(mBuckets[index]);

    // Lock the mutex after computing the hash.
    Lock lock(mMutex);
    return bucket.Lookup(str);
}


} // namespace Detail
} // namespace Theron


#ifdef _MSC_VER
#pragma warning(pop)
#endif //_MSC_VER
