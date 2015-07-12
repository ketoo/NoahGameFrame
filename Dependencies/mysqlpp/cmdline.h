/***********************************************************************
 cmdline.h - Declares the interface to the MySQL++'s command line
	parsing logic, used by the examples and the utility programs.
	Not intended for use by third parties!	If it breaks, you
	get to keep all the pieces.

 Copyright (c) 2007-2009 by Educational Technology Resources, Inc.
 Others may also hold copyrights on code in this file.  See the
 CREDITS.txt file in the top directory of the distribution for details.

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

#if !defined(MYSQLPP_CMDLINE_H)
#define MYSQLPP_CMDLINE_H

#include "common.h"

#include <string>
#include <vector>

#include <assert.h>

namespace mysqlpp {
	/// \brief Parses command line arguments and holds the results.
	///
	/// This class just contains common functionality and data
	/// structures; instantiable subclasses follow.
	class MYSQLPP_EXPORT CommandLineBase
	{
	public:
		//// Public types
		/// \brief Type for a list of arguments
		typedef std::vector<std::string> ArgumentList;
		/// \brief Iterator into ArgumentList
		typedef ArgumentList::const_iterator ArgumentListIt;

		//// Public interface
		/// \brief Get reference to list of command line arguments past
		/// the last flag and its possible argument.
		const ArgumentList& extra_args() const 
				{ return extra_args_; }

		/// \brief Return truthy value if command line was parsed
		/// successfully
		operator void*() const
		{
			return successful_ ? const_cast<bool*>(&successful_) : 0;
		}

	protected:
		//// Subclass interface
		/// \brief Hidden ctor to prevent instantiation
		CommandLineBase(int argc, char* const argv[], const char* opts) :
		argc_(argc),
		argv_(argv),
		opts_(opts),
		successful_(argc > 0 && argv && opts)
		{
			assert(successful_);
		}
		/// \brief Hidden dtor to prevent instantiation
		virtual ~CommandLineBase() { }

		/// \brief If object is still marked as "successful", save
		/// non-option arguments to extra_args_ list.  Subclass ctor
		/// should call this after the parse_next() loop gets EOF.
		void finish_parse();

		/// \brief Accessor for getopt()'s optarg global
		const char* option_argument() const;
		/// \brief Accessor for getopt()'s optind global
		int option_index() const;

		/// \brief Called by a subclass when encountering a command
		/// line parsing error.
		///
		/// Prints the passed message, calls subclass's print_usage(),
		/// and marks the object as no longer successful.
		void parse_error(const char* message = 0);

		/// \brief Wrapper for getopt()
		int parse_next() const;

		/// \brief Show a message explaining the program's proper usage
		virtual void print_usage() const = 0;

		/// \brief Get the file name of the program's executable
		const char* program_name() const { return argv_[0]; }

		/// \brief Returns true if nothing has gone wrong since calling
		/// the ctor.
		bool successful() const { return successful_; }

	private:
		//// Internal data
		int argc_;
		char* const* argv_;
		const char* opts_;
		bool successful_;
		ArgumentList extra_args_;
	};


	/// \brief Stuff related to MySQL++ examples specifically
	namespace examples {
		/// \brief Name of examples' DB
		extern MYSQLPP_EXPORT const char* db_name;

		/// \brief Command line parsing mechanism for ../examples/*.cpp
		class MYSQLPP_EXPORT CommandLine : public CommandLineBase
		{
		public:
			//// Public interface
			/// \brief Constructor
			CommandLine(int argc, char* const argv[],
					const char* user = 0, const char* pass = 0,
					const char* usage_extra = 0);

			/// \brief Show a message explaining the program's proper usage
			///
			/// Calls print_usage(const char*), passing along the
			/// "usage_extra" parameter passed to the ctor
			void print_usage() const { print_usage(usage_extra_); }

			/// \brief Show a message explaining the program's proper
			/// usage, with custom extra info after standard command
			/// line usage bits.
			void print_usage(const char* extra) const;

			/// \brief Return true if we're in "dtest" mode
			/// This happens when an example gets the -D flag, always
			/// passed by the dtest script to ask the programs it runs
			/// to suppress any nondeterministic output.
			bool dtest_mode() const { return dtest_mode_; }

			/// \brief Return the DB password (-p argument)
			const char* pass() const { return pass_; }

			/// \brief Return the -m flag value
			///
			/// This flag is currently only used by examples/deadlock,
			/// but it's really a nonspecific "mode" value, which could
			/// be used by other examples in the future.
			int run_mode() const { return run_mode_; }

			/// \brief Return the DB server name (-s argument)
			const char* server() const { return server_; }

			/// \brief Return the DB user name (-u argument)
			const char* user() const { return user_; }

		private:
			//// Internal data
			// Examples-specific command line parse results
			bool dtest_mode_;
			int run_mode_;
			const char* server_;
			const char* user_;
			const char* pass_;
			const char* usage_extra_;
		};
	} // end namespace mysqlpp::examples


	/// \brief Stuff specific to the ssqlsxlat tool
	namespace ssqlsxlat {
		/// \brief Command line parser for MySQL++'s ssqlsxlat tool
		class MYSQLPP_EXPORT CommandLine : public CommandLineBase
		{
		public:
			//// Public types
			/// \brief Types of inputs that ssqlsxlat will accept
			enum SourceSink {
				ss_unknown,	///< no known input type given yet
				ss_ssqls1,	///< a C++ file containing an SSQLS v1 declaration
				ss_ssqls2,	///< an SSQLS v2 file
				ss_table	///< an existing DB table schema
			};

			//// Public interface
			/// \brief Constructor
			CommandLine(int argc, char* const argv[]);

			/// \brief Show a message explaining the program's proper usage
			void print_usage() const;

			/// \brief Return the name of the input source
			///
			/// This can be a file name, a table name, etc.  Call
			/// input_source() to determine the proper interpretation.
			const char* input() const { return input_; }

			/// \brief The input source type
			SourceSink input_source() const { return input_source_; }

			/// \brief The output sink (destination) type
			SourceSink output_sink() const { return output_sink_; }

			/// \brief The base name of the output file
			const char* output() const { return output_; }

			/// \brief DB password, when input type is is_table
			const char* pass() const { return pass_; }

			/// \brief DB server name, when input type is is_table
			const char* server() const { return server_; }

			/// \brief DB user name, when input type is is_table
			const char* user() const { return user_; }

		private:
			//// Internal data: command line parse results
			const char* input_;
			const char* output_;
			const char* pass_;
			const char* server_;
			const char* user_;
			SourceSink input_source_;
			SourceSink output_sink_;
		};
	} // end namespace mysqlpp::ssqlsxlat
} // end namespace mysqlpp

#endif // !defined(MYSQLPP_CMDLINE_H)

