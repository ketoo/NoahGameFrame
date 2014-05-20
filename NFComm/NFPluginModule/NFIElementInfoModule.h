// -------------------------------------------------------------------------
//    @FileName         :    NFIElementInfoModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFIElementInfoModule
//
// -------------------------------------------------------------------------

#ifndef _NFI_ELEMENTINFO_MODULE_H_
#define _NFI_ELEMENTINFO_MODULE_H_

#include <iostream>
#include "NFILogicModule.h"
#include "NFComm/NFCore/NFCValueList.h"
#include "NFComm/NFCore/NFCPropertyManager.h"
#include "NFComm/NFCore/NFCRecordManager.h"

class NFIElementInfoModule
    : public NFILogicModule
{
public:
    virtual bool Load() = 0;
	virtual bool Save() = 0;
    virtual bool Clear() = 0;

    //special
    virtual bool LoadSceneInfo(const std::string& strFileName, const std::string& strClassName) = 0;

    virtual bool ExistElement(const std::string& strConfigName) = 0;

    virtual NFIPropertyManager* GetPropertyManager(const std::string& strConfigName) = 0;
    virtual NFIRecordManager* GetRecordManager(const std::string& strConfigName) = 0;

    virtual int QueryPropertyInt(const std::string& strConfigName, const std::string& strPropertyName) = 0;
    virtual float QueryPropertyFloat(const std::string& strConfigName, const std::string& strPropertyName) = 0;
    virtual double QueryPropertyDouble(const std::string& strConfigName, const std::string& strPropertyName) = 0;
    virtual const std::string& QueryPropertyString(const std::string& strConfigName, const std::string& strPropertyName) = 0;

protected:
private:
};
#endif