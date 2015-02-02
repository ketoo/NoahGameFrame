using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml;
using UnityEngine;

namespace NFCoreEx
{
    public class NFCLogicClassManager : NFILogicClassManager
    {

        #region Instance
        private static NFCLogicClassManager _Instance = null;
        public static NFCLogicClassManager Instance
        {
            get
            {
                if (_Instance == null)
                {
                    _Instance = new NFCLogicClassManager();
                    _Instance.Load();
                }
                return _Instance;
            }
        }
        #endregion

        private bool Load()
        {
            XmlDocument xmldoc = new XmlDocument();

            string strLogicPath = "../../NFDataCfg/Struct/LogicClass.xml";
            xmldoc.Load(strLogicPath);

            XmlNode root = xmldoc.SelectSingleNode("XML");

            LoadLogicClass(root);
            LoadLogicClassProperty();

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

        private void LoadLogicClassProperty()
        {
            Hashtable xTable = NFCLogicClassManager.Instance.GetElementList();
            foreach (DictionaryEntry de in xTable)
            {
                LoadLogicClassProperty((string)de.Key);
            }

            //再为每个类加载iobject的属性
            foreach (DictionaryEntry de in xTable)
            {
                if ((string)de.Key != "IObject")
                {
                    AddBasePropertyFormOther((string)de.Key, "IObject");
                }
            }
        }

        private void LoadLogicClassProperty(string strName)
        {
            NFILogicClass xLogicClass = GetElement(strName);
            if (null != xLogicClass)
            {
                string strLogicPath = xLogicClass.GetPath();

                XmlDocument xmldoc = new XmlDocument();

                xmldoc.Load(strLogicPath);
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
                                NFIValueList xValue = new NFCValueList();
                                xValue.AddInt(0);
                                xLogicClass.GetPropertyManager().AddProperty(strID.Value, xValue);
                            }
                            break;
                        case "float":
                            {
                                NFIValueList xValue = new NFCValueList();
                                xValue.AddFloat(0.0f);
                                xLogicClass.GetPropertyManager().AddProperty(strID.Value, xValue);
                            }
                            break;
                        case "double":
                            {
                                NFIValueList xValue = new NFCValueList();
                                xValue.AddDouble(0.0f);
                                xLogicClass.GetPropertyManager().AddProperty(strID.Value, xValue);
                            }
                            break;
                        case "string":
                            {
                                NFIValueList xValue = new NFCValueList();
                                xValue.AddString("");
                                xLogicClass.GetPropertyManager().AddProperty(strID.Value, xValue);
                            }
                            break;
                        case "object":
                            {
                                NFIValueList xValue = new NFCValueList();
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

        void AddBasePropertyFormOther(string strName, string strOther)
        {
            NFILogicClass xOtherClass = GetElement(strOther);
            NFILogicClass xLogicClass = GetElement(strName);
            if (null != xLogicClass && null != xOtherClass)
            {
                NFIValueList xValue = xOtherClass.GetPropertyManager().GetPropertyList();
                for (int i = 0; i < xValue.Count(); ++i )
                {
                    NFIProperty xProperty = xOtherClass.GetPropertyManager().GetProperty(xValue.StringVal(i));
                    xLogicClass.GetPropertyManager().AddProperty(xValue.StringVal(i), xProperty.GetValue());
                }
            }
        }

        public override Hashtable GetElementList()
        {
            return mhtObject;
        }
        /////////////////////////////////////////
        private Hashtable mhtObject = new Hashtable();
    }
}