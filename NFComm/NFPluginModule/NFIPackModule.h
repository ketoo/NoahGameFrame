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
#include "NFComm/NFMessageDefine/NFRecordDefine.pb.h"
#include "NFComm/NFMessageDefine/NFDefine.pb.h"

enum PackTableType
{
	NormalPack  = 0, // 普通背包
    ViewPack    = 1, // 可视装备背包
    DropItemList = 3, // 副本掉落列表

	MaxPack,
};

class NFIPackModule : public NFILogicModule
{
public:
	//添加装备:装备config
	virtual const NFGUID& CreateEquip( const NFGUID& self, const std::string& strConfigName) = 0;

	//添加普通道具
	virtual bool CreateItem( const NFGUID& self, const std::string& strConfigName, const int nCount ) = 0;

	//删除某格子内物品
	virtual bool DeleteGrid( const NFGUID& self, const int nOrigin) = 0;

	//////////////////////////////////////////////////////////////////////////
	//得到configID
	virtual const std::string& GetGridConfigID( const NFGUID& self, const int nRow) = 0;

	//设置道具数量
	virtual bool SetGridCount( const NFGUID& self, const int nOrigin, const int nCount) = 0;
	virtual NFINT64 GetGridCount( const NFGUID& self, const int nOrigin) = 0;

	//是否绑定
	virtual bool SetGridBan( const NFGUID& self, const int nOrigin, const bool bBan ) = 0;
	virtual bool GetGridBan( const NFGUID& self, const int nOrigin ) = 0;

	//设置创建时间(如果是非永久道具)
	virtual bool SetEquipCreatTime( const NFGUID& self, const int nOrigin, const const NFINT64 nTime  ) = 0;
	virtual const NFINT64 GetEquipCreatTime( const NFGUID& self, const int nOrigin ) = 0;

	//得到某样物品有多少个
	virtual int QueryCount(const NFGUID& self, const std::string& strItemConfigID) const = 0;

	//消费N个某样物品
	virtual bool DeleteItem(const NFGUID& self, const std::string& strItemConfigID, const int nCount) = 0;
};

#endif