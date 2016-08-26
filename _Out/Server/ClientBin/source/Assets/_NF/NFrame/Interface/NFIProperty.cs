//-----------------------------------------------------------------------
// <copyright file="NFIProperty.cs">
//     Copyright (C) 2015-2015 lvsheng.huang <https://github.com/ketoo/NFrame>
// </copyright>
//-----------------------------------------------------------------------
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace NFrame
{
    public abstract class NFIProperty
    {
        public delegate void PropertyEventHandler(NFGUID self, string strProperty, NFIDataList.TData oldVar, NFIDataList.TData newVar);

        public abstract string GetKey();

        public abstract NFIDataList.VARIANT_TYPE GetType();
        public abstract NFIDataList.TData GetData();

        public abstract void SetUpload(bool upload);

        public abstract bool GetUpload();

        public abstract Int64 QueryInt();

        public abstract double QueryFloat();

        public abstract string QueryString();

        public abstract NFGUID QueryObject();

        public abstract bool SetInt(Int64 value);

        public abstract bool SetFloat(double value);

        public abstract bool SetString(string value);

        public abstract bool SetObject(NFGUID value);

        public abstract bool SetData(NFIDataList.TData x);

        public abstract void RegisterCallback(PropertyEventHandler handler);
    }
}