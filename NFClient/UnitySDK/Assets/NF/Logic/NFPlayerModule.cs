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
		private NFUIModule mUIModule;


        public override bool Awake()
        {
            return true;
        }

        public override bool Init()
        {
            return true;
        }

        public override bool Execute()
        {
            return true;
        }

        public override bool BeforeShut()
        {
            return true;
        }

        public override bool Shut()
        {
            return true;
        }

        public NFPlayerModule(NFIPluginManager pluginManager)
        {
            mPluginManager = pluginManager;
		}
        
        public override bool AfterInit()
        {
			mHelpModule = mPluginManager.FindModule<NFHelpModule>();
			mLoginModule = mPluginManager.FindModule<NFLoginModule>();
			mNetModule = mPluginManager.FindModule<NFNetModule>();
			mEventModule = mPluginManager.FindModule<NFIEventModule>();
			mUIModule = mPluginManager.FindModule<NFUIModule>();


			mNetModule.AddReceiveCallBack(NFMsg.EGameMsgID.EGMI_ACK_ROLE_LIST, OnRoleList);
			mNetModule.AddReceiveCallBack(NFMsg.EGameMsgID.EGMI_ACK_OBJECT_ENTRY, OnObjectEntry);
			mNetModule.AddReceiveCallBack(NFMsg.EGameMsgID.EGMI_ACK_OBJECT_LEAVE, OnObjectLeave);
			mNetModule.AddReceiveCallBack(NFMsg.EGameMsgID.EGMI_ACK_MOVE, OnObjectMove);
			mNetModule.AddReceiveCallBack(NFMsg.EGameMsgID.EGMI_ACK_MOVE_IMMUNE, OnObjectJump); 
			mNetModule.AddReceiveCallBack(NFMsg.EGameMsgID.EGMI_ACK_SWAP_SCENE, OnSwapScene);
            return true;
        }
        
        // ������Ϣ
        public void RequireRoleList()
        {
            NFMsg.ReqRoleList xData = new NFMsg.ReqRoleList();
			xData.game_id = mLoginModule.mServerID;
			xData.account = UnicodeEncoding.Default.GetBytes(mLoginModule.mAccount);

            MemoryStream stream = new MemoryStream();
            Serializer.Serialize<NFMsg.ReqRoleList>(stream, xData);

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
            xData.noob_name = UnicodeEncoding.Default.GetBytes(strRoleName);
            xData.account = UnicodeEncoding.Default.GetBytes(mLoginModule.mAccount);
            xData.race = 0;
			xData.game_id = mLoginModule.mServerID;

            MemoryStream stream = new MemoryStream();
            Serializer.Serialize<NFMsg.ReqCreateRole>(stream, xData);

            mNetModule.SendToServerByPB(NFMsg.EGameMsgID.EGMI_REQ_CREATE_ROLE, stream);
        }
	    public void RequireEnterGameServer(int nRoleIndex)
        {
            m_nRoleIndex = nRoleIndex;
            Debug.Log("RequireEnterGameServer:" + nRoleIndex);
            NFMsg.RoleLiteInfo info = (NFMsg.RoleLiteInfo)mRoleList[nRoleIndex];
            NFMsg.ReqEnterGameServer xData = new NFMsg.ReqEnterGameServer();
            xData.name = info.noob_name;
			xData.account = UnicodeEncoding.Default.GetBytes(mLoginModule.mAccount);
            xData.game_id = mLoginModule.mServerID;
            xData.id = info.id;
            MemoryStream stream = new MemoryStream();
            Serializer.Serialize<NFMsg.ReqEnterGameServer>(stream, xData);

			mRoleID = mHelpModule.PBToNF(info.id);
            mNetModule.mOwnerID = mRoleID;

            mNetModule.SendToServerByPB(NFMsg.EGameMsgID.EGMI_REQ_ENTER_GAME, stream);
        }
	    public void RequireMove(UnityEngine.Vector3 pos)
        {
            NFMsg.ReqAckPlayerMove xData = new NFMsg.ReqAckPlayerMove();
			xData.mover = mHelpModule.NFToPB(mRoleID);
            xData.moveType = 0;
            xData.speed = 2f;
            xData.time = 0;
            NFMsg.Vector3 xTargetPos = new NFMsg.Vector3();
            xTargetPos.x = pos.x;
            xTargetPos.y = pos.z;
            xTargetPos.z = pos.y;
            xData.target_pos.Add(xTargetPos);

            MemoryStream stream = new MemoryStream();
            Serializer.Serialize<NFMsg.ReqAckPlayerMove>(stream, xData);

            mNetModule.SendToServerByPB(NFMsg.EGameMsgID.EGMI_REQ_MOVE, stream);
        }

		private void OnRoleList(UInt16 id, MemoryStream stream)
        {
            NFMsg.MsgBase xMsg = new NFMsg.MsgBase();
            xMsg = Serializer.Deserialize<NFMsg.MsgBase>(stream);

            NFMsg.AckRoleLiteInfoList xData = new NFMsg.AckRoleLiteInfoList();
            xData = Serializer.Deserialize<NFMsg.AckRoleLiteInfoList>(new MemoryStream(xMsg.msg_data));

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
            NFMsg.MsgBase xMsg = new NFMsg.MsgBase();
            xMsg = Serializer.Deserialize<NFMsg.MsgBase>(stream);

            NFMsg.AckPlayerEntryList xData = new NFMsg.AckPlayerEntryList();
            xData = Serializer.Deserialize<NFMsg.AckPlayerEntryList>(new MemoryStream(xMsg.msg_data));

            for (int i = 0; i < xData.object_list.Count; ++i)
            {
                NFMsg.PlayerEntryInfo xInfo = xData.object_list[i];

                NFIDataList var = new NFCDataList();
                var.AddString("X");
                var.AddFloat(xInfo.x);
                var.AddString("Y");
                var.AddFloat(xInfo.z);
                var.AddString("Z");
                var.AddFloat(xInfo.y);

                Debug.LogWarning("Object Enter: " + mHelpModule.PBToNF(xInfo.object_guid).ToString() + "  ConfigID:" + System.Text.Encoding.Default.GetString(xInfo.class_id));
				NFIObject xGO = NFCKernelModule.Instance().CreateObject(mHelpModule.PBToNF(xInfo.object_guid), xInfo.scene_id, 0, System.Text.Encoding.Default.GetString(xInfo.class_id), System.Text.Encoding.Default.GetString(xInfo.config_id), var);
                if (null == xGO)
                {
                    Debug.LogError("ID: " + xInfo.object_guid + "  ConfigID:" + System.Text.Encoding.Default.GetString(xInfo.config_id));
                    continue;
                }
            }
        }
	    private void OnObjectLeave(UInt16 id, MemoryStream stream)
        {
            NFMsg.MsgBase xMsg = new NFMsg.MsgBase();
            xMsg = Serializer.Deserialize<NFMsg.MsgBase>(stream);

            NFMsg.AckPlayerLeaveList xData = new NFMsg.AckPlayerLeaveList();
            xData = Serializer.Deserialize<NFMsg.AckPlayerLeaveList>(new MemoryStream(xMsg.msg_data));

            for (int i = 0; i < xData.object_list.Count; ++i)
            {
				NFCKernelModule.Instance().DestroyObject(mHelpModule.PBToNF(xData.object_list[i]));
            }
        }

		private void OnSwapScene(UInt16 id, MemoryStream stream)
		{
			NFMsg.MsgBase xMsg = new NFMsg.MsgBase();
            xMsg = Serializer.Deserialize<NFMsg.MsgBase>(stream);

			NFMsg.ReqAckSwapScene xData = new NFMsg.ReqAckSwapScene();
			xData = Serializer.Deserialize<NFMsg.ReqAckSwapScene>(new MemoryStream(xMsg.msg_data));

			mUIModule.ShowUI<UIMain>();

			NFIDataList var = new NFCDataList();
			NFVector3 v = new NFVector3(xData.x, xData.y, xData.z);

			var.AddInt(xData.scene_id);
			var.AddVector3(v);

			mEventModule.DoEvent((int)Event.SwapScene, var);
		}

	    private void OnObjectMove(UInt16 id, MemoryStream stream)
        {
            NFMsg.MsgBase xMsg = new NFMsg.MsgBase();
            xMsg = Serializer.Deserialize<NFMsg.MsgBase>(stream);

            NFMsg.ReqAckPlayerMove xData = new NFMsg.ReqAckPlayerMove();
            xData = Serializer.Deserialize<NFMsg.ReqAckPlayerMove>(new MemoryStream(xMsg.msg_data));
            if (xData.target_pos.Count <= 0)
            {
                return;
            }
			float fSpeed = NFCKernelModule.Instance().QueryPropertyInt(mHelpModule.PBToNF(xData.mover), "MOVE_SPEED") / 10000.0f;

			Debug.LogWarning("Object Move: " + mHelpModule.PBToNF(xData.mover).ToString());
            NFIDataList var = new NFCDataList();
			var.AddObject(mHelpModule.PBToNF(xData.mover));
            var.AddFloat(fSpeed);
			NFVector3 pos = mHelpModule.PBToNF(xData.target_pos[0]);
            float fTemp = pos.Z();
            pos.SetZ(pos.Y());
            pos.SetY(fTemp);
            var.AddVector3(pos);

			mEventModule.DoEvent((int)Event.PlayerMove, var);
        }

	    private void OnObjectJump(UInt16 id, MemoryStream stream)
        {
            NFMsg.MsgBase xMsg = new NFMsg.MsgBase();
            xMsg = Serializer.Deserialize<NFMsg.MsgBase>(stream);

            NFMsg.ReqAckPlayerMove xData = new NFMsg.ReqAckPlayerMove();
            xData = Serializer.Deserialize<NFMsg.ReqAckPlayerMove>(new MemoryStream(xMsg.msg_data));
            if (xData.target_pos.Count <= 0)
            {
                return;
            }
			float fSpeed = NFCKernelModule.Instance().QueryPropertyInt(mHelpModule.PBToNF(xData.mover), "MOVE_SPEED") / 10000.0f;

            NFIDataList var = new NFCDataList();
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