#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#ifdef WIN32
#define ntohll(x)       (x)
#define htonll(x)       (x)
#endif

// 必须放这边 引导 cocos和libevent有一样的timezone
//#include "NFComm/NFPluginModule/NFINetClientModule.h"

USING_NS_CC;
using namespace cocostudio;
using namespace CocosDenshion;

using namespace std;

#include <string>
#include <vector>
#include <map>
#include <cmath>	
#include <time.h>
#include <iostream>

#include <fstream>
#include <sstream>
#include <deque>
#include <list>
#include <math.h>