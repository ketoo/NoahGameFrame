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
#include "NFCMemory .h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <boost/lexical_cast.hpp>
#include <boost/multiprecision/cpp_int.hpp>
//#pragma pack(push, 1)

struct NFIDENTID
{
	NFINT64 nSvrID;
	NFINT64 nData64;
    //boost::multiprecision::int128_t xID;


    NFIDENTID()
    {
		nSvrID = 0;
        nData64 = 0;
    }

	NFIDENTID(NFINT64 nSvr, NFINT64 nData)
	{
		nSvrID = nSvr;
		nData64 = nData;
	}

    bool IsNull() const
    {
        return (0 == nData64) && (0 == nSvrID);
    }

    bool operator == (const NFIDENTID& id) const
    {
        return (this->nData64 == id.nData64) && (this->nSvrID == id.nSvrID);
    }

    bool operator != (const NFIDENTID& id) const
    {
        return (this->nData64 != id.nData64) || (this->nSvrID != id.nSvrID);
    }

    bool operator < (const NFIDENTID& id) const
    {
        if (this->nSvrID == id.nSvrID)
        {
            return this->nData64 < id.nData64;
        }

        return this->nSvrID < id.nSvrID;
    }

    std::string ToString() const
    {
        return boost::lexical_cast<std::string>(this->nSvrID) + "-" + boost::lexical_cast<std::string>(this->nData64);
    }

    bool FormString(const char* str)
    {
        //return boost::lexical_cast<std::string>(this->nSvrID) + "-" + boost::lexical_cast<std::string>(this->nData64);
    }
};
//#pragma pack(pop)
#endif
