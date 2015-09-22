// -------------------------------------------------------------------------
//    @FileName      :    NFCActor.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2014-04-05
//    @Module           :    NFCActor
//
// -------------------------------------------------------------------------

#include "NFCActor.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"


void NFCActor::HandlerEx( const NFIActorMessage& message, const Theron::Address from )
{
	//添加到主线程事件
	m_pActorManager->GetPluginManager()->HandlerEx(message, from);
}

void NFCActor::AddComponent( NF_SHARE_PTR<NFIComponent> pComponent )
{
	if (nullptr != pComponent)
	{
		mxComponentList.Add(pComponent);

		pComponent->Init();
		pComponent->AfterInit();
	}
}

void NFCActor::HandlerSelf( const NFIActorMessage& message, const Theron::Address from )
{
	std::string strData = message.data;

	NF_SHARE_PTR<NFIComponent> pComponent;
	bool bRet = mxComponentList.First(pComponent);
	while (bRet)
	{
		pComponent->OnASyncEvent(message.self, message.nSubMsgID, strData);

		bRet = mxComponentList.Next(pComponent);
	}
	//m_pComponent->OnASyncEvent(message.self, message.nSubMsgID, strData);

	//message.xActorEventFunc->xBeginFuncptr->operator()(message.self, message.nSubMsgID, strData);

	////////////////////////////////////////////////////////

	NFIActorMessage xReturnMessage;

	xReturnMessage.eType = NFIActorMessage::EACTOR_RETURN_EVENT_MSG;
	xReturnMessage.nSubMsgID = message.nSubMsgID;
	xReturnMessage.data = strData;
	////////////////////event/////////////////////////////////////////////////
	xReturnMessage.self = message.self;
	xReturnMessage.nFormActor = this->GetAddress().AsInteger();
	xReturnMessage.xEndFuncptr = mxFunctorEndPtr;

	Send(xReturnMessage, from);
}

bool NFCActor::AddEndFunc( EVENT_ASYNC_PROCESS_END_FUNCTOR_PTR functorPtr_end )
{
	mxFunctorEndPtr = functorPtr_end;
	return true;
}

bool NFCActor::SetPropertyInt( const NFIDENTID& self, const std::string& strPropertyName, const NFINT64 nValue )
{
	if (Self() == self)
	{

	}
	else
	{

	}

	return false;
}

bool NFCActor::SetPropertyFloat( const NFIDENTID& self, const std::string& strPropertyName, const float fValue )
{
	if (Self() == self)
	{

	}
	else
	{

	}

	return false;
}

bool NFCActor::SetPropertyDouble( const NFIDENTID& self, const std::string& strPropertyName, const double dwValue )
{
	if (Self() == self)
	{

	}
	else
	{

	}

	return false;
}

bool NFCActor::SetPropertyString( const NFIDENTID& self, const std::string& strPropertyName, const std::string& strValue )
{
	if (Self() == self)
	{

	}
	else
	{

	}

	return false;
}

bool NFCActor::SetPropertyObject( const NFIDENTID& self, const std::string& strPropertyName, const NFIDENTID& obj )
{
	if (Self() == self)
	{

	}
	else
	{

	}

	return false;
}

NFINT64 NFCActor::GetPropertyInt( const NFIDENTID& self, const std::string& strPropertyName )
{
	if (Self() == self)
	{

	}
	else
	{

	}

	return 0;
}

float NFCActor::GetPropertyFloat( const NFIDENTID& self, const std::string& strPropertyName )
{
	if (Self() == self)
	{

	}
	else
	{

	}

	return 0.0f;
}

double NFCActor::GetPropertyDouble( const NFIDENTID& self, const std::string& strPropertyName )
{
	if (Self() == self)
	{

	}
	else
	{

	}

	return 0.0f;
}

const std::string& NFCActor::GetPropertyString( const NFIDENTID& self, const std::string& strPropertyName )
{
	if (Self() == self)
	{

	}
	else
	{

	}

	return NULL_STR;
}

NFIDENTID NFCActor::GetPropertyObject( const NFIDENTID& self, const std::string& strPropertyName )
{
	if (Self() == self)
	{

	}
	else
	{

	}

	return NFIDENTID();
}

bool NFCActor::SetRecordInt( const NFIDENTID& self, const std::string& strRecordName, const int nRow, const int nCol, const NFINT64 nValue )
{
	if (Self() == self)
	{

	}
	else
	{

	}

	return false;
}

bool NFCActor::SetRecordInt( const NFIDENTID& self, const std::string& strRecordName, const int nRow, const std::string& strColTag, const NFINT64 value )
{
	if (Self() == self)
	{

	}
	else
	{

	}

	return false;
}

bool NFCActor::SetRecordFloat( const NFIDENTID& self, const std::string& strRecordName, const int nRow, const int nCol, const float fValue )
{
	if (Self() == self)
	{

	}
	else
	{

	}

	return false;
}

