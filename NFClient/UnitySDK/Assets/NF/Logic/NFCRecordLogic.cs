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
    public class NFCRecordyLogic : NFCLogicBase
    {
        public enum Event : int
        {
        };
        private static NFCRecordyLogic _instance = null;
        public static NFCRecordyLogic Instance()
        {
            return _instance;
        }
        public NFCRecordyLogic(NFIPluginManager pluginManager)
        {
            _instance = this;
            mPluginManager = pluginManager;
		}

        public override bool AfterInit()
        {
            NFCNetLogic.Instance().AddReceiveCallBack(NFMsg.EGameMsgID.EGMI_ACK_RECORD_INT, OnRecordInt);
            NFCNetLogic.Instance().AddReceiveCallBack(NFMsg.EGameMsgID.EGMI_ACK_RECORD_FLOAT, OnRecordFloat);
            NFCNetLogic.Instance().AddReceiveCallBack(NFMsg.EGameMsgID.EGMI_ACK_RECORD_STRING, OnRecordString);
            NFCNetLogic.Instance().AddReceiveCallBack(NFMsg.EGameMsgID.EGMI_ACK_RECORD_OBJECT, OnRecordObject);
            NFCNetLogic.Instance().AddReceiveCallBack(NFMsg.EGameMsgID.EGMI_ACK_SWAP_ROW, OnSwapRow);
            NFCNetLogic.Instance().AddReceiveCallBack(NFMsg.EGameMsgID.EGMI_ACK_ADD_ROW, OnAddRow);
            NFCNetLogic.Instance().AddReceiveCallBack(NFMsg.EGameMsgID.EGMI_ACK_REMOVE_ROW, OnRemoveRow);
            NFCNetLogic.Instance().AddReceiveCallBack(NFMsg.EGameMsgID.EGMI_ACK_OBJECT_RECORD_ENTRY, OnObjectRecordEntry);

            return true;
        }

        private void ADD_ROW(NFGUID self, string strRecordName, NFMsg.RecordAddRowStruct xAddStruct)
        {
            NFIObject go = NFCKernelModule.Instance().GetObject(self);
            NFIRecordManager xRecordManager = go.GetRecordManager();


            Hashtable recordVecDesc = new Hashtable();
            Hashtable recordVecData = new Hashtable();

            for (int k = 0; k < xAddStruct.record_int_list.Count; ++k)
            {
                NFMsg.RecordInt addIntStruct = (NFMsg.RecordInt)xAddStruct.record_int_list[k];

                if (addIntStruct.col >= 0)
                {
                    recordVecDesc[addIntStruct.col] = NFIDataList.VARIANT_TYPE.VTYPE_INT;
                    recordVecData[addIntStruct.col] = addIntStruct.data;
                }
            }

            for (int k = 0; k < xAddStruct.record_float_list.Count; ++k)
            {
                NFMsg.RecordFloat addFloatStruct = (NFMsg.RecordFloat)xAddStruct.record_float_list[k];

                if (addFloatStruct.col >= 0)
                {
                    recordVecDesc[addFloatStruct.col] = NFIDataList.VARIANT_TYPE.VTYPE_FLOAT;
                    recordVecData[addFloatStruct.col] = addFloatStruct.data;

                }
            }

            for (int k = 0; k < xAddStruct.record_string_list.Count; ++k)
            {
                NFMsg.RecordString addStringStruct = (NFMsg.RecordString)xAddStruct.record_string_list[k];

                if (addStringStruct.col >= 0)
                {
                    recordVecDesc[addStringStruct.col] = NFIDataList.VARIANT_TYPE.VTYPE_STRING;
                    recordVecData[addStringStruct.col] = System.Text.Encoding.Default.GetString(addStringStruct.data);

                }
            }

            for (int k = 0; k < xAddStruct.record_object_list.Count; ++k)
            {
                NFMsg.RecordObject addObjectStruct = (NFMsg.RecordObject)xAddStruct.record_object_list[k];

                if (addObjectStruct.col >= 0)
                {
                    recordVecDesc[addObjectStruct.col] = NFIDataList.VARIANT_TYPE.VTYPE_OBJECT;
                    recordVecData[addObjectStruct.col] = PBToNF(addObjectStruct.data);

                }
            }

            NFIDataList varListDesc = new NFCDataList();
            NFIDataList varListData = new NFCDataList();
            for (int m = 0; m < recordVecDesc.Count; m++)
            {
                if (recordVecDesc.ContainsKey(m) && recordVecData.ContainsKey(m))
                {
                    NFIDataList.VARIANT_TYPE nType = (NFIDataList.VARIANT_TYPE)recordVecDesc[m];
                    switch (nType)
                    {
                        case NFIDataList.VARIANT_TYPE.VTYPE_INT:
                            {
                                varListDesc.AddInt(0);
                                varListData.AddInt((Int64)recordVecData[m]);
                            }

                            break;
                        case NFIDataList.VARIANT_TYPE.VTYPE_FLOAT:
                            {
                                varListDesc.AddFloat(0.0f);
                                varListData.AddFloat((float)recordVecData[m]);
                            }
                            break;
                        case NFIDataList.VARIANT_TYPE.VTYPE_STRING:
                            {
                                varListDesc.AddString("");
                                varListData.AddString((string)recordVecData[m]);
                            }
                            break;
                        case NFIDataList.VARIANT_TYPE.VTYPE_OBJECT:
                            {
                                varListDesc.AddObject(new NFGUID());
                                varListData.AddObject((NFGUID)recordVecData[m]);
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

            NFIRecord xRecord = xRecordManager.GetRecord(strRecordName);
            if (null == xRecord)
            {
                xRecord = xRecordManager.AddRecord(strRecordName, 512, varListDesc);
            }

            xRecord.AddRow(xAddStruct.row, varListData);
        }

        private void OnRecordInt(UInt16 id, MemoryStream stream)
        {
            NFMsg.MsgBase xMsg = new NFMsg.MsgBase();
            xMsg = Serializer.Deserialize<NFMsg.MsgBase>(stream);

            NFMsg.ObjectRecordInt recordData = new NFMsg.ObjectRecordInt();
            recordData = Serializer.Deserialize<NFMsg.ObjectRecordInt>(new MemoryStream(xMsg.msg_data));

            NFIObject go = NFCKernelModule.Instance().GetObject(PBToNF(recordData.player_id));
            NFIRecordManager recordManager = go.GetRecordManager();
            NFIRecord record = recordManager.GetRecord(System.Text.Encoding.Default.GetString(recordData.record_name));

            for (int i = 0; i < recordData.property_list.Count; i++)
            {
                record.SetInt(recordData.property_list[i].row, recordData.property_list[i].col, (int)recordData.property_list[i].data);
            }
        }
        private void OnRecordFloat(UInt16 id, MemoryStream stream)
        {
            NFMsg.MsgBase xMsg = new NFMsg.MsgBase();
            xMsg = Serializer.Deserialize<NFMsg.MsgBase>(stream);

            NFMsg.ObjectRecordFloat recordData = new NFMsg.ObjectRecordFloat();
            recordData = Serializer.Deserialize<NFMsg.ObjectRecordFloat>(new MemoryStream(xMsg.msg_data));

            NFIObject go = NFCKernelModule.Instance().GetObject(PBToNF(recordData.player_id));
            NFIRecordManager recordManager = go.GetRecordManager();
            NFIRecord record = recordManager.GetRecord(System.Text.Encoding.Default.GetString(recordData.record_name));

            for (int i = 0; i < recordData.property_list.Count; i++)
            {
                record.SetFloat(recordData.property_list[i].row, recordData.property_list[i].col, (float)recordData.property_list[i].data);
            }
        }
        private void OnRecordString(UInt16 id, MemoryStream stream)
        {
            NFMsg.MsgBase xMsg = new NFMsg.MsgBase();
            xMsg = Serializer.Deserialize<NFMsg.MsgBase>(stream);

            NFMsg.ObjectRecordString recordData = new NFMsg.ObjectRecordString();
            recordData = Serializer.Deserialize<NFMsg.ObjectRecordString>(new MemoryStream(xMsg.msg_data));

            NFIObject go = NFCKernelModule.Instance().GetObject(PBToNF(recordData.player_id));
            NFIRecordManager recordManager = go.GetRecordManager();
            NFIRecord record = recordManager.GetRecord(System.Text.Encoding.Default.GetString(recordData.record_name));

            for (int i = 0; i < recordData.property_list.Count; i++)
            {
                record.SetString(recordData.property_list[i].row, recordData.property_list[i].col, System.Text.Encoding.Default.GetString(recordData.property_list[i].data));
            }
        }
        private void OnRecordObject(UInt16 id, MemoryStream stream)
        {
            NFMsg.MsgBase xMsg = new NFMsg.MsgBase();
            xMsg = Serializer.Deserialize<NFMsg.MsgBase>(stream);

            NFMsg.ObjectRecordObject recordData = new NFMsg.ObjectRecordObject();
            recordData = Serializer.Deserialize<NFMsg.ObjectRecordObject>(new MemoryStream(xMsg.msg_data));

            NFIObject go = NFCKernelModule.Instance().GetObject(PBToNF(recordData.player_id));
            NFIRecordManager recordManager = go.GetRecordManager();
            NFIRecord record = recordManager.GetRecord(System.Text.Encoding.Default.GetString(recordData.record_name));


            for (int i = 0; i < recordData.property_list.Count; i++)
            {
                record.SetObject(recordData.property_list[i].row, recordData.property_list[i].col, PBToNF(recordData.property_list[i].data));
            }
        }
        private void OnSwapRow(UInt16 id, MemoryStream stream)
        {
            NFMsg.MsgBase xMsg = new NFMsg.MsgBase();
            xMsg = Serializer.Deserialize<NFMsg.MsgBase>(stream);

            NFMsg.ObjectRecordSwap recordData = new NFMsg.ObjectRecordSwap();
            recordData = Serializer.Deserialize<NFMsg.ObjectRecordSwap>(new MemoryStream(xMsg.msg_data));

            NFIObject go = NFCKernelModule.Instance().GetObject(PBToNF(recordData.player_id));
            NFIRecordManager recordManager = go.GetRecordManager();
            NFIRecord record = recordManager.GetRecord(System.Text.Encoding.Default.GetString(recordData.origin_record_name));


            //目前认为在同一张表中交换吧
            record.SwapRow(recordData.row_origin, recordData.row_target);
        }
        private void OnAddRow(UInt16 id, MemoryStream stream)
        {
            NFMsg.MsgBase xMsg = new NFMsg.MsgBase();
            xMsg = Serializer.Deserialize<NFMsg.MsgBase>(stream);

            NFMsg.ObjectRecordAddRow recordData = new NFMsg.ObjectRecordAddRow();
            recordData = Serializer.Deserialize<NFMsg.ObjectRecordAddRow>(new MemoryStream(xMsg.msg_data));

            NFIObject go = NFCKernelModule.Instance().GetObject(PBToNF(recordData.player_id));
            NFIRecordManager recordManager = go.GetRecordManager();

            for (int i = 0; i < recordData.row_data.Count; i++)
            {
                ADD_ROW(PBToNF(recordData.player_id), System.Text.Encoding.Default.GetString(recordData.record_name), recordData.row_data[i]);
            }
        }
        private void OnRemoveRow(UInt16 id, MemoryStream stream)
        {
            NFMsg.MsgBase xMsg = new NFMsg.MsgBase();
            xMsg = Serializer.Deserialize<NFMsg.MsgBase>(stream);

            NFMsg.ObjectRecordRemove recordData = new NFMsg.ObjectRecordRemove();
            recordData = Serializer.Deserialize<NFMsg.ObjectRecordRemove>(new MemoryStream(xMsg.msg_data));

            NFIObject go = NFCKernelModule.Instance().GetObject(PBToNF(recordData.player_id));
            NFIRecordManager recordManager = go.GetRecordManager();
            NFIRecord record = recordManager.GetRecord(System.Text.Encoding.Default.GetString(recordData.record_name));

            for (int i = 0; i < recordData.remove_row.Count; i++)
            {
                record.Remove(recordData.remove_row[i]);
            }
        }
        private void OnObjectRecordEntry(UInt16 id, MemoryStream stream)
        {
            NFMsg.MsgBase xMsg = new NFMsg.MsgBase();
            xMsg = Serializer.Deserialize<NFMsg.MsgBase>(stream);

            NFMsg.MultiObjectRecordList xMultiObjectRecordData = new NFMsg.MultiObjectRecordList();
            xMultiObjectRecordData = Serializer.Deserialize<NFMsg.MultiObjectRecordList>(new MemoryStream(xMsg.msg_data));

            for (int i = 0; i < xMultiObjectRecordData.multi_player_record.Count; i++)
            {
                NFMsg.ObjectRecordList xObjectRecordList = xMultiObjectRecordData.multi_player_record[i];
                for (int j = 0; j < xObjectRecordList.record_list.Count; j++)
                {
                    NFMsg.ObjectRecordBase xObjectRecordBase = xObjectRecordList.record_list[j];
                    for (int k = 0; k < xObjectRecordBase.row_struct.Count; ++k)
                    {
                        NFMsg.RecordAddRowStruct xAddRowStruct = xObjectRecordBase.row_struct[k];

                        ADD_ROW(PBToNF(xObjectRecordList.player_id), System.Text.Encoding.Default.GetString(xObjectRecordBase.record_name), xAddRowStruct);
                    }
                }
            }
        }
    };
}