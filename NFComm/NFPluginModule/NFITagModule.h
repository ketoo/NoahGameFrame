// -------------------------------------------------------------------------
//    @FileName         £º    NFITagModule.h
//    @Author           £º    LvSheng.Huang
//    @Date             £º    2012-12-15
//    @Module           £º    NFITagModule
//
// -------------------------------------------------------------------------

#ifndef _NFI_TAG_MODULE_H_
#define _NFI_TAG_MODULE_H_

#include <iostream>
#include "NFILogicModule.h"


class NFITagModule
    : public NFILogicModule
{

public:
	enum TAG_DEFINE
	{
		TAG_DEFAULT,
		TAG_BC,
		TAG_CHAT,
		TAG_TEAM,
	};

	virtual bool GetTagList(const std::string& strTag, NFList<NFIDENTID>& xList) = 0;
	virtual bool AddToTagList(const std::string& strTag, const NFIDENTID object) = 0;
	virtual bool RemoveFrmTagList(const std::string& strTag, const NFIDENTID object) = 0;

	virtual bool GetTagList(const TAG_DEFINE eTag, const std::string& strTag, NFList<NFIDENTID>& xList) = 0;
	virtual bool AddToTagList(const TAG_DEFINE eTag, const std::string& strTag, const NFIDENTID object) = 0;
	virtual bool RemoveFrmTagList(const TAG_DEFINE eTag, const std::string& strTag, const NFIDENTID object) = 0;

};

#endif