// -------------------------------------------------------------------------
//    @FileName      :    NFCEquipModule.h
//    @Author           :    chuanbo.guo
//    @Date             :    2016-01-12
//    @Module           :    NFCEquipModule
//
// -------------------------------------------------------------------------

#ifndef NFC_EUIP_MODULE_H
#define NFC_EUIP_MODULE_H

#include <boost/static_assert.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
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

private:
    NFIKernelModule* m_pKernelModule;
    NFILogModule* m_pLogModule;
    NFIElementInfoModule* m_pElementInfoModule;
    NFISceneProcessModule* m_pSceneProcessModule;
    NFIPropertyModule* m_pPropertyModule;
    NFIUUIDModule* m_pUUIDModule;
    NFIPackModule* m_pPackModule;
};


#endif
