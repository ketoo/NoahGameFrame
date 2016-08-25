//-----------------------------------------------------------------------
// <copyright file="NFCElement.cs">
//     Copyright (C) 2015-2015 lvsheng.huang <https://github.com/ketoo/NFrame>
// </copyright>
//-----------------------------------------------------------------------
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace NFrame
{
	class NFCElement : NFIElement
	{
        public NFCElement()
        {
            mxPropertyManager = new NFCPropertyManager(new NFGUID());
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

        public override double QueryFloat(string strName)
        {
            NFIProperty xProperty = GetPropertyManager().GetProperty(strName);
            if (null != xProperty)
            {
                return xProperty.QueryFloat();
            }

            return 0.0;
        }

        public override string QueryString(string strName)
        {
            NFIProperty xProperty = GetPropertyManager().GetProperty(strName);
            if (null != xProperty)
            {
                return xProperty.QueryString();
            }

            return NFIDataList.NULL_STRING;
        }

        public override NFGUID QueryObject(string strName)
        {
            NFIProperty xProperty = GetPropertyManager().GetProperty(strName);
            if (null != xProperty)
            {
                return xProperty.QueryObject();
            }

            return NFIDataList.NULL_OBJECT;
        }

        private NFIPropertyManager mxPropertyManager;
	}
}
