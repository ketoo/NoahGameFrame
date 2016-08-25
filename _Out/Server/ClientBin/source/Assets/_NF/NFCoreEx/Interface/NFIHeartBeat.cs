//-----------------------------------------------------------------------
// <copyright file="NFIHeartBeat.cs">
//     Copyright (C) 2015-2015 lvsheng.huang <https://github.com/ketoo/NFrame>
// </copyright>
//-----------------------------------------------------------------------
using System;
using System.Linq;
using System.Text;
using System.Collections;
using System.Collections.Generic;

namespace NFrame
{
	public abstract class NFIHeartBeat
	{
		public delegate void HeartBeatEventHandler(NFGUID self, string strHeartBeat, float fTime, int nRemainCount);


		public abstract void RegisterCallback(NFIHeartBeat.HeartBeatEventHandler handler);
		public abstract bool Update(float fPassTime);
        public abstract bool NeedRemove();
	}
}