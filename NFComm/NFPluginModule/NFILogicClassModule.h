// -------------------------------------------------------------------------
//    @FileName         :    NFILogicClassModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFILogicClassModule
//
// -------------------------------------------------------------------------

#ifndef _NFI_LOGICCLASS_MODULE_H
#define _NFI_LOGICCLASS_MODULE_H

#include <iostream>
#include "NFILogicModule.h"
#include "NFComm/NFCore/NFIPropertyManager.h"
#include "NFComm/NFCore/NFIRecordManager.h"
#include "NFComm/NFCore/NFIComponentManager.h"

class NFILogicClass
    : public NFList<std::string>//include files
{
public:
    virtual ~NFILogicClass(){}

    virtual NF_SHARE_PTR<NFIPropertyManager> GetPropertyManager() = 0;

    virtual NF_SHARE_PTR<NFIRecordManager> GetRecordManager() = 0;

    virtual NF_SHARE_PTR<NFIComponentManager> GetComponentManager() = 0;

    virtual void SetParent(NF_SHARE_PTR<NFILogicClass> pClass) = 0;
    virtual NF_SHARE_PTR<NFILogicClass> GetParent() = 0;
    virtual void SetTypeName(const char* strType) = 0;
    virtual const std::string& GetTypeName() = 0;
    virtual const std::string& GetClassName() = 0;
    virtual const bool AddConfigName(std::string& strConfigName) = 0;
    virtual NFList<std::string>& GetConfigNameList() = 0;
    virtual void SetInstancePath(const std::string& strPath) = 0;
    virtual const std::string& GetInstancePath() = 0;
};

class NFILogicClassModule
    : public NFILogicModule,
      public NFMapEx<std::string, NFILogicClass>
{
public:
    virtual ~NFILogicClassModule() {}
	virtual bool Load() = 0;
	virtual bool Save() = 0;
	virtual bool Clear() = 0;

    virtual NF_SHARE_PTR<NFIPropertyManager> GetClassPropertyManager(const std::string& strClassName) = 0;

    virtual NF_SHARE_PTR<NFIRecordManager> GetClassRecordManager(const std::string& strClassName) = 0;

    virtual NF_SHARE_PTR<NFIComponentManager> GetClassComponentManager(const std::string& strClassName) = 0;

};

#endif