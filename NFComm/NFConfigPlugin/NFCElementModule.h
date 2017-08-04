// -------------------------------------------------------------------------
//    @FileName			:    NFCElementModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFCElementModule
//
// -------------------------------------------------------------------------

#ifndef NFC_ELEMENT_MODULE_H
#define NFC_ELEMENT_MODULE_H

#include <map>
#include <string>
#include <iostream>
#include "Dependencies/RapidXML/rapidxml.hpp"
#include "Dependencies/RapidXML/rapidxml_iterators.hpp"
#include "Dependencies/RapidXML/rapidxml_print.hpp"
#include "Dependencies/RapidXML/rapidxml_utils.hpp"
#include "NFComm/NFCore/NFMap.hpp"
#include "NFComm/NFCore/NFList.hpp"
#include "NFComm/NFCore/NFDataList.hpp"
#include "NFComm/NFCore/NFCRecord.h"
#include "NFComm/NFCore/NFCPropertyManager.h"
#include "NFComm/NFCore/NFCRecordManager.h"
#include "NFComm/NFPluginModule/NFIElementModule.h"
#include "NFComm/NFPluginModule/NFIClassModule.h"

class NFCClass;

class ElementConfigInfo
{
public:
    ElementConfigInfo()
    {
        m_pPropertyManager = NF_SHARE_PTR<NFIPropertyManager>(NF_NEW NFCPropertyManager(NFGUID()));
        m_pRecordManager = NF_SHARE_PTR<NFIRecordManager>(NF_NEW NFCRecordManager(NFGUID()));
    }

    virtual ~ElementConfigInfo()
    {
    }

    NF_SHARE_PTR<NFIPropertyManager> GetPropertyManager()
    {
        return m_pPropertyManager;
    }

    NF_SHARE_PTR<NFIRecordManager> GetRecordManager()
    {
        return m_pRecordManager;
    }

protected:

    //std::string mstrConfigID;

    NF_SHARE_PTR<NFIPropertyManager> m_pPropertyManager;
    NF_SHARE_PTR<NFIRecordManager> m_pRecordManager;
};

class NFCElementModule
    : public NFIElementModule,
      NFMapEx<std::string, ElementConfigInfo>
{
public:
    NFCElementModule(NFIPluginManager* p);
    virtual ~NFCElementModule();

    virtual bool Init();
    virtual bool Shut();

    virtual bool AfterInit();
    virtual bool BeforeShut();
    virtual bool Execute();

    virtual bool Load();
    virtual bool Save();
    virtual bool Clear();

    virtual bool LoadSceneInfo(const std::string& strFileName, const std::string& strClassName);

    virtual bool ExistElement(const std::string& strConfigName);
    virtual bool ExistElement(const std::string& strClassName, const std::string& strConfigName);

    virtual NF_SHARE_PTR<NFIPropertyManager> GetPropertyManager(const std::string& strConfigName);
    virtual NF_SHARE_PTR<NFIRecordManager> GetRecordManager(const std::string& strConfigName);

    virtual NFINT64 GetPropertyInt(const std::string& strConfigName, const std::string& strPropertyName);
	virtual int GetPropertyInt32(const std::string& strConfigName, const std::string& strPropertyName);
    virtual double GetPropertyFloat(const std::string& strConfigName, const std::string& strPropertyName);
    virtual const std::string& GetPropertyString(const std::string& strConfigName, const std::string& strPropertyName);

	virtual const std::vector<std::string> GetListByProperty(const std::string& strClassName, const std::string& strPropertyName, const NFINT64 nValue);
	virtual const std::vector<std::string> GetListByProperty(const std::string& strClassName, const std::string& strPropertyName, const std::string& nValue);

protected:
    virtual NF_SHARE_PTR<NFIProperty> GetProperty(const std::string& strConfigName, const std::string& strPropertyName);

    virtual bool Load(rapidxml::xml_node<>* attrNode, NF_SHARE_PTR<NFIClass> pLogicClass);
    virtual bool CheckRef();
    virtual bool LegalNumber(const char* str);

protected:
    NFIClassModule* m_pClassModule;
    bool mbLoaded;
};

#endif
