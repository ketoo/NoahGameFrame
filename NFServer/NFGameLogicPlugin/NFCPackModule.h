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

    //���װ��:װ��config,װ����������,�����������ﱦʯ�б�ǿ���ȼ�����ħ�ȼ���Ԫ�ؿ�Ƭ�б�
    virtual NFIDENTID CreateEquip( const NFIDENTID& self, const std::string& strConfigName, const EGameItemExpiredType eExpiredType, const int nSoltCount,
                           const NFIValueList& inlayCardList, const int nIntensiveLevel, const int nEnchantLevel, const std::string& enchantCardList );

    //���װ��:װ��config,װ����������,������
    virtual NFIDENTID CreateEquip( const NFIDENTID& self, const std::string& strConfigName, const EGameItemExpiredType eExpiredType, const int nSoltCount );

    //�����ͨ����
    virtual bool CreateItem( const NFIDENTID& self, const std::string& strConfigName, const EGameItemExpiredType eExpiredType, const int nCount );


    //����2������[��װ]
    virtual bool SwapGrid( const NFIDENTID& self, const int nOrigin, const int nTarget, const PackTableType origin_name = PackTableType::NormalPack, const PackTableType target_name = PackTableType::NormalPack );

    //ɾ��ĳ��������Ʒ
    virtual bool DeleteGrid( const NFIDENTID& self, const int nOrigin, const PackTableType name = PackTableType::NormalPack );

    //////////////////////////////////////////////////////////////////////////
    //�õ�configID
    virtual const std::string& GetGridConfigID( const NFIDENTID& self, const int nRow, const PackTableType name = PackTableType::NormalPack );

    //���õ�������
    virtual bool SetGridCount( const NFIDENTID& self, const int nOrigin, const int nCount, const PackTableType name = PackTableType::NormalPack );
    virtual int GetGridCount( const NFIDENTID& self, const int nOrigin, const PackTableType name = PackTableType::NormalPack );

    //�Ƿ��
    virtual bool SetGridBan( const NFIDENTID& self, const int nOrigin, const bool bBan );
    virtual bool GetGridBan( const NFIDENTID& self, const int nOrigin );

    //����װ��ǿ���ȼ�
    virtual bool SetEquipIntensiveLevel( const NFIDENTID& self, const int nOrigin, const int nLevel );
    virtual int GetEquipIntensiveLevel( const NFIDENTID& self, const int nOrigin );

    //����װ��������
    virtual bool SetEquipSlotCount( const NFIDENTID& self, const int nOrigin, const int nCount );
    virtual int GetEquipSlotCount( const NFIDENTID& self, const int nOrigin );

    //����װ��Ƕ��ʯID
    virtual bool SetEquipInlayCard( const NFIDENTID& self, const int nOrigin, const EGameItemStoreType eSlotIndex, const std::string& strCardIndex );
    virtual const std::string& GetEquipInlayCard( const NFIDENTID& self, const int nOrigin, const EGameItemStoreType eSlotIndex );

    //����װ����ħ�ȼ�
    virtual bool SetEquipEnchantmentLevel( const NFIDENTID& self, const int nOrigin, const int nLevel );
    virtual int GetEquipEnchantmentLevel( const NFIDENTID& self, const int nOrigin );

    //����װ��ħ��ʯID
    virtual bool SetEquipEnchantmentCard( const NFIDENTID& self, const int nOrigin, const std::string& strCardIndex );
    virtual const std::string& GetEquipEnchantmentCard( const NFIDENTID& self, const int nOrigin );

    //���ô���ʱ��(����Ƿ����õ���)
    virtual bool SetEquipCreatTime( const NFIDENTID& self, const int nOrigin, const std::string& strTime );
    virtual const std::string& GetEquipCreatTime( const NFIDENTID& self, const int nOrigin );

    virtual bool SetGridData( const NFIDENTID& self, const int nRow, const int nCol, const NFIValueList& var, const PackTableType name = PackTableType::NormalPack );

	virtual int GetCanUsedCount(const NFIDENTID& self, const PackTableType name = PackTableType::NormalPack) const;

    //�õ�ĳ����Ʒ�ж��ٸ�
    virtual int QueryCount(const NFIDENTID& self, const std::string& strItemConfigID) const;

    //����N��ĳ����Ʒ
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
    bool can_normal_pack_item_swap( const NFIDENTID& self, NFIRecord* pOriginRecord, NFIRecord* pTargetRecord, const int origin, const int target ); // �ж��Ƿ���Խ�����ͨ������Ʒ(װ�������ж�)
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
