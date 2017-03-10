// -------------------------------------------------------------------------
//    @FileName			:    NFCSceneAOIModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2016-12-05
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

	virtual bool RequestEnterScene(const NFGUID& self, const int nSceneID, const int nGrupID, const int nType, const NFDataList& argList);
	virtual bool AddSeedData(const int nSceneID, const std::string& strSeedID, const std::string& strConfigID, const NFVector3& vPos, const int nWeight);
	virtual bool AddRelivePosition(const int nSceneID, const int nIndex, const NFVector3& vPos);
	virtual NFVector3 GetRelivePosition(const int nSceneID, const int nIndex);

	virtual bool CreateSceneNPC(const int nSceneID, const int nGroupID);
	virtual bool DestroySceneNPC(const int nSceneID, const int nGroupID);

	virtual bool RemoveSwapSceneEventCallBack();

protected:
	virtual bool AddObjectEnterCallBack(const OBJECT_ENTER_EVENT_FUNCTOR_PTR& cb);
	virtual bool AddObjectLeaveCallBack(const OBJECT_LEAVE_EVENT_FUNCTOR_PTR& cb);
	virtual bool AddPropertyEnterCallBack(const PROPERTY_ENTER_EVENT_FUNCTOR_PTR& cb);
	virtual bool AddRecordEnterCallBack(const RECORD_ENTER_EVENT_FUNCTOR_PTR& cb);

	virtual bool AddPropertyEventCallBack(const PROPERTY_SINGLE_EVENT_FUNCTOR_PTR& cb);
	virtual bool AddRecordEventCallBack(const RECORD_SINGLE_EVENT_FUNCTOR_PTR& cb);

	virtual bool AddEnterSceneConditionCallBack(const SCENE_EVENT_FUNCTOR_PTR& cb);

	virtual bool AddBeforeEnterSceneGroupCallBack(const SCENE_EVENT_FUNCTOR_PTR& cb);
	virtual bool AddAfterEnterSceneGroupCallBack(const SCENE_EVENT_FUNCTOR_PTR& cb);
	virtual bool AddSwapSceneEventCallBack(const SCENE_EVENT_FUNCTOR_PTR& cb);
	virtual bool AddBeforeLeaveSceneGroupCallBack(const SCENE_EVENT_FUNCTOR_PTR& cb);
	virtual bool AddAfterLeaveSceneGroupCallBack(const SCENE_EVENT_FUNCTOR_PTR& cb);

protected:
	bool SwitchScene(const NFGUID& self, const int nTargetSceneID, const int nTargetGroupID, const int nType, const float fX, const float fY, const float fZ, const float fOrient, const NFDataList& arg);

protected:
	int OnPropertyCommonEvent(const NFGUID& self, const std::string& strPropertyName, const NFData& oldVar, const NFData& newVar);
	int OnRecordCommonEvent(const NFGUID& self, const RECORD_EVENT_DATA& xEventData, const NFData& oldVar, const NFData& newVar);
	int OnClassCommonEvent(const NFGUID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFDataList& var);

	int OnPlayerGroupEvent(const NFGUID& self, const std::string& strPropertyName, const NFData& oldVar, const NFData& newVar);
	int OnPlayerSceneEvent(const NFGUID& self, const std::string& strPropertyName, const NFData& oldVar, const NFData& newVar);
	
	int GetBroadCastObject(const NFGUID& self, const std::string& strPropertyName, const bool bTable, NFDataList& valueObject);

	int EnterSceneCondition(const NFGUID& self, const int nSceneID, const int nGroupID, const int nType, const NFDataList& argList);

	int BeforeLeaveSceneGroup(const NFGUID& self, const int nSceneID, const int nGroupID, const int nType, const NFDataList& argList);
	int AfterLeaveSceneGroup(const NFGUID& self, const int nSceneID, const int nGroupID, const int nType, const NFDataList& argList);
	int OnSwapSceneEvent(const NFGUID& self, const int nSceneID, const int nGroupID, const int nType, const NFDataList& argList);
	int BeforeEnterSceneGroup(const NFGUID& self, const int nSceneID, const int nGroupID, const int nType, const NFDataList& argList);
	int AfterEnterSceneGroup(const NFGUID& self, const int nSceneID, const int nGroupID, const int nType, const NFDataList& argList);

protected:
	////////////////interface for broadcast event//////////////////////////////////
	//broad the data of argvar to self
	int OnObjectListEnter(const NFDataList& self, const NFDataList& argVar);
	int OnObjectListLeave(const NFDataList& self, const NFDataList& argVar);

	//broad the data of self to argvar 
	int OnPropertyEnter(const NFDataList& argVar, const NFGUID& self);
	int OnRecordEnter(const NFDataList& argVar, const NFGUID& self);

	int OnPropertyEvent(const NFGUID& self, const std::string& strProperty, const NFData& oldVar, const NFData& newVar, const NFDataList& argVar);
	int OnRecordEvent(const NFGUID& self, const std::string& strRecord, const RECORD_EVENT_DATA& xEventData, const NFData& oldVar, const NFData& newVar, const NFDataList& argVar);
	////////////////interface for broadcast event///////////////////////////////////

private:

	std::vector<OBJECT_ENTER_EVENT_FUNCTOR_PTR> mtObjectEnterCallback;
	std::vector<OBJECT_LEAVE_EVENT_FUNCTOR_PTR> mtObjectLeaveCallback;

	std::vector<PROPERTY_ENTER_EVENT_FUNCTOR_PTR> mtPropertyEnterCallback;
	std::vector<RECORD_ENTER_EVENT_FUNCTOR_PTR> mtRecordEnterCallback;

	std::vector<PROPERTY_SINGLE_EVENT_FUNCTOR_PTR> mtPropertySingleCallback;
	std::vector<RECORD_SINGLE_EVENT_FUNCTOR_PTR> mtRecordSingleCallback;

	std::vector<SCENE_EVENT_FUNCTOR_PTR> mtEnterSceneConditionCallback;
	std::vector<SCENE_EVENT_FUNCTOR_PTR> mtBeforeEnterSceneCallback;
	std::vector<SCENE_EVENT_FUNCTOR_PTR> mtAfterEnterSceneCallback;
	std::vector<SCENE_EVENT_FUNCTOR_PTR> mtOnSwapSceneCallback;
	std::vector<SCENE_EVENT_FUNCTOR_PTR> mtBeforeLeaveSceneCallback;
	std::vector<SCENE_EVENT_FUNCTOR_PTR> mtAfterLeaveSceneCallback;
private:
	NFIKernelModule* m_pKernelModule;
	NFIClassModule* m_pClassModule;
	NFILogModule* m_pLogModule;
	NFIElementModule* m_pElementModule;
	NFIEventModule* m_pEventModule;
};

#endif