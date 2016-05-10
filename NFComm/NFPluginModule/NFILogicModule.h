// -------------------------------------------------------------------------
//    @FileName      :    NFILogicModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFILogicModule
//
// -------------------------------------------------------------------------

#ifndef NFI_LOGIC_MODULE_H
#define NFI_LOGIC_MODULE_H

#include <string>
#include "NFComm/NFCore/NFMap.h"
#include "NFComm/NFCore/NFList.h"
#include "NFComm/NFCore/NFDefine.h"
#include "NFComm/NFCore/NFIDataList.h"
#include "NFComm/NFCore/NFIRecord.h"
#include "NFComm/NFEventDefine/NFEventDefine.h"

template<typename DerivedType, typename BaseType>
class TIsDerived
{
public:
    static int AnyFunction(BaseType* base)
    {
        return 1;
    }

    static  char AnyFunction(void* t2)
    {
        return 0;
    }

    enum
    {
        Result = (sizeof(int) == sizeof(AnyFunction((DerivedType*)NULL))),
    };

};

class  NFCModuleHeartBeatElement
    : public NFList<MODULE_HEART_BEAT_FUNCTOR_PTR>
{
public:
    bool operator==(const NFCModuleHeartBeatElement& src)
    {
        if (strBeatName == src.strBeatName)
        {
            return true;
        }

        return false;
    }

    NFCModuleHeartBeatElement()
    {
        nBeatTime = 0;
        nNextTriggerTime = 0;
        strBeatName = "";
    };

    virtual ~NFCModuleHeartBeatElement()
    {
    }

    void DoHeartBeatEvent();

    NFINT64 nBeatTime;
    NFINT64 nNextTriggerTime;//next trigger time, millisecond
    std::string strBeatName;
};

class NFIPluginManager;

class NFILogicModule
{

public:
    NFILogicModule()
    {
        bCanReload = true;
    }

    virtual ~NFILogicModule() {}

    virtual bool Init()
    {

        return true;
    }

    virtual bool AfterInit()
    {
        return true;
    }

    virtual bool CheckConfig()
    {
        return true;
    }

    virtual bool BeforeShut()
    {
        return true;
    }

    virtual bool Shut()
    {
        return true;
    }

    virtual bool Execute()
    {
        return true;
    }

    virtual void OnReload(const char* strModuleName, NFILogicModule* pModule)
    {
        BeforeShut();
        Shut();

        Init();
        AfterInit();
    }

    virtual NFIPluginManager* GetPluginManager() const
    {
        return pPluginManager;
    }

    template<typename BaseType>
    bool AddHeartBeatExecute(const std::string& strHeartBeatName, BaseType* pBase, int (BaseType::*handler)(), const float fTime)
    {
        MODULE_HEART_BEAT_FUNCTOR functor = std::bind(handler, pBase);
        MODULE_HEART_BEAT_FUNCTOR_PTR functorPtr(NF_NEW MODULE_HEART_BEAT_FUNCTOR(functor));

        return AddHeartBeat(strHeartBeatName, functorPtr, fTime);
    }

    bool RemoveHeartBeat(const std::string& strHeartBeatName)
    {
        return mRemoveListEx.Add(strHeartBeatName);
    }

public:
    std::string strName;
    bool  bCanReload;

protected:
    NFIPluginManager* pPluginManager;

private:
    bool AddHeartBeat(const std::string& strHeartBeatName, const MODULE_HEART_BEAT_FUNCTOR_PTR& cb, const int nTime)
    {
        NFCModuleHeartBeatElement xHeartBeat;
        xHeartBeat.nNextTriggerTime = time(NULL) + nTime;
        xHeartBeat.nBeatTime = nTime;
        xHeartBeat.strBeatName = strHeartBeatName;
        xHeartBeat.Add(cb);
        mAddListEx.push_back(xHeartBeat);
    }

    void ExecuteHeartBeat()
    {
        NF_SHARE_PTR<NFCModuleHeartBeatElement> pElement = mHeartBeatElementMapEx.First();
        while (pElement.get())
        {
            //second
            NFINT64 nTime = time(NULL);

            if (nTime > pElement->nNextTriggerTime)
            {

                pElement->DoHeartBeatEvent();

                //Do Event
                pElement->nNextTriggerTime = nTime + pElement->nBeatTime;
            }

            pElement = mHeartBeatElementMapEx.Next();
        }

        //删除所有过时心跳
        std::string strHeartBeatName;
        bool bRet = mRemoveListEx.First(strHeartBeatName);
        while (bRet)
        {
            mHeartBeatElementMapEx.RemoveElement(strHeartBeatName);

            bRet = mRemoveListEx.Next(strHeartBeatName);
        }

        mRemoveListEx.ClearAll();

        //////////////////////////////////////////////////////////////////////////
        //添加新心跳也是延时添加的
        for (std::list<NFCModuleHeartBeatElement>::iterator iter = mAddListEx.begin(); iter != mAddListEx.end(); ++iter)
        {
            if (NULL == mHeartBeatElementMapEx.GetElement(iter->strBeatName))
            {
                NF_SHARE_PTR<NFCModuleHeartBeatElement> pHeartBeatEx(NF_NEW NFCModuleHeartBeatElement());
                *pHeartBeatEx = *iter;
                mHeartBeatElementMapEx.AddElement(pHeartBeatEx->strBeatName, pHeartBeatEx);
            }
        }

        mAddListEx.clear();
    }


private:
    NFList<std::string> mRemoveListEx;
    std::list<NFCModuleHeartBeatElement> mAddListEx;
    NFMapEx<std::string, NFCModuleHeartBeatElement> mHeartBeatElementMapEx;
};
#endif
