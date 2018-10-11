using UnityEngine;
using System.Collections;

namespace NFSDK
{
    public class NFLogicPlugin : NFIPlugin
    {
        public NFLogicPlugin(NFIPluginManager pluginManager)
        {
            mPluginManager = pluginManager;
        }
        public override string GetPluginName()
        {
			return "NFLogicPlugin";
        }

        public override void Install()
        {
			Debug.Log("NFLogicPlugin Install");
            AddModule<NFNetModule>(new NFNetModule(mPluginManager));
            AddModule<NFLoginModule>(new NFLoginModule(mPluginManager));
            AddModule<NFPlayerModule>(new NFPlayerModule(mPluginManager));
            AddModule<NFPropertyModule>(new NFPropertyModule(mPluginManager));
			AddModule<NFRecordModule>(new NFRecordModule(mPluginManager));
			AddModule<NFHelpModule>(new NFHelpModule(mPluginManager));
        }
        public override void Uninstall()
        {
			Debug.Log("NFLogicPlugin Uninstall");

			mPluginManager.RemoveModule("NFHelpModule");
			mPluginManager.RemoveModule("NFNetModule");
			mPluginManager.RemoveModule("NFLoginModule");
			mPluginManager.RemoveModule("NFPlayerModule");
			mPluginManager.RemoveModule("NFPropertyModule");
			mPluginManager.RemoveModule("NFRecordyModule");
            mModules.Clear();
        }
    }
}
