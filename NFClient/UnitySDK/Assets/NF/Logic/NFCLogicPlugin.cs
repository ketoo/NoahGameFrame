using UnityEngine;
using System.Collections;

namespace NFSDK
{
    public class NFCLogicPlugin : NFIPlugin
    {
        public NFCLogicPlugin(NFIPluginManager pluginManager)
        {
            mPluginManager = pluginManager;
        }
        public override string GetPluginName()
        {
            return "NFCLogicPlugin";
        }

        public override void Install()
        {
            Debug.Log("NFCLogicPlugin Install");
            AddModule<NFCNetLogic>(new NFCNetLogic(mPluginManager));
            AddModule<NFCLoginLogic>(new NFCLoginLogic(mPluginManager));
            AddModule<NFCPlayerLogic>(new NFCPlayerLogic(mPluginManager));
            AddModule<NFCPropertyLogic>(new NFCPropertyLogic(mPluginManager));
            AddModule<NFCRecordyLogic>(new NFCRecordyLogic(mPluginManager));
        }
        public override void Uninstall()
        {
            Debug.Log("NFCLogicPlugin Uninstall");
            mPluginManager.RemoveModule("NFCNetLogic");
            mPluginManager.RemoveModule("NFCLoginLogic");
            mPluginManager.RemoveModule("NFCPlayerLogic");
            mPluginManager.RemoveModule("NFCPropertyLogic");
            mPluginManager.RemoveModule("NFCRecordyLogic");
            mModules.Clear();
        }
    }
}
