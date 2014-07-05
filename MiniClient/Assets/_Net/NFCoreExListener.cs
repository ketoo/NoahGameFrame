using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;
using System.IO;
using UnityEngine;
using NFMsg;
using ProtoBuf;
using NFCoreEx;

namespace NFTCPClient
{
	
	public class NFCoreExListener
	{
        NFNet mNet = null;
        public ArrayList aWorldList = new ArrayList();
        public ArrayList aServerList = new ArrayList();
        public ArrayList aCharList = new ArrayList();
        public ArrayList aObjectList = new ArrayList();
        public ArrayList aChatMsgList = new ArrayList();
        public ArrayList aMsgList = new ArrayList();

        public NFCoreExListener(NFNet net  )
        {
            mNet = net;
            InitLog();
        }


        ~NFCoreExListener()
        {
            FinalLog();
        }

        FileStream fs = null;
        StreamWriter sw = null;
        void InitLog()
        {
            DirectoryInfo oDir = new DirectoryInfo(Path.GetFullPath("./log/"));
            if (!oDir.Exists)
            {
                oDir.Create();
            }

            FinalLog();

            fs = new FileStream("./log/MsgLog_" + mNet.strAccount + ".txt", FileMode.Create);
            sw = new StreamWriter(fs, Encoding.Default);
        }

        void FinalLog()
        {
            if (null != fs)
            {
                sw.Close();
                fs.Close();
                sw = null;
                fs = null;
            }
        }

        public void Log(string text)
        {
            if (null != sw)
            {
                string strData = "[" + DateTime.Now.Year + "/" + DateTime.Now.Month + "/" + DateTime.Now.Day + " " + DateTime.Now.Hour + ":" + DateTime.Now.Minute + ":" + DateTime.Now.Second + "] " + text;
                Debug.Log(strData);
                sw.WriteLine(strData);
                sw.Flush();
            }
        }
		// Use this for initialization
		public void Init() 
		{

            mNet.binMsgEvent.RegisteredDelegation(NFMsg.EGameMsgID.EGMI_ACK_LOGIN, EGMI_ACK_LOGIN);
            mNet.binMsgEvent.RegisteredDelegation(NFMsg.EGameMsgID.EGMI_ACK_WORLD_LIST, EGMI_ACK_WORLD_LIST);
            mNet.binMsgEvent.RegisteredDelegation(NFMsg.EGameMsgID.EGMI_EVENT_RESULT, EGMI_EVENT_RESULT);
            mNet.binMsgEvent.RegisteredDelegation(NFMsg.EGameMsgID.EGMI_ACK_ROLE_LIST, EGMI_ACK_ROLE_LIST);
            mNet.binMsgEvent.RegisteredDelegation(NFMsg.EGameMsgID.EGMI_ACK_CONNECT_WORLD, EGMI_ACK_CONNECT_WORLD);
            mNet.binMsgEvent.RegisteredDelegation(NFMsg.EGameMsgID.EGMI_ACK_CONNECT_KEY, EGMI_ACK_CONNECT_KEY);
            mNet.binMsgEvent.RegisteredDelegation(NFMsg.EGameMsgID.EGMI_ACK_SELECT_SERVER, EGMI_ACK_SELECT_SERVER);
            

            mNet.binMsgEvent.RegisteredDelegation(NFMsg.EGameMsgID.EGMI_ACK_OBJECT_ENTRY, EGMI_ACK_OBJECT_ENTRY);
            mNet.binMsgEvent.RegisteredDelegation(NFMsg.EGameMsgID.EGMI_ACK_OBJECT_LEAVE, EGMI_ACK_OBJECT_LEAVE);
            mNet.binMsgEvent.RegisteredDelegation(NFMsg.EGameMsgID.EGMI_ACK_MOVE, EGMI_ACK_MOVE);
            mNet.binMsgEvent.RegisteredDelegation(NFMsg.EGameMsgID.EGMI_ACK_MOVE_IMMUNE, EGMI_ACK_MOVE);

            mNet.binMsgEvent.RegisteredDelegation(NFMsg.EGameMsgID.EGMI_ACK_PROPERTY_INT, EGMI_ACK_PROPERTY_INT);
            mNet.binMsgEvent.RegisteredDelegation(NFMsg.EGameMsgID.EGMI_ACK_PROPERTY_FLOAT, EGMI_ACK_PROPERTY_FLOAT);
            mNet.binMsgEvent.RegisteredDelegation(NFMsg.EGameMsgID.EGMI_ACK_PROPERTY_STRING, EGMI_ACK_PROPERTY_STRING);
            mNet.binMsgEvent.RegisteredDelegation(NFMsg.EGameMsgID.EGMI_ACK_PROPERTY_OBJECT, EGMI_ACK_PROPERTY_OBJECT);

            mNet.binMsgEvent.RegisteredDelegation(NFMsg.EGameMsgID.EGMI_ACK_RECORD_INT, EGMI_ACK_RECORD_INT);
            mNet.binMsgEvent.RegisteredDelegation(NFMsg.EGameMsgID.EGMI_ACK_RECORD_FLOAT, EGMI_ACK_RECORD_FLOAT);
            mNet.binMsgEvent.RegisteredDelegation(NFMsg.EGameMsgID.EGMI_ACK_RECORD_STRING, EGMI_ACK_RECORD_STRING);
            mNet.binMsgEvent.RegisteredDelegation(NFMsg.EGameMsgID.EGMI_ACK_RECORD_OBJECT, EGMI_ACK_RECORD_OBJECT);
            mNet.binMsgEvent.RegisteredDelegation(NFMsg.EGameMsgID.EGMI_ACK_SWAP_ROW, EGMI_ACK_SWAP_ROW);
            mNet.binMsgEvent.RegisteredDelegation(NFMsg.EGameMsgID.EGMI_ACK_ADD_ROW, EGMI_ACK_ADD_ROW);
            mNet.binMsgEvent.RegisteredDelegation(NFMsg.EGameMsgID.EGMI_ACK_REMOVE_ROW, EGMI_ACK_REMOVE_ROW);
            //mNet.binMsgEvent.RegisteredDelegation(NFMsg.EGameMsgID.EGMI_ACK_RECORD_CLEAR, EGMI_ACK_RECORD_CLEAR);

//             mNet.binMsgEvent.RegisteredDelegation(NFMsg.EGameMsgID.MGPT_CHAT_MESSAGE, MGPT_CHAT_MESSAGE);		
            
            InitLog();
			
		}

