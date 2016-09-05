// -------------------------------------------------------------------------
//    @FileName         :    NFIEventModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFIEventModule
//
// -------------------------------------------------------------------------

#ifndef NFI_EVENT_MODULE_H
#define NFI_EVENT_MODULE_H

#include <iostream>
#include "NFIModule.h"


class NFIEventModule
    : public NFIModule
{

    virtual bool DoEvent(const int nEventID, const NFIDataList& valueList) = 0;

    virtual bool AddEventCallBack(const int nEventID, const EVENT_PROCESS_FUNCTOR_PTR& cb) = 0;

    virtual bool ExistEventCallBack(const int nEventID) = 0;
    
    virtual bool RemoveEventCallBack(const int nEventID) = 0;

/*
    template<typename BaseType>
    virtual bool AddEventCallBack(const NFGUID self,const int nEventID, const EVENT_PROCESS_FUNCTOR_PTR& cb)
    {
        return true;
    }
*/
    ///////////////////////////////////////////////////////////////////////////////////////////////

    virtual bool DoEvent(const NFGUID self, const int nEventID, const NFIDataList& valueList) = 0;

    virtual bool AddEventCallBack(const NFGUID self,const int nEventID, const EVENT_PROCESS_FUNCTOR_PTR& cb) = 0;

    virtual bool ExistEventCallBack(const NFGUID self,const int nEventID) = 0;
    
    virtual bool RemoveEventCallBack(const NFGUID self,const int nEventID) = 0;
/*
    template<typename BaseType>
    virtual bool AddEventCallBack(const NFGUID self,const int nEventID, const EVENT_PROCESS_FUNCTOR_PTR& cb)
    {
        return true;
    }
    */
    ////////////////////time event////////////////////////////////////////////////////////////

    virtual bool ExistTimeEvent(const std::string& strHeartBeatName) = 0;

    virtual bool AddTimeEvent(const std::string& strHeartBeatName, const HEART_BEAT_FUNCTOR_PTR& cb, const float fTime, const int nCount) = 0;
    virtual bool RemoveTimeEvent(const std::string& strHeartBeatName) = 0;
/*
    template<typename BaseType>
    bool AddTimeEvent(const std::string& strHeartBeatName, BaseType* pBase, int (BaseType::*handler)(const NFGUID&, const std::string&, const float, const int), const float fTime, const int nCount)
    {
        HEART_BEAT_FUNCTOR functor = std::bind(handler, pBase, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
        HEART_BEAT_FUNCTOR_PTR functorPtr(NF_NEW HEART_BEAT_FUNCTOR(functor));
        return AddHeartBeat(self, strHeartBeatName, functorPtr, fTime, nCount);
    }
    */

    virtual bool ExistTimeEvent(const NFGUID self, const std::string& strHeartBeatName) = 0;

    virtual bool AddTimeEvent(const NFGUID self, const std::string& strHeartBeatName, const HEART_BEAT_FUNCTOR_PTR& cb, const float fTime, const int nCount) = 0;
    virtual bool RemoveTimeEvent(const NFGUID self, const std::string& strHeartBeatName) = 0;
/*
    template<typename BaseType>
    bool AddTimeEvent(const NFGUID self, const std::string& strHeartBeatName, BaseType* pBase, int (BaseType::*handler)(const NFGUID&, const std::string&, const float, const int), const float fTime, const int nCount)
    {
        HEART_BEAT_FUNCTOR functor = std::bind(handler, pBase, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
        HEART_BEAT_FUNCTOR_PTR functorPtr(NF_NEW HEART_BEAT_FUNCTOR(functor));
        return AddHeartBeat(self, strHeartBeatName, functorPtr, fTime, nCount);
    }
    */







};

#endif