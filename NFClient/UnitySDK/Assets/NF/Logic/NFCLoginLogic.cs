using System;
using System.Linq;
using System.Text;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using ProtoBuf;
using NFMsg;
using UnityEngine;

namespace NFSDK
{
    public class NFCLoginLogic : NFCLogicBase
    {
        public enum Event : int
        {
            LoginSuccess,
            LoginFailure,
            WorldList,
            ServerList,
            SelectServerSuccess,
        };

        private static NFCLoginLogic _instance = null;
        public static NFCLoginLogic Instance()
        {
            return _instance;
        }
        public NFCLoginLogic(NFIPluginManager pluginManager)
        {
            _instance = this;
            mPluginManager = pluginManager;
		}

        public override bool AfterInit()
        {
            NFCNetLogic.Instance().AddReceiveCallBack(NFMsg.EGameMsgID.EGMI_ACK_LOGIN, OnLoginProcess);
            NFCNetLogic.Instance().AddReceiveCallBack(NFMsg.EGameMsgID.EGMI_ACK_WORLD_LIST, OnWorldList);
            NFCNetLogic.Instance().AddReceiveCallBack(NFMsg.EGameMsgID.EGMI_ACK_CONNECT_WORLD, OnConnectWorld);
            NFCNetLogic.Instance().AddReceiveCallBack(NFMsg.EGameMsgID.EGMI_ACK_CONNECT_KEY, OnConnectKey);
            NFCNetLogic.Instance().AddReceiveCallBack(NFMsg.EGameMsgID.EGMI_ACK_SELECT_SERVER, OnSelectServer);

            return true;
        }
        
        // 请求消息
	    public void LoginPB(string strAccount, string strPwd, string strKey)
        {
            Debug.Log("LoginPB:" + strAccount);
            NFMsg.ReqAccountLogin xData = new NFMsg.ReqAccountLogin();
            xData.account = System.Text.Encoding.Default.GetBytes(strAccount);
            xData.password = System.Text.Encoding.Default.GetBytes(strPwd);
            xData.security_code = System.Text.Encoding.Default.GetBytes(strKey);
            xData.signBuff = System.Text.Encoding.Default.GetBytes("");
            xData.clientVersion = 1;
            xData.loginMode = 0;
            xData.clientIP = 0;
            xData.clientMAC = 0;
            xData.device_info = System.Text.Encoding.Default.GetBytes("");
            xData.extra_info = System.Text.Encoding.Default.GetBytes("");

            mAccount = strAccount;

            MemoryStream stream = new MemoryStream();
            Serializer.Serialize<NFMsg.ReqAccountLogin>(stream, xData);

            NFCNetLogic.Instance().SendToServerByPB(NFMsg.EGameMsgID.EGMI_REQ_LOGIN, stream);
        }
	    public void RequireWorldList()
        {
            NFMsg.ReqServerList xData = new NFMsg.ReqServerList();
            xData.type = NFMsg.ReqServerListType.RSLT_WORLD_SERVER;

            MemoryStream stream = new MemoryStream();
            Serializer.Serialize<NFMsg.ReqServerList>(stream, xData);

            NFCNetLogic.Instance().SendToServerByPB(NFMsg.EGameMsgID.EGMI_REQ_WORLD_LIST, stream);
        }
	    public void RequireConnectWorld(int nWorldID)
        {
            NFMsg.ReqConnectWorld xData = new NFMsg.ReqConnectWorld();
            xData.world_id = nWorldID;

            MemoryStream stream = new MemoryStream();
            Serializer.Serialize<NFMsg.ReqConnectWorld>(stream, xData);

            NFCNetLogic.Instance().SendToServerByPB(NFMsg.EGameMsgID.EGMI_REQ_CONNECT_WORLD, stream);
        }
	    public void RequireVerifyWorldKey(string strAccount, string strKey)
        {
            NFMsg.ReqAccountLogin xData = new NFMsg.ReqAccountLogin();
            xData.account = System.Text.Encoding.Default.GetBytes(strAccount);
            xData.password = System.Text.Encoding.Default.GetBytes("");
            xData.security_code = System.Text.Encoding.Default.GetBytes(strKey);
            xData.signBuff = System.Text.Encoding.Default.GetBytes("");
            xData.clientVersion = 1;
            xData.loginMode = 0;
            xData.clientIP = 0;
            xData.clientMAC = 0;
            xData.device_info = System.Text.Encoding.Default.GetBytes("");
            xData.extra_info = System.Text.Encoding.Default.GetBytes("");

            MemoryStream stream = new MemoryStream();
            Serializer.Serialize<NFMsg.ReqAccountLogin>(stream, xData);

            NFCNetLogic.Instance().SendToServerByPB(NFMsg.EGameMsgID.EGMI_REQ_CONNECT_KEY, stream);
        }
	    public void RequireServerList()
        {
            NFMsg.ReqServerList xData = new NFMsg.ReqServerList();
            xData.type = NFMsg.ReqServerListType.RSLT_GAMES_ERVER;

            MemoryStream stream = new MemoryStream();
            Serializer.Serialize<NFMsg.ReqServerList>(stream, xData);

            NFCNetLogic.Instance().SendToServerByPB(NFMsg.EGameMsgID.EGMI_REQ_WORLD_LIST, stream);
        }
	    public void RequireSelectServer(int nServerID)
        {
            NFMsg.ReqSelectServer xData = new NFMsg.ReqSelectServer();
            xData.world_id = nServerID;
            mServerID = nServerID;
            MemoryStream stream = new MemoryStream();
            Serializer.Serialize<NFMsg.ReqSelectServer>(stream, xData);

            NFCNetLogic.Instance().SendToServerByPB(NFMsg.EGameMsgID.EGMI_REQ_SELECT_SERVER, stream);
        }
        // 接收消息
	    private void OnLoginProcess(UInt16 id, MemoryStream stream)
        {
            Debug.Log("OnLoginProcess1");
            NFMsg.MsgBase xMsg = new NFMsg.MsgBase();
            xMsg = Serializer.Deserialize<NFMsg.MsgBase>(stream);

            Debug.Log("OnLoginProcess2");
            NFMsg.AckEventResult xData = new NFMsg.AckEventResult();
            xData = Serializer.Deserialize<NFMsg.AckEventResult>(new MemoryStream(xMsg.msg_data));

            Debug.Log("OnLoginProcess3");
            if (EGameEventCode.EGEC_ACCOUNT_SUCCESS == xData.event_code)
            {
                Debug.Log("Login  SUCCESS");
                DoEvent((int)Event.LoginSuccess);
            }
            else
            {
                Debug.Log("Login Faild,Code: " + xData.event_code);
                NFCDataList varList = new NFCDataList();
                varList.AddInt((Int64)xData.event_code);
                DoEvent((int)Event.LoginFailure);
            }
        }
        private void OnWorldList(UInt16 id, MemoryStream stream)
        {
            NFMsg.MsgBase xMsg = new NFMsg.MsgBase();
            xMsg = Serializer.Deserialize<NFMsg.MsgBase>(stream);

            NFMsg.AckServerList xData = new NFMsg.AckServerList();
            xData = Serializer.Deserialize<NFMsg.AckServerList>(new MemoryStream(xMsg.msg_data));

            if (ReqServerListType.RSLT_WORLD_SERVER == xData.type)
            {
                for (int i = 0; i < xData.info.Count; ++i)
                {
                    ServerInfo info = xData.info[i];
                    Debug.Log("WorldList  ServerId: " + info.server_id + " Name: " + System.Text.Encoding.Default.GetString(info.name) + " Status: " + info.status);
                    mWorldServerList.Add(info);
                }
                DoEvent((int)Event.WorldList);
            }
            else if (ReqServerListType.RSLT_GAMES_ERVER == xData.type)
            {
                for (int i = 0; i < xData.info.Count; ++i)
                {
                    ServerInfo info = xData.info[i];
                    Debug.Log("GameList  ServerId: " + info.server_id + " Name: " + System.Text.Encoding.Default.GetString(info.name) + " Status: " + info.status);
                    mGameServerList.Add(info);
                }
                DoEvent((int)Event.ServerList);
            }
        }
        private void OnConnectWorld(UInt16 id, MemoryStream stream)
        {
            NFMsg.MsgBase xMsg = new NFMsg.MsgBase();
            xMsg = Serializer.Deserialize<NFMsg.MsgBase>(stream);

            NFMsg.AckConnectWorldResult xData = new NFMsg.AckConnectWorldResult();
            
            xData = Serializer.Deserialize<NFMsg.AckConnectWorldResult>(new MemoryStream(xMsg.msg_data));

            NFCNetLogic.Instance().Shut();
            NFCNetLogic.Instance().ConnectServer(System.Text.Encoding.Default.GetString(xData.world_ip), (ushort)xData.world_port);
            mKey = System.Text.Encoding.Default.GetString(xData.world_key);

            RequireVerifyWorldKey(mAccount, mKey);
        }
        private void OnConnectKey(UInt16 id, MemoryStream stream)
        {
            NFMsg.MsgBase xMsg = new NFMsg.MsgBase();
            xMsg = Serializer.Deserialize<NFMsg.MsgBase>(stream);

            NFMsg.AckEventResult xData = new NFMsg.AckEventResult();
            xData = Serializer.Deserialize<NFMsg.AckEventResult>(new MemoryStream(xMsg.msg_data));

            if (xData.event_code == EGameEventCode.EGEC_VERIFY_KEY_SUCCESS)
            {
                Debug.Log("VerifyKey SUCCESS");
                RequireServerList();
            }
            else
            {
                Debug.Log("VerifyKey Failed");
            }
        }
        private void OnSelectServer(UInt16 id, MemoryStream stream)
        {
            NFMsg.MsgBase xMsg = new NFMsg.MsgBase();
            xMsg = Serializer.Deserialize<NFMsg.MsgBase>(stream);

            NFMsg.AckEventResult xData = new NFMsg.AckEventResult();
            xData = Serializer.Deserialize<NFMsg.AckEventResult>(new MemoryStream(xMsg.msg_data));

            if (xData.event_code == EGameEventCode.EGEC_SELECTSERVER_SUCCESS)
            {
                Debug.Log("SelectGame SUCCESS ");
                DoEvent((int)Event.SelectServerSuccess);
            }
            else
            {
                Debug.Log("SelectGame Failed ");
            }
        }

        public string mAccount;
        public string mKey;
        public int mServerID;
        public ArrayList mWorldServerList = new ArrayList();
        public ArrayList mGameServerList = new ArrayList();
    };
}