        private void EGMI_EVENT_RESULT(MsgHead head, MemoryStream stream)
        {
            //OnResultMsg
            NFMsg.AckEventResult xResultCode = new NFMsg.AckEventResult();
            xResultCode = Serializer.Deserialize<NFMsg.AckEventResult>(stream);
            NFMsg.EGameEventCode eEvent = xResultCode.event_code;

            mNet.binMsgEvent.DoResultCodeDelegation(eEvent);
        }

        private void EGMI_ACK_LOGIN(MsgHead head, MemoryStream stream)
        {
            NFMsg.MsgBase xMsg = new NFMsg.MsgBase();
            xMsg = Serializer.Deserialize<NFMsg.MsgBase>(stream);

            NFMsg.AckEventResult xData = new NFMsg.AckEventResult();
            xData = Serializer.Deserialize<NFMsg.AckEventResult>(new MemoryStream(xMsg.msg_data));

            if (EGameEventCode.EGEC_ACCOUNT_SUCCESS == xData.event_code)
            {
                mNet.mPlayerState = NFNet.PLAYER_STATE.E_HAS_PLAYER_LOGIN;

                mNet.sendLogic.RequireWorldList();
            }
        }

        private void EGMI_ACK_WORLD_LIST(MsgHead head, MemoryStream stream)
        {
            NFMsg.MsgBase xMsg = new NFMsg.MsgBase();
            xMsg = Serializer.Deserialize<NFMsg.MsgBase>(stream);

            NFMsg.AckServerList xData = new NFMsg.AckServerList();
            xData = Serializer.Deserialize<NFMsg.AckServerList>(new MemoryStream(xMsg.msg_data));

            if (ReqServerListType.RSLT_WORLD_SERVER == xData.type)
            {
                for(int i = 0; i < xData.info.Count; ++i)
                {
                    ServerInfo info = xData.info[i];
                    aWorldList.Add(info);
                }
            }
            else if (ReqServerListType.RSLT_GAMES_ERVER == xData.type)
            {
                for (int i = 0; i < xData.info.Count; ++i)
                {
                    ServerInfo info = xData.info[i];
                    aServerList.Add(info);
                }
            }
        }

