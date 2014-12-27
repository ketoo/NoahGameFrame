/*=============================================================================
    Copyright (c) 2002 2004 2006 Joel de Guzman
    Copyright (c) 2004 Eric Niebler
    Copyright (c) 2010 Daniel James
    http://spirit.sourceforge.net/

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#if !defined(BOOST_SPIRIT_QUICKBOOK_GRAMMARS_IMPL_HPP)
#define BOOST_SPIRIT_QUICKBOOK_GRAMMARS_IMPL_HPP

#include "grammar.hpp"
#include "cleanup.hpp"
#include "values.hpp"
#include <boost/spirit/include/classic_symbols.hpp>

namespace quickbook
{
    namespace cl = boost::spirit::classic;

    struct element_info
    {
        enum type_enum {
            nothing = 0,
            block = 1,
            conditional_or_block = 2,
            nested_block = 4,
            phrase = 8,
            maybe_block = 16
        };

        enum context {
            in_phrase = phrase | maybe_block,
            in_nested_block = phrase | maybe_block | nested_block,
            in_conditional = phrase | maybe_block | nested_block | conditional_or_block,
            in_block = phrase | maybe_block | nested_block | conditional_or_block | block,
            only_nested_block = nested_block,
            only_block = nested_block | conditional_or_block | block,
            only_list_block = nested_block | conditional_or_block,
            only_contextual_block = maybe_block | nested_block | conditional_or_block | block
        };

        element_info()
            : type(nothing), rule(), tag(0) {}

        element_info(
                type_enum t,
                cl::rule<scanner>* r,
                value::tag_type tag = value::default_tag,
                unsigned int v = 0)
            : type(t), rule(r), tag(tag), qbk_version(v) {}

        type_enum type;
        cl::rule<scanner>* rule;
        value::tag_type tag;
        unsigned int qbk_version;
    };

    struct quickbook_grammar::impl
    {
        quickbook::state& state;
        cleanup cleanup_;

        // Main Grammar
        cl::rule<scanner> block_start;
        cl::rule<scanner> phrase_start;
        cl::rule<scanner> nested_phrase;
        cl::rule<scanner> inline_phrase;
        cl::rule<scanner> paragraph_phrase;
        cl::rule<scanner> extended_phrase;
        cl::rule<scanner> table_title_phrase;
        cl::rule<scanner> inside_preformatted;
        cl::rule<scanner> inside_paragraph;
        cl::rule<scanner> command_line;
        cl::rule<scanner> attribute_value_1_7;
        cl::rule<scanner> escape;
        cl::rule<scanner> raw_escape;

        // Miscellaneous stuff
        cl::rule<scanner> hard_space;
        cl::rule<scanner> space;
        cl::rule<scanner> blank;
        cl::rule<scanner> eol;
        cl::rule<scanner> phrase_end;
        cl::rule<scanner> comment;
        cl::rule<scanner> line_comment;
        cl::rule<scanner> macro_identifier;

        // Element Symbols       
        cl::symbols<element_info> elements;
        
        // Doc Info
        cl::rule<scanner> doc_info_details;
        
        impl(quickbook::state&);

    private:

        void init_main();
        void init_block_elements();
        void init_phrase_elements();
        void init_doc_info();
    };
}

#endif // BOOST_SPIRIT_QUICKBOOK_GRAMMARS_HPP
