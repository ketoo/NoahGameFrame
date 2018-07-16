// -------------------------------------------------------------------------
//    @FileName         :    NFException.hpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2016-12-22
//    @Module           :    NFException
//
// -------------------------------------------------------------------------

#ifndef NF_EXCEPTION_HPP
#define NF_EXCEPTION_HPP

#include <string>
#include <cstdarg>

class NFException
{
public:
	NFException(const char *format, ...)
	{
		char buf[1024] = {0};
		va_list args;
		va_start(args, format);
		vsprintf(buf, format, args);
		message = buf;
		va_end(args);
	}
	std::string GetMessage() { return message; }
protected:
	std::string message;
};

#endif