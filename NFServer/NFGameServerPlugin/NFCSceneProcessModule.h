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
#include "NFComm/NFPluginModule/NFIElementInfoModule.h"
#include "NFComm/NFPluginModule/NFILogicClassModule.h"
#include "NFComm/NFPluginModule/NFIGameServerConfigModule.h"
#include "NFComm/NFPluginModule/NFISceneProcessModule.h"
#include "NFComm/NFPluginModule/NFIPropertyModule.h"
#include "NFComm/NFPluginModule/NFILogModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"

class NFCSceneProcessModule
    : public NFISceneProcessModule
{
public:
    NFCSceneProcessModule( NFIPluginManager* p )
    {
        pPluginManager = p;
    }
    virtual ~NFCSceneProcessModule() {};

    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute();
    virtual bool AfterInit();

    virtual E_SCENE_TYPE GetCloneSceneType(const int nContainerID);
    virtual bool IsCloneScene(const int nSceneID);

protected:
    int CreateCloneScene( const int& nContainerID, const int nGroupID, const std::string& strResourceID, const NFIDataList& arg );
    bool DestroyCloneScene( const int& nContainerID, const int& nGroupID, const NFIDataList& arg );


protected:
    bool LoadInitFileResource( const int& nContainerID );

    bool CreateContinerObjectByFile( const int nContainerID, const int nGroupID, const std::string& strFileName );
    bool CreateContinerObject( const int nContainerID, const int nGroupID, const std::string& strFileName, const std::string& strSeedID );

protected:

    int OnObjectClassEvent( const NFGUID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFIDataList& var );

    int OnEnterSceneEvent( const NFGUID& object, const int nEventID, const NFIDataList& var );
    int OnLeaveSceneEvent( const NFGUID& object, const int nEventID, const NFIDataList& var );

private:
    int mnContainerLine;
    int mnLineMaxPlayer;

    NFIElementInfoModule* m_pElementInfoModule;
    NFIPropertyModule* m_pPropertyModule;
    NFILogicClassModule* m_pLogicClassModule;
    NFIKernelModule* m_pKernelModule;
    NFILogModule* m_pLogModule;
    
    //////////////////////////////////////////////////////////////////////////
    struct SceneSeedResource
    {
        std::string strSeedID;
        std::string strConfigID;
        float fSeedX;
        float fSeedY;
        float fSeedZ;
    };

    struct SceneGroupResource
    {
        bool bCanClone;
        //资源文件ID,<NPC种子列表>
        //NPC.xml
        NFMapEx<std::string, NFMapEx<std::string, SceneSeedResource>> xSceneGroupResource;
    };

    //场景ID,对应资源
    //Map<int, SceneGroupResource> mtSceneResourceConfig;
    //场景ID,(File.xml,分组资源)
    NFMapEx<int, NFMapEx<std::string, SceneGroupResource>> mtSceneResourceConfig;

    //////////////////////////////////////////////////////////////////////////
};

#endif
