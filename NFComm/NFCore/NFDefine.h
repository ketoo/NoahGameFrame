// -------------------------------------------------------------------------
//    @FileName         :    NFDefine.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-03-01
//    @Module           :    NFDefine
//
// -------------------------------------------------------------------------

#ifndef _NF_DEFINE_H_
#define _NF_DEFINE_H_

#include "NFIDENTID.h"
#include "NFIValueList.h"

#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/bind/placeholders.hpp>
#include <boost/bind.hpp>


enum CLASS_OBJECT_EVENT
{
    COE_DESTROY = 0,
    COE_BEFOREDESTROY,
    COE_CREATE_NODATA,
    COE_CREATE_LOADDATA,
    COE_CREATE_BEFORE_EFFECT,
    COE_CREATE_EFFECTDATA,
    COE_CREATE_HASDATA,
    COE_CREATE_FINISH,  // ToModify任务创建完成后再挂回调
};

////heart beat call back event func define
//typedef int(* HEART_BEAT_FUNC)(const NFIDENTID& self, const NFIValueList& var);
//
////property call back event func define
//typedef int(* PROPERTY_EVENT_FUNC)(const NFIDENTID& self, const std::string& strPropertyName, const NFIValueList& oldVar, const NFIValueList& newVar, const NFIValueList& argVar);
//
////record call back event func define, nOpType: Add = 0,Del, Changed,    Create, UpData,
//typedef int(* RECORD_EVENT_FUNC)(const NFIDENTID& self, const std::string& strRecordName, const int nOpType, const int nRow, const int nCol, const NFIValueList& oldVar, const NFIValueList& newVar, const NFIValueList& argVar);
////AddEventCallBack( "Player", CEF_OnDisconnect, SceneModule::OnDisconnect );
//
////class call back event func define. nClassEvent:0[destroy],1[created],2[enterContainer]
//typedef int(* CLASS_EVENT_FUNC)(const NFIDENTID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFIValueList& var);
////AddEventCallBack( "Player", CEF_OnDisconnect, SceneModule::OnDisconnect );
//
////event call back event func define
//typedef int(* EVENT_PROCESS_FUNC)(const NFIDENTID& self, const int nEventID, const NFIValueList& var);

// functor
typedef boost::function<int(const NFIDENTID&, const NFIValueList&)> HEART_BEAT_FUNCTOR;
typedef boost::function<int(const NFIDENTID&, const std::string&, const NFIValueList&, const NFIValueList&, const NFIValueList&)> PROPERTY_EVENT_FUNCTOR;
typedef boost::function<int(const NFIDENTID&, const std::string&, const int, const int, const int, const NFIValueList&, const NFIValueList&, const NFIValueList&)> RECORD_EVENT_FUNCTOR;
typedef boost::function<int(const NFIDENTID&, const std::string&, const CLASS_OBJECT_EVENT, const NFIValueList&)> CLASS_EVENT_FUNCTOR;
typedef boost::function<int(const NFIDENTID&, const int, const NFIValueList&)> EVENT_PROCESS_FUNCTOR;

typedef std::shared_ptr<HEART_BEAT_FUNCTOR> HEART_BEAT_FUNCTOR_PTR;
typedef std::shared_ptr<PROPERTY_EVENT_FUNCTOR> PROPERTY_EVENT_FUNCTOR_PTR;
typedef std::shared_ptr<RECORD_EVENT_FUNCTOR> RECORD_EVENT_FUNCTOR_PTR;
typedef std::shared_ptr<CLASS_EVENT_FUNCTOR> CLASS_EVENT_FUNCTOR_PTR;
typedef std::shared_ptr<EVENT_PROCESS_FUNCTOR> EVENT_PROCESS_FUNCTOR_PTR;


#endif
