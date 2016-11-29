/// \file row.h
/// \brief Declares the classes for holding row data from a result set.

/***********************************************************************
 Copyright (c) 1998 by Kevin Atkinson, (c) 1999-2001 by MySQL AB, and
 (c) 2004-2008 by Educational Technology Resources, Inc.  Others may
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

#if !defined(MYSQLPP_ROW_H)
#define MYSQLPP_ROW_H

#include "common.h"

#include "mystring.h"
#include "noexceptions.h"
#include "refcounted.h"
#include "vallist.h"

#include <vector>
#include <string>

namespace mysqlpp {

#if !defined(DOXYGEN_IGNORE)
// Make Doxygen ignore this
class FieldNames;
class MYSQLPP_EXPORT ResultBase;
#endif

/// \brief Manages rows from a result set.
///
/// This class is like an extended version of a \c const \c std::vector
/// of mysqlpp::String.  It adds stuff for populating the vector.  As
/// for why it's \c const, what would it mean to modify a Row?  If we
/// ever did support such semantics, it should probably actually modify
/// the database.  We can't do that if we just derive from std::vector.
///
/// Not that we could derive from std::vector even if we wanted to:
/// \c vector::operator[](size_type) would interfere with our
/// \c operator[](const \c char*).  We can avoid this only by
/// maintaining our own public inteface independent of that of
/// \c vector.

class MYSQLPP_EXPORT Row : public OptionalExceptions
{
private:
	/// \brief Pointer to bool data member, for use by safe bool
	/// conversion operator.
	///
	/// \see http://www.artima.com/cppsource/safebool.html
	typedef bool Row::*private_bool_type;

public:
	/// \brief type of our internal data list
	///
	/// This is public because all other typedefs we have for
	/// mirroring std::vector's public interface depend on it.
	typedef std::vector<String> list_type;

	/// \brief constant iterator type
	typedef list_type::const_iterator const_iterator;

	/// \brief constant reference type
	typedef list_type::const_reference const_reference;

	/// \brief const reverse iterator type
	typedef list_type::const_reverse_iterator const_reverse_iterator;

	/// \brief type for index differences
	typedef list_type::difference_type difference_type;

	/// \brief iterator type
	///
	/// Note that this is just an alias for the const iterator.  Row
	/// is immutable, but people are in the habit of saying 'iterator'
	/// even when they don't intend to use the iterator to modify the
	/// container, so we provide this as a convenience.
	typedef const_iterator iterator;

	/// \brief reference type
	///
	/// \sa iterator for justification for this const_reference alias
	typedef const_reference reference;

	/// \brief mutable reverse iterator type
	///
	/// \sa iterator for justification for this const_reverse_iterator
	/// alias
	typedef const_reverse_iterator reverse_iterator;

	/// \brief type of returned sizes
	typedef list_type::size_type size_type;

	/// \brief type of data in container
	typedef list_type::value_type value_type;

	/// \brief Default constructor
	Row() :
	initialized_(false)
	{
	}
	
	/// \brief Copy constructor
	Row(const Row& r) :
	OptionalExceptions(),
	data_(r.data_.begin(), r.data_.end()),
	field_names_(r.field_names_),
	initialized_(r.initialized_)
	{
	}

	/// \brief Create a row object
	///
	/// \param row MySQL C API row data
	/// \param res result set that the row comes from
	/// \param lengths length of each item in row
	/// \param te if true, throw exceptions on errors
	Row(MYSQL_ROW row, const ResultBase* res,
			const unsigned long* lengths, bool te = true);

	/// \brief Destroy object
	~Row() { }

	/// \brief Get a const reference to the field given its index
	///
	/// \throw mysqlpp::BadIndex if the row is not initialized or there
	/// are less than \c i fields in the row.
	const_reference at(size_type i) const;

	/// \brief Get a reference to the last element of the vector
	const_reference back() const { return data_.back(); }

	/// \brief Return a const iterator pointing to first element in the
	/// container
	const_iterator begin() const { return data_.begin(); }

	/// \brief Returns true if container is empty
	bool empty() const { return data_.empty(); }

	/// \brief Return a const iterator pointing to one past the last
	/// element in the container
	const_iterator end() const { return data_.end(); }

	/// \brief Get an "equal list" of the fields and values in this row
	///
	/// When inserted into a C++ stream, the delimiter 'd' will be used
	/// between the items, " = " is the relationship operator, and items
	/// will be quoted and escaped.
	equal_list_ba<FieldNames, Row, quote_type0>
			equal_list(const char* d = ",", const char* e = " = ") const;

	/// \brief Get an "equal list" of the fields and values in this row
	///
	/// This method's parameters govern how the returned list will
	/// behave when you insert it into a C++ stream:
	///
	/// \param d delimiter to use between items
	/// \param e the operator to use between elements
	/// \param m the manipulator to use for each element
	///
	/// For example, if d is ",", e is " = ", and m is the quote
	/// manipulator, then the field and value lists (a, b) (c, d'e)
	/// will yield an equal list that gives the following when inserted
	/// into a C++ stream:
	///
	/// \code
	///   'a' = 'c', 'b' = 'd''e'
	/// \endcode
	///
	/// Notice how the single quote was 'escaped' in the SQL way to
	/// avoid a syntax error.
	template <class Manip>
	equal_list_ba<FieldNames, Row, Manip> equal_list(const char* d,
			const char* e, Manip m) const;

	/// \brief Get a list of the field names in this row
	///
	/// When inserted into a C++ stream, the delimiter 'd' will be used
	/// between the items, and no manipulator will be used on the items.
	value_list_ba<FieldNames, do_nothing_type0>
			field_list(const char* d = ",") const;

	/// \brief Get a list of the field names in this row
	///
	/// \param d delimiter to place between the items when the list is
	/// inserted into a C++ stream
	/// \param m manipulator to use before each item when the list is
	/// inserted into a C++ stream
	template <class Manip>
	value_list_ba<FieldNames, Manip> field_list(const char* d,
			Manip m) const;

	/// \brief Get a list of the field names in this row
	///
	/// \param d delimiter to place between the items when the list is
	/// inserted into a C++ stream
	/// \param m manipulator to use before each item when the list is
	/// inserted into a C++ stream
	/// \param vb for each true item in this list, add that field name
	/// to the returned list; ignore the others
	template <class Manip>
	value_list_b<FieldNames, Manip> field_list(const char* d, Manip m,
			const std::vector<bool>& vb) const;

	/// \brief Get a list of the field names in this row
	///
	/// \param d delimiter to place between the items when the list is
	/// inserted into a C++ stream
	/// \param vb for each true item in this list, add that field name
	/// to the returned list; ignore the others
	///
	/// Field names will be quoted and escaped when inserted into a C++
	/// stream.
	value_list_b<FieldNames, quote_type0> field_list(
			const char* d, const std::vector<bool>& vb) const;

	/// \brief Get a list of the field names in this row
	///
	/// \param vb for each true item in this list, add that field name
	/// to the returned list; ignore the others
	///
	/// Field names will be quoted and escaped when inserted into a C++
	/// stream, and a comma will be placed between them as a delimiter.
	value_list_b<FieldNames, quote_type0> field_list(
			const std::vector<bool>& vb) const;

	/// \brief Get a list of the field names in this row
	///
	/// For each true parameter, the field name in that position within
	/// the row is added to the returned list.  When the list is
	/// inserted into a C++ stream, the delimiter 'd' will be placed
	/// between the items as a delimiter, and the manipulator 'm' used
	/// before each item.
	template <class Manip>
	value_list_b<FieldNames, Manip> field_list(const char *d, Manip m,
			bool t0,
			bool t1 = false, bool t2 = false, bool t3 = false,
			bool t4 = false, bool t5 = false, bool t6 = false,
			bool t7 = false, bool t8 = false, bool t9 = false,
			bool ta = false, bool tb = false, bool tc = false) const;

	/// \brief Get a list of the field names in this row
	///
	/// For each true parameter, the field name in that position within
	/// the row is added to the returned list.  When the list is
	/// inserted into a C++ stream, the delimiter 'd' will be placed
	/// between the items as a delimiter, and the items will be quoted
	/// and escaped.
	value_list_b<FieldNames, quote_type0> field_list(
			const char *d, bool t0,
			bool t1 = false, bool t2 = false, bool t3 = false,
			bool t4 = false, bool t5 = false, bool t6 = false,
			bool t7 = false, bool t8 = false, bool t9 = false,
			bool ta = false, bool tb = false, bool tc = false) const;

	/// \brief Get a list of the field names in this row
	///
	/// For each true parameter, the field name in that position within
	/// the row is added to the returned list.  When the list is
	/// inserted into a C++ stream, a comma will be placed between the
	/// items as a delimiter, and the items will be quoted and escaped.
	value_list_b<FieldNames, quote_type0> field_list(
			bool t0,
			bool t1 = false, bool t2 = false, bool t3 = false,
			bool t4 = false, bool t5 = false, bool t6 = false,
			bool t7 = false, bool t8 = false, bool t9 = false,
			bool ta = false, bool tb = false, bool tc = false) const;

	/// \brief Returns a field's index given its name
	size_type field_num(const char* name) const;

	/// \brief Get a reference to the first element of the vector
	const_reference front() const { return data_.front(); }

	/// \brief Return maximum number of elements that can be stored
	/// in container without resizing.
	size_type max_size() const { return data_.max_size(); }

	/// \brief Assignment operator
	Row& operator =(const Row& rhs)
	{
		data_.assign(rhs.data_.begin(), rhs.data_.end());
		field_names_.assign(rhs.field_names_);
		initialized_ = rhs.initialized_;
		return *this;
	}

	/// \brief Get the value of a field given its name.
	///
	/// If the field does not exist in this row, we throw a BadFieldName
	/// exception if exceptions are enabled, or an empty row if not.
	/// An empty row tests as false in bool context.
	///
	/// This operator is fairly inefficient.  operator[](int) is faster.
	const_reference operator [](const char* field) const;

	/// \brief Get the value of a field given its index.
	///
	/// This function is just syntactic sugar, wrapping the at() method.
	///
	/// It's \b critical that the parameter type be \c int, not
	/// \c size_type, because it will interfere with the \c const
	/// \c char* overload otherwise.  row[0] is ambiguous when there
	/// isn't an int overload.
	///
	/// \throw mysqlpp::BadIndex if the row is not initialized or there
	/// are less than \c i fields in the row.
	const_reference operator [](int i) const
			{ return at(static_cast<size_type>(i)); }

	/// \brief Returns true if row object was fully initialized and
	/// has data.
	///
	/// This operator lets you use Row in bool context, which lets you
	/// do things like tell when you've run off the end of a "use"
	/// query's result set:
	///
	/// \code
	///   Query q("....");
	///   if (UseQueryResult res = q.use()) {
	///       // Can use 'res', query succeeded
	///       while (Row row = res.fetch_row()) {
	///           // Retreived another row in the result set, can use 'row'
	///       }
	///   }
	/// \endcode
	///
	operator private_bool_type() const
	{
		return data_.size() && initialized_ ? &Row::initialized_ : 0;
	}

	/// \brief Return reverse iterator pointing to first element in the
	/// container
	const_reverse_iterator rbegin() const { return data_.rbegin(); }

	/// \brief Return reverse iterator pointing to one past the last
	/// element in the container
	const_reverse_iterator rend() const { return data_.rend(); }

	/// \brief Get the number of fields in the row.
	size_type size() const { return data_.size(); }

	/// \brief Get a list of the values in this row
	///
	/// When inserted into a C++ stream, the delimiter 'd' will be used
	/// between the items, and the quoting and escaping rules will be
	/// set by the manipulator 'm' you choose.
	///
	/// \param d delimiter to use between values
	/// \param m manipulator to use when inserting values into a stream
	template <class Manip>
	value_list_ba<Row, Manip> value_list(const char* d = ",",
			Manip m = quote) const
	{
		return value_list_ba<Row, Manip>(*this, d, m);
	}
	
	/// \brief Get a list of the values in this row
	///
	/// \param d delimiter to use between values
	/// \param vb for each true item in this list, add that value to the
	/// returned list; ignore the others
	/// \param m manipulator to use when inserting values into a stream
	template <class Manip>
	value_list_b<Row, Manip> value_list(const char *d,
			const std::vector<bool>& vb, Manip m = quote) const
	{
		return value_list_b<Row, Manip>(*this, vb, d, m);
	}
	
	/// \brief Get a list of the values in this row
	///
	/// \param vb for each true item in this list, add that value to the
	/// returned list; ignore the others
	///
	/// Items will be quoted and escaped when inserted into a C++ stream,
	/// and a comma will be used as a delimiter between the items.
	value_list_b<Row, quote_type0> value_list(
			const std::vector<bool> &vb) const
	{
		return value_list_b<Row, quote_type0>(*this, vb, ",", quote);
	}
	
	/// \brief Get a list of the values in this row
	///
	/// For each true parameter, the value in that position within the
	/// row is added to the returned list.  When the list is inserted
	/// into a C++ stream, the delimiter 'd' will be placed between the
	/// items, and the manipulator 'm' used before each item.
	template <class Manip>
	value_list_b<Row, Manip> value_list(const char *d, Manip m,
			bool t0, bool t1 = false, bool t2 = false, bool t3 = false,
			bool t4 = false, bool t5 = false, bool t6 = false,
			bool t7 = false, bool t8 = false, bool t9 = false,
			bool ta = false, bool tb = false, bool tc = false) const
	{
		std::vector<bool> vb;
		create_vector(size(), vb, t0, t1, t2, t3, t4, t5, t6,
				t7, t8, t9, ta, tb, tc);
		return value_list_b<Row, Manip>(*this, vb, d, m);
	}
	
	/// \brief Get a list of the values in this row
	///
	/// For each true parameter, the value in that position within the
	/// row is added to the returned list.  When the list is inserted
	/// into a C++ stream, the delimiter 'd' will be placed between the
	/// items, and items will be quoted and escaped.
	value_list_b <Row, quote_type0>
	value_list(const char *d, bool t0, bool t1 = false, bool t2 = false,
			bool t3 = false, bool t4 = false, bool t5 = false,
			bool t6 = false, bool t7 = false, bool t8 = false,
			bool t9 = false, bool ta = false, bool tb = false,
			bool tc = false) const
	{
		std::vector<bool> vb;
		create_vector(size(), vb, t0, t1, t2, t3, t4, t5, t6,
				t7, t8, t9, ta, tb, tc);
		return value_list_b<Row, quote_type0>(*this, vb, d, quote);
	}
	
	/// \brief Get a list of the values in this row
	///
	/// For each true parameter, the value in that position within the
	/// row is added to the returned list.  When the list is inserted
	/// into a C++ stream, the a comma will be placed between the items,
	/// as a delimiter, and items will be quoted and escaped.
	value_list_b<Row, quote_type0> value_list(bool t0,
			bool t1 = false, bool t2 = false, bool t3 = false,
			bool t4 = false, bool t5 = false, bool t6 = false,
			bool t7 = false, bool t8 = false, bool t9 = false,
			bool ta = false, bool tb = false, bool tc = false) const
	{
		std::vector<bool> vb;
		create_vector(size(), vb, t0, t1, t2, t3, t4, t5, t6,
				t7, t8, t9, ta, tb, tc);
		return value_list_b<Row, quote_type0>(*this, vb, ",", quote);
	}
	
	/// \brief Get a list of the values in this row
	///
	/// The 's' parameters name the fields that will be added to the
	/// returned list.  When inserted into a C++ stream, the delimiter
	/// 'd' will be placed between the items, and the manipulator 'm'
	/// will be inserted before each item.
	template <class Manip>
	value_list_b<Row, Manip> value_list(const char *d, Manip m,
			std::string s0, std::string s1 = "", std::string s2 = "",
			std::string s3 = "", std::string s4 = "",
			std::string s5 = "", std::string s6 = "",
			std::string s7 = "", std::string s8 = "",
			std::string s9 = "", std::string sa = "",
			std::string sb = "", std::string sc = "") const
	{
		std::vector<bool> vb;
		create_vector(*this, vb, s0, s1, s2, s3, s4, s5, s6, s7, s8,
				s9, sa, sb, sc);
		return value_list_b<Row, Manip>(*this, vb, d, m);
	}
	
	/// \brief Get a list of the values in this row
	///
	/// The 's' parameters name the fields that will be added to the
	/// returned list.  When inserted into a C++ stream, the delimiter
	/// 'd' will be placed between the items, and items will be quoted
	/// and escaped.
	value_list_b<Row, quote_type0> value_list(
			const char *d,
			std::string s0, std::string s1 = "", std::string s2 = "",
			std::string s3 = "", std::string s4 = "",
			std::string s5 = "", std::string s6 = "",
			std::string s7 = "", std::string s8 = "",
			std::string s9 = "", std::string sa = "",
			std::string sb = "", std::string sc = "") const
	{
		std::vector<bool> vb;
		create_vector(*this, vb, s0, s1, s2, s3, s4, s5, s6, s7, s8,
				s9, sa, sb, sc);
		return value_list_b<Row, quote_type0>(*this, vb, d, quote);
	}
	
	/// \brief Get a list of the values in this row
	///
	/// The 's' parameters name the fields that will be added to the
	/// returned list.  When inserted into a C++ stream, a comma will be
	/// placed between the items as a delimiter, and items will be
	/// quoted and escaped.
	value_list_b<Row, quote_type0> value_list(
			std::string s0,
			std::string s1 = "", std::string s2 = "",
			std::string s3 = "", std::string s4 = "",
			std::string s5 = "", std::string s6 = "",
			std::string s7 = "", std::string s8 = "",
			std::string s9 = "", std::string sa = "",
			std::string sb = "", std::string sc = "") const
	{
		std::vector<bool> vb;
		create_vector(*this, vb, s0, s1, s2, s3, s4, s5, s6, s7, s8,
				s9, sa, sb, sc);
		return value_list_b<Row, quote_type0>(*this, vb, ",", quote);
	}

private:
	list_type data_;
	RefCountedPointer<FieldNames> field_names_;
	bool initialized_;
};

} // end namespace mysqlpp

#endif // !defined(MYSQLPP_ROW_H)
