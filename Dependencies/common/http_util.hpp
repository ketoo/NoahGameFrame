#pragma once
#include <unordered_map>
namespace http
{
    namespace util
    {
        inline char tolower(unsigned char c)
        {
            return static_cast<char>(std::tolower(c));
        }

        template<typename String>
        inline bool iequal_string(const String&str1, const String& str2)
        {
            if (str1.size() != str2.size())
                return false;

            auto iter1begin = str1.begin();
            auto iter2begin = str2.begin();

            auto iter1end = str1.end();
            auto iter2end = str2.end();

            for (; iter1begin != iter1end && iter2begin != iter2end; ++iter1begin, ++iter2begin)
            {
                if (!(toupper(*iter1begin) == toupper(*iter2begin)))
                {
                    return false;
                }
            }
            return true;
        }

        template<typename String>
        struct ihash_string_functor
        {
            size_t operator()(const String &str) const noexcept
            {
                size_t h = 0;
                std::hash<int> hash;
                for (auto c : str)
                    h ^= hash(tolower(c)) + 0x9e3779b9 + (h << 6) + (h >> 2);
                return h;
            }
        };

        template<typename String>
        struct iequal_string_functor
        {
            bool operator()(const String &str1, const String &str2) const noexcept
            {
                return iequal_string(str1, str2);
            }
        };

        using iequal_string_functor_t = iequal_string_functor<std::string>;
        using iequal_string_view_functor_t = iequal_string_functor<string_view_t>;

        using ihash_string_view_functor_t = ihash_string_functor<string_view_t>;
        using  case_insensitive_multimap_view = std::unordered_multimap<string_view_t, string_view_t, ihash_string_view_functor_t, iequal_string_view_functor_t>;

        string_view_t readline(const char* data, size_t size, size_t& readpos)
        {
            string_view_t strref(data + readpos, size);
            size_t pos = strref.find("\r\n");
            if (pos != string_view_t::npos)
            {
                readpos += (pos + 2);
                return string_view_t(strref.data(), pos);
            }
            pos = size;
            readpos += pos;
            return string_view_t(strref.data(), pos);
        }

        class http_header {
        public:
            /// Parse header fields
            static case_insensitive_multimap_view parse(string_view_t sv) noexcept {
                case_insensitive_multimap_view result;
                auto data = sv.data();
                auto size = sv.size();
                size_t rpos = 0;
                string_view_t line = readline(data, size, rpos);
                size_t param_end = 0;
                while ((param_end = line.find(':')) != string_view_t::npos) {
                    size_t value_start = param_end + 1;
                    if (value_start < line.size()) {
                        if (line[value_start] == ' ')
                            value_start++;
                        if (value_start < line.size())
                            result.emplace(line.substr(0, param_end), line.substr(value_start, line.size() - value_start));
                    }
                    line = readline(data, size, rpos);
                }
                return result;
            }
        };

        class request_parser
        {
        public:
            /// Parse request line and header fields
            static bool parse(string_view_t sv, string_view_t& method, string_view_t& path, string_view_t& query_string, string_view_t& http_version, case_insensitive_multimap_view& header) noexcept
            {
                auto data = sv.data();
                auto size = sv.size();
                size_t rpos = 0;
                string_view_t line = readline(data, size, rpos);

                size_t method_end;
                if ((method_end = line.find(' ')) != string_view_t::npos)
                {
                    method = line.substr(0, method_end);
                    size_t query_start = string_view_t::npos;
                    size_t path_and_query_string_end = string_view_t::npos;
                    for (size_t i = method_end + 1; i < line.size(); ++i)
                    {
                        if (line[i] == '?' && (i + 1) < line.size())
                        {
                            query_start = i + 1;
                        }
                        else if (line[i] == ' ')
                        {
                            path_and_query_string_end = i;
                            break;
                        }
                    }
                    if (path_and_query_string_end != string_view_t::npos)
                    {
                        if (query_start != string_view_t::npos) {
                            path = line.substr(method_end + 1, query_start - method_end - 2);
                            query_string = line.substr(query_start, path_and_query_string_end - query_start);
                        }
                        else
                            path = line.substr(method_end + 1, path_and_query_string_end - method_end - 1);
                        size_t protocol_end;
                        if ((protocol_end = line.find('/', path_and_query_string_end + 1)) != string_view_t::npos) {
                            if (line.compare(path_and_query_string_end + 1, protocol_end - path_and_query_string_end - 1, "HTTP") != 0)
                                return false;
                            http_version = line.substr(protocol_end + 1, line.size() - protocol_end - 1);
                        }
                        else
                            return false;

                        header = http_header::parse(string_view_t{ data + rpos,size - rpos });
                    }
                    else
                        return false;
                }
                else
                    return false;
                return true;
            }
        };

        template<typename TMap, typename TKey, typename TValue>
        inline bool try_get_header(const TMap& map, const TKey& key, TValue& value)
        {
            auto iter = map.find(key);
            if (iter != map.end())
            {
                value = iter->second;
                return true;
            }
            return false;
        }
    }
}



