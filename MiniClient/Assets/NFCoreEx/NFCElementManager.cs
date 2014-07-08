using System.Collections;

namespace NFCoreEx
{
    public class NFCElementManager : NFIElementManager
	{
        public NFCElementManager()
        {
            mhtObject = new Hashtable();
        }

        public override bool Load()
        {
            return false;
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

        public override int QueryPropertyInt(string strConfigName, string strPropertyName)
        {
            NFIElement xElement = GetElement(strConfigName);
            if (null != xElement)
            {
                NFIProperty xProperty = xElement.GetPropertyManager().GetProperty(strPropertyName);
                if (null != xElement)
                {
                    return xProperty.QueryInt();
                }
            }

            return 0;
        }

        public override float QueryPropertyFloat(string strConfigName, string strPropertyName)
        {
            NFIElement xElement = GetElement(strConfigName);
            if (null != xElement)
            {
                NFIProperty xProperty = xElement.GetPropertyManager().GetProperty(strPropertyName);
                if (null != xElement)
                {
                    return xProperty.QueryFloat();
                }
            }

            return 0;
        }

        public override double QueryPropertyDouble(string strConfigName, string strPropertyName)
        {
            NFIElement xElement = GetElement(strConfigName);
            if (null != xElement)
            {
                NFIProperty xProperty = xElement.GetPropertyManager().GetProperty(strPropertyName);
                if (null != xElement)
                {
                    return xProperty.QueryDouble();
                }
            }

            return 0;
        }

        public override string QueryPropertyString(string strConfigName, string strPropertyName)
        {
            NFIElement xElement = GetElement(strConfigName);
            if (null != xElement)
            {
                NFIProperty xProperty = xElement.GetPropertyManager().GetProperty(strPropertyName);
                if (null != xElement)
                {
                    return xProperty.QueryString();
                }
            }

            return "";
        }

        public override bool AddElement(string strName)
        {
            if (!mhtObject.ContainsKey(strName))
            {
                NFIElement xElement = new NFCElement();
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

        /////////////////////////////////////////
        private Hashtable mhtObject;
	}
}