using System;
using System.Linq;
using System.Text;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using NFMsg;
using UnityEngine;

namespace NFSDK
{
	public class NFRecordModule : NFIModule
    {
		private NFHelpModule mHelpModule;
        private NFNetModule mNetModule;
		private NFIKernelModule mKernelModule;
		private NFIClassModule mClassModule;

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

        public NFRecordModule(NFIPluginManager pluginManager)
        {
            mPluginManager = pluginManager;
		}

        public override void AfterInit()
        {
			mHelpModule = FindModule<NFHelpModule>();
			mNetModule = FindModule<NFNetModule>();
			mKernelModule = FindModule<NFIKernelModule>();

			mNetModule.AddReceiveCallBack(NFMsg.EGameMsgID.EGMI_ACK_RECORD_INT, OnRecordInt);
            mNetModule.AddReceiveCallBack(NFMsg.EGameMsgID.EGMI_ACK_RECORD_FLOAT, OnRecordFloat);
            mNetModule.AddReceiveCallBack(NFMsg.EGameMsgID.EGMI_ACK_RECORD_STRING, OnRecordString);
			mNetModule.AddReceiveCallBack(NFMsg.EGameMsgID.EGMI_ACK_RECORD_OBJECT, OnRecordObject);
			mNetModule.AddReceiveCallBack(NFMsg.EGameMsgID.EGMI_ACK_RECORD_VECTOR2, OnRecordVector2);
			mNetModule.AddReceiveCallBack(NFMsg.EGameMsgID.EGMI_ACK_RECORD_VECTOR3, OnRecordVector3);

            mNetModule.AddReceiveCallBack(NFMsg.EGameMsgID.EGMI_ACK_SWAP_ROW, OnSwapRow);
            mNetModule.AddReceiveCallBack(NFMsg.EGameMsgID.EGMI_ACK_ADD_ROW, OnAddRow);
            mNetModule.AddReceiveCallBack(NFMsg.EGameMsgID.EGMI_ACK_REMOVE_ROW, OnRemoveRow);

            mNetModule.AddReceiveCallBack(NFMsg.EGameMsgID.EGMI_ACK_OBJECT_RECORD_ENTRY, OnObjectRecordEntry);

        }

