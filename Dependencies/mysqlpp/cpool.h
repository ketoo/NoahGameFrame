/// \file cpool.h
/// \brief Declares the ConnectionPool class.

/***********************************************************************
 Copyright (c) 2007-2008 by Educational Technology Resources, Inc. and
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

#if !defined(MYSQLPP_CPOOL_H)
#define MYSQLPP_CPOOL_H

#include "beemutex.h"

#include <list>

#include <assert.h>
#include <time.h>

namespace mysqlpp {

#if !defined(DOXYGEN_IGNORE)
// Make Doxygen ignore this
class MYSQLPP_EXPORT Connection;
#endif

/// \brief Manages a pool of connections for programs that need more
/// than one Connection object at a time, but can't predict how many
/// they need in advance.
///
/// This class is useful in programs that need to make multiple
/// simultaneous queries on the database; this requires multiple
/// Connection objects due to a hard limitation of the underlying
/// C API.  Connection pools are most useful in multithreaded programs,
/// but it can be helpful to have one in a single-threaded program as
/// well.  Sometimes it's necessary to get more data from the server
/// while in the middle of processing data from an earlier query; this
/// requires multiple connections.  Whether you use a pool or manage
/// connections yourself is up to you, but realize that this class
/// takes care of a lot of subtle details for you that aren't obvious.
///
/// The pool's policy for connection reuse is to always return the 
/// \em most recently used connection that's not being used right now.
/// This ensures that excess connections don't hang around any longer
/// than they must.  If the pool were to return the \em least recently
/// used connection, it would be likely to result in a large pool of
/// sparsely used connections because we'd keep resetting the last-used 
/// time of whichever connection is least recently used at that moment.

class MYSQLPP_EXPORT ConnectionPool
{
public:
	/// \brief Create empty pool
	ConnectionPool() { }

	/// \brief Destroy object
	///
	/// If the pool raises an assertion on destruction, it means our
	/// subclass isn't calling clear() in its dtor as it should.
	virtual ~ConnectionPool() { assert(empty()); }

	/// \brief Returns true if pool is empty
	bool empty() const { return pool_.empty(); }

	/// \brief Return a defective connection to the pool and get a new
	/// one back.
	///
	/// Call this on receiving a BadQuery exception, with errnum()
	/// equal to CR_SERVER_GONE_ERROR.  It means the server was
	/// restarted or otherwise dropped your connection to it, so the
	/// Connection object is no longer usable.  You can avoid the
	/// need to use this by setting the ReconnectOption in your grab()
	/// override, but perhaps there are other reasons to need to
	/// exchange a bad connection for a good one.
	///
	/// This function wraps grab(), not safe_grab(), even though that
	/// could return another dead connection.  The assumption is that if
	/// your code is smart enough to detect one bad connection, it should
	/// be smart enough to detect a whole string of them.  Worst case,
	/// the whole pool is bad -- remote server went away -- and we have
	/// to empty the pool and start re-filling it.
	///
	/// \param pc pointer to a Connection object to be returned to the
	/// pool and marked as unused.
	///
	/// \retval a pointer to a different Connection object; not
	/// guaranteed to still be connected!
	virtual Connection* exchange(const Connection* pc);

	/// \brief Grab a free connection from the pool.
	///
	/// This method creates a new connection if an unused one doesn't
	/// exist, and destroys any that have remained unused for too long.
	/// If there is more than one free connection, we return the most
	/// recently used one; this allows older connections to die off over
	/// time when the caller's need for connections decreases.
	///
	/// Do not delete the returned pointer.  This object manages the
	/// lifetime of connection objects it creates.
	///
	/// \retval a pointer to the connection
	virtual Connection* grab();

	/// \brief Return a connection to the pool
	///
	/// Marks the connection as no longer in use.
	///
	/// The pool updates the last-used time of a connection only on
	/// release, on the assumption that it was used just prior.  There's
	/// nothing forcing you to do it this way: your code is free to
	/// delay releasing idle connections as long as it likes.  You
	/// want to avoid this because it will make the pool perform poorly;
	/// if it doesn't know approximately how long a connection has
	/// really been idle, it can't make good judgements about when to
	/// remove it from the pool.
	///
	/// \param pc pointer to a Connection object to be returned to the
	/// pool and marked as unused.
	virtual void release(const Connection* pc);

	/// \brief Removes the given connection from the pool
	///
	/// If you mean to simply return a connection to the pool after
	/// you're finished using it, call release() instead.  This method
	/// is primarily for error handling: you somehow have figured out
	/// that the connection is defective, so want it destroyed and
	/// removed from the pool.  If you also want a different connection
	/// to retry your operation on, call exchange() instead.
	///
	/// \param pc pointer to a Connection object to be removed from
	/// the pool and destroyed
	void remove(const Connection* pc);

	/// \brief Grab a free connection from the pool, testing that it's
	/// connected before returning it.
	///
	/// This is just a wrapper around grab(), Connection::ping() and
	/// release(), and is thus less efficient than grab().  Use it only
	/// when it's possible for MySQL server connections to go away
	/// unexpectedly, such as when the DB server can be restarted out
	/// from under your application.
	///
	/// \retval a pointer to the connection
	virtual Connection* safe_grab();

	/// \brief Remove all unused connections from the pool
	void shrink() { clear(false); }

protected:
	/// \brief Drains the pool, freeing all allocated memory.
	///
	/// A derived class must call this in its dtor to avoid leaking all
	/// Connection objects still in existence.  We can't do it up at
	/// this level because this class's dtor can't call our subclass's
	/// destroy() method.
	///
	/// \param all if true, remove all connections, even those in use
	void clear(bool all = true);

	/// \brief Create a new connection
	///
	/// Subclasses must override this.
	///
	/// Essentially, this method lets your code tell ConnectionPool
	/// what server to connect to, what login parameters to use, what
	/// connection options to enable, etc.  ConnectionPool can't know
	/// any of this without your help.
	///
	/// \retval A connected Connection object
	virtual Connection* create() = 0;

	/// \brief Destroy a connection
	///
	/// Subclasses must override this.
	///
	/// This is for destroying the objects returned by create().
	/// Because we can't know what the derived class did to create the
	/// connection we can't reliably know how to destroy it.
	virtual void destroy(Connection*) = 0;

	/// \brief Returns the maximum number of seconds a connection is
	/// able to remain idle before it is dropped.
	///
	/// Subclasses must override this as it encodes a policy issue,
	/// something that MySQL++ can't declare by fiat.
	///
	/// \retval number of seconds before an idle connection is destroyed
	/// due to lack of use
	virtual unsigned int max_idle_time() = 0;

	/// \brief Returns the current size of the internal connection pool.
	size_t size() const { return pool_.size(); }

private:
	//// Internal types
	struct ConnectionInfo {
		Connection* conn;
		time_t last_used;
		bool in_use;

		ConnectionInfo(Connection* c) :
		conn(c),
		last_used(time(0)),
		in_use(true)
		{
		}

		// Strict weak ordering for ConnectionInfo objects.
		// 
		// This ordering defines all in-use connections to be "less
		// than" those not in use.  Within each group, connections
		// less recently touched are less than those more recent.
		bool operator<(const ConnectionInfo& rhs) const
		{
			const ConnectionInfo& lhs = *this;
			return lhs.in_use == rhs.in_use ?
					lhs.last_used < rhs.last_used :
					lhs.in_use;
		}
	};
	typedef std::list<ConnectionInfo> PoolT;
	typedef PoolT::iterator PoolIt;

	//// Internal support functions
	Connection* find_mru();
	void remove(const PoolIt& it);
	void remove_old_connections();

	//// Internal data
	PoolT pool_;
	BeecryptMutex mutex_;
};

} // end namespace mysqlpp

#endif // !defined(MYSQLPP_CPOOL_H)

