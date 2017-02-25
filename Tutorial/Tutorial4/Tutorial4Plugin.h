// -------------------------------------------------------------------------
//    @FileName			:		Tutorial4Plugin.h
//    @Author           :		LvSheng.Huang
//    @Date             :		2017-02-06
//    @Module           :		Tutorial4Plugin
//
// -------------------------------------------------------------------------

#ifndef NF_TUTORIAL4_H
#define NF_TUTORIAL4_H

///
#include "NFComm/NFPluginModule/NFIPlugin.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"

class Tutorial4Plugin : public NFIPlugin
{
public:
    Tutorial4Plugin(NFIPluginManager* p)
    {
        pPluginManager = p;
    }
    virtual const int GetPluginVersion();

    virtual const std::string GetPluginName();

    virtual void Install();

    virtual void Uninstall();
};
#endif