        private void EGMI_ACK_CONNECT_WORLD(MsgHead head, MemoryStream stream)
        {
            mNet.client.Disconnect();
            mNet.listener.FinalLog();

            NFMsg.MsgBase xMsg = new NFMsg.MsgBase();
            xMsg = Serializer.Deserialize<NFMsg.MsgBase>(stream);

            NFMsg.AckConnectWorldResult xData = new NFMsg.AckConnectWorldResult();
            xData = Serializer.Deserialize<NFMsg.AckConnectWorldResult>(new MemoryStream(xMsg.msg_data));

            ///
            mNet.mPlayerState = NFNet.PLAYER_STATE.E_WAITING_PLAYER_TO_GATE;
            mNet.strKey = System.Text.Encoding.Default.GetString(xData.world_key);
            mNet.strWorldIP = System.Text.Encoding.Default.GetString(xData.world_ip);
            mNet.nWorldPort = xData.world_port;
        }

        private void EGMI_ACK_CONNECT_KEY(MsgHead head, MemoryStream stream)
        {
            NFMsg.MsgBase xMsg = new NFMsg.MsgBase();
            xMsg = Serializer.Deserialize<NFMsg.MsgBase>(stream);

            NFMsg.AckEventResult xData = new NFMsg.AckEventResult();
            xData = Serializer.Deserialize<NFMsg.AckEventResult>(new MemoryStream(xMsg.msg_data));

            if (xData.event_code == EGameEventCode.EGEC_VERIFY_KEY_SUCCESS)
            {
                //验证成功
                mNet.mPlayerState = NFNet.PLAYER_STATE.E_HAS_VERIFY;
                //申请世界内的服务器列表
                mNet.sendLogic.RequireServerList();
            }
        }

        private void EGMI_ACK_SELECT_SERVER(MsgHead head, MemoryStream stream)
        {
            NFMsg.MsgBase xMsg = new NFMsg.MsgBase();
            xMsg = Serializer.Deserialize<NFMsg.MsgBase>(stream);

            NFMsg.AckEventResult xData = new NFMsg.AckEventResult();
            xData = Serializer.Deserialize<NFMsg.AckEventResult>(new MemoryStream(xMsg.msg_data));

            if (xData.event_code == EGameEventCode.EGEC_SELECTSERVER_SUCCESS)
            {
                //申请角色列表
                mNet.sendLogic.RequireRoleList(mNet.nServerID);
            }
        }
        
        
        private void EGMI_ACK_ROLE_LIST(MsgHead head, MemoryStream stream)
        {
            mNet.mPlayerState = NFNet.PLAYER_STATE.E_HAS_PLAYER_ROLELIST;

            NFMsg.MsgBase xMsg = new NFMsg.MsgBase();
            xMsg = Serializer.Deserialize<NFMsg.MsgBase>(stream);

            NFMsg.AckRoleLiteInfoList xData = new NFMsg.AckRoleLiteInfoList();
            xData = Serializer.Deserialize<NFMsg.AckRoleLiteInfoList>(new MemoryStream(xMsg.msg_data));

            for (int i = 0; i < xData.char_data.Count; ++i)
            {
                NFMsg.RoleLiteInfo info = xData.char_data[i];
                aCharList.Add(info);
            }
        }

        private void EGMI_ACK_OBJECT_ENTRY(MsgHead head, MemoryStream stream)
        {
            NFMsg.MsgBase xMsg = new NFMsg.MsgBase();
            xMsg = Serializer.Deserialize<NFMsg.MsgBase>(stream);

//             NFMsg.ACKENTRy xData = new NFMsg.AckServerList();
//             xData = Serializer.Deserialize<NFMsg.AckServerList>(new MemoryStream(xMsg.msg_data));
        }

        private void EGMI_ACK_OBJECT_LEAVE(MsgHead head, MemoryStream stream)
		{

		}

        private void EGMI_ACK_MOVE(MsgHead head, MemoryStream stream)
        {

        }
        /////////////////////////////////////////////////////////////////////
        private void EGMI_ACK_PROPERTY_INT(MsgHead head, MemoryStream stream)
		{
			NFMsg.ObjectPropertyInt propertyData = new NFMsg.ObjectPropertyInt();	
			propertyData = Serializer.Deserialize<NFMsg.ObjectPropertyInt>(stream);

            NFIObject go = mNet.kernel.GetObject(new NFIDENTID(propertyData.player_id));
            NFIPropertyManager propertyManager = go.GetPropertyManager();
			
			for(int i = 0; i < propertyData.property_list.Count; i++)
			{
                NFIProperty property = propertyManager.GetProperty(propertyData.property_list[i].property_name.ToString());
                if(null == property)
                {
                    NFIValueList varList = new NFCValueList();
                    varList.AddInt(0);

                    property = propertyManager.AddProperty(propertyData.property_list[i].property_name.ToString(), varList);
                }

                property.SetInt(propertyData.property_list[i].data);
			}
		}
		
