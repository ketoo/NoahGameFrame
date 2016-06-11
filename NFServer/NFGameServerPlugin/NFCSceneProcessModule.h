// -------------------------------------------------------------------------
//    @FileName      :    NFCSceneProcessModule.h
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
#include "NFComm/NFCore/NFMap.h"
#include "NFComm/NFCore/NFList.h"
#include "NFComm/RapidXML/rapidxml.hpp"
#include "NFComm/RapidXML/rapidxml_iterators.hpp"
#include "NFComm/RapidXML/rapidxml_print.hpp"
#include "NFComm/RapidXML/rapidxml_utils.hpp"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIGameLogicModule.h"
#include "NFComm/NFPluginModule/NFIElementModule.h"
#include "NFComm/NFPluginModule/NFILogicClassModule.h"
#include "NFComm/NFPluginModule/NFIGameServerConfigModule.h"
#include "NFComm/NFPluginModule/NFISceneProcessModule.h"
#include "NFComm/NFPluginModule/NFIPropertyModule.h"
#include "NFComm/NFPluginModule/NFILogModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"
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
    virtual bool IsCloneScene(const int nSceneID);
    virtual bool ApplyCloneGroup(const int nSceneID, int& nGroupID);
    virtual bool ExitCloneGroup(const int nSceneID, const int& nGroupID);

protected:
    int CreateCloneScene(const int& nSceneID);

    bool CreateSceneObject(const int nSceneID, const int nGroupID);

    bool LoadSceneResource(const int nSceneID);

protected:

    int OnObjectClassEvent(const NFGUID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFIDataList& var);

    int OnEnterSceneEvent(const NFGUID& object, const int nEventID, const NFIDataList& var);
    int OnLeaveSceneEvent(const NFGUID& object, const int nEventID, const NFIDataList& var);

protected:
    void OnClienSwapSceneProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);

private:

    NFIElementModule* m_pElementModule;
    NFILogicClassModule* m_pLogicClassModule;
    NFIKernelModule* m_pKernelModule;
    NFILogModule* m_pLogModule;
    NFIGameServerNet_ServerModule* m_pGameServerNet_ServerModule;
    //////////////////////////////////////////////////////////////////////////
    struct SceneSeedResource
    {
        std::string strSeedID;
        std::string strConfigID;
        float fSeedX;
        float fSeedY;
        float fSeedZ;
    };

    //SceneID,(SeedID,SeedData)
    NFMapEx<int, NFMapEx<std::string, SceneSeedResource>> mtSceneResourceConfig;

    //////////////////////////////////////////////////////////////////////////
};

#endif
