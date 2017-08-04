// -------------------------------------------------------------------------
//    @FileName         :    NFCEquipModule.h
//    @Author           :    chuanbo.guo
//    @Date             :    2016-01-12
//    @Module           :    NFCEquipModule
//
// -------------------------------------------------------------------------

#ifndef NFC_EUIP_MODULE_H
#define NFC_EUIP_MODULE_H

#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIEquipModule.h"
#include "NFComm/NFPluginModule/NFIElementModule.h"
#include "NFComm/NFPluginModule/NFISceneProcessModule.h"
#include "NFComm/NFPluginModule/NFIPropertyModule.h"
#include "NFComm/NFPluginModule/NFILogModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"
#include "NFComm/NFPluginModule/NFIPackModule.h"
#include "NFComm/NFPluginModule/NFICommonConfigModule.h"
#include "NFComm/NFPluginModule/NFIGameServerNet_ServerModule.h"

class NFCEquipModule
    : public NFIEquipModule
{
public:

    NFCEquipModule( NFIPluginManager* p )
    {
        pPluginManager = p;
    }
    virtual ~NFCEquipModule() {};

    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute();
    virtual bool AfterInit();

	/////////////for hero/////////////////////////////////////////////////////////////
	virtual bool DressEquipForHero(const NFGUID& self, const NFGUID& hero, const NFGUID& id);
	virtual bool TakeOffEquipForm(const NFGUID& self, const NFGUID& hero, const NFGUID& id);

protected:
	///////////////////////////////////////////////
	bool InlaystoneToEquip(const NFGUID& self, const NFGUID& xEquipID, const std::string& strStoneID, const int nHoleIndex);
	bool HoleToEquip(const NFGUID& self, const NFGUID& xEquipID);
	bool IntensifylevelToEquip(const NFGUID& self, const NFGUID& xEquipID);
	bool ElementlevelToEquip(const NFGUID& self, const NFGUID& xEquipID, const NFMsg::EGameElementType& eElemetType);

	/////////////random propertyid/////////////////////////////////////////////////////////////
	bool SetEquipRandPropertyID(const NFGUID& self, const NFGUID& id, const std::string& strPropertyID);
	/////////////sotne/////////////////////////////////////////////////////////////
	bool SetEquipInlayStoneID(const NFGUID& self, const NFGUID& id, const int eIndex, const std::string& strPropertyID);
	/////////////slot/hole/////////////////////////////////////////////////////////////
	bool AddEquipHoleCount(const NFGUID& self, const NFGUID& id);
	int GetEquipHoleCount(const NFGUID& self, const NFGUID& id);
	/////////////intensify/////////////////////////////////////////////////////////////
	bool AddEquipIntensifyLevel(const NFGUID& self, const NFGUID& id);
	int GetEquipIntensifyLevel(const NFGUID& self, const NFGUID& id);
	/////////////wind, fire, ice, light, poison/////////////////////////////////////////////////////////////
	bool AddEquipElementLevel(const NFGUID& self, const NFGUID& id, int eIndex);
	int GetEquipElementLevel(const NFGUID& self, const NFGUID& id, int eIndex);

protected:
    bool Ramdom(const int nNowLevel, const int nMaxLevel);
	//Equip
	void OnReqWearEquipMsg(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	void OnTakeOffEquipMsg(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);

	void OnIntensifylevelToEquipMsg(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	void OnHoleToEquipMsg(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	void OnInlaystoneToEquipMsg(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	void OnElementlevelToEquipMsg(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);

private:
	NFINetModule* m_pNetModule;
    NFIKernelModule* m_pKernelModule;
    NFILogModule* m_pLogModule;
    NFIElementModule* m_pElementModule;
    NFISceneProcessModule* m_pSceneProcessModule;
    NFIPropertyModule* m_pPropertyModule;
	NFIPackModule* m_pPackModule;
	NFICommonConfigModule* m_pCommonConfigModule;
	NFIGameServerNet_ServerModule* m_pGameServerNet_ServerModule;
};


#endif
