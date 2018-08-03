using UnityEngine;
using UnityEngine.UI;
using System.Collections;
using NFSDK;

public class UILogin : UIDialog
{
	private NFNetModule mNetModule;
	private NFLoginModule mLoginModule;
	private NFUIModule mUIModule;
	private NFIEventModule mEventModule;

	public InputField mAccount;
	public InputField mPassword;
	public Button mLogin;
	// Use this for initialization
	void Start () 
	{
		mNetModule = NFCPluginManager.Instance().FindModule<NFNetModule>();
		mLoginModule = NFCPluginManager.Instance().FindModule<NFLoginModule>();
		mUIModule = NFCPluginManager.Instance().FindModule<NFUIModule>();
		mEventModule = NFCPluginManager.Instance().FindModule<NFIEventModule>();

        mLogin.onClick.AddListener(onLoginClick);

        mNetModule.ConnectServer("104.160.35.67", 14001);

		mEventModule.RegisterCallback((int)NFUIModule.Event.LoginSuccess, OnLoginSuccess);
	}

    // UI Event
    private void onLoginClick()
    {
        mLoginModule.LoginPB(mAccount.text, mPassword.text, "");
    }
    
    // Logic Event
	public void OnLoginSuccess(NFDataList valueList)
    {
		mUIModule.ShowUI<UISelectServer>();
    }
}
