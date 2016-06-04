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
#include "NFComm/NFPluginModule/NFIGameLogicModule.h"
#include "NFComm/NFPluginModule/NFIEquipModule.h"
#include "NFComm/NFPluginModule/NFIElementInfoModule.h"
#include "NFComm/NFPluginModule/NFISceneProcessModule.h"
#include "NFComm/NFPluginModule/NFIPropertyModule.h"
#include "NFComm/NFPluginModule/NFILogModule.h"
#include "NFComm/NFPluginModule/NFIUUIDModule.h"
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

    virtual bool WearEquip(const NFGUID& self, const NFGUID& xEquipID, const NFGUID& xTareget);
    virtual bool TakeOffEquip(const NFGUID& self, const NFGUID& xEquipID, const NFGUID& xTareget);
    virtual bool IntensifylevelToEquip(const NFGUID& self, const NFGUID& xEquipID);
    virtual bool HoleToEquip(const NFGUID& self, const NFGUID& xEquipID);
    virtual bool InlaystoneToEquip(const NFGUID& self, const NFGUID& xEquipID, const std::string& strStoneID, const int nHoleIndex);
    virtual bool ElementlevelToEquip(const NFGUID& self, const NFGUID& xEquipID, const NFMsg::EGameElementType& eElemetType);

    /////////////英雄装备和背包互换/////////////////////////////////////////////////////////////

    virtual bool DressEquipForHero(const NFGUID& self, const NFGUID& hero, const NFGUID& id);
    virtual bool TakeOffEquipForm(const NFGUID& self, const NFGUID& hero, const NFGUID& id);

    /////////////装备的随机属性/////////////////////////////////////////////////////////////

    virtual int SetEquipRandPropertyID(const NFGUID& self, const NFGUID& id, const std::string& strPropertyID);
    virtual const std::string& GetEquipRandPropertyID(const NFGUID& self, const NFGUID& id);

    /////////////装备的打洞/////////////////////////////////////////////////////////////

    virtual bool SetEquipHoleCount(const NFGUID& self, const NFGUID& id, const int nCount);
    virtual int GetEquipHoleCount(const NFGUID& self, const NFGUID& id);

    /////////////装备镶嵌的石头/////////////////////////////////////////////////////////////

    virtual bool SetEquipInlayStoneID(const NFGUID& self, const NFGUID& id, NFrame::Player::BagEquipList eIndex, const std::string& strPropertyID);
    virtual const std::string& GetEquipInlayStoneID(const NFGUID& self, const NFGUID& id, NFrame::Player::BagEquipList eIndex);

    /////////////装备强化的等级/////////////////////////////////////////////////////////////

    virtual bool SetEquipIntensifyLevel(const NFGUID& self, const NFGUID& id, const int nLevel);
    virtual int GetEquipIntensifyLevel(const NFGUID& self, const NFGUID& id);

    /////////////装备元素的等级/////////////////////////////////////////////////////////////

    virtual bool SetEquipElementLevel(const NFGUID& self, const NFGUID& id, NFrame::Player::BagEquipList eIndex, const int nLevel);
    virtual int GetEquipElementLevel(const NFGUID& self, const NFGUID& id, NFrame::Player::BagEquipList eIndex);

protected:
    virtual bool RamdomSucess(const int nNowLevel, const int nMaxLevel);
    virtual float GetEquipLevelRate(const int nNowLevel);
    int OnClassObjectEvent( const NFGUID& self, const std::string& strClassNames, const CLASS_OBJECT_EVENT eClassEvent, const NFIDataList& var );
    int OnObjectBagEquipRecordEvent( const NFGUID& self, const RECORD_EVENT_DATA& xEventData, const NFIDataList::TData& oldVar, const NFIDataList::TData& newVar );

    int AddEquipProperty( const NFGUID& self );
    int AddEquipProperty( const NFGUID& self, const int nRow );

    int AddEffectDataProperty( const NFGUID& self, const std::string& strConfigID, const float fAddRate);
    int RemoveEffectDataProperty( const NFGUID& self, const std::string& strConfigID, const float fAddRate);
    int RemoveEquipProperty( const NFGUID& self, const int nRow );

    bool NeedEquipProperty( const NFGUID& self, const NFGUID& xWearID);

protected:
	//Equip
	void OnIntensifylevelToEquipMsg(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	void OnHoleToEquipMsg(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	void OnInlaystoneToEquipMsg(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	void OnElementlevelToEquipMsg(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	void OnReqWearEquipMsg(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	void OnTakeOffEquipMsg(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);

private:
    NFIKernelModule* m_pKernelModule;
    NFILogModule* m_pLogModule;
    NFIElementInfoModule* m_pElementInfoModule;
    NFISceneProcessModule* m_pSceneProcessModule;
    NFIPropertyModule* m_pPropertyModule;
    NFIUUIDModule* m_pUUIDModule;
	NFIPackModule* m_pPackModule;
	NFICommonConfigModule* m_pCommonConfigModule;
	NFIGameServerNet_ServerModule* m_pGameServerNet_ServerModule;
};


#endif
