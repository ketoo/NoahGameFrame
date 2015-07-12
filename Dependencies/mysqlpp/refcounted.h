/// \file refcounted.h
/// \brief Declares the RefCountedPointer template

/***********************************************************************
 Copyright (c) 2007-2011 by Educational Technology Resources, Inc. and
 (c) 2007 by Jonathan Wakely.  Others may also hold copyrights on
 code in this file.  See the CREDITS.txt file in the top directory
 of the distribution for details.

 This file is part of MySQL++.

 MySQL++ is free software; you can redistribute it and/or modify it
 under the terms of the GNU Lesser General Public License as published
 by the Free Software Foundation; either version 2.1 of the License, or
 (at your option) any later version.

 MySQL++ is distributed in the hope that it will be useful, but WITHOUT
 ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
 License for more details.

 You should have received a copy of the GNU Lesser General Public
 License along with MySQL++; if not, write to the Free Software
 Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301
 USA
***********************************************************************/

#if !defined(MYSQLPP_REFCOUNTED_H)
#define MYSQLPP_REFCOUNTED_H

#include <memory>

#include <stddef.h>

namespace mysqlpp {

/// \brief Functor to call delete on the pointer you pass to it
///
/// The default "destroyer" for RefCountedPointer.  You won't use this
/// directly, you'll pass a functor of your own devising for the second
/// parameter to the RefCountedPointer template to override this.  Or
/// simpler, just specialize this template for your type if possible:
/// see ResUse::result_.
template <class T>
struct RefCountedPointerDestroyer
{
	/// \brief Functor implementation
	void operator()(T* doomed) const { delete doomed; }
};


/// \brief Creates an object that acts as a reference-counted pointer
/// to another object.
///
/// Resulting type acts like a pointer in all respects, except that it
/// manages the memory it points to by observing how many users there
/// are for the object.
///
/// This attempts to be as automatic as reference counting in a
/// programming language with memory management.  Like all automatic
/// memory management schemes, it has penalties: it turns the single
/// indirection of an unmanaged pointer into a double indirection, and
/// has additional management overhead in the assignment operators due
/// to the reference counter.  This is an acceptable tradeoff when
/// wrapping objects that are expensive to copy, and which need to
/// be "owned" by disparate parties: you can allocate the object just
/// once, then pass around the reference counted pointer, knowing that
/// the last user will "turn out the lights".
///
/// \b Implementation \b detail: You may notice that this class manages
/// two pointers, one to the data we're managing, and one to the
/// reference count.  You might wonder why we don't wrap these up into a
/// structure and keep just a pointer to an instance of it to simplify
/// the memory management.  It would indeed do that, but then every
/// access to the data we manage would be a triple indirection instead
/// of just double.  It's a tradeoff, and we've chosen to take a minor
/// complexity hit to avoid the performance hit.

template <class T, class Destroyer = RefCountedPointerDestroyer<T> >
class RefCountedPointer
{
public:
	typedef RefCountedPointer<T> ThisType;	///< alias for this object's type

	/// \brief Default constructor
	///
	/// An object constructed this way is useless until you vivify it
	/// with operator =() or assign().
	RefCountedPointer() :
	counted_(0),
	refs_(0)
	{
	}
	
	/// \brief Standard constructor
	///
	/// \param c A pointer to the object to be managed.  If you pass 0,
	/// it's like calling the default ctor instead, only more work: the
	/// object's useless until you vivify it with operator =() or assign().
	explicit RefCountedPointer(T* c) :
	counted_(c),
	refs_(0)
	{
		std::auto_ptr<T> exception_guard(counted_);
		if (counted_) {
			refs_ = new size_t(1);
		}
		exception_guard.release();	// previous new didn't throw
	}

	/// \brief Copy constructor
	RefCountedPointer(const ThisType& other) :
	counted_(other.counted_),
	refs_(other.counted_ ? other.refs_ : 0)
	{
		if (counted_) {
			++(*refs_);
		}
	}

	/// \brief Destructor
	///
	/// This only destroys the managed memory if the reference count
	/// drops to 0.
	~RefCountedPointer()
	{
		if (refs_ && (--(*refs_) == 0)) {
			Destroyer()(counted_);
			delete refs_;
		}
	}

	/// \brief Sets (or resets) the pointer to the counted object.
	///
	/// If we are managing a pointer, this decrements the refcount for
	/// it and destroys the managed object if the refcount falls to 0.
	///
	/// This is a no-op if you pass the same pointer we're already
	/// managing.
	ThisType& assign(T* c)
	{
		// The create-temporary-and-swap idiom lets us keep memory
		// allocation in the ctor and deallocation in the dtor so
		// we don't leak in the face of an exception.
		ThisType(c).swap(*this);
		return *this;
	}

	/// \brief Copy an existing refcounted pointer
	///
	/// If we are managing a pointer, this decrements the refcount for
	/// it and destroys the managed object if the refcount falls to 0.
	/// Then we increment the other object's reference count and copy
	/// that refcount and the managed pointer into this object.
	///
	/// This is a no-op if you pass a reference to this same object.
	ThisType& assign(const ThisType& other)
	{
		// The create-temporary-and-swap idiom lets us keep memory
		// allocation in the ctor and deallocation in the dtor so
		// we don't leak in the face of an exception.
		ThisType(other).swap(*this);
		return *this;
	}

	/// \brief Set (or reset) the pointer to the counted object
	///
	/// This is essentially the same thing as assign(T*).  The choice
	/// between the two is just a matter of syntactic preference.
	ThisType& operator =(T* c)
	{
		return assign(c);
	}

	/// \brief Copy an existing refcounted pointer
	///
	/// This is essentially the same thing as assign(const ThisType&).
	/// The choice between the two is just a matter of syntactic
	/// preference.
	ThisType& operator =(const ThisType& rhs)
	{
		return assign(rhs);
	}

	/// \brief Access the object through the smart pointer
	T* operator ->() const
	{
		return counted_;
	}	

	/// \brief Dereference the smart pointer
	T& operator *() const
	{
		return *counted_;
	}	

	/// \brief Returns the internal raw pointer converted to void*
	///
	/// This isn't intended to be used directly; if you need the
	/// pointer, call raw() instead.  It's used internally by the
	/// compiler to implement operators bool, ==, and !=
	///
	/// \b WARNING: This makes it possible to say
	/// \code
	/// RefCountedPointer<Foo> bar(new Foo);
	/// delete bar;
	/// \endcode
	///
	/// This will almost kinda sorta do the right thing: the Foo
	/// object held by the refcounted pointer will be destroyed as
	/// you wanted, but then when the refcounted pointer goes out of
	/// scope, the memory is deleted a second time, which will probably
	/// crash your program.  This is easy to accidentally do when
	/// converting a good ol' unmanaged pointer to a refcounted pointer
	/// and forgetting to remove the delete calls needed previously.
	operator void*()
	{
		return counted_;
	}

	/// \brief Returns the internal raw pointer converted to const void*
	///
	/// \see comments for operator void*()
	operator const void*() const
	{
		return counted_;
	}

	/// \brief Return the raw pointer in T* context
	T* raw()
	{
		return counted_;
	}

	/// \brief Return the raw pointer when used in const T* context
	const T* raw() const
	{
		return counted_;
	}

	/// \brief Exchange our managed memory with another pointer.
	///
	/// \internal This exists primarily to implement assign() in an
	/// exception-safe manner.
	void swap(ThisType& other)
	{
		std::swap(counted_, other.counted_);
		std::swap(refs_, other.refs_);
	}	

private:
	/// \brief Pointer to the reference-counted object
	T* counted_;

	/// \brief Pointer to the reference count.
	///
	/// We can't keep this as a plain integer because this object
	/// allows itself to be copied.  All copies need to share this
	/// reference count, not just the pointer to the counted object.
	size_t* refs_;
};


} // end namespace mysqlpp

#endif // !defined(MYSQLPP_REFCOUNTED_H)

