// -------------------------------------------------------------------------
//    @FileName      :    NFTutorial2.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2014-05-14 08:51
//    @Module           :   NFTutorial2
//
// -------------------------------------------------------------------------

#ifndef _NF_TUTORIAL2_H_
#define _NF_TUTORIAL2_H_

///
#include "NFComm/NFPluginModule/NFIPlugin.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"

class Tutorial2 : public NFIPlugin
{
public:
    Tutorial2(NFIPluginManager* p)
    {
        pPluginManager = p;
    }
    virtual const int GetPluginVersion();

    virtual const std::string GetPluginName();

    virtual void Install();

    virtual void Uninstall();
};
#endif