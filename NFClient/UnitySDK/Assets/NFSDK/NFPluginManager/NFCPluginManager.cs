using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using System;

namespace NFSDK
{
    public class NFCPluginManager : NFIPluginManager
    {
        public NFCPluginManager()
        {
            mPluginManager = this;
        }
        //------------- ½Ó¿Ú -------------------//

        public override bool Awake()
        {
            foreach (NFIPlugin plugin in mPlugins.Values)
            {
                if (plugin != null)
                {
                    plugin.Awake();
                }
            }
            return true;
        }

        public override bool Init()
        {
            mInitTime = DateTime.Now.Ticks / 10000;
            foreach (NFIPlugin plugin in mPlugins.Values)
            {
                if (plugin != null)
                {
                    plugin.Init();
                }
            }
            return true;
        }

        public override bool AfterInit()
        {
            foreach (NFIPlugin plugin in mPlugins.Values)
            {
                if (plugin != null)
                {
                    plugin.AfterInit();
                }
            }
            return true;
        }
        public override bool Execute()
        {
            mNowTime = DateTime.Now.Ticks / 10000;

            foreach (NFIPlugin plugin in mPlugins.Values)
            {
                if (plugin != null)
                {
                    plugin.Execute();
                }
            }
            return true;
        }

        public override bool BeforeShut()
        {
            foreach (NFIPlugin plugin in mPlugins.Values)
            {
                if (plugin != null)
                {
                    plugin.BeforeShut();
                }
            }
            return true;
        }

        public override bool Shut()
        {
            foreach (NFIPlugin plugin in mPlugins.Values)
            {
                if (plugin != null)
                {
                    plugin.Shut();
                }
            }
            return true;
        }


        public override T _FindModule<T>()
        {
            NFIModule module = _FindModule(typeof(T).ToString());

            return (T)module;
        }

        public override NFIModule _FindModule(string strModuleName)
        {
            NFIModule module;
            mModules.TryGetValue(strModuleName, out module);
            return module;
        }
        public override void Registered(NFIPlugin plugin)
        {
            mPlugins.Add(plugin.GetPluginName(), plugin);
            plugin.Install();
        }
        public override void UnRegistered(NFIPlugin plugin)
        {
            mPlugins.Remove(plugin.GetPluginName());
            plugin.Uninstall();
        }
        public override void AddModule(string strModuleName, NFIModule pModule)
        {
            Debug.LogFormat("addModule {0}", strModuleName);
            mModules.Add(strModuleName, pModule);
        }
        public override void RemoveModule(string strModuleName)
        {
            mModules.Remove(strModuleName);
        }
        public override Int64 GetInitTime()
        {
            return mInitTime;
        }
        public override Int64 GetNowTime()
        {
            return mNowTime;
        }

        protected Int64 mInitTime;
        protected Int64 mNowTime;
        protected Dictionary<string, NFIPlugin> mPlugins = new Dictionary<string, NFIPlugin>();
        protected Dictionary<string, NFIModule> mModules = new Dictionary<string, NFIModule>();
    };
}