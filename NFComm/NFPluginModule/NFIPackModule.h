// -------------------------------------------------------------------------
//    @FileName         ��    NFIPackModule.h
//    @Author           ��    LvSheng.Huang
//    @Date             ��    2013-07-10
//    @Module           ��    NFIPackModule
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
    NormalPack  = 0, // ��ͨ����
    ViewPack    = 1, // ����װ������
    BuyBackPack = 2, // �ع�����
    DropItemList = 3, // ���������б�
    FriendEquipPack = 4, // ����װ���б�

    MaxPack,
};

#define MAX_PACK_SIZE 200    //��󱳰�����

class NFIPackModule : public NFILogicModule
{
public:

    enum EDrawDropItemState
    {
        E_DRAW_STATE_NONE   = 0, // ��ʼ״̬
        E_DRAW_STATE_GAIN   = 1, // �ɻ��
        E_DRAW_STATE_RECV   = 2, // ����ȡ
    };

    enum DropItemListColType
    {
        DROP_MONSTER_ID     = 0,
        DROP_ITEM_ID        = 1,
        DROP_ITEM_COUNT     = 2,
        DROP_DRAW_STATE     = 3,
    };


    //���װ��:װ��config,װ����������,�����������ﱦʯ�б�ǿ���ȼ�����ħ�ȼ���Ԫ�ؿ�Ƭ�б�
    virtual const NFGUID& CreateEquip(const NFGUID& self, const std::string& strConfigName, const EGameItemExpiredType eExpiredType, const int nSoltCount,
                                  const NFIDataList& inlayCardList, const int nIntensiveLevel, const int nEnchantLevel, const std::string& enchantCardList) = 0;

    //���װ��:װ��config,װ����������,������
    virtual const NFGUID& CreateEquip(const NFGUID& self, const std::string& strConfigName, const EGameItemExpiredType eExpiredType, const int nSoltCount) = 0;

    //�����ͨ����
    virtual bool CreateItem(const NFGUID& self, const std::string& strConfigName, const EGameItemExpiredType eExpiredType, const int nCount) = 0;

    //����2������[��װ]
    virtual bool SwapGrid(const NFGUID& self, const int nOrigin, const int nTarget, const PackTableType origin_name = PackTableType::NormalPack, const PackTableType target_name = PackTableType::NormalPack) = 0;

    //ɾ��ĳ��������Ʒ
    virtual bool DeleteGrid(const NFGUID& self, const int nOrigin, const PackTableType name = PackTableType::NormalPack) = 0;

    //////////////////////////////////////////////////////////////////////////
    //�õ��˸��ӵ�configID
    virtual const std::string& GetGridConfigID(const NFGUID& self, const int nRow, const PackTableType name = PackTableType::NormalPack) = 0;

    //���õ�������
    virtual bool SetGridCount(const NFGUID& self, const int nOrigin, const int nCount, const PackTableType name = PackTableType::NormalPack) = 0;
    virtual NFINT64 GetGridCount(const NFGUID& self, const int nOrigin, const PackTableType name = PackTableType::NormalPack) = 0;

    //�Ƿ��
    virtual bool SetGridBan(const NFGUID& self, const int nOrigin, const bool bBan) = 0;
    virtual bool GetGridBan(const NFGUID& self, const int nOrigin) = 0;

    //����װ��ǿ���ȼ�
    virtual bool SetEquipIntensiveLevel(const NFGUID& self, const int nOrigin, const int nLevel) = 0;
    virtual int GetEquipIntensiveLevel(const NFGUID& self, const int nOrigin) = 0;

    //����װ��������
    virtual bool SetEquipSlotCount(const NFGUID& self, const int nOrigin, const int nCount) = 0;
    virtual int GetEquipSlotCount(const NFGUID& self, const int nOrigin) = 0;

    //����װ��Ƕ��ʯID
    virtual bool SetEquipInlayCard(const NFGUID& self, const int nOrigin, const EGameItemStoreType eSlotIndex, const std::string& strCardIndex) = 0;
    virtual const std::string& GetEquipInlayCard(const NFGUID& self, const int nOrigin, const EGameItemStoreType eSlotIndex) = 0;

    //����װ����ħ�ȼ�
    virtual bool SetEquipEnchantmentLevel(const NFGUID& self, const int nOrigin, const int nLevel) = 0;
    virtual int GetEquipEnchantmentLevel(const NFGUID& self, const int nOrigin) = 0;

    //����װ��ħ��ʯID
    virtual bool SetEquipEnchantmentCard(const NFGUID& self, const int nOrigin, const std::string& strCardIndex) = 0;
    virtual const std::string& GetEquipEnchantmentCard(const NFGUID& self, const int nOrigin) = 0;

    //���ô���ʱ��(����Ƿ����õ���)
    virtual bool SetEquipCreatTime(const NFGUID& self, const int nOrigin, const std::string& strTime) = 0;
    virtual const std::string& GetEquipCreatTime(const NFGUID& self, const int nOrigin) = 0;

    // �޸�����
    virtual bool SetGridData(const NFGUID& self, const int mRow, const int nCol, const NFIDataList& var, const PackTableType name = PackTableType::NormalPack) = 0;

    //�õ����ж��ٿո���
    virtual int GetCanUsedCount(const NFGUID& self, const PackTableType name = PackTableType::NormalPack) const = 0;

    //�õ�ĳ����Ʒ�ж��ٸ�
    virtual int QueryCount(const NFGUID& self, const std::string& strItemConfigID) const = 0;

    //����N��ĳ����Ʒ
    virtual bool DeleteItem(const NFGUID& self, const std::string& strItemConfigID, const int nCount) = 0;

    // ���佱��
    virtual void DrawDropAward(const NFGUID& self, int& nMoney, int& nExp, NFIDataList& xItemList, NFIDataList& xCountList) = 0;
};

#endif