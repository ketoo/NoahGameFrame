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
#include "NFComm/NFPluginModule/NFIPackModule.h"
#include "NFComm/NFPluginModule/NFIElementInfoModule.h"
#include "NFComm/NFPluginModule/NFISceneProcessModule.h"
#include "NFComm/NFPluginModule/NFIPropertyModule.h"
#include "NFComm/NFPluginModule/NFILogModule.h"
#include "NFComm/NFPluginModule/NFIUUIDModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"

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
    virtual bool Execute();
    virtual bool AfterInit();

    virtual const std::string& GetPackName( const PackTableType name );

	/////////////����/////////////////////////////////////////////////////////////

	virtual const NFGUID& CreateEquip( const NFGUID& self, const std::string& strConfigName);
	virtual bool CreateItem( const NFGUID& self, const std::string& strConfigName, const int nCount );

	virtual bool DeleteEquip( const NFGUID& self, const NFGUID& id);
    virtual bool DeleteItem(const NFGUID& self, const std::string& strItemConfigID, const int nCount);
    virtual bool EnoughItem(const NFGUID& self, const std::string& strItemConfigID, const int nCount);

	/////////////Ӣ��װ���ͱ�������/////////////////////////////////////////////////////////////

	virtual bool DressEquipForHero( const NFGUID& self, const NFGUID& hero, const NFGUID& id);
	virtual bool TakeOffEquipForm( const NFGUID& self, const NFGUID& hero, const NFGUID& id);

	/////////////װ�����������/////////////////////////////////////////////////////////////

	virtual int SetEquipRandPropertyID( const NFGUID& self, const NFGUID& id, const std::string& strPropertyID); 
	virtual const std::string& GetEquipRandPropertyID( const NFGUID& self, const NFGUID& id);

	/////////////װ���Ĵ�/////////////////////////////////////////////////////////////

	virtual bool SetEquipHoleCount( const NFGUID& self, const NFGUID& id, const int nCount);
	virtual int GetEquipHoleCount( const NFGUID& self, const NFGUID& id);

	/////////////װ����Ƕ��ʯͷ/////////////////////////////////////////////////////////////

	virtual bool SetEquipInlayStoneID( const NFGUID& self, const NFGUID& id, NFrame::Player::BagEquipList eIndex, const std::string& strPropertyID);
	virtual const std::string& GetEquipInlayStoneID( const NFGUID& self, const NFGUID& id, NFrame::Player::BagEquipList eIndex);

	/////////////װ��ǿ���ĵȼ�/////////////////////////////////////////////////////////////

	virtual bool SetEquipIntensifyLevel( const NFGUID& self, const NFGUID& id, const int nLevel);
	virtual int GetEquipIntensifyLevel( const NFGUID& self, const NFGUID& id);

	/////////////װ��Ԫ�صĵȼ�/////////////////////////////////////////////////////////////

	virtual bool SetEquipElementLevel( const NFGUID& self, const NFGUID& id, NFrame::Player::BagEquipList eIndex, const int nLevel);
	virtual int GetEquipElementLevel( const NFGUID& self, const NFGUID& id, NFrame::Player::BagEquipList eIndex);

protected:

	int FindItemRowByConfig(const NFGUID& self, const std::string& strItemConfigID);
    int OnClassObjectEvent( const NFGUID& self, const std::string& strClassNames, const CLASS_OBJECT_EVENT eClassEvent, const NFIDataList& var );

    // ��ӵ������
    void AddDropItem(const NFGUID& self, const NFIDataList& var);
    int OnObjectBeKilled(const NFGUID& self, const int nEventID, const NFIDataList& var);

private:
    NFIKernelModule* m_pKernelModule;
    NFILogModule* m_pLogModule;
    NFIElementInfoModule* m_pElementInfoModule;
    NFISceneProcessModule* m_pSceneProcessModule;
    NFIPropertyModule* m_pPropertyModule;
    NFIUUIDModule* m_pUUIDModule;
};


#endif
