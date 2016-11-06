#ifndef NFC_TIME_H
#define NFC_TIME_H

#include <iostream>
#include <string>
#include <sstream>
#include <chrono>
#include "NFComm/NFCore/NFIObject.h"
#include "NFCalendar.h"

class NFTime
{
public:

	static NFINT64 GetTime();

	static std::string GetStr();
	static std::string GetStr_YMD();
	static std::string GetStr_HMS();
	static std::string GetStr_YMDHMSM();

public:
	static const NFINT64 WEEK = 7 * 24 * 60 * 60 * 1000L;
	static const NFINT64 DAY = 24 * 60 * 60 * 1000L;
	static const NFINT64 HOUR = 60 * 60 * 1000L;
	static const NFINT64 MINUTE = 60 * 1000L;
	static const NFINT64 SECOND = 1000L;
	static const NFINT64 MILLISECOND = 1L;
private:
	//NFCalendar calendar;
};

#endif





