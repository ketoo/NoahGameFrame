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

	virtual bool AddClassCallBack(const CLASS_EVENT_FUNCTOR_PTR& cb) = 0;
	virtual bool DoEvent(const NFGUID& objectID, const CLASS_OBJECT_EVENT eClassEvent, const NFIDataList& valueList) = 0;
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

	template<typename BaseType>
	bool AddClassCallBack(const std::string& strClassName, BaseType* pBase, int (BaseType::*handler)(const NFGUID&, const std::string&, const CLASS_OBJECT_EVENT, const NFIDataList&))
	{
		CLASS_EVENT_FUNCTOR functor = std::bind(handler, pBase, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
		CLASS_EVENT_FUNCTOR_PTR functorPtr(new CLASS_EVENT_FUNCTOR(functor));
		return AddClassCallBack(strClassName, functorPtr);
	}

	virtual bool DoEvent(const NFGUID& objectID, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFIDataList& valueList) = 0;

	virtual bool AddClassCallBack(const std::string& strClassName, const CLASS_EVENT_FUNCTOR_PTR& cb) = 0;

    virtual NF_SHARE_PTR<NFIPropertyManager> GetClassPropertyManager(const std::string& strClassName) = 0;

    virtual NF_SHARE_PTR<NFIRecordManager> GetClassRecordManager(const std::string& strClassName) = 0;

    virtual NF_SHARE_PTR<NFIComponentManager> GetClassComponentManager(const std::string& strClassName) = 0;
};

#endif