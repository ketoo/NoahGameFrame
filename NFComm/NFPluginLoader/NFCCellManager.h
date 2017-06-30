// -------------------------------------------------------------------------
//    @FileName			:    NFCCellManager.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2017-06-30
//    @Module           :    NFCCellManager
//
// -------------------------------------------------------------------------

#ifndef NFC_CELL_MANAGER_H
#define NFC_CELL_MANAGER_H

#include "NFComm/NFCore/NFSingleton.hpp"
#include "NFComm/NFPluginModule/NFICellManager.h"

class NFCCellManager :
	public NFICellManager,
	public NFSingleton<NFCCellManager>
{
public:
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

private:
	void InitCell(const int nCellNum);
};

#endif