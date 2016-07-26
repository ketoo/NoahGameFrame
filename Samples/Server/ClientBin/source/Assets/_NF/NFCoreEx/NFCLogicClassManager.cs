using System;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Xml;
using UnityEngine;

namespace NFCoreEx
{
    public class NFCLogicClassManager : NFILogicClassManager
    {
        private bool mbCepher = false;
        private String mstrRootPath = null;
        #region Instance
        private static NFCLogicClassManager _Instance = null;
        public static NFCLogicClassManager Instance
        {
            get
            {
                if (_Instance == null)
                {
                    _Instance = new NFCLogicClassManager();
                }

                return _Instance;
            }
        }
        #endregion

        public bool GetCepher()
        {
            return mbCepher;
        }

        public void LoadFromConfig(String strConfigPath)
        {
            mstrRootPath = strConfigPath;
            _Instance.Load();
        }

        private bool Load()
        {
            ClearLogicClass();

            XmlDocument xmldoc = new XmlDocument();

            string strLogicPath = mstrRootPath + "NFDataCfg/Struct/LogicClass.xml";
            if (File.Exists(strLogicPath))
            {
                mbCepher = false;
            }
            else
            {
                strLogicPath = mstrRootPath + "NFDataCfg/Struct/LogicClass.NF";
                mbCepher = true;
            }

            ///////////////////////////////////////////////////////////////////////////////////////
            if (mbCepher)
            {
                StreamReader cepherReader = new StreamReader(strLogicPath); ;
                string strContent = cepherReader.ReadToEnd();
                cepherReader.Close();

                byte[] data = Convert.FromBase64String(strContent);

                MemoryStream stream = new MemoryStream(data);
                XmlReader x = XmlReader.Create(stream);
                x.MoveToContent();
                string res = x.ReadOuterXml();

                xmldoc.LoadXml(res);
            }
            else
            {
                xmldoc.Load(strLogicPath);
            }
            /////////////////////////////////////////////////////////////////
            XmlNode root = xmldoc.SelectSingleNode("XML");

            LoadLogicClass(root);
            LoadLogicClassProperty();
            LoadLogicClassRecord();

            return false;
        }

        public override bool Clear()
        {
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
            Dictionary<string, NFILogicClass> xTable = NFCLogicClassManager.Instance.GetElementList();
            foreach (KeyValuePair<string, NFILogicClass> kv in xTable)
            {
                LoadLogicClassProperty((string)kv.Key);
            }

            //��Ϊÿ�������iobject������
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
            Dictionary<string, NFILogicClass> xTable = NFCLogicClassManager.Instance.GetElementList();
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
                string strLogicPath = mstrRootPath + xLogicClass.GetPath();

                XmlDocument xmldoc = new XmlDocument();
                ///////////////////////////////////////////////////////////////////////////////////////
                if (mbCepher)
                {
                    StreamReader cepherReader = new StreamReader(strLogicPath); ;
                    string strContent = cepherReader.ReadToEnd();
                    cepherReader.Close();

                    byte[] data = Convert.FromBase64String(strContent);

                    MemoryStream stream = new MemoryStream(data);
                    XmlReader x = XmlReader.Create(stream);
                    x.MoveToContent();
                    string res = x.ReadOuterXml();

                    xmldoc.LoadXml(res);
                }
                else
                {
                    xmldoc.Load(strLogicPath);
                }
                /////////////////////////////////////////////////////////////////

                XmlNode xRoot = xmldoc.SelectSingleNode("XML");
                XmlNode xNodePropertys = xRoot.SelectSingleNode("Propertys");
                XmlNodeList xNodeList = xNodePropertys.SelectNodes("Property");
                for (int i = 0; i < xNodeList.Count; ++i)
                {
                    XmlNode xPropertyNode = xNodeList.Item(i);
                    XmlAttribute strID = xPropertyNode.Attributes["Id"];
                    XmlAttribute strType = xPropertyNode.Attributes["Type"];

                    switch (strType.Value)
                    {
                        case "int":
                            {
                                NFIDataList xValue = new NFCDataList();
                                xValue.AddInt(0);
                                xLogicClass.GetPropertyManager().AddProperty(strID.Value, xValue);
                            }
                            break;
                        case "float":
                            {
                                NFIDataList xValue = new NFCDataList();
                                xValue.AddFloat(0.0f);
                                xLogicClass.GetPropertyManager().AddProperty(strID.Value, xValue);
                            }
                            break;
                        case "double":
                            {
                                NFIDataList xValue = new NFCDataList();
                                xValue.AddDouble(0.0f);
                                xLogicClass.GetPropertyManager().AddProperty(strID.Value, xValue);
                            }
                            break;
                        case "string":
                            {
                                NFIDataList xValue = new NFCDataList();
                                xValue.AddString("");
                                xLogicClass.GetPropertyManager().AddProperty(strID.Value, xValue);
                            }
                            break;
                        case "object":
                            {
                                NFIDataList xValue = new NFCDataList();
                                xValue.AddObject(new NFIDENTID(0, 0));
                                xLogicClass.GetPropertyManager().AddProperty(strID.Value, xValue);
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
                string strLogicPath = mstrRootPath + xLogicClass.GetPath();

                XmlDocument xmldoc = new XmlDocument();
                ///////////////////////////////////////////////////////////////////////////////////////
                if (mbCepher)
                {
                    StreamReader cepherReader = new StreamReader(strLogicPath); ;
                    string strContent = cepherReader.ReadToEnd();
                    cepherReader.Close();

                    byte[] data = Convert.FromBase64String(strContent);

                    MemoryStream stream = new MemoryStream(data);
                    XmlReader x = XmlReader.Create(stream);
                    x.MoveToContent();
                    string res = x.ReadOuterXml();

                    xmldoc.LoadXml(res);
                }
                else
                {
                    xmldoc.Load(strLogicPath);
                }
                /////////////////////////////////////////////////////////////////
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
                                            xValue.AddFloat(0.0f);
                                        }
                                        break;
                                    case "double":
                                        {
                                            xValue.AddDouble(0.0f);
                                        }
                                        break;
                                    case "string":
                                        {
                                            xValue.AddString("");
                                        }
                                        break;
                                    case "object":
                                        {
                                            xValue.AddObject(new NFIDENTID(0, 0));
                                        }
                                        break;
                                    default:
                                        break;

                                }
                            }

                            xLogicClass.GetRecordManager().AddRecord(strID, int.Parse(strRow), xValue);
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
                    xLogicClass.GetPropertyManager().AddProperty(xValue.StringVal(i), xProperty.GetValue());
                }
            }
        }

        public override Dictionary<string, NFILogicClass> GetElementList()
        {
            return mhtObject;
        }
        /////////////////////////////////////////
        private Dictionary<string, NFILogicClass> mhtObject = new Dictionary<string, NFILogicClass>();
    }
}