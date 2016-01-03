// -------------------------------------------------------------------------
//    @FileName      :    NFCPackModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-07-10
//    @Module           :    NFCPackModule
//
// -------------------------------------------------------------------------

#ifndef NFC_PACK_MODULE_H
#define NFC_PACK_MODULE_H

#include <boost/static_assert.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIGameLogicModule.h"
#include "NFComm/NFPluginModule/NFIEventProcessModule.h"
#include "NFComm/NFPluginModule/NFIPackModule.h"
#include "NFComm/NFPluginModule/NFIElementInfoModule.h"
#include "NFComm/NFPluginModule/NFISceneProcessModule.h"
#include "NFComm/NFPluginModule/NFIPropertyModule.h"
#include "NFComm/NFPluginModule/NFILogModule.h"
#include "NFComm/NFPluginModule/NFIUUIDModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"

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
    virtual bool Execute( const float fLasFrametime, const float fStartedTime );
    virtual bool AfterInit();

    //添加装备:装备config
    virtual const NFGUID& CreateEquip( const NFGUID& self, const std::string& strConfigName);

    //添加普通道具
    virtual bool CreateItem( const NFGUID& self, const std::string& strConfigName, const int nCount );

    //删除某格子内物品
    virtual bool DeleteGrid( const NFGUID& self, const int nOrigin);

    //////////////////////////////////////////////////////////////////////////
    //得到configID
    virtual const std::string& GetGridConfigID( const NFGUID& self, const int nRow);

    //设置道具数量
    virtual bool SetGridCount( const NFGUID& self, const int nOrigin, const int nCount);
    virtual NFINT64 GetGridCount( const NFGUID& self, const int nOrigin);

    //是否绑定
    virtual bool SetGridBan( const NFGUID& self, const int nOrigin, const bool bBan );
    virtual bool GetGridBan( const NFGUID& self, const int nOrigin );

    //设置创建时间(如果是非永久道具)
    virtual bool SetEquipCreatTime( const NFGUID& self, const int nOrigin, const const NFINT64 nTime  );
    virtual const NFINT64 GetEquipCreatTime( const NFGUID& self, const int nOrigin );

    //得到某样物品有多少个
    virtual int QueryCount(const NFGUID& self, const std::string& strItemConfigID) const;

    //消费N个某样物品
    virtual bool DeleteItem(const NFGUID& self, const std::string& strItemConfigID, const int nCount);

protected:
    int RefreshEquipProperty( const NFGUID& self );
    int RefreshEquipProperty( const NFGUID& self, const int nRow );
    
    int AddEquipProperty( const NFGUID& self, const std::string& strConfigID, const int nRow );
    int RemoveEquipProperty( const NFGUID& self, const std::string& strConfigID, const int nRow );

    int OnClassObjectEvent( const NFGUID& self, const std::string& strClassNames, const CLASS_OBJECT_EVENT eClassEvent, const NFIDataList& var );
    int OnSwapTableRowEvent( const NFGUID& object, const int nEventID, const NFIDataList& var );
    int OnObjectPackViewRecordEvent( const NFGUID& self, const RECORD_EVENT_DATA& xEventData, const NFIDataList& oldVar, const NFIDataList& newVar );

    // 添加掉落道具
    void AddDropItem(const NFGUID& self, const NFIDataList& var);
    int OnObjectBeKilled(const NFGUID& self, const int nEventID, const NFIDataList& var);


	bool SetGridData( const NFGUID& self, const int nRow, const int nCol, const NFIDataList& var);

private:
    NFIEventProcessModule* m_pEventProcessModule;
    NFIKernelModule* m_pKernelModule;
    NFILogModule* m_pLogModule;
    NFIElementInfoModule* m_pElementInfoModule;
    NFISceneProcessModule* m_pSceneProcessModule;
    NFIPropertyModule* m_pPropertyModule;
    NFIUUIDModule* m_pUUIDModule;
};


#endif
