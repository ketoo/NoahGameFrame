// -------------------------------------------------------------------------
//    @FileName      :    NFCLogicClassModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFCLogicClassModule
//
// -------------------------------------------------------------------------

#ifndef NFC_LOGICCLASS_MODULE_H
#define NFC_LOGICCLASS_MODULE_H

#include <string>
#include <map>
#include <iostream>
#include "NFCElementInfoModule.h"
#include "NFComm/NFCore/NFMap.h"
#include "NFComm/NFCore/NFList.h"
#include "NFComm/RapidXML/rapidxml.hpp"
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

        m_pPropertyManager = NF_SHARE_PTR<NFIPropertyManager>(NF_NEW NFCPropertyManager(NFGUID()));
        m_pRecordManager = NF_SHARE_PTR<NFIRecordManager>(NF_NEW NFCRecordManager(NFGUID()));
        m_pComponentManager = NF_SHARE_PTR<NFIComponentManager>(NF_NEW NFCComponentManager(NFGUID()));
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

	virtual bool AddClassCallBack(const CLASS_EVENT_FUNCTOR_PTR& cb)
	{
		return mxClassEventInfo.Add(cb);
	}

	virtual bool DoEvent(const NFGUID& objectID, const CLASS_OBJECT_EVENT eClassEvent, const NFIDataList& valueList)
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

	NFList<CLASS_EVENT_FUNCTOR_PTR> mxClassEventInfo;
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

	virtual bool AddClassCallBack(const std::string& strClassName, const CLASS_EVENT_FUNCTOR_PTR& cb);
	virtual bool DoEvent(const NFGUID& objectID, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFIDataList& valueList);

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

    //���ش˽ڵ����
    virtual bool Load(rapidxml::xml_node<>* attrNode, NF_SHARE_PTR<NFILogicClass> pParentClass);

public:
    static bool ReadFileToString(const std::string& strFile, std::string& strOutData);
    static bool IsEncoded(unsigned char c);
    static std::string Encode(unsigned char const* bytes_to_encode, unsigned int in_len);
    static std::string Decode(const std::string& encoded_string);
    static std::string cepher;
    static bool bCipher;
protected:
    NFIElementInfoModule* m_pElementInfoModule;

    std::string msConfigFileName;

    int mnPropertyIndex;
    std::map<std::string, int> mxPropertyIndexMap;
};

#endif