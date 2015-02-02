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
using NFMsg;
using ProtoBuf;

public class NFStart : MonoBehaviour
{
    //看开启多人模式还是单人模式



    NFConfig mConfig = null;
    NFNet mxNetFocus = null;
    string strTargetIP = "";
    int nPort = 0;
    public bool bCommand = false;
    public bool bDebugMode = false;

    public Transform[] mTrans;

    public NFNet GetFocusNet()
    {
        return mxNetFocus;
    }

    public void SetFocusNet(NFNet xNet)
    {
        mxNetFocus = xNet;
    }

    public NFBinarySendLogic GetFocusSender()
    {
        if (null != mxNetFocus)
        {
            return mxNetFocus.mxSendLogic;
        }

        return null;
    }

    public NFCoreExListener GetFocusListener()
    {
        if (null != mxNetFocus)
        {
            return mxNetFocus.mxListener;
        }

        return null;
    }

    #region Instance
    private static NFStart _Instance = null;
    public static NFStart Instance
    {
        get
        {
            return _Instance;
        }

    }
    #endregion



    void Awake()
    {
        _Instance = this;

        foreach (Transform trans in mTrans)
        {
            if (null != trans)
            {
                trans.gameObject.SetActive(true);
            }
        }


    }

    // Use this for initialization
    void Start()
    {
        mConfig = new NFConfig();
        mConfig.Load();
        mConfig.GetSelectServer(ref strTargetIP, ref nPort);

        NFCElementManager.Instance.Load();
        NFCRenderInterface.Instance.Init();

    }

    void OnDestroy()
    {

        if (null != mxNetFocus)
        {
            mxNetFocus.Destroy();
        }
    }

