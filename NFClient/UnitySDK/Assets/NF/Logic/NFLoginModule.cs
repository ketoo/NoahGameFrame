using System;
using System.Linq;
using System.Text;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using NFMsg;
using UnityEngine;
using Google.Protobuf;

namespace NFSDK
{
	public class NFLoginModule : NFIModule
    {      
		private NFNetModule mNetModule;
		private NFIEventModule mEventModule;

        public NFLoginModule(NFIPluginManager pluginManager)
        {
            mPluginManager = pluginManager;
		}

        public override void AfterInit()
        {
			mNetModule = FindModule<NFNetModule>();
			mEventModule = FindModule<NFIEventModule>();

			mNetModule.AddReceiveCallBack(NFMsg.EGameMsgID.EGMI_ACK_LOGIN, OnLoginProcess);
            mNetModule.AddReceiveCallBack(NFMsg.EGameMsgID.EGMI_ACK_WORLD_LIST, OnWorldList);
            mNetModule.AddReceiveCallBack(NFMsg.EGameMsgID.EGMI_ACK_CONNECT_WORLD, OnConnectWorld);
            mNetModule.AddReceiveCallBack(NFMsg.EGameMsgID.EGMI_ACK_CONNECT_KEY, OnConnectKey);
            mNetModule.AddReceiveCallBack(NFMsg.EGameMsgID.EGMI_ACK_SELECT_SERVER, OnSelectServer);

        }
        
