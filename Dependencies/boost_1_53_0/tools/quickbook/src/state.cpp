/*=============================================================================
    Copyright (c) 2002 2004 2006 Joel de Guzman
    Copyright (c) 2004 Eric Niebler
    Copyright (c) 2005 Thomas Guest
    http://spirit.sourceforge.net/

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#include "state.hpp"
#include "state_save.hpp"
#include "quickbook.hpp"
#include "grammar.hpp"
#include "input_path.hpp"
#include <boost/filesystem/operations.hpp>

#if (defined(BOOST_MSVC) && (BOOST_MSVC <= 1310))
#pragma warning(disable:4355)
#endif

namespace quickbook
{
    char const* quickbook_get_date = "__quickbook_get_date__";
    char const* quickbook_get_time = "__quickbook_get_time__";

    unsigned qbk_version_n = 0; // qbk_major_version * 100 + qbk_minor_version

    state::state(fs::path const& filein_, fs::path const& xinclude_base_,
            string_stream& out_, id_manager& ids)
        : grammar_()

        , xinclude_base(xinclude_base_)

        , templates()
        , error_count(0)
        , anchors()
        , warned_about_breaks(false)
        , conditional(true)
        , ids(ids)
        , callouts()
        , callout_depth(0)

        , imported(false)
        , macro()
        , source_mode("c++")
        , source_mode_next()
        , current_file(0)
        , filename_relative(filein_.filename())

        , template_depth(0)
        , min_section_level(1)

        , out(out_)
        , phrase()
        , values(&current_file)
    {
        // add the predefined macros
        macro.add
            ("__DATE__", std::string(quickbook_get_date))
            ("__TIME__", std::string(quickbook_get_time))
            ("__FILENAME__", detail::path_to_generic(filename_relative))
        ;

        boost::scoped_ptr<quickbook_grammar> g(
            new quickbook_grammar(*this));
        grammar_.swap(g);
    }

    quickbook_grammar& state::grammar() const {
        return *grammar_;
    }

    bool state::add_dependency(fs::path const& f) {
        fs::path p = fs::absolute(f);
        bool found = fs::exists(fs::status(p));

        // Pop path sections from path until we find an existing
        // path, adjusting for any dot path sections.
        fs::path extra;
        int parent_count = 0;
        while (!fs::exists(fs::status(p))) {
            fs::path name = p.filename();
            p = p.parent_path();
            if (name == "..") {
                ++parent_count;
            }
            else if (name == ".") {
            }
            else if (parent_count) {
                --parent_count;
            }
            else {
                extra = name / extra;
            }
        }

        // If there are any left over ".." sections, then add them
        // on to the end of the real path, and trust Boost.Filesystem
        // to sort them out.
        while (parent_count) {
            p = p / "..";
            --parent_count;
        }

        p = fs::canonical(p) / extra;
        dependencies[p] |= found;
        return found;
    }

    file_state::file_state(quickbook::state& state, scope_flags scope)
        : state(state)
        , scope(scope)
        , qbk_version(qbk_version_n)
        , imported(state.imported)
        , current_file(state.current_file)
        , filename_relative(state.filename_relative)
        , xinclude_base(state.xinclude_base)
        , source_mode(state.source_mode)
        , macro()
    {
        if (scope & scope_macros) macro = state.macro;
        if (scope & scope_templates) state.templates.push();
        if (scope & scope_output) {
            state.out.push();
            state.phrase.push();
        }
        state.values.builder.save();
    }

    file_state::~file_state()
    {
        state.values.builder.restore();
        boost::swap(qbk_version_n, qbk_version);
        boost::swap(state.imported, imported);
        boost::swap(state.current_file, current_file);
        boost::swap(state.filename_relative, filename_relative);
        boost::swap(state.xinclude_base, xinclude_base);
        boost::swap(state.source_mode, source_mode);
        if (scope & scope_output) {
            state.out.pop();
            state.phrase.pop();
        }
        if (scope & scope_templates) state.templates.pop();
        if (scope & scope_macros) state.macro = macro;
    }

    template_state::template_state(quickbook::state& state)
        : file_state(state, file_state::scope_all)
        , template_depth(state.template_depth)
        , min_section_level(state.min_section_level)
    {
    }

    template_state::~template_state()
    {
        boost::swap(state.template_depth, template_depth);
        boost::swap(state.min_section_level, min_section_level);
    }
}
