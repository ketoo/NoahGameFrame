//-----------------------------------------------------------------------
// <copyright file="NFCPropertyManager.cs">
//     Copyright (C) 2015-2015 lvsheng.huang <https://github.com/ketoo/NFrame>
// </copyright>
//-----------------------------------------------------------------------
using System.Collections;

namespace NFrame
{
	public class NFCPropertyManager : NFIPropertyManager
	{
		public NFCPropertyManager(NFGUID self)
		{
			mSelf = self;
			mhtProperty = new Hashtable();
		}
		
		public override NFIProperty AddProperty(string strPropertyName, NFIDataList varData)
		{
			NFIProperty xProperty = null;
			if (!mhtProperty.ContainsKey(strPropertyName))
			{
				xProperty  = new NFCProperty(mSelf, strPropertyName, varData);
				mhtProperty[strPropertyName] = xProperty;
			}

			return xProperty;
		}

        public override NFIProperty AddProperty(string strPropertyName, NFIDataList.TData varData)
        {
            NFIProperty xProperty = null;
            if (!mhtProperty.ContainsKey(strPropertyName))
            {
                xProperty = new NFCProperty(mSelf, strPropertyName, varData);
                mhtProperty[strPropertyName] = xProperty;
            }

            return xProperty;
        }

		public override bool SetProperty(string strPropertyName, NFIDataList varData)
		{
			if (mhtProperty.ContainsKey(strPropertyName))
			{
				NFIProperty xProperty = (NFCProperty)mhtProperty[strPropertyName];
				if (null != xProperty)
				{
					xProperty.SetData(varData.GetData(0));
				} 
			}
			return true;
		}

		public override NFIProperty GetProperty(string strPropertyName)
		{
			NFIProperty xProperty = null;
			if (mhtProperty.ContainsKey(strPropertyName))
			{
				xProperty = (NFCProperty)mhtProperty[strPropertyName];
				return xProperty;
			}

			return xProperty;
		}

		public override void RegisterCallback(string strPropertyName, NFIProperty.PropertyEventHandler handler)
		{
			if (mhtProperty.ContainsKey(strPropertyName))
			{
				NFIProperty xProperty = (NFCProperty)mhtProperty[strPropertyName];
				xProperty.RegisterCallback(handler);
			}
		}
		
		public override NFIDataList GetPropertyList()
		{
			NFIDataList varData = new NFCDataList();
			foreach( DictionaryEntry de in mhtProperty) 
			{
				varData.AddString(de.Key.ToString());				
			}
			
			return varData;
		}
		
		NFGUID mSelf;
		Hashtable mhtProperty;
	}
}