// -------------------------------------------------------------------------
//    Created by LUSHENG HUANG on 30/08/17.
//
/*
solution 1: the mmo game, one instance runing one or more different scene

    1) the client request to switch scene
    2) if the goal scene not be held in the same game server, finding where is it then call SwitchServer function
    3) the SwitchServer function tell the gate that the player now changing the game server, the gate give up the player binding information
    4) the SwitchServer function packing the user's data and tell the world that the player now changing the game server
    5) the old game server destroy the player object (and save the player's data into DB server)
    6) the world transfer the message to the target game server
    7) the game server receives the changing server request and create a new player object
    8) the new game server tell the gate about the binding information
    9) the player enter the target scene and group in the new game server
    10) all finished 

solution 2: the mmo game, one instance runing all scenes as a game-line (easy to expand the capacity)

    1) the client request to switch scene
    2) if the goal scene not be held in the same game server, finding where is it then call SwitchServer function
    3) the SwitchServer function tell the gate that the player now changing the game server, the gate give up the player binding information
    4) the SwitchServer function packing the user's data and tell the world that the player now changing the game server
    5) the old game server destroy the player object (and save the player's data into DB server)
    6) the world transfer the message to the target game server
    7) the game server receives the changing server request and create a new player object
    8) the new game server tell the gate about the binding information
    9) the player enter the target scene and group in the new game server
    10) all finished 

solution 3: the game like kinds of lol or dota2 that creating a romm for fighting, one instance runing all scenes as a game-line (easy to expand the capacity)

    1) the client request to join the room
    2) if the goal room not be held in the same game server, finding where is it then call SwitchServer function
    3) the SwitchServer function tell the gate that the player now changing the game server, the gate give up the player binding information
    4) the SwitchServer function packing the user's data and tell the world that the player now changing the game server
    5) the old game server destroy the player object (and save the player's data into DB server)
    6) the world transfer the message to the target game server
    7) the game server receives the changing server request and create a new player object
    8) the new game server tell the gate about the binding information
    9) the player enter the target scene and group in the new game server
    10) all finished 

*/
// -------------------------------------------------------------------------


#ifndef NOAHFRAME_NFCSWITCHGAMESERVERMODULE_H
#define NOAHFRAME_NFCSWITCHGAMESERVERMODULE_H

#include "NFComm/NFPluginModule/NFISwitchGameServerModule.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIElementModule.h"
#include "NFComm/NFPluginModule/NFISceneProcessModule.h"
#include "NFComm/NFPluginModule/NFIPropertyModule.h"
#include "NFComm/NFPluginModule/NFILogModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"
#include "NFComm/NFPluginModule/NFINetClientModule.h"
#include "NFComm/NFPluginModule/NFIPackModule.h"
#include "NFComm/NFPluginModule/NFISceneAOIModule.h"
#include "NFComm/NFPluginModule/NFIGameServerNet_ServerModule.h"

class NFCSwitchGameServerModule
    : public NFISwitchGameServerModule
{
public:
    NFCSwitchGameServerModule( NFIPluginManager* p )
    {
        pPluginManager = p;
    }

    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute();
    virtual bool AfterInit();

    virtual bool SwitchServer(const NFGUID &self, const int nServer, const int nSceneID, const int nGroup);

protected:

    void OnClientReqSwitchServer(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);

    void OnReqSwitchServer(const NFSOCK nSockIndex, const int nMsgID, const char *msg, const uint32_t nLen);
    void OnAckSwitchServer(const NFSOCK nSockIndex, const int nMsgID, const char *msg, const uint32_t nLen);

private:
    NFINetModule*				    m_pNetModule;
    NFIKernelModule*			    m_pKernelModule;
    NFILogModule*				    m_pLogModule;
    NFIElementModule*			    m_pElementModule;
    NFISceneProcessModule*          m_pSceneProcessModule;
    NFIPropertyModule*              m_pPropertyModule;
    NFINetClientModule*             m_pNetClientModule;
    NFIPackModule*                  m_pPackModule;
    NFISceneAOIModule*              m_pSceneAOIModule;
    NFIGameServerNet_ServerModule*  m_pGameServerNet_ServerModule;
};


#endif //NOAHFRAME_NFCSWITCHGAMESERVERMODULE_H