		private void EGMI_ACK_PROPERTY_FLOAT(MsgHead head, MemoryStream stream)
		{
			NFMsg.ObjectPropertyFloat propertyData = new NFMsg.ObjectPropertyFloat();	
			propertyData = Serializer.Deserialize<NFMsg.ObjectPropertyFloat>(stream);

            NFIObject go = mNet.kernel.GetObject(new NFIDENTID(propertyData.player_id));
			
			for(int i = 0; i < propertyData.property_list.Count; i++)
			{
                NFIPropertyManager propertyManager = go.GetPropertyManager();
                NFIProperty property = propertyManager.GetProperty(propertyData.property_list[i].property_name.ToString());
                if (null == property)
                {
                    NFIValueList varList = new NFCValueList();
                    varList.AddFloat(0.0f);

                    property = propertyManager.AddProperty(propertyData.property_list[i].property_name.ToString(), varList);
                }

                property.SetFloat(propertyData.property_list[i].data);
			}
		}
		
		private void EGMI_ACK_PROPERTY_STRING(MsgHead head, MemoryStream stream)
		{
			NFMsg.ObjectPropertyString propertyData = new NFMsg.ObjectPropertyString();	
			propertyData = Serializer.Deserialize<NFMsg.ObjectPropertyString>(stream);

            NFIObject go = mNet.kernel.GetObject(new NFIDENTID(propertyData.player_id));

			for(int i = 0; i < propertyData.property_list.Count; i++)
			{
                NFIPropertyManager propertyManager = go.GetPropertyManager();
                NFIProperty property = propertyManager.GetProperty(propertyData.property_list[i].property_name.ToString());
                if (null == property)
                {
                    NFIValueList varList = new NFCValueList();
                    varList.AddString("");

                    property = propertyManager.AddProperty(propertyData.property_list[i].property_name.ToString(), varList);
                }

                property.SetString(propertyData.property_list[i].data.ToString());
			}
		}
		
		private void EGMI_ACK_PROPERTY_OBJECT(MsgHead head, MemoryStream stream)
		{
			NFMsg.ObjectPropertyObject propertyData = new NFMsg.ObjectPropertyObject();	
			propertyData = Serializer.Deserialize<NFMsg.ObjectPropertyObject>(stream);

            NFIObject go = mNet.kernel.GetObject(new NFIDENTID(propertyData.player_id));
			
			for(int i = 0; i < propertyData.property_list.Count; i++)
			{
                NFIPropertyManager propertyManager = go.GetPropertyManager();
                NFIProperty property = propertyManager.GetProperty(propertyData.property_list[i].property_name.ToString());
                if (null == property)
                {
                    NFIValueList varList = new NFCValueList();
                    varList.AddObject(new NFIDENTID(0));

                    property = propertyManager.AddProperty(propertyData.property_list[i].property_name.ToString(), varList);
                }

                property.SetObject(new NFIDENTID(propertyData.property_list[i].data));
			}
		}
		
		private void EGMI_ACK_RECORD_INT(MsgHead head, MemoryStream stream)
		{
			NFMsg.ObjectRecordInt recordData = new NFMsg.ObjectRecordInt();	
			recordData = Serializer.Deserialize<NFMsg.ObjectRecordInt>(stream);

            NFIObject go = mNet.kernel.GetObject(new NFIDENTID(recordData.player_id));
            NFIRecordManager recordManager = go.GetRecordManager();
            NFIRecord record = recordManager.GetRecord(recordData.record_name.ToString());

            for (int i = 0; i < recordData.property_list.Count; i++)
            {
                record.SetInt(recordData.property_list[i].row, recordData.property_list[i].col, (int)recordData.property_list[i].data);
            }
		}
		
