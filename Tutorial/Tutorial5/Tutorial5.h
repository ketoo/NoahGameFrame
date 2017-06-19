// -------------------------------------------------------------------------
//    @FileName			:    NFTutorial5.h
//    @Author           :    stone.xin
//    @Date             :    2017-06-14 08:51
//    @Module           :   NFTutorial5
//
// -------------------------------------------------------------------------

#ifndef NF_TUTORIAL5_H
#define NF_TUTORIAL5_H

///
#include "NFComm/NFPluginModule/NFIPlugin.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"

class Tutorial5 : public NFIPlugin
{
public:
	Tutorial5(NFIPluginManager* p)
    {
        pPluginManager = p;
    }
    virtual const int GetPluginVersion();

    virtual const std::string GetPluginName();

    virtual void Install();

    virtual void Uninstall();
};
#endif