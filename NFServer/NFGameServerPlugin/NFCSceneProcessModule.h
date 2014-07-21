// -------------------------------------------------------------------------
//    @FileName      :    NFCSceneProcessModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-04-14
//    @Module           :    NFCSceneProcessModule
//
// -------------------------------------------------------------------------

#ifndef _NFC_SCENEPROCESS_MODULE_H_
#define _NFC_SCENEPROCESS_MODULE_H_

#include <string>
#include <map>
#include <iostream>
#include "NFComm/NFCore/NFMap.h"
#include "NFComm/NFCore/NFList.h"
#include "NFComm/RapidXML/rapidxml.hpp"
#include "NFComm/RapidXML/rapidxml_iterators.hpp"
#include "NFComm/RapidXML/rapidxml_print.hpp"
#include "NFComm/RapidXML/rapidxml_utils.hpp"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIGameLogicModule.h"
#include "NFComm/NFPluginModule/NFIEventProcessModule.h"
#include "NFComm/NFPluginModule/NFIElementInfoModule.h"
#include "NFComm/NFPluginModule/NFILogicClassModule.h"
#include "NFComm/NFPluginModule/NFIGameServerConfigModule.h"
#include "NFComm/NFPluginModule/NFISceneProcessModule.h"
#include "NFComm/NFPluginModule/NFIPropertyModule.h"
#include "NFComm/NFPluginModule/NFILogModule.h"

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
    virtual bool Execute( const float fLasFrametime, const float fStartedTime );
    virtual bool AfterInit();

    virtual E_SCENE_TYPE GetCloneSceneType(const int nContainerID);

protected:
    int CreateCloneScene( const int& nContainerID, const int nGroupID, const std::string& strResourceID, const NFIValueList& arg );
    bool DestroyCloneScene( const int& nContainerID, const int& nGroupID, const NFIValueList& arg );


protected:
    bool LoadInitFileResource( const int& nContainerID );

    bool CreateContinerObjectByFile( const int nContainerID, const int nGroupID, const std::string& strFileName );
    bool CreateContinerObject( const int nContainerID, const int nGroupID, const std::string& strFileName, const std::string& strSeedID );

protected:

    int OnObjectClassEvent( const NFIDENTID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFIValueList& var );

    int OnEnterSceneEvent( const NFIDENTID& object, const int nEventID, const NFIValueList& var );
    int OnLeaveSceneEvent( const NFIDENTID& object, const int nEventID, const NFIValueList& var );

private:
    int mnContainerLine;
    int mnLineMaxPlayer;

    NFIElementInfoModule* m_pElementInfoModule;
    NFIPropertyModule* m_pPropertyModule;
    NFILogicClassModule* m_pLogicClassModule;
    NFIKernelModule* m_pKernelModule;
    NFIEventProcessModule* m_pEventProcessModule;
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
        NFMap<std::string, NFMap<std::string, SceneSeedResource>> xSceneGroupResource;
    };

    //场景ID,对应资源
    //Map<int, SceneGroupResource> mtSceneResourceConfig;
    //场景ID,(File.xml,分组资源)
    NFMap<int, NFMap<std::string, SceneGroupResource>> mtSceneResourceConfig;

    //////////////////////////////////////////////////////////////////////////
};

#endif
