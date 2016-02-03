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
#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"
#include "NFComm/NFMessageDefine/NFDefine.pb.h"

enum PackTableType
{
	NONE_PACK  = -1, // 普通背包
	BagItemPack  = 0, // 普通背包
	BagEquipPack  = 1, // 可视装备背包
	BagHeroPack = 2, // 英雄列表
	DropItemList = 3, // 副本掉落列表

	MaxPack,
};

class NFIPackModule : public NFILogicModule
{
public:
    virtual const std::string& GetPackName( const PackTableType name ) = 0;
    /////////////基础/////////////////////////////////////////////////////////////

	virtual const NFGUID& CreateEquip( const NFGUID& self, const std::string& strConfigName) = 0;
	virtual bool CreateItem( const NFGUID& self, const std::string& strConfigName, const int nCount ) = 0;

	virtual bool DeleteEquip( const NFGUID& self, const NFGUID& id) = 0;
	virtual bool DeleteItem(const NFGUID& self, const std::string& strItemConfigID, const int nCount) = 0;
    virtual bool EnoughItem(const NFGUID& self, const std::string& strItemConfigID, const int nCount) = 0;
	/////////////英雄装备和背包互换/////////////////////////////////////////////////////////////

	virtual bool DressEquipForHero( const NFGUID& self, const NFGUID& hero, const NFGUID& id) = 0;
	virtual bool TakeOffEquipForm( const NFGUID& self, const NFGUID& hero, const NFGUID& id) = 0;

	/////////////装备的随机属性/////////////////////////////////////////////////////////////

	virtual int SetEquipRandPropertyID( const NFGUID& self, const NFGUID& id, const std::string& strPropertyID) = 0; 
	virtual const std::string& GetEquipRandPropertyID( const NFGUID& self, const NFGUID& id) = 0;

	/////////////装备的打洞/////////////////////////////////////////////////////////////

	virtual bool SetEquipHoleCount( const NFGUID& self, const NFGUID& id, const int nCount) = 0;
	virtual int GetEquipHoleCount( const NFGUID& self, const NFGUID& id) = 0;

	/////////////装备镶嵌的石头/////////////////////////////////////////////////////////////

	virtual bool SetEquipInlayStoneID( const NFGUID& self, const NFGUID& id, NFrame::Player::BagEquipList eIndex, const std::string& strPropertyID) = 0;
	virtual const std::string& GetEquipInlayStoneID( const NFGUID& self, const NFGUID& id, NFrame::Player::BagEquipList eIndex) = 0;

	/////////////装备强化的等级/////////////////////////////////////////////////////////////

	virtual bool SetEquipIntensifyLevel( const NFGUID& self, const NFGUID& id, const int nLevel) = 0;
	virtual int GetEquipIntensifyLevel( const NFGUID& self, const NFGUID& id) = 0;

	/////////////装备元素的等级/////////////////////////////////////////////////////////////

	virtual bool SetEquipElementLevel( const NFGUID& self, const NFGUID& id, NFrame::Player::BagEquipList eIndex, const int nLevel) = 0;
	virtual int GetEquipElementLevel( const NFGUID& self, const NFGUID& id, NFrame::Player::BagEquipList eIndex) = 0;


};

#endif