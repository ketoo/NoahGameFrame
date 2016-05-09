// -------------------------------------------------------------------------
//    @FileName         :   NFCTileModule.h
//    @Author           :   Nick Yang
//    @Date             :   2016/03/07
//    @Module           :   NFCTileModule
//
// -------------------------------------------------------------------------

#ifndef NFC_TILE_MODULE_H
#define NFC_TILE_MODULE_H

#include "NFComm/NFCore/NFMap.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIGameLogicModule.h"
#include "NFComm/NFPluginModule/NFITileModule.h"
#include "NFComm/NFPluginModule/NFILogicModule.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFILogicClassModule.h"
#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"
#include "NFComm/NFPluginModule/NFIGameServerNet_ServerModule.h"
#include "NFComm/NFPluginModule/NFIUUIDModule.h"
#include "NFComm/NFPluginModule/NFIElementInfoModule.h"

class NFCTileModule
    : public NFITileModule
{
public:
    NFCTileModule(NFIPluginManager* p)
    {
        pPluginManager = p;
    }

    virtual ~NFCTileModule() {};

    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute();
    virtual bool AfterInit();

    virtual void SetTileData(const NFGUID& self, const int x, const int y, const int type);

protected:
    int OnClassObjectEvent(const NFGUID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT eEvent, const NFIDataList& args);

    void TransferTileDataToMap(const NFGUID& self);
    void TransferTileDataToRecord(const NFGUID& self);

protected:
    NFILogicClassModule* m_pLogicClassModule;
    NFIElementInfoModule* m_pElementInfoModule;
	NFIKernelModule* m_pKernelModule;
	NFIGameServerNet_ServerModule* m_pGameServerNet_ServerModule;
    NFIUUIDModule* m_pUUIDModule;

};

#endif // !NFC_TILE_MODULE_H
