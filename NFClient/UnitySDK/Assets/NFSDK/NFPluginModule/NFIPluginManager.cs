using UnityEngine;
using System.Collections;
using System;

namespace NFSDK
{
    public abstract class NFIPluginManager : NFIModule
    {
        //------------- ½Ó¿Ú -------------------//
        public abstract T _FindModule<T>() where T : NFIModule;
        public abstract NFIModule _FindModule(string strModuleName);
        public abstract void Registered(NFIPlugin plugin);
        public abstract void UnRegistered(NFIPlugin plugin);
        public abstract void AddModule(string strModuleName, NFIModule pModule);
        public abstract void RemoveModule(string strModuleName);
        public abstract Int64 GetInitTime();
        public abstract Int64 GetNowTime();
    };
}