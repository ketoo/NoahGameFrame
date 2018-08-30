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
	public class NFPropertyModule : NFIModule
    {
		private NFHelpModule mHelpModule;
		private NFNetModule mNetModule;
		private NFIKernelModule mKernelModule;

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

        public NFPropertyModule(NFIPluginManager pluginManager)
        {
            mPluginManager = pluginManager;
		}

        public override void AfterInit()
        {
			mHelpModule = FindModule<NFHelpModule>();
			mNetModule = FindModule<NFNetModule>();
			mKernelModule = FindModule<NFIKernelModule>();

            mNetModule.AddReceiveCallBack(NFMsg.EGameMsgID.EGMI_ACK_PROPERTY_INT, OnPropertyInt);
            mNetModule.AddReceiveCallBack(NFMsg.EGameMsgID.EGMI_ACK_PROPERTY_FLOAT, OnPropertyFloat);
            mNetModule.AddReceiveCallBack(NFMsg.EGameMsgID.EGMI_ACK_PROPERTY_STRING, OnPropertyString);
			mNetModule.AddReceiveCallBack(NFMsg.EGameMsgID.EGMI_ACK_PROPERTY_OBJECT, OnPropertyObject);
			mNetModule.AddReceiveCallBack(NFMsg.EGameMsgID.EGMI_ACK_PROPERTY_VECTOR2, OnPropertyVector2);
			mNetModule.AddReceiveCallBack(NFMsg.EGameMsgID.EGMI_ACK_PROPERTY_VECTOR3, OnPropertyVector3);

            mNetModule.AddReceiveCallBack(NFMsg.EGameMsgID.EGMI_ACK_OBJECT_PROPERTY_ENTRY, OnObjectPropertyEntry);

        }
		/////////////////////////////////////////////////////////////////////      

        private void OnPropertyInt(UInt16 id, MemoryStream stream)
        {
            NFMsg.MsgBase xMsg = NFMsg.MsgBase.Parser.ParseFrom(stream);

            NFMsg.ObjectPropertyInt propertyData = NFMsg.ObjectPropertyInt.Parser.ParseFrom(xMsg.msg_data);

			NFIObject go = mKernelModule.GetObject(mHelpModule.PBToNF(propertyData.player_id));
            NFIPropertyManager propertyManager = go.GetPropertyManager();

            for (int i = 0; i < propertyData.property_list.Count; i++)
            {
                NFIProperty property = propertyManager.GetProperty(propertyData.property_list[i].property_name.ToStringUtf8());
                if (null == property)
                {
                    NFDataList varList = new NFDataList();
                    varList.AddInt(0);

                    property = propertyManager.AddProperty(propertyData.property_list[i].property_name.ToStringUtf8(), varList);
                }

                property.SetInt(propertyData.property_list[i].data);
            }
        }
        private void OnPropertyFloat(UInt16 id, MemoryStream stream)
        {
            NFMsg.MsgBase xMsg = NFMsg.MsgBase.Parser.ParseFrom(stream);

            NFMsg.ObjectPropertyFloat propertyData = NFMsg.ObjectPropertyFloat.Parser.ParseFrom(xMsg.msg_data);

			NFIObject go = mKernelModule.GetObject(mHelpModule.PBToNF(propertyData.player_id));

            for (int i = 0; i < propertyData.property_list.Count; i++)
            {
                NFIPropertyManager propertyManager = go.GetPropertyManager();
                NFIProperty property = propertyManager.GetProperty(propertyData.property_list[i].property_name.ToStringUtf8());
                if (null == property)
                {
                    NFDataList varList = new NFDataList();
                    varList.AddFloat(0.0f);

                    property = propertyManager.AddProperty(propertyData.property_list[i].property_name.ToStringUtf8(), varList);
                }

                property.SetFloat(propertyData.property_list[i].data);
            }
        }
        private void OnPropertyString(UInt16 id, MemoryStream stream)
        {
            NFMsg.MsgBase xMsg = NFMsg.MsgBase.Parser.ParseFrom(stream);

            NFMsg.ObjectPropertyString propertyData = NFMsg.ObjectPropertyString.Parser.ParseFrom(xMsg.msg_data);

			NFIObject go = mKernelModule.GetObject(mHelpModule.PBToNF(propertyData.player_id));

            for (int i = 0; i < propertyData.property_list.Count; i++)
            {
                NFIPropertyManager propertyManager = go.GetPropertyManager();
                NFIProperty property = propertyManager.GetProperty(propertyData.property_list[i].property_name.ToStringUtf8());
                if (null == property)
                {
                    NFDataList varList = new NFDataList();
                    varList.AddString("");

                    property = propertyManager.AddProperty(propertyData.property_list[i].property_name.ToStringUtf8(), varList);
                }

                property.SetString(propertyData.property_list[i].data.ToStringUtf8());
            }
        }
        private void OnPropertyObject(UInt16 id, MemoryStream stream)
        {
            NFMsg.MsgBase xMsg = NFMsg.MsgBase.Parser.ParseFrom(stream);

            NFMsg.ObjectPropertyObject propertyData = NFMsg.ObjectPropertyObject.Parser.ParseFrom(xMsg.msg_data);

			NFIObject go = mKernelModule.GetObject(mHelpModule.PBToNF(propertyData.player_id));

            for (int i = 0; i < propertyData.property_list.Count; i++)
            {
                NFIPropertyManager propertyManager = go.GetPropertyManager();
                NFIProperty property = propertyManager.GetProperty(propertyData.property_list[i].property_name.ToStringUtf8());
                if (null == property)
                {
                    NFDataList varList = new NFDataList();
                    varList.AddObject(new NFGUID());

                    property = propertyManager.AddProperty(propertyData.property_list[i].property_name.ToStringUtf8(), varList);
                }

				property.SetObject(mHelpModule.PBToNF(propertyData.property_list[i].data));
            }
        }

		private void OnPropertyVector2(UInt16 id, MemoryStream stream)
        {
            NFMsg.MsgBase xMsg = NFMsg.MsgBase.Parser.ParseFrom(stream);

            NFMsg.ObjectPropertyVector2 propertyData = NFMsg.ObjectPropertyVector2.Parser.ParseFrom(xMsg.msg_data);

            NFIObject go = mKernelModule.GetObject(mHelpModule.PBToNF(propertyData.player_id));

			for (int i = 0; i < propertyData.property_list.Count; i++)
            {
                NFIPropertyManager propertyManager = go.GetPropertyManager();
				NFIProperty property = propertyManager.GetProperty(propertyData.property_list[i].property_name.ToStringUtf8());
                if (null == property)
                {
                    NFDataList varList = new NFDataList();
                    varList.AddVector2(new NFVector2());

					property = propertyManager.AddProperty(propertyData.property_list[i].property_name.ToStringUtf8(), varList);
                }

				property.SetVector2(mHelpModule.PBToNF(propertyData.property_list[i].data));
            }
        }

		private void OnPropertyVector3(UInt16 id, MemoryStream stream)
        {
            NFMsg.MsgBase xMsg = NFMsg.MsgBase.Parser.ParseFrom(stream);

            NFMsg.ObjectPropertyVector3 propertyData = NFMsg.ObjectPropertyVector3.Parser.ParseFrom(xMsg.msg_data);

            NFIObject go = mKernelModule.GetObject(mHelpModule.PBToNF(propertyData.player_id));

			for (int i = 0; i < propertyData.property_list.Count; i++)
            {
                NFIPropertyManager propertyManager = go.GetPropertyManager();
				NFIProperty property = propertyManager.GetProperty(propertyData.property_list[i].property_name.ToStringUtf8());
                if (null == property)
                {
                    NFDataList varList = new NFDataList();
                    varList.AddVector3(new NFVector3());

					property = propertyManager.AddProperty(propertyData.property_list[i].property_name.ToStringUtf8(), varList);
                }

				property.SetVector3(mHelpModule.PBToNF(propertyData.property_list[i].data));
            }
        }

        internal void OnObjectPropertyEntry(UInt16 id, MemoryStream stream)
        {
            NFMsg.MsgBase xMsg = NFMsg.MsgBase.Parser.ParseFrom(stream);

            NFMsg.MultiObjectPropertyList xMultiObjectPropertyList = NFMsg.MultiObjectPropertyList.Parser.ParseFrom(xMsg.msg_data);

            for (int i = 0; i < xMultiObjectPropertyList.multi_player_property.Count; i++)
            {
                NFMsg.ObjectPropertyList xPropertyData = xMultiObjectPropertyList.multi_player_property[i];
				NFIObject go = mKernelModule.GetObject(mHelpModule.PBToNF(xPropertyData.player_id));
                NFIPropertyManager xPropertyManager = go.GetPropertyManager();

                for (int j = 0; j < xPropertyData.property_int_list.Count; j++)
                {
                    string strPropertyName = xPropertyData.property_int_list[j].property_name.ToStringUtf8();
                    NFIProperty xProperty = xPropertyManager.GetProperty(strPropertyName);
                    if (null == xProperty)
                    {
                        NFDataList varList = new NFDataList();
                        varList.AddInt(0);

                        xProperty = xPropertyManager.AddProperty(strPropertyName, varList);
                    }

                    xProperty.SetInt(xPropertyData.property_int_list[j].data);
                }

                for (int j = 0; j < xPropertyData.property_float_list.Count; j++)
                {
                    string strPropertyName = xPropertyData.property_float_list[j].property_name.ToStringUtf8();
                    NFIProperty xProperty = xPropertyManager.GetProperty(strPropertyName);
                    if (null == xProperty)
                    {
                        NFDataList varList = new NFDataList();
                        varList.AddFloat(0);

                        xProperty = xPropertyManager.AddProperty(strPropertyName, varList);
                    }

                    xProperty.SetFloat(xPropertyData.property_float_list[j].data);
                }

                for (int j = 0; j < xPropertyData.property_string_list.Count; j++)
                {
                    string strPropertyName = xPropertyData.property_string_list[j].property_name.ToStringUtf8();
                    NFIProperty xProperty = xPropertyManager.GetProperty(strPropertyName);
                    if (null == xProperty)
                    {
                        NFDataList varList = new NFDataList();
                        varList.AddString("");

                        xProperty = xPropertyManager.AddProperty(strPropertyName, varList);
                    }

                    xProperty.SetString(xPropertyData.property_string_list[j].data.ToStringUtf8());
                }

                for (int j = 0; j < xPropertyData.property_object_list.Count; j++)
                {
                    string strPropertyName = xPropertyData.property_object_list[j].property_name.ToStringUtf8();
                    NFIProperty xProperty = xPropertyManager.GetProperty(strPropertyName);
                    if (null == xProperty)
                    {
                        NFDataList varList = new NFDataList();
                        varList.AddObject(new NFGUID());

                        xProperty = xPropertyManager.AddProperty(strPropertyName, varList);
                    }

					xProperty.SetObject(mHelpModule.PBToNF(xPropertyData.property_object_list[j].data));
                }
            }
        }
    };
}