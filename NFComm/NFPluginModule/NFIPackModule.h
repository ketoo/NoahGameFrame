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
#include "NFComm/NFMessageDefine/NFRecordDefine.pb.h"
#include "NFComm/NFMessageDefine/NFDefine.pb.h"

enum PackTableType
{
	NONE_PACK  = -1, // ��ͨ����
	BagItemPack  = 0, // ��ͨ����
	BagEquipPack  = 1, // ����װ������
	BagHeroPack = 2, // Ӣ���б�
	DropItemList = 3, // ���������б�

	MaxPack,
};

class NFIPackModule : public NFILogicModule
{
public:
	/////////////����/////////////////////////////////////////////////////////////

	virtual const NFGUID& CreateEquip( const NFGUID& self, const std::string& strConfigName) = 0;
	virtual bool CreateItem( const NFGUID& self, const std::string& strConfigName, const int nCount ) = 0;

	virtual bool DeleteEquip( const NFGUID& self, const NFGUID& id) = 0;
	virtual bool DeleteItem(const NFGUID& self, const std::string& strItemConfigID, const int nCount) = 0;
	/////////////Ӣ��װ���ͱ�������/////////////////////////////////////////////////////////////

	virtual bool DressEquipForHero( const NFGUID& self, const NFGUID& hero, const NFGUID& id) = 0;
	virtual bool TakeOffEquipForm( const NFGUID& self, const NFGUID& hero, const NFGUID& id) = 0;

	/////////////װ�����������/////////////////////////////////////////////////////////////

	virtual int SetEquipRandPropertyID( const NFGUID& self, const NFGUID& id, const std::string& strPropertyID) = 0; 
	virtual const std::string& GetEquipRandPropertyID( const NFGUID& self, const NFGUID& id) = 0;

	/////////////װ���Ĵ�/////////////////////////////////////////////////////////////

	virtual bool SetEquipHoleCount( const NFGUID& self, const NFGUID& id, const int nCount) = 0;
	virtual int GetEquipHoleCount( const NFGUID& self, const NFGUID& id) = 0;

	/////////////װ����Ƕ��ʯͷ/////////////////////////////////////////////////////////////

	virtual bool SetEquipInlayStoneID( const NFGUID& self, const NFGUID& id, NFMsg::BagEquipList_RecordColType eIndex, const std::string& strPropertyID) = 0;
	virtual const std::string& GetEquipInlayStoneID( const NFGUID& self, const NFGUID& id, NFMsg::BagEquipList_RecordColType eIndex) = 0;

	/////////////װ��ǿ���ĵȼ�/////////////////////////////////////////////////////////////

	virtual bool SetEquipIntensifyLevel( const NFGUID& self, const NFGUID& id, const int nLevel) = 0;
	virtual int GetEquipIntensifyLevel( const NFGUID& self, const NFGUID& id) = 0;

	/////////////װ��Ԫ�صĵȼ�/////////////////////////////////////////////////////////////

	virtual bool SetEquipElementLevel( const NFGUID& self, const NFGUID& id, NFMsg::BagEquipList_RecordColType eIndex, const int nLevel) = 0;
	virtual int GetEquipElementLevel( const NFGUID& self, const NFGUID& id, NFMsg::BagEquipList_RecordColType eIndex) = 0;


};

#endif