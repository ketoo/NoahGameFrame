// -------------------------------------------------------------------------
//    @FileName			:    NFCGameScene.h
//    @Author           :    Johance
//    @Date             :    2016-12-28
//    @Module           :    NFCGameScene
//
// -------------------------------------------------------------------------
#ifndef NFCGameScene_H
#define NFCGameScene_H

#include "NFCSceneManager.h"

class NFCGameScene : public IUniqueScene<NFCGameScene>
{
public:
	NFCGameScene();
	~NFCGameScene();
	
	virtual bool initLayout();
	virtual void initData(void *pData);

private:
	int OnObjectClassEvent(const NFGUID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFIDataList& var);

private:
	NFMap<NFGUID, Sprite> m_Players;
};

#endif // __HELLOWORLD_SCENE_H__
