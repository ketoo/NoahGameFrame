using UnityEngine;
using UnityEngine.UI;
using System.Collections;
using NFSDK;

public class UILogin : UIDialog
{
    private InputField mAccount;
    private InputField mPassword;
    private Button mLogin;
	// Use this for initialization
	void Start () {
        mAccount = transform.Find("Panel/Account").GetComponent<InputField>();
        mPassword = transform.Find("Panel/Password").GetComponent<InputField>();
        mLogin = transform.Find("Panel/Login").GetComponent<Button>();
        
        mLogin.onClick.AddListener(onLoginClick);

        //NFCNetLogic.Instance().ConnectServer("104.160.35.67", 14001);
        NFCNetLogic.Instance().ConnectServer("127.0.0.1", 14001);

        NFCLoginLogic.Instance().RegisterCallback((int)NFCLoginLogic.Event.LoginSuccess, OnLoginSuccess);
	}

    // UI Event
    private void onLoginClick()
    {
        NFCLoginLogic.Instance().LoginPB(mAccount.text, mPassword.text, "");
    }
    
    // Logic Event
    public bool OnLoginSuccess(NFIDataList valueList)
    {
        NFCUIManager.Instance().ShowUI<UISelectServer>();
        return true;
    }
}
