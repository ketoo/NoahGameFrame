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
    public class NFCPropertyLogic : NFCLogicBase
    {
        public enum Event : int
        {
        };
        private static NFCPropertyLogic _instance = null;
        public static NFCPropertyLogic Instance()
        {
            return _instance;
        }
        public NFCPropertyLogic(NFIPluginManager pluginManager)
        {
            _instance = this;
            mPluginManager = pluginManager;
		}

        public override bool AfterInit()
        {
            NFCNetLogic.Instance().AddReceiveCallBack(NFMsg.EGameMsgID.EGMI_ACK_PROPERTY_INT, OnPropertyInt);
            NFCNetLogic.Instance().AddReceiveCallBack(NFMsg.EGameMsgID.EGMI_ACK_PROPERTY_FLOAT, OnPropertyFloat);
            NFCNetLogic.Instance().AddReceiveCallBack(NFMsg.EGameMsgID.EGMI_ACK_PROPERTY_STRING, OnPropertyString);
            NFCNetLogic.Instance().AddReceiveCallBack(NFMsg.EGameMsgID.EGMI_ACK_PROPERTY_OBJECT, OnPropertyObject);
            NFCNetLogic.Instance().AddReceiveCallBack(NFMsg.EGameMsgID.EGMI_ACK_OBJECT_PROPERTY_ENTRY, OnObjectPropertyEntry);

            return true;
        }
        private void OnPropertyInt(UInt16 id, MemoryStream stream)
        {
            NFMsg.MsgBase xMsg = new NFMsg.MsgBase();
            xMsg = Serializer.Deserialize<NFMsg.MsgBase>(stream);

            NFMsg.ObjectPropertyInt propertyData = new NFMsg.ObjectPropertyInt();
            propertyData = Serializer.Deserialize<NFMsg.ObjectPropertyInt>(new MemoryStream(xMsg.msg_data));

            NFIObject go = NFCKernelModule.Instance().GetObject(PBToNF(propertyData.player_id));
            NFIPropertyManager propertyManager = go.GetPropertyManager();

            for (int i = 0; i < propertyData.property_list.Count; i++)
            {
                NFIProperty property = propertyManager.GetProperty(System.Text.Encoding.Default.GetString(propertyData.property_list[i].property_name));
                if (null == property)
                {
                    NFIDataList varList = new NFCDataList();
                    varList.AddInt(0);

                    property = propertyManager.AddProperty(System.Text.Encoding.Default.GetString(propertyData.property_list[i].property_name), varList);
                }

                property.SetInt(propertyData.property_list[i].data);
            }
        }
        private void OnPropertyFloat(UInt16 id, MemoryStream stream)
        {
            NFMsg.MsgBase xMsg = new NFMsg.MsgBase();
            xMsg = Serializer.Deserialize<NFMsg.MsgBase>(stream);

            NFMsg.ObjectPropertyFloat propertyData = new NFMsg.ObjectPropertyFloat();
            propertyData = Serializer.Deserialize<NFMsg.ObjectPropertyFloat>(new MemoryStream(xMsg.msg_data));

            NFIObject go = NFCKernelModule.Instance().GetObject(PBToNF(propertyData.player_id));

            for (int i = 0; i < propertyData.property_list.Count; i++)
            {
                NFIPropertyManager propertyManager = go.GetPropertyManager();
                NFIProperty property = propertyManager.GetProperty(System.Text.Encoding.Default.GetString(propertyData.property_list[i].property_name));
                if (null == property)
                {
                    NFIDataList varList = new NFCDataList();
                    varList.AddFloat(0.0f);

                    property = propertyManager.AddProperty(System.Text.Encoding.Default.GetString(propertyData.property_list[i].property_name), varList);
                }

                property.SetFloat(propertyData.property_list[i].data);
            }
        }
        private void OnPropertyString(UInt16 id, MemoryStream stream)
        {
            NFMsg.MsgBase xMsg = new NFMsg.MsgBase();
            xMsg = Serializer.Deserialize<NFMsg.MsgBase>(stream);

            NFMsg.ObjectPropertyString propertyData = new NFMsg.ObjectPropertyString();
            propertyData = Serializer.Deserialize<NFMsg.ObjectPropertyString>(new MemoryStream(xMsg.msg_data));

            NFIObject go = NFCKernelModule.Instance().GetObject(PBToNF(propertyData.player_id));

            for (int i = 0; i < propertyData.property_list.Count; i++)
            {
                NFIPropertyManager propertyManager = go.GetPropertyManager();
                NFIProperty property = propertyManager.GetProperty(System.Text.Encoding.Default.GetString(propertyData.property_list[i].property_name));
                if (null == property)
                {
                    NFIDataList varList = new NFCDataList();
                    varList.AddString("");

                    property = propertyManager.AddProperty(System.Text.Encoding.Default.GetString(propertyData.property_list[i].property_name), varList);
                }

                property.SetString(System.Text.Encoding.Default.GetString(propertyData.property_list[i].data));
            }
        }
        private void OnPropertyObject(UInt16 id, MemoryStream stream)
        {
            NFMsg.MsgBase xMsg = new NFMsg.MsgBase();
            xMsg = Serializer.Deserialize<NFMsg.MsgBase>(stream);

            NFMsg.ObjectPropertyObject propertyData = new NFMsg.ObjectPropertyObject();
            propertyData = Serializer.Deserialize<NFMsg.ObjectPropertyObject>(new MemoryStream(xMsg.msg_data));

            NFIObject go = NFCKernelModule.Instance().GetObject(PBToNF(propertyData.player_id));

            for (int i = 0; i < propertyData.property_list.Count; i++)
            {
                NFIPropertyManager propertyManager = go.GetPropertyManager();
                NFIProperty property = propertyManager.GetProperty(System.Text.Encoding.Default.GetString(propertyData.property_list[i].property_name));
                if (null == property)
                {
                    NFIDataList varList = new NFCDataList();
                    varList.AddObject(new NFGUID());

                    property = propertyManager.AddProperty(System.Text.Encoding.Default.GetString(propertyData.property_list[i].property_name), varList);
                }

                property.SetObject(PBToNF(propertyData.property_list[i].data));
            }
        }
        private void OnObjectPropertyEntry(UInt16 id, MemoryStream stream)
        {
            NFMsg.MsgBase xMsg = new NFMsg.MsgBase();
            xMsg = Serializer.Deserialize<NFMsg.MsgBase>(stream);

            NFMsg.MultiObjectPropertyList xMultiObjectPropertyList = new NFMsg.MultiObjectPropertyList();
            xMultiObjectPropertyList = Serializer.Deserialize<NFMsg.MultiObjectPropertyList>(new MemoryStream(xMsg.msg_data));

            for (int i = 0; i < xMultiObjectPropertyList.multi_player_property.Count; i++)
            {
                NFMsg.ObjectPropertyList xPropertyData = xMultiObjectPropertyList.multi_player_property[i];
                NFIObject go = NFCKernelModule.Instance().GetObject(PBToNF(xPropertyData.player_id));
                NFIPropertyManager xPropertyManager = go.GetPropertyManager();

                for (int j = 0; j < xPropertyData.property_int_list.Count; j++)
                {
                    string strPropertyName = System.Text.Encoding.Default.GetString(xPropertyData.property_int_list[j].property_name);
                    NFIProperty xProperty = xPropertyManager.GetProperty(strPropertyName);
                    if (null == xProperty)
                    {
                        NFIDataList varList = new NFCDataList();
                        varList.AddInt(0);

                        xProperty = xPropertyManager.AddProperty(strPropertyName, varList);
                    }

                    xProperty.SetInt(xPropertyData.property_int_list[j].data);
                }

                for (int j = 0; j < xPropertyData.property_float_list.Count; j++)
                {
                    string strPropertyName = System.Text.Encoding.Default.GetString(xPropertyData.property_float_list[j].property_name);
                    NFIProperty xProperty = xPropertyManager.GetProperty(strPropertyName);
                    if (null == xProperty)
                    {
                        NFIDataList varList = new NFCDataList();
                        varList.AddFloat(0);

                        xProperty = xPropertyManager.AddProperty(strPropertyName, varList);
                    }

                    xProperty.SetFloat(xPropertyData.property_float_list[j].data);
                }

                for (int j = 0; j < xPropertyData.property_string_list.Count; j++)
                {
                    string strPropertyName = System.Text.Encoding.Default.GetString(xPropertyData.property_string_list[j].property_name);
                    NFIProperty xProperty = xPropertyManager.GetProperty(strPropertyName);
                    if (null == xProperty)
                    {
                        NFIDataList varList = new NFCDataList();
                        varList.AddString("");

                        xProperty = xPropertyManager.AddProperty(strPropertyName, varList);
                    }

                    xProperty.SetString(System.Text.Encoding.Default.GetString(xPropertyData.property_string_list[j].data));
                }

                for (int j = 0; j < xPropertyData.property_object_list.Count; j++)
                {
                    string strPropertyName = System.Text.Encoding.Default.GetString(xPropertyData.property_object_list[j].property_name);
                    NFIProperty xProperty = xPropertyManager.GetProperty(strPropertyName);
                    if (null == xProperty)
                    {
                        NFIDataList varList = new NFCDataList();
                        varList.AddObject(new NFGUID());

                        xProperty = xPropertyManager.AddProperty(strPropertyName, varList);
                    }

                    xProperty.SetObject(PBToNF(xPropertyData.property_object_list[j].data));
                }
            }
        }
    };
}