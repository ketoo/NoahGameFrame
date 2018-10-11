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
	public class NFPlayerModule : NFIModule
    {
        public enum Event : int
        {
            RoleList = 10000,
            PlayerMove,
			PlayerJump,
			SwapScene,
        };


		private NFHelpModule mHelpModule;
		private NFNetModule mNetModule;
		private NFLoginModule mLoginModule;
		private NFIEventModule mEventModule;
		private NFIKernelModule mKernelModule;
		private NFUIModule mUIModule;


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

        public NFPlayerModule(NFIPluginManager pluginManager)
        {
            mPluginManager = pluginManager;
		}
        
        public override void AfterInit()
        {
			mHelpModule = mPluginManager.FindModule<NFHelpModule>();
			mLoginModule = mPluginManager.FindModule<NFLoginModule>();
			mNetModule = mPluginManager.FindModule<NFNetModule>();
			mEventModule = mPluginManager.FindModule<NFIEventModule>();
			mUIModule = mPluginManager.FindModule<NFUIModule>();
			mKernelModule = mPluginManager.FindModule<NFIKernelModule>();


			mNetModule.AddReceiveCallBack(NFMsg.EGameMsgID.EGMI_ACK_ROLE_LIST, OnRoleList);
			mNetModule.AddReceiveCallBack(NFMsg.EGameMsgID.EGMI_ACK_OBJECT_ENTRY, OnObjectEntry);
			mNetModule.AddReceiveCallBack(NFMsg.EGameMsgID.EGMI_ACK_OBJECT_LEAVE, OnObjectLeave);

			mNetModule.AddReceiveCallBack(NFMsg.EGameMsgID.EGMI_ACK_MOVE, OnObjectMove);
			mNetModule.AddReceiveCallBack(NFMsg.EGameMsgID.EGMI_ACK_MOVE_IMMUNE, OnObjectJump);
			mNetModule.AddReceiveCallBack(NFMsg.EGameMsgID.EGMI_ACK_STATE_SYNC, OnObjectStatusSync);

			mNetModule.AddReceiveCallBack(NFMsg.EGameMsgID.EGMI_ACK_SWAP_SCENE, OnSwapScene);
			mNetModule.AddReceiveCallBack(NFMsg.EGameMsgID.EGMI_ACK_ENTER_GAME, OnEnterGame);
			mNetModule.AddReceiveCallBack(NFMsg.EGameMsgID.EGMI_ACK_ENTER_GAME_FINISH, OnEnterGameFinish);
			mNetModule.AddReceiveCallBack(NFMsg.EGameMsgID.EGMI_ACK_DATA_FINISHED, OnLoadDataFinish);

			mNetModule.AddReceiveCallBack(NFMsg.EGameMsgID.EGMI_EVENT_RESULT, OnEventResult);
            
            /*
            mxNetListener.RegisteredDelegation(NFMsg.EGameMsgID.EGMI_ACK_SKILL_OBJECTX, EGMI_ACK_SKILL_OBJECTX);
            mxNetListener.RegisteredDelegation(NFMsg.EGameMsgID.EGMI_ACK_CHAT, EGMI_ACK_CHAT);
            mxNetListener.RegisteredDelegation(NFMsg.EGameMsgID.EGMI_REQ_ACK_RELIVE, EGMI_REQ_ACK_RELIVE);


            mxNetListener.RegisteredDelegation(NFMsg.EGameMsgID.EGEC_ACK_MINING_TITLE, EGEC_ACK_MINING_TITLE);
            mxNetListener.RegisteredDelegation(NFMsg.EGameMsgID.EGMI_ACK_SEARCH_OPPNENT, EGMI_ACK_SEARCH_OPPNENT);
            mxNetListener.RegisteredDelegation(NFMsg.EGameMsgID.EGMI_ACK_START_OPPNENT, EGMI_ACK_START_OPPNENT);
            mxNetListener.RegisteredDelegation(NFMsg.EGameMsgID.EGMI_ACK_END_OPPNENT, EGMI_ACK_END_OPPNENT);
             */
        }
 
        public void RequireRoleList()
        {
            NFMsg.ReqRoleList xData = new NFMsg.ReqRoleList();
			xData.game_id = mLoginModule.mServerID;
			xData.account = ByteString.CopyFromUtf8(mLoginModule.mAccount);

            MemoryStream stream = new MemoryStream();
            xData.WriteTo(stream);

            mNetModule.SendToServerByPB(NFMsg.EGameMsgID.EGMI_REQ_ROLE_LIST, stream);
            Debug.Log("RequireRoleList");
        }
	    public void RequireCreateRole(string strRoleName, int byCareer, int bySex)
        {
            if (strRoleName.Length >= 20 || strRoleName.Length < 1)
            {
                return;
            }

            NFMsg.ReqCreateRole xData = new NFMsg.ReqCreateRole();
            xData.career = byCareer;
            xData.sex = bySex;
            xData.noob_name = ByteString.CopyFromUtf8(strRoleName);
            xData.account = ByteString.CopyFromUtf8(mLoginModule.mAccount);
            xData.race = 0;
			xData.game_id = mLoginModule.mServerID;

            MemoryStream stream = new MemoryStream();
            xData.WriteTo(stream);

            mNetModule.SendToServerByPB(NFMsg.EGameMsgID.EGMI_REQ_CREATE_ROLE, stream);
        }
	    public void RequireEnterGameServer(int nRoleIndex)
        {
            m_nRoleIndex = nRoleIndex;
            Debug.Log("RequireEnterGameServer:" + nRoleIndex);
            NFMsg.RoleLiteInfo info = (NFMsg.RoleLiteInfo)mRoleList[nRoleIndex];
            NFMsg.ReqEnterGameServer xData = new NFMsg.ReqEnterGameServer();
            xData.name = info.noob_name;
			xData.account = ByteString.CopyFromUtf8(mLoginModule.mAccount);
            xData.game_id = mLoginModule.mServerID;
            xData.id = info.id;
            MemoryStream stream = new MemoryStream();
            xData.WriteTo(stream);

            mRoleID = mHelpModule.PBToNF(info.id);
            mNetModule.mOwnerID = mRoleID;

            mNetModule.SendToServerByPB(NFMsg.EGameMsgID.EGMI_REQ_ENTER_GAME, stream);
        }


        public void RequireJump(UnityEngine.Vector3 pos)
        {
            NFMsg.ReqAckPlayerMove xData = new NFMsg.ReqAckPlayerMove();
            xData.mover = mHelpModule.NFToPB(mRoleID);
            xData.moveType = 1;
            xData.speed = 2f;
            xData.time = 0;
            NFMsg.Vector3 xTargetPos = new NFMsg.Vector3();
            xTargetPos.x = pos.x;
            xTargetPos.y = pos.y;
            xTargetPos.z = pos.z;
            xData.target_pos.Add(xTargetPos);

            MemoryStream stream = new MemoryStream();
            xData.WriteTo(stream);

            mNetModule.SendToServerByPB(NFMsg.EGameMsgID.EGMI_REQ_MOVE, stream);
        }

	    public void RequireMove(UnityEngine.Vector3 pos)
        {
            NFMsg.ReqAckPlayerMove xData = new NFMsg.ReqAckPlayerMove();
			xData.mover = mHelpModule.NFToPB(mRoleID);
            xData.moveType = 0;
			xData.speed = 4f;
            xData.time = 0;
            NFMsg.Vector3 xTargetPos = new NFMsg.Vector3();
            xTargetPos.x = pos.x;
            xTargetPos.y = pos.y;
            xTargetPos.z = pos.z;
            xData.target_pos.Add(xTargetPos);

            MemoryStream stream = new MemoryStream();
            xData.WriteTo(stream);

            mNetModule.SendToServerByPB(NFMsg.EGameMsgID.EGMI_REQ_MOVE, stream);
        }

		private void OnRoleList(UInt16 id, MemoryStream stream)
        {
            NFMsg.MsgBase xMsg = NFMsg.MsgBase.Parser.ParseFrom(stream);

            NFMsg.AckRoleLiteInfoList xData = NFMsg.AckRoleLiteInfoList.Parser.ParseFrom(xMsg.msg_data);

            Debug.Log("QueryRoleList  SUCCESS");
            mRoleList.Clear();
            for (int i = 0; i < xData.char_data.Count; ++i)
            {
                NFMsg.RoleLiteInfo info = xData.char_data[i];
                mRoleList.Add(info);
            }

			mEventModule.DoEvent((int)Event.RoleList);
        }
	    private void OnObjectEntry(UInt16 id, MemoryStream stream)
        {
            NFMsg.MsgBase xMsg = NFMsg.MsgBase.Parser.ParseFrom(stream);

            NFMsg.AckPlayerEntryList xData = NFMsg.AckPlayerEntryList.Parser.ParseFrom(xMsg.msg_data);

            for (int i = 0; i < xData.object_list.Count; ++i)
            {
                NFMsg.PlayerEntryInfo xInfo = xData.object_list[i];

                NFDataList var = new NFDataList();
                var.AddString("X");
                var.AddFloat(xInfo.x);
                var.AddString("Y");
                var.AddFloat(xInfo.z);
                var.AddString("Z");
                var.AddFloat(xInfo.y);

                Debug.LogWarning("Object Enter: " + mHelpModule.PBToNF(xInfo.object_guid).ToString() + "  ConfigID:" + xInfo.class_id.ToStringUtf8());
				NFIObject xGO = mKernelModule.CreateObject(mHelpModule.PBToNF(xInfo.object_guid), xInfo.scene_id, 0, xInfo.class_id.ToStringUtf8(), xInfo.config_id.ToStringUtf8(), var);
                if (null == xGO)
                {
                    Debug.LogError("ID: " + xInfo.object_guid + "  ConfigID:" + xInfo.config_id.ToStringUtf8());
                    continue;
                }
            }
        }
	    private void OnObjectLeave(UInt16 id, MemoryStream stream)
        {
            NFMsg.MsgBase xMsg = NFMsg.MsgBase.Parser.ParseFrom(stream);

            NFMsg.AckPlayerLeaveList xData = NFMsg.AckPlayerLeaveList.Parser.ParseFrom(xMsg.msg_data);

            for (int i = 0; i < xData.object_list.Count; ++i)
            {
				mKernelModule.DestroyObject(mHelpModule.PBToNF(xData.object_list[i]));
            }
        }

		private void OnSwapScene(UInt16 id, MemoryStream stream)
		{
			NFMsg.MsgBase xMsg = NFMsg.MsgBase.Parser.ParseFrom(stream);

            NFMsg.ReqAckSwapScene xData = NFMsg.ReqAckSwapScene.Parser.ParseFrom(xMsg.msg_data);

			mUIModule.ShowUI<UIMain>();

			NFDataList var = new NFDataList();
			NFVector3 v = new NFVector3(xData.x, xData.y, xData.z);

			var.AddInt(xData.scene_id);
			var.AddVector3(v);

			mEventModule.DoEvent((int)Event.SwapScene, var);
		}


		private void OnEnterGame(UInt16 id, MemoryStream stream)
        {

        }

		private void OnEnterGameFinish(UInt16 id, MemoryStream stream)
        {
     
        }

		private void OnLoadDataFinish(UInt16 id, MemoryStream stream)
        {

        }

		private void OnEventResult(UInt16 id, MemoryStream stream)
        {

        }


	    private void OnObjectMove(UInt16 id, MemoryStream stream)
        {
            NFMsg.MsgBase xMsg = NFMsg.MsgBase.Parser.ParseFrom(stream);

            NFMsg.ReqAckPlayerMove xData = NFMsg.ReqAckPlayerMove.Parser.ParseFrom(xMsg.msg_data);
            if (xData.target_pos.Count <= 0)
            {
                return;
            }

			NFGUID mover = mHelpModule.PBToNF(xData.mover);
			float fSpeed = mKernelModule.QueryPropertyInt(mover, NFrame.Player.MOVE_SPEED) / 100.0f;

            NFDataList var = new NFDataList();

			var.AddObject(mHelpModule.PBToNF(xData.mover));
			var.AddFloat(fSpeed);
			var.AddInt(xData.moveType);

			NFVector3 pos = mHelpModule.PBToNF(xData.target_pos[0]);
            var.AddVector3(pos);

			mEventModule.DoEvent((int)Event.PlayerMove, var);
        }

		private void OnObjectStatusSync(UInt16 id, MemoryStream stream)
		{
		}

	    private void OnObjectJump(UInt16 id, MemoryStream stream)
        {
            NFMsg.MsgBase xMsg = NFMsg.MsgBase.Parser.ParseFrom(stream);

            NFMsg.ReqAckPlayerMove xData = NFMsg.ReqAckPlayerMove.Parser.ParseFrom(xMsg.msg_data);
            if (xData.target_pos.Count <= 0)
            {
                return;
            }
			float fSpeed = mKernelModule.QueryPropertyInt(mHelpModule.PBToNF(xData.mover), "MOVE_SPEED") / 100.0f;

            NFDataList var = new NFDataList();
			var.AddObject(mHelpModule.PBToNF(xData.mover));
            var.AddFloat(fSpeed);
			var.AddVector3(mHelpModule.PBToNF(xData.target_pos[0]));

			mEventModule.DoEvent((int)Event.PlayerJump, var);
            //DoEvent((int)Event.PlayerJump, var);
        }

        public NFGUID mRoleID;
        public int m_nRoleIndex;
        public ArrayList mRoleList = new ArrayList();
    };
}