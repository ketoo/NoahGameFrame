#ifndef SMART_ENUM
#define SMART_ENUM

#include <stdarg.h>
#include <algorithm>
#include <string> 
#include <vector>
#include <sstream>
#include <map>
#include <iostream>

#define NF_SMART_ENUM(EnumName, ...)                                   \
class EnumName                                                      \
{                                                                   \
public:                                                             \
    EnumName() : value(0) {}                                        \
    EnumName(int x) : value(x) {}                                   \
public:                                                             \
    enum {__VA_ARGS__};                                             \
private:                                                            \
    static void initMap(std::map<int, std::string>& tmp)                     \
    {                                                               \
        using namespace std;                                        \
                                                                    \
        int val = 0;                                                \
        string buf_1, buf_2, str = #__VA_ARGS__;                    \
        replace(str.begin(), str.end(), '=', ' ');                  \
        stringstream stream(str);                                   \
        vector<string> strings;                                     \
        while (getline(stream, buf_1, ','))                         \
            strings.push_back(buf_1);                               \
        for(vector<string>::iterator it = strings.begin();          \
                                                it != strings.end(); \
                                                ++it)                \
        {                                                           \
            buf_1.clear(); buf_2.clear();                           \
            stringstream localStream(*it);                          \
            localStream>> buf_1 >> buf_2;                           \
            if(buf_2.size() > 0)                                    \
                val = atoi(buf_2.c_str());                          \
            tmp[val++] = buf_1;                                     \
        }                                                           \
    }                                                               \
    int value;                                                      \
public:                                                             \
    operator int () const { return value; }                         \
    std::string toString(void) const {                              \
            return toString(value);                                 \
    }                                                               \
    static std::string toString(int aInt)                           \
    {                                                               \
        return nameMap()[aInt];                                     \
    }                                                               \
    static EnumName fromString(const std::string& s)                \
    {                                                               \
        auto it = find_if(nameMap().begin(), nameMap().end(), [s](const std::pair<int,std::string>& p) { \
            return p.second == s;                                   \
        });                                                         \
        if (it == nameMap().end()) {                                \
        /*value not found*/                                         \
            throw EnumName::Exception();                            \
        } else {                                                    \
            return EnumName(it->first);                             \
        }                                                           \
    }                                                               \
    class Exception : public std::exception {};                     \
    static std::map<int,std::string>& nameMap() {                   \
      static std::map<int,std::string> nameMap0;                    \
      if (nameMap0.size() ==0) initMap(nameMap0);                   \
      return nameMap0;                                              \
    }                                                               \
    static std::vector<EnumName> allValues() {                      \
      std::vector<EnumName> x{ __VA_ARGS__ };                       \
      return x;                                                     \
    }                                                               \
    bool operator<(const EnumName a) const { return (int)*this < (int)a; } \
}; 


#endif