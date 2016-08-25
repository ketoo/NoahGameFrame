//-----------------------------------------------------------------------
// <copyright file="NFIPropertyManager.cs">
//     Copyright (C) 2015-2015 lvsheng.huang <https://github.com/ketoo/NFrame>
// </copyright>
//-----------------------------------------------------------------------
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace NFrame
{
    public abstract class NFIPropertyManager
    {
        public abstract NFIProperty AddProperty(string strPropertyName, NFIDataList varData);
        public abstract NFIProperty AddProperty(string strPropertyName, NFIDataList.TData varData);

        public abstract bool SetProperty(string strPropertyName, NFIDataList varData);

        public abstract NFIProperty GetProperty(string strPropertyName);
		
		public abstract NFIDataList GetPropertyList();
		
		public abstract void RegisterCallback(string strPropertyName, NFIProperty.PropertyEventHandler handler);
    }
}