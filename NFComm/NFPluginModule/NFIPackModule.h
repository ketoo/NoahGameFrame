// -------------------------------------------------------------------------
//    @FileName         ：    NFIPackModule.h
//    @Author           ：    LvSheng.Huang
//    @Date             ：    2013-07-10
//    @Module           ：    NFIPackModule
//
// -------------------------------------------------------------------------

#ifndef _NFI_PACK_MODULE_H
#define _NFI_PACK_MODULE_H

#include <iostream>
#include "NFILogicModule.h"
#include "NFComm/Define/NFItem_def.h"

enum PackTableType
{
    None_Pack   = -1,
    NormalPack  = 0, // 普通背包
    ViewPack    = 1, // 可视装备背包
    BuyBackPack = 2, // 回购背包
    DropItemList = 3, // 副本掉落列表
    FriendEquipPack = 4, // 好友装备列表

    MaxPack,
};

#define MAX_PACK_SIZE 200    //最大背包数量

class NFIPackModule : public NFILogicModule
{
public:

    enum EDrawDropItemState
    {
        E_DRAW_STATE_NONE   = 0, // 初始状态
        E_DRAW_STATE_GAIN   = 1, // 可获得
        E_DRAW_STATE_RECV   = 2, // 已领取
    };

    enum DropItemListColType
    {
        DROP_MONSTER_ID     = 0,
        DROP_ITEM_ID        = 1,
        DROP_ITEM_COUNT     = 2,
        DROP_DRAW_STATE     = 3,
    };


    //添加装备:装备config,装备过期类型,孔数量，空里宝石列表，强化等级，附魔等级，元素卡片列表
    virtual const NFGUID& CreateEquip(const NFGUID& self, const std::string& strConfigName, const EGameItemExpiredType eExpiredType, const int nSoltCount,
                                  const NFIDataList& inlayCardList, const int nIntensiveLevel, const int nEnchantLevel, const std::string& enchantCardList) = 0;

    //添加装备:装备config,装备过期类型,孔数量
    virtual const NFGUID& CreateEquip(const NFGUID& self, const std::string& strConfigName, const EGameItemExpiredType eExpiredType, const int nSoltCount) = 0;

    //添加普通道具
    virtual bool CreateItem(const NFGUID& self, const std::string& strConfigName, const EGameItemExpiredType eExpiredType, const int nCount) = 0;

    //交换2个格子[换装]
    virtual bool SwapGrid(const NFGUID& self, const int nOrigin, const int nTarget, const PackTableType origin_name = PackTableType::NormalPack, const PackTableType target_name = PackTableType::NormalPack) = 0;

    //删除某格子内物品
    virtual bool DeleteGrid(const NFGUID& self, const int nOrigin, const PackTableType name = PackTableType::NormalPack) = 0;

    //////////////////////////////////////////////////////////////////////////
    //得到此格子的configID
    virtual const std::string& GetGridConfigID(const NFGUID& self, const int nRow, const PackTableType name = PackTableType::NormalPack) = 0;

    //设置道具数量
    virtual bool SetGridCount(const NFGUID& self, const int nOrigin, const int nCount, const PackTableType name = PackTableType::NormalPack) = 0;
    virtual NFINT64 GetGridCount(const NFGUID& self, const int nOrigin, const PackTableType name = PackTableType::NormalPack) = 0;

    //是否绑定
    virtual bool SetGridBan(const NFGUID& self, const int nOrigin, const bool bBan) = 0;
    virtual bool GetGridBan(const NFGUID& self, const int nOrigin) = 0;

    //设置装备强化等级
    virtual bool SetEquipIntensiveLevel(const NFGUID& self, const int nOrigin, const int nLevel) = 0;
    virtual int GetEquipIntensiveLevel(const NFGUID& self, const int nOrigin) = 0;

    //设置装备洞数量
    virtual bool SetEquipSlotCount(const NFGUID& self, const int nOrigin, const int nCount) = 0;
    virtual int GetEquipSlotCount(const NFGUID& self, const int nOrigin) = 0;

    //设置装镶嵌宝石ID
    virtual bool SetEquipInlayCard(const NFGUID& self, const int nOrigin, const EGameItemStoreType eSlotIndex, const std::string& strCardIndex) = 0;
    virtual const std::string& GetEquipInlayCard(const NFGUID& self, const int nOrigin, const EGameItemStoreType eSlotIndex) = 0;

    //设置装备附魔等级
    virtual bool SetEquipEnchantmentLevel(const NFGUID& self, const int nOrigin, const int nLevel) = 0;
    virtual int GetEquipEnchantmentLevel(const NFGUID& self, const int nOrigin) = 0;

    //设置装附魔宝石ID
    virtual bool SetEquipEnchantmentCard(const NFGUID& self, const int nOrigin, const std::string& strCardIndex) = 0;
    virtual const std::string& GetEquipEnchantmentCard(const NFGUID& self, const int nOrigin) = 0;

    //设置创建时间(如果是非永久道具)
    virtual bool SetEquipCreatTime(const NFGUID& self, const int nOrigin, const std::string& strTime) = 0;
    virtual const std::string& GetEquipCreatTime(const NFGUID& self, const int nOrigin) = 0;

    // 修改属性
    virtual bool SetGridData(const NFGUID& self, const int mRow, const int nCol, const NFIDataList& var, const PackTableType name = PackTableType::NormalPack) = 0;

    //得到还有多少空格子
    virtual int GetCanUsedCount(const NFGUID& self, const PackTableType name = PackTableType::NormalPack) const = 0;

    //得到某样物品有多少个
    virtual int QueryCount(const NFGUID& self, const std::string& strItemConfigID) const = 0;

    //消费N个某样物品
    virtual bool DeleteItem(const NFGUID& self, const std::string& strItemConfigID, const int nCount) = 0;

    // 掉落奖励
    virtual void DrawDropAward(const NFGUID& self, int& nMoney, int& nExp, NFIDataList& xItemList, NFIDataList& xCountList) = 0;
};

#endif