using System.Collections;

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
			NFIProperty property = null;
			if (!mhtProperty.ContainsKey(strPropertyName))
			{
				property  = new NFCProperty(mSelf, strPropertyName, varData);
				mhtProperty[strPropertyName] = property;
			}

			return property;
		}

		public override bool SetProperty(string strPropertyName, NFIValueList varData)
		{
			if (mhtProperty.ContainsKey(strPropertyName))
			{
				NFIProperty property = (NFCProperty)mhtProperty[strPropertyName];
				if (null != property)
				{
					property.SetValue(varData);
				} 
			}
			return true;
		}

		public override NFIProperty GetProperty(string strPropertyName)
		{
			NFIProperty property = null;
			if (mhtProperty.ContainsKey(strPropertyName))
			{
				property = (NFCProperty)mhtProperty[strPropertyName];
				return property;
			}

			return property;
		}

		public override void RegisterCallback(string strPropertyName, NFIProperty.PropertyEventHandler handler)
		{
			if (mhtProperty.ContainsKey(strPropertyName))
			{
				NFIProperty property = (NFCProperty)mhtProperty[strPropertyName];
				property.RegisterCallback(handler);
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