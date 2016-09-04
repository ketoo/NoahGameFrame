#ifndef NFC_CALENDAR_H
#define NFC_CALENDAR_H

#include <iostream>
#include <string>
#include "NFComm/NFCore/NFIObject.h"

class NFCalendar
{
public:
	enum TYPE
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
public:
	NFCalendar();
	NFCalendar(NFINT64 nTime);
	NFCalendar(NFINT64 nTime, NFINT32 nTimeZone);

	NFINT64 Get(TYPE type);
private:
	NFINT64 GetFixTime();;//add timeZone value
private:
	static const NFINT32 mnDefaultTimeZone = 8;

	NFINT32 mnTimeZone;
	NFINT64 mnTime;
};

#endif



