using UnityEngine;
using System.Collections;

namespace NFSDK
{
    public class NFCUIPlugin : NFIPlugin
    {
        public NFCUIPlugin(NFIPluginManager pluginManager)
        {
            mPluginManager = pluginManager;
        }
        public override string GetPluginName()
        {
            return "NFCUIManager";
        }

        public override void Install()
        {
            Debug.Log("NFCUIManager Install");
            AddModule<NFCUIManager>(new NFCUIManager(mPluginManager));
        }
        public override void Uninstall()
        {
            Debug.Log("NFCUIManager Uninstall");
            mPluginManager.RemoveModule("NFCUIManager");
            mModules.Clear();
        }
    }
}
