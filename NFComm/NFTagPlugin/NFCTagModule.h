// -------------------------------------------------------------------------
//    @FileName      :    NFCTagModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-07-08
//    @Module           :    NFCTagModule
//    @Desc             :
// -------------------------------------------------------------------------

#ifndef NFC_TAG_MODULE_H
#define NFC_TAG_MODULE_H

#include "NFComm/NFPluginModule/NFITagModule.h"

class NFCTagModule
    : public NFITagModule
{
public:
    NFCTagModule(NFIPluginManager* p)
    {
        pPluginManager = p;
    }

    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute();

    virtual bool AfterInit();
    virtual bool BeforeShut();

    virtual bool GetTagList(const std::string& strTag, NFList<NFGUID>& xList);
    virtual bool AddToTagList(const std::string& strTag, const NFGUID object);
    virtual bool RemoveFrmTagList(const std::string& strTag, const NFGUID object);

    virtual bool GetTagList(const TAG_DEFINE eTag, const std::string& strTag, NFList<NFGUID>& xList);
    virtual bool AddToTagList(const TAG_DEFINE eTag, const std::string& strTag, const NFGUID object);
    virtual bool RemoveFrmTagList(const TAG_DEFINE eTag, const std::string& strTag, const NFGUID object);

protected:

private:
    //tag_id->(tag_layer, object_list)
    NFMap<int, NFMap<std::string, NFList<NFGUID>>> mxTagManager;
};

#endif