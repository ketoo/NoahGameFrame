//-----------------------------------------------------------------------
// <copyright file="NFIEventModule.cs">
//     Copyright (C) 2015-2015 lvsheng.huang <https://github.com/ketoo/NFrame>
// </copyright>
//-----------------------------------------------------------------------
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace NFSDK
{
	public abstract class NFIEventModule : NFIModule
	{

		public abstract void RegisterCallback(int nEventID, NFIEvent.EventHandler handler);
		public abstract void DoEvent(int nEventID);
		public abstract void DoEvent(int nEventID, NFIDataList valueList);
	}
}
