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

    enum TOOL_STATE
    {
        E_TOOL_MODE,//等待选择模式
        E_TOOL_CONFIG,//多人配置确认界面
        E_TOOL_LOGING,//登录
        E_TOOL_GAMEING,//游戏中
    };


    PLAYER_MODE mToolMode = PLAYER_MODE.E_NONE;
    TOOL_STATE meToolState = TOOL_STATE.E_TOOL_MODE;//全局的状态
    Vector2 mVecStartPos = new Vector2(100.0f, 100.0f);

    NFConfig mConfig = null;

    string strTargetIP;
    int nTargetPort;

    NFNet netFocus = null;
    ArrayList netList = new ArrayList();

    /////////////Logic///////////////////////////////////////////////
    public string strReqSwapSceneID = "sceneID";
    public string strReqSwapSceneLine = "groupID";

    public string strReqMoveX = "0.0";
    public string strReqMoveZ = "0.0";

    public string strReqAcceptTaskNPC = "npcid";
    public string strReqAcceptTaskID = "taskid";

    public string strReqKillID = "skillid";
    public string strReqKillNPCID = "npcid";

    public string strPullCustom = "0 or 1";

    public string strPickUpItemID = "itemid";
    public string strPickUpNPCID = "npcid";
    public string strPickUpRow = "row";

    public string strReqSetProperty = "property";
    public string strReqPropertyValue = "value";

    public string strReqAddItem = "additem";
    public string strReqAddCount = "count";

    public string strSwapOrigin = "swaporigin";
    public string strSwapTarget = "target";

    public string strChatTargetID = "target";
    public string strType = "type";
    public string strChatData = "data";

    public string strOnagerName = "onagerName";
    public string strOnagerLevel = "level";
    /// <summary>
    /// 
    /// 
    /// </summary>
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
        else
        {
            for (int i = 0; i < netList.Count; i++)
            {
                NFNet rbNet = (NFNet)netList[i];
                rbNet.Update();
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
        else
        {
            for (int i = 0; i < netList.Count; i++)
            {
                NFNet rbNet = (NFNet)netList[i];
                rbNet.client.Disconnect();
                rbNet = null;
            }
        }
    }

    float fTime = 0.0f;
    int indexer = 0;
//     void RobotAI(NFNet rbNet)
//     {
//         //进入主界面后没人就随机时间后创建人--看见俺有效帐号
//         if(rbNet.listener.aCharList.Count <= 0)
//         {
//             if(rbNet.mPlayerState == NFNet.PLAYER_STATE.E_HAS_PLAYER_SELECTSERVER)
//             {
//                 indexer++;
//                 string strName = "rb_";
//                 strName += DateTime.Now.DayOfYear + "_" + DateTime.Now.Hour + "_" + DateTime.Now.Minute + "_" + DateTime.Now.Second + "_" + indexer;
//             
//                 rbNet.sendLogic.RequireCreateChar((UInt32)rbNet.nUserID, strName, 0, 1, (UInt32)rbNet.nServerID);
// 
//                 Debug.Log(strName);
// 
//                 rbNet.mPlayerState = NFNet.PLAYER_STATE.E_HAS_PLAYER_ROLELIST;
//             }
//         }
//         else
//         {
//             if (rbNet.mPlayerState == NFNet.PLAYER_STATE.E_HAS_PLAYER_ROLELIST)
//             {
//                 rbNet.mPlayerState = NFNet.PLAYER_STATE.E_PLAYER_WAITING_TO_GAME;
//                 //可是在等待过程中呢
//                 NFMsg.CharLiteInfo xLiteInfo = (NFMsg.CharLiteInfo)rbNet.listener.aCharList[0];
//                 rbNet.nSeleroleID = (uint)xLiteInfo.char_id;
// 
//                 rbNet.sendLogic.RequireEnterGameServer(rbNet.nUserID, rbNet.nServerID, rbNet.nSeleroleID);
//             }
//             else if (rbNet.mPlayerState == NFNet.PLAYER_STATE.E_PLAYER_GAMEING)
//             {
//                 fTime += Time.deltaTime;
//                 if(fTime > 2f)
//                 {
//                     fTime = 0f;
//                     
//                     //几秒一次呢
//                     //随机走路啊
// 
//                     System.Random ro = new System.Random();
//                     int nRD = ro.Next(0, 100);
//                     if (nRD < 20)
//                     {
//                         int nX = ro.Next(30, 60);
//                         int nZ = ro.Next(10, 70);
// 
//                         rbNet.sendLogic.RequireMove(rbNet.nUserID, (uint)rbNet.nSeleroleID, rbNet.nSeleroleID, (float)nX, (float)nZ);
//                     }
//                 
//                     //刷副本啊。。。
//                     //随机走路啊。。。
//                     }
//  
//             }
//         }
//         
// 
//     }

    void RobotAI()
    {
        for (int i = 0; i < netList.Count; i++)
        {
            NFNet rbNet = (NFNet)netList[i];
            //RobotAI(rbNet);
        }
    }

    bool bBig = false;
    void OnGUI()
    {
        //工具状态
        if (TOOL_STATE.E_TOOL_MODE == meToolState)
        {
            DrawWaitingMode();
        }
        else if (TOOL_STATE.E_TOOL_CONFIG == meToolState)
        {
            DrawWaitingConfig();
        }
        else if (TOOL_STATE.E_TOOL_LOGING == meToolState)
        {
            DrawWaitingLogin();
        }
        else if (TOOL_STATE.E_TOOL_GAMEING == meToolState)
        {
            if (PLAYER_MODE.E_MUTILE_COUNT == mToolMode)
            {
                DrawGameing();
            }
        }

        if (PLAYER_MODE.E_SINGLE_PLAYER == mToolMode)
        {
            if (null != netFocus)
            {
                switch (netFocus.mPlayerState)
                {
                    case NFNet.PLAYER_STATE.E_WAITING_PLAYER_LOGIN:
                        DrawWaitingLogin();
                        break;

                    case NFNet.PLAYER_STATE.E_HAS_PLAYER_SELECTSERVER:
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
        }
    }

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    void Do_DrawWaitingMode(int windowId)
    {
        if (GUI.Button(new Rect(0, 50, 200, 100), "单人模式"))
        {
            mToolMode = PLAYER_MODE.E_SINGLE_PLAYER;
            meToolState = TOOL_STATE.E_TOOL_CONFIG;
            //netFocus.mPlayerState = NFNet.PLAYER_STATE.E_WAITING_PLAYER_CONFIG;
        }
        if (GUI.Button(new Rect(200, 50, 200, 100), "多人机器人模式"))
        {
            mToolMode = PLAYER_MODE.E_MUTILE_COUNT;
            meToolState = TOOL_STATE.E_TOOL_CONFIG;
        }
        //如果要想整个 窗口 都允许鼠标拖动则使用不带参数的构造方法如下
        //GUI.DragWindow();
    }
    //Rect windowDialog = new Rect(mVecStartPos.x, mVecStartPos.x, 150, 100);
    void DrawWaitingMode()
    {

        int width = 400;
        int height = 200;
        GUI.Window(0, new Rect((Screen.width - width) / 2, (Screen.height - height) / 2, width, height), Do_DrawWaitingMode, "选择模式");
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Vector2 scrollVecConfigMsg = new Vector2(0, 0);
    void Do_DrawWaitingConfigSingle(int windowId)
    {
        ArrayList strServerList = mConfig.GetServerList();

        int height = 50;
        int nOffest = 0;
        scrollVecConfigMsg = GUI.BeginScrollView(new Rect(50, 0, 250, strServerList.Count * height), scrollVecConfigMsg, new Rect(0, 0, 200, 3000));

        for (int i = 0; i < strServerList.Count; i++)
        {
            string strData = (string)strServerList[i];
            nOffest += height;

            if (GUI.Button(new Rect(0, nOffest, 200, 50), strData))
            {
                string[] str = strData.Split(':');
                if (str.Length == 2)
                {
                    strTargetIP = str[0];
                    nTargetPort = int.Parse(str[1]);
                    meToolState = TOOL_STATE.E_TOOL_LOGING;
                }
            }

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
        if (null == netFocus)
        {
            netFocus = new NFNet();
            netFocus.StartConnect(strTargetIP, nTargetPort);
        }
        else
        {
            netFocus.strLoginAccount = GUI.TextField(new Rect(25, nOffest, 150, height), netFocus.strLoginAccount);
            nOffest += height;

            netFocus.strPassword = GUI.TextField(new Rect(25, nOffest, 150, height), netFocus.strPassword);
            nOffest += height;

            if (GUI.Button(new Rect(25, nOffest, 150, height), "Login"))
            {
                if (null != netFocus.client)
                {
                    netFocus.sendLogic.LoginPB(netFocus.strLoginAccount, netFocus.strPassword, "");
                }
            }
            nOffest += height;

            NFTCPClientState eState = netFocus.client.GetState();
            switch (eState)
            {
                case NFTCPClientState.Connecting:
                    GUI.Label(new Rect(25, nOffest, 150, height), "连接中......");
                    break;

                case NFTCPClientState.Connected:
                    GUI.Label(new Rect(25, nOffest, 150, height), "连接成功!");
                    break;

                case NFTCPClientState.Disconnected:
                    {
                        netFocus.mPlayerState = NFNet.PLAYER_STATE.E_WAITING_PLAYER_LOGIN;
                        netFocus = null;
                        GUI.Label(new Rect(25, nOffest, 150, height), "掉线!");
                    }
                    break;
                default:
                    break;
            }


            ////////////////////
//             string strState = netFocus.strLoginAccount + " 连接中...";
//             if (netFocus.mPlayerState == NFNet.PLAYER_STATE.E_PLAYER_LOGINING)
//             {
//                 strState = netFocus.strLoginAccount + " 登录中...";
//             }
//             else if (netFocus.mPlayerState == NFNet.PLAYER_STATE.E_HAS_PLAYER_LOGIN)
//             {
//                 strState = netFocus.strLoginAccount + " 登录成功";
//             }
//             else if (netFocus.mPlayerState == NFNet.PLAYER_STATE.E_HAS_PLAYER_SELECTSERVER)
//             {
//                 strState = netFocus.strLoginAccount + " 获取服务器列表成功";
//                 //展示服务器，选择服务器后自动创建角色
//             }
//             else if (netFocus.mPlayerState == NFNet.PLAYER_STATE.E_HAS_PLAYER_ROLELIST)
//             {
//                 strState = netFocus.strLoginAccount + " 获取角色列表成功";
//                 //展示自动创建了的角色，提供进入游戏按钮
//                 netFocus.nServerID = mConfig.GetServerID();
//             }
//             else if (netFocus.mPlayerState == NFNet.PLAYER_STATE.E_DISCOUNT)
//             {
//                 strState = netFocus.strLoginAccount + " 已掉线,正自动重连...";
//                 netFocus.StartConnect(strTargetIP, nTargetPort);
//             }
//             else if (netFocus.mPlayerState == NFNet.PLAYER_STATE.E_PLAYER_GAMEING)
//             {
//                 strState = netFocus.strLoginAccount + " 已进入游戏。";
//             }

        }
    }

    string strAccountPre = "robot";
    string strAccountStart = "0";
    string strAccountEnd = "50";

    Vector2 scrollRobotLoginState = new Vector2(0, 0);
    Vector2 scrollRobotSelectState = new Vector2(0, 0);
    void Do_DrawWaitingLoginMulti(int windowId)
    {
        int height = 50;
        int width = 150;
        int nOffestWidth = 225;
        int nStartWidth = 25;
        int nOffest = 25;


        GUI.Label(new Rect(nStartWidth, nOffest, width, height), "机器人帐号前缀");
        strAccountPre = GUI.TextField(new Rect(nOffestWidth, nOffest, width, height), strAccountPre);

        nOffest += height;
        GUI.Label(new Rect(nStartWidth, nOffest, width, height), "帐号序号范围");
        strAccountStart = GUI.TextField(new Rect(nOffestWidth, nOffest, width / 2, height), strAccountStart);
        strAccountEnd = GUI.TextField(new Rect(nOffestWidth + width / 2, nOffest, width / 2, height), strAccountEnd);

        int nStartAccountIndex = int.Parse(strAccountStart);
        int nEndAccountIndex = int.Parse(strAccountEnd);

        nOffest += height;
        if (GUI.Button(new Rect(nStartWidth, nOffest, width, height), "开始添加"))
        {
            for (int i = nStartAccountIndex; i < nEndAccountIndex; i++)
            {
                NFNet rbNet = new NFNet();
                netList.Add(rbNet);

                rbNet.strLoginAccount = strAccountPre + i;
                rbNet.StartConnect(strTargetIP, nTargetPort);
            }
        }

        if (netList.Count > 0)
        {
            if (GUI.Button(new Rect(nOffestWidth, nOffest, width, height), "清空"))
            {
                for (int i = 0; i < netList.Count; i++)
                {
                    NFNet rbNet = (NFNet)netList[i];
                    rbNet.client.Disconnect();
                    rbNet = null;
                }

                netList.Clear();
            }
        }

        nOffest += height;
        scrollRobotLoginState = GUI.BeginScrollView(new Rect(height, nOffest, width * 2, 200), scrollRobotLoginState, new Rect(0, 0, 200, netList.Count * height / 2));
        for (int i = 0; i < netList.Count; i++)
        {
            NFNet rbNet = (NFNet)netList[i];

            string strState = rbNet.strLoginAccount + " 连接中...";
            if (rbNet.mPlayerState == NFNet.PLAYER_STATE.E_WAITING_PLAYER_LOGIN)
            {
                if (rbNet.client.IsConnected())
                {
                    strState = rbNet.strLoginAccount + " 连接成功";
                    
                    rbNet.sendLogic.LoginPB(rbNet.strLoginAccount, rbNet.strPassword, "");
                }

            }
            else if (rbNet.mPlayerState == NFNet.PLAYER_STATE.E_PLAYER_LOGINING)
            {
                strState = rbNet.strLoginAccount + " 登录中...";
            }
            else if (rbNet.mPlayerState == NFNet.PLAYER_STATE.E_HAS_PLAYER_LOGIN)
            {
                strState = rbNet.strLoginAccount + " 登录成功";
            }
            else if (rbNet.mPlayerState == NFNet.PLAYER_STATE.E_HAS_PLAYER_SELECTSERVER)
            {
                strState = rbNet.strLoginAccount + " 获取服务器列表成功";
                //展示服务器，选择服务器后自动创建角色
            }
            else if (rbNet.mPlayerState == NFNet.PLAYER_STATE.E_HAS_PLAYER_ROLELIST)
            {
                strState = rbNet.strLoginAccount + " 获取角色列表成功";
                //展示自动创建了的角色，提供进入游戏按钮
                rbNet.nServerID = mConfig.GetServerID();
                ////////////////////////////删除所有角色
                for (int j = 0; j < rbNet.listener.aCharList.Count; ++j)
                {
//                     NFMsg.CharLiteInfo xLiteInfo = (NFMsg.CharLiteInfo)rbNet.listener.aCharList[i];
//                     if (xLiteInfo.delete_flag <= 0)
//                     {
//                         rbNet.sendLogic.RequireDelChar((UInt32)rbNet.nUserID, (UInt32)xLiteInfo.char_id);
//                     }
                }
            }
            else if (rbNet.mPlayerState == NFNet.PLAYER_STATE.E_DISCOUNT)
            {
                strState = rbNet.strLoginAccount + " 已掉线,正自动重连...";
                rbNet.StartConnect(strTargetIP, nTargetPort);
            }
            else if (rbNet.mPlayerState == NFNet.PLAYER_STATE.E_PLAYER_GAMEING)
            {
                strState = rbNet.strLoginAccount + " 已进入游戏。";
            }
            GUI.Label(new Rect(0, height / 2 * i, width * 4, height / 2), strState);
        }
        GUI.EndScrollView();



        if (netList.Count > 0 && null != mConfig)
        {
            nOffest += 200;
            if (GUI.Button(new Rect(nStartWidth, nOffest, width * 2, height), "全部删除角色:" + mConfig.GetServerID()))
            {
                for (int i = 0; i < netList.Count; i++)
                {
                    NFNet rbNet = (NFNet)netList[i];
                    rbNet.nServerID = mConfig.GetServerID();
                    ////////////////////////////删除所有角色
//                     for (int j = 0; j < rbNet.listener.aCharList.Count; ++j)
//                     {
//                         NFMsg.CharLiteInfo xLiteInfo = (NFMsg.CharLiteInfo)rbNet.listener.aCharList[j];
//                         if (xLiteInfo.delete_flag <= 0)
//                         {
//                             rbNet.sendLogic.RequireDelChar((UInt32)rbNet.nUserID, (UInt32)xLiteInfo.char_id);
//                         }
//                     }

                    rbNet.listener.aCharList.Clear();
                }
            }

            nOffest += height;
            if (GUI.Button(new Rect(nStartWidth, nOffest, width * 2, height), "进入主控制界面:" + mConfig.GetServerID()))
            {
                meToolState = TOOL_STATE.E_TOOL_GAMEING;
            }
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
        else if (PLAYER_MODE.E_MUTILE_COUNT == mToolMode)
        {
            int width = 400;
            int height = 600;
            GUI.Window(0, new Rect((Screen.width - width) / 2, (Screen.height - height) / 2, width, height), Do_DrawWaitingLoginMulti, "机器人登录配置");
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Vector2 scrollVecSelectServer = new Vector2();
    void Do_DrawWaitingSelectServerSingle(int windowId)
    {
        int height = 50;
        int nOffest = 0;


        scrollVecSelectServer = GUI.BeginScrollView(new Rect(50, 50, 250, 450), scrollVecSelectServer, new Rect(0, 0, 200, netFocus.listener.aServerList.Count * height));

        for (int i = 0; i < netFocus.listener.aServerList.Count; i++)
        {
            nOffest += height;

            NFMsg.ServerInfo info = (NFMsg.ServerInfo)netFocus.listener.aServerList[i];

            if (GUI.Button(new Rect(0, nOffest, 200, 50), info.name + " " + info.server_id + " " + info.status))
            {
                netFocus.mPlayerState = NFNet.PLAYER_STATE.E_HAS_PLAYER_ROLELIST;

                netFocus.nServerID = info.server_id;

            }


        }

        GUI.EndScrollView();
    }


    void DrawWaitingSelectServer()
    {
        if (PLAYER_MODE.E_SINGLE_PLAYER == mToolMode)
        {
            int width = 300;
            int height = 500;
            GUI.Window(0, new Rect((Screen.width - width) / 2, (Screen.height - height) / 2, width, height), Do_DrawWaitingSelectServerSingle, "选择服务器");
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
//             NFMsg.CharLiteInfo xLiteInfo = (NFMsg.CharLiteInfo)netFocus.listener.aCharList[i];
//             if (xLiteInfo.delete_flag <= 0 && xLiteInfo.base_info.citizenship == netFocus.nServerID)
//             {
//                 string strState = "";
//                 if (netFocus.nSeleroleID == xLiteInfo.char_id)
//                 {
//                     strState = "选中";
//                 }
// 
//                 if (GUI.Button(new Rect(height / 2, nOffest, 200, 50), xLiteInfo.base_info.charName + " " + xLiteInfo.char_id + " " + strState))
//                 {
//                     netFocus.nSeleroleID = (uint)(xLiteInfo.char_id);
//                 }
// 
//                 nOffest += height;
//             }
        }


        GUI.EndScrollView();

        netFocus.stCreaterRoleName = GUI.TextField(new Rect(400 / 2 + 75, 50, 200, 50), netFocus.stCreaterRoleName);

        if (netFocus.stCreaterRoleName.Length > 0)
        {
            if (GUI.Button(new Rect(400 / 2 + 75, 100, 200, 50), "创建角色"))
            {
                netFocus.sendLogic.RequireCreateChar(netFocus.stCreaterRoleName, 0, 1, (UInt32)netFocus.nServerID);
            }
        }
        else
        {
            if (netFocus.nSeleroleID > 0)
            {
                if (GUI.Button(new Rect(400 / 2 + 75, 200, 200, 50), "删除角色"))
                {
                    netFocus.sendLogic.RequireDelChar(netFocus.nSeleroleID);
                    netFocus.nSeleroleID = 0;
                }

                if (GUI.Button(new Rect(300 / 2, 400, 200, 50), "进入游戏"))
                {
                    netFocus.sendLogic.RequireEnterGameServer(netFocus.nServerID, netFocus.nSeleroleID);
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
        else
        {

        }
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    void Do_DrawGameingSingle(int windowId)
    {
        netFocus.OnGUI(768, 1024);
    }

    private Vector2 scrollPositionRobot = Vector2.zero;
    private Vector2 scrollVecBtn = Vector2.zero;
    void Do_DrawGameingMulti(int windowId)
    {
        int nCol = 8;
        int nRow = netList.Count;
        int nOffestCol = 100;
        int nOffestRow = 20;

        scrollPositionRobot = GUI.BeginScrollView(new Rect(0, 20, 500, 600), scrollPositionRobot, new Rect(0, 0, (nCol + 1) * nOffestCol, nOffestRow * (nRow + 1)));
        GUI.Button(new Rect(0 * nOffestCol, 0, nOffestCol, nOffestRow), "帐号");
        GUI.Button(new Rect(1 * nOffestCol, 0, nOffestCol, nOffestRow), "角色ID");
        GUI.Button(new Rect(2 * nOffestCol, 0, nOffestCol, nOffestRow), "连接状态");
        GUI.Button(new Rect(3 * nOffestCol, 0, nOffestCol*2, nOffestRow), "角色状态");
        GUI.Button(new Rect(5 * nOffestCol, 0, nOffestCol, nOffestRow), "场景ID");

        for (int i = 0; i < nRow; ++i)
        {
            NFNet rbNet = (NFNet)netList[i];

            GUI.Button(new Rect(0 * nOffestCol, (i+1) * nOffestRow, nOffestCol, nOffestRow), rbNet.strLoginAccount);
            GUI.Button(new Rect(1 * nOffestCol, (i+1) * nOffestRow, nOffestCol, nOffestRow), rbNet.nSeleroleID.ToString());
            GUI.Button(new Rect(2 * nOffestCol, (i+1) * nOffestRow, nOffestCol, nOffestRow), rbNet.client.GetState().ToString());
            GUI.Button(new Rect(3 * nOffestCol, (i+1) * nOffestRow, nOffestCol*2, nOffestRow), rbNet.mPlayerState.ToString());
            GUI.Button(new Rect(5 * nOffestCol, (i+1) * nOffestRow, nOffestCol, nOffestRow), rbNet.kernel.QueryPropertyInt(new NFCoreEx.NFIDENTID(rbNet.nSeleroleID), "SceneID").ToString());
//             GUI.Button(new Rect(4 * nOffestCol, i * nOffestRow, nOffestCol, nOffestRow), col.ToString() + "  [" + record.GetColType(col) + "]");
//             GUI.Button(new Rect(5 * nOffestCol, i * nOffestRow, nOffestCol, nOffestRow), col.ToString() + "  [" + record.GetColType(col) + "]");
//             GUI.Button(new Rect(6 * nOffestCol, i * nOffestRow, nOffestCol, nOffestRow), col.ToString() + "  [" + record.GetColType(col) + "]");
//             GUI.Button(new Rect(7 * nOffestCol, i * nOffestRow, nOffestCol, nOffestRow), col.ToString() + "  [" + record.GetColType(col) + "]");


        }

        GUI.EndScrollView();

        //操作功能区
        scrollVecBtn = GUI.BeginScrollView(new Rect(500, 20, 350, 600), scrollVecBtn, new Rect(0, 0, 600, 3000));
        if (GUI.Button(new Rect(0, 0, 100, 50), "切换场景"))
        {
            for (int i = 0; i < netList.Count; ++i)
            {
                NFNet rbNet = (NFNet)netList[i];
                //切换场景
                rbNet.sendLogic.RequireSwapScene(rbNet.nSeleroleID, 0, uint.Parse(strReqSwapSceneID), int.Parse(strReqSwapSceneLine));
            }

            
        }
        strReqSwapSceneID = GUI.TextField(new Rect(100, 0, 100, 50), strReqSwapSceneID);
        strReqSwapSceneLine = GUI.TextField(new Rect(200, 0, 100, 50), strReqSwapSceneLine);



        if (GUI.Button(new Rect(0, 50, 100, 50), "移动"))
        {
            //移动
            for (int i = 0; i < netList.Count; ++i)
            {
                NFNet rbNet = (NFNet)netList[i];
                rbNet.sendLogic.RequireMove(rbNet.nSeleroleID, rbNet.nSeleroleID, float.Parse(strReqMoveX), float.Parse(strReqMoveZ));
            }
        }
        strReqMoveX = GUI.TextField(new Rect(100, 50, 100, 50), strReqMoveX);
        strReqMoveZ = GUI.TextField(new Rect(200, 50, 100, 50), strReqMoveZ);

        if (GUI.Button(new Rect(0, 100, 100, 50), "杀怪"))
        {
            for (int i = 0; i < netList.Count; ++i)
            {
                NFNet rbNet = (NFNet)netList[i];

                //杀所有怪
                rbNet.sendLogic.RequireSkill(rbNet.nSeleroleID, strReqKillID, long.Parse(strReqKillNPCID));
            }
        }
        strReqKillID = GUI.TextField(new Rect(100, 100, 100, 50), strReqKillID);
        strReqKillNPCID = GUI.TextField(new Rect(200, 100, 100, 50), strReqKillNPCID);


        if (GUI.Button(new Rect(0, 150, 100, 50), "结算副本"))
        {
            for (int i = 0; i < netList.Count; ++i)
            {
                NFNet rbNet = (NFNet)netList[i];
                //结算
                rbNet.sendLogic.RequirePullDownCustom(rbNet.nSeleroleID, int.Parse(strPullCustom));
            }
        }
        strPullCustom = GUI.TextField(new Rect(100, 150, 100, 50), strPullCustom);


        if (GUI.Button(new Rect(0, 200, 100, 50), "设置属性"))
        {
            for (int i = 0; i < netList.Count; ++i)
            {
                NFNet rbNet = (NFNet)netList[i];
                //无敌设置属性
                rbNet.sendLogic.RequireProperty(rbNet.nSeleroleID, strReqSetProperty, int.Parse(strReqPropertyValue));
            }
        }

        strReqSetProperty = GUI.TextField(new Rect(100, 200, 100, 50), strReqSetProperty);
        strReqPropertyValue = GUI.TextField(new Rect(200, 200, 100, 50), strReqPropertyValue);

        if (GUI.Button(new Rect(0, 250, 100, 50), "添加道具"))
        {
            for (int i = 0; i < netList.Count; ++i)
            {
                NFNet rbNet = (NFNet)netList[i];
                //添加道具
                rbNet.sendLogic.RequireItem(rbNet.nSeleroleID, strReqAddItem, uint.Parse(strReqAddCount));
            }
        }
        strReqAddItem = GUI.TextField(new Rect(100, 250, 100, 50), strReqAddItem);
        strReqAddCount = GUI.TextField(new Rect(200, 250, 100, 50), strReqAddCount);
        

        //聊天
        if (GUI.Button(new Rect(0, 300, 100, 50), "聊天"))
        {
            for (int i = 0; i < netList.Count; ++i)
            {
                NFNet rbNet = (NFNet)netList[i];
                rbNet.sendLogic.RequireChat(rbNet.nSeleroleID, uint.Parse(strChatTargetID), int.Parse(strType), strChatData);
            }
        }
        strChatTargetID = GUI.TextField(new Rect(100, 300, 100, 50), strChatTargetID);
        strType = GUI.TextField(new Rect(200, 300, 100, 50), strType);
        strChatData = GUI.TextField(new Rect(300, 300, 100, 50), strChatData);


        GUI.EndScrollView();
    }

    void DrawGameing()
    {
        if (PLAYER_MODE.E_SINGLE_PLAYER == mToolMode)
        {
            int width = 1024;
            int height = 768;
            string strTitle = "角色正在游戏中 " + netFocus.nSeleroleID;
            GUI.Window(0, new Rect((Screen.width - width) / 2, (Screen.height - height) / 2, width, height), Do_DrawGameingSingle, strTitle);
        }
        else
        {

            RobotAI();

            int width = 1024;
            int height = 768;
            string strTitle = "机器人正在运行，数量:" + netList.Count;
            GUI.Window(0, new Rect((Screen.width - width) / 2, (Screen.height - height) / 2, width, height), Do_DrawGameingMulti, strTitle);
        }
    }


}