        private void ADD_ROW(NFGUID self, string strRecordName, NFMsg.RecordAddRowStruct xAddStruct)
        {
			NFIObject go = mKernelModule.GetObject(self);
            if (go == null)
            {
                return;
            }
            NFIRecordManager xRecordManager = go.GetRecordManager();

            Hashtable recordVecDesc = new Hashtable();
            Hashtable recordVecData = new Hashtable();

            for (int k = 0; k < xAddStruct.record_int_list.Count; ++k)
            {
                NFMsg.RecordInt addIntStruct = (NFMsg.RecordInt)xAddStruct.record_int_list[k];

                if (addIntStruct.col >= 0)
                {
                    recordVecDesc[addIntStruct.col] = NFDataList.VARIANT_TYPE.VTYPE_INT;
                    recordVecData[addIntStruct.col] = addIntStruct.data;
                }
            }

            for (int k = 0; k < xAddStruct.record_float_list.Count; ++k)
            {
                NFMsg.RecordFloat addFloatStruct = (NFMsg.RecordFloat)xAddStruct.record_float_list[k];

                if (addFloatStruct.col >= 0)
                {
                    recordVecDesc[addFloatStruct.col] = NFDataList.VARIANT_TYPE.VTYPE_FLOAT;
                    recordVecData[addFloatStruct.col] = addFloatStruct.data;

                }
            }

            for (int k = 0; k < xAddStruct.record_string_list.Count; ++k)
            {
                NFMsg.RecordString addStringStruct = (NFMsg.RecordString)xAddStruct.record_string_list[k];

                if (addStringStruct.col >= 0)
                {
                    recordVecDesc[addStringStruct.col] = NFDataList.VARIANT_TYPE.VTYPE_STRING;
                    recordVecData[addStringStruct.col] = addStringStruct.data.ToStringUtf8();

                }
            }

            for (int k = 0; k < xAddStruct.record_object_list.Count; ++k)
            {
                NFMsg.RecordObject addObjectStruct = (NFMsg.RecordObject)xAddStruct.record_object_list[k];

                if (addObjectStruct.col >= 0)
                {
                    recordVecDesc[addObjectStruct.col] = NFDataList.VARIANT_TYPE.VTYPE_OBJECT;
					recordVecData[addObjectStruct.col] = mHelpModule.PBToNF(addObjectStruct.data);

                }
            }

            for (int k = 0; k < xAddStruct.record_vector2_list.Count; ++k)
            {
                NFMsg.RecordVector2 addObjectStruct = (NFMsg.RecordVector2)xAddStruct.record_vector2_list[k];

                if (addObjectStruct.col >= 0)
                {
                    recordVecDesc[addObjectStruct.col] = NFDataList.VARIANT_TYPE.VTYPE_VECTOR2;
					recordVecData[addObjectStruct.col] = mHelpModule.PBToNF(addObjectStruct.data);

                }
            }

            for (int k = 0; k < xAddStruct.record_vector3_list.Count; ++k)
            {
                NFMsg.RecordVector3 addObjectStruct = (NFMsg.RecordVector3)xAddStruct.record_vector3_list[k];

                if (addObjectStruct.col >= 0)
                {
                    recordVecDesc[addObjectStruct.col] = NFDataList.VARIANT_TYPE.VTYPE_VECTOR3;
					recordVecData[addObjectStruct.col] = mHelpModule.PBToNF(addObjectStruct.data);

                }
            }

            NFDataList varListDesc = new NFDataList();
            NFDataList varListData = new NFDataList();
            for (int m = 0; m < recordVecDesc.Count; m++)
            {
                if (recordVecDesc.ContainsKey(m) && recordVecData.ContainsKey(m))
                {
                    NFDataList.VARIANT_TYPE nType = (NFDataList.VARIANT_TYPE)recordVecDesc[m];
                    switch (nType)
                    {
                        case NFDataList.VARIANT_TYPE.VTYPE_INT:
                            {
                                varListDesc.AddInt(0);
                                varListData.AddInt((Int64)recordVecData[m]);
                            }

                            break;
                        case NFDataList.VARIANT_TYPE.VTYPE_FLOAT:
                            {
                                varListDesc.AddFloat(0.0f);
                                varListData.AddFloat((float)recordVecData[m]);
                            }
                            break;
                        case NFDataList.VARIANT_TYPE.VTYPE_STRING:
                            {
                                varListDesc.AddString("");
                                varListData.AddString((string)recordVecData[m]);
                            }
                            break;
                        case NFDataList.VARIANT_TYPE.VTYPE_OBJECT:
                            {
                                varListDesc.AddObject(new NFGUID());
                                varListData.AddObject((NFGUID)recordVecData[m]);
                            }
                            break;
                        case NFDataList.VARIANT_TYPE.VTYPE_VECTOR2:
                            {
                                varListDesc.AddVector2(new NFVector2());
                                varListData.AddVector2((NFVector2)recordVecData[m]);
                            }
                            break;
                        case NFDataList.VARIANT_TYPE.VTYPE_VECTOR3:
                            {
                                varListDesc.AddVector3(new NFVector3());
                                varListData.AddVector3((NFVector3)recordVecData[m]);
                            }
                            break;
                        default:
                            break;

                    }
                }
                else
                {
                    //����
                    //Debug.LogException(i);
                }
            }

            NFIRecord xRecord = xRecordManager.GetRecord(strRecordName);
            if (null == xRecord)
            {
                string strClassName = mKernelModule.QueryPropertyString(self, NFrame.IObject.ClassName);
				NFIClass xLogicClass = mClassModule.GetElement(strClassName);
                NFIRecord xStaticRecord = xLogicClass.GetRecordManager().GetRecord(strRecordName);

                xRecord = xRecordManager.AddRecord(strRecordName, 512, varListDesc, xStaticRecord.GetTagData());
            }

            xRecord.AddRow(xAddStruct.row, varListData);
        }