		private void EGMI_ACK_RECORD_FLOAT(MsgHead head, MemoryStream stream)
		{
			NFMsg.ObjectRecordFloat recordData = new NFMsg.ObjectRecordFloat();	
			recordData = Serializer.Deserialize<NFMsg.ObjectRecordFloat>(stream);

            NFIObject go = mNet.kernel.GetObject(new NFIDENTID(recordData.player_id));
            NFIRecordManager recordManager = go.GetRecordManager();
            NFIRecord record = recordManager.GetRecord(recordData.record_name.ToString());

            for (int i = 0; i < recordData.property_list.Count; i++)
            {
                record.SetFloat(recordData.property_list[i].row, recordData.property_list[i].col, (float)recordData.property_list[i].data);
            }
		}
		
		private void EGMI_ACK_RECORD_STRING(MsgHead head, MemoryStream stream)
		{
			NFMsg.ObjectRecordString recordData = new NFMsg.ObjectRecordString();	
			recordData = Serializer.Deserialize<NFMsg.ObjectRecordString>(stream);

            NFIObject go = mNet.kernel.GetObject(new NFIDENTID(recordData.player_id));
            NFIRecordManager recordManager = go.GetRecordManager();
            NFIRecord record = recordManager.GetRecord(recordData.record_name.ToString());

            for (int i = 0; i < recordData.property_list.Count; i++)
            {
                record.SetString(recordData.property_list[i].row, recordData.property_list[i].col, recordData.property_list[i].data.ToString());
            }
		}
		
		private void EGMI_ACK_RECORD_OBJECT(MsgHead head, MemoryStream stream)
		{
			NFMsg.ObjectRecordObject recordData = new NFMsg.ObjectRecordObject();	
			recordData = Serializer.Deserialize<NFMsg.ObjectRecordObject>(stream);

            NFIObject go = mNet.kernel.GetObject(new NFIDENTID(recordData.player_id));
            NFIRecordManager recordManager = go.GetRecordManager();
            NFIRecord record = recordManager.GetRecord(recordData.record_name.ToString());


            for (int i = 0; i < recordData.property_list.Count; i++)
            {
                record.SetObject(recordData.property_list[i].row, recordData.property_list[i].col, new NFIDENTID((Int64)recordData.property_list[i].data));
            }
		}
		
		private void EGMI_ACK_SWAP_ROW(MsgHead head, MemoryStream stream)
		{
			NFMsg.ObjectRecordSwap recordData = new NFMsg.ObjectRecordSwap();	
			recordData = Serializer.Deserialize<NFMsg.ObjectRecordSwap>(stream);

            NFIObject go = mNet.kernel.GetObject(new NFIDENTID(recordData.player_id));
            NFIRecordManager recordManager = go.GetRecordManager();
            NFIRecord record = recordManager.GetRecord(recordData.origin_record_name.ToString());


            //目前认为在同一张表中交换吧
            record.SwapRow(recordData.row_origin, recordData.row_target);
        
        }

