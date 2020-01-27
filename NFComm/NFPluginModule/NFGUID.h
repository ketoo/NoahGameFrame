/*
            This file is part of: 
                NoahFrame
            https://github.com/ketoo/NoahGameFrame

   Copyright 2009 - 2020 NoahFrame(NoahGameFrame)

   File creator: lvsheng.huang
   
   NoahFrame is open-source software and you can redistribute it and/or modify
   it under the terms of the License; besides, anyone who use this file/software must include this copyright announcement.

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#ifndef NF_IDENTID_H
#define NF_IDENTID_H

#include "NFPlatform.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

class NFGUID
{
private:
	static NFINT64 nInstanceID;
	static NFINT64 nGUIDIndex;

public:

    NFINT64 nData64;
    NFINT64 nHead64;

	static void SetInstanceID(NFINT64 id)
	{
		/*
		if (nInstanceID != 0)
		{
			std::cout << "ERROR-------------------- set instance id again!!!" << std::endl;
			return;
		}
		*/
		nInstanceID = id;
		nGUIDIndex = 0;
	}

    NFGUID()
    {
        nData64 = 0;
        nHead64 = 0;
    }

    NFGUID(NFINT64 nHeadData, NFINT64 nData)
    {
        nHead64 = nHeadData;
        nData64 = nData;
    }

    NFGUID(const NFGUID& xData)
    {
        nHead64 = xData.nHead64;
        nData64 = xData.nData64;
    }
  
    NFGUID(const std::string& strID)
    {
        FromString(strID);
    }

	static NFGUID CreateID()
	{
		int64_t value = 0;
		uint64_t time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();


		//value = time << 16;
		value = time * 1000000;


		//value |= nGUIDIndex++;
		value += nGUIDIndex++;

		//if (sequence_ == 0x7FFF)
		if (nGUIDIndex == 999999)
		{
			nGUIDIndex = 0;
		}

		NFGUID xID;
		xID.nHead64 = nInstanceID;
		xID.nData64 = value;

		return xID;
	}

    NFGUID& operator=(const NFGUID& xData)
    {
        nHead64 = xData.nHead64;
        nData64 = xData.nData64;

        return *this;
    }
  
    NFGUID& operator=(const std::string& strID)
    {
        FromString(strID);

        return *this;
    }

    const NFINT64 GetData() const
    {
        return nData64;
    }

    const NFINT64 GetHead() const
    {
        return nHead64;
    }

    void SetData(const NFINT64 nData)
    {
        nData64 = nData;
    }

    void SetHead(const NFINT64 nData)
    {
        nHead64 = nData;
    }

    bool IsNull() const
    {
        return 0 == nData64 && 0 == nHead64;
    }

    bool operator == (const NFGUID& id) const
    {
        return this->nData64 == id.nData64 && this->nHead64 == id.nHead64;
    }

    bool operator != (const NFGUID& id) const
    {
        return this->nData64 != id.nData64 || this->nHead64 != id.nHead64;
    }

    bool operator < (const NFGUID& id) const
    {
        if (this->nHead64 == id.nHead64)
        {
            return this->nData64 < id.nData64;
        }

        return this->nHead64 < id.nHead64;
    }

    std::string ToString() const
    {
        return lexical_cast<std::string>(nHead64) + "-" + lexical_cast<std::string>(nData64);
    }

    bool FromString(const std::string& strID)
    {
        size_t nStrLength = strID.length();
        size_t nPos = strID.find('-');
        if (nPos == std::string::npos)
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
            nHead64 = lexical_cast<NFINT64>(strHead);
            nData64 = lexical_cast<NFINT64>(strData);

            return true;
        }
        catch (...)
        {
            return false;
        }
    }
};
/*
NFINT64 NFGUID::nInstanceID = 0;
NFINT64 NFGUID::nGUIDIndex = 0;
*/
#endif
