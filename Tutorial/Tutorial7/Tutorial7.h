// -------------------------------------------------------------------------
//    @FileName      :    Tutorial7.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2014-05-14 08:51
//    @Module           :   Tutorial7
//
// -------------------------------------------------------------------------

#ifndef NF_Tutorial7_H
#define NF_Tutorial7_H

#include "NFComm/NFPluginModule/NFIPlugin.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"

class Tutorial7 : public NFIPlugin
{
public:
    Tutorial7(NFIPluginManager* p)
    {
        pPluginManager = p;
    }
    virtual const int GetPluginVersion();

    virtual const std::string GetPluginName();

    virtual void Install();

    virtual void Uninstall();
};
#endif