// -------------------------------------------------------------------------
//    @FileName			:    NFCSceneAOIModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFCSceneAOIModule
//
// -------------------------------------------------------------------------

#ifndef NFC_SCENE_AOI__MODULE_H
#define NFC_SCENE_AOI__MODULE_H

#include <iostream>
#include "NFComm/NFCore/NFIObject.h"
#include "NFComm/NFPluginModule/NFGUID.h"
#include "NFComm/NFPluginModule/NFISceneAOIModule.h"
#include "NFComm/NFPluginModule/NFILogModule.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIClassModule.h"
#include "NFComm/NFPluginModule/NFIElementModule.h"
#include "NFComm/NFPluginModule/NFIEventModule.h"

class NFCSceneAOIModule
    : public NFISceneAOIModule
{
public:
	NFCSceneAOIModule(NFIPluginManager* p)
    {
        pPluginManager = p;
    }


    virtual ~NFCSceneAOIModule()
    {
    }

    virtual bool Init();
    virtual bool AfterInit();
    virtual bool BeforeShut();
    virtual bool Shut();
    virtual bool Execute();

protected:
	virtual bool AddObjectEnterCallBack(const OBJECT_ENTER_EVENT_FUNCTOR_PTR& cb);
	virtual bool AddObjectLeaveCallBack(const OBJECT_LEAVE_EVENT_FUNCTOR_PTR& cb);
	virtual bool AddPropertyEnterCallBack(const PROPERTY_ENTER_EVENT_FUNCTOR_PTR& cb);
	virtual bool AddRecordEnterCallBack(const RECORD_ENTER_EVENT_FUNCTOR_PTR& cb);
	virtual bool AddPropertyEventCallBack(const PROPERTY_SINGLE_EVENT_FUNCTOR_PTR& cb);
	virtual bool AddRecordEventCallBack(const RECORD_SINGLE_EVENT_FUNCTOR_PTR& cb);

protected:
	int OnPropertyCommonEvent(const NFGUID& self, const std::string& strPropertyName, const NFIDataList::TData& oldVar, const NFIDataList::TData& newVar);
	int OnRecordCommonEvent(const NFGUID& self, const RECORD_EVENT_DATA& xEventData, const NFIDataList::TData& oldVar, const NFIDataList::TData& newVar);
	int OnClassCommonEvent(const NFGUID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFIDataList& var);

	int OnGroupEvent(const NFGUID& self, const std::string& strPropertyName, const NFIDataList::TData& oldVar, const NFIDataList::TData& newVar);
	int OnSceneEvent(const NFGUID& self, const std::string& strPropertyName, const NFIDataList::TData& oldVar, const NFIDataList::TData& newVar);
	
	int GetBroadCastObject(const NFGUID& self, const std::string& strPropertyName, const bool bTable, NFIDataList& valueObject);
	int GetBroadCastObject(const int nObjectContainerID, const int nGroupID, NFIDataList& valueObject);


protected:
	////////////////interface for broadcast event//////////////////////////////////
	//broad the data of self to argvar
	int OnObjectListEnter(const NFIDataList& self, const NFIDataList& argVar);
	int OnObjectListLeave(const NFIDataList& self, const NFIDataList& argVar);

	//broad the data of argvar to self
	int OnPropertyEnter(const NFIDataList& argVar, const NFGUID& self);
	int OnRecordEnter(const NFIDataList& argVar, const NFGUID& self);

	int OnPropertyEvent(const NFGUID& self, const std::string& strProperty, const NFIDataList::TData& oldVar, const NFIDataList::TData& newVar, const NFIDataList& argVar);
	int OnRecordEvent(const NFGUID& self, const RECORD_EVENT_DATA& xEventData, const NFIDataList::TData& oldVar, const NFIDataList::TData& newVar, const NFIDataList& argVar);
	////////////////interface for broadcast event///////////////////////////////////

private:

	std::vector<OBJECT_ENTER_EVENT_FUNCTOR_PTR> mtObjectEnterCallback;
	std::vector<OBJECT_LEAVE_EVENT_FUNCTOR_PTR> mtObjectLeaveCallback;
	std::vector<PROPERTY_ENTER_EVENT_FUNCTOR_PTR> mtPropertyEnterCallback;
	std::vector<RECORD_ENTER_EVENT_FUNCTOR_PTR> mtRecordEnterCallback;
	std::vector<PROPERTY_SINGLE_EVENT_FUNCTOR_PTR> mtPropertySingleCallback;
	std::vector<RECORD_SINGLE_EVENT_FUNCTOR_PTR> mtRecordSingleCallback;

private:
	NFIKernelModule* m_pKernelModule;
	NFIClassModule* m_pClassModule;
	NFILogModule* m_pLogModule;
	NFIElementModule* m_pElementModule;
	NFIEventModule* m_pEventModule;
};

#endif