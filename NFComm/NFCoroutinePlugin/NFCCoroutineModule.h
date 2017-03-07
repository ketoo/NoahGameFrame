// -------------------------------------------------------------------------
//    @FileName			:    NFCCoroutineModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2017-03-07
//    @Module           :    NFCCoroutineModule
//    @Desc             :
// -------------------------------------------------------------------------

#ifndef NF_COROUTINE_MODULE_H
#define NF_COROUTINE_MODULE_H

#include "NFContextData.hpp"

class NFCCoroutineModule
	: public NFICoroutineModule
{
public:
	NFCCoroutineModule(NFIPluginManager* p);
	virtual ~NFCCoroutineModule();

	virtual bool Init();
	virtual bool AfterInit();
	virtual bool BeforeShut();
	virtual bool Shut();
	virtual bool Execute();

private:
	void Schedule();
	void StartCoroutine(NF_SHARE_PTR<NFContextData> xContextData);
	void ResumeCoroutine(NF_SHARE_PTR<NFContextData> xContextData);
private:
	std::map<NFINT64, NF_SHARE_PTR<NFContextData>> mxCoroutineMap;
};


#endif
