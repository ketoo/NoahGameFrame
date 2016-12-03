//-----------------------------------------------------------------------
// <copyright file="NFKernelPlugin.cs">
//     Copyright (C) 2015-2015 lvsheng.huang <https://github.com/ketoo/NFrame>
// </copyright>
//-----------------------------------------------------------------------

#if UNITY_EDITOR
#define NF_CLIENT_FRAME
#elif UNITY_IPHONE
#define NF_CLIENT_FRAME
#elif UNITY_ANDROID
#define NF_CLIENT_FRAME
#elif UNITY_STANDALONE_OSX
#define NF_CLIENT_FRAME
#elif UNITY_STANDALONE_WIN
#define NF_CLIENT_FRAME
#endif
//-----------------------------------------------------------------------
// <copyright file="NFCHeartBeat.cs">
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
#if NF_CLIENT_FRAME
#else

	public class NFKernelPlugin : NFIPlugin
	{
        public override void Init()
        {
            CreateModule<NFCLogicClassModule>();
            CreateModule<NFCElementModule>();
            CreateModule<NFCKernelModule>();
            CreateModule<NFCEventModule>();
        }

        public override void AfterInit() { }

        public override void BeforeShut() { }

        public override void Shut() { }

        public override void Execute() { }

    }
#endif
}