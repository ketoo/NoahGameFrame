#include "NFCalendar.h"
#include "NFTime.h"

NFCalendar::NFCalendar()
{
	mnTime = NFTime::GetTime();
	mnTimeZone = mnDefaultTimeZone;
}
NFCalendar::NFCalendar(NFINT64 nTime)
{
	mnTime = nTime;
	mnTimeZone = mnDefaultTimeZone;
}
NFCalendar::NFCalendar(NFINT64 nTime, NFINT32 nTimeZone)
{
	mnTime = nTime;
	mnTimeZone = nTimeZone;
}

NFINT64 NFCalendar::Get(TYPE type)
{
	int year = 1970;
	NFINT64 nFixTime = GetFixTime();
	switch (type)
	{
		case TYPE::YEAR:
		{
			while (true)
			{
				int days = 365;
				if (((year % 4 == 0) && (year % 100 != 0)) || (year % 400 == 0))
				{
					days++;
				}
				if (nFixTime < days*NFTime::DAY)
				{
					break;
				}
				nFixTime -= days*NFTime::DAY;
				year++;
			}
			return year;
		}
		case TYPE::MONTH:
		{
			while (true)
			{
				int days = 365;
				if (((year % 4 == 0) && (year % 100 != 0)) || (year % 400 == 0))
				{
					days++;
				}
				for (int month = 1; month <= 12; month++)
				{
					NFINT64 nMothDays;
					switch (month)
					{
						case 2:
						{
							if (days == 365)
							{
								nMothDays = 28;
							}
							else
							{
								nMothDays = 29;
							}
						}
						break;
						case 1:
						case 3:
						case 5:
						case 7:
						case 8:
						case 10:
						case 12:
						{
							nMothDays = 31;
						}
						break;
						default:
						{
							nMothDays = 30;
						}
						break;
					}
					if (nFixTime < nMothDays*NFTime::DAY)
					{
						return month - 1;
					}
					nFixTime -= nMothDays*NFTime::DAY;
				}
				year++;
			}
		}
		case TYPE::DAY:
		{
			while (true)
			{
				int days = 365;
				if (((year % 4 == 0) && (year % 100 != 0)) || (year % 400 == 0))
				{
					days++;
				}
				for (int month = 1; month <= 12; month++)
				{
					NFINT64 nMothDays;
					switch (month)
					{
						case 2:
						{
							if (days == 365)
							{
								nMothDays = 28;
							}
							else
							{
								nMothDays = 29;
							}
						}
						break;
						case 1:
						case 3:
						case 5:
						case 7:
						case 8:
						case 10:
						case 12:
						{
							nMothDays = 31;
						}
						break;
						default:
						{
							nMothDays = 30;
						}
					}
					if (nFixTime < nMothDays*NFTime::DAY)
					{
						return nFixTime / NFTime::DAY+1;
					}
					nFixTime -= nMothDays*NFTime::DAY;
				}
				year++;
			}
		}
		case TYPE::HOUR:
		{
			return (nFixTime%NFTime::DAY) / NFTime::HOUR;
		}
		case TYPE::MINUTE:
		{
			return (nFixTime%NFTime::HOUR) / NFTime::MINUTE;
		}
		case TYPE::SECOND:
		{
			return (nFixTime%NFTime::MINUTE) / NFTime::SECOND;
		}
		case TYPE::MILLISECOND:
		{
			return (nFixTime%NFTime::SECOND) / NFTime::MILLISECOND;
		}
		case TYPE::WEEK_SINCE_EPOCH:
		{
			return (mnTime + NFTime::DAY * 4) / NFTime::WEEK;// 1970.1.1 is thursday , sunday is first day of week
		}
		case TYPE::DAY_SINCE_EPOCH:
		{
			return mnTime / NFTime::DAY;
		}
		case TYPE::HOUR_SINCE_EPOCH:
		{
			return mnTime / NFTime::HOUR;
		}
		case TYPE::DAY_OF_WEEK:
		{
			return ((nFixTime+NFTime::DAY*4)%NFTime::WEEK) / NFTime::DAY +1 ;// 1970.1.1 is thursday , sunday is first day of week ,start by 1
		}
	}
}
NFINT64 NFCalendar::GetFixTime()
{
	return mnTime + mnTimeZone*NFTime::HOUR;
}