using UnityEngine;
using System.Collections;
using System.Collections.Generic;

namespace NFSDK
{
    public abstract class NFIPlugin : NFIModule
    {
        //------------- ½Ó¿Ú -------------------//
        public abstract string GetPluginName();
        public abstract void Install();
        public abstract void Uninstall();
        public override bool Awake()
        {
            foreach (NFIModule module in mModules.Values)
            {
                if (module != null)
                {
                    module.Awake();
                }
            }
            return true;
        }

        public override bool Init()
        {
            foreach (NFIModule module in mModules.Values)
            {
                if (module != null)
                {
                    module.Init();
                    Debug.LogFormat("NFIPlugin module Init");
                }
            }
            return true;
        }

        public override bool AfterInit()
        {
            foreach (NFIModule module in mModules.Values)
            {
                if (module != null)
                {
                    module.AfterInit();
                }
            }
            return true;
        }

        public override bool Execute()
        {
            foreach (NFIModule module in mModules.Values)
            {
                if (module != null)
                {
                    module.Execute();
                }
            }
            return true;
        }

        public override bool BeforeShut()
        {
            foreach (NFIModule module in mModules.Values)
            {
                if (module != null)
                {
                    module.BeforeShut();
                }
            }
            return true;
        }

        public override bool Shut()
        {
            foreach (NFIModule module in mModules.Values)
            {
                if (module != null)
                {
                    module.Shut();
                }
            }
            return true;
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