        private void EGMI_ACK_ADD_ROW(MsgHead head, MemoryStream stream)
		{
			NFMsg.ObjectRecordAddRow recordData = new NFMsg.ObjectRecordAddRow();	
			recordData = Serializer.Deserialize<NFMsg.ObjectRecordAddRow>(stream);

            NFIObject go = mNet.kernel.GetObject(new NFIDENTID(recordData.player_id));
            NFIRecordManager recordManager = go.GetRecordManager();

            for (int i = 0; i < recordData.row_data.Count; i++)
            {
                NFMsg.RecordAddRowStruct addStruct = recordData.row_data[i];


                Hashtable recordVecDesc = new Hashtable();
                Hashtable recordVecData = new Hashtable();

                for (int k = 0; k < addStruct.record_int_list.Count; ++k)
                {
                    NFMsg.RecordInt addIntStruct = (NFMsg.RecordInt)addStruct.record_int_list[k];

                    if (addIntStruct.col >= 0)
                    {
                        recordVecDesc[addIntStruct.col] = NFIValueList.VARIANT_TYPE.VTYPE_INT;
                        recordVecData[addIntStruct.col] = addIntStruct.data;
                    }
                }

                for (int k = 0; k < addStruct.record_float_list.Count; ++k)
                {
                    NFMsg.RecordFloat addFloatStruct = (NFMsg.RecordFloat)addStruct.record_float_list[k];

                    if (addFloatStruct.col >= 0)
                    {
                        recordVecDesc[addFloatStruct.col] = NFIValueList.VARIANT_TYPE.VTYPE_FLOAT;
                        recordVecData[addFloatStruct.col] = addFloatStruct.data;

                    }
                }

                for (int k = 0; k < addStruct.record_string_list.Count; ++k)
                {
                    NFMsg.RecordString addStringStruct = (NFMsg.RecordString)addStruct.record_string_list[k];

                    if (addStringStruct.col >= 0)
                    {
                        recordVecDesc[addStringStruct.col] = NFIValueList.VARIANT_TYPE.VTYPE_STRING;
                        recordVecData[addStringStruct.col] = addStringStruct.data;

                    }
                }

                for (int k = 0; k < addStruct.record_object_list.Count; ++k)
                {
                    NFMsg.RecordObject addObjectStruct = (NFMsg.RecordObject)addStruct.record_object_list[k];

                    if (addObjectStruct.col >= 0)
                    {
                        recordVecDesc[addObjectStruct.col] = NFIValueList.VARIANT_TYPE.VTYPE_OBJECT;
                        recordVecData[addObjectStruct.col] = addObjectStruct.data;

                    }
                }

                NFIValueList varListDesc = new NFCValueList();
                NFIValueList varListData = new NFCValueList();
                for (int m = 0; m < recordVecDesc.Count; m++ )
                {
                    if (recordVecDesc.ContainsKey(m) && recordVecData.ContainsKey(m))
                    {
                        NFIValueList.VARIANT_TYPE nType = (NFIValueList.VARIANT_TYPE)recordVecDesc[m];
                        switch (nType)
                        {
                            case NFIValueList.VARIANT_TYPE.VTYPE_INT:
                                {
                                    varListDesc.AddInt(0);
                                    varListData.AddInt((int)recordVecData[m]);
                                }

                                break;
                            case NFIValueList.VARIANT_TYPE.VTYPE_FLOAT:
                                {
                                    varListDesc.AddFloat(0.0f);
                                    varListData.AddFloat((float)recordVecData[m]);
                                }
                                break;
                            case NFIValueList.VARIANT_TYPE.VTYPE_STRING:
                                {
                                    varListDesc.AddString("");
                                    varListData.AddString((string)recordVecData[m]);
                                }
                                break;
                            case NFIValueList.VARIANT_TYPE.VTYPE_OBJECT:
                                {
                                    varListDesc.AddObject(new NFIDENTID());
                                    long ident = long.Parse(recordVecData[m].ToString());
                                    varListData.AddObject(new NFIDENTID(ident));
                                }
                                break;
                            default:
                                break;

                        }
                    }
                    else
                    {
                        //报错
                        //Debug.LogException(i);
                    }
                }

                NFIRecord record = recordManager.GetRecord(recordData.record_name.ToString());                
                if(null == record)
                {
                    record = recordManager.AddRecord(recordData.record_name.ToString(), 512, varListDesc);
                }

                record.AddRow(addStruct.row, varListData);
	
            }
		}

        private void EGMI_ACK_REMOVE_ROW(MsgHead head, MemoryStream stream)
		{
			NFMsg.ObjectRecordRemove recordData = new NFMsg.ObjectRecordRemove();	
			recordData = Serializer.Deserialize<NFMsg.ObjectRecordRemove>(stream);

            NFIObject go = mNet.kernel.GetObject(new NFIDENTID(recordData.player_id));
            NFIRecordManager recordManager = go.GetRecordManager();
            NFIRecord record = recordManager.GetRecord(recordData.record_name.ToString());

            for (int i = 0; i < recordData.remove_row.Count; i++)
            {
                record.Remove(recordData.remove_row[i]);
            }
		}
// 
//         private void MGPT_CHAT_MESSAGE(MsgHead head, MemoryStream stream)
//         {
//             NFMsg.PlayersChat xMsg = new NFMsg.PlayersChat();
//             xMsg = Serializer.Deserialize<NFMsg.PlayersChat>(stream);
// 
// 
// 
//             for (int i = 0; i < xMsg.chat_data.Count; i++)
//             {
//                 NFMsg.ChatInfo xInfo = xMsg.chat_data[i];
//                 if (0 == xInfo.chat_type)
//                 {
//                     string strData = xInfo.other_name + "[" + xInfo.other_id + "]" + "对您说:" + xInfo.chat_info;
//                     aChatMsgList.Add(strData);
//                 }
//                 else if (2 == xInfo.chat_type)
//                 {
//                     string strData = xInfo.other_name + "[" + xInfo.other_id + "]" + "对世界说:" + xInfo.chat_info;
//                     aChatMsgList.Add(strData);
//                 }
//             }
//         }
    }

}