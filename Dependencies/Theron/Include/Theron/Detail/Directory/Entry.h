// Copyright (C) by Ashton Mason. See LICENSE.txt for licensing information.
#ifndef THERON_DETAIL_DIRECTORY_ENTRY_H
#define THERON_DETAIL_DIRECTORY_ENTRY_H


#include <Theron/Assert.h>
#include <Theron/BasicTypes.h>
#include <Theron/Defines.h>

#include <Theron/Detail/Threading/SpinLock.h>


namespace Theron
{
namespace Detail
{


/**
An entry in the directory, potentially recording the registration of one entity.
*/
class Entry
{
public:

    /**
    A non-copyable entity that can be registered in a directory.
    In order to be registered in the directory, types must derive from this class.
    */
    class Entity
    {
    public:

        /**
        \brief Default constructor.
        */
        inline Entity()
        {
        }

    private:

        Entity(const Entity &other);
        Entity &operator=(const Entity &other);
    };

    /**
    Default constructor.
    */
    inline Entry() :
      mSpinLock(),
      mEntity(0),
      mPinCount(0)
    {
    }

    /**
    Lock the entry, acquiring exclusive access.
    */
    inline void Lock() const;

    /**
    Unlock the entry, relinquishing exclusive access.
    */
    inline void Unlock() const;

    /**
    Deregisters any entity registered at this entry.
    */
    inline void Free();

    /**
    Registers the given entity at this entry.
    */
    inline void SetEntity(Entity *const entity);

    /**
    Returns a pointer to the entity registered at this entry.
    \return A pointer to the registered entity, or zero if no entity is registered.
    */
    inline Entity *GetEntity() const;

    /**
    Pins the entry, preventing the registered entry from being changed.
    */
    inline void Pin();

    /**
    Unpins the entry, allowed the registered entry to be changed.
    */
    inline void Unpin();

    /**
    Returns true if the entry has been pinned more times than unpinned.
    */
    inline bool IsPinned() const;

private:

    Entry(const Entry &other);
    Entry &operator=(const Entry &other);

    mutable SpinLock mSpinLock;                 ///< Thread synchronization object protecting the entry.
    Entity *mEntity;                            ///< Pointer to the registered entity.
    uint32_t mPinCount;                         ///< Number of times this entity has been pinned and not unpinned.
};


THERON_FORCEINLINE void Entry::Lock() const
{
    mSpinLock.Lock();
}


THERON_FORCEINLINE void Entry::Unlock() const
{
    mSpinLock.Unlock();
}


THERON_FORCEINLINE void Entry::Free()
{
    THERON_ASSERT(mPinCount == 0);
    mEntity = 0;
}


THERON_FORCEINLINE void Entry::SetEntity(Entity *const entity)
{
    THERON_ASSERT(mPinCount == 0);
    mEntity = entity;
}


THERON_FORCEINLINE Entry::Entity *Entry::GetEntity() const
{
    return mEntity;
}


THERON_FORCEINLINE void Entry::Pin()
{
    ++mPinCount;
}


THERON_FORCEINLINE void Entry::Unpin()
{
    THERON_ASSERT(mPinCount > 0);
    --mPinCount;
}


THERON_FORCEINLINE bool Entry::IsPinned() const
{
    return (mPinCount > 0);
}


} // namespace Detail
} // namespace Theron


#endif // THERON_DETAIL_DIRECTORY_ENTRY_H

