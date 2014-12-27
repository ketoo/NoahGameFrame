#include <limits.h>
#include <time.h>
#include "NFTimer.h"

// static int64_t g_dwCurrentTime = 0;
// static int64_t g_dwLastTime     = 0;

struct  S2DWORD
{
    uint32_t dw0, dw1;
};

static union unionTime
{
    GameTime::Time64 time;
    S2DWORD          dword2;
} g_time;


int64_t GameTime::HQ_TimeGetTime()
{
#if defined(_WIN32) || defined(_WIN64)
    return time(NULL);
#else
    struct timeval start;
    gettimeofday(&start, NULL);
    return start.tv_sec;
#endif
}
// 
// void GameTime::MyGetCurrentTime()
// {
//     if (0 == g_dwCurrentTime)
//     {
//         g_time.time = 0;
//     }
// 
//     g_dwLastTime = g_dwCurrentTime;
// 
//     g_dwCurrentTime = HQ_TimeGetTime();
//     g_time.dword2.dw0 = g_dwCurrentTime;
// 
//     if (g_dwLastTime > g_dwCurrentTime)
//     {
//         g_time.dword2.dw1 ++;
//     }
// 
// }
// 
// GameTime::Time64 GameTime::GetCurrentTime64()
// {
//     return g_time.time;
// }

bool GameTime::IsOverDeadLine(Time64 deadTime)
{
    return IsLessOrEqualCurrentTime(deadTime);
}

bool GameTime::IsOverDeadLine(int64_t dwCurrentTime, Time64 n64DeadTime)
{
    return IsLessOrEqualCurrentTime(dwCurrentTime, n64DeadTime);
}

bool GameTime::IsPassCurrentTime(int64_t dwBaseTime, uint32_t dwKeepTime)
{
    return IsOverDeadLine((Time64)dwBaseTime + (Time64)dwKeepTime);
}

bool GameTime::IsPassCurrentTime(int64_t dwCurrentTime, int64_t dwBaseTime, uint32_t dwKeepTime)
{
    return IsOverDeadLine(dwCurrentTime, (Time64) dwBaseTime + (Time64)dwKeepTime);
}

uint32_t GameTime::GetLeftKeepTime(int64_t dwCurrentTime, int64_t dwLastTime, uint32_t dwKeepTime)
{
    int64_t dwPassTime = GetPassTime(dwCurrentTime, dwLastTime);

    uint32_t dwLeftTime = 0;
    if (dwKeepTime > dwPassTime)
    {
        dwLeftTime = dwKeepTime - dwPassTime;
    }

    return dwLeftTime;
}

int64_t GameTime::GetPassTime(int64_t dwCurrentTime, int64_t dwLastTime)
{
    int64_t dwpassTime = 0;
    if (dwCurrentTime >= dwLastTime)
    {
        dwpassTime = dwCurrentTime - dwLastTime;
    }
    else
    {
        dwpassTime = ULONG_MAX - dwLastTime;
    }
    return dwpassTime;
}

bool GameTime::IsLessOrEqualCurrentTime(Time64 n64DeadTime)
{
    return IsLessOrEqualCurrentTime(HQ_TimeGetTime(), n64DeadTime);
}

