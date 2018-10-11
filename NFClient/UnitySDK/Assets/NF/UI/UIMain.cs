using UnityEngine;
using UnityEngine.UI;
using System.Collections;
using NFSDK;

public class UIMain : UIDialog
{
	   
	private NFLoginModule mLoginModule;
	private NFUIModule mUIModule;
	private NFIEventModule mEventModule;

	// Use this for initialization
	void Start ()
	{
		NFIPluginManager xPluginManager = NFCPluginManager.Instance();
		mLoginModule = xPluginManager.FindModule<NFLoginModule>();
		mUIModule = xPluginManager.FindModule<NFUIModule>();
		mEventModule = xPluginManager.FindModule<NFIEventModule>();

	}

}
