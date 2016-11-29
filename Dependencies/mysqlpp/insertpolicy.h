/// \file insertpolicy.h
/// \brief Declares the InsertPolicy classes.
///
/// One creates an insert policy object to control how
/// Query::insertfrom() builds queries from collections of SSQLS
/// objects.  Policy objects are what cause Query::insertfrom() to
/// have different insertion behavior than Query::insert(iter, iter)
///
/// These templates use a class called the AccessController, which
/// is a stand-in for the mysqlpp::Transaction class and defaults to 
/// the Transaction class.  Since some of the InsertPolicy objects
/// (SizeThresholdInsertPolicy and MaxPacketInsertPolicy) may cause 
/// Query::insertfrom() to issue multiple SQL statements to perform
/// the insertion of all the objects in the container, and a failure in
/// one of these statements would leave the table in an indeterminate
/// state, the whole operation is wrapped in a transaction.
///
/// However, a transaction may not be desired if the calling code
/// is managing transactions, or transactions are not being used for 
/// some other reason.  In this case, the template can be instantiated
/// with the NoTransaction class.  It provides the complete Transaction
/// class interface, while doing nothing.
///
/// Where possible, you should use one of the provided insert
/// policy classes, but you can define your own if you need a behavior
/// that the provided set doesn't include.
/// 
/// This file is not meant to be included in end-user code.  It's
/// included in Query's public interface, since it is only used with
/// Query::insertfrom().  You access it as Query::InsertPolicy<T>

/***********************************************************************
 Copyright (c) 2008-2009 by AboveNet, Inc., and (c) 2009 by Educational
 Technology Resources, Inc.  Others may also hold copyrights on code
 in this file.  See the CREDITS file in the top directory of the
 distribution for details.

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

#if !defined(MYSQLPP_INSERTPOLICY_H)
#define MYSQLPP_INSERTPOLICY_H

/// \brief An insert policy object that triggers a new INSERT
/// statement after a given number of rows have been inserted.
///
/// This policy is very lightweight, but is only reliable when you
/// can predict the size of each INSERT in advance.  The others do
/// more processing to reduce the risk of unpredictable row sizes.
template <class AccessController = Transaction>
class MYSQLPP_EXPORT RowCountInsertPolicy
{
public:
	/// \brief Constructor
	RowCountInsertPolicy(unsigned int rows) :
	cur_rows_(0),
	max_rows_(rows)
	{
	}

	/// \brief Destructor
	~RowCountInsertPolicy() { }

	/// \brief Can we add another object to the query?
	///
	/// \retval true if the object is allowed to be added to the
	/// INSERT statement
	template <class RowT>
	bool can_add(int, const RowT&)
	{
		if (++cur_rows_ > max_rows_) {
			cur_rows_ = 0;
			return false;
		}
		else {
			return true;
		}
	}

	/// \brief Alias for our access controller type
	typedef AccessController access_controller;

private:
	unsigned int cur_rows_;
	unsigned const int max_rows_;
};


/// \brief An insert policy object that triggers a new INSERT statement
/// after a size threshold for the length of the INSERT statement
/// is exceeded.
///
/// Although this insert policy isn't completely deterministic, it
/// avoids building the VALUES expression for the SSQLS object
/// passed in.
template <class AccessController = Transaction>
class MYSQLPP_EXPORT SizeThresholdInsertPolicy
{
public:
	/// \brief Constructor
	SizeThresholdInsertPolicy(int size) :
	size_(size)
	{
	}

	/// \brief Destructor
	~SizeThresholdInsertPolicy() { }

	/// \brief Can we add another object to the query?
	///
	/// \param size current length of the INSERT statement
	/// \param object the SSQLS object to be added
	///
	/// \retval true if the object is allowed to be added to the
	/// INSERT statement
	template <class RowT>
	bool can_add(int size, const RowT& object) const
	{
		return (size < size_);
	}

	/// \brief Alias for our access controller type
	typedef AccessController access_controller;

private:
	int size_;
};


/// \brief An insert policy object that triggers a new INSERT statement
/// if the object to be added would cause the statement to exceed
/// a maximum size.
///
/// This differs from the SizeThresholdInsertPolicy in that it builds
/// the VALUES expression and checks whether it would cause the
/// length of the INSERT statement to exceed the maximum size.
template <class AccessController = Transaction>
class MYSQLPP_EXPORT MaxPacketInsertPolicy
{
public:
	/// \brief Constructor
	///
	/// \param con connection object used for escaping text
	/// \param size the maximum allowed size for an INSERT
	///     statement
	MaxPacketInsertPolicy(Connection* con, int size) :
	conn_(con), size_(size)
	{
	}

	/// \brief Constructor
	///
	/// This version does not use a Connection* so it will not be
	/// able to take the character set into account when escaping
	/// the text.
	///
	/// \param size the maximum allowed size for an INSERT
	///     statement
	MaxPacketInsertPolicy(int size) :
	conn_(0), size_(size)
	{
	}

	/// \brief Destructor
	~MaxPacketInsertPolicy() { }

	/// \brief Can we add another object to the query?
	///
	/// \param size current length of the INSERT statement
	/// \param object the SSQLS object to be added
	///
	/// \retval true if the object is allowed to be added to the
	///     INSERT statement
	template <class RowT>
	bool can_add(int size, const RowT& object) const
	{
		if (size < size_) {
			// Haven't hit size threshold yet, so see if this next
			// item pushes it over the line.
			SQLStream s(conn_);
			s << ",(" << object.value_list() << ")";
			return (size_ - size) >= static_cast<int>(s.str().size());
		}
		else {
			// Already too much in query buffer!
			return false;
		}
	}

	/// \brief Alias for our access controller type
	typedef AccessController access_controller;

private:
	Connection* conn_;
	int size_;
};

#endif // !defined(MYSQLPP_INSERTPOLICY_H)

