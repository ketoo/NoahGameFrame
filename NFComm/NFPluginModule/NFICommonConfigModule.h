// -------------------------------------------------------------------------
//    @FileName         £º    NFICommonConfigModule.h
//    @Author           £º    LvSheng.Huang
//    @Date             £º    2012-12-15
//    @Module           £º    NFICosmmonConfigModule
//
// -------------------------------------------------------------------------

#ifndef _NFI_COMMONCONFIG_MODULE_H
#define _NFI_COMMONCONFIG_MODULE_H

#include <iostream>
#include "NFILogicModule.h"
#include "NFIBuffConfigModule.h"


class NFICommonConfigModule
    : public NFILogicModule
{
public:
	virtual bool ClearConfig() = 0;
	virtual const int GetAttributeInt(const std::string& strStructName, const std::string& strStructItemName, const std::string& strAttribute) = 0;
	virtual const int GetAttributeInt(const std::string& strStructName, const std::string& strSDKAttribute) = 0;
	virtual const std::string& GetAttributeString(const std::string& strStructName, const std::string& strStructItemName, const std::string& strAttribute) = 0;
	virtual const std::string& GetAttributeString(const std::string& strStructName,const std::string& strSDKAttribute) = 0;
	virtual bool LoadConfig(const std::string& strFile) = 0;
	virtual const int GetStructItemList(const std::string&strStructName, std::vector<std::string>& xList) = 0;
};

#endif