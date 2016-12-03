//-----------------------------------------------------------------------
// <copyright file="NFIHeartBeatManager.cs">
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
    public abstract class NFIHeartBeatManager
    {
        public abstract void AddHeartBeat(string strHeartBeatName, float fTime, int nCount, NFIHeartBeat.HeartBeatEventHandler handler);
        public abstract bool FindHeartBeat(string strHeartBeatName);
        public abstract void RemoveHeartBeat(string strHeartBeatName);
		public abstract void Update(float fPassTime);
    }
}