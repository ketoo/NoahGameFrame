// -------------------------------------------------------------------------
//    @FileName         :    NFIElementModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFIElementModule
//
// -------------------------------------------------------------------------

#ifndef NFI_ELEMENTINFO_MODULE_H
#define NFI_ELEMENTINFO_MODULE_H

#include <iostream>
#include "NFIModule.h"
#include "NFComm/NFCore/NFDataList.hpp"
#include "NFComm/NFCore/NFIPropertyManager.h"
#include "NFComm/NFCore/NFIRecordManager.h"

class NFIElementModule
    : public NFIModule
{
public:
    virtual bool Load() = 0;
    virtual bool Save() = 0;
    virtual bool Clear() = 0;

    //special
    virtual bool LoadSceneInfo(const std::string& strFileName, const std::string& strClassName) = 0;

    virtual bool ExistElement(const std::string& strConfigName) = 0;
    virtual bool ExistElement(const std::string& strClassName, const std::string& strConfigName) = 0;

    virtual std::shared_ptr<NFIPropertyManager> GetPropertyManager(const std::string& strConfigName) = 0;
    virtual std::shared_ptr<NFIRecordManager> GetRecordManager(const std::string& strConfigName) = 0;

    virtual NFINT64 GetPropertyInt(const std::string& strConfigName, const std::string& strPropertyName) = 0;
	virtual int GetPropertyInt32(const std::string& strConfigName, const std::string& strPropertyName) = 0;
    virtual double GetPropertyFloat(const std::string& strConfigName, const std::string& strPropertyName) = 0;
    virtual const std::string& GetPropertyString(const std::string& strConfigName, const std::string& strPropertyName) = 0;
	
	virtual const std::vector<std::string> GetListByProperty(const std::string& strClassName, const std::string& strPropertyName, const NFINT64 nValue) = 0;
	virtual const std::vector<std::string> GetListByProperty(const std::string& strClassName, const std::string& strPropertyName, const std::string& nValue) = 0;

};
#endif