bool NFCActor::SetRecordFloat( const NFIDENTID& self, const std::string& strRecordName, const int nRow, const std::string& strColTag, const float value )
{
	if (Self() == self)
	{

	}
	else
	{

	}

	return false;
}

bool NFCActor::SetRecordDouble( const NFIDENTID& self, const std::string& strRecordName, const int nRow, const int nCol, const double dwValue )
{
	if (Self() == self)
	{

	}
	else
	{

	}

	return false;
}

bool NFCActor::SetRecordDouble( const NFIDENTID& self, const std::string& strRecordName, const int nRow, const std::string& strColTag, const double value )
{
	if (Self() == self)
	{

	}
	else
	{

	}

	return false;
}

bool NFCActor::SetRecordString( const NFIDENTID& self, const std::string& strRecordName, const int nRow, const int nCol, const std::string& strValue )
{
	if (Self() == self)
	{

	}
	else
	{

	}

	return false;
}

bool NFCActor::SetRecordString( const NFIDENTID& self, const std::string& strRecordName, const int nRow, const std::string& strColTag, const std::string& value )
{
	if (Self() == self)
	{

	}
	else
	{

	}

	return false;
}

bool NFCActor::SetRecordObject( const NFIDENTID& self, const std::string& strRecordName, const int nRow, const int nCol, const NFIDENTID& obj )
{
	if (Self() == self)
	{

	}
	else
	{

	}

	return false;
}

bool NFCActor::SetRecordObject( const NFIDENTID& self, const std::string& strRecordName, const int nRow, const std::string& strColTag, const NFIDENTID& value )
{
	if (Self() == self)
	{

	}
	else
	{

	}

	return false;
}

NFINT64 NFCActor::GetRecordInt( const NFIDENTID& self, const std::string& strRecordName, const int nRow, const int nCol )
{
	if (Self() == self)
	{

	}
	else
	{

	}

	return 0;
}

NFINT64 NFCActor::GetRecordInt( const NFIDENTID& self, const std::string& strRecordName, const int nRow, const std::string& strColTag )
{
	if (Self() == self)
	{

	}
	else
	{

	}

	return 0;
}

float NFCActor::GetRecordFloat( const NFIDENTID& self, const std::string& strRecordName, const int nRow, const int nCol )
{
	if (Self() == self)
	{

	}
	else
	{

	}

	return 0.0f;
}

float NFCActor::GetRecordFloat( const NFIDENTID& self, const std::string& strRecordName, const int nRow, const std::string& strColTag )
{
	if (Self() == self)
	{

	}
	else
	{

	}

	return 0.0f;
}

double NFCActor::GetRecordDouble( const NFIDENTID& self, const std::string& strRecordName, const int nRow, const int nCol )
{
	if (Self() == self)
	{

	}
	else
	{

	}

	return 0.0f;
}

double NFCActor::GetRecordDouble( const NFIDENTID& self, const std::string& strRecordName, const int nRow, const std::string& strColTag )
{
	if (Self() == self)
	{

	}
	else
	{

	}

	return 0.0f;
}

const std::string& NFCActor::GetRecordString( const NFIDENTID& self, const std::string& strRecordName, const int nRow, const int nCol )
{
	if (Self() == self)
	{

	}
	else
	{

	}

	return NULL_STR;
}

const std::string& NFCActor::GetRecordString( const NFIDENTID& self, const std::string& strRecordName, const int nRow, const std::string& strColTag )
{
	if (Self() == self)
	{

	}
	else
	{

	}

	return NULL_STR;
}

NFIDENTID NFCActor::GetRecordObject( const NFIDENTID& self, const std::string& strRecordName, const int nRow, const int nCol )
{
	if (Self() == self)
	{

	}
	else
	{

	}

	return NFIDENTID();
}

NFIDENTID NFCActor::GetRecordObject( const NFIDENTID& self, const std::string& strRecordName, const int nRow, const std::string& strColTag )
{
	if (Self() == self)
	{

	}
	else
	{
		NF_SHARE_PTR<NFIActor> xActor = GetActor(self);
		if (nullptr != xActor)
		{
			NFIActorMessage xMessage;

			xMessage.eType = NFIActorMessage::EACTOR_SET_RECORD_MSG_OBJECT;
			xMessage.nSubMsgID = nRow;
			xMessage.data = strRecordName;

			////////////////////event/////////////////////////////////////////////////

			xMessage.self = self;
			xMessage.nFormActor = this->GetAddress().AsInteger();
			//xMessage.xEndFuncptr = mxFunctorEndPtr;

			Send(xMessage, xActor->GetAddress());
		}

	}

	return NFIDENTID();
}

NFIDENTID NFCActor::Self()
{
	return NFIDENTID();
}

NF_SHARE_PTR<NFIActor> NFCActor::GetActor( const NFIDENTID& self )
{
	if (Self() == self)
	{
		return NF_SHARE_PTR<NFIActor>(this);
	}
	else
	{
		return NF_SHARE_PTR<NFIActor>(this);
	}

	return NF_SHARE_PTR<NFIActor>(nullptr);
}
