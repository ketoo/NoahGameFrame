using System.Collections;
using UnityEngine;

namespace NFCoreEx
{
	public class NFCPropertyManager : NFIPropertyManager
	{
		public NFCPropertyManager(NFIDENTID self)
		{
			mSelf = self;
			mhtProperty = new Hashtable();
		}
		
		public override NFIProperty AddProperty(string strPropertyName, NFIValueList varData)
		{
			NFIProperty xProperty = null;
			if (!mhtProperty.ContainsKey(strPropertyName))
			{
				xProperty  = new NFCProperty(mSelf, strPropertyName, varData);
				mhtProperty[strPropertyName] = xProperty;
			}

			return xProperty;
		}

		public override bool SetProperty(string strPropertyName, NFIValueList varData)
		{
			if (mhtProperty.ContainsKey(strPropertyName))
			{
				NFIProperty xProperty = (NFCProperty)mhtProperty[strPropertyName];
				if (null != xProperty)
				{
					xProperty.SetValue(varData);
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
		
		public override NFIValueList GetPropertyList()
		{
			NFIValueList varData = new NFCValueList();
			foreach( DictionaryEntry de in mhtProperty) 
			{
				varData.AddString(de.Key.ToString());				
			}
			
			return varData;
		}
		
		NFIDENTID mSelf;
		Hashtable mhtProperty;
	}
}