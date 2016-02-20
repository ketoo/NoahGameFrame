// Copyright (C) by Ashton Mason. See LICENSE.txt for licensing information.
#ifndef THERON_DETAIL_STRINGS_STRINGPOOL_H
#define THERON_DETAIL_STRINGS_STRINGPOOL_H


#include <new>

#include <string.h>
#include <stdlib.h>

#include <Theron/Align.h>
#include <Theron/AllocatorManager.h>
#include <Theron/Assert.h>
#include <Theron/BasicTypes.h>
#include <Theron/Defines.h>
#include <Theron/IAllocator.h>

#include <Theron/Detail/Containers/List.h>
#include <Theron/Detail/Threading/Mutex.h>


#ifdef _MSC_VER
#pragma warning(push)
#pragma warning (disable:4996)  // function or variable may be unsafe.
#endif //_MSC_VER


namespace Theron
{
namespace Detail
{


/**
Static class that manages a pool of unique strings.
*/
class StringPool
{
public:

    /**
    Holds a reference to the string pool, ensuring that it has been created.
    */
    class Ref
    {
    public:

        inline Ref()
        {
            StringPool::Reference();
        }

        inline ~Ref()
        {
            StringPool::Dereference();
        }
    };

    friend class Ref;

    /**
    Gets the address of the pooled version of the given literal string.
    The pooled version is created if it doesn't already exist.
    */
    inline static const char *Get(const char *const str);

private:

    class Entry : public List<Entry>::Node
    {
    public:

        THERON_FORCEINLINE static uint32_t GetSize(const char *const str)
        {
            const uint32_t length(static_cast<uint32_t>(strlen(str)));
            uint32_t lengthWithNull(length + 1);
            const uint32_t roundedLength(THERON_ROUNDUP(lengthWithNull, 4));

            return sizeof(Entry) + roundedLength;
        }

        THERON_FORCEINLINE static Entry *Initialize(void *const memory, const char *const str)
        {
            char *const buffer(reinterpret_cast<char *>(memory) + sizeof(Entry));
            strcpy(buffer, str);

            return new (memory) Entry(buffer);
        }

        THERON_FORCEINLINE explicit Entry(const char *const val) : mValue(val)
        {
        }

        THERON_FORCEINLINE const char *Value() const
        {
            return mValue;
        }

    private:

        const char *mValue;
    };

    class Bucket
    {
    public:

        inline Bucket()
        {
        }

        inline ~Bucket()
        {
            IAllocator *const allocator(AllocatorManager::GetCache());

            // Free all entries at end of day.
            while (!mEntries.Empty())
            {
                Entry *const entry(mEntries.Front());
                mEntries.Remove(entry);

                const uint32_t size(Entry::GetSize(entry->Value()));
                entry->~Entry();
                allocator->Free(entry, size);
            }
        }

        THERON_FORCEINLINE const char *Lookup(const char *const str)
        {
            IAllocator *const allocator(AllocatorManager::GetCache());
            Entry *entry(0);

            // Search the bucket for an an existing entry for this string.
            List<Entry>::Iterator entries(mEntries.GetIterator());
            while (entries.Next())
            {
                Entry *const e(entries.Get());
                if (strcmp(e->Value(), str) == 0)
                {
                    entry = e;
                    break;
                }
            }

            if (entry == 0)
            {
                // Create a new entry.
                const uint32_t size(Entry::GetSize(str));
                void *const memory(allocator->Allocate(size));
                entry = Entry::Initialize(memory, str);
                mEntries.Insert(entry);
            }

            return entry->Value();
        }

    private:

        List<Entry> mEntries;
    };

    /**
    References the string pool, creating the singleton instance if it doesn't already exist.
    */
    static void Reference();

    /**
    Releases a reference to the string pool, destroying the singleton instance if it is no longer referenced.
    */
    static void Dereference();

    inline static uint32_t Hash(const char *const str);

    static StringPool *smInstance;          ///< Pointer to the singleton instance.
    static Mutex smReferenceMutex;          ///< Synchronization object protecting reference counting.
    static uint32_t smReferenceCount;       ///< Counts the number of references to the singleton.

    static const uint32_t BUCKET_COUNT = 128;

    StringPool();
    ~StringPool();

    /**
    Finds the entry for a given string, and creates it if it doesn't exist yet.
    */
    const char *Lookup(const char *const str);

    Mutex mMutex;
    Bucket mBuckets[BUCKET_COUNT];
};


THERON_FORCEINLINE const char *StringPool::Get(const char *const str)
{
    THERON_ASSERT(smInstance);
    THERON_ASSERT(str);

    return smInstance->Lookup(str);
}


THERON_FORCEINLINE uint32_t StringPool::Hash(const char *const str)
{
    THERON_ASSERT(str);

    // XOR the first n characters of the string together.
    const char *const end(str + 64);

    const char *ch(str);
    uint8_t hash(0);

    while (ch != end && *ch != '\0')
    {
        hash ^= static_cast<uint8_t>(*ch);
        ++ch;
    }

    // Zero the 8th bit since it's usually zero anyway in ASCII.
    // This gives an effective range of [0, 127].
    hash &= 127;

    return static_cast<uint32_t>(hash);
}


} // namespace Detail
} // namespace Theron


#ifdef _MSC_VER
#pragma warning(pop)
#endif //_MSC_VER


#endif // THERON_DETAIL_STRINGS_STRINGPOOL_H

