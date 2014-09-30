// -------------------------------------------------------------------------
//    @FileName         :    NFDefine.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-03-01
//    @Module           :    NFDefine
//
// -------------------------------------------------------------------------

#ifndef _NF_DEFINE_H_
#define _NF_DEFINE_H_

#include "NFIdentID.h"
#include "NFIDataList.h"
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

typedef std::shared_ptr<HEART_BEAT_FUNCTOR> HEART_BEAT_FUNCTOR_PTR;
typedef std::shared_ptr<PROPERTY_EVENT_FUNCTOR> PROPERTY_EVENT_FUNCTOR_PTR;
typedef std::shared_ptr<RECORD_EVENT_FUNCTOR> RECORD_EVENT_FUNCTOR_PTR;
typedef std::shared_ptr<CLASS_EVENT_FUNCTOR> CLASS_EVENT_FUNCTOR_PTR;
typedef std::shared_ptr<EVENT_PROCESS_FUNCTOR> EVENT_PROCESS_FUNCTOR_PTR;


#endif
