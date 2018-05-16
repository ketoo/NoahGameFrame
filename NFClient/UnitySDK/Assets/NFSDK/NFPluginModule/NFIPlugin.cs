using UnityEngine;
using System.Collections;
using System.Collections.Generic;

namespace NFSDK
{
    public abstract class NFIPlugin : NFIModule
    {
        //------------- 接口 -------------------//
        public abstract string GetPluginName();
        public abstract void Install();
        public abstract void Uninstall();
        public override void Awake()
        {
            foreach (NFIModule module in mModules.Values)
            {
                if (module != null)
                {
                    module.Awake();
                }
            }
        }

        public override void Init()
        {
            foreach (NFIModule module in mModules.Values)
            {
                if (module != null)
                {
                    module.Init();
                    Debug.LogFormat("NFIPlugin module Init");
                }
            }
        }

        public override void AfterInit()
        {
            foreach (NFIModule module in mModules.Values)
            {
                if (module != null)
                {
                    module.AfterInit();
                }
            }
        }

        public override void Execute()
        {
            foreach (NFIModule module in mModules.Values)
            {
                if (module != null)
                {
					module.Execute();
                }
            }
        }

        public override void BeforeShut()
        {
            foreach (NFIModule module in mModules.Values)
            {
                if (module != null)
                {
                    module.BeforeShut();
                }
            }
        }

        public override void Shut()
        {
            foreach (NFIModule module in mModules.Values)
            {
                if (module != null)
                {
                    module.Shut();
                }
            }
        }

        public void AddModule<T1>(NFIModule module)
        {
            string strName = typeof(T1).ToString();
            mPluginManager.AddModule(strName, module);
            mModules.Add(strName, module);
        }

        protected Dictionary<string, NFIModule> mModules = new Dictionary<string, NFIModule>();
    };
}