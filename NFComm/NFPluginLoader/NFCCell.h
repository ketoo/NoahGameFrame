// -------------------------------------------------------------------------
//    @FileName			:    NFCCell.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2017-06-30
//    @Module           :    NFCCell
//
// -------------------------------------------------------------------------

#ifndef NFC_CELL_H
#define NFC_CELL_H

#include "NFComm/NFPluginModule/NFICell.h"

class NFCCell : public NFICell
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
};

#endif