bool GameTime::IsLessOrEqualCurrentTime(int64_t dwCurrentTime, Time64 n64DeadTime)
{
    Time64 n64CurrentTime = dwCurrentTime;
    if (n64CurrentTime < 60000 && n64DeadTime > 0xffff0000)
    {
        n64CurrentTime += ULONG_MAX;
    }
    return n64CurrentTime >= n64DeadTime;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// TimeSpan
NFTimeSpan::NFTimeSpan() throw() :
    m_timeSpan(0)
{
}

NFTimeSpan::NFTimeSpan(time_t time) throw() :
    m_timeSpan(time)
{
}

NFTimeSpan::NFTimeSpan(uint32_t dwTime /* */)
{
    //Min TimeEx
    //毫秒级
    uint32_t dayTime = 24 * 60 * 60;
    int HoursTime = 60 * 60;
    int nMinsTime = 60;
    //int nSecsTime = 1;

    uint32_t lDays = dwTime / (dayTime);
    int nHours = (dwTime - lDays * (dayTime)) / HoursTime;
    int nMins = (dwTime - lDays * (dayTime) - nHours * HoursTime) / nMinsTime;
    int nSecs = (dwTime - lDays * (dayTime) - nHours * HoursTime - nMins * nMinsTime);


    NFTimeSpan TimeSpanT(lDays, nHours, nMins, nSecs);
    *this = TimeSpanT;

}


NFTimeSpan::NFTimeSpan(uint32_t lDays, int nHours, int nMins, int nSecs) throw()
{
    m_timeSpan = nSecs + 60 * (nMins + 60 * (nHours + time_t(24) * lDays));
}

int64_t NFTimeSpan::GetDays() const throw()
{
    return(m_timeSpan / (24 * 3600));
}

int64_t NFTimeSpan::GetTotalHours() const throw()
{
    return(m_timeSpan / 3600);
}

uint32_t NFTimeSpan::GetHours() const throw()
{
    return(uint32_t(GetTotalHours() - (GetDays() * 24)));
}

int64_t NFTimeSpan::GetTotalMinutes() const throw()
{
    return(m_timeSpan / 60);
}

uint32_t NFTimeSpan::GetMinutes() const throw()
{
    return(uint32_t(GetTotalMinutes() - (GetTotalHours() * 60)));
}

int64_t NFTimeSpan::GetTotalSeconds() const throw()
{
    return(m_timeSpan);
}

uint32_t NFTimeSpan::GetSeconds() const throw()
{
    return(uint32_t(GetTotalSeconds() - (GetTotalMinutes() * 60)));
}

time_t NFTimeSpan::GetTimeSpan() const throw()
{
    return(m_timeSpan);
}

NFTimeSpan NFTimeSpan::operator+(NFTimeSpan span) const throw()
{
    return(NFTimeSpan(m_timeSpan + span.m_timeSpan));
}

NFTimeSpan NFTimeSpan::operator-(NFTimeSpan span) const throw()
{
    return(NFTimeSpan(m_timeSpan - span.m_timeSpan));
}

NFTimeSpan& NFTimeSpan::operator+=(NFTimeSpan span) throw()
{
    m_timeSpan += span.m_timeSpan;
    return(*this);
}

NFTimeSpan& NFTimeSpan::operator-=(NFTimeSpan span) throw()
{
    m_timeSpan -= span.m_timeSpan;
    return(*this);
}

bool NFTimeSpan::operator==(NFTimeSpan span) const throw()
{
    return(m_timeSpan == span.m_timeSpan);
}

bool NFTimeSpan::operator!=(NFTimeSpan span) const throw()
{
    return(m_timeSpan != span.m_timeSpan);
}

bool NFTimeSpan::operator<(NFTimeSpan span) const throw()
{
    return(m_timeSpan < span.m_timeSpan);
}

bool NFTimeSpan::operator>(NFTimeSpan span) const throw()
{
    return(m_timeSpan > span.m_timeSpan);
}

bool NFTimeSpan::operator<=(NFTimeSpan span) const throw()
{
    return(m_timeSpan <= span.m_timeSpan);
}

bool NFTimeSpan::operator>=(NFTimeSpan span) const throw()
{
    return(m_timeSpan >= span.m_timeSpan);
}


/////////////////////////////////////////////////////////////////////////////
// TimeEx
/////////////////////////////////////////////////////////////////////////////

//TimeEx TimeEx::GetCurrentTime() throw()
//{
//    return TimeEx(time_t(NULL));
//}

NFTimeEx::NFTimeEx() throw() :
    m_time(0)
{
}

NFTimeEx::NFTimeEx(time_t time)  throw():
    m_time(time)
{
}

NFTimeEx::NFTimeEx(int nYear, int nMonth, int nDay, int nHour, int nMin, int nSec, int nDST)
{
    struct tm atm;

    atm.tm_sec = nSec;
    atm.tm_min = nMin;
    atm.tm_hour = nHour;
    atm.tm_mday = nDay;
    atm.tm_mon = nMonth - 1;        // tm_mon is 0 based
    atm.tm_year = nYear - 1900;     // tm_year is 1900 based
    atm.tm_isdst = nDST;

    m_time = mktime(&atm);
    if (m_time == -1)
    {
        //  AtlThrow(E_INVALIDARG);
    }
}

NFTimeEx::NFTimeEx(uint16_t wDosDate, uint16_t wDosTime, int nDST)
{
    struct tm atm;
    atm.tm_sec = (wDosTime & ~0xFFE0) << 1;
    atm.tm_min = (wDosTime & ~0xF800) >> 5;
    atm.tm_hour = wDosTime >> 11;

    atm.tm_mday = wDosDate & ~0xFFE0;
    atm.tm_mon = ((wDosDate & ~0xFE00) >> 5) - 1;
    atm.tm_year = (wDosDate >> 9) + 80;
    atm.tm_isdst = nDST;
    m_time = mktime(&atm);

    if (m_time == -1)
    {
        //      AtlThrow(E_INVALIDARG);
    }

}

NFTimeEx& NFTimeEx::operator=(time_t time) throw()
{
    m_time = time;

    return(*this);
}

NFTimeEx& NFTimeEx::operator+=(NFTimeSpan span) throw()
{
    m_time += span.GetTimeSpan();

    return(*this);
}

NFTimeEx& NFTimeEx::operator-=(NFTimeSpan span) throw()
{
    m_time -= span.GetTimeSpan();

    return(*this);
}

NFTimeSpan NFTimeEx::operator-(NFTimeEx time) const throw()
{
    return(NFTimeSpan(m_time - time.m_time));
}

NFTimeEx NFTimeEx::operator-(NFTimeSpan span) const throw()
{
    return(NFTimeEx(m_time - span.GetTimeSpan()));
}

NFTimeEx NFTimeEx::operator+(NFTimeSpan span) const throw()
{
    return(NFTimeEx(m_time + span.GetTimeSpan()));
}

bool NFTimeEx::operator==(NFTimeEx time) const throw()
{
    return(m_time == time.m_time);
}

bool NFTimeEx::operator!=(NFTimeEx time) const throw()
{
    return(m_time != time.m_time);
}

bool NFTimeEx::operator<(NFTimeEx time) const throw()
{
    return(m_time < time.m_time);
}

bool NFTimeEx::operator>(NFTimeEx time) const throw()
{
    return(m_time > time.m_time);
}

bool NFTimeEx::operator<=(NFTimeEx time) const throw()
{
    return(m_time <= time.m_time);
}

bool NFTimeEx::operator>=(NFTimeEx time) const throw()
{
    return(m_time >= time.m_time);
}

#if !_SECURE_ATL

//_ATL_INSECURE_DEPRECATE("Pass an output time structure to TimeEx::GetGmtTm")
struct tm* NFTimeEx::GetGmtTm() const throw()
{
    return gmtime(&m_time);
}

//_ATL_INSECURE_DEPRECATE("Pass an output time structure to TimeEx::GetLocalTm")
struct tm* NFTimeEx::GetLocalTm() const throw()
{
#pragma warning(push)
#pragma warning(disable : 4996)
    return localtime(&m_time);
#pragma warning(pop)
}

#endif // !_SECURE_ATL

struct tm* NFTimeEx::GetGmtTm(struct tm* ptm) const
{
#if _SECURE_ATL
    // Ensure ptm is valid
    ATLENSURE(ptm != NULL);

    if (ptm != NULL)
    {
        struct tm ptmTemp;
        errno_t err = gmtime_s(&ptmTemp, &m_time);

        // Be sure the call succeeded
        if (err != 0)
        {
            return NULL;
        }

        *ptm = ptmTemp;
        return ptm;
    }

    return NULL;
#else
    if (ptm != NULL)
    {
        struct tm* ptmTemp;
        ptmTemp = gmtime(&m_time);

        if (ptmTemp == NULL)
        {
            return NULL;
        }

        *ptm = *ptmTemp;

        return ptm;
    }
    else
    {
        return gmtime(&m_time);
    }
#endif
}

struct tm* NFTimeEx::GetLocalTm(struct tm* ptm) const
{
#if _SECURE_ATL
    // Ensure ptm is valid
    ATLENSURE(ptm != NULL);

    if (ptm != NULL)
    {
        struct tm ptmTemp;
        errno_t err = localtime_s(&ptmTemp, &m_time);

        if (err != 0)
        {
            return NULL;    // indicates that m_time was not initialized!
        }

        *ptm = ptmTemp;
        return ptm;
    }

    return NULL;
#else
    if (ptm != NULL)
    {
        struct tm* ptmTemp = localtime(&m_time);
        if (ptmTemp == NULL)
        {
            return NULL;    // indicates the m_time was not initialized!
        }

        *ptm = *ptmTemp;
        return ptm;
    }
    else
    {
        return localtime(&m_time);
    }
#endif
}

// bool TimeEx::GetAsSystemTime(SYSTEMTIME& timeDest) const throw()
// {
//  struct tm ttm;
//  struct tm* ptm;
//
//  ptm = GetLocalTm(&ttm);
//
//  if(!ptm) { return false; }
//
//  timeDest.wYear = (WORD) (1900 + ptm->tm_year);
//  timeDest.wMonth = (WORD) (1 + ptm->tm_mon);
//  timeDest.wDayOfWeek = (WORD) ptm->tm_wday;
//  timeDest.wDay = (WORD) ptm->tm_mday;
//  timeDest.wHour = (WORD) ptm->tm_hour;
//  timeDest.wMinute = (WORD) ptm->tm_min;
//  timeDest.wSecond = (WORD) ptm->tm_sec;
//  timeDest.wMilliseconds = 0;
//
//  return true;
// }

time_t NFTimeEx::GetTime() const throw()
{
    return(m_time);
}

int NFTimeEx::GetYear() const throw()
{
    struct tm ttm;
    struct tm* ptm;

    ptm = GetLocalTm(&ttm);
    return ptm ? (ptm->tm_year) + 1900 : 0 ;
}

int NFTimeEx::GetMonth() const throw()
{
    struct tm ttm;
    struct tm* ptm;

    ptm = GetLocalTm(&ttm);
    return ptm ? ptm->tm_mon + 1 : 0;
}

int NFTimeEx::GetDay() const throw()
{
    struct tm ttm;
    struct tm* ptm;

    ptm = GetLocalTm(&ttm);
    return ptm ? ptm->tm_mday : 0 ;
}

int NFTimeEx::GetHour() const throw()
{
    struct tm ttm;
    struct tm* ptm;

    ptm = GetLocalTm(&ttm);
    return ptm ? ptm->tm_hour : -1 ;
}

int NFTimeEx::GetMinute() const throw()
{
    struct tm ttm;
    struct tm* ptm;

    ptm = GetLocalTm(&ttm);
    return ptm ? ptm->tm_min : -1 ;
}

int NFTimeEx::GetSecond() const throw()
{
    struct tm ttm;
    struct tm* ptm;

    ptm = GetLocalTm(&ttm);
    return ptm ? ptm->tm_sec : -1 ;
}

int NFTimeEx::GetDayOfWeek() const throw()
{
    struct tm ttm;
    struct tm* ptm;

    ptm = GetLocalTm(&ttm);
    return ptm ? ptm->tm_wday + 1 : 0 ;
}

bool NFTimeEx::SameMinute(int64_t nTime)
{
    NFTimeEx xLastTime(nTime);
    if (xLastTime.GetMinute() == GetMinute()
        && xLastTime.GetDay() == GetDay()
        && xLastTime.GetMonth() == GetMonth()
        && xLastTime.GetYear() == GetYear())
    {
        return true;
    }

    return false;
}

bool NFTimeEx::SameDay(int64_t nTime)
{
    NFTimeEx xLastTime(nTime);
    if (xLastTime.GetDay() == GetDay()
        && xLastTime.GetMonth() == GetMonth()
        && xLastTime.GetYear() == GetYear())
    {
        return true;
    }

    return false;
}

bool NFTimeEx::SameHour(int64_t nTime)
{
    NFTimeEx xLastTime(nTime);
    if (xLastTime.GetHour() == GetHour()
        && xLastTime.GetDay() == GetDay()
        && xLastTime.GetMonth() == GetMonth()
        && xLastTime.GetYear() == GetYear())
    {
        return true;
    }

    return false;
}

bool NFTimeEx::SameWeek(int64_t nTime)
{
    if (SameDay(nTime))
    {
        return true;
    }

    //将2个日期都归到周1，看周1的日期是否相等[距离1970年][中国人做游戏说一周开始都是从周1开始...]
    NFTimeEx xLastTime(nTime);
    NFTimeEx xSourceTime(1970, 1, 1, 0, 0, 0);

    NFTimeEx xLastMondayTime = xLastTime - NFTimeSpan(xLastTime.GetDayOfWeek(), 0, 0, 0);
    NFTimeEx xThisMondayTime = *this - NFTimeSpan(GetDayOfWeek(), 0, 0, 0);

    if (xThisMondayTime.SameDay(xLastMondayTime.m_time))
    {
        return true;
    }

    return false;
}

bool NFTimeEx::SameMonth(int64_t nTime)
{
    NFTimeEx xLastTime(nTime);
    if (xLastTime.GetMonth() == GetMonth()
        && xLastTime.GetYear() == GetYear())
    {
        return true;
    }

    return false;
}

bool NFTimeEx::SameYear(int64_t nTime)
{
    NFTimeEx xLastTime(nTime);
    if (xLastTime.GetYear() == GetYear())
    {
        return true;
    }

    return false;
}

//////////////////////////////////////////////////////////////////////////
