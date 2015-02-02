using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace NFCoreEx
{
	class NFCElement : NFIElement
	{
        public NFCElement()
        {
            mxPropertyManager = new NFCPropertyManager(new NFIDENTID());
        }

        public override NFIPropertyManager GetPropertyManager()
        {
            return mxPropertyManager;
        }


        public override Int64 QueryInt(string strName)
        {
            NFIProperty xProperty = GetPropertyManager().GetProperty(strName);
            if (null != xProperty)
            {
                return xProperty.QueryInt();
            }

            return 0;
        }

        public override float QueryFloat(string strName)
        {
            NFIProperty xProperty = GetPropertyManager().GetProperty(strName);
            if (null != xProperty)
            {
                return xProperty.QueryFloat();
            }

            return 0f;
        }

        public override double QueryDouble(string strName)
        {
            NFIProperty xProperty = GetPropertyManager().GetProperty(strName);
            if (null != xProperty)
            {
                return xProperty.QueryDouble();
            }

            return 0f;
        }

        public override string QueryString(string strName)
        {
            NFIProperty xProperty = GetPropertyManager().GetProperty(strName);
            if (null != xProperty)
            {
                return xProperty.QueryString();
            }

            return "";
        }

        public override NFIDENTID QueryObject(string strName)
        {
            NFIProperty xProperty = GetPropertyManager().GetProperty(strName);
            if (null != xProperty)
            {
                return xProperty.QueryObject();
            }

            return new NFIDENTID();
        }

        private NFIPropertyManager mxPropertyManager;
	}
}
