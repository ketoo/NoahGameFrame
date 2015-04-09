// -------------------------------------------------------------------------
//    @FileName         :    NFIDENTID.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-07-11
//    @Module           :    NFIDENTID
//
// -------------------------------------------------------------------------

#ifndef _NF_IDENTID_H_
#define _NF_IDENTID_H_

#include "NFPlatform.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <boost/lexical_cast.hpp>
#include <boost/multiprecision/cpp_int.hpp>
//#pragma pack(push, 1)

struct NFIDENTID
{
	NFINT64 nHead64;
	NFINT64 nData64;
    //boost::multiprecision::int128_t xID;


    NFIDENTID()
    {
		nHead64 = 0;
        nData64 = 0;
    }

	NFIDENTID(NFINT64 nSvr, NFINT64 nData)
	{
		nHead64 = nSvr;
		nData64 = nData;
	}

    bool IsNull() const
    {
        return (0 == nData64) && (0 == nHead64);
    }

    bool operator == (const NFIDENTID& id) const
    {
        return (this->nData64 == id.nData64) && (this->nHead64 == id.nHead64);
    }

    bool operator != (const NFIDENTID& id) const
    {
        return (this->nData64 != id.nData64) || (this->nHead64 != id.nHead64);
    }

    bool operator < (const NFIDENTID& id) const
    {
        if (this->nHead64 == id.nHead64)
        {
            return this->nData64 < id.nData64;
        }

        return this->nHead64 < id.nHead64;
    }

    std::string ToString() const
    {
        return boost::lexical_cast<std::string>(this->nHead64) + "-" + boost::lexical_cast<std::string>(this->nData64);
    }

    bool FormString(const std::string& strID)
    {
        size_t nStrLength = strID.length();
        size_t nPos = strID.find('-');
        if (nPos < 0)
        {
            return false;
        }

        std::string strHead = strID.substr(0, nPos);
        std::string strData = "";
        if (nPos + 1 < nStrLength)
        {
            strData = strID.substr(nPos + 1, nStrLength - nPos);
        }

        try
        {
            nHead64 = boost::lexical_cast<NFINT64>(strHead);
            nData64 = boost::lexical_cast<NFINT64>(strData);

            return true;
        }
        catch (...)
        {
            return false;
        }
    }
};
//#pragma pack(pop)
#endif
