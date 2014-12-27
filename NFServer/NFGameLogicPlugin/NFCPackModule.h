// -------------------------------------------------------------------------
//    @FileName      :    NFCPackModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-07-10
//    @Module           :    NFCPackModule
//
// -------------------------------------------------------------------------

#ifndef _NFC_PACK_MODULE_H_
#define _NFC_PACK_MODULE_H_

#include <boost/static_assert.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIGameLogicModule.h"
#include "NFComm/NFPluginModule/NFIEventProcessModule.h"
#include "NFComm/NFPluginModule/NFIPackModule.h"
#include "NFComm/NFPluginModule/NFIElementInfoModule.h"
#include "NFComm/NFPluginModule/NFISceneProcessModule.h"
#include "NFComm/NFPluginModule/NFIPropertyModule.h"
#include "NFComm/NFPluginModule/NFILogModule.h"

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
    virtual NFIDENTID CreateEquip( const NFIDENTID& self, const std::string& strConfigName, const EGameItemExpiredType eExpiredType, const int nSoltCount,
                           const NFIValueList& inlayCardList, const int nIntensiveLevel, const int nEnchantLevel, const std::string& enchantCardList );

    //添加装备:装备config,装备过期类型,孔数量
    virtual NFIDENTID CreateEquip( const NFIDENTID& self, const std::string& strConfigName, const EGameItemExpiredType eExpiredType, const int nSoltCount );

    //添加普通道具
    virtual bool CreateItem( const NFIDENTID& self, const std::string& strConfigName, const EGameItemExpiredType eExpiredType, const int nCount );


    //交换2个格子[换装]
    virtual bool SwapGrid( const NFIDENTID& self, const int nOrigin, const int nTarget, const PackTableType origin_name = PackTableType::NormalPack, const PackTableType target_name = PackTableType::NormalPack );

    //删除某格子内物品
    virtual bool DeleteGrid( const NFIDENTID& self, const int nOrigin, const PackTableType name = PackTableType::NormalPack );

    //////////////////////////////////////////////////////////////////////////
    //得到configID
    virtual const std::string& GetGridConfigID( const NFIDENTID& self, const int nRow, const PackTableType name = PackTableType::NormalPack );

    //设置道具数量
    virtual bool SetGridCount( const NFIDENTID& self, const int nOrigin, const int nCount, const PackTableType name = PackTableType::NormalPack );
    virtual int GetGridCount( const NFIDENTID& self, const int nOrigin, const PackTableType name = PackTableType::NormalPack );

    //是否绑定
    virtual bool SetGridBan( const NFIDENTID& self, const int nOrigin, const bool bBan );
    virtual bool GetGridBan( const NFIDENTID& self, const int nOrigin );

    //设置装备强化等级
    virtual bool SetEquipIntensiveLevel( const NFIDENTID& self, const int nOrigin, const int nLevel );
    virtual int GetEquipIntensiveLevel( const NFIDENTID& self, const int nOrigin );

    //设置装备洞数量
    virtual bool SetEquipSlotCount( const NFIDENTID& self, const int nOrigin, const int nCount );
    virtual int GetEquipSlotCount( const NFIDENTID& self, const int nOrigin );

    //设置装镶嵌宝石ID
    virtual bool SetEquipInlayCard( const NFIDENTID& self, const int nOrigin, const EGameItemStoreType eSlotIndex, const std::string& strCardIndex );
    virtual const std::string& GetEquipInlayCard( const NFIDENTID& self, const int nOrigin, const EGameItemStoreType eSlotIndex );

    //设置装备附魔等级
    virtual bool SetEquipEnchantmentLevel( const NFIDENTID& self, const int nOrigin, const int nLevel );
    virtual int GetEquipEnchantmentLevel( const NFIDENTID& self, const int nOrigin );

    //设置装附魔宝石ID
    virtual bool SetEquipEnchantmentCard( const NFIDENTID& self, const int nOrigin, const std::string& strCardIndex );
    virtual const std::string& GetEquipEnchantmentCard( const NFIDENTID& self, const int nOrigin );

    //设置创建时间(如果是非永久道具)
    virtual bool SetEquipCreatTime( const NFIDENTID& self, const int nOrigin, const std::string& strTime );
    virtual const std::string& GetEquipCreatTime( const NFIDENTID& self, const int nOrigin );

    virtual bool SetGridData( const NFIDENTID& self, const int nRow, const int nCol, const NFIValueList& var, const PackTableType name = PackTableType::NormalPack );

	virtual int GetCanUsedCount(const NFIDENTID& self, const PackTableType name = PackTableType::NormalPack) const;

    //得到某样物品有多少个
    virtual int QueryCount(const NFIDENTID& self, const std::string& strItemConfigID) const;

    //消费N个某样物品
    virtual bool DeleteItem(const NFIDENTID& self, const std::string& strItemConfigID, const int nCount);
protected:
    virtual int RefreshEquipProperty( const NFIDENTID& self );
    virtual int RefreshEquipProperty( const NFIDENTID& self, const int nRow );
    
    virtual int AddEquipProperty( const NFIDENTID& self, const std::string& strConfigID, const int nRow );
    virtual int RemoveEquipProperty( const NFIDENTID& self, const std::string& strConfigID, const int nRow );

    virtual int CheckEquip();

private:

protected:
    int OnClassObjectEvent( const NFIDENTID& self, const std::string& strClassNames, const CLASS_OBJECT_EVENT eClassEvent, const NFIValueList& var );


    int OnSwapTableRowEvent( const NFIDENTID& object, const int nEventID, const NFIValueList& var );
protected:
    int OnObjectPackRecordEvent( const NFIDENTID& self, const std::string& strRecordName, const int nOpType, const int nRow, const int nCol, const NFIValueList& oldVar, const NFIValueList& newVar, const NFIValueList& argVar );
    int OnObjectPackViewRecordEvent( const NFIDENTID& self, const std::string& strRecordName, const int nOpType, const int nRow, const int nTargetRow, const NFIValueList& oldVar, const NFIValueList& newVar, const NFIValueList& argVar );

protected:
    PackTableType GetPackType( const std::string& name );
    bool pack_item_type_greater( NFIValueList* elem1, NFIValueList* elem2 );
    bool can_normal_pack_item_swap( const NFIDENTID& self, NFIRecord* pOriginRecord, NFIRecord* pTargetRecord, const int origin, const int target ); // 判断是否可以交换普通背包物品(装备特殊判断)
private:
    //char* mstrPackTableName;
    //char* mstrViewTableName;
    //   char* mstrBuyBackPackTableName;




    NFIEventProcessModule* m_pEventProcessModule;
    NFIKernelModule* m_pKernelModule;
    NFILogModule* m_pLogModule;
    NFIElementInfoModule* m_pElementInfoModule;
    NFISceneProcessModule* m_pSceneProcessModule;
    NFIPropertyModule* m_pPropertyModule;

};


#endif