        private void OnRecordInt(UInt16 id, MemoryStream stream)
        {
            NFMsg.MsgBase xMsg = NFMsg.MsgBase.Parser.ParseFrom(stream);

            NFMsg.ObjectRecordInt recordData = NFMsg.ObjectRecordInt.Parser.ParseFrom(xMsg.msg_data);

			NFIObject go = mKernelModule.GetObject(mHelpModule.PBToNF(recordData.player_id));
            NFIRecordManager recordManager = go.GetRecordManager();
            NFIRecord record = recordManager.GetRecord(recordData.record_name.ToStringUtf8());

            for (int i = 0; i < recordData.property_list.Count; i++)
            {
                record.SetInt(recordData.property_list[i].row, recordData.property_list[i].col, (int)recordData.property_list[i].data);
            }
        }
        private void OnRecordFloat(UInt16 id, MemoryStream stream)
        {
            NFMsg.MsgBase xMsg = NFMsg.MsgBase.Parser.ParseFrom(stream);

            NFMsg.ObjectRecordFloat recordData = NFMsg.ObjectRecordFloat.Parser.ParseFrom(xMsg.msg_data);

			NFIObject go = mKernelModule.GetObject(mHelpModule.PBToNF(recordData.player_id));
            NFIRecordManager recordManager = go.GetRecordManager();
            NFIRecord record = recordManager.GetRecord(recordData.record_name.ToStringUtf8());

            for (int i = 0; i < recordData.property_list.Count; i++)
            {
                record.SetFloat(recordData.property_list[i].row, recordData.property_list[i].col, (float)recordData.property_list[i].data);
            }
        }
        private void OnRecordString(UInt16 id, MemoryStream stream)
        {
            NFMsg.MsgBase xMsg = NFMsg.MsgBase.Parser.ParseFrom(stream);

            NFMsg.ObjectRecordString recordData = NFMsg.ObjectRecordString.Parser.ParseFrom(xMsg.msg_data);

			NFIObject go = mKernelModule.GetObject(mHelpModule.PBToNF(recordData.player_id));
            NFIRecordManager recordManager = go.GetRecordManager();
            NFIRecord record = recordManager.GetRecord(recordData.record_name.ToStringUtf8());

            for (int i = 0; i < recordData.property_list.Count; i++)
            {
                record.SetString(recordData.property_list[i].row, recordData.property_list[i].col, recordData.property_list[i].data.ToStringUtf8());
            }
        }
        private void OnRecordObject(UInt16 id, MemoryStream stream)
        {
            NFMsg.MsgBase xMsg = NFMsg.MsgBase.Parser.ParseFrom(stream);

            NFMsg.ObjectRecordObject recordData = NFMsg.ObjectRecordObject.Parser.ParseFrom(xMsg.msg_data);

			NFIObject go = mKernelModule.GetObject(mHelpModule.PBToNF(recordData.player_id));
            NFIRecordManager recordManager = go.GetRecordManager();
            NFIRecord record = recordManager.GetRecord(recordData.record_name.ToStringUtf8());


            for (int i = 0; i < recordData.property_list.Count; i++)
            {
				record.SetObject(recordData.property_list[i].row, recordData.property_list[i].col, mHelpModule.PBToNF(recordData.property_list[i].data));
            }
        }

		private void OnRecordVector2(UInt16 id, MemoryStream stream)
        {
            NFMsg.MsgBase xMsg = NFMsg.MsgBase.Parser.ParseFrom(stream);

            NFMsg.ObjectRecordVector2 recordData = NFMsg.ObjectRecordVector2.Parser.ParseFrom(xMsg.msg_data);

            NFIObject go = mKernelModule.GetObject(mHelpModule.PBToNF(recordData.player_id));
            NFIRecordManager recordManager = go.GetRecordManager();
            NFIRecord record = recordManager.GetRecord(recordData.record_name.ToStringUtf8());


            for (int i = 0; i < recordData.property_list.Count; i++)
            {
                record.SetVector2(recordData.property_list[i].row, recordData.property_list[i].col, mHelpModule.PBToNF(recordData.property_list[i].data));
            }
        }

		private void OnRecordVector3(UInt16 id, MemoryStream stream)
        {
            NFMsg.MsgBase xMsg = NFMsg.MsgBase.Parser.ParseFrom(stream);

            NFMsg.ObjectRecordVector3 recordData = NFMsg.ObjectRecordVector3.Parser.ParseFrom(xMsg.msg_data);

            NFIObject go = mKernelModule.GetObject(mHelpModule.PBToNF(recordData.player_id));
            NFIRecordManager recordManager = go.GetRecordManager();
            NFIRecord record = recordManager.GetRecord(recordData.record_name.ToStringUtf8());


            for (int i = 0; i < recordData.property_list.Count; i++)
            {
                record.SetVector3(recordData.property_list[i].row, recordData.property_list[i].col, mHelpModule.PBToNF(recordData.property_list[i].data));
            }
        }


        private void OnSwapRow(UInt16 id, MemoryStream stream)
        {
            NFMsg.MsgBase xMsg = NFMsg.MsgBase.Parser.ParseFrom(stream);

            NFMsg.ObjectRecordSwap recordData = NFMsg.ObjectRecordSwap.Parser.ParseFrom(xMsg.msg_data);

			NFIObject go = mKernelModule.GetObject(mHelpModule.PBToNF(recordData.player_id));
            NFIRecordManager recordManager = go.GetRecordManager();
            NFIRecord record = recordManager.GetRecord(recordData.origin_record_name.ToStringUtf8());


            //目前认为在同一张表中交换吧
            record.SwapRow(recordData.row_origin, recordData.row_target);
        }
        private void OnAddRow(UInt16 id, MemoryStream stream)
        {
            NFMsg.MsgBase xMsg = NFMsg.MsgBase.Parser.ParseFrom(stream);

            NFMsg.ObjectRecordAddRow recordData = NFMsg.ObjectRecordAddRow.Parser.ParseFrom(xMsg.msg_data);

			NFIObject go = mKernelModule.GetObject(mHelpModule.PBToNF(recordData.player_id));
            NFIRecordManager recordManager = go.GetRecordManager();

            for (int i = 0; i < recordData.row_data.Count; i++)
            {
				ADD_ROW(mHelpModule.PBToNF(recordData.player_id), recordData.record_name.ToStringUtf8(), recordData.row_data[i]);
            }
        }
        private void OnRemoveRow(UInt16 id, MemoryStream stream)
        {
            NFMsg.MsgBase xMsg = NFMsg.MsgBase.Parser.ParseFrom(stream);

            NFMsg.ObjectRecordRemove recordData = NFMsg.ObjectRecordRemove.Parser.ParseFrom(xMsg.msg_data);

			NFIObject go = mKernelModule.GetObject(mHelpModule.PBToNF(recordData.player_id));
            NFIRecordManager recordManager = go.GetRecordManager();
            NFIRecord record = recordManager.GetRecord(recordData.record_name.ToStringUtf8());

            for (int i = 0; i < recordData.remove_row.Count; i++)
            {
                record.Remove(recordData.remove_row[i]);
            }
        }
        private void OnObjectRecordEntry(UInt16 id, MemoryStream stream)
        {
            NFMsg.MsgBase xMsg = NFMsg.MsgBase.Parser.ParseFrom(stream);

            NFMsg.MultiObjectRecordList xMultiObjectRecordData = NFMsg.MultiObjectRecordList.Parser.ParseFrom(xMsg.msg_data);

            for (int i = 0; i < xMultiObjectRecordData.multi_player_record.Count; i++)
            {
                NFMsg.ObjectRecordList xObjectRecordList = xMultiObjectRecordData.multi_player_record[i];
                for (int j = 0; j < xObjectRecordList.record_list.Count; j++)
                {
                    NFMsg.ObjectRecordBase xObjectRecordBase = xObjectRecordList.record_list[j];
                    for (int k = 0; k < xObjectRecordBase.row_struct.Count; ++k)
                    {
                        NFMsg.RecordAddRowStruct xAddRowStruct = xObjectRecordBase.row_struct[k];

						ADD_ROW(mHelpModule.PBToNF(xObjectRecordList.player_id), xObjectRecordBase.record_name.ToStringUtf8(), xAddRowStruct);
                    }
                }
            }
        }
    };
}