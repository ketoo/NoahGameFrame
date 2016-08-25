//-----------------------------------------------------------------------
// <copyright file="NFILogicClass.cs">
//     Copyright (C) 2015-2015 lvsheng.huang <https://github.com/ketoo/NFrame>
// </copyright>
//-----------------------------------------------------------------------
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Collections;
using System.Runtime.InteropServices;
using System.Threading;
using System.IO;

namespace NFrame
{
    public abstract class NFILogicClass
	{
        public abstract NFIPropertyManager GetPropertyManager();
        public abstract NFIRecordManager GetRecordManager();
        public abstract List<string> GetConfigNameList();
        public abstract bool AddConfigName(string strConfigName);

        public abstract string GetName();
        public abstract void SetName(string strConfigName);

        public abstract string GetPath();
        public abstract void SetPath(string strPath);

        public abstract string GetInstance();
        public abstract void SetInstance(string strInstancePath);

        public abstract void SetEncrypt(bool bEncrypt);
        public abstract bool GetEncrypt();
    }
}
