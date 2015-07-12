/// \file vallist.h
/// \brief Declares templates for holding lists of values.

/***********************************************************************
 Copyright (c) 1998 by Kevin Atkinson, (c) 1999, 2000 and 2001 by
 MySQL AB, and (c) 2004, 2005 by Educational Technology Resources, Inc.
 Others may also hold copyrights on code in this file.  See the CREDITS
 file in the top directory of the distribution for details.

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

#if !defined(MYSQLPP_VALLIST_H)
#define MYSQLPP_VALLIST_H

#include "manip.h"

#include <string>
#include <vector>

namespace mysqlpp {


/// \brief Holds two lists of items, typically used to construct a
/// SQL "equals clause".
///
/// The WHERE clause in a SQL SELECT statment is an example of an
/// equals clause.
///
/// Imagine an object of this type contains the lists (a, b) (c, d),
/// and that the object's delimiter and equals symbols are set to ", "
/// and " = " respectively.  When you insert that object into a C++
/// stream, you would get "a = c, b = d".
///
/// This class is never instantiated by hand.  The equal_list()
/// functions build instances of this structure template to do their
/// work.  MySQL++'s SSQLS mechanism calls those functions when
/// building SQL queries; you can call them yourself to do similar work.
/// The "Harnessing SSQLS Internals" section of the user manual has
/// some examples of this.
///
/// \sa equal_list_b

template <class Seq1, class Seq2, class Manip>
struct equal_list_ba
{
	/// \brief the list of objects on the left-hand side of the
	/// equals sign
	const Seq1* list1;

	/// \brief the list of objects on the right-hand side of the
	/// equals sign
	const Seq2* list2;

	/// \brief delimiter to use between each pair of elements
	const char* delim;

	/// \brief "equal" sign to use between each item in each equal
	/// pair; doesn't have to actually be " = "
	const char* equl;

	/// \brief manipulator to use when inserting the equal_list into
	/// a C++ stream
	Manip manip;

	/// \brief Create object
	///
	/// \param s1 list of objects on left-hand side of equal sign
	/// \param s2 list of objects on right-hand side of equal sign
	/// \param d what delimiter to use between each group in the list
	///		when inserting the list into a C++ stream
	/// \param e the "equals" sign between each pair of items in the
	/// 	equal list; doesn't actually have to be " = "!
	/// \param m manipulator to use when inserting the list into a
	/// 	C++ stream
	equal_list_ba(const Seq1& s1, const Seq2& s2, const char* d,
			const char* e, Manip m) :
	list1(&s1),
	list2(&s2),
	delim(d),
	equl(e),
	manip(m)
	{
	}
};


/// \brief Same as equal_list_ba, plus the option to have some elements
/// of the equals clause suppressed.
///
/// Imagine an object of this type contains the lists (a, b, c)
/// (d, e, f), that the object's 'fields' list is (true, false, true),
/// and that the object's delimiter and equals symbols are set to
/// " AND " and " = " respectively.  When you insert that object into a
/// C++ stream, you would get "a = d AND c = f".
///
/// See equal_list_ba's documentation for more details.

template <class Seq1, class Seq2, class Manip>
struct equal_list_b
{
	/// \brief the list of objects on the left-hand side of the
	/// equals sign
	const Seq1* list1;

	/// \brief the list of objects on the right-hand side of the
	/// equals sign
	const Seq2* list2;

	/// \brief for each true item in the list, the pair in that position
	/// will be inserted into a C++ stream
	const std::vector<bool> fields;

	/// \brief delimiter to use between each pair of elements
	const char* delim;

	/// \brief "equal" sign to use between each item in each equal
	/// pair; doesn't have to actually be " = "
	const char* equl;

	/// \brief manipulator to use when inserting the equal_list into
	/// a C++ stream
	Manip manip;

	/// \brief Create object
	///
	/// \param s1 list of objects on left-hand side of equal sign
	/// \param s2 list of objects on right-hand side of equal sign
	/// \param f for each true item in the list, the pair of items
	///		in that position will be inserted into a C++ stream
	/// \param d what delimiter to use between each group in the list
	///		when inserting the list into a C++ stream
	/// \param e the "equals" sign between each pair of items in the
	/// 	equal list; doesn't actually have to be " = "!
	/// \param m manipulator to use when inserting the list into a
	/// 	C++ stream
	equal_list_b(const Seq1& s1, const Seq2& s2,
			const std::vector<bool>& f, const char* d,
			const char* e, Manip m) :
	list1(&s1),
	list2(&s2),
	fields(f),
	delim(d),
	equl(e),
	manip(m)
	{
	}
};


/// \brief Holds a list of items, typically used to construct a SQL
/// "value list".
///
/// The SQL INSERT statement has a VALUES clause; this class can
/// be used to construct the list of items for that clause.
///
/// Imagine an object of this type contains the list (a, b, c), and
/// that the object's delimiter symbol is set to ", ".  When you
/// insert that object into a C++ stream, you would get "a, b, c".
///
/// This class is never instantiated by hand.  The value_list()
/// functions build instances of this structure template to do their
/// work.  MySQL++'s SSQLS mechanism calls those functions when
/// building SQL queries; you can call them yourself to do similar work.
/// The "Harnessing SSQLS Internals" section of the user manual has
/// some examples of this.
///
/// \sa value_list_b

template <class Seq, class Manip>
struct value_list_ba
{
	/// \brief set of objects in the value list
	const Seq* list;

	/// \brief delimiter to use between each value in the list when
	/// inserting it into a C++ stream
	const char* delim;

	/// \brief manipulator to use when inserting the list into a
	/// C++ stream
	Manip manip;

	/// \brief Create object
	///
	/// \param s set of objects in the value list
	/// \param d what delimiter to use between each value in the list
	///		when inserting the list into a C++ stream
	/// \param m manipulator to use when inserting the list into a
	/// 	C++ stream
	value_list_ba(const Seq& s, const char* d, Manip m) :
	list(&s),
	delim(d),
	manip(m)
	{
	}
};


/// \brief Same as value_list_ba, plus the option to have some elements
/// of the list suppressed.
///
/// Imagine an object of this type contains the list (a, b, c), that
/// the object's 'fields' list is (true, false, true), and that the
/// object's delimiter is set to ":".  When you insert that object
/// into a C++ stream, you would get "a:c".
///
/// See value_list_ba's documentation for more details.

template <class Seq, class Manip>
struct value_list_b
{
	/// \brief set of objects in the value list
	const Seq* list;

	/// \brief delimiter to use between each value in the list when
	/// inserting it into a C++ stream
	const std::vector<bool> fields;

	/// \brief delimiter to use between each value in the list when
	/// inserting it into a C++ stream
	const char* delim;

	/// \brief manipulator to use when inserting the list into a C++
	/// stream
	Manip manip;

	/// \brief Create object
	///
	/// \param s set of objects in the value list
	/// \param f for each true item in the list, the list item
	///		in that position will be inserted into a C++ stream
	/// \param d what delimiter to use between each value in the list
	///		when inserting the list into a C++ stream
	/// \param m manipulator to use when inserting the list into a
	///		C++ stream
	value_list_b(const Seq& s, const std::vector<bool>& f,
			const char* d, Manip m) :
	list(&s),
	fields(f),
	delim(d),
	manip(m)
	{
	}
};


/// \brief Inserts an equal_list_ba into an std::ostream.
///
/// Given two lists (a, b) and (c, d), a delimiter D, and an equals
/// symbol E, this operator will insert "aEcDbEd" into the stream.
///
/// See equal_list_ba's documentation for concrete examples.
///
/// \sa equal_list()

template <class Seq1, class Seq2, class Manip>
std::ostream& operator <<(std::ostream& o,
		const equal_list_ba<Seq1, Seq2, Manip>& el)
{
	typename Seq1::const_iterator i = el.list1->begin();
	typename Seq2::const_iterator j = el.list2->begin();

	while (1) {
		o << *i << el.equl << el.manip << *j;
		if ((++i == el.list1->end()) || (++j == el.list2->end())) {
			break;
		}
		o << el.delim;
	}

	return o;
}


/// \brief Same as operator<< for equal_list_ba, plus the option to
/// suppress insertion of some list items in the stream.
///
/// See equal_list_b's documentation for examples of how this works.

template <class Seq1, class Seq2, class Manip>
std::ostream& operator <<(std::ostream& o,
		const equal_list_b <Seq1, Seq2, Manip>& el)
{
	typename Seq1::const_iterator i = el.list1->begin();
	typename Seq2::const_iterator j = el.list2->begin();

	int k = 0;
	while (1) {
		if (el.fields[k++]) {
			o << *i << el.equl << el.manip << *j;
		}
		if ((++i == el.list1->end()) || (++j == el.list2->end())) {
			break;
		}
		if (el.fields[k]) {
			o << el.delim;
		}
	}

	return o;
}


/// \brief Inserts a value_list_ba into an std::ostream.
///
/// Given a list (a, b) and a delimiter D, this operator will insert
/// "aDb" into the stream.
///
/// See value_list_ba's documentation for concrete examples.
///
/// \sa value_list()

template <class Seq, class Manip>
std::ostream& operator <<(std::ostream& o,
		const value_list_ba<Seq, Manip>& cl)
{
	typename Seq::const_iterator i = cl.list->begin();

	while (1) {
		o << cl.manip << *i;
		if (++i == cl.list->end()) {
			break;
		}
		o << cl.delim;
	}

	return o;
}


/// \brief Same as operator<< for value_list_ba, plus the option to
/// suppress insertion of some list items in the stream.
///
/// See value_list_b's documentation for examples of how this works.

template <class Seq, class Manip>
std::ostream& operator <<(std::ostream& o,
		const value_list_b<Seq, Manip>& cl)
{
	typename Seq::const_iterator i = cl.list->begin();

	int k = 0;
	while (1) {
		if (cl.fields[k++]) {
			o << cl.manip << *i;
		}
		if (++i == cl.list->end()) {
			break;
		}
		if (cl.fields[k]) {
			o << cl.delim;
		}
	}

	return o;
}


/// \brief Create a vector of bool with the given arguments as values.
///
/// This function takes up to 13 bools, with the size parameter
/// controlling the actual number of parameters we pay attention to.
///
/// This function is used within the library to build the vector used
/// in calling the vector form of Row::equal_list(), Row::value_list(),
/// and Row::field_list().  See the "Harnessing SSQLS Internals" section
/// of the user manual to see that feature at work.

void create_vector(size_t size, std::vector<bool>& v, bool t0,
		bool t1 = false, bool t2 = false, bool t3 = false,
		bool t4 = false, bool t5 = false, bool t6 = false,
		bool t7 = false, bool t8 = false, bool t9 = false,
		bool ta = false, bool tb = false, bool tc = false);


/// \brief Create a vector of bool using a list of named fields.
///
/// This function is used with the ResUse and Result containers,
/// which have a field_num() member function that maps a field name
/// to its position number.  So for each named field, we set the
/// bool in the vector at the corresponding position to true.
///
/// This function is used within the library to build the vector used
/// in calling the vector form of Row::equal_list(), Row::value_list(),
/// and Row::field_list().  See the "Harnessing SSQLS Internals" section
/// of the user manual to see that feature at work.

template <class Container>
void create_vector(const Container& c, std::vector<bool>& v,
		std::string s0, std::string s1, std::string s2,
		std::string s3, std::string s4, std::string s5,
		std::string s6, std::string s7, std::string s8,
		std::string s9, std::string sa, std::string sb,
		std::string sc);



/// \brief Constructs a value_list_ba
///
/// This function returns a value list that uses the 'do_nothing'
/// manipulator.  That is, the items are not quoted or escaped in any
/// way.  See value_list(Seq, const char*, Manip) if you need to
/// specify a manipulator.
///
/// \param s an STL sequence of items in the value list
/// \param d delimiter operator<< should place between items

template <class Seq>
value_list_ba<Seq, do_nothing_type0>
value_list(const Seq& s, const char* d = ",")
{
	return value_list_ba<Seq, do_nothing_type0>(s, d, do_nothing);
}


/// \brief Constructs a value_list_ba
///
/// \param s an STL sequence of items in the value list
/// \param d delimiter operator<< should place between items
/// \param m manipulator to use when inserting items into a stream

template <class Seq, class Manip>
value_list_ba<Seq, Manip>
value_list(const Seq& s, const char* d, Manip m)
{
	return value_list_ba<Seq, Manip>(s, d, m);
}


/// \brief Constructs a value_list_b (sparse value list)
///
/// \param s an STL sequence of items in the value list
/// \param d delimiter operator<< should place between items
/// \param m manipulator to use when inserting items into a stream
/// \param vb for each item in this vector that is true, the
/// corresponding item in the value list is inserted into a stream;
/// the others are suppressed

template <class Seq, class Manip>
inline value_list_b<Seq, Manip>
value_list(const Seq& s, const char* d, Manip m,
		const std::vector<bool>& vb)
{
	return value_list_b<Seq, Manip>(s, vb, d, m);
}


/// \brief Constructs a value_list_b (sparse value list)
///
/// Same as value_list(Seq&, const char*, Manip, const vector<bool>&),
/// except that it takes the bools as arguments instead of wrapped up
/// in a vector object.

template <class Seq, class Manip>
value_list_b<Seq, Manip>
value_list(const Seq& s, const char* d, Manip m, bool t0,
		bool t1 = false, bool t2 = false, bool t3 = false,
		bool t4 = false, bool t5 = false, bool t6 = false,
		bool t7 = false, bool t8 = false, bool t9 = false,
		bool ta = false, bool tb = false, bool tc = false)
{
	std::vector<bool> vb;
	create_vector(s.size(), vb, t0, t1, t2, t3, t4, t5, t6, t7, t8, t9,
				  ta, tb, tc);
	return value_list_b<Seq, Manip>(s, vb, d, m);
}

/// \brief Constructs a sparse value list
///
/// Same as value_list(Seq&, const char*, Manip, bool, bool...) but
/// without the Manip parameter.  We use the do_nothing manipulator,
/// meaning that the value list items are neither escaped nor quoted
/// when being inserted into a stream.

template <class Seq>
value_list_b<Seq, do_nothing_type0>
value_list(const Seq& s, const char* d, bool t0,
		bool t1 = false, bool t2 = false, bool t3 = false,
		bool t4 = false, bool t5 = false, bool t6 = false,
		bool t7 = false, bool t8 = false, bool t9 = false,
		bool ta = false, bool tb = false, bool tc = false)
{
	std::vector<bool> vb;
	create_vector(s.size(), vb, t0, t1, t2, t3, t4, t5, t6, t7, t8, t9,
				  ta, tb, tc);
	return value_list_b<Seq, do_nothing_type0>(s, vb, d, do_nothing);
}


/// \brief Constructs a sparse value list
///
/// Same as value_list(Seq&, const char*, Manip, bool, bool...) but
/// without the Manip or delimiter parameters.  We use the do_nothing
/// manipulator, meaning that the value list items are neither escaped
/// nor quoted when being inserted into a stream.  The delimiter is a
/// comma.  This form is suitable for lists of simple data, such as
/// integers.

template <class Seq>
value_list_b<Seq, do_nothing_type0>
value_list(const Seq& s, bool t0,
		bool t1 = false, bool t2 = false, bool t3 = false,
		bool t4 = false, bool t5 = false, bool t6 = false,
		bool t7 = false, bool t8 = false, bool t9 = false,
		bool ta = false, bool tb = false, bool tc = false)
{
	std::vector<bool> vb;
	create_vector(s.size(), vb, t0, t1, t2, t3, t4, t5, t6, t7, t8, t9,
				  ta, tb, tc);
	return value_list_b<Seq, do_nothing_type0>(s, vb, ",", do_nothing);
}


/// \brief Constructs an equal_list_ba
///
/// This function returns an equal list that uses the 'do_nothing'
/// manipulator.  That is, the items are not quoted or escaped in any
/// way when inserted into a stream.  See equal_list(Seq, Seq,
/// const char*, const char*, Manip) if you need a different
/// manipulator.
///
/// The idea is for both lists to be of equal length because
/// corresponding elements from each list are handled as pairs, but if
/// one list is shorter than the other, the generated list will have
/// that many elements.
///
/// \param s1 items on the left side of the equals sign when the
/// equal list is inserted into a stream
/// \param s2 items on the right side of the equals sign
/// \param d delimiter operator<< should place between pairs
/// \param e what operator<< should place between items in each pair;
/// by default, an equals sign, as that is the primary use for this
/// mechanism.

template <class Seq1, class Seq2>
equal_list_ba<Seq1, Seq2, do_nothing_type0>
equal_list(const Seq1& s1, const Seq2& s2, const char *d = ",",
		const char *e = " = ")
{
	return equal_list_ba<Seq1, Seq2, do_nothing_type0>(s1, s2, d,
			e, do_nothing);
}


/// \brief Constructs an equal_list_ba
///
/// Same as equal_list(Seq&, Seq&, const char*, const char*) except that
/// it also lets you specify the manipulator.  Use this version if the
/// data must be escaped or quoted when being inserted into a stream.

template <class Seq1, class Seq2, class Manip>
equal_list_ba<Seq1, Seq2, Manip>
equal_list(const Seq1& s1, const Seq2& s2, const char* d,
		const char* e, Manip m)
{
	return equal_list_ba<Seq1, Seq2, Manip>(s1, s2, d, e, m);
}


/// \brief Constructs a equal_list_b (sparse equal list)
///
/// Same as equal_list(Seq&, Seq&, const char*, const char*, Manip) except
/// that you can pass a vector of bools. For each true item in that
/// list, operator<< adds the corresponding item is put in the equal
/// list.  This lets you pass in sequences when you don't want all of
/// the elements to be inserted into a stream.

template <class Seq1, class Seq2, class Manip>
equal_list_b<Seq1, Seq2, Manip>
equal_list(const Seq1& s1, const Seq2& s2, const char* d,
		const char *e, Manip m, const std::vector<bool>& vb)
{
	return equal_list_b<Seq1, Seq2, Manip>(s1, s2, vb, d, e, m);
}


/// \brief Constructs a equal_list_b (sparse equal list)
///
/// Same as equal_list(Seq&, Seq&, const char*, const char*, Manip,
/// vector<bool>&) except that it takes boolean parameters
/// instead of a list of bools.

template <class Seq1, class Seq2, class Manip>
equal_list_b<Seq1, Seq2, Manip>
equal_list(const Seq1& s1, const Seq2& s2, const char* d,
		const char* e, Manip m, bool t0, bool t1 = false,
		bool t2 = false, bool t3 = false, bool t4 = false,
		bool t5 = false, bool t6 = false, bool t7 = false,
		bool t8 = false, bool t9 = false, bool ta = false,
		bool tb = false, bool tc = false)
{
	std::vector<bool> vb;
	create_vector(s1.size(), vb, t0, t1, t2, t3, t4, t5, t6, t7, t8,
				  t9, ta, tb, tc);
	return equal_list_b<Seq1, Seq2, Manip>(s1, s2, vb, d, e, m);
}


/// \brief Constructs a equal_list_b (sparse equal list)
///
/// Same as equal_list(Seq&, Seq&, const char*, const char*, Manip,
/// bool, bool...) except that it doesn't take the Manip argument.
/// It uses the do_nothing manipulator instead, meaning that none of
/// the elements are escaped when being inserted into a stream.

template <class Seq1, class Seq2>
equal_list_b<Seq1, Seq2, do_nothing_type0>
equal_list(const Seq1& s1, const Seq2& s2, const char* d,
		const char* e, bool t0, bool t1 = false, bool t2 = false,
		bool t3 = false, bool t4 = false, bool t5 = false,
		bool t6 = false, bool t7 = false, bool t8 = false,
		bool t9 = false, bool ta = false, bool tb = false,
		bool tc = false)
{
	std::vector<bool> vb;
	create_vector(s1.size(), vb, t0, t1, t2, t3, t4, t5, t6, t7, t8,
				  t9, ta, tb, tc);
	return equal_list_b<Seq1, Seq2, do_nothing_type0>(s1, s2, vb,
			d, e, do_nothing);
}


/// \brief Constructs a equal_list_b (sparse equal list)
///
/// Same as equal_list(Seq&, Seq&, const char*, const char*, bool,
/// bool...) except that it doesn't take the second const char*
/// argument.  It uses " = " for the equals symbol.

template <class Seq1, class Seq2>
equal_list_b<Seq1, Seq2, do_nothing_type0>
equal_list(const Seq1& s1, const Seq2& s2, const char* d, bool t0,
		bool t1 = false, bool t2 = false, bool t3 = false,
		bool t4 = false, bool t5 = false, bool t6 = false,
		bool t7 = false, bool t8 = false, bool t9 = false,
		bool ta = false, bool tb = false, bool tc = false)
{
	std::vector<bool> vb;
	create_vector(s1.size(), vb, t0, t1, t2, t3, t4, t5, t6, t7, t8,
				  t9, ta, tb, tc);
	return equal_list_b<Seq1, Seq2, do_nothing_type0>(s1, s2, vb,
			d, " = ", do_nothing);
}


/// \brief Constructs a equal_list_b (sparse equal list)
///
/// Same as equal_list(Seq&, Seq&, const char*, bool, bool...) except
/// that it doesn't take the const char* argument.  It uses a comma for
/// the delimiter.  This form is useful for building simple equals
/// lists, where no manipulators are necessary, and the default
/// delimiter and equals symbol are suitable.

template <class Seq1, class Seq2>
equal_list_b<Seq1, Seq2, do_nothing_type0>
equal_list(const Seq1& s1, const Seq2& s2, bool t0, bool t1 = false,
		bool t2 = false, bool t3 = false, bool t4 = false,
		bool t5 = false, bool t6 = false, bool t7 = false,
		bool t8 = false, bool t9 = false, bool ta = false,
		bool tb = false, bool tc = false)
{
	std::vector<bool> vb;
	create_vector(s1.size(), vb, t0, t1, t2, t3, t4, t5, t6, t7, t8,
				  t9, ta, tb, tc);
	return equal_list_b<Seq1, Seq2, do_nothing_type0>(s1, s2, vb,
			",", " = ", do_nothing);
}

} // end namespace mysqlpp

#endif // !defined(MYSQLPP_VALLIST_H)
