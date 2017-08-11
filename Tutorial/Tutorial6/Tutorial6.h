// -------------------------------------------------------------------------
//    @FileName			:    NFTutorial6.h
//    @Author           :    bluesky
//    @Date             :    2017-08-07 08:51
//    @Module           :	 NFTutorial6
//
// -------------------------------------------------------------------------

#ifndef NF_TUTORIAL6_H
#define NF_TUTORIAL6_H

///
#include "NFComm/NFPluginModule/NFIPlugin.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"

class Tutorial6 : public NFIPlugin
{
public:
	Tutorial6(NFIPluginManager* p)
    {
        pPluginManager = p;
    }
    virtual const int GetPluginVersion();

    virtual const std::string GetPluginName();

    virtual void Install();

    virtual void Uninstall();
};
#endif