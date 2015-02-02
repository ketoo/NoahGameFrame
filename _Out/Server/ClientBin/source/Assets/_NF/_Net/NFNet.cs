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
using NFCoreEx;

public class NFNet
{
    public NFObjectElement mxObjectElement = null;
    public NFClientNet mxClient = null;
    public NFCoreExListener mxListener = null;
    public NFBinarySendLogic mxSendLogic = null;
    public NFCResultLogic mxResultLogic = null;
    public NFBinraryMessageEvent mxBinMsgEvent = null;


    public enum PLAYER_STATE
    {
        E_NONE,//等待选择服务器登录
        E_WAITING_PLAYER_LOGIN,//等待登录(已经连接成功)
        E_PLAYER_LOGINING,//登录中
        E_HAS_PLAYER_LOGIN,//登录成功
        E_HAS_PLAYER_SELECT_WORLD,//已选择服务器
        E_WAITING_PLAYER_TO_GATE,//等待连接网关
        E_START_CONNECT_TO_GATE,//已开始连接网关
        E_HAS_PLAYER_GATE,//已连接gate,没验证KEY
        E_WATING_VERIFY,//已连接gate(等待验证KEY)
        E_HAS_VERIFY,//验证成功
        E_HAS_PLAYER_ROLELIST,//有了角色列表
        E_PLAYER_WAITING_TO_GAME,//申请了进游戏，等待进入
        E_PLAYER_GAMEING,//游戏中
        E_DISCOUNT,//掉线

    };

    public NFNet()
    {
    }

    public void StartConnect(string strIP, int nPort)
    {
        mxObjectElement = new NFObjectElement();

        mxClient = new NFClientNet(this);        
        mxBinMsgEvent = new NFBinraryMessageEvent(this);
        mxSendLogic = new NFBinarySendLogic(this);
        mxResultLogic = new NFCResultLogic(this);
        mxListener = new NFCoreExListener(this);
        mxListener.Init();

        mxClient.Connect(strIP, nPort);

    }



    /////////////Logic///////////////////////////////////////////////
    public string strReqSwapSceneID = "1";

    public string strReqMoveX = "0.0";
    public string strReqMoveZ = "0.0";

    public string strReqAcceptTaskID = "taskid";

    public string strReqKillNPCID = "npcid";
    public string strReqUseItemID = "itemid";

    public string strPickUpItemID = "0";

    public string strReqSetProperty = "property";
    public string strReqPropertyValue = "value";

    public string strReqAddItem = "additem";
    public string strAddCount= "count";

    public string strSwapOrigin = "swaporigin";
    public string strSwapTarget = "target";

    public string strChatTargetID = "target";
    public string strType = "0";
    public string strChatData = "data";

