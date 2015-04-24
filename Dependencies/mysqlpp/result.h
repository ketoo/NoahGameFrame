/// \file result.h
/// \brief Declares classes for holding information about SQL query
/// results.

/***********************************************************************
 Copyright (c) 1998 by Kevin Atkinson, (c) 1999-2001 by MySQL AB, and
 (c) 2004-2007 by Educational Technology Resources, Inc.  Others may
 also hold copyrights on code in this file.  See the CREDITS.txt file
 in the top directory of the distribution for details.

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

#if !defined(MYSQLPP_RESULT_H)
#define MYSQLPP_RESULT_H

#include "common.h"

#include "exceptions.h"
#include "field.h"
#include "field_names.h"
#include "field_types.h"
#include "noexceptions.h"
#include "refcounted.h"
#include "row.h"

namespace mysqlpp {


/// \brief Holds information about the result of queries that don't
/// return rows.

class MYSQLPP_EXPORT SimpleResult
{
private:
	/// \brief Pointer to bool data member, for use by safe bool
	/// conversion operator.
	///
	/// \see http://www.artima.com/cppsource/safebool.html
	typedef bool SimpleResult::*private_bool_type;

public:
	/// \brief Default ctor
	SimpleResult() :
	copacetic_(false),
	insert_id_(0),
	rows_(0)
	{
	}

	/// \brief Initialize object
	SimpleResult(bool copacetic, ulonglong insert_id,
			ulonglong rows, const std::string& info) :
	copacetic_(copacetic),
	insert_id_(insert_id),
	rows_(rows),
	info_(info)
	{
	}

	/// \brief Test whether the query that created this result succeeded
	///
	/// If you test this object in bool context and it's false, it's a
	/// signal that the query this was created from failed in some way.
	/// Call Query::error() or Query::errnum() to find out what exactly
	/// happened.
	operator private_bool_type() const
	{
		return copacetic_ ? &SimpleResult::copacetic_ : 0;
	}

	/// \brief Get the last value used for an AUTO_INCREMENT field
	ulonglong insert_id() const { return insert_id_; }

	/// \brief Get the number of rows affected by the query
	ulonglong rows() const { return rows_; }

	/// \brief Get any additional information about the query returned
	/// by the server.
	const char* info() const { return info_.c_str(); }

private:
	bool copacetic_;
	ulonglong insert_id_;
	ulonglong rows_;
	std::string info_;
};


/// \brief Base class for StoreQueryResult and UseQueryResult.
///
/// Not useful directly.  Just contains common functionality for its
/// subclasses.

class MYSQLPP_EXPORT ResultBase : public OptionalExceptions
{
public:
	/// \brief Destroy object
	virtual ~ResultBase() { }

	/// \brief Returns the next field in this result set
	const Field& fetch_field() const
			{ return fields_.at(current_field_++); }

	/// \brief Returns the given field in this result set
	const Field& fetch_field(Fields::size_type i) const
			{ return fields_.at(i); }

	/// \brief Get the underlying Field structure given its index.
	const Field& field(unsigned int i) const { return fields_.at(i); }

	/// \brief Get the underlying Fields structure.
	const Fields& fields() const { return fields_; }

	/// \brief Get the name of the field at the given index.
	const std::string& field_name(int i) const
			{ return names_->at(i); }

	/// \brief Get the names of the fields within this result set.
	const RefCountedPointer<FieldNames>& field_names() const
			{ return names_; }

	/// \brief Get the index of the named field.
	///
	/// This is the inverse of field_name().
	int field_num(const std::string&) const;

	/// \brief Get the type of a particular field within this result set.
	const FieldTypes::value_type& field_type(int i) const
			{ return types_->at(i); }

	/// \brief Get a list of the types of the fields within this
	/// result set.
	const RefCountedPointer<FieldTypes>& field_types() const
			{ return types_; }

	/// \brief Returns the number of fields in this result set
	size_t num_fields() const { return fields_.size(); }

	/// \brief Return the name of the table the result set comes from
	const char* table() const
			{ return fields_.empty() ? "" : fields_[0].table(); }

protected:
	/// \brief Create empty object
	ResultBase() :
	driver_(0),
	current_field_(0)
	{
	}
	
	/// \brief Create the object, fully initialized
	ResultBase(MYSQL_RES* result, DBDriver* dbd, bool te = true);
	
	/// \brief Create object as a copy of another ResultBase
	ResultBase(const ResultBase& other) :
	OptionalExceptions()
	{
		copy(other);
	}

	/// \brief Copy another ResultBase object's contents into this one.
	ResultBase& copy(const ResultBase& other);

	DBDriver* driver_;	///< Access to DB driver; fully initted if nonzero
	Fields fields_;		///< list of fields in result

	/// \brief list of field names in result
	RefCountedPointer<FieldNames> names_;

	/// \brief list of field types in result
	RefCountedPointer<FieldTypes> types_;

	/// \brief Default field index used by fetch_field()
	///
	/// It's mutable because it's just internal housekeeping: it's
	/// changed by fetch_field(void), but it doesn't change the "value"
	/// of the result.  See mutability justification for
	/// UseQueryResult::result_: this field provides functionality we
	/// used to get through result_, so it's relevant here, too.
	mutable Fields::size_type current_field_;
};


/// \brief StoreQueryResult set type for "store" queries
///
/// This is the obvious C++ implementation of a class to hold results 
/// from a SQL query that returns rows: a specialization of std::vector
/// holding Row objects in memory so you get random-access semantics.
/// MySQL++ also supports UseQueryResult which is less friendly, but has
/// better memory performance.  See the user manual for more details on
/// the distinction and the usage patterns required.

class MYSQLPP_EXPORT StoreQueryResult :
		public ResultBase,
		public std::vector<Row>
{
private:
	/// \brief Pointer to bool data member, for use by safe bool
	/// conversion operator.
	///
	/// \see http://www.artima.com/cppsource/safebool.html
	typedef bool StoreQueryResult::*private_bool_type;

public:
	typedef std::vector<Row> list_type;	///< type of vector base class

	/// \brief Default constructor
	StoreQueryResult() :
	ResultBase(),
	copacetic_(false)
	{
	}
	
	/// \brief Fully initialize object
	StoreQueryResult(MYSQL_RES* result, DBDriver* dbd, bool te = true);

	/// \brief Initialize object as a copy of another StoreQueryResult
	/// object
	StoreQueryResult(const StoreQueryResult& other) :
	ResultBase(),
	std::vector<Row>(),
	copacetic_(false)
	{
		copy(other);
	}

	/// \brief Destroy result set
	~StoreQueryResult() { }

	/// \brief Returns the number of rows in this result set
	list_type::size_type num_rows() const { return size(); }

	/// \brief Copy another StoreQueryResult object's data into this
	/// object
	StoreQueryResult& operator =(const StoreQueryResult& rhs)
			{ return this != &rhs ? copy(rhs) : *this; }

	/// \brief Test whether the query that created this result succeeded
	///
	/// If you test this object in bool context and it's false, it's a
	/// signal that the query this was created from failed in some way.
	/// Call Query::error() or Query::errnum() to find out what exactly
	/// happened.
	operator private_bool_type() const
	{
		return copacetic_ ? &StoreQueryResult::copacetic_ : 0;
	}

private:
	/// \brief Copy another StoreQueryResult object's contents into this
	/// one.
	StoreQueryResult& copy(const StoreQueryResult& other);

	bool copacetic_;	///< true if initialized from a good result set
};


/// \brief Functor to call mysql_free_result() on the pointer you pass
/// to it.
///
/// This overrides RefCountedPointer's default destroyer, which uses
/// operator delete; it annoys the C API when you nuke its data
/// structures this way. :)
template <>
struct RefCountedPointerDestroyer<MYSQL_RES>
{
	/// \brief Functor implementation
	void operator()(MYSQL_RES* doomed) const
	{
		if (doomed) {
			mysql_free_result(doomed);
		}
	}
};


/// \brief StoreQueryResult set type for "use" queries
///
/// See the user manual for the reason you might want to use this even
/// though its interface is less friendly than StoreQueryResult's.

class MYSQLPP_EXPORT UseQueryResult : public ResultBase
{
public:
	/// \brief Default constructor
	UseQueryResult() :
	ResultBase()
	{
	}
	
	/// \brief Create the object, fully initialized
	UseQueryResult(MYSQL_RES* result, DBDriver* dbd, bool te = true);
	
	/// \brief Create a copy of another UseQueryResult object
	UseQueryResult(const UseQueryResult& other) :
	ResultBase()
	{
		copy(other);
	}
	
	/// \brief Destroy object
	~UseQueryResult() { }

	/// \brief Copy another UseQueryResult object's data into this object
	UseQueryResult& operator =(const UseQueryResult& rhs)
			{ return this != &rhs ? copy(rhs) : *this; }

	/// \brief Returns the next field in this result set
	const Field& fetch_field() const
			{ return fields_.at(current_field_++); }

	/// \brief Returns the given field in this result set
	const Field& fetch_field(Fields::size_type i) const
			{ return fields_.at(i); }

	/// \brief Returns the lengths of the fields in the current row of
	/// the result set.
	///
	/// \internal This should not be terribly useful to end-user code.
	/// The Row object returned by fetch_row() contains these lengths.
	const unsigned long* fetch_lengths() const;

	/// \brief Returns the next row in a "use" query's result set
	///
	/// This is a thick wrapper around DBDriver::fetch_row().  It does a
	/// lot of error checking before returning the Row object containing
	/// the row data.
	///
	/// \sa fetch_raw_row()
	Row fetch_row() const;

	/// \brief Wraps mysql_fetch_row() in MySQL C API.
	///
	/// \internal You almost certainly want to call fetch_row() instead.
	/// It is anticipated that this is only useful within the library,
	/// to implement higher-level query types on top of raw "use"
	/// queries. Query::storein() uses it, for example.
	MYSQL_ROW fetch_raw_row() const;

	/// \brief Jumps to the given field within the result set
	///
	/// Calling this allows you to reset the default field index used
	/// by fetch_field().
	void field_seek(Fields::size_type field) const
			{ current_field_ = field; }

	/// \brief Return the pointer to the underlying MySQL C API
	/// result set object.
	///
	/// While this has obvious inherent value for those times you need
	/// to dig beneath the MySQL++ interface, it has subtler value.
	/// It effectively stands in for operator bool(), operator !(),
	/// operator ==(), and operator !=(), because the C++ compiler can
	/// implement all of these with a MYSQL_RES*.
	///
	/// Of these uses, the most valuable is using the UseQueryResult
	/// object in bool context to determine if the query that created
	// it was successful:
	///
	/// \code
	///   Query q("....");
	///   if (UseQueryResult res = q.use()) {
	///       // Can use 'res', query succeeded
	///   }
	///   else {
	///       // Query failed, call Query::error() or ::errnum() for why
	///   }
	/// \endcode
	operator MYSQL_RES*() const { return result_.raw(); }
	
private:
	/// \brief Copy another ResultBase object's contents into this one.
	UseQueryResult& copy(const UseQueryResult& other);

	/// \brief Reference to underlying C API result set
	///
	/// This is mutable because so many methods in this class are
	/// are justifiably const because they don't modify the result
	/// set's "value" but they call C API methods that take non-const
	/// MYSQL_RES* so they can only be const if this is mutable.  It's
	/// quite likely that these API functions do modify the MYSQL_RES
	/// object, so strict constness says this object changed, too, but
	/// this has always been mutable and the resulting behavior hasn't 
	/// confused anyone yet.
	mutable RefCountedPointer<MYSQL_RES> result_;
};


/// \brief Swaps two StoreQueryResult objects
inline void
swap(StoreQueryResult& x, StoreQueryResult& y)
{
	StoreQueryResult tmp = x;
	x = y;
	y = tmp;
}

/// \brief Swaps two UseQueryResult objects
inline void
swap(UseQueryResult& x, UseQueryResult& y)
{
	UseQueryResult tmp = x;
	x = y;
	y = tmp;
}

} // end namespace mysqlpp

#endif // !defined(MYSQLPP_RESULT_H)
