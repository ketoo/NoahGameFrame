//-----------------------------------------------------------------------
// <copyright file="NFBehaviour.cs">
//     Copyright (C) 2015-2015 lvsheng.huang <https://github.com/ketoo/NFrame>
// </copyright>
//-----------------------------------------------------------------------
#define NF_CLIENT_FRAME

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Collections.ObjectModel;

namespace NFrame
{
    public class NFBehaviour : IDisposable
    {
        public virtual void Dispose()
        {
        }

        public virtual void Init(){}

        public virtual void AfterInit() {}

        public virtual void BeforeShut() {}

        public virtual void Shut() {}

        public virtual void Execute() {}

        public virtual void SetSelf(NFGUID xID) { mxID = xID; }
        public virtual NFGUID Self() { return mxID; }

        private NFGUID mxID = new NFGUID();
    }
}
