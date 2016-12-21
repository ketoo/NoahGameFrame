//-----------------------------------------------------------------------
// <copyright file="NFCLogicClassModule.cs">
//     Copyright (C) 2015-2015 lvsheng.huang <https://github.com/ketoo/NFrame>
// </copyright>
//-----------------------------------------------------------------------
#if UNITY_EDITOR
#define NF_CLIENT_FRAME
#elif UNITY_IPHONE
#define NF_CLIENT_FRAME
#elif UNITY_ANDROID
#define NF_CLIENT_FRAME
#elif UNITY_STANDALONE_OSX
#define NF_CLIENT_FRAME
#elif UNITY_STANDALONE_WIN
#define NF_CLIENT_FRAME
#endif

//-----------------------------------------------------------------------
// <copyright file="NFCLogicClassModule.cs">
//     Copyright (C) 2015-2015 lvsheng.huang <https://github.com/ketoo/NFrame>
// </copyright>
//-----------------------------------------------------------------------
using System;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Xml;

namespace NFrame
{
    public class NFCLogicClassModule : NFILogicClassModule
    {
        public override void Init()
        {
#if NF_CLIENT_FRAME
#else
            mstrPath = GetMng().GetClassPath();
#endif
            mbEncrypt = false;
            Load();
        }
        public override void SetDataPath(string strDataPath)
        {
            mstrPath = strDataPath;
        }

        public override string GetDataPath()
        {
            return mstrPath;
        }

        private bool Load()
        {
            ClearLogicClass();
            
            XmlDocument xmldoc = new XmlDocument();

            if (System.IO.File.Exists(mstrPath + "/NFDataCfg/Struct/LogicClass.xml"))
            {
                mbEncrypt = false;
                
            }
            else
            {
                mbEncrypt = true;
            }

            if (!mbEncrypt)
            {
                string strLogicPath = mstrPath + "/NFDataCfg/Struct/LogicClass.xml";

                xmldoc.Load(strLogicPath);
            }
            else
            {
                //加密了
                string strLogicPath = mstrPath + "/NFDataCfg/Struct/LogicClass.NF";

                ///////////////////////////////////////////////////////////////////////////////////////
                StreamReader cepherReader = new StreamReader(strLogicPath); ;
                string strContent = cepherReader.ReadToEnd();
                cepherReader.Close();

                byte[] data = Convert.FromBase64String(strContent);
                string res = System.Text.ASCIIEncoding.Default.GetString(data);

                xmldoc.LoadXml(res);
            }

            XmlNode root = xmldoc.SelectSingleNode("XML");

            LoadLogicClass(root);
            LoadLogicClassProperty();
            LoadLogicClassRecord();
            

            return false;
        }

        public override bool ExistElement(string strClassName)
        {
            if (mhtObject.ContainsKey(strClassName))
            {
                return true;
            }

            return false;
        }

        public override bool AddElement(string strName)
        {
            if (!mhtObject.ContainsKey(strName))
            {
                NFILogicClass xElement = new NFCLogicClass();
                xElement.SetName(strName);
                xElement.SetEncrypt(mbEncrypt);

                mhtObject.Add(strName, xElement);

                return true;
            }

            return false;
        }

        public override NFILogicClass GetElement(string strClassName)
        {
            if (mhtObject.ContainsKey(strClassName))
            {
                return (NFILogicClass)mhtObject[strClassName];
            }

            return null;
        }
        /////////////////////////////////////////
        private void LoadLogicClass(XmlNode xNode)
        {
            XmlNodeList xNodeList = xNode.SelectNodes("Class");
            for (int i = 0; i < xNodeList.Count; ++i)
            {
                XmlNode xNodeClass = xNodeList.Item(i);
                XmlAttribute strID = xNodeClass.Attributes["Id"];
                XmlAttribute strPath = xNodeClass.Attributes["Path"];
                XmlAttribute strInstancePath = xNodeClass.Attributes["InstancePath"];

                NFILogicClass xLogicClass = new NFCLogicClass();
                mhtObject.Add(strID.Value, xLogicClass);

                xLogicClass.SetName(strID.Value);
                xLogicClass.SetPath(strPath.Value);
                xLogicClass.SetInstance(strInstancePath.Value);
                xLogicClass.SetEncrypt(mbEncrypt);

                XmlNodeList xNodeSubClassList = xNodeClass.SelectNodes("Class");
                if (xNodeSubClassList.Count > 0)
                {
                    LoadLogicClass(xNodeClass);
                }
            }
        }
        
        private void ClearLogicClass()
        {
            mhtObject.Clear();
        }

        private void LoadLogicClassProperty()
        {
            Dictionary<string, NFILogicClass> xTable = GetElementList();
            foreach (KeyValuePair<string, NFILogicClass> kv in xTable)
            {
                LoadLogicClassProperty((string)kv.Key);
            }

            //再为每个类加载iobject的属性
            foreach (KeyValuePair<string, NFILogicClass> kv in xTable)
            {
                if (kv.Key != "IObject")
                {
                    AddBasePropertyFormOther(kv.Key, "IObject");
                }
            }
        }

        private void LoadLogicClassRecord()
        {
            Dictionary<string, NFILogicClass> xTable = GetElementList();
            foreach (KeyValuePair<string, NFILogicClass> kv in xTable)
            {
                LoadLogicClassRecord(kv.Key);
            }
        }

