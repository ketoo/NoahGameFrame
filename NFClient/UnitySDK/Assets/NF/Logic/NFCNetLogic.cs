using System;
using System.Linq;
using System.Text;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using ProtoBuf;
using UnityEngine;

namespace NFSDK
{
    public class NFCNetLogic : NFCLogicBase
    {
        private static NFCNetLogic _instance = null;
        public static NFCNetLogic Instance()
        {
            return _instance;
        }
        public NFCNetLogic(NFIPluginManager pluginManager)
        {
            _instance = this;
            mPluginManager = pluginManager;
		}

        public override bool AfterInit()
        {
            return true;
        }
        public override bool Execute() 
        {
            NFCNet.Instance().doUpdate();
            return true; 
        }

        public void ConnectServer(string ip, UInt16 port)
        {
            if (NFCNet.Instance().isConnected())
                NFCNet.Instance().shutDown();

            if (ip == "127.0.0.1" && port != 14001)
            {
                ip = NFCNet.Instance().ip;
            }

            NFCNet.Instance().ready(ip, port);
            NFCNet.Instance().connect();
        }

        public void AddReceiveCallBack(NFMsg.EGameMsgID id, NFCMessageDispatcher.MessageHandler netHandler)
        {
            NFCNetDispatcher.Instance().AddReceiveCallBack((UInt16)id, netHandler);
        }

        public void SendToServerByPB(NFMsg.EGameMsgID unMsgID, MemoryStream stream)
        {
            NFMsg.MsgBase xData = new NFMsg.MsgBase();
            xData.player_id = NFToPB(mOwnerID);
            xData.msg_data = stream.ToArray();
            
            MemoryStream body = new MemoryStream();
            Serializer.Serialize<NFMsg.MsgBase>(body, xData);

            MemoryStream pack = new MemoryStream();
            BinaryWriter writer = new BinaryWriter(pack);
            UInt32 msgLen = (UInt32)body.Length + (UInt32)ConstDefine.NF_PACKET_HEAD_SIZE;
            writer.Write(NFCNet.ConvertUint16((UInt16)unMsgID));
            writer.Write(NFCNet.ConvertUint32((UInt32)msgLen));
            body.WriteTo(pack);
            NFCNet.Instance().sendMsg(pack);
        }

        public NFGUID mOwnerID = new NFGUID();
    }
}