using UnityEngine;
using UnityEngine.UI;
using System.Collections;
using NFSDK;

public class UISelectRole : UIDialog {

    private Transform mContentList;
    private GameObject mItemModle;
    private Button mCreateRole;
	// Use this for initialization
	void Start () {
        mContentList = transform.Find("Panel/ListView/Mask/Content");
        mItemModle = mContentList.Find("Item").gameObject;
        mItemModle.transform.SetParent(null);

        mCreateRole = transform.Find("Panel/CreateRole").GetComponent<Button>();
        mCreateRole.onClick.AddListener(onCreateRoleClick);

        NFCPlayerLogic.Instance().RegisterCallback((int)NFCPlayerLogic.Event.RoleList, OnRoleList);

        NFCPlayerLogic.Instance().RequireRoleList();
    }

    // UI Event
    private void onCreateRoleClick()
    {
        string strRoleName = "TestRole" + Random.Range(0, 1000);
        NFCPlayerLogic.Instance().RequireCreateRole(strRoleName, 1, 1);
    }
    private void OnRoleClick(int nIndex)
    {
        NFCPlayerLogic.Instance().RequireEnterGameServer(nIndex);
        NFCUIManager.Instance().CloseUI();
        Application.LoadLevel(1);
    }

    // Logic Event
    public bool OnRoleList(NFIDataList valueList)
    {
        ArrayList serverList = NFCPlayerLogic.Instance().mRoleList;

        Debug.Log("OnRoleList" + serverList.Count);

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
        foreach (NFMsg.RoleLiteInfo info in serverList)
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
        return true;
    }
}
