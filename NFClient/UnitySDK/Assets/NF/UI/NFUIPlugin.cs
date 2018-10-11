using UnityEngine;
using System.Collections;

namespace NFSDK
{
    public class NFUIPlugin : NFIPlugin
    {
        public NFUIPlugin(NFIPluginManager pluginManager)
        {
            mPluginManager = pluginManager;
        }
        public override string GetPluginName()
        {
			return "NFUIPlugin";
        }

        public override void Install()
        {
			Debug.Log("NFUIPlugin Install");
            AddModule<NFUIModule>(new NFUIModule(mPluginManager));
        }
        public override void Uninstall()
        {
			Debug.Log("NFUIPlugin Uninstall");

			mPluginManager.RemoveModule("NFUIModule");

            mModules.Clear();
        }
    }
}
