using UnityEngine;
using System.Collections;
using NFSDK;

public class NFCRoot : MonoBehaviour {

    private NFCPluginManager mPluginManager;
    private static NFCRoot _instance = null;
    public static NFCRoot Instance()
    {
        return _instance;
    }
    void Start()
    {
        _instance = this;

        Debug.Log("Root Start");
        mPluginManager = new NFCPluginManager();
        mPluginManager.Registered(new NFCSDKPlugin(mPluginManager));
        mPluginManager.Registered(new NFCLogicPlugin(mPluginManager));
        mPluginManager.Registered(new NFCUIPlugin(mPluginManager));

#if UNITY_EDITOR
        mPluginManager.FindModule<NFILogicClassModule>().SetDataPath("../../_Out/");
#else
        mPluginManager.FindModule<NFILogicClassModule>().SetDataPath("./");
#endif

        mPluginManager.Init();
        mPluginManager.AfterInit();

        NFCUIManager.Instance().ShowUI<UILogin>();

        DontDestroyOnLoad(gameObject);
	}
	
    void OnDestroy()
    {
        Debug.Log("Root OnDestroy");
        mPluginManager.BeforeShut();
        mPluginManager.Shut();
        mPluginManager = null;
    }
	
	void Update () 
    {
        mPluginManager.Execute();
	}
}
