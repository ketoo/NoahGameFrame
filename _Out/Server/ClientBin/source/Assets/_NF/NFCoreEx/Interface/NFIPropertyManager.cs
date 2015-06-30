using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace NFCoreEx
{
    public abstract class NFIPropertyManager
    {
        public abstract NFIProperty AddProperty(string strPropertyName, NFIDataList varData);

        public abstract bool SetProperty(string strPropertyName, NFIDataList varData);

        public abstract NFIProperty GetProperty(string strPropertyName);
		
		public abstract NFIDataList GetPropertyList();
		
		public abstract void RegisterCallback(string strPropertyName, NFIProperty.PropertyEventHandler handler);
    }
}