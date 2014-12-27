/*=============================================================================
    Copyright (c) 2002 2004 2006 Joel de Guzman
    Copyright (c) 2004 Eric Niebler
    http://spirit.sourceforge.net/

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#include "files.hpp"
#include <boost/filesystem/fstream.hpp>
#include <boost/unordered_map.hpp>
#include <boost/range/algorithm/upper_bound.hpp>
#include <boost/range/algorithm/transform.hpp>
#include <boost/foreach.hpp>
#include <fstream>
#include <iterator>

namespace quickbook
{
    namespace
    {
        boost::unordered_map<fs::path, file_ptr> files;
    }

    // Read the first few bytes in a file to see it starts with a byte order
    // mark. If it doesn't, then write the characters we've already read in.
    // Although, given how UTF-8 works, if we've read anything in, the files
    // probably broken.

    template <typename InputIterator, typename OutputIterator>
    bool check_bom(InputIterator& begin, InputIterator end,
            OutputIterator out, char const* chars, int length)
    {
        char const* ptr = chars;

        while(begin != end && *begin == *ptr) {
            ++begin;
            ++ptr;
            --length;
            if(length == 0) return true;
        }

        // Failed to match, so write the skipped characters to storage:
        while(chars != ptr) *out++ = *chars++;

        return false;
    }

    template <typename InputIterator, typename OutputIterator>
    std::string read_bom(InputIterator& begin, InputIterator end,
            OutputIterator out)
    {
        if(begin == end) return "";

        const char* utf8 = "\xef\xbb\xbf" ;
        const char* utf32be = "\0\0\xfe\xff";
        const char* utf32le = "\xff\xfe\0\0";

        unsigned char c = *begin;
        switch(c)
        {
        case 0xEF: { // UTF-8
            return check_bom(begin, end, out, utf8, 3) ? "UTF-8" : "";
        }
        case 0xFF: // UTF-16/UTF-32 little endian
            return !check_bom(begin, end, out, utf32le, 2) ? "" :
                check_bom(begin, end, out, utf32le + 2, 2) ? "UTF-32" : "UTF-16";
        case 0: // UTF-32 big endian
            return check_bom(begin, end, out, utf32be, 4) ? "UTF-32" : "";
        case 0xFE: // UTF-16 big endian
            return check_bom(begin, end, out, utf32be + 2, 2) ? "UTF-16" : "";
        default:
            return "";
        }
    }

    // Copy a string, converting mac and windows style newlines to unix
    // newlines.

    template <typename InputIterator, typename OutputIterator>
    void normalize(InputIterator begin, InputIterator end,
            OutputIterator out)
    {
        std::string encoding = read_bom(begin, end, out);

        if(encoding != "UTF-8" && encoding != "")
        throw load_error(encoding +
            " is not supported. Please use UTF-8.");

        while(begin != end) {
            if(*begin == '\r') {
                *out++ = '\n';
                ++begin;
                if(begin != end && *begin == '\n') ++begin;
            }
            else {
                *out++ = *begin++;
            }
        }
    }

    file_ptr load(fs::path const& filename, unsigned qbk_version)
    {
        boost::unordered_map<fs::path, file_ptr>::iterator pos
            = files.find(filename);

        if (pos == files.end())
        {
            fs::ifstream in(filename, std::ios_base::in);

            if (!in)
                throw load_error("Could not open input file.");

            // Turn off white space skipping on the stream
            in.unsetf(std::ios::skipws);

            std::string source;
            normalize(
                std::istream_iterator<char>(in),
                std::istream_iterator<char>(),
                std::back_inserter(source));

            if (in.bad())
                throw load_error("Error reading input file.");

            bool inserted;

            boost::tie(pos, inserted) = files.emplace(
                filename, new file(filename, source, qbk_version));

            assert(inserted);
        }

        return pos->second;
    }

    file_position relative_position(
        std::string::const_iterator begin,
        std::string::const_iterator iterator)
    {
        file_position pos;
        std::string::const_iterator line_begin = begin;

        while (begin != iterator)
        {
            if (*begin == '\r')
            {
                ++begin;
                ++pos.line;
                line_begin = begin;
            }
            else if (*begin == '\n')
            {
                ++begin;
                ++pos.line;
                line_begin = begin;
                if (begin == iterator) break;
                if (*begin == '\r')
                {
                    ++begin;
                    line_begin = begin;
                }
            }
            else
            {
                ++begin;
            }
        }

        pos.column = iterator - line_begin + 1;
        return pos;
    }

    file_position file::position_of(std::string::const_iterator iterator) const
    {
        return relative_position(source.begin(), iterator);
    }

    // Mapped files.

    struct mapped_file_section
    {
        enum section_types {
            normal,
            empty,
            indented
        };
    
        std::string::size_type original_pos;
        std::string::size_type our_pos;
        section_types section_type;

        mapped_file_section(
                std::string::size_type original_pos,
                std::string::size_type our_pos,
                section_types section_type = normal) :
            original_pos(original_pos), our_pos(our_pos), section_type(section_type) {}

        std::string::size_type to_original_pos(std::string::size_type pos)
        {
            switch (section_type) {
            case normal:
                return pos - our_pos + original_pos;
            case empty:
                return original_pos;
            case indented:
                // Indented doesn't really work, but that's okay because we
                // currently don't break up indented code.
                assert(pos == our_pos);
                return pos - our_pos + original_pos;
            default:
                assert(false);
                return original_pos;
            }
        }
        
        // If 'to_original_pos' worked for indented blocks, this wouldn't
        // be necessary.
        file_position calculate_position(
                file_position const& original,
                file_position const& relative) const
        {
            switch (section_type) {
            case normal:
                return file_position(
                    original.line + relative.line - 1,
                    relative.line == 1 ?
                        original.column + relative.column - 1 :
                        relative.column);
            case empty:
                return original;
            case indented:
                return file_position(
                    original.line + relative.line - 1,
                    original.column + relative.column - 1);
            default:
                assert(false);
                return file_position();
            }
        }
    };

    struct mapped_section_original_cmp
    {
        bool operator()(mapped_file_section const& x,
                mapped_file_section const& y)
        {
            return x.original_pos < y.original_pos;
        }

        bool operator()(mapped_file_section const& x,
                std::string::size_type const& y)
        {
            return x.original_pos < y;
        }

        bool operator()(std::string::size_type const& x,
                mapped_file_section const& y)
        {
            return x < y.original_pos;
        }
    };

    struct mapped_section_pos_cmp
    {
        bool operator()(mapped_file_section const& x,
                mapped_file_section const& y)
        {
            return x.our_pos < y.our_pos;
        }

        bool operator()(mapped_file_section const& x,
                std::string::size_type const& y)
        {
            return x.our_pos < y;
        }

        bool operator()(std::string::size_type const& x,
                mapped_file_section const& y)
        {
            return x < y.our_pos;
        }
    };
    
    struct mapped_file : file
    {
        mapped_file(file_ptr original) :
            file(*original, std::string()),
            original(original), mapped_sections()
        {}

        file_ptr original;
        std::vector<mapped_file_section> mapped_sections;
        
        void add_empty_mapped_file_section(std::string::const_iterator pos) {
            std::string::size_type original_pos =
                pos - original->source.begin();
        
            if (mapped_sections.empty() ||
                    mapped_sections.back().section_type !=
                        mapped_file_section::empty ||
                    mapped_sections.back().original_pos != original_pos)
            {
                mapped_sections.push_back(mapped_file_section(
                        original_pos, source.size(),
                        mapped_file_section::empty));
            }
        }

        void add_mapped_file_section(std::string::const_iterator pos) {
            mapped_sections.push_back(mapped_file_section(
                pos - original->source.begin(), source.size()));
        }

        void add_indented_mapped_file_section(std::string::const_iterator pos) {
            mapped_sections.push_back(mapped_file_section(
                pos - original->source.begin(), source.size(),
                mapped_file_section::indented));
        }

        virtual file_position position_of(std::string::const_iterator) const;
    };

    namespace {
        std::list<mapped_file> mapped_files;
    }

    struct mapped_file_builder_data
    {
        mapped_file_builder_data() { reset(); }
        void reset() { new_file.reset(); }
    
        boost::intrusive_ptr<mapped_file> new_file;
    };

    mapped_file_builder::mapped_file_builder() : data(0) {}
    mapped_file_builder::~mapped_file_builder() { delete data; }

    void mapped_file_builder::start(file_ptr f)
    {
        if (!data) {
            data = new mapped_file_builder_data;
        }

        assert(!data->new_file);
        data->new_file = new mapped_file(f);
    }

    file_ptr mapped_file_builder::release()
    {
        file_ptr r = data->new_file;
        data->reset();
        return r;
    }

    void mapped_file_builder::clear()
    {
        data->reset();
    }
    
    bool mapped_file_builder::empty() const
    {
        return data->new_file->source.empty();
    }

    mapped_file_builder::pos mapped_file_builder::get_pos() const
    {
        return data->new_file->source.size();
    }
    
    void mapped_file_builder::add(char const* x, iterator pos)
    {
        data->new_file->add_empty_mapped_file_section(pos);
        data->new_file->source.append(x);
    }

    void mapped_file_builder::add(std::string const& x, iterator pos)
    {
        data->new_file->add_empty_mapped_file_section(pos);
        data->new_file->source.append(x);
    }

    void mapped_file_builder::add(iterator begin, iterator end)
    {
        data->new_file->add_mapped_file_section(begin);
        data->new_file->source.append(begin, end);
    }

    void mapped_file_builder::add(mapped_file_builder const& x)
    {
        add(x, 0, x.data->new_file->source.size());
    }

    void mapped_file_builder::add(mapped_file_builder const& x,
            pos begin, pos end)
    {
        assert(data->new_file->original == x.data->new_file->original);
        assert(begin <= x.data->new_file->source.size());
        assert(end <= x.data->new_file->source.size());

        if (begin != end)
        {
            std::vector<mapped_file_section>::iterator start =
                boost::upper_bound(x.data->new_file->mapped_sections,
                    begin, mapped_section_pos_cmp());
            assert(start != x.data->new_file->mapped_sections.begin());
            --start;
    
            std::string::size_type size = data->new_file->source.size();
    
            data->new_file->mapped_sections.push_back(mapped_file_section(
                    start->to_original_pos(begin), size,
                    start->section_type));
    
            for (++start; start != x.data->new_file->mapped_sections.end() &&
                    start->our_pos < end; ++start)
            {
                data->new_file->mapped_sections.push_back(mapped_file_section(
                    start->original_pos, start->our_pos - begin + size,
                    start->section_type));
            }
    
            data->new_file->source.append(
                x.data->new_file->source.begin() + begin,
            x.data->new_file->source.begin() + end);
        }
    }

    void mapped_file_builder::unindent_and_add(iterator begin, iterator end)
    {
        std::string program(begin, end);

        // Erase leading blank lines and newlines:
        std::string::size_type start = program.find_first_not_of(" \t");
        if (start != std::string::npos &&
            (program[start] == '\r' || program[start] == '\n'))
        {
            program.erase(0, start);
        }
        start = program.find_first_not_of("\r\n");
        program.erase(0, start);

        if (program.size() == 0)
            return; // nothing left to do

        // Get the first line indent
        std::string::size_type indent = program.find_first_not_of(" \t");
        std::string::size_type pos = 0;
        if (std::string::npos == indent)
        {
            // Nothing left to do here. The code is empty (just spaces).
            // We clear the program to signal the caller that it is empty
            // and return early.
            program.clear();
            return;
        }

        // Calculate the minimum indent from the rest of the lines
        do
        {
            pos = program.find_first_not_of("\r\n", pos);
            if (std::string::npos == pos)
                break;

            std::string::size_type n = program.find_first_not_of(" \t", pos);
            if (n != std::string::npos)
            {
                char ch = program[n];
                if (ch != '\r' && ch != '\n') // ignore empty lines
                    indent = (std::min)(indent, n-pos);
            }
        }
        while (std::string::npos != (pos = program.find_first_of("\r\n", pos)));

        // Trim white spaces from column 0..indent
        pos = 0;
        program.erase(0, indent);
        while (std::string::npos != (pos = program.find_first_of("\r\n", pos)))
        {
            if (std::string::npos == (pos = program.find_first_not_of("\r\n", pos)))
            {
                break;
            }

            std::string::size_type next = program.find_first_of("\r\n", pos);
            program.erase(pos, (std::min)(indent, next-pos));
        }

        data->new_file->add_indented_mapped_file_section(begin + indent);
        data->new_file->source.append(program);
    }

    file_position mapped_file::position_of(std::string::const_iterator pos) const
    {
        std::vector<mapped_file_section>::const_iterator section =
            boost::upper_bound(mapped_sections,
                std::string::size_type(pos - source.begin()),
                mapped_section_pos_cmp());
        assert(section != mapped_sections.begin());
        --section;

        return section->calculate_position(
            original->position_of(
                original->source.begin() + section->original_pos),
            relative_position(source.begin() + section->our_pos, pos)
        );
    }
}
