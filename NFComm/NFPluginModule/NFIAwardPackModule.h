// -------------------------------------------------------------------------
//    @FileName         £º    NFIAwardPackModule.h
//    @Author           £º    LvSheng.Huang
//    @Date             £º    2012-12-15
//    @Module           £º    NFIAwardPackModule
//
// -------------------------------------------------------------------------

#ifndef _NFI_AWAED_MODULE_H_
#define _NFI_AWAED_MODULE_H_

#include <iostream>
#include "NFILogicModule.h"
#include "NFComm/NFCore/NFCDataList.h"
#include "NFComm/NFMessageDefine/NFDefine.pb.h"

class NFIAwardPackModule
    : public NFILogicModule
{
public:
    struct AwardItem
    {
        AwardItem() : strConfigID(""), nCount(0), nType(NFMsg::AWARD_TYPE_NORMAL), nRate(0)
        {

        }

        std::string strConfigID;
        int nCount;
        NFUINT16 nType;
        int nRate;
    };

    struct AwardBag
    {
        AwardBag()
        {
            strBagID = "";
            bRandom = false;
            nTotalRate = 0;
            nPackRate = 0;
            nCount = 0;
        }

        typedef NFList<NF_SHARE_PTR<AwardItem> > AwardItemList;

        NF_SHARE_PTR<AwardItem> GetRandItem(const int nRate)
        {
            NF_SHARE_PTR<AwardItem> pAwardItem;
            bool bRet = xAwardItemList.First(pAwardItem);
            while (bRet && pAwardItem != nullptr)
            {
                if (pAwardItem->nRate >= nRate)
                {
                    return pAwardItem;
                }

                bRet = xAwardItemList.Next(pAwardItem);
            }

            return nullptr;
        }

        std::string strBagID;
        bool bRandom;
        NFINT64 nPackRate;
        NFUINT32 nCount;
        NFUINT32 nTotalRate;
        AwardItemList xAwardItemList;
    };

	virtual bool LoadAwardPackConfig() = 0;
    virtual bool ExistAawardPack(const std::string& strAawardPack) = 0;


	virtual NF_SHARE_PTR<NFIAwardPackModule::AwardBag> GetAwardPack(const std::string& strPackName) = 0;

    virtual bool DoAward(const NFIDENTID self, const std::string& strPack, const int nMailType, NFIDataList& varItemList, NFIDataList& varCountList) = 0;
    virtual bool DoAward(const NFIDENTID self, const std::string& strPack) = 0; 

};

#endif
