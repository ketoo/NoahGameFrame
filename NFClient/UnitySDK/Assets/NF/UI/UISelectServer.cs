using UnityEngine;
using UnityEngine.UI;
using System.Collections;
using NFSDK;

public class UISelectServer : UIDialog
{
    private Transform mContentList;
    private GameObject mItemModle;
	// Use this for initialization
	void Start () {

        mContentList = transform.Find("Panel/ListView/Mask/Content");
        mItemModle = mContentList.Find("Item").gameObject;
        mItemModle.transform.SetParent(null);
        NFCLoginLogic.Instance().RegisterCallback((int)NFCLoginLogic.Event.WorldList, OnWorldList);
        NFCLoginLogic.Instance().RegisterCallback((int)NFCLoginLogic.Event.ServerList, OnServerList);
        NFCLoginLogic.Instance().RegisterCallback((int)NFCLoginLogic.Event.SelectServerSuccess, OnSelectServer);

        NFCLoginLogic.Instance().RequireWorldList();
	}
	
    // UI Event
    private void OnWorldServerClick(NFMsg.ServerInfo info)
    {
        NFCLoginLogic.Instance().RequireConnectWorld(info.server_id);
    }
    private void OnGameServerClick(NFMsg.ServerInfo info)
    {
        NFCLoginLogic.Instance().RequireSelectServer(info.server_id);
    }

    // Logic Event
    public bool OnWorldList(NFIDataList valueList)
    {
        ArrayList serverList = NFCLoginLogic.Instance().mWorldServerList;

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
        return true;
    }
    public bool OnServerList(NFIDataList valueList)
    {
        ArrayList serverList = NFCLoginLogic.Instance().mGameServerList;

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
        return true;
    }
    public bool OnSelectServer(NFIDataList valueList)
    {
        NFCUIManager.Instance().ShowUI<UISelectRole>();
        return true;
    }
}
