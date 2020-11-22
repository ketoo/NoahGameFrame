//
// Created by James on 21/11/20.
//

#ifndef NFGAMEDEMO_NFLUANETMODULE_H
#define NFGAMEDEMO_NFLUANETMODULE_H

#include "NFComm/NFPluginModule/NFILogModule.h"

class NFILuaNetModule : public NFIModule
{

};

class NFLuaNetModule : public NFILuaNetModule
{
private:
	NFLuaNetModule(){}

public:

	NFLuaNetModule(NFIPluginManager* p)
	{
		pPluginManager = p;
	}

	virtual bool Awake();
	virtual bool Init();

	virtual bool AfterInit();
	virtual bool CheckConfig();
	virtual bool ReadyExecute();

	virtual bool Execute();

	virtual bool BeforeShut();
	virtual bool Shut();

	virtual bool Finalize();
	virtual bool OnReloadPlugin();
};


#endif //NFGAMEDEMO_NFLUANETMODULE_H
