using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml;
using System.Collections;
using System.IO;
using UnityEngine;

namespace NFSDK
{
    public class NFCElementModule : NFIElementModule
    {

        public NFCElementModule(NFIPluginManager pluginManager)
        {
            mPluginManager = pluginManager;
            mhtObject = new Dictionary<string, NFIElement>();
        }

        public override void Awake() 
		{  
		}

        public override void Init()
        {
            mxLogicClassModule = FindModule<NFIClassModule>();
            mstrRootPath = mxLogicClassModule.GetDataPath();
        }

        public override void AfterInit()
        {
            Load();
        }

        public override void BeforeShut()
        {
        }

        public override void Shut()
        {
        }

        public override void Execute()
        {
        }

        public override bool Load()
        {
            ClearInstanceElement();

            Dictionary<string, NFIClass> xTable = mxLogicClassModule.GetElementList();
            foreach (KeyValuePair<string, NFIClass> kv in xTable)
            {
                LoadInstanceElement(kv.Value);
			}

			return true;
        }

        public override bool Clear()
        {
            return false;
        }

        public override bool ExistElement(string strConfigName)
        {
            if (mhtObject.ContainsKey(strConfigName))
            {
                return true;
            }

            return false;
        }

        public override Int64 QueryPropertyInt(string strConfigName, string strPropertyName)
        {
            NFIElement xElement = GetElement(strConfigName);
            if (null != xElement)
            {
                return xElement.QueryInt(strPropertyName);
            }

            return 0;
        }

        public override double QueryPropertyFloat(string strConfigName, string strPropertyName)
        {
            NFIElement xElement = GetElement(strConfigName);
            if (null != xElement)
            {
                 return xElement.QueryFloat(strPropertyName);
            }

            return 0.0;
        }

        public override string QueryPropertyString(string strConfigName, string strPropertyName)
        {
            NFIElement xElement = GetElement(strConfigName);
            if (null != xElement)
            {
                return xElement.QueryString(strPropertyName);
            }

            return NFDataList.NULL_STRING;
        }

        public override bool AddElement(string strName, NFIElement xElement)
        {
            if (!mhtObject.ContainsKey(strName))
            {
                mhtObject.Add(strName, xElement);

                return true;
            }

            return false;
        }

        public override NFIElement GetElement(string strConfigName)
        {
            if (mhtObject.ContainsKey(strConfigName))
            {
                return (NFIElement)mhtObject[strConfigName];
            }

            return null;
        }

        private void ClearInstanceElement()
        {
            mhtObject.Clear();
        }

        private void LoadInstanceElement(NFIClass xLogicClass)
        {
            string strLogicPath = mstrRootPath;
            strLogicPath += xLogicClass.GetInstance();

            XmlDocument xmldoc = new XmlDocument();

            if (xLogicClass.GetEncrypt())
            {
                ///////////////////////////////////////////////////////////////////////////////////////
                StreamReader cepherReader = new StreamReader(strLogicPath); ;
                string strContent = cepherReader.ReadToEnd();
                cepherReader.Close();

                byte[] data = Convert.FromBase64String(strContent);

                string res = System.Text.ASCIIEncoding.Default.GetString(data);

                xmldoc.LoadXml(res);
                /////////////////////////////////////////////////////////////////
            }
            else
            {
                xmldoc.Load(strLogicPath);
            }

            XmlNode xRoot = xmldoc.SelectSingleNode("XML");

            XmlNodeList xNodeList = xRoot.SelectNodes("Object");
            for (int i = 0; i < xNodeList.Count; ++i)
            {
                //NFCLog.Instance.Log("Class:" + xLogicClass.GetName());

                XmlNode xNodeClass = xNodeList.Item(i);
                XmlAttribute strID = xNodeClass.Attributes["Id"];

                //NFCLog.Instance.Log("ClassID:" + strID.Value);

                NFIElement xElement = GetElement(strID.Value);
                if (null == xElement)
                {
                    xElement = new NFCElement();
                    AddElement(strID.Value, xElement);
                    xLogicClass.AddConfigName(strID.Value);

                    XmlAttributeCollection xCollection = xNodeClass.Attributes;
                    for (int j = 0; j < xCollection.Count; ++j)
                    {
                        XmlAttribute xAttribute = xCollection[j];
                        NFIProperty xProperty = xLogicClass.GetPropertyManager().GetProperty(xAttribute.Name);
                        if (null != xProperty)
                        {
                            NFDataList.VARIANT_TYPE eType = xProperty.GetType();
                            switch (eType)
                            {
                                case NFDataList.VARIANT_TYPE.VTYPE_INT:
                                    {
                                        NFDataList xValue = new NFDataList();
                                        xValue.AddInt(int.Parse(xAttribute.Value));
                                        NFIProperty property = xElement.GetPropertyManager().AddProperty(xAttribute.Name, xValue);
                                        property.SetUpload(xProperty.GetUpload());
                                    }
                                    break;
                                case NFDataList.VARIANT_TYPE.VTYPE_FLOAT:
                                    {
                                        NFDataList xValue = new NFDataList();
                                        xValue.AddFloat(float.Parse(xAttribute.Value));
                                        NFIProperty property = xElement.GetPropertyManager().AddProperty(xAttribute.Name, xValue);
                                        property.SetUpload(xProperty.GetUpload());
                                    }
                                    break;
                                case NFDataList.VARIANT_TYPE.VTYPE_STRING:
                                    {
                                        NFDataList xValue = new NFDataList();
                                        xValue.AddString(xAttribute.Value);
                                        NFIProperty property = xElement.GetPropertyManager().AddProperty(xAttribute.Name, xValue);
                                        property.SetUpload(xProperty.GetUpload());
                                    }
                                    break;
                                case NFDataList.VARIANT_TYPE.VTYPE_OBJECT:
                                    {
                                        NFDataList xValue = new NFDataList();
                                        xValue.AddObject(new NFGUID(0, int.Parse(xAttribute.Value)));
                                        NFIProperty property = xElement.GetPropertyManager().AddProperty(xAttribute.Name, xValue);
                                        property.SetUpload(xProperty.GetUpload());
                                    }
                                    break;
                                default:
                                    break;
                            }
                        }
                    }
                }
            }
        }

        /////////////////////////////////////////
        private NFIClassModule mxLogicClassModule;
        /////////////////////////////////////////
        private Dictionary<string, NFIElement> mhtObject;
        private string mstrRootPath;
    }
}