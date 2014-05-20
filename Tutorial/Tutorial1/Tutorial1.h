// -------------------------------------------------------------------------
//    @FileName      :    Tutorial1.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2014-05-14 08:51
//    @Module           :   Tutorial1
//
// -------------------------------------------------------------------------

#ifndef _NF_TUTORIAL1_H_
#define _NF_TUTORIAL1_H_

#include "NFComm/NFPluginModule/NFIPlugin.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"

class Tutorial1 : public NFIPlugin
{
public:
    Tutorial1(NFIPluginManager* p)
    {
        pPluginManager = p;
    }
    virtual const int GetPluginVersion();

    virtual const std::string GetPluginName();

    virtual void Install();

    virtual void Uninstall();
};
#endif