#ifndef _NFCOMM_CONFIG_H_
#define  _NFCOMM_CONFIG_H_

#include <map>
#include <vector>
#include <fstream>
#include "StringVariant.h"

class NFConfig
{
public:
    NFConfig() {}

    // Loads key/value pairs from a file
    // Returns whether or not this operation was successful
    bool Load(const std::string& filename);

    // Use the [] operator to access values just like a map container
    // If a key does not exist, will return an empty StringVariant
    const StringVariant& operator[](const std::string& keyName) const;

    const StringVariant& operator[](const int keyIndex);

    const std::string& Key(const int keyIndex);

    // Use the [] operator to get/set values just like a map container
    StringVariant& operator[](const std::string& keyName)
    {
        return map[keyName];
    }

    const int Count()
    {
        return map.size();
    }

    // The actual map used to store key/value pairs
    std::map<std::string, StringVariant> map;
    std::vector<std::string> vec;
};

inline std::string trim(std::string source)
{
    std::string result = source.erase(source.find_last_not_of(" \t") + 1);
    return result.erase(0, result.find_first_not_of(" \t"));
}

inline bool NFConfig::Load(const std::string& filename)
{
    std::string line;
    std::string comment = "//";
    std::string delimiter = ":";

    std::ifstream file(filename.c_str());

    if (!file.is_open())
    {
        return false;
    }

    while (!file.eof())
    {
        getline(file, line);

        // Remove any comments
        size_t commIdx = line.find(comment);
        if (commIdx != std::string::npos)
        {
            line = line.substr(0, commIdx);
        }

        size_t delimIdx = line.find(delimiter);
        if (delimIdx == std::string::npos)
        {
            if (!line.empty())
            {
                vec.push_back(line);
            }
            continue;
        }

        std::string key = trim(line.substr(0, delimIdx));
        std::string value = trim(line.substr(delimIdx + 1));

        if (!key.empty())
        {
            map[key] = value;
        }
    }

    file.close();

    return true;
}

inline const StringVariant& NFConfig::operator[](const std::string& keyName) const
{
    std::map<std::string, StringVariant>::const_iterator iter = map.find(keyName);

    if (iter != map.end())
    {
        return iter->second;
    }

    static StringVariant empty;

    return empty;
}

inline const std::string& NFConfig::Key(const int keyIndex)
{
    static std::string empty = "";

    if (keyIndex >= map.size())
    {
        return empty;
    }

    std::map<std::string, StringVariant>::iterator iter = map.begin();
    for (int i = 0; i < keyIndex; i++)
    {
        iter ++;
    }

    if (iter != map.end())
    {
        return iter->first;
    }

    return empty;
}

inline const StringVariant& NFConfig::operator[](const int keyIndex)
{
    static StringVariant empty;

    if (keyIndex >= map.size())
    {
        return empty;
    }

    std::map<std::string, StringVariant>::iterator iter = map.begin();
    for (int i = 0; i < keyIndex; i++)
    {
        iter ++;
    }

    if (iter != map.end())
    {
        return iter->second;
    }

    return empty;
}

#endif
