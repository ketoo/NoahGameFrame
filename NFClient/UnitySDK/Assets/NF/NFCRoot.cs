using UnityEngine;
using System.Collections;
using NFSDK;

public class NFCRoot : MonoBehaviour 
{

	NFIClassModule mClassModule;
	NFNetModule mNetModule;
	NFUIModule mUIModule;

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
        mPluginManager.Registered(new NFLogicPlugin(mPluginManager));
		mPluginManager.Registered(new NFUIPlugin(mPluginManager));
		mPluginManager.Registered(new NFScenePlugin(mPluginManager));


		mClassModule = mPluginManager.FindModule<NFIClassModule>();
		mNetModule = mPluginManager.FindModule<NFNetModule>();
		mUIModule = mPluginManager.FindModule<NFUIModule>();

		mClassModule.SetDataPath("../../_Out/");

		if (RuntimePlatform.Android == Application.platform
		    ||RuntimePlatform.IPhonePlayer == Application.platform)
		{
			mPluginManager.FindModule<NFIClassModule>().SetDataPath("./");
		}

        mPluginManager.Init();
        mPluginManager.AfterInit();

		mUIModule.ShowUI<UILogin>();

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
