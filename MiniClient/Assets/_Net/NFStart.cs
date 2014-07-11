using UnityEngine;
using System.Collections;
using NFTCPClient;
using System;
using System.Collections.Generic;
using System.IO;
using System.Net;
using System.Net.Sockets;
using System.Threading;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;

public class NFStart : MonoBehaviour
{
    //看开启多人模式还是单人模式
    enum PLAYER_MODE
    {
        E_NONE,
        E_SINGLE_PLAYER,//单人
        E_MUTILE_COUNT,//多人
    };


    PLAYER_MODE mToolMode = PLAYER_MODE.E_SINGLE_PLAYER;

    NFConfig mConfig = null;
    NFNet netFocus = null;

    void Start()
    {
        mConfig = new NFConfig();
        mConfig.Load();
    }


    void Update()
    {
        if (PLAYER_MODE.E_SINGLE_PLAYER == mToolMode)
        {
            if (null != netFocus)
            {
                netFocus.Update();
            }
        }
    }

    void OnDestroy()
    {
        if (PLAYER_MODE.E_SINGLE_PLAYER == mToolMode)
        {
            if (null != netFocus)
            {
                netFocus.Destroy();
            }
        }
    }

    void OnGUI()
    {
        if (PLAYER_MODE.E_SINGLE_PLAYER == mToolMode)
        {
            if (null != netFocus)
            {
                switch (netFocus.mPlayerState)
                {
                    case NFNet.PLAYER_STATE.E_NONE:
                        DrawWaitingConfig();
                        break;
                    case NFNet.PLAYER_STATE.E_WAITING_PLAYER_LOGIN:
                        {
                            if (netFocus.strKey.Length > 0)
                            {
                                netFocus.mPlayerState = NFNet.PLAYER_STATE.E_HAS_PLAYER_GATE;
                            }
                            else
                            {
                                DrawWaitingLogin();
                            }
                        }
                        break;

                    case NFNet.PLAYER_STATE.E_HAS_PLAYER_LOGIN:
                        DrawWaitingSelectWorld();
                        break;

                    case NFNet.PLAYER_STATE.E_WAITING_PLAYER_TO_GATE:
                    case NFNet.PLAYER_STATE.E_HAS_PLAYER_SELECT_WORLD:
                    case NFNet.PLAYER_STATE.E_HAS_PLAYER_GATE:
                        DrawWaitingWaitingGate();
                        break;
                    case NFNet.PLAYER_STATE.E_HAS_VERIFY:
                        DrawWaitingSelectServer();
                        break;

                    case NFNet.PLAYER_STATE.E_HAS_PLAYER_ROLELIST:
                        DrawWaitingRoleList();
                        break;

                    case NFNet.PLAYER_STATE.E_PLAYER_GAMEING:
                        DrawGameing();
                        break;

                    default:
                        break;

                }
            }
            else
            {
                netFocus = new NFNet();
            }
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Vector2 scrollVecConfigMsg = new Vector2(0, 0);
    void Do_DrawWaitingConfigSingle(int windowId)
    {
        ArrayList strServerList = mConfig.GetServerList();

        int height = 50;
        int nOffest = 0;

        int nScrolHeight = strServerList.Count * height + 1;
        scrollVecConfigMsg = GUI.BeginScrollView(new Rect(height, height, 250, strServerList.Count * height), scrollVecConfigMsg, new Rect(0, 0, 200, nScrolHeight));
        for (int i = 0; i < strServerList.Count; i++)
        {
            string strData = (string)strServerList[i];
            if (GUI.Button(new Rect(0, nOffest, 200, 50), strData))
            {
                string[] str = strData.Split(':');
                if (str.Length == 2)
                {
                    string strTargetIP = str[0];
                    int nTargetPort = int.Parse(str[1]);

                    netFocus.StartConnect(strTargetIP, nTargetPort);
                }
            }

            nOffest += height;
        }

        GUI.EndScrollView();

    }

    void DrawWaitingConfig()
    {

        int width = 300;
        int height = 500;
        if (PLAYER_MODE.E_SINGLE_PLAYER == mToolMode
            || PLAYER_MODE.E_MUTILE_COUNT == mToolMode)
        {
            GUI.Window(0, new Rect((Screen.width - width) / 2, (Screen.height - height) / 2, width, height), Do_DrawWaitingConfigSingle, "服务器选择");
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    void Do_DrawWaitingLoginSingle(int windowId)
    {
        int height = 50;
        int nOffest = 0;

        nOffest += height;
        netFocus.strAccount = GUI.TextField(new Rect(25, nOffest, 150, height), netFocus.strAccount);
        nOffest += height;

        netFocus.strPassword = GUI.TextField(new Rect(25, nOffest, 150, height), netFocus.strPassword);
        nOffest += height;

        if (GUI.Button(new Rect(25, nOffest, 150, height), "Login"))
        {
            if (null != netFocus.client)
            {
                netFocus.sendLogic.LoginPB(netFocus.strAccount, netFocus.strPassword, "");
            }
        }

        nOffest += height;
        if (null != netFocus)
        {
            ////////////////////
            string strState = netFocus.strAccount + " 连接中...";

            if (netFocus.mPlayerState == NFNet.PLAYER_STATE.E_WAITING_PLAYER_LOGIN)
            {
                strState = netFocus.strAccount + " 连接成功";
            }
            else if (netFocus.mPlayerState == NFNet.PLAYER_STATE.E_PLAYER_LOGINING)
            {
                strState = netFocus.strAccount + " 登录中...";
            }
            else if (netFocus.mPlayerState == NFNet.PLAYER_STATE.E_HAS_PLAYER_LOGIN)
            {
                strState = netFocus.strAccount + " 登录成功";
            }

            GUI.Label(new Rect(25, nOffest, 150, height), strState);
        }
    }

    void DrawWaitingLogin()
    {
        if (PLAYER_MODE.E_SINGLE_PLAYER == mToolMode)
        {
            int width = 200;
            int height = 500;
            GUI.Window(0, new Rect((Screen.width - width) / 2, (Screen.height - height) / 2, width, height), Do_DrawWaitingLoginSingle, "登录");
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Vector2 scrollVecSelectWorld = new Vector2();
    void Do_DrawWaitingSelectWorldSingle(int windowId)
    {
        int height = 50;
        int nOffest = 0;

        int nScrolHeight = netFocus.listener.aWorldList.Count * height + 1;
        scrollVecSelectWorld = GUI.BeginScrollView(new Rect(height, height, 250, 450), scrollVecSelectWorld, new Rect(0, 0, 200, nScrolHeight));
        for (int i = 0; i < netFocus.listener.aWorldList.Count; i++)
        {
            NFMsg.ServerInfo info = (NFMsg.ServerInfo)netFocus.listener.aWorldList[i];

            if (GUI.Button(new Rect(0, nOffest, 200, 50), System.Text.Encoding.Default.GetString(info.name) + " " + info.server_id + " " + info.status))
            {
                netFocus.nServerID = info.server_id;

                netFocus.sendLogic.RequireConnectWorld(info.server_id);
                netFocus.mPlayerState = NFNet.PLAYER_STATE.E_HAS_PLAYER_SELECT_WORLD;
            }

            nOffest += height;
        }
        GUI.EndScrollView();
    }


    void DrawWaitingSelectWorld()
    {
        if (PLAYER_MODE.E_SINGLE_PLAYER == mToolMode)
        {
            int width = 300;
            int height = 500;
            GUI.Window(0, new Rect((Screen.width - width) / 2, (Screen.height - height) / 2, width, height), Do_DrawWaitingSelectWorldSingle, "选择游戏世界");
        }
    }

    void Do_DrawWaitingSelectWaitingGate(int windowId)
    {
        if (netFocus.mPlayerState == NFNet.PLAYER_STATE.E_HAS_PLAYER_SELECT_WORLD)
        {
            GUI.Label(new Rect(25, 50, 150, 50), "已选择服务器...等待Key...");
        }
        else if (netFocus.mPlayerState == NFNet.PLAYER_STATE.E_WAITING_PLAYER_TO_GATE)
        {
            GUI.Label(new Rect(25, 50, 150, 50), "key到手，正在尝试连接网关...");

            string strWorpdIP = netFocus.strWorldIP;
            string strWorpdKey = netFocus.strKey;
            string strAccount = netFocus.strKey;
            int nPort = netFocus.nWorldPort;

            netFocus = new NFNet();
            netFocus.strWorldIP = strWorpdIP;
            netFocus.strKey = strWorpdKey;
            netFocus.strAccount = strAccount;
            netFocus.nWorldPort = nPort;

            netFocus.mPlayerState = NFNet.PLAYER_STATE.E_START_CONNECT_TO_GATE;
            netFocus.StartConnect(netFocus.strWorldIP, nPort);
            netFocus.listener.InitLog();
        }
        else if (netFocus.mPlayerState == NFNet.PLAYER_STATE.E_HAS_PLAYER_GATE)
        {
            GUI.Label(new Rect(25, 50, 150, 50), "已成功连接网关...等待验证key...");

            netFocus.sendLogic.RequireVerifyWorldKey(netFocus.strAccount, netFocus.strKey);
            netFocus.mPlayerState = NFNet.PLAYER_STATE.E_WATING_VERIFY;
        }
    }


    void DrawWaitingWaitingGate()
    {
        if (PLAYER_MODE.E_SINGLE_PLAYER == mToolMode)
        {
            int width = 300;
            int height = 500;
            GUI.Window(0, new Rect((Screen.width - width) / 2, (Screen.height - height) / 2, width, height), Do_DrawWaitingSelectWaitingGate, "网关验证");
        }
    }

    Vector2 scrollVecSelectServer = new Vector2();
    void Do_DrawWaitingSelectServer(int windowId)
    {
        int height = 50;
        int nOffest = 0;

        int nScrolHeight = netFocus.listener.aServerList.Count * height + 1;
        scrollVecSelectServer = GUI.BeginScrollView(new Rect(height, height, 250, 450), scrollVecSelectServer, new Rect(0, 0, 200, nScrolHeight));

        for (int i = 0; i < netFocus.listener.aServerList.Count; i++)
        {
            NFMsg.ServerInfo info = (NFMsg.ServerInfo)netFocus.listener.aServerList[i];

            if (GUI.Button(new Rect(0, nOffest, 200, 50), System.Text.Encoding.Default.GetString(info.name) + " " + info.server_id + " " + info.status))
            {
                netFocus.nServerID = info.server_id;

                netFocus.sendLogic.RequireSelectServer(netFocus.nServerID);
            }

            nOffest += height;
        }
        GUI.EndScrollView();
    }


    void DrawWaitingSelectServer()
    {
        if (PLAYER_MODE.E_SINGLE_PLAYER == mToolMode)
        {
            int width = 300;
            int height = 500;
            GUI.Window(0, new Rect((Screen.width - width) / 2, (Screen.height - height) / 2, width, height), Do_DrawWaitingSelectServer, "服务器选择");
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Vector2 scrollVecRoleList = new Vector2();
    void Do_DrawWaitingRoleList(int windowId)
    {
        //1：显示角色列表
        //2：创建删除
        int height = 50;
        int nOffest = 0;

        nOffest += height;

        scrollVecRoleList = GUI.BeginScrollView(new Rect(0, nOffest, 250, 350), scrollVecRoleList, new Rect(0, 0, 200, 3000));

        for (int i = 0; i < netFocus.listener.aCharList.Count; i++)
        {
            NFMsg.RoleLiteInfo xLiteInfo = (NFMsg.RoleLiteInfo)netFocus.listener.aCharList[i];
            if (xLiteInfo.delete_time <= 0 && xLiteInfo.game_id == netFocus.nServerID)
            {
                string strState = "";
                if (netFocus.strRoleName == System.Text.Encoding.Default.GetString(xLiteInfo.noob_name))
                {
                    strState = "选中";
                }

                if (GUI.Button(new Rect(height / 2, nOffest, 200, 50), System.Text.Encoding.Default.GetString(xLiteInfo.noob_name) + " " + strState))
                {
                    netFocus.strRoleName = System.Text.Encoding.Default.GetString(xLiteInfo.noob_name);
                }

                nOffest += height;
            }
        }


        GUI.EndScrollView();

        netFocus.strRoleName = GUI.TextField(new Rect(400 / 2 + 75, 50, 200, 50), netFocus.strRoleName);


        if (netFocus.strRoleName.Length > 0)
        {
            if (netFocus.listener.aCharList.Count <= 0)
            {
                if (GUI.Button(new Rect(400 / 2 + 75, 100, 200, 50), "创建角色"))
                {
                    netFocus.sendLogic.RequireCreateRole(netFocus.strAccount, netFocus.strRoleName, 0, 1, netFocus.nServerID);
                }
            }
            else
            {
                if (GUI.Button(new Rect(400 / 2 + 75, 200, 200, 50), "删除角色"))
                {
                    netFocus.sendLogic.RequireDelRole(netFocus.strAccount, netFocus.strRoleName, netFocus.nServerID);
                }

                if (GUI.Button(new Rect(300 / 2, 400, 200, 50), "进入游戏"))
                {
                    netFocus.sendLogic.RequireEnterGameServer(netFocus.strAccount, netFocus.strRoleName, netFocus.nServerID);
                }
            }
        }
    }

    void DrawWaitingRoleList()
    {
        if (PLAYER_MODE.E_SINGLE_PLAYER == mToolMode)
        {
            int width = 500;
            int height = 500;
            GUI.Window(0, new Rect((Screen.width - width) / 2, (Screen.height - height) / 2, width, height), Do_DrawWaitingRoleList, "角色列表------------------------创建角色");
        }
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    void Do_DrawGameingSingle(int windowId)
    {
        netFocus.OnGUI(768, 1024);
    }

    void DrawGameing()
    {
        if (PLAYER_MODE.E_SINGLE_PLAYER == mToolMode)
        {
            int width = 1024;
            int height = 768;
            string strTitle = "角色正在游戏中 " + netFocus.strRoleName;
            GUI.Window(0, new Rect((Screen.width - width) / 2, (Screen.height - height) / 2, width, height), Do_DrawGameingSingle, strTitle);
        }
    }


}
