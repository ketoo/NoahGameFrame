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
	NormalPack  = 0, // ��ͨ����
    ViewPack    = 1, // ����װ������
    DropItemList = 3, // ���������б�

	MaxPack,
};

class NFIPackModule : public NFILogicModule
{
public:
	//���װ��:װ��config
	virtual const NFGUID& CreateEquip( const NFGUID& self, const std::string& strConfigName) = 0;

	//�����ͨ����
	virtual bool CreateItem( const NFGUID& self, const std::string& strConfigName, const int nCount ) = 0;

	//ɾ��ĳ��������Ʒ
	virtual bool DeleteGrid( const NFGUID& self, const int nOrigin) = 0;

	//////////////////////////////////////////////////////////////////////////
	//�õ�configID
	virtual const std::string& GetGridConfigID( const NFGUID& self, const int nRow) = 0;

	//���õ�������
	virtual bool SetGridCount( const NFGUID& self, const int nOrigin, const int nCount) = 0;
	virtual NFINT64 GetGridCount( const NFGUID& self, const int nOrigin) = 0;

	//�Ƿ��
	virtual bool SetGridBan( const NFGUID& self, const int nOrigin, const bool bBan ) = 0;
	virtual bool GetGridBan( const NFGUID& self, const int nOrigin ) = 0;

	//���ô���ʱ��(����Ƿ����õ���)
	virtual bool SetEquipCreatTime( const NFGUID& self, const int nOrigin, const const NFINT64 nTime  ) = 0;
	virtual const NFINT64 GetEquipCreatTime( const NFGUID& self, const int nOrigin ) = 0;

	//�õ�ĳ����Ʒ�ж��ٸ�
	virtual int QueryCount(const NFGUID& self, const std::string& strItemConfigID) const = 0;

	//����N��ĳ����Ʒ
	virtual bool DeleteItem(const NFGUID& self, const std::string& strItemConfigID, const int nCount) = 0;
};

#endif