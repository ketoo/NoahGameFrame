#include <chrono>
#include "NFCalendar.h"

NFCalendar::NFCalendar()
{
	mnTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	mnTimeZone = mnDefaultTimeZone;
}
NFCalendar::NFCalendar(NFINT64 nTime)
{
	NFCalendar(nTime, mnDefaultTimeZone);
}
NFCalendar::NFCalendar(NFINT64 nTime, int nTimeZone)
{
	mnTime = nTime;
	mnTimeZone = nTimeZone;
}
NFCalendar::NFCalendar(std::string strTime)
{
	mnTime = 0;
	mnTimeZone = mnDefaultTimeZone;
	InitWithYMDHMSM(strTime);
}
NFCalendar::NFCalendar(std::string strTime, int nTimeZone)
{
	mnTime = 0;
	mnTimeZone = nTimeZone;
	InitWithYMDHMSM(strTime);

}

NFINT64 NFCalendar::GetTime()
{
	return mnTime;
}
std::string NFCalendar::GetStr()
{
	std::string str;
	std::stringstream ss;
	ss << this->Get(CalendarType::YEAR) << "-" << this->Get(CalendarType::MONTH) + 1 << "-" << this->Get(CalendarType::DAY) << " " << this->Get(CalendarType::HOUR) << ":" << this->Get(CalendarType::MINUTE) << ":" << this->Get(CalendarType::SECOND);
	return ss.str();
}
NFINT64 NFCalendar::Get(CalendarType CalendarType)
{
	int year = 1970;
	NFINT64 nFixTime = GetFixTime();
	switch (CalendarType)
	{
		case CalendarType::YEAR:
		{
			while (true)
			{
				int nDays = 365;
				if (IsLeapYear(year))
				{
					nDays++;
				}
				if (nFixTime < nDays*NFCalendar::DAY)
				{
					break;
				}
				nFixTime -= nDays*NFCalendar::DAY;
				year++;
			}
			return year;
		}
		case CalendarType::MONTH:
		{
			while (true)
			{
				int nDays = 365;
				if (IsLeapYear(year))
				{
					nDays++;
				}
				for (int month = 1; month <= 12; month++)
				{
					NFINT64 nMothDays;
					switch (month)
					{
						case 2:
						{
							if (nDays == 365)
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
					if (nFixTime < nMothDays*NFCalendar::DAY)
					{
						return month - 1;
					}
					nFixTime -= nMothDays*NFCalendar::DAY;
				}
				year++;
			}
		}
		case CalendarType::DAY:
		{
			while (true)
			{
				int nDays = 365;
				if (IsLeapYear(year))
				{
					nDays++;
				}
				for (int month = 1; month <= 12; month++)
				{
					NFINT64 nMothDays;
					switch (month)
					{
						case 2:
						{
							if (nDays == 365)
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
					if (nFixTime < nMothDays*NFCalendar::DAY)
					{
						return nFixTime / NFCalendar::DAY + 1;
					}
					nFixTime -= nMothDays*NFCalendar::DAY;
				}
				year++;
			}
		}
		case CalendarType::HOUR:
		{
			return (nFixTime%NFCalendar::DAY) / NFCalendar::HOUR;
		}
		case CalendarType::MINUTE:
		{
			return (nFixTime%NFCalendar::HOUR) / NFCalendar::MINUTE;
		}
		case CalendarType::SECOND:
		{
			return (nFixTime%NFCalendar::MINUTE) / NFCalendar::SECOND;
		}
		case CalendarType::MILLISECOND:
		{
			return (nFixTime%NFCalendar::SECOND) / NFCalendar::MILLISECOND;
		}
		case CalendarType::WEEK_SINCE_EPOCH:
		{
			return (mnTime + NFCalendar::DAY * 4) / NFCalendar::WEEK;// 1970.1.1 is thursday , sunday is first day of week
		}
		case CalendarType::DAY_SINCE_EPOCH:
		{
			return mnTime / NFCalendar::DAY;
		}
		case CalendarType::HOUR_SINCE_EPOCH:
		{
			return mnTime / NFCalendar::HOUR;
		}
		case CalendarType::DAY_OF_WEEK:
		{
			return ((nFixTime + NFCalendar::DAY * 4) % NFCalendar::WEEK) / NFCalendar::DAY + 1;// 1970.1.1 is thursday , sunday is first day of week ,start by 1
		}
		default:
		{
			return 0;
		}
	}
}

int NFCalendar::GetTimeZone()
{
	return mnTime;
}

void NFCalendar::SetTime(NFINT64 nTime)
{
	mnTime = nTime;
}
void NFCalendar::SetTimeZone(int nTimeZone)
{
	mnTimeZone = nTimeZone;
}

void NFCalendar::InitWithYMDHMSM(std::string strTime)
{
	std::vector<std::string> cells;
	split(strTime, cells, " ");
	if (cells.size() < 2)
	{
		return;
	}
	std::vector<std::string> cellsYMD;
	split(cells.at(0), cellsYMD, "-");
	if (cellsYMD.size() != 3)
	{
		return;
	}
	std::vector<std::string> cellsHMS;
	split(cells.at(1), cellsHMS, ":");
	if (cellsHMS.size() != 3)
	{
		return;
	}
	int nYear = lexical_cast<int>(cellsYMD[0]);
	int nMonth = lexical_cast<int>(cellsYMD[1]);
	int nDay = lexical_cast<int>(cellsYMD[2]);
	int nHour = lexical_cast<int>(cellsHMS[0]);
	int nMinute = lexical_cast<int>(cellsHMS[1]);
	int nSecond = lexical_cast<int>(cellsHMS[2]);
	int nMilliSecond = 0;
	if (cells.size() == 3)
	{
		nMilliSecond = lexical_cast<int>(cells[2]);
	}
	if (nYear < 1970)
	{
		return;
	}
	int nTmpYear = 1970;
	while (nTmpYear < nYear)
	{
		mnTime += NFCalendar::DAY * 365;
		if (IsLeapYear(nTmpYear))
		{
			mnTime += NFCalendar::DAY;
		}
		nTmpYear++;
	}
	bool bIsLeapYear = IsLeapYear(nYear);
	for (int month = 1; month < nMonth; month++)
	{
		NFINT64 nMothDays;
		switch (month)
		{
			case 2:
			{
				if (!bIsLeapYear)
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
		mnTime += NFCalendar::DAY*nMothDays;
	}
	mnTime += NFCalendar::DAY*(nDay - 1);
	mnTime += NFCalendar::HOUR*nHour;
	mnTime += NFCalendar::MINUTE*nMinute;
	mnTime += NFCalendar::SECOND*nSecond;
	mnTime += nMilliSecond;

	mnTime -= NFCalendar::HOUR*mnTimeZone;
}

NFINT64 NFCalendar::GetFixTime()
{
	return mnTime + mnTimeZone*NFCalendar::HOUR;
}

bool NFCalendar::IsLeapYear(int nYear)
{
	if ((nYear % 4 == 0) && (nYear % 100 != 0))
	{
		return true;
	}
	if (nYear % 400 == 0)
	{
		return true;
	}
	return false;
}
int NFCalendar::split(const string& str, vector<string>& ret_, string sep)
{
	if (str.empty())
	{
		return 0;
	}

	string tmp;
	string::size_type pos_begin = str.find_first_not_of(sep);
	string::size_type comma_pos = 0;

	while (pos_begin != string::npos)
	{
		comma_pos = str.find(sep, pos_begin);
		if (comma_pos != string::npos)
		{
			tmp = str.substr(pos_begin, comma_pos - pos_begin);
			pos_begin = comma_pos + sep.length();
		}
		else
		{
			tmp = str.substr(pos_begin);
			pos_begin = comma_pos;
		}

		if (!tmp.empty())
		{
			ret_.push_back(tmp);
			tmp.clear();
		}
	}
	return 0;
}