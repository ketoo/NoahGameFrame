using UnityEngine;
using UnityEngine.UI;
using System.Collections;
using NFSDK;

public class UISelectRole : UIDialog {

    private NFLoginModule mLoginModule;
	private NFUIModule mUIModule;
	private NFPlayerModule mPlayerModule;
	private NFIEventModule mEventModule;

    private Transform mContentList;
    private GameObject mItemModle;
	public Button mCreateRole;
	public Button mEnterGameRole;

	// Use this for initialization
	void Start ()
	{

		NFIPluginManager xPluginManager = NFCPluginManager.Instance();
        mLoginModule = xPluginManager.FindModule<NFLoginModule>();
		mUIModule = xPluginManager.FindModule<NFUIModule>();
		mPlayerModule = xPluginManager.FindModule<NFPlayerModule>();
		mEventModule = xPluginManager.FindModule<NFIEventModule>();


        mContentList = transform.Find("Panel/ListView/Mask/Content");
        mItemModle = mContentList.Find("Item").gameObject;
        mItemModle.transform.SetParent(null);

		mCreateRole.onClick.AddListener(onCreateRoleClick);
		mEnterGameRole.onClick.AddListener(onEnterGameClick);

		mEventModule.RegisterCallback((int)NFPlayerModule.Event.RoleList, OnRoleList);

		mPlayerModule.RequireRoleList();
    }

    // UI Event
    private void onCreateRoleClick()
    {
        string strRoleName = "TestRole" + Random.Range(0, 10000);
		mPlayerModule.RequireCreateRole(strRoleName, 1, 1);
    }

	private void onEnterGameClick()
    {
		ArrayList roleList = mPlayerModule.mRoleList;
		if (roleList.Count > 0)
		{
			OnRoleClick(0);
		}
    }

    private void OnRoleClick(int nIndex)
    {
		mPlayerModule.RequireEnterGameServer(nIndex);
		mUIModule.CloseUI();
    }

    // Logic Event
    public void OnRoleList(NFDataList valueList)
    {
		ArrayList roleList = mPlayerModule.mRoleList;

		Debug.Log("OnRoleList" + roleList.Count);

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
        int nIndex = 0;
		foreach (NFMsg.RoleLiteInfo info in roleList)
        {
            GameObject item = Instantiate(mItemModle);
            item.transform.SetParent(mContentList);
            item.transform.localScale = UnityEngine.Vector3.one;
            item.GetComponentInChildren<Text>().text = System.Text.Encoding.Default.GetString(info.noob_name);
            int nRoleIndex = nIndex;
            item.GetComponent<Button>().onClick.AddListener(() =>
            {
                OnRoleClick(nRoleIndex);
            });
            nIndex++;
        }
    }
}
