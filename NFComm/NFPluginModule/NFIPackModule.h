// -------------------------------------------------------------------------
//    @FileName         ：    NFIPackModule.h
//    @Author           ：    LvSheng.Huang
//    @Date             ：    2013-07-10
//    @Module           ：    NFIPackModule
//
// -------------------------------------------------------------------------

#ifndef _NFI_PACK_MODULE_H_
#define _NFI_PACK_MODULE_H_

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
        E_DRAW_STATE_GAIN   = 1, // 获得
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
    virtual NFIDENTID CreateEquip(const NFIDENTID& self, const std::string& strConfigName, const EGameItemExpiredType eExpiredType, const int nSoltCount,
                                  const NFIDataList& inlayCardList, const int nIntensiveLevel, const int nEnchantLevel, const std::string& enchantCardList) = 0;

    //添加装备:装备config,装备过期类型,孔数量
    virtual NFIDENTID CreateEquip(const NFIDENTID& self, const std::string& strConfigName, const EGameItemExpiredType eExpiredType, const int nSoltCount) = 0;

    //添加普通道具
    virtual bool CreateItem(const NFIDENTID& self, const std::string& strConfigName, const EGameItemExpiredType eExpiredType, const int nCount) = 0;

    //交换2个格子[换装]
    virtual bool SwapGrid(const NFIDENTID& self, const int nOrigin, const int nTarget, const PackTableType origin_name = PackTableType::NormalPack, const PackTableType target_name = PackTableType::NormalPack) = 0;

    //删除某格子内物品
    virtual bool DeleteGrid(const NFIDENTID& self, const int nOrigin, const PackTableType name = PackTableType::NormalPack) = 0;

    //////////////////////////////////////////////////////////////////////////
    //得到此格子的configID
    virtual const std::string& GetGridConfigID(const NFIDENTID& self, const int nRow, const PackTableType name = PackTableType::NormalPack) = 0;

    //设置道具数量
    virtual bool SetGridCount(const NFIDENTID& self, const int nOrigin, const int nCount, const PackTableType name = PackTableType::NormalPack) = 0;
    virtual int GetGridCount(const NFIDENTID& self, const int nOrigin, const PackTableType name = PackTableType::NormalPack) = 0;

    //是否绑定
    virtual bool SetGridBan(const NFIDENTID& self, const int nOrigin, const bool bBan) = 0;
    virtual bool GetGridBan(const NFIDENTID& self, const int nOrigin) = 0;

    //设置装备强化等级
    virtual bool SetEquipIntensiveLevel(const NFIDENTID& self, const int nOrigin, const int nLevel) = 0;
    virtual int GetEquipIntensiveLevel(const NFIDENTID& self, const int nOrigin) = 0;

    //设置装备洞数量
    virtual bool SetEquipSlotCount(const NFIDENTID& self, const int nOrigin, const int nCount) = 0;
    virtual int GetEquipSlotCount(const NFIDENTID& self, const int nOrigin) = 0;

    //设置装镶嵌宝石ID
    virtual bool SetEquipInlayCard(const NFIDENTID& self, const int nOrigin, const EGameItemStoreType eSlotIndex, const std::string& strCardIndex) = 0;
    virtual const std::string& GetEquipInlayCard(const NFIDENTID& self, const int nOrigin, const EGameItemStoreType eSlotIndex) = 0;

    //设置装备附魔等级
    virtual bool SetEquipEnchantmentLevel(const NFIDENTID& self, const int nOrigin, const int nLevel) = 0;
    virtual int GetEquipEnchantmentLevel(const NFIDENTID& self, const int nOrigin) = 0;

    //设置装附魔宝石ID
    virtual bool SetEquipEnchantmentCard(const NFIDENTID& self, const int nOrigin, const std::string& strCardIndex) = 0;
    virtual const std::string& GetEquipEnchantmentCard(const NFIDENTID& self, const int nOrigin) = 0;

    //设置创建时间(如果是非永久道具)
    virtual bool SetEquipCreatTime(const NFIDENTID& self, const int nOrigin, const std::string& strTime) = 0;
    virtual const std::string& GetEquipCreatTime(const NFIDENTID& self, const int nOrigin) = 0;

    // 修改属性
    virtual bool SetGridData(const NFIDENTID& self, const int mRow, const int nCol, const NFIDataList& var, const PackTableType name = PackTableType::NormalPack) = 0;

    //得到还有多少空格子
    virtual int GetCanUsedCount(const NFIDENTID& self, const PackTableType name = PackTableType::NormalPack) const = 0;

    //得到某样物品有多少个
    virtual int QueryCount(const NFIDENTID& self, const std::string& strItemConfigID) const = 0;

    //消费N个某样物品
    virtual bool DeleteItem(const NFIDENTID& self, const std::string& strItemConfigID, const int nCount) = 0;

};

#endif