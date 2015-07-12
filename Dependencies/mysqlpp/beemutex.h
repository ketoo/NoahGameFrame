/// \file beemutex.h
/// \brief MUTually EXclusive lock class.
/// \author Bob Deblier <bob.deblier@telenet.be>
///
/// Modified by Warren Young of Educational Technology Resources, Inc.
/// from version in Beecrypt 4.1.2:
///
/// - minor style changes to make it fit within MySQL++
/// - changed init() to a ctor and destroy() to a dtor
/// - class just becomes a no-op if no supported mutex type is available
/// - throwing MutexFailed instead of \c char*
/// - moved all method implementations from inline in the .h file to
///   a .cpp file so we don't have to make the header depend on config.h
///   on autoconf-using systems
/// - made private mutex member a void* so we don't have to define the
///   full type in the .h file, due to previous item
/// - added more Doxygen comments, and changed some existing comments

/***********************************************************************
 Copyright (c) 2004 Beeyond Software Holding BV and (c) 2007-2008
 by Educational Technology Resources, Inc.  Others may also hold
 copyrights on code in this file.  See the CREDITS.txt file in the
 top directory of the distribution for details.

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

#if !defined(MYSQLPP_BEEMUTEX_H)
#define MYSQLPP_BEEMUTEX_H

#include "exceptions.h"

namespace mysqlpp {

/// \brief Wrapper around platform-specific mutexes.
///
/// This class is only intended to be used within the library.  We don't
/// really want to support this as a general purpose class.  If it works
/// for you as-is, that's great, we won't try to stop you.  But if you
/// run into a problem that doesn't affect MySQL++ itself, we're not
/// likely to bother enhancing this class to fix the problem.
class MYSQLPP_EXPORT BeecryptMutex
{
public:
	/// \brief Create the mutex object
	///
	/// Throws a MutexFailed exception if we can't acquire the lock for
	/// some reason.  The exception contains a message saying why.
	BeecryptMutex() throw (MutexFailed);

	/// \brief Destroy the mutex
	///
	/// Failures are quietly ignored.
	~BeecryptMutex();

	/// \brief Acquire the mutex, blocking if it can't be acquired
	/// immediately.
	void lock() throw (MutexFailed);

	/// \brief Acquire the mutex immediately and return true, or return
	/// false if it would have to block to acquire the mutex.
	bool trylock() throw (MutexFailed);

	/// \brief Release the mutex
	void unlock() throw (MutexFailed);

private:
	void* pmutex_;
};


/// \brief Wrapper around BeecryptMutex to add scope-bound locking
/// and unlocking.
///
/// This allows code to lock a mutex and ensure it will unlock on exit
/// from the enclosing scope even in the face of exceptions.  This is
/// separate from BeecryptMutex because we don't want to make this
/// behavior mandatory.

class ScopedLock
{
public:
	/// \brief Lock the mutex.
	explicit ScopedLock(BeecryptMutex& mutex) :
	mutex_(mutex)
	{
		mutex.lock();
	}

	/// \brief Unlock the mutex.
	~ScopedLock() { mutex_.unlock(); }

private:
	ScopedLock(const ScopedLock&);				// can't copy
	ScopedLock& operator =(const ScopedLock&);	// can't assign

	BeecryptMutex& mutex_;	///< the mutex object we manage
};

} // end namespace mysqlpp

#endif // !defined(MYSQLPP_BEEMUTEX_H)

