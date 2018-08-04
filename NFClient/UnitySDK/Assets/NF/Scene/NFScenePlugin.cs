using UnityEngine;
using System.Collections;

namespace NFSDK
{
    public class NFScenePlugin : NFIPlugin
    {
		public NFScenePlugin(NFIPluginManager pluginManager)
        {
            mPluginManager = pluginManager;
        }
        public override string GetPluginName()
        {
			return "NFScenePlugin";
        }

        public override void Install()
        {
			Debug.Log("NFScenePlugin Install");
			AddModule<NFSceneModule>(new NFSceneModule(mPluginManager));
        }
        public override void Uninstall()
        {
			Debug.Log("NFScenePlugin Uninstall");

			mPluginManager.RemoveModule("NFSceneModule");

            mModules.Clear();
        }
    }
}
