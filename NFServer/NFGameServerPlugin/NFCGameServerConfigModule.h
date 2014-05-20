// -------------------------------------------------------------------------
//    @FileName      :    NFCGameServerConfigModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-04-14
//    @Module           :    NFCGameServerConfigModule
//
// -------------------------------------------------------------------------

#ifndef _NFC_GAMESERVER_CONFIG_MODULE_H_
#define _NFC_GAMESERVER_CONFIG_MODULE_H_

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
#include "NFComm/NFPluginModule/NFIGameServerConfigModule.h"
#include "NFComm/NFPluginModule/NFIElementInfoModule.h"
#include "NFComm/NFPluginModule/NFILogModule.h"

class NFCGameServerConfigModule
    : public NFIGameServerConfigModule
{
public:
    NFCGameServerConfigModule( NFIPluginManager* p )
    {
        pPluginManager = p;
    }
    virtual ~NFCGameServerConfigModule() {};

    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute( const float fLasFrametime, const float fStartedTime );
    virtual bool AfterInit();

    virtual bool AddSceneToActor( const int nSceneID, const int nGameServer );
    virtual int GetActorID( const int nSceneID );

protected:
    void Load();

private:
    NFMap<int, int> mtSceneConfig;

    NFIKernelModule* m_pKernelModule;
    NFILogModule* m_pLogModule;
    NFIElementInfoModule* m_pElementInfoModule;

};

#endif
