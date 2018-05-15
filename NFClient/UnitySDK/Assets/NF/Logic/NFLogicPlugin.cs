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
            mPluginManager.RemoveModule("NFNetLogic");
            mPluginManager.RemoveModule("NFLoginLogic");
            mPluginManager.RemoveModule("NFPlayerLogic");
            mPluginManager.RemoveModule("NFPropertyLogic");
            mPluginManager.RemoveModule("NFRecordyLogic");
            mModules.Clear();
        }
    }
}
