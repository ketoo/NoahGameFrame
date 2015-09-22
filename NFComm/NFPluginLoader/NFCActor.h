// -------------------------------------------------------------------------
//    @FileName      :    Actor.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2014-04-05
//    @Module           :    Actor
//
// -------------------------------------------------------------------------

#ifndef _NFC_ACTOR_H_
#define _NFC_ACTOR_H_

#include <map>
#include <string>
#include "NFComm/NFCore/NFSingleton.h"
#include "NFComm/NFPluginModule/NFPlatform.h"
#include "NFComm/NFPluginModule/NFIActor.h"
#include "NFComm/NFCore/NFIComponent.h"
#include "../NFCore/NFList.h"

class NFCActor
	: public NFIActor
{
public:
	NFCActor(Theron::Framework &framework, NFIActorManager* pManager) : NFIActor(framework, pManager)
	{

	}

	~NFCActor()
	{
		NF_SHARE_PTR<NFIComponent> pComponent;
		bool bRet = mxComponentList.First(pComponent);
		while (bRet)
		{
			pComponent->BeforeShut();
			pComponent->Shut();

			bRet = mxComponentList.Next(pComponent);
		}

		mxComponentList.ClearAll();
	}

	virtual void HandlerEx(const NFIActorMessage& message, const Theron::Address from);
	virtual void HandlerSelf(const NFIActorMessage& message, const Theron::Address from);
	virtual void AddComponent(NF_SHARE_PTR<NFIComponent> pComponent);
	virtual bool AddEndFunc(EVENT_ASYNC_PROCESS_END_FUNCTOR_PTR functorPtr_end);

	//////////////////////////////////////////////////////////////////////////
	virtual NFIDENTID Self();
	virtual NF_SHARE_PTR<NFIActor> GetActor(const NFIDENTID& self);

	virtual bool SetPropertyInt(const NFIDENTID& self, const std::string& strPropertyName, const NFINT64 nValue);
	virtual bool SetPropertyFloat(const NFIDENTID& self, const std::string& strPropertyName,  const float fValue);
	virtual bool SetPropertyDouble(const NFIDENTID& self, const std::string& strPropertyName, const double dwValue);
	virtual bool SetPropertyString(const NFIDENTID& self, const std::string& strPropertyName, const std::string& strValue);
	virtual bool SetPropertyObject(const NFIDENTID& self, const std::string& strPropertyName, const NFIDENTID& obj);

	virtual NFINT64 GetPropertyInt(const NFIDENTID& self, const std::string& strPropertyName);
	virtual float GetPropertyFloat(const NFIDENTID& self, const std::string& strPropertyName);
	virtual double GetPropertyDouble(const NFIDENTID& self, const std::string& strPropertyName);
	virtual const std::string& GetPropertyString(const NFIDENTID& self, const std::string& strPropertyName);
	virtual NFIDENTID GetPropertyObject(const NFIDENTID& self, const std::string& strPropertyName);

	virtual bool SetRecordInt(const NFIDENTID& self, const std::string& strRecordName, const int nRow, const int nCol, const NFINT64 nValue);
	virtual bool SetRecordFloat(const NFIDENTID& self, const std::string& strRecordName, const int nRow, const int nCol,  const float fValue);
	virtual bool SetRecordDouble(const NFIDENTID& self, const std::string& strRecordName, const int nRow, const int nCol, const double dwValue);
	virtual bool SetRecordString(const NFIDENTID& self, const std::string& strRecordName, const int nRow, const int nCol, const std::string& strValue);
	virtual bool SetRecordObject(const NFIDENTID& self, const std::string& strRecordName, const int nRow, const int nCol, const NFIDENTID& obj);

	virtual bool SetRecordInt(const NFIDENTID& self, const std::string& strRecordName, const int nRow, const std::string& strColTag, const NFINT64 value);
	virtual bool SetRecordFloat(const NFIDENTID& self, const std::string& strRecordName, const int nRow, const std::string& strColTag, const float value);
	virtual bool SetRecordDouble(const NFIDENTID& self, const std::string& strRecordName, const int nRow, const std::string& strColTag, const double value);
	virtual bool SetRecordString(const NFIDENTID& self, const std::string& strRecordName, const int nRow, const std::string& strColTag, const std::string& value);
	virtual bool SetRecordObject(const NFIDENTID& self, const std::string& strRecordName, const int nRow, const std::string& strColTag, const NFIDENTID& value);

	virtual NFINT64 GetRecordInt(const NFIDENTID& self, const std::string& strRecordName, const int nRow, const int nCol);
	virtual float GetRecordFloat(const NFIDENTID& self, const std::string& strRecordName, const int nRow, const int nCol);
	virtual double GetRecordDouble(const NFIDENTID& self, const std::string& strRecordName, const int nRow, const int nCol);
	virtual const std::string& GetRecordString(const NFIDENTID& self, const std::string& strRecordName, const int nRow, const int nCol);
	virtual NFIDENTID GetRecordObject(const NFIDENTID& self, const std::string& strRecordName, const int nRow, const int nCol);

	virtual NFINT64 GetRecordInt(const NFIDENTID& self, const std::string& strRecordName, const int nRow, const std::string& strColTag);
	virtual float GetRecordFloat(const NFIDENTID& self, const std::string& strRecordName, const int nRow, const std::string& strColTag);
	virtual double GetRecordDouble(const NFIDENTID& self, const std::string& strRecordName, const int nRow, const std::string& strColTag);
	virtual const std::string& GetRecordString(const NFIDENTID& self, const std::string& strRecordName, const int nRow, const std::string& strColTag);
	virtual NFIDENTID GetRecordObject(const NFIDENTID& self, const std::string& strRecordName, const int nRow, const std::string& strColTag);
	//////////////////////////////////////////////////////////////////////////


protected:
	NFList< NF_SHARE_PTR<NFIComponent> > mxComponentList;
	EVENT_ASYNC_PROCESS_END_FUNCTOR_PTR mxFunctorEndPtr;
};
#endif