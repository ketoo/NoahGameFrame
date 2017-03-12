// -------------------------------------------------------------------------
//    @FileName         :    NFIClassModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFIClassModule
//
// -------------------------------------------------------------------------

#ifndef NFI_LOGICCLASS_MODULE_H
#define NFI_LOGICCLASS_MODULE_H

#include <iostream>
#include "NFIModule.h"
#include "NFComm/NFCore/NFIObject.h"

typedef std::function<int(const NFGUID&, const std::string&, const CLASS_OBJECT_EVENT, const NFDataList&)> CLASS_EVENT_FUNCTOR;
typedef NF_SHARE_PTR<CLASS_EVENT_FUNCTOR> CLASS_EVENT_FUNCTOR_PTR;//EVENT

class NFIClass
    : public NFList<std::string>//include files
{
public:
    virtual ~NFIClass() {}

    virtual NF_SHARE_PTR<NFIPropertyManager> GetPropertyManager() = 0;

    virtual NF_SHARE_PTR<NFIRecordManager> GetRecordManager() = 0;

    virtual NF_SHARE_PTR<NFIComponentManager> GetComponentManager() = 0;

    virtual void SetParent(NF_SHARE_PTR<NFIClass> pClass) = 0;
    virtual NF_SHARE_PTR<NFIClass> GetParent() = 0;
    virtual void SetTypeName(const char* strType) = 0;
    virtual const std::string& GetTypeName() = 0;
    virtual const std::string& GetClassName() = 0;
    virtual const bool AddId(std::string& strConfigName) = 0;
    virtual const std::vector<std::string>& GetIDList() = 0;
    virtual const std::string& GetInstancePath() = 0;
	virtual void SetInstancePath(const std::string& strPath) = 0;

    virtual bool AddClassCallBack(const CLASS_EVENT_FUNCTOR_PTR& cb) = 0;
    virtual bool DoEvent(const NFGUID& objectID, const CLASS_OBJECT_EVENT eClassEvent, const NFDataList& valueList) = 0;
};

class NFIClassModule
    : public NFIModule,
  public NFMapEx<std::string, NFIClass>
{
public:
    virtual ~NFIClassModule() {}
    virtual bool Load() = 0;
    virtual bool Save() = 0;
    virtual bool Clear() = 0;

    template<typename BaseType>
    bool AddClassCallBack(const std::string& strClassName, BaseType* pBase, int (BaseType::*handler)(const NFGUID&, const std::string&, const CLASS_OBJECT_EVENT, const NFDataList&))
    {
        CLASS_EVENT_FUNCTOR functor = std::bind(handler, pBase, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
        CLASS_EVENT_FUNCTOR_PTR functorPtr(new CLASS_EVENT_FUNCTOR(functor));
        return AddClassCallBack(strClassName, functorPtr);
    }

    virtual bool DoEvent(const NFGUID& objectID, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFDataList& valueList) = 0;

    virtual bool AddClassCallBack(const std::string& strClassName, const CLASS_EVENT_FUNCTOR_PTR& cb) = 0;

    virtual NF_SHARE_PTR<NFIPropertyManager> GetClassPropertyManager(const std::string& strClassName) = 0;

    virtual NF_SHARE_PTR<NFIRecordManager> GetClassRecordManager(const std::string& strClassName) = 0;

    virtual NF_SHARE_PTR<NFIComponentManager> GetClassComponentManager(const std::string& strClassName) = 0;
};

#endif