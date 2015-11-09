// -------------------------------------------------------------------------
//    @FileName      :    NFIActor.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFIActor
//
// -------------------------------------------------------------------------

#ifndef _NFI_ACTOR_H_
#define _NFI_ACTOR_H_

#include <map>
#include <string>

#include <Theron/Theron.h>

#include "NFILogicModule.h"
#include "NFIActorManager.h"
#include "NFComm/NFCore/NFCDataList.h"
#include "NFComm/NFCore/NFIComponent.h"

class NFIActorMessage
{
public:
    NFIActorMessage()
    {
        eType = EACTOR_UNKNOW;
        nSubMsgID = 0;
		nFormActor = 0;
    }

	enum EACTOR_MESSAGE_ID
	{
        EACTOR_UNKNOW,
		EACTOR_INIT,
		EACTOR_AFTER_INIT,
        EACTOR_CHECKCONFIG,
		EACTOR_EXCUTE,
		EACTOR_BEFORE_SHUT,
		EACTOR_SHUT,
		EACTOR_NET_MSG,
        EACTOR_TRANS_MSG,
		EACTOR_LOG_MSG,
		EACTOR_EVENT_MSG,
		EACTOR_RETURN_EVENT_MSG,

		EACTOR_GET_PROPERTY_MSG_INT = 100,
		EACTOR_GET_PROPERTY_MSG_FLOAT,
		EACTOR_GET_PROPERTY_MSG_DOUBLE,
		EACTOR_GET_PROPERTY_MSG_STRING,
		EACTOR_GET_PROPERTY_MSG_OBJECT,

		EACTOR_SET_PROPERTY_MSG_INT = 110,
		EACTOR_SET_PROPERTY_MSG_FLOAT,
		EACTOR_SET_PROPERTY_MSG_DOUBLE,
		EACTOR_SET_PROPERTY_MSG_STRING,
		EACTOR_SET_PROPERTY_MSG_OBJECT,

		EACTOR_GET_RECORD_MSG_INT = 120,
		EACTOR_GET_RECORD_MSG_FLOAT,
		EACTOR_GET_RECORD_MSG_DOUBLE,
		EACTOR_GET_RECORD_MSG_STRING,
		EACTOR_GET_RECORD_MSG_OBJECT,

		EACTOR_SET_RECORD_MSG_INT = 130,
		EACTOR_SET_RECORD_MSG_FLOAT,
		EACTOR_SET_RECORD_MSG_DOUBLE,
		EACTOR_SET_RECORD_MSG_STRING,
		EACTOR_SET_RECORD_MSG_OBJECT,
	};

    EACTOR_MESSAGE_ID eType;
	int nSubMsgID;
	int nFormActor;
	std::string data;
	////////////////////event/////////////////////////////////////////////////
	NFIDENTID self;
	//////////////////////////////////////////////////////////////////////////
	EVENT_ASYNC_PROCESS_END_FUNCTOR_PTR xEndFuncptr;
protected:
private:
};

class NFIActor : public Theron::Actor, public NFILogicModule
{
public:
	NFIActor(Theron::Framework &framework, NFIActorManager* pManager) : Theron::Actor(framework)
    {
        m_pActorManager = pManager;

        RegisterHandler(this, &NFIActor::Handler);
    }

	NFIActorManager* GetActorManager(){return m_pActorManager;}

	virtual void AddComponent(NF_SHARE_PTR<NFIComponent> pComponent) = 0;
	virtual bool AddEndFunc(EVENT_ASYNC_PROCESS_END_FUNCTOR_PTR functorPtr_end) = 0;
	virtual bool SendMsg(const Theron::Address address, const NFIActorMessage& message) = 0;

public:
	virtual NFIDENTID Self() = 0;
	virtual NF_SHARE_PTR<NFIActor> GetActor(const NFIDENTID& self) = 0;

	virtual bool SetPropertyInt(const NFIDENTID& self, const std::string& strPropertyName, const NFINT64 nValue) = 0;
	virtual bool SetPropertyFloat(const NFIDENTID& self, const std::string& strPropertyName, const double dwValue) = 0;
	virtual bool SetPropertyString(const NFIDENTID& self, const std::string& strPropertyName, const std::string& strValue) = 0;
	virtual bool SetPropertyObject(const NFIDENTID& self, const std::string& strPropertyName, const NFIDENTID& obj) = 0;

	virtual NFINT64 GetPropertyInt(const NFIDENTID& self, const std::string& strPropertyName) = 0;
	virtual double GetPropertyFloat(const NFIDENTID& self, const std::string& strPropertyName) = 0;
	virtual const std::string& GetPropertyString(const NFIDENTID& self, const std::string& strPropertyName) = 0;
	virtual NFIDENTID GetPropertyObject(const NFIDENTID& self, const std::string& strPropertyName) = 0;

	virtual bool SetRecordInt(const NFIDENTID& self, const std::string& strRecordName, const int nRow, const int nCol, const NFINT64 nValue) = 0;
	virtual bool SetRecordFloat(const NFIDENTID& self, const std::string& strRecordName, const int nRow, const int nCol, const double dwValue) = 0;
	virtual bool SetRecordString(const NFIDENTID& self, const std::string& strRecordName, const int nRow, const int nCol, const std::string& strValue) = 0;
	virtual bool SetRecordObject(const NFIDENTID& self, const std::string& strRecordName, const int nRow, const int nCol, const NFIDENTID& obj) = 0;

	virtual bool SetRecordInt(const NFIDENTID& self, const std::string& strRecordName, const int nRow, const std::string& strColTag, const NFINT64 value) = 0;
	virtual bool SetRecordFloat(const NFIDENTID& self, const std::string& strRecordName, const int nRow, const std::string& strColTag, const double value) = 0;
	virtual bool SetRecordString(const NFIDENTID& self, const std::string& strRecordName, const int nRow, const std::string& strColTag, const std::string& value) = 0;
	virtual bool SetRecordObject(const NFIDENTID& self, const std::string& strRecordName, const int nRow, const std::string& strColTag, const NFIDENTID& value) = 0;

	virtual NFINT64 GetRecordInt(const NFIDENTID& self, const std::string& strRecordName, const int nRow, const int nCol) = 0;
	virtual double GetRecordFloat(const NFIDENTID& self, const std::string& strRecordName, const int nRow, const int nCol) = 0;
	virtual const std::string& GetRecordString(const NFIDENTID& self, const std::string& strRecordName, const int nRow, const int nCol) = 0;
	virtual NFIDENTID GetRecordObject(const NFIDENTID& self, const std::string& strRecordName, const int nRow, const int nCol) = 0;

	virtual NFINT64 GetRecordInt(const NFIDENTID& self, const std::string& strRecordName, const int nRow, const std::string& strColTag) = 0;
	virtual double GetRecordFloat(const NFIDENTID& self, const std::string& strRecordName, const int nRow, const std::string& strColTag) = 0;
	virtual const std::string& GetRecordString(const NFIDENTID& self, const std::string& strRecordName, const int nRow, const std::string& strColTag) = 0;
	virtual NFIDENTID GetRecordObject(const NFIDENTID& self, const std::string& strRecordName, const int nRow, const std::string& strColTag) = 0;


protected:

	virtual void HandlerEx(const NFIActorMessage& message, const Theron::Address from){};
	virtual void HandlerSelf(const NFIActorMessage& message, const Theron::Address from){};

private:
    void Handler(const NFIActorMessage& message, const Theron::Address from)
    {
		
		//收到消息要处理逻辑
		if (message.eType == NFIActorMessage::EACTOR_EVENT_MSG)
		{
			HandlerSelf(message, from);

		}
		else
		{
			HandlerEx(message, from);
		}
	}

protected:
    NFIActorManager* m_pActorManager;

};

#endif