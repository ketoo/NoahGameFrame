//-----------------------------------------------------------------------
// <copyright file="NFIPropertyManager.cs">
//     Copyright (C) 2015-2015 lvsheng.huang <https://github.com/ketoo/NFrame>
// </copyright>
//-----------------------------------------------------------------------
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace NFSDK
{
    public abstract class NFIPropertyManager
    {
        public abstract NFIProperty AddProperty(string strPropertyName, NFDataList varData);
        public abstract NFIProperty AddProperty(string strPropertyName, NFDataList.TData varData);

        public abstract bool SetProperty(string strPropertyName, NFDataList varData);

        public abstract NFIProperty GetProperty(string strPropertyName);
		
		public abstract NFDataList GetPropertyList();
		
		public abstract void RegisterCallback(string strPropertyName, NFIProperty.PropertyEventHandler handler);
    }
}