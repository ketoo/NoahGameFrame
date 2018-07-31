//-----------------------------------------------------------------------
// <copyright file="NFILogicClassModule.cs">
//     Copyright (C) 2015-2015 lvsheng.huang <https://github.com/ketoo/NFrame>
// </copyright>
//-----------------------------------------------------------------------
using System;
using System.Linq;
using System.Text;
using System.Collections;
using System.Collections.Generic;

namespace NFSDK
{
	public abstract class NFIClassModule : NFIModule
    {
        public abstract void SetDataPath(string strDataPath);
        public abstract string GetDataPath();
        public abstract bool ExistElement(string strClassName);
        public abstract bool AddElement(string strClassName);

        public abstract NFIClass GetElement(string strClassName);
        public abstract Dictionary<string, NFIClass> GetElementList();

    }
}