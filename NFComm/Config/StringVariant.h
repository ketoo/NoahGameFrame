#ifndef __STRING_VARIANT_H__
#define __STRING_VARIANT_H__

#include <string>
#include <sstream>

/**
* The StringVariant class is a simple string-based variant implementation that allows
* the user to easily convert between simple numeric/string types.
*/

class StringVariant
{
    std::string data;
public:
    StringVariant() : data() {}

    template<typename ValueType>
    StringVariant(ValueType val)
    {
        std::ostringstream stream;
        stream << val;
        data.assign(stream.str());
    }

    template<typename ValueType>
    StringVariant& operator=(const ValueType val)
    {
        std::ostringstream stream;
        stream << val;
        data.assign(stream.str());

        return *this;
    }

    template<typename NumberType>
    NumberType toNumber() const
    {
        NumberType result = 0;
        std::istringstream stream(data);

        if (stream >> result)
        {
            return result;
        }
        else if (data == "yes" || data == "true")
        {
            return 1;
        }

        return 0;
    }

    std::string str() const
    {
        return data;
    }
};

#endif