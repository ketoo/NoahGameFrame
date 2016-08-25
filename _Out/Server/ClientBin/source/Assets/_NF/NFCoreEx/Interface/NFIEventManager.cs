//-----------------------------------------------------------------------
// <copyright file="NFIEventModule.cs">
//     Copyright (C) 2015-2015 lvsheng.huang <https://github.com/ketoo/NFrame>
// </copyright>
//-----------------------------------------------------------------------
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace NFrame
{
	public abstract class NFIEventModule : NFILogicModule
	{
		public abstract void RegisterCallback(NFGUID self, int nEventID, NFIEvent.EventHandler handler, NFIDataList valueList);
		public abstract void DoEvent(NFGUID self, int nEventID, NFIDataList valueList);
	}
}
