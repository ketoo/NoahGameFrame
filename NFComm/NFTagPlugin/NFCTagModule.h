// -------------------------------------------------------------------------
//    @FileName      :    NFCTagModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-07-08
//    @Module           :    NFCTagModule
//    @Desc             :
// -------------------------------------------------------------------------

#ifndef _NFC_TAG_MODULE_H_
#define _NFC_TAG_MODULE_H_

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
    virtual bool Execute(const float fLasFrametime, const float fStartedTime);

    virtual bool AfterInit();
    virtual bool BeforeShut();

	virtual NFList<NFIDENTID>& GetTagList(const std::string& strTag);
	virtual bool  AddToTagList(const std::string& strTag, const NFIDENTID object);
	virtual bool  RemoveFrmTagList(const std::string& strTag, const NFIDENTID object);

	virtual NFList<NFIDENTID>& GetTagList(const TAG_DEFINE eTag, const std::string& strTag);
	virtual bool  AddToTagList(const TAG_DEFINE eTag, const std::string& strTag, const NFIDENTID object);
	virtual bool  RemoveFrmTagList(const TAG_DEFINE eTag, const std::string& strTag, const NFIDENTID object);

protected:

private:
	//tag_id->(tag_layer, object_list)
	NFMap<int, NFMap<std::string, NFList<NFIDENTID>>> mxTagManager;
};

#endif