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
#include "NFComm/NFPluginModule/NFIAwardPackModule.h"
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

    //添加装备:装备config,装备过期类型,孔数量，空里宝石列表，强化等级，附魔等级，元素卡片列表
    virtual const NFGUID& CreateEquip( const NFGUID& self, const std::string& strConfigName, const EGameItemExpiredType eExpiredType, const int nSoltCount,
                           const NFIDataList& inlayCardList, const int nIntensiveLevel, const int nEnchantLevel, const std::string& enchantCardList );

    //添加装备:装备config,装备过期类型,孔数量
    virtual const NFGUID& CreateEquip( const NFGUID& self, const std::string& strConfigName, const EGameItemExpiredType eExpiredType, const int nSoltCount );

    //添加普通道具
    virtual bool CreateItem( const NFGUID& self, const std::string& strConfigName, const EGameItemExpiredType eExpiredType, const int nCount );


    //交换2个格子[换装]
    virtual bool SwapGrid( const NFGUID& self, const int nOrigin, const int nTarget, const PackTableType origin_name = PackTableType::NormalPack, const PackTableType target_name = PackTableType::NormalPack );

    //删除某格子内物品
    virtual bool DeleteGrid( const NFGUID& self, const int nOrigin, const PackTableType name = PackTableType::NormalPack );

    //////////////////////////////////////////////////////////////////////////
    //得到configID
    virtual const std::string& GetGridConfigID( const NFGUID& self, const int nRow, const PackTableType name = PackTableType::NormalPack );

    //设置道具数量
    virtual bool SetGridCount( const NFGUID& self, const int nOrigin, const int nCount, const PackTableType name = PackTableType::NormalPack );
    virtual NFINT64 GetGridCount( const NFGUID& self, const int nOrigin, const PackTableType name = PackTableType::NormalPack );

    //是否绑定
    virtual bool SetGridBan( const NFGUID& self, const int nOrigin, const bool bBan );
    virtual bool GetGridBan( const NFGUID& self, const int nOrigin );

    //设置装备强化等级
    virtual bool SetEquipIntensiveLevel( const NFGUID& self, const int nOrigin, const int nLevel );
    virtual int GetEquipIntensiveLevel( const NFGUID& self, const int nOrigin );

    //设置装备洞数量
    virtual bool SetEquipSlotCount( const NFGUID& self, const int nOrigin, const int nCount );
    virtual int GetEquipSlotCount( const NFGUID& self, const int nOrigin );

    //设置装镶嵌宝石ID
    virtual bool SetEquipInlayCard( const NFGUID& self, const int nOrigin, const EGameItemStoreType eSlotIndex, const std::string& strCardIndex );
    virtual const std::string& GetEquipInlayCard( const NFGUID& self, const int nOrigin, const EGameItemStoreType eSlotIndex );

    //设置装备附魔等级
    virtual bool SetEquipEnchantmentLevel( const NFGUID& self, const int nOrigin, const int nLevel );
    virtual int GetEquipEnchantmentLevel( const NFGUID& self, const int nOrigin );

    //设置装附魔宝石ID
    virtual bool SetEquipEnchantmentCard( const NFGUID& self, const int nOrigin, const std::string& strCardIndex );
    virtual const std::string& GetEquipEnchantmentCard( const NFGUID& self, const int nOrigin );

    //设置创建时间(如果是非永久道具)
    virtual bool SetEquipCreatTime( const NFGUID& self, const int nOrigin, const std::string& strTime );
    virtual const std::string& GetEquipCreatTime( const NFGUID& self, const int nOrigin );

    virtual bool SetGridData( const NFGUID& self, const int nRow, const int nCol, const NFIDataList& var, const PackTableType name = PackTableType::NormalPack );

	virtual int GetCanUsedCount(const NFGUID& self, const PackTableType name = PackTableType::NormalPack) const;

    //得到某样物品有多少个
    virtual int QueryCount(const NFGUID& self, const std::string& strItemConfigID) const;

    //消费N个某样物品
    virtual bool DeleteItem(const NFGUID& self, const std::string& strItemConfigID, const int nCount);

    // 掉落奖励
    virtual void DrawDropAward(const NFGUID& self, int& nMoney, int& nExp, NFIDataList& xItemList, NFIDataList& xCountList);
protected:
    virtual int RefreshEquipProperty( const NFGUID& self );
    virtual int RefreshEquipProperty( const NFGUID& self, const int nRow );
    
    virtual int AddEquipProperty( const NFGUID& self, const std::string& strConfigID, const int nRow );
    virtual int RemoveEquipProperty( const NFGUID& self, const std::string& strConfigID, const int nRow );

    virtual int CheckEquip();
    int OnClassObjectEvent( const NFGUID& self, const std::string& strClassNames, const CLASS_OBJECT_EVENT eClassEvent, const NFIDataList& var );
    int OnSwapTableRowEvent( const NFGUID& object, const int nEventID, const NFIDataList& var );
    int OnAddDropListEvent(const NFGUID& object, const int nEventID, const NFIDataList& var);
    int OnObjectPackRecordEvent( const NFGUID& self, const RECORD_EVENT_DATA& xEventData, const NFIDataList& oldVar, const NFIDataList& newVar );
    int OnObjectPackViewRecordEvent( const NFGUID& self, const RECORD_EVENT_DATA& xEventData, const NFIDataList& oldVar, const NFIDataList& newVar);

    PackTableType GetPackType( const std::string& name );
    bool pack_item_type_greater( NFIDataList* elem1, NFIDataList* elem2 );
    bool can_normal_pack_item_swap( const NFGUID& self, NF_SHARE_PTR<NFIRecord> pOriginRecord, NF_SHARE_PTR<NFIRecord> pTargetRecord, const int origin, const int target ); // 判断是否可以交换普通背包物品(装备特殊判断)
    
    // 计算掉落包
    bool ComputerDropPack(NF_SHARE_PTR<NFIObject> pObject, const NFGUID identMonster,  const std::string& strDropPackConfig);
    
    // 添加掉落道具
    void AddDropItem(const NFGUID& self, const NFIDataList& var);
    int OnObjectBeKilled(const NFGUID& self, const int nEventID, const NFIDataList& var);
private:
    NFIEventProcessModule* m_pEventProcessModule;
    NFIKernelModule* m_pKernelModule;
    NFILogModule* m_pLogModule;
    NFIElementInfoModule* m_pElementInfoModule;
    NFISceneProcessModule* m_pSceneProcessModule;
    NFIPropertyModule* m_pPropertyModule;
    NFIUUIDModule* m_pUUIDModule;
    NFIAwardPackModule* m_pAwardPackModule;
};


#endif
