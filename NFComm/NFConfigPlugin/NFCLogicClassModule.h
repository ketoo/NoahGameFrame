// -------------------------------------------------------------------------
//    @FileName      :    NFCLogicClassModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFCLogicClassModule
//
// -------------------------------------------------------------------------

#ifndef _NFC_LOGICCLASS_MODULE_H_
#define _NFC_LOGICCLASS_MODULE_H_

#include <string>
#include <map>
#include <iostream>
#include "NFCElementInfoModule.h"
#include "NFComm/NFCore/NFMap.h"
#include "NFComm/NFCore/NFList.h"
#include "NFComm/RapidXML/rapidxml.hpp"
#include "NFComm/RapidXML/rapidxml_iterators.hpp"
#include "NFComm/RapidXML/rapidxml_print.hpp"
#include "NFComm/RapidXML/rapidxml_utils.hpp"
#include "NFComm/NFCore/NFCDataList.h"
#include "NFComm/NFCore/NFCRecord.h"
#include "NFComm/NFCore/NFList.h"
#include "NFComm/NFCore/NFCPropertyManager.h"
#include "NFComm/NFCore/NFCRecordManager.h"
#include "NFComm/NFPluginModule/NFILogicClassModule.h"
#include "NFComm/NFPluginModule/NFIElementInfoModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"

class NFCLogicClass : public NFILogicClass
{
public:

    NFCLogicClass(const std::string& strClassName)
    {
        m_pParentClass = NULL;
        mstrClassName = strClassName;

        m_pPropertyManager = NF_SHARE_PTR<NFIPropertyManager>(NF_NEW NFCPropertyManager(NFIDENTID()));
        m_pRecordManager = NF_SHARE_PTR<NFIRecordManager>(NF_NEW NFCRecordManager(NFIDENTID()));
        m_pComponentManager = NF_SHARE_PTR<NFIComponentManager>(NF_NEW NFCComponentManager(NFIDENTID()));
    }

    virtual ~NFCLogicClass()
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

    void SetParent(NF_SHARE_PTR<NFILogicClass> pClass)
    {
        m_pParentClass = pClass;
    }

    NF_SHARE_PTR<NFILogicClass> GetParent()
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

    const bool AddConfigName(std::string& strConfigName)
    {
        mlConfigList.Add(strConfigName);

        return true;
    }

    NFList<std::string>& GetConfigNameList()
    {
        return mlConfigList;
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

    NF_SHARE_PTR<NFILogicClass> m_pParentClass;
    std::string mstrType;
    std::string mstrClassName;
    std::string mstrClassInstancePath;

    NFList<std::string> mlConfigList;
};

class NFCLogicClassModule
    : public NFILogicClassModule
{
public:
    NFCLogicClassModule(NFIPluginManager* p);
    virtual ~NFCLogicClassModule();

    virtual bool Init();
    virtual bool Shut();

    virtual bool Load();
    virtual bool Save();
    virtual bool Clear();

    virtual NF_SHARE_PTR<NFIPropertyManager> GetClassPropertyManager(const std::string& strClassName);
    virtual NF_SHARE_PTR<NFIRecordManager> GetClassRecordManager(const std::string& strClassName);
    virtual NF_SHARE_PTR<NFIComponentManager> GetClassComponentManager(const std::string& strClassName);


    virtual bool AddClass(const std::string& strClassName, const std::string& strParentName);
protected:

    virtual TDATA_TYPE ComputerType(const char* pstrTypeName, NFIDataList::TData& var);
    virtual bool AddPropertys(rapidxml::xml_node<>* pPropertyRootNode, NF_SHARE_PTR<NFILogicClass> pClass);
    virtual bool AddRecords(rapidxml::xml_node<>* pRecordRootNode, NF_SHARE_PTR<NFILogicClass> pClass);
    virtual bool AddComponents(rapidxml::xml_node<>* pRecordRootNode, NF_SHARE_PTR<NFILogicClass> pClass);
    virtual bool AddClassInclude(const char* pstrClassFilePath, NF_SHARE_PTR<NFILogicClass> pClass);
    virtual bool AddClass(const char* pstrClassFilePath, NF_SHARE_PTR<NFILogicClass> pClass);

    //加载此节点的类
    virtual bool Load(rapidxml::xml_node<>* attrNode, NF_SHARE_PTR<NFILogicClass> pParentClass);

protected:
    NFIElementInfoModule* m_pElementInfoModule;

    std::string msConfigFileName;
    int mnPropertyIndex;

    std::map<std::string, int> mxPropertyIndexMap;
};

#endif