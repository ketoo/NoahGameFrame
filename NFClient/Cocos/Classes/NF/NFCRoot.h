// -------------------------------------------------------------------------
//    @FileName			:    NFCRoot.h
//    @Author           :    Johance
//    @Date             :    2016-12-27
//    @Module           :    NFCRoot
//
// -------------------------------------------------------------------------

#ifndef __NFCRoot_H__
#define __NFCRoot_H__

#include "cocos2d.h"
#include "NFComm/NFCore/NFSingleton.hpp"

class NFCRoot : public cocos2d::CCNode, public NFSingleton<NFCRoot>
{
public:
	enum IndexZOrder
	{
		eIZO_Scene = 0,
        eIZO_UI,
        eIZO_WebView,
	};

	NFCRoot();
	~NFCRoot();
    virtual bool init();  
    static cocos2d::CCScene* scene();
	virtual void update(float dt);
    CREATE_FUNC(NFCRoot);
};

#endif // __HELLOWORLD_SCENE_H__
