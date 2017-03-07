using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace NFSDK
{
    public class NFCNetDispatcher : NFCMessageDispatcher
    {
        private static NFCNetDispatcher _instance = null;
        public static NFCNetDispatcher Instance()
        {
            if (_instance == null)
            {
                _instance = new NFCNetDispatcher();
            }
            return _instance;
        }
    };
}