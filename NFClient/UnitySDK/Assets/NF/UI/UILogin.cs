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

    private InputField mAccount;
    private InputField mPassword;
    private Button mLogin;
	// Use this for initialization
	void Start () 
	{
		mNetModule = NFCPluginManager.Instance().FindModule<NFNetModule>();
		mLoginModule = NFCPluginManager.Instance().FindModule<NFLoginModule>();
		mUIModule = NFCPluginManager.Instance().FindModule<NFUIModule>();
		mEventModule = NFCPluginManager.Instance().FindModule<NFIEventModule>();


        mAccount = transform.Find("Panel/Account").GetComponent<InputField>();
        mPassword = transform.Find("Panel/Password").GetComponent<InputField>();
        mLogin = transform.Find("Panel/Login").GetComponent<Button>();
        
        mLogin.onClick.AddListener(onLoginClick);

        mNetModule.ConnectServer("104.160.35.67", 14001);
        //NFCNetLogic.Instance().ConnectServer("127.0.0.1", 14001);

		mEventModule.RegisterCallback((int)NFUIModule.Event.LoginSuccess, OnLoginSuccess);
	}

    // UI Event
    private void onLoginClick()
    {
        mLoginModule.LoginPB(mAccount.text, mPassword.text, "");
    }
    
    // Logic Event
	public void OnLoginSuccess(NFIDataList valueList)
    {
		mUIModule.ShowUI<UISelectServer>();
    }
}