    Vector2 scrollVecChatMsg = new Vector2();
    Vector2 scrollVecMsg = new Vector2();
    Vector2 scrollVecBtn = new Vector2();
    ////////////////////////////////////////////////////////////
    void OnOpratorGUI(int nHeight, int nWidth)
    {
        //////////////////////////////////

        if (null != mxClient
            && mxClient.IsConnected()
            && mPlayerState == PLAYER_STATE.E_PLAYER_GAMEING)
        {

            ////聊天
            scrollVecChatMsg = GUI.BeginScrollView(new Rect(0, nHeight / 2 + 20, 150 * 1.5f + 40, nHeight / 2 - 40), scrollVecChatMsg, new Rect(0, 0, 1500, 3000));
            int nChatIndex = 0;
            for (int i = mxListener.aChatMsgList.Count - 1; i >= 0 ; i--)
            {
                string strData = (string)mxListener.aChatMsgList[i];
                GUI.Label(new Rect(0, nChatIndex * 20, 2000, 20), strData);
                nChatIndex++;
            }

            GUI.EndScrollView();

            ////网络消息
            scrollVecMsg = GUI.BeginScrollView(new Rect(150 * 1.5f + 40, nHeight / 2 + 20, 150 * 2, nHeight / 2 - 40), scrollVecMsg, new Rect(0, 0, 1500, 3000));
            int nNetIndex = 0;
            for (int i = mxListener.aMsgList.Count - 1; i >= 0; i--)
            {

                string strData = (string)mxListener.aMsgList[i];
                GUI.Label(new Rect(0, nNetIndex * 20, 2000, 20), strData);
                nNetIndex++;
            }

            GUI.EndScrollView();

            //操作功能区
            scrollVecBtn = GUI.BeginScrollView(new Rect(570, 20, 350, nHeight-40), scrollVecBtn, new Rect(0, 0, 600, 3000));

            ////////////////////////////////////////////////////////////////////////////////////////////////

            if (GUI.Button(new Rect(0, 0, 100, 50), "切换场景"))
            {
                mxSendLogic.RequireSwapScene(nMainRoleID, 0, int.Parse(strReqSwapSceneID), -1);
            }
            strReqSwapSceneID = GUI.TextField(new Rect(100, 0, 100, 50), strReqSwapSceneID);
        
            ////////////////////////////////////////////////////////////////////////////////////////////////
            if (GUI.Button(new Rect(0, 50, 100, 50), "移动"))
            {
                mxSendLogic.RequireMove(nMainRoleID, float.Parse(strReqMoveX), float.Parse(strReqMoveZ));
            }
            strReqMoveX = GUI.TextField(new Rect(100, 50, 100, 50), strReqMoveX);
            strReqMoveZ = GUI.TextField(new Rect(200, 50, 100, 50), strReqMoveZ);

            ////////////////////////////////////////////////////////////////////////////////////////////////
            /*

                        if (GUI.Button(new Rect(0, 100, 100, 50), "接任务"))
                        {
                            sendLogic.RequireAcceptTask(strReqAcceptTaskID);
                        }
                        strReqAcceptTaskID = GUI.TextField(new Rect(100, 100, 100, 50), strReqAcceptTaskID);

                        ////////////////////////////////////////////////////////////////////////////////////////////////

                        if (GUI.Button(new Rect(0, 150, 100, 50), "交任务"))
                        {
                            sendLogic.RequireCompeleteTask(nSelfID, strReqAcceptTaskID);
                        }
                        strReqAcceptTaskID = GUI.TextField(new Rect(100, 150, 100, 50), strReqAcceptTaskID);

                        ////////////////////////////////////////////////////////////////////////////////////////////////
            
                        if (GUI.Button(new Rect(0, 200, 100, 50), "杀怪"))
                        {
                            sendLogic.RequireUseSkill("", long.Parse(strReqKillNPCID));
                        }
                        strReqKillNPCID = GUI.TextField(new Rect(100, 200, 100, 50), strReqKillNPCID);
            
                        ////////////////////////////////////////////////////////////////////////////////////////////////
                        if (GUI.Button(new Rect(0, 250, 100, 50), "使用道具"))
                        {
                            sendLogic.RequireUseItem(long.Parse(strReqUseItemID), nSelfID);
                        }
                        strReqUseItemID = GUI.TextField(new Rect(100, 250, 100, 50), strReqUseItemID);

                        ////////////////////////////////////////////////////////////////////////////////////////////////

                        if (GUI.Button(new Rect(0, 300, 100, 50), "拾取"))
                        {
                            sendLogic.RequirePickUpItem(long.Parse(strPickUpItemID));
                        }
                        strPickUpItemID = GUI.TextField(new Rect(100, 300, 100, 50), strPickUpItemID);

                        ////////////////////////////////////////////////////////////////////////////////////////////////
                        */
        
            if (GUI.Button(new Rect(0, 350, 100, 50), "设置属性"))
            {
                mxSendLogic.RequireProperty(nMainRoleID, strReqSetProperty, int.Parse(strReqPropertyValue));
            }
            strReqSetProperty = GUI.TextField(new Rect(100, 350, 100, 50), strReqSetProperty);
            strReqPropertyValue = GUI.TextField(new Rect(200, 350, 100, 50), strReqPropertyValue);
            ////////////////////////////////////////////////////////////////////////////////////////////////
            /*
            if (GUI.Button(new Rect(0, 400, 100, 50), "添加道具"))
            {
                sendLogic.RequireItem(strReqAddItem, int.Parse(strAddCount));
            }
            strReqAddItem = GUI.TextField(new Rect(100, 400, 100, 50), strReqAddItem);
            strAddCount = GUI.TextField(new Rect(200, 400, 100, 50), strAddCount);
*/

            ////////////////////////////////////////////////////////////////////////////////////////////////

            if (GUI.Button(new Rect(0, 450, 100, 50), "聊天"))
            {
                mxSendLogic.RequireChat(nMainRoleID, nTarget, int.Parse(strType), strChatData);
            }
            strChatTargetID = nTarget.ToString();
            strChatTargetID = GUI.TextField(new Rect(100, 450, 100, 50), strChatTargetID);
            strType = GUI.TextField(new Rect(200, 450, 100, 50), strType);
            strChatData = GUI.TextField(new Rect(300, 450, 100, 50), strChatData);
            GUI.EndScrollView();
        }
    }

    public void OnGUI(int nHeight, int nWidth)
    {
        if (null != mxObjectElement)
        {
            mxObjectElement.OnGUI(NFCKernel.Instance, nHeight, nWidth);
        }
        OnOpratorGUI(nHeight, nWidth);

    }

    // Update is called once per frame
    float fTime = 0.0f;
    public void Update()
    {
        if (null != mxObjectElement)
        {
            nTarget = mxObjectElement.xTargetIdent;
        }

        if (null != mxClient)
        {
            mxClient.Update();
            fTime += Time.deltaTime;

            if (mxClient.IsConnected())
            {
                if (fTime > 10.0f)
                {
                    fTime = 0.0f;
                    mxSendLogic.RequireHeartBeat(nMainRoleID);
                }
            }
        }
    }

    public void Destroy()
    {
        if (null != mxClient)
        {
            mxClient.Disconnect();
        }
    }

    public string strWorldIP = "";
    public int nWorldPort = 0;
    public string strKey = "";

    public string strAccount = "server1";
    public string strPassword = "123456";
    public string strRoleName = "";

    public int nServerID = 0;
    public NFIDENTID nMainRoleID = new NFIDENTID();//主角ID

    public NFIDENTID nTarget = new NFIDENTID();
    public Int64 nSceneID = 0;
    public Int64 nLineID = 0;
    public PLAYER_STATE mPlayerState = NFNet.PLAYER_STATE.E_NONE;
}
