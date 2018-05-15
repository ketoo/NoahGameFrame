using UnityEngine;
using UnityEngine.UI;
using System.Collections;
using NFSDK;

public class UISelectServer : UIDialog
{
	   
	private NFLoginModule mLoginModule;
	private NFUIModule mUIModule;
	private NFIEventModule mEventModule;

    private Transform mContentList;
    private GameObject mItemModle;
	// Use this for initialization
	void Start ()
	{
		NFIPluginManager xPluginManager = NFCPluginManager.Instance();
		mLoginModule = xPluginManager.FindModule<NFLoginModule>();
		mUIModule = xPluginManager.FindModule<NFUIModule>();
		mEventModule = xPluginManager.FindModule<NFIEventModule>();

        mContentList = transform.Find("Panel/ListView/Mask/Content");
        mItemModle = mContentList.Find("Item").gameObject;
        mItemModle.transform.SetParent(null);


		mEventModule.RegisterCallback((int)NFUIModule.Event.WorldList, OnWorldList);
		mEventModule.RegisterCallback((int)NFUIModule.Event.ServerList, OnServerList);
		mEventModule.RegisterCallback((int)NFUIModule.Event.SelectServerSuccess, OnSelectServer);

        mLoginModule.RequireWorldList();
	}
	
    // UI Event
    private void OnWorldServerClick(NFMsg.ServerInfo info)
    {
        mLoginModule.RequireConnectWorld(info.server_id);
    }
    private void OnGameServerClick(NFMsg.ServerInfo info)
    {
        mLoginModule.RequireSelectServer(info.server_id);
    }

    // Logic Event
    public void OnWorldList(NFIDataList valueList)
    {
        ArrayList serverList = mLoginModule.mWorldServerList;

        Debug.Log("OnWorldList" + serverList.Count);

        foreach (NFMsg.ServerInfo info in serverList)
        {
            GameObject item = Instantiate(mItemModle);
            item.transform.SetParent(mContentList);
            item.transform.localScale = UnityEngine.Vector3.one;
            item.GetComponentInChildren<Text>().text = System.Text.Encoding.Default.GetString(info.name);
            item.GetComponent<Button>().onClick.AddListener(() =>
            {
                OnWorldServerClick(info);
            });
        }
    }
	public void OnServerList(NFIDataList valueList)
    {
        ArrayList serverList = mLoginModule.mGameServerList;

        Debug.Log("OnServerList" + serverList.Count);

        ArrayList lst = new ArrayList();
        foreach (Transform child in mContentList.transform)  
        {  
            lst.Add(child);   
            Debug.Log(child.gameObject.name);
        }
        foreach (Transform child in lst)
        {
            Destroy(child.gameObject); 
        }
        foreach (NFMsg.ServerInfo info in serverList)
        {
            GameObject item = Instantiate(mItemModle);
            item.transform.SetParent(mContentList);
            item.transform.localScale = UnityEngine.Vector3.one;
            item.GetComponentInChildren<Text>().text = System.Text.Encoding.Default.GetString(info.name);
            item.GetComponent<Button>().onClick.AddListener(()=>
            {
                OnGameServerClick(info);
            });
        }
    }
	public void OnSelectServer(NFIDataList valueList)
    {
		mUIModule.ShowUI<UISelectRole>();
    }
}