    void OnGUI()
    {

        if (null != mxNetFocus)
        {
            mxNetFocus.Update();
            mxNetFocus.OnGUI(1024, 768);
        }

        if (null != mxNetFocus)
        {
            switch (mxNetFocus.mPlayerState)
            {
                case NFNet.PLAYER_STATE.E_NONE:
                    {
                        if (strTargetIP.Length > 0)
                        {
                            mxNetFocus.StartConnect(strTargetIP, nPort);
                            mxNetFocus.mPlayerState = NFNet.PLAYER_STATE.E_WAITING_PLAYER_LOGIN;
                        }
                    }

                    break;
                case NFNet.PLAYER_STATE.E_WAITING_PLAYER_LOGIN:
                    {
                        if (mxNetFocus.strKey.Length > 0)
                        {
                            mxNetFocus.mPlayerState = NFNet.PLAYER_STATE.E_HAS_PLAYER_GATE;
                        }
                        else
                        {
                            mxNetFocus.strAccount = GUI.TextField(new Rect(10, 10, 150, 50), mxNetFocus.strAccount);
                            mxNetFocus.strPassword = GUI.TextField(new Rect(10, 100, 150, 50), mxNetFocus.strPassword);
                            if (GUI.Button (new Rect (10, 200, 150, 50), "Login"))
                            {
                                mxNetFocus.mxSendLogic.LoginPB(mxNetFocus.strAccount, mxNetFocus.strPassword, "");
                            }
                        }
                    }
                    break;

                case NFNet.PLAYER_STATE.E_HAS_PLAYER_LOGIN:
                    {
                        int nHeight = 50;
                        for (int i = 0; i < mxNetFocus.mxListener.aWorldList.Count; ++i )
                        {
                            ServerInfo xInfo = (ServerInfo)mxNetFocus.mxListener.aWorldList[i];
                            if (GUI.Button(new Rect(10, i * nHeight, 150, nHeight), System.Text.Encoding.Default.GetString(xInfo.name)))
                            {
                                NFStart.Instance.GetFocusNet().nServerID = xInfo.server_id;
                                mxNetFocus.mxSendLogic.RequireConnectWorld(xInfo.server_id);
                            }
                        }
                    }
                    break;

                case NFNet.PLAYER_STATE.E_WAITING_PLAYER_TO_GATE:
                    {
                        string strWorpdIP = NFStart.Instance.GetFocusNet().strWorldIP;
                        string strWorpdKey = NFStart.Instance.GetFocusNet().strKey;
                        string strAccount = NFStart.Instance.GetFocusNet().strKey;
                        int nPort = NFStart.Instance.GetFocusNet().nWorldPort;

                        NFNet xNet = new NFNet();
                        xNet.strWorldIP = strWorpdIP;
                        xNet.strKey = strWorpdKey;
                        xNet.strAccount = strAccount;
                        xNet.nWorldPort = nPort;

                        xNet.mPlayerState = NFNet.PLAYER_STATE.E_START_CONNECT_TO_GATE;
                        xNet.StartConnect(xNet.strWorldIP, nPort);
                        NFStart.Instance.SetFocusNet(xNet);
                    }
                    break;
                case NFNet.PLAYER_STATE.E_HAS_PLAYER_SELECT_WORLD:
                case NFNet.PLAYER_STATE.E_HAS_PLAYER_GATE:
                    {
                        NFStart.Instance.GetFocusNet().mxSendLogic.RequireVerifyWorldKey(NFStart.Instance.GetFocusNet().strAccount, NFStart.Instance.GetFocusNet().strKey);
                        NFStart.Instance.GetFocusNet().mPlayerState = NFNet.PLAYER_STATE.E_WATING_VERIFY;
                    }
                    break;
                case NFNet.PLAYER_STATE.E_HAS_VERIFY:
                    {

                        int nWidth = 200;
                        for (int i = 0; i < mxNetFocus.mxListener.aServerList.Count; ++i)
                        {
                            ServerInfo xInfo = (ServerInfo)mxNetFocus.mxListener.aServerList[i];
                            if (GUI.Button(new Rect(nWidth, i * 50, 150, 50), System.Text.Encoding.Default.GetString(xInfo.name)))
                            {
                                NFStart.Instance.GetFocusNet().nServerID = xInfo.server_id;
                                NFStart.Instance.GetFocusSender().RequireSelectServer(xInfo.server_id);
                            }
                        }
                    }
                    break;

                case NFNet.PLAYER_STATE.E_HAS_PLAYER_ROLELIST:
                    {
                        if (mxNetFocus.mxListener.aCharList.Count > 0)
                        {
                            for (int i = 0; i < mxNetFocus.mxListener.aCharList.Count; ++i)
                            {
                                NFMsg.RoleLiteInfo xLiteInfo = (NFMsg.RoleLiteInfo)mxNetFocus.mxListener.aCharList[i];
                                if (GUI.Button(new Rect(200, i * 50, 150, 50), System.Text.Encoding.Default.GetString(xLiteInfo.noob_name)))
                                {
                                    NFStart.Instance.GetFocusNet().nMainRoleID = NFTCPClient.NFCoreExListener.PBToNF(xLiteInfo.id);
                                    mxNetFocus.mxSendLogic.RequireEnterGameServer(NFStart.Instance.GetFocusNet().nMainRoleID, mxNetFocus.strAccount, mxNetFocus.strRoleName, mxNetFocus.nServerID);
                                }
                            }
                            
                        }
                        else
                        {
                            mxNetFocus.strRoleName = GUI.TextField(new Rect(10, 10, 150, 50), mxNetFocus.strRoleName);
                            if (GUI.Button(new Rect(10, 200, 150, 50), "CreateRole"))
                            {
                                mxNetFocus.mxSendLogic.RequireCreateRole(mxNetFocus.strAccount, mxNetFocus.strRoleName, 0, 0, mxNetFocus.nServerID);
                            }
                        }
                    }
                    break;

                case NFNet.PLAYER_STATE.E_PLAYER_GAMEING:
                    //NFCSectionManager.Instance.SetGameState(NFCSectionManager.UI_SECTION_STATE.UISS_GAMEING);
                    break;

                default:
                    break;

            }
        }
        else
        {
            mxNetFocus = new NFNet();
        }
    }
}
