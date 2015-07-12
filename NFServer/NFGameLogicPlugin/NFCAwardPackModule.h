// -------------------------------------------------------------------------
//    @FileName         ：    NFCAwardPackModule.h
//    @Author           ：    LvSheng.Huang
//    @Date             ：    2012-12-15
//    @Module           ：    NFCAwardPackModule
//
// -------------------------------------------------------------------------

#ifndef _NFC_AWAED_MODULE_H_
#define _NFC_AWAED_MODULE_H_

#include <iostream>
#include "NFComm/Config/NFConfig.h"
#include "NFComm/RapidXML/rapidxml.hpp"
#include "NFComm/RapidXML/rapidxml_iterators.hpp"
#include "NFComm/RapidXML/rapidxml_print.hpp"
#include "NFComm/RapidXML/rapidxml_utils.hpp"
#include "NFComm/NFPluginModule/NFIAwardPackModule.h"
#include "NFComm/NFPluginModule/NFIElementInfoModule.h"
#include "NFComm/NFPluginModule/NFIEventProcessModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIPackModule.h"
#include "NFComm/NFPluginModule/NFILogModule.h"

class NFCAwardPackModule
    : public NFIAwardPackModule
{
public:
    NFCAwardPackModule(NFIPluginManager* p)
    {
        pPluginManager = p;
    }

    virtual ~NFCAwardPackModule() {};

    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute(const float fLasFrametime, const float fStartedTime);
    virtual bool AfterInit();
    virtual bool BeforeShut();

    //@param: nMailType 是 MailType 枚举
    virtual bool DoAward(const NFIDENTID self, const std::string& strPack, const int nMailType, NFIDataList& varItemList, NFIDataList& varCountList);

    virtual bool DoAward(const NFIDENTID self, const std::string& strPack);
    virtual NF_SHARE_PTR<NFIAwardPackModule::AwardBag> GetAwardPack(const std::string& strPackName);
   
    virtual bool LoadAwardPackConfig();

    //装备加成
    virtual float GetEquipItemExtenRatio(const NFIDENTID& self, const std::string& strAwardPackID);

    //货币的加成
    virtual float GetNoneItemExtenRatio(const NFIDENTID& self, const std::string& strAwardPackID);

    virtual bool IsEquipItem(const NFIDENTID& self, const std::string& strID);

    virtual bool ExistAawardPack(const std::string& strAawardPack);
protected:

    bool UnLoadProductionConfig();

    void CalcAward(const std::string& strAwardPack, NFIDataList& varItemList, NFIDataList& varCountList, NFIDataList& varTypeList);
    void GivingAward(const NFIDENTID& self, NFIDataList& varItemList, NFIDataList& varCountList, NFIDataList& varTypeList, bool bMail = false, int nMailType = 0);
private: 
    NFMapEx<std::string, AwardBag> mtAwardPack;
private:
    NFIElementInfoModule* m_pElementInfoModule;
    NFIPackModule* m_pPackModule;
    NFIKernelModule* m_pKernelModule;
    NFILogModule* m_pLogModule;
    NFIEventProcessModule* m_pEventProcessModule;
};
#endif