        // 请求消息
	    public void LoginPB(string strAccount, string strPwd, string strKey)
        {
            Debug.Log("LoginPB:" + strAccount);
            NFMsg.ReqAccountLogin xData = new NFMsg.ReqAccountLogin();
            xData.account = ByteString.CopyFromUtf8(strAccount);
            xData.password = ByteString.CopyFromUtf8(strPwd);
            xData.security_code = ByteString.CopyFromUtf8(strKey);
            xData.signBuff = ByteString.CopyFromUtf8("");
            xData.clientVersion = 1;
            xData.loginMode = 0;
            xData.clientIP = 0;
            xData.clientMAC = 0;
            xData.device_info = ByteString.CopyFromUtf8("");
            xData.extra_info = ByteString.CopyFromUtf8("");

            mAccount = strAccount;

            MemoryStream stream = new MemoryStream();
            xData.WriteTo(stream);

			mNetModule.SendToServerByPB(NFMsg.EGameMsgID.EGMI_REQ_LOGIN, stream);
        }
	    public void RequireWorldList()
        {
            NFMsg.ReqServerList xData = new NFMsg.ReqServerList();
            xData.type = NFMsg.ReqServerListType.RSLT_WORLD_SERVER;

            MemoryStream stream = new MemoryStream();
            xData.WriteTo(stream);

            mNetModule.SendToServerByPB(NFMsg.EGameMsgID.EGMI_REQ_WORLD_LIST, stream);
        }
	    public void RequireConnectWorld(int nWorldID)
        {
            NFMsg.ReqConnectWorld xData = new NFMsg.ReqConnectWorld();
            xData.world_id = nWorldID;

            MemoryStream stream = new MemoryStream();
            xData.WriteTo(stream);

            mNetModule.SendToServerByPB(NFMsg.EGameMsgID.EGMI_REQ_CONNECT_WORLD, stream);
        }
	    public void RequireVerifyWorldKey(string strAccount, string strKey)
        {
            NFMsg.ReqAccountLogin xData = new NFMsg.ReqAccountLogin();
            xData.account = ByteString.CopyFromUtf8(strAccount);
            xData.password = ByteString.CopyFromUtf8("");
            xData.security_code = ByteString.CopyFromUtf8(strKey);
            xData.signBuff = ByteString.CopyFromUtf8("");
            xData.clientVersion = 1;
            xData.loginMode = 0;
            xData.clientIP = 0;
            xData.clientMAC = 0;
            xData.device_info = ByteString.CopyFromUtf8("");
            xData.extra_info = ByteString.CopyFromUtf8("");

            MemoryStream stream = new MemoryStream();
            xData.WriteTo(stream);

            mNetModule.SendToServerByPB(NFMsg.EGameMsgID.EGMI_REQ_CONNECT_KEY, stream);
        }
	    public void RequireServerList()
        {
            NFMsg.ReqServerList xData = new NFMsg.ReqServerList();
            xData.type = NFMsg.ReqServerListType.RSLT_GAMES_ERVER;

            MemoryStream stream = new MemoryStream();
            xData.WriteTo(stream);

            mNetModule.SendToServerByPB(NFMsg.EGameMsgID.EGMI_REQ_WORLD_LIST, stream);
        }
	    public void RequireSelectServer(int nServerID)
        {
            NFMsg.ReqSelectServer xData = new NFMsg.ReqSelectServer();
            xData.world_id = nServerID;
            mServerID = nServerID;
            MemoryStream stream = new MemoryStream();
            xData.WriteTo(stream);

            mNetModule.SendToServerByPB(NFMsg.EGameMsgID.EGMI_REQ_SELECT_SERVER, stream);
        }
        // 接收消息
	    private void OnLoginProcess(UInt16 id, MemoryStream stream)
        {
            Debug.Log("OnLoginProcess1");
            NFMsg.MsgBase xMsg = NFMsg.MsgBase.Parser.ParseFrom(stream);

            Debug.Log("OnLoginProcess2");
            NFMsg.AckEventResult xData = NFMsg.AckEventResult.Parser.ParseFrom(xMsg.msg_data);

            Debug.Log("OnLoginProcess3");
            if (EGameEventCode.EGEC_ACCOUNT_SUCCESS == xData.event_code)
            {
                Debug.Log("Login  SUCCESS");
				mEventModule.DoEvent((int)NFUIModule.Event.LoginSuccess);
            }
            else
            {
                Debug.Log("Login Faild,Code: " + xData.event_code);
                NFDataList varList = new NFDataList();
                varList.AddInt((Int64)xData.event_code);
				mEventModule.DoEvent((int)NFUIModule.Event.LoginFailure);
            }
        }
        private void OnWorldList(UInt16 id, MemoryStream stream)
        {
            NFMsg.MsgBase xMsg = NFMsg.MsgBase.Parser.ParseFrom(stream);

            NFMsg.AckServerList xData = NFMsg.AckServerList.Parser.ParseFrom(xMsg.msg_data);

            if (ReqServerListType.RSLT_WORLD_SERVER == xData.type)
            {
                for (int i = 0; i < xData.info.Count; ++i)
                {
                    ServerInfo info = xData.info[i];
                    Debug.Log("WorldList  ServerId: " + info.server_id + " Name: " + info.name.ToStringUtf8() + " Status: " + info.status);
                    mWorldServerList.Add(info);
                }

				mEventModule.DoEvent((int)NFUIModule.Event.WorldList);
            }
            else if (ReqServerListType.RSLT_GAMES_ERVER == xData.type)
            {
                for (int i = 0; i < xData.info.Count; ++i)
                {
                    ServerInfo info = xData.info[i];
                    Debug.Log("GameList  ServerId: " + info.server_id + " Name: " + info.name.ToStringUtf8() + " Status: " + info.status);
                    mGameServerList.Add(info);
                }
				mEventModule.DoEvent((int)NFUIModule.Event.ServerList);
            }
        }
        private void OnConnectWorld(UInt16 id, MemoryStream stream)
        {
            NFMsg.MsgBase xMsg = NFMsg.MsgBase.Parser.ParseFrom(stream);

            NFMsg.AckConnectWorldResult xData = NFMsg.AckConnectWorldResult.Parser.ParseFrom(xMsg.msg_data);

			mNetModule.Shut();
			mNetModule.ConnectServer(xData.world_ip.ToStringUtf8(), (ushort)xData.world_port);
            mKey = xData.world_key.ToStringUtf8();

            RequireVerifyWorldKey(mAccount, mKey);
        }
        private void OnConnectKey(UInt16 id, MemoryStream stream)
        {
            NFMsg.MsgBase xMsg = NFMsg.MsgBase.Parser.ParseFrom(stream);

            NFMsg.AckEventResult xData = NFMsg.AckEventResult.Parser.ParseFrom(xMsg.msg_data);

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
            NFMsg.MsgBase xMsg = NFMsg.MsgBase.Parser.ParseFrom(stream);

            NFMsg.AckEventResult xData = NFMsg.AckEventResult.Parser.ParseFrom(xMsg.msg_data);

            if (xData.event_code == EGameEventCode.EGEC_SELECTSERVER_SUCCESS)
            {
                Debug.Log("SelectGame SUCCESS ");
				mEventModule.DoEvent((int)NFUIModule.Event.SelectServerSuccess);
            }
            else
            {
                Debug.Log("SelectGame Failed ");
            }
        }

		public override void Awake()
		{
		}

		public override void Init()
		{
		}

		public override void Execute()
		{
		}

		public override void BeforeShut()
		{
		}

		public override void Shut()
		{
		}

		public string mAccount;
        public string mKey;
        public int mServerID;
        public ArrayList mWorldServerList = new ArrayList();
        public ArrayList mGameServerList = new ArrayList();
    };
}