// -------------------------------------------------------------------------
//    @FileName      :    NFCElementInfoModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFCElementInfoModule
//
// -------------------------------------------------------------------------

#ifndef _NFC_ELEMENTINFO_MODULE_H_
#define _NFC_ELEMENTINFO_MODULE_H_

#include <string>
#include <map>
///
#include <iostream>
#include <iostream>
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
#include "NFComm/NFCore/NFCComponentManager.h"
#include "NFComm/NFPluginModule/NFIElementInfoModule.h"
#include "NFComm/NFPluginModule/NFILogicClassModule.h"

class NFCLogicClass;

class ElementConfigInfo
{
public:
    ElementConfigInfo()
    {
        m_pPropertyManager = std::shared_ptr<NFIPropertyManager>(NF_NEW NFCPropertyManager(NFIDENTID(0)));
        m_pRecordManager = std::shared_ptr<NFIRecordManager>(NF_NEW NFCRecordManager(NFIDENTID(0)));
        m_pComponentManager = std::shared_ptr<NFIComponentManager>(NF_NEW NFCComponentManager(NFIDENTID(0)));
    }

    virtual ~ElementConfigInfo()
    {
    }

    std::shared_ptr<NFIPropertyManager> GetPropertyManager()
    {
        return m_pPropertyManager;
    }

    std::shared_ptr<NFIRecordManager> GetRecordManager()
    {
        return m_pRecordManager;
    }

    std::shared_ptr<NFIComponentManager> GetComponentManager()
    {
        return m_pComponentManager;
    }
protected:

    //std::string mstrConfigID;

    std::shared_ptr<NFIPropertyManager> m_pPropertyManager;
    std::shared_ptr<NFIRecordManager> m_pRecordManager;
    std::shared_ptr<NFIComponentManager> m_pComponentManager;
};

class NFCElementInfoModule
    : public NFIElementInfoModule,
      NFMapEx<std::string, ElementConfigInfo>
{
public:
    NFCElementInfoModule(NFIPluginManager* p);
    virtual ~NFCElementInfoModule();

    virtual bool Init();
    virtual bool Shut();

    virtual bool AfterInit();
    virtual bool BeforeShut();
    virtual bool Execute(const float fLasFrametime, const float fStartedTime);

    virtual bool Load();
    virtual bool Save();
    virtual bool Clear();

    virtual bool LoadSceneInfo(const std::string& strFileName, const std::string& strClassName);

    virtual bool ExistElement(const std::string& strConfigName);

    virtual std::shared_ptr<NFIPropertyManager> GetPropertyManager(const std::string& strConfigName);
    virtual std::shared_ptr<NFIRecordManager> GetRecordManager(const std::string& strConfigName);
    virtual std::shared_ptr<NFIComponentManager> GetComponentManager(const std::string& strConfigName);

    virtual NFINT64 GetPropertyInt(const std::string& strConfigName, const std::string& strPropertyName);
    virtual float GetPropertyFloat(const std::string& strConfigName, const std::string& strPropertyName);
    virtual double GetPropertyDouble(const std::string& strConfigName, const std::string& strPropertyName);
    virtual const std::string& GetPropertyString(const std::string& strConfigName, const std::string& strPropertyName);

protected:
    virtual std::shared_ptr<NFIProperty> GetProperty(const std::string& strConfigName, const std::string& strPropertyName);

    virtual bool Load(rapidxml::xml_node<>* attrNode, std::shared_ptr<NFILogicClass> pLogicClass);

    virtual bool LegalNumber(const char* str);
protected:
    NFILogicClassModule* m_pLogicClassModule;
    bool mbLoaded;
};

#endif
