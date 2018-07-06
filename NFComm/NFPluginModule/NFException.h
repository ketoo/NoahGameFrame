// -------------------------------------------------------------------------
//    @FileName         :    NFException.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2018-07-03
//    @Module           :    NFException
//
// -------------------------------------------------------------------------

#ifndef NF_EXCEPTION_H
#define NF_EXCEPTION_H

#include <string>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <exception>

class NFException
{
private:
    static std::string _msgDelay;
    static std::string indexStr;
    static int index;

    std::string _msg;
public:
    NFException(const std::string& msg)
    {
        if (indexStr.empty())
        {
            index = 0;
            indexStr = "1";
        }

        index++;
        if (!_msgDelay.empty())
        {
            _msg.append(_msgDelay) + " ";
            _msgDelay.clear();
        }

        _msg.append(" NFException" + std::to_string(index) + " ");
        _msg.append(msg);
    }

    NFException(const std::string& msg, const char* file, const char* line)
    {
        if (indexStr.empty())
        {
            index = 0;
            indexStr = "1";
        }

        index++;
        if (!_msgDelay.empty())
        {
            _msg.append(_msgDelay);
            _msgDelay.clear();
        }

        _msg.append(" NFException" + std::to_string(index) + " ");
        _msg.append(msg);
        _msg.append(file);
        _msg.append(line);
    }

    static void ExceptionDelay(const std::string& msg, const char* file, const char* line)
    {
        if (indexStr.empty())
        {
            index = 0;
            indexStr = "1";
        }

        index++;

        _msgDelay.append(" NFException" + std::to_string(index) + " ");
        _msgDelay.append(msg);
        _msgDelay.append(file);
        _msgDelay.append(line);
    }

#if NF_PLATFORM == NF_PLATFORM_WIN
    char const * what() const { return msg_.c_str(); }
#else
    char const * what() const noexcept{ return _msg.c_str(); }
#endif
};

#endif
