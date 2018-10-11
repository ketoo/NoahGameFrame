#include "main.h"
#include "AppDelegate.h"
#include "cocos2d.h"

#if NF_PLATFORM == NF_PLATFORM_WIN
#pragma comment( lib, "Dbghelp.lib" )
#endif

USING_NS_CC;

int WINAPI _tWinMain(HINSTANCE hInstance,
                       HINSTANCE hPrevInstance,
                       LPTSTR    lpCmdLine,
                       int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // create the application instance
    AppDelegate app;
    return Application::getInstance()->run();
}
