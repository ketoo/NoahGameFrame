// -------------------------------------------------------------------------
//    @FileName      :    NFCCommonConfigModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-07-05
//    @Module           :    NFCCommonConfigModule
//
// -------------------------------------------------------------------------

#ifndef NFI_COMMONCONFIG_MODULE_H
#define NFI_COMMONCONFIG_MODULE_H

#include <iostream>
#include "NFIBuffConfigModule.h"


class NFICommonConfigModule
    : public NFIModule
{
public:
	virtual bool LoadConfig(const std::string& strFile) = 0;
    virtual bool ClearConfig() = 0;

	virtual const int GetFieldInt(const std::string& key1, const std::string& field) = 0;
    virtual const int GetFieldInt(const std::string& key1, const std::string& key2, const std::string& field) = 0;

	virtual const std::string& GetFieldString(const std::string& key1, const std::string& field) = 0;
    virtual const std::string& GetFieldString(const std::string& key1, const std::string& key2, const std::string& field) = 0;

	//virtual std::vector<std::string> GetFieldList(const std::string& key1) = 0;
	virtual std::vector<std::string> GetSubKeyList(const std::string& key1) = 0;

	//virtual std::vector<std::string> GetFieldList(const std::string& key1, const std::string& key2) = 0;
};

#endif