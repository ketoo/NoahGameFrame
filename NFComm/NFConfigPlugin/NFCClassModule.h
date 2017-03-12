// -------------------------------------------------------------------------
//    @FileName			:    NFCClassModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFCClassModule
//
// -------------------------------------------------------------------------

#ifndef NFC_LOGICCLASS_MODULE_H
#define NFC_LOGICCLASS_MODULE_H

#include <string>
#include <map>
#include <iostream>
#include "NFCElementModule.h"
#include "Dependencies/RapidXML/rapidxml.hpp"
#include "NFComm/NFCore/NFMap.hpp"
#include "NFComm/NFCore/NFList.hpp"
#include "NFComm/NFCore/NFDataList.hpp"
#include "NFComm/NFCore/NFCRecord.h"
#include "NFComm/NFCore/NFList.hpp"
#include "NFComm/NFCore/NFCPropertyManager.h"
#include "NFComm/NFCore/NFCRecordManager.h"
#include "NFComm/NFPluginModule/NFIClassModule.h"
#include "NFComm/NFPluginModule/NFIElementModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"

class NFCClass : public NFIClass
{
public:

    NFCClass(const std::string& strClassName)
    {
        m_pParentClass = NULL;
        mstrClassName = strClassName;

        m_pPropertyManager = NF_SHARE_PTR<NFIPropertyManager>(NF_NEW NFCPropertyManager(NFGUID()));
        m_pRecordManager = NF_SHARE_PTR<NFIRecordManager>(NF_NEW NFCRecordManager(NFGUID()));
        m_pComponentManager = NF_SHARE_PTR<NFIComponentManager>(NF_NEW NFCComponentManager(NFGUID()));
    }

    virtual ~NFCClass()
    {
        ClearAll();
    }

    virtual NF_SHARE_PTR<NFIPropertyManager> GetPropertyManager()
    {
        return m_pPropertyManager;
    }

    virtual NF_SHARE_PTR<NFIRecordManager> GetRecordManager()
    {
        return m_pRecordManager;
    }

    virtual NF_SHARE_PTR<NFIComponentManager> GetComponentManager()
    {
        return m_pComponentManager;
    }

    virtual bool AddClassCallBack(const CLASS_EVENT_FUNCTOR_PTR& cb)
    {
        return mxClassEventInfo.Add(cb);
    }

    virtual bool DoEvent(const NFGUID& objectID, const CLASS_OBJECT_EVENT eClassEvent, const NFDataList& valueList)
    {
        CLASS_EVENT_FUNCTOR_PTR cb;
        bool bRet = mxClassEventInfo.First(cb);
        while (bRet)
        {
            cb->operator()(objectID, mstrClassName, eClassEvent,  valueList);

            bRet = mxClassEventInfo.Next(cb);
        }

        return true;
    }

    void SetParent(NF_SHARE_PTR<NFIClass> pClass)
    {
        m_pParentClass = pClass;
    }

    NF_SHARE_PTR<NFIClass> GetParent()
    {
        return m_pParentClass;
    }

    void SetTypeName(const char* strType)
    {
        mstrType = strType;
    }

    const std::string& GetTypeName()
    {
        return mstrType;
    }

    const std::string& GetClassName()
    {
        return mstrClassName;
    }

    const bool AddId(std::string& strId)
    {
		mIdList.push_back(strId);
        return true;
    }

	const std::vector<std::string>& GetIDList()
    {
        return mIdList;
    }

    void SetInstancePath(const std::string& strPath)
    {
        mstrClassInstancePath = strPath;
    }

    const std::string& GetInstancePath()
    {
        return mstrClassInstancePath;
    }

private:
    NF_SHARE_PTR<NFIPropertyManager> m_pPropertyManager;
    NF_SHARE_PTR<NFIRecordManager> m_pRecordManager;
    NF_SHARE_PTR<NFIComponentManager> m_pComponentManager;

    NF_SHARE_PTR<NFIClass> m_pParentClass;
    std::string mstrType;
    std::string mstrClassName;
    std::string mstrClassInstancePath;

    std::vector<std::string> mIdList;

    NFList<CLASS_EVENT_FUNCTOR_PTR> mxClassEventInfo;
};

class NFCClassModule
    : public NFIClassModule
{
public:
    NFCClassModule(NFIPluginManager* p);
    virtual ~NFCClassModule();

    virtual bool Init();
    virtual bool Shut();

    virtual bool Load();
    virtual bool Save();
    virtual bool Clear();

    virtual bool AddClassCallBack(const std::string& strClassName, const CLASS_EVENT_FUNCTOR_PTR& cb);
    virtual bool DoEvent(const NFGUID& objectID, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFDataList& valueList);

    virtual NF_SHARE_PTR<NFIPropertyManager> GetClassPropertyManager(const std::string& strClassName);
    virtual NF_SHARE_PTR<NFIRecordManager> GetClassRecordManager(const std::string& strClassName);
    virtual NF_SHARE_PTR<NFIComponentManager> GetClassComponentManager(const std::string& strClassName);

    virtual bool AddClass(const std::string& strClassName, const std::string& strParentName);

protected:
    virtual NFDATA_TYPE ComputerType(const char* pstrTypeName, NFData& var);
    virtual bool AddPropertys(rapidxml::xml_node<>* pPropertyRootNode, NF_SHARE_PTR<NFIClass> pClass);
    virtual bool AddRecords(rapidxml::xml_node<>* pRecordRootNode, NF_SHARE_PTR<NFIClass> pClass);
    virtual bool AddComponents(rapidxml::xml_node<>* pRecordRootNode, NF_SHARE_PTR<NFIClass> pClass);
    virtual bool AddClassInclude(const char* pstrClassFilePath, NF_SHARE_PTR<NFIClass> pClass);
    virtual bool AddClass(const char* pstrClassFilePath, NF_SHARE_PTR<NFIClass> pClass);

    
    virtual bool Load(rapidxml::xml_node<>* attrNode, NF_SHARE_PTR<NFIClass> pParentClass);

protected:
    NFIElementModule* m_pElementModule;

    std::string msConfigFileName;
};

#endif
