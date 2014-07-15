// -------------------------------------------------------------------------
//    @FileName      :    NFCSceneProcessModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-04-14
//    @Module           :    NFCSceneProcessModule
//
//      种子NPC加载原理：
//      种子NPC有个专门的类(NPCSeed),种子NPC和NPC有一份共用的属性配置文件
//      NPC生成的时候，会查询种子NPC配置的这些属性，把种子NPC的属性赋予普通NPC
//
//      场景第一次启动时，会创建第一批NPC，此后不在主动创建NPC，完全靠NPC自己的死亡消息自己维护NPC种群的数量和类型
//      最为关键的是，种子NPC一直只是作为属性参考对象，并不会在场景容器中真是的创建与存在，他提供NPC被创建出来需要赋予的一些属性的索引
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
    bool CreateContinerNPCObject( const int nContainerID, const int nGroupID, const std::string& strSeedID );
    int CreateCloneScene( const int& nContainerID, const int nGroupID, const std::string& strResourceID, const NFIValueList& arg );
    bool DestroyCloneScene( const int& nContainerID, const int& nGroupID, const NFIValueList& arg );

protected:
    int CreateContinerNPCObjectByFile( const int nContainerID, const int nGroupID, const std::string& strFileName );

protected:

    int OnContainerObjectEvent( const NFIDENTID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFIValueList& var );
    int OnObjectClassEvent( const NFIDENTID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFIValueList& var );

    int OnEnterSceneEvent( const NFIDENTID& object, const int nEventID, const NFIValueList& var );

    int OnLeaveSceneEvent( const NFIDENTID& object, const int nEventID, const NFIValueList& var );

private:
    //新建立的连接对象，等待他们自己发验证KEY，KEY验证后删掉
    //-1
    //int mnConnectContainer;
    //选人大厅容器
    //-3
    int mnRoleHallContainer;

    int mnContainerLine;
    int mnLineMaxPlayer;

    NFIElementInfoModule* m_pElementInfoModule;
    NFIPropertyModule* m_pPropertyModule;
    NFILogicClassModule* m_pLogicClassModule;
    NFIKernelModule* m_pKernelModule;
    NFIEventProcessModule* m_pEventProcessModule;
    NFILogModule* m_pLogModule;
    //场景ID<资源ID,<NPCID列表>>
    struct SceneGroupResource
    {
        bool bCanClone;
        NFMap<std::string, NFList<std::string>> mtSceneGroupResource;
    };

    NFMap<int, SceneGroupResource> mtSceneResourceConfig;
};

#endif
