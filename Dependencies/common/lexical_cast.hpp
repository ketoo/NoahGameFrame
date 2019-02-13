#ifndef CPP_11_LEXICAL_CAST_HPP
#define CPP_11_LEXICAL_CAST_HPP

#include <type_traits>
#include <string>
#include <cstdlib>
#include <algorithm>
#include <stdexcept>
#include <cctype>
#include <cstring>
using namespace std;

namespace detail
{
    static const char* strue = "true";
    static const char* sfalse = "false";

    template <typename To, typename From>
    struct Converter
    {
    };

    //to numeric
    template <typename From>
    struct Converter<int, From>
    {
        static int convert(const From& from)
        {
            return std::atoi(from);
        }
    };

    template<>
    struct Converter<int, std::string>
    {
        static int convert(const std::string& from)
        {
            return std::atoi(from.c_str());
        }
    };

    template <typename From>
    struct Converter<long, From>
    {
        static long convert(const From& from)
        {
            return std::atol(from);
        }
    };

    template <>
    struct Converter<long, std::string>
    {
        static long convert(const std::string& from)
        {
            return std::atol(from.c_str());
        }
    };

    template <typename From>
    struct Converter<long long, From>
    {
        static long long convert(const From& from)
        {
#if defined(_MSC_VER) && _MSC_VER <= 1700 // vs2012
            return _atoi64(from);
#else
            return std::atoll(from);
#endif
        }
    };

    template <>
    struct Converter<long long, std::string>
    {
        static long long convert(const std::string& from)
        {
#if defined(_MSC_VER) && _MSC_VER <= 1700 // vs2012
            return _atoi64(from.c_str());
#else
            return std::atoll(from.c_str());
#endif
        }
    };

    template <typename From>
    struct Converter<double, From>
    {
        static double convert(const From& from)
        {
            return std::atof(from);
        }
    };

    template <>
    struct Converter<double, std::string>
    {
        static double convert(const std::string& from)
        {
            return std::atof(from.c_str());
        }
    };

    template <typename From>
    struct Converter<float, From>
    {
        static float convert(const From& from)
        {
            return (float)std::atof(from);
        }
    };

    template <>
    struct Converter<float, std::string>
    {
        static float convert(const std::string& from)
        {
            return float(std::atof(from.c_str()));
        }
    };

    //to bool
    template <typename From>
    struct Converter<bool, From>
    {
        static typename std::enable_if<std::is_integral<From>::value, bool>::type convert(From from)
        {
            return !!from;
        }
    };

    static bool checkbool(const char* from, const size_t len, const char* s)
    {
        for (size_t i = 0; i < len; i++)
        {
            if (from[i] != s[i])
            {
                return false;
            }
        }

        return true;
    }

    static bool convert(const char* from)
    {
        size_t len = strlen(from);
        //if (len != 4 && len != 5)
        //    throw std::invalid_argument("argument is invalid");

        bool r = true;
        if (len == 4)
        {// "true"
            r = checkbool(from, len, strue);

            if (r)
                return true;
        }
        else if (len == 5)
        {//"false"
            r = checkbool(from, len, sfalse);

            if (r)
                return false;
        }
        else
        {
            int value = Converter<int, const char*>::convert(from);
            return (value > 0);
        }

        throw std::invalid_argument("argument is invalid");
    }

    template <>
    struct Converter<bool, string>
    {
        static bool convert(const string& from)
        {
            return detail::convert(from.c_str());
        }
    };

    template <>
    struct Converter<bool, const char*>
    {
        static bool convert(const char* from)
        {
            return detail::convert(from);
        }
    };

    template <>
    struct Converter<bool, char*>
    {
        static bool convert(char* from)
        {
            return detail::convert(from);
        }
    };

    template <unsigned N>
    struct Converter<bool, const char[N]>
    {
        static bool convert(const char(&from)[N])
        {
            return detail::convert(from);
        }
    };

    template <unsigned N>
    struct Converter<bool, char[N]>
    {
        static bool convert(const char(&from)[N])
        {
            return detail::convert(from);
        }
    };

    //to string
    template <typename From>
    struct Converter<string, From>
    {
        static string convert(const From& from)
        {
            return std::to_string(from);
        }
    };
}

template <typename To, typename From>
typename std::enable_if<!std::is_same<To, From>::value, To>::type lexical_cast(const From& from)
{
    return detail::Converter<To, From>::convert(from);
}

template <typename To, typename From>
typename std::enable_if<std::is_same<To, From>::value, To>::type lexical_cast(const From& from)
{
    return from;
}

//test code
//void test()
//{
//    cout<<lexical_cast<int>(1)<<endl;
//    cout << lexical_cast<int>("1") << endl;
//    cout << lexical_cast<long>("1") << endl;
//    cout << lexical_cast<string>(1) << endl;
//    cout << lexical_cast<bool>(1) << endl;
//    cout << lexical_cast<double>("1.2") << endl;
//    cout << lexical_cast<float>("1.2") << endl;
//    string s = "true";
//    cout << lexical_cast<bool>(s) << endl;
//    char* p = "false";
//    cout << lexical_cast<bool>(p) << endl;
//    const char* q = "false";
//    cout << lexical_cast<bool>(q) << endl;
//    cout << lexical_cast<bool>("false") << endl;
//    cout << lexical_cast<bool>("test") << endl;
//}
//
//int main()
//{
//    try
//    {
//        test();
//    }
//    catch (const std::exception& e)
//    {
//        cout << e.what() << endl;
//    }
//
//    return 0;
//}

#endif // !CPP_11_LEXICAL_CAST_HPP
