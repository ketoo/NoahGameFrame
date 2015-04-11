// -------------------------------------------------------------------------
//    @FileName         :    NFDefine.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-03-01
//    @Module           :    NFDefine
//
// -------------------------------------------------------------------------

#ifndef _NF_DEFINE_H_
#define _NF_DEFINE_H_

#include "NFIDataList.h"
#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/bind/placeholders.hpp>
#include <boost/bind.hpp>
#include "NFComm/NFPluginModule/NFIdentID.h"

enum CLASS_OBJECT_EVENT
{
    COE_DESTROY = 0,
    COE_BEFOREDESTROY,
    COE_CREATE_NODATA,
    COE_CREATE_LOADDATA,
    COE_CREATE_BEFORE_EFFECT,
    COE_CREATE_EFFECTDATA,
    COE_CREATE_AFTER_EFFECT,
    COE_CREATE_HASDATA,
    COE_CREATE_FINISH,  // ToModify任务创建完成后再挂回调
};

// functor
typedef boost::function<int(const NFIDENTID&, const std::string&, const float, const int, const NFIDataList&)> HEART_BEAT_FUNCTOR;
typedef boost::function<int(const NFIDENTID&, const std::string&, const NFIDataList&, const NFIDataList&, const NFIDataList&)> PROPERTY_EVENT_FUNCTOR;
typedef boost::function<int(const NFIDENTID&, const std::string&, const int, const int, const int, const NFIDataList&, const NFIDataList&, const NFIDataList&)> RECORD_EVENT_FUNCTOR;
typedef boost::function<int(const NFIDENTID&, const std::string&, const CLASS_OBJECT_EVENT, const NFIDataList&)> CLASS_EVENT_FUNCTOR;
typedef boost::function<int(const NFIDENTID&, const int, const NFIDataList&)> EVENT_PROCESS_FUNCTOR;
typedef boost::function<int(const NFIDENTID&, const std::string&, const CLASS_OBJECT_EVENT, const std::string&)> CLASS_ASYNC_EVENT_FUNCTOR;
typedef boost::function<int(const NFIDENTID&, const int, const std::string&)> EVENT_ASYNC_PROCESS_FUNCTOR;

typedef NF_SHARE_PTR<HEART_BEAT_FUNCTOR> HEART_BEAT_FUNCTOR_PTR;
typedef NF_SHARE_PTR<PROPERTY_EVENT_FUNCTOR> PROPERTY_EVENT_FUNCTOR_PTR;
typedef NF_SHARE_PTR<RECORD_EVENT_FUNCTOR> RECORD_EVENT_FUNCTOR_PTR;
typedef NF_SHARE_PTR<CLASS_EVENT_FUNCTOR> CLASS_EVENT_FUNCTOR_PTR;
typedef NF_SHARE_PTR<EVENT_PROCESS_FUNCTOR> EVENT_PROCESS_FUNCTOR_PTR;

typedef NF_SHARE_PTR<CLASS_ASYNC_EVENT_FUNCTOR> CLASS_ASYNC_EVENT_FUNCTOR_PTR;
typedef NF_SHARE_PTR<EVENT_ASYNC_PROCESS_FUNCTOR> EVENT_ASYNC_PROCESS_FUNCTOR_PTR;

#endif
