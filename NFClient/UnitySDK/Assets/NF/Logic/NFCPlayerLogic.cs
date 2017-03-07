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
    public class NFCPlayerLogic : NFCLogicBase
    {
        public enum Event : int
        {
            RoleList,
            PlayerMove,
            PlayerJump,
        };
        private static NFCPlayerLogic _instance = null;
        public static NFCPlayerLogic Instance()
        {
            return _instance;
        }
        public NFCPlayerLogic(NFIPluginManager pluginManager)
        {
            _instance = this;
            mPluginManager = pluginManager;
		}

        public override bool AfterInit()
        {
            NFCNetLogic.Instance().AddReceiveCallBack(NFMsg.EGameMsgID.EGMI_ACK_ROLE_LIST, OnRoleList);
            NFCNetLogic.Instance().AddReceiveCallBack(NFMsg.EGameMsgID.EGMI_ACK_OBJECT_ENTRY, OnObjectEntry);
            NFCNetLogic.Instance().AddReceiveCallBack(NFMsg.EGameMsgID.EGMI_ACK_OBJECT_LEAVE, OnObjectLeave);
            NFCNetLogic.Instance().AddReceiveCallBack(NFMsg.EGameMsgID.EGMI_ACK_MOVE, OnObjectMove);
            NFCNetLogic.Instance().AddReceiveCallBack(NFMsg.EGameMsgID.EGMI_ACK_MOVE_IMMUNE, OnObjectJump);

            return true;
        }
        
        // 请求消息
        public void RequireRoleList()
        {
            NFMsg.ReqRoleList xData = new NFMsg.ReqRoleList();
            xData.game_id = NFCLoginLogic.Instance().mServerID;
            xData.account = UnicodeEncoding.Default.GetBytes(NFCLoginLogic.Instance().mAccount);

            MemoryStream stream = new MemoryStream();
            Serializer.Serialize<NFMsg.ReqRoleList>(stream, xData);

            NFCNetLogic.Instance().SendToServerByPB(NFMsg.EGameMsgID.EGMI_REQ_ROLE_LIST, stream);
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
            xData.account = UnicodeEncoding.Default.GetBytes(NFCLoginLogic.Instance().mAccount);
            xData.race = 0;
            xData.game_id = NFCLoginLogic.Instance().mServerID;

            MemoryStream stream = new MemoryStream();
            Serializer.Serialize<NFMsg.ReqCreateRole>(stream, xData);

            NFCNetLogic.Instance().SendToServerByPB(NFMsg.EGameMsgID.EGMI_REQ_CREATE_ROLE, stream);
        }
	    public void RequireEnterGameServer(int nRoleIndex)
        {
            m_nRoleIndex = nRoleIndex;
            Debug.Log("RequireEnterGameServer:" + nRoleIndex);
            NFMsg.RoleLiteInfo info = (NFMsg.RoleLiteInfo)mRoleList[nRoleIndex];
            NFMsg.ReqEnterGameServer xData = new NFMsg.ReqEnterGameServer();
            xData.name = info.noob_name;
            xData.account = UnicodeEncoding.Default.GetBytes(NFCLoginLogic.Instance().mAccount);
            xData.game_id = NFCLoginLogic.Instance().mServerID;
            xData.id = info.id;
            MemoryStream stream = new MemoryStream();
            Serializer.Serialize<NFMsg.ReqEnterGameServer>(stream, xData);

            mRoleID = PBToNF(info.id);
            NFCNetLogic.Instance().mOwnerID = mRoleID;

            NFCNetLogic.Instance().SendToServerByPB(NFMsg.EGameMsgID.EGMI_REQ_ENTER_GAME, stream);
        }
	    public void RequireMove(UnityEngine.Vector3 pos)
        {
            NFMsg.ReqAckPlayerMove xData = new NFMsg.ReqAckPlayerMove();
            xData.mover = NFToPB(mRoleID);
            xData.moveType = 0;
            NFMsg.Position xTargetPos = new NFMsg.Position();
            xTargetPos.x = pos.x;
            xTargetPos.y = pos.z;
            xTargetPos.z = pos.y;
            xData.target_pos.Add(xTargetPos);

            MemoryStream stream = new MemoryStream();
            Serializer.Serialize<NFMsg.ReqAckPlayerMove>(stream, xData);

            NFCNetLogic.Instance().SendToServerByPB(NFMsg.EGameMsgID.EGMI_REQ_MOVE, stream);
        }
        // 接收消息
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

            DoEvent((int)Event.RoleList);
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

                Debug.LogWarning("Object Enter: " + PBToNF(xInfo.object_guid).ToString() + "  ConfigID:" + System.Text.Encoding.Default.GetString(xInfo.class_id));
                NFIObject xGO = NFCKernelModule.Instance().CreateObject(PBToNF(xInfo.object_guid), xInfo.scene_id, 0, System.Text.Encoding.Default.GetString(xInfo.class_id), System.Text.Encoding.Default.GetString(xInfo.config_id), var);
                if (null == xGO)
                {
                    Debug.LogError("ID冲突: " + xInfo.object_guid + "  ConfigID:" + System.Text.Encoding.Default.GetString(xInfo.config_id));
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
                NFCKernelModule.Instance().DestroyObject(PBToNF(xData.object_list[i]));
            }
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
            float fSpeed = NFCKernelModule.Instance().QueryPropertyInt(PBToNF(xData.mover), "MOVE_SPEED") / 10000.0f;

            Debug.LogWarning("Object Move: " + PBToNF(xData.mover).ToString());
            NFIDataList var = new NFCDataList();
            var.AddObject(PBToNF(xData.mover));
            var.AddFloat(fSpeed);
            NFVector3 pos = PBToNF(xData.target_pos[0]);
            float fTemp = pos.Z();
            pos.SetZ(pos.Y());
            pos.SetY(fTemp);
            var.AddVector3(pos);

            DoEvent((int)Event.PlayerMove, var);
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
            float fSpeed = NFCKernelModule.Instance().QueryPropertyInt(PBToNF(xData.mover), "MOVE_SPEED") / 10000.0f;

            NFIDataList var = new NFCDataList();
            var.AddObject(PBToNF(xData.mover));
            var.AddFloat(fSpeed);
            var.AddVector3(PBToNF(xData.target_pos[0]));
            DoEvent((int)Event.PlayerJump, var);
        }

        public NFGUID mRoleID;
        public int m_nRoleIndex;
        public ArrayList mRoleList = new ArrayList();
    };
}