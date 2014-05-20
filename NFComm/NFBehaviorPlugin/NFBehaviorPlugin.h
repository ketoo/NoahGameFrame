#pragma once

#include <Windows.h>
#include "NFComm/NFPluginModule/NFIPlugin.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"

#ifdef NFBEHAVIOR_EXPORTS
#define NFBEHAVIOR_API __declspec(dllexport)
#else
#define NFBEHAVIOR_API __declspec(dllimport)
#endif

//////////////////////////////////////////////////////////////////////////
class NFCBehaviorPlugin : public NFIPlugin
{
public:

    virtual const int GetPluginVersion();

    virtual const std::string GetPluginName();

    virtual void Install();

    virtual void Uninstall();
};