        private void LoadLogicClassProperty(string strName)
        {
            NFILogicClass xLogicClass = GetElement(strName);
            if (null != xLogicClass)
            {
                string strLogicPath = mstrPath + xLogicClass.GetPath();

                XmlDocument xmldoc = new XmlDocument();
                if (mbEncrypt)
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
                XmlNode xNodePropertys = xRoot.SelectSingleNode("Propertys");
                XmlNodeList xNodeList = xNodePropertys.SelectNodes("Property");
                for (int i = 0; i < xNodeList.Count; ++i)
                {
                    XmlNode xPropertyNode = xNodeList.Item(i);
                    XmlAttribute strID = xPropertyNode.Attributes["Id"];
                    XmlAttribute strType = xPropertyNode.Attributes["Type"];
                    XmlAttribute strUpload = xPropertyNode.Attributes["Upload"];
                    bool bUpload = strUpload.Value.Equals("1");

                    switch (strType.Value)
                    {
                        case "int":
                            {
                                NFIDataList xValue = new NFCDataList();
                                xValue.AddInt(0);
                                NFIProperty xProperty = xLogicClass.GetPropertyManager().AddProperty(strID.Value, xValue);
                                xProperty.SetUpload(bUpload);
                            }
                            break;
                        case "float":
                            {
                                NFIDataList xValue = new NFCDataList();
                                xValue.AddFloat(0.0);
                                NFIProperty xProperty = xLogicClass.GetPropertyManager().AddProperty(strID.Value, xValue);
                                xProperty.SetUpload(bUpload);
                            }
                            break;
                        case "string":
                            {
                                NFIDataList xValue = new NFCDataList();
                                xValue.AddString("");
                                NFIProperty xProperty = xLogicClass.GetPropertyManager().AddProperty(strID.Value, xValue);
                                xProperty.SetUpload(bUpload);
                            }
                            break;
                        case "object":
                            {
                                NFIDataList xValue = new NFCDataList();
                                xValue.AddObject(new NFGUID(0, 0));
                                NFIProperty xProperty = xLogicClass.GetPropertyManager().AddProperty(strID.Value, xValue);
                                xProperty.SetUpload(bUpload);
                            }
                            break;
                        default:
                            break;

                    }
                }
            }
        }

        private void LoadLogicClassRecord(string strName)
        {
            NFILogicClass xLogicClass = GetElement(strName);
            if (null != xLogicClass)
            {
                string strLogicPath = mstrPath + xLogicClass.GetPath();

                XmlDocument xmldoc = new XmlDocument();

                if(mbEncrypt)
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
                XmlNode xNodePropertys = xRoot.SelectSingleNode("Records");
                if (null != xNodePropertys)
                {
                    XmlNodeList xNodeList = xNodePropertys.SelectNodes("Record");
                    if (null != xNodeList)
                    {
                        for (int i = 0; i < xNodeList.Count; ++i)
                        {
                            XmlNode xRecordNode = xNodeList.Item(i);

                            string strID = xRecordNode.Attributes["Id"].Value;
                            string strRow = xRecordNode.Attributes["Row"].Value;
                            string strUpload = xRecordNode.Attributes["Upload"].Value;
                            bool bUpload = strUpload.Equals("1");
                            NFIDataList xValue = new NFCDataList();

                            XmlNodeList xTagNodeList = xRecordNode.SelectNodes("Col");
                            for (int j = 0; j < xTagNodeList.Count; ++j)
                            {
                                XmlNode xColTagNode = xTagNodeList.Item(j);

                                XmlAttribute strTagID = xColTagNode.Attributes["Tag"];
                                XmlAttribute strTagType = xColTagNode.Attributes["Type"];


                                switch (strTagType.Value)
                                {
                                    case "int":
                                        {
                                            xValue.AddInt(0);
                                        }
                                        break;
                                    case "float":
                                        {
                                            xValue.AddFloat(0.0);
                                        }
                                        break;
                                    case "string":
                                        {
                                            xValue.AddString("");
                                        }
                                        break;
                                    case "object":
                                        {
                                            xValue.AddObject(new NFGUID(0, 0));
                                        }
                                        break;
                                    default:
                                        break;

                                }
                            }
                            NFIRecord xRecord = xLogicClass.GetRecordManager().AddRecord(strID, int.Parse(strRow), xValue);
                            xRecord.SetUpload(bUpload);
                        }
                    }
                }
            }
        }

        void AddBasePropertyFormOther(string strName, string strOther)
        {
            NFILogicClass xOtherClass = GetElement(strOther);
            NFILogicClass xLogicClass = GetElement(strName);
            if (null != xLogicClass && null != xOtherClass)
            {
                NFIDataList xValue = xOtherClass.GetPropertyManager().GetPropertyList();
                for (int i = 0; i < xValue.Count(); ++i)
                {
                    NFIProperty xProperty = xOtherClass.GetPropertyManager().GetProperty(xValue.StringVal(i));
                    xLogicClass.GetPropertyManager().AddProperty(xValue.StringVal(i), xProperty.GetData());
                }
            }
        }

        public override Dictionary<string, NFILogicClass> GetElementList()
        {
            return mhtObject;
        }
        /////////////////////////////////////////
        private Dictionary<string, NFILogicClass> mhtObject = new Dictionary<string, NFILogicClass>();
        private string mstrPath = "";
        private bool mbEncrypt;
    }
}