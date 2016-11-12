// -------------------------------------------------------------------------
//    @FileName         :    NFMemoryCounter.hpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2016-11-08
//    @Module           :    NFMemoryCounter
//
// -------------------------------------------------------------------------

#ifndef NF_COUNTER_MEMORY_HPP
#define NF_COUNTER_MEMORY_HPP

#include <iostream>
#include <string>

class NFMemoryCounter
{
public:
    NFMemoryCounter(const std::string& strClassName)
    {
        std::map<std::string, int>::iterate it = mxCounter.find(strClassName);
        if(it != mxCounter.end())
        {
            it->second++;
        }
        else
        {
            mxCounter.insert(std::map<std::string, int>::typeof(strClassName, 1));
        }
    }
	
private:
    static std::map<std::string, int> mxCounter;
};

#endif