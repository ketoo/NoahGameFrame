// -------------------------------------------------------------------------
//    @FileName			:    NFCSceneProcessModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-04-14
//    @Module           :    NFCSceneProcessModule
//
// -------------------------------------------------------------------------

#ifndef NFC_SCENEPROCESS_MODULE_H
#define NFC_SCENEPROCESS_MODULE_H

#include <string>
#include <map>
#include <iostream>
#include "NFComm/NFCore/NFMap.hpp"
#include "NFComm/NFCore/NFList.hpp"
#include "Dependencies/RapidXML/rapidxml.hpp"
#include "Dependencies/RapidXML/rapidxml_iterators.hpp"
#include "Dependencies/RapidXML/rapidxml_print.hpp"
#include "Dependencies/RapidXML/rapidxml_utils.hpp"
#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIGameLogicModule.h"
#include "NFComm/NFPluginModule/NFIElementModule.h"
#include "NFComm/NFPluginModule/NFIClassModule.h"
#include "NFComm/NFPluginModule/NFIGameServerConfigModule.h"
#include "NFComm/NFPluginModule/NFISceneProcessModule.h"
#include "NFComm/NFPluginModule/NFIPropertyModule.h"
#include "NFComm/NFPluginModule/NFILogModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFComm/NFPluginModule/NFIEventModule.h"
#include "NFComm/NFPluginModule/NFISceneAOIModule.h"
#include "NFComm/NFPluginModule/NFIGameServerNet_ServerModule.h"

class NFCSceneProcessModule
    : public NFISceneProcessModule
{
public:
    NFCSceneProcessModule(NFIPluginManager* p)
    {
        pPluginManager = p;
    }
    virtual ~NFCSceneProcessModule() {};

    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute();
    virtual bool AfterInit();

    virtual E_SCENE_TYPE GetCloneSceneType(const int nSceneID);

protected:
	bool LoadSceneResource(const std::string& strSceneIDName);

protected:

    int OnObjectClassEvent(const NFGUID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFIDataList& var);
	
	int BeforeEnterSceneEvent(const NFGUID& self, const int nSceneID, const int nGroupID, const int nType, const NFIDataList& argList);
	int AfterEnterSceneEvent(const NFGUID& self, const int nSceneID, const int nGroupID, const int nType, const NFIDataList& argList);

	int BeforeLeaveSceneEvent(const NFGUID& self, const int nSceneID, const int nGroupID, const int nType, const NFIDataList& argList);
	int AfterLeaveSceneEvent(const NFGUID& self, const int nSceneID, const int nGroupID, const int nType, const NFIDataList& argList);

private:

    NFIElementModule* m_pElementModule;
    NFIClassModule* m_pClassModule;
    NFIKernelModule* m_pKernelModule;
    NFILogModule* m_pLogModule;
	NFIEventModule* m_pEventModule;
	NFISceneAOIModule* m_pSceneAOIModule;
    NFIGameServerNet_ServerModule* m_pGameServerNet_ServerModule;
};

#endif
