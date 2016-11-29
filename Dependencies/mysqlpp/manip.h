/// \file manip.h
/// \brief Declares the Query stream manipulators and operators.
///
/// These manipulators let you automatically quote elements or escape
/// characters that are special in SQL when inserting them into a
/// Query stream.  They make it easier to build syntactically-correct
/// SQL queries.
///
/// This file also includes special \c operator<< definitions for a few
/// key MySQL++ data types, since we know when to do automatic quoting
/// and escaping for these types.  This only works with Query streams,
/// not regular std::ostreams, since we're only concerned with making
/// correct SQL, not with presentation matters.
///
/// test/test_manip.cpp exercises the mechanisms defined here.

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

#ifndef MYSQLPP_MANIP_H
#define MYSQLPP_MANIP_H

#include "common.h"

#include "myset.h"
#include "stadapter.h"

#include <iostream>

namespace mysqlpp {

class SQLQueryParms;


/// \enum quote_type0
/// \anchor quote_manip
///
/// The standard 'quote' manipulator.  It is the most widely useful
/// manipulator in MySQL++.
///
/// Insert this manipulator into a Query or SQLQueryParms stream to put
/// single quotes around the next item in the stream, and escape any
/// characters within it that are special in SQL, if the data type of
/// the next item in the stream may require it.  By contrast, Date
/// objects only require escaping, not quoting, and integers never
/// require either.  The manipulators won't do work they know is not
/// necessary to ensure syntactially-correct SQL.

enum quote_type0
{
	quote ///< insert into a Query stream to single-quote and escape next item
};


#if !defined(DOXYGEN_IGNORE)
// Doxygen will not generate documentation for this section.

struct quote_type1
{
	std::ostream * ostr;
	quote_type1(std::ostream * o) :
	ostr(o)
	{
	}
};


inline quote_type1
operator <<(std::ostream& o, quote_type0 /* esc */)
{
	return quote_type1(&o);
}


struct quote_type2
{
	SQLQueryParms *qparms;
	quote_type2(SQLQueryParms* p) :
	qparms(p)
	{
	}
};


inline quote_type2
operator <<(SQLQueryParms& p, quote_type0 /* esc */)
{
	return quote_type2(&p);
}


/// \brief Inserts a SQLTypeAdapter into a stream, quoted and escaped
/// as appropriate to the data type the object was initialized from.

MYSQLPP_EXPORT SQLQueryParms& operator <<(quote_type2 p,
		SQLTypeAdapter& in);


/// \brief Inserts a anything that can be converted to SQLTypeAdapter
/// into a stream, quoted and escaped as needed if it's a Query stream

MYSQLPP_EXPORT std::ostream& operator <<(quote_type1 o,
		const SQLTypeAdapter& in);


/// \brief Inserts a SQLTypeAdapter into a non-Query stream.
///
/// Although we know how to quote and escape SQLTypeAdapter objects, we
/// only do that when inserting them into Query streams or when given an
/// explicit manipulator because this feature is only intended to make
/// it easier to build syntactically-correct SQL queries.

MYSQLPP_EXPORT std::ostream& operator <<(std::ostream& o,
		const SQLTypeAdapter& in);


template <class ST>
inline std::ostream&
operator <<(quote_type1 o, const Set<ST>& in)
{
	return *o.ostr << '\'' << in << '\'';
}

#endif // !defined(DOXYGEN_IGNORE)


/// \enum quote_only_type0
/// \anchor quote_only_manip
///
/// The 'quote_only' manipulator.
///
/// Similar to <a href="#quote_manip">quote manipulator</a>, except that
/// it doesn't escape special SQL characters.

enum quote_only_type0
{
	quote_only				///< insert into a std::ostream to single-quote next item
};


#if !defined(DOXYGEN_IGNORE)
// Doxygen will not generate documentation for this section.

struct quote_only_type1
{
	std::ostream* ostr;
	quote_only_type1(std::ostream* o) :
	ostr(o)
	{
	}
};


inline quote_only_type1
operator <<(std::ostream& o, quote_only_type0 /* esc */)
{
	return quote_only_type1(&o);
}


struct quote_only_type2
{
	SQLQueryParms* qparms;
	quote_only_type2(SQLQueryParms* p) :
	qparms(p)
	{
	}
};


inline quote_only_type2
operator <<(SQLQueryParms& p, quote_only_type0 /* esc */)
{
	return quote_only_type2(&p);
}


/// \brief Inserts a SQLTypeAdapter into a stream, quoting it unless it's
/// data that needs no quoting.
///
/// We make the decision to quote the data based on the in.quote_q()
/// flag.  You can set it yourself, but SQLTypeAdapter's ctors should set
/// it correctly for you.

MYSQLPP_EXPORT SQLQueryParms&
operator <<(quote_only_type2 p, SQLTypeAdapter& in);


MYSQLPP_EXPORT std::ostream&
operator <<(quote_only_type1 o, const SQLTypeAdapter& in);


template <class ST>
inline std::ostream&
operator <<(quote_only_type1 o, const Set<ST>& in)
{
	return *o.ostr << '\'' << in << '\'';
}

#endif // !defined(DOXYGEN_IGNORE)


/// \enum quote_double_only_type0
/// \anchor quote_double_manip
///
/// The 'double_quote_only' manipulator.
///
/// Similar to <a href="#quote_only_manip">quote_only manipulator</a>,
/// except that it uses double quotes instead of single quotes.
///
/// You might care to use it when you have MySQL's \c ANSI_QUOTES mode
/// enabled.  In that mode, single quotes are used only for string
/// literals, and double quotes for identifiers.  Otherwise,
/// \c quote_only and \c quote are quite sufficient.

enum quote_double_only_type0
{
	quote_double_only ///< insert into a std::ostream to double-quote next item
};


#if !defined(DOXYGEN_IGNORE)
// Doxygen will not generate documentation for this section.

struct quote_double_only_type1
{
	std::ostream* ostr;
	quote_double_only_type1(std::ostream* o) :
	ostr(o)
	{
	}
};


inline quote_double_only_type1
operator <<(std::ostream& o, quote_double_only_type0 /* esc */)
{
	return quote_double_only_type1(&o);
}


struct quote_double_only_type2
{
	SQLQueryParms *qparms;
	quote_double_only_type2(SQLQueryParms* p) :
	qparms(p)
	{
	}
};


inline quote_double_only_type2
operator <<(SQLQueryParms& p, quote_double_only_type0 /* esc */)
{
	return quote_double_only_type2(&p);
}


/// \brief Inserts a SQLTypeAdapter into a stream, double-quoting it (")
/// unless it's data that needs no quoting.
///
/// We make the decision to quote the data based on the in.quote_q()
/// flag.  You can set it yourself, but SQLTypeAdapter's ctors should set
/// it correctly for you.

MYSQLPP_EXPORT SQLQueryParms&
operator <<(quote_double_only_type2 p, SQLTypeAdapter& in);


MYSQLPP_EXPORT std::ostream&
operator <<(quote_double_only_type1 o, const SQLTypeAdapter& in);


template <class ST>
inline std::ostream&
operator <<(quote_double_only_type1 o, const Set<ST>& in)
{
	return *o.ostr << '"' << in << '"';
}

#endif // !defined(DOXYGEN_IGNORE)


/// \enum escape_type0
/// The 'escape' manipulator.
///
/// SQL-escapes following argument if it is of a data type that
/// may require escaping when inserted into a Query or SQLQueryParms
/// stream.  This is useful with string types, for example, to avoid
/// bad SQL when they contain special characters like single quotes,
/// nulls, and newlines.  Data types like integers which never benefit
/// from escaping don't get run through the escaping routine even if
/// you ask for it.

enum escape_type0 { escape };


#if !defined(DOXYGEN_IGNORE)
// Doxygen will not generate documentation for this section.

struct escape_type1
{
	std::ostream* ostr;
	escape_type1(std::ostream* o) :
	ostr(o)
	{
	}
};


inline escape_type1
operator <<(std::ostream& o, escape_type0 /* esc */)
{
	return escape_type1(&o);
}


struct escape_type2
{
	SQLQueryParms *qparms;
	escape_type2(SQLQueryParms* p) :
	qparms(p)
	{
	}
};


inline escape_type2
operator <<(SQLQueryParms& p, escape_type0 /* esc */)
{
	return escape_type2(&p);
}

#endif // !defined(DOXYGEN_IGNORE)


/// \brief Inserts a SQLTypeAdapter into a stream, escaping special SQL
/// characters
///
/// We actually only do the escaping if in.escape_q() returns true but
/// in.dont_escape is not.  If that is not the case, we insert the
/// string data directly.

MYSQLPP_EXPORT SQLQueryParms&
operator <<(escape_type2 p, SQLTypeAdapter& in);


/// \brief Inserts anything that can be converted to SQLTypeAdapter into
/// a stream, escaping special SQL characters as needed.

MYSQLPP_EXPORT std::ostream&
operator <<(escape_type1 o, const SQLTypeAdapter& in);


/// \enum do_nothing_type0
/// \anchor do_nothing_manip
///
/// The 'do_nothing' manipulator.
///
/// Does exactly what it says: nothing. Used as a dummy manipulator when
/// you are required to use some manipulator but don't want anything to
/// be done to the following item. When used with SQLQueryParms it will
/// make sure that it does not get formatted in any way, overriding any
/// setting set by the template query.

enum do_nothing_type0
{
	do_nothing				///< insert into a std::ostream to override manipulation of next item
};


#if !defined(DOXYGEN_IGNORE)
// Doxygen will not generate documentation for this section.

struct do_nothing_type1
{
	std::ostream* ostr;
	do_nothing_type1(std::ostream* o) :
	ostr(o)
	{
	}
};


inline do_nothing_type1
operator <<(std::ostream& o, do_nothing_type0 /* esc */)
{
	return do_nothing_type1(&o);
}


MYSQLPP_EXPORT std::ostream&
operator <<(do_nothing_type1 o, const SQLTypeAdapter& in);


struct do_nothing_type2
{
	SQLQueryParms *qparms;
	do_nothing_type2(SQLQueryParms* p) :
	qparms(p)
	{
	}
};


inline do_nothing_type2
operator <<(SQLQueryParms& p, do_nothing_type0 /* esc */)
{
	return do_nothing_type2(&p);
}


/// \brief Inserts a SQLTypeAdapter into a stream, with no escaping or
/// quoting.

MYSQLPP_EXPORT SQLQueryParms&
operator <<(do_nothing_type2 p, SQLTypeAdapter& in);

#endif // !defined(DOXYGEN_IGNORE)


/// \enum ignore_type0
/// \anchor ignore_manip
///
/// The 'ignore' manipulator.
///
/// Only valid when used with SQLQueryParms. It's a dummy manipulator
/// like the <a href="#do_nothing_manip">do_nothing manipulator</a>,
/// except that it will not override formatting set by the template
/// query.  It is simply ignored.

enum ignore_type0
{
	ignore					///< insert into a std::ostream as a dummy manipulator
};


#if !defined(DOXYGEN_IGNORE)
// Doxygen will not generate documentation for this section.

struct ignore_type2
{
	SQLQueryParms* qparms;
	ignore_type2(SQLQueryParms* p) :
	qparms(p)
	{
	}
};


inline ignore_type2
operator <<(SQLQueryParms& p, ignore_type0 /* esc */)
{
	return ignore_type2(&p);
}


/// \brief Inserts a SQLTypeAdapter into a stream, with no escaping or
/// quoting, and without marking the string as having been "processed".

MYSQLPP_EXPORT SQLQueryParms&
operator <<(ignore_type2 p, SQLTypeAdapter& in);

#endif // !defined(DOXYGEN_IGNORE)

} // end namespace mysqlpp

#endif
