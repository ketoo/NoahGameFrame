#ifndef NFC_CALENDAR_H
#define NFC_CALENDAR_H

#include <iostream>
#include <string>
#include "NFComm/NFCore/NFIObject.h"

enum CalendarType
{
	//-----------BASE-----------
	YEAR,
	MONTH,//start by 0
	DAY,
	HOUR,
	MINUTE,
	SECOND,
	MILLISECOND,
	//------------EX-------------
	WEEK_SINCE_EPOCH,//start by 0
	DAY_SINCE_EPOCH,//start by 0
	HOUR_SINCE_EPOCH,//start by 0
	DAY_OF_WEEK,//start by 1 ,1 means Sunday
};
class NFCalendar
{
public:

private:
	static const NFINT64 NWEEK = 7 * 24 * 60 * 60 * 1000L;
	static const NFINT64 NDAY = 24 * 60 * 60 * 1000L;
	static const NFINT64 NHOUR = 60 * 60 * 1000L;
	static const NFINT64 NMINUTE = 60 * 1000L;
	static const NFINT64 NSECOND = 1000L;
	static const int mnDefaultTimeZone = 8;
public:
	NFCalendar();
	NFCalendar(NFINT64 nTime);
	NFCalendar(NFINT64 nTime, int nTimeZone);
	NFCalendar(std::string strTime);
	NFCalendar(std::string strTime, int nTimeZone);


	NFINT64 GetTime();
	std::string GetStr();
	NFINT64 Get(CalendarType type);
	int GetTimeZone();

	void SetTime(NFINT64 nTime);
	void SetTimeZone(int nTimeZone);

private:
	void InitWithYMDHMSM(std::string strTime);
	NFINT64 GetFixTime();//add timeZone value
	bool IsLeapYear(int nYear);
	int split(const string& str, vector<string>& ret_, string sep);

private:
	NFINT64 mnTime;
	int mnTimeZone;
};

#endif



