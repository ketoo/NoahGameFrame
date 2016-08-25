//-----------------------------------------------------------------------
// <copyright file="NFIElement.cs">
//     Copyright (C) 2015-2015 lvsheng.huang <https://github.com/ketoo/NFrame>
// </copyright>
//-----------------------------------------------------------------------
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace NFrame
{
	public abstract class NFIElement
	{
        public abstract NFIPropertyManager GetPropertyManager();

        public abstract Int64 QueryInt(string strName);
        public abstract double QueryFloat(string strName);
        public abstract string QueryString(string strName);
        public abstract NFGUID QueryObject(string strName);

	}
}
