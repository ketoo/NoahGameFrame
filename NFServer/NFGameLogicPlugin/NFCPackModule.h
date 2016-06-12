// -------------------------------------------------------------------------
//    @FileName         :    NFCPackModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-07-10
//    @Module           :    NFCPackModule
//
// -------------------------------------------------------------------------

#ifndef NFC_PACK_MODULE_H
#define NFC_PACK_MODULE_H

#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIGameLogicModule.h"
#include "NFComm/NFPluginModule/NFIPackModule.h"
#include "NFComm/NFPluginModule/NFIElementModule.h"
#include "NFComm/NFPluginModule/NFISceneProcessModule.h"
#include "NFComm/NFPluginModule/NFIPropertyModule.h"
#include "NFComm/NFPluginModule/NFILogModule.h"
#include "NFComm/NFPluginModule/NFIUUIDModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"

class NFCPackModule
    : public NFIPackModule
{
public:

    NFCPackModule( NFIPluginManager* p )
    {
        pPluginManager = p;
    }
    virtual ~NFCPackModule() {};

    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute();
    virtual bool AfterInit();

	/////////////基础/////////////////////////////////////////////////////////////
	virtual const NFGUID& CreateEquip( const NFGUID& self, const std::string& strConfigName);
	virtual bool CreateItem(const NFGUID& self, const std::string& strConfigName, const int nCount);

	virtual bool DeleteEquip(const NFGUID& self, const NFGUID& id);
    virtual bool DeleteItem(const NFGUID& self, const std::string& strItemConfigID, const int nCount);
    virtual bool EnoughItem(const NFGUID& self, const std::string& strItemConfigID, const int nCount);


    virtual PackTableType GetPackBagType(int nItemType);

protected:

	int FindItemRowByConfig(const NFGUID& self, const std::string& strItemConfigID);
    int OnClassObjectEvent(const NFGUID& self, const std::string& strClassNames, const CLASS_OBJECT_EVENT eClassEvent, const NFIDataList& var );

    // 添加掉落道具
    int OnObjectBeKilled(const NFGUID& self, const int nEventID, const NFIDataList& var);

private:
    NFIKernelModule* m_pKernelModule;
    NFILogModule* m_pLogModule;
    NFIElementModule* m_pElementModule;
    NFISceneProcessModule* m_pSceneProcessModule;
    NFIPropertyModule* m_pPropertyModule;
    NFIUUIDModule* m_pUUIDModule;
};


#endif
