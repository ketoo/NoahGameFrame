using UnityEngine;
using UnityEngine.UI;
using System.Collections;
using NFSDK;

public class GameScene : MonoBehaviour {
	// Use this for initialization
    public NFIDataList mUserData = null;
	void Start () {
        NFCKernelModule.Instance().RegisterClassCallBack(NFrame.Player.ThisName, OnClassPlayerEventHandler);
        NFCKernelModule.Instance().RegisterClassCallBack(NFrame.NPC.ThisName, OnClassNPCEventHandler);

        NFCPlayerLogic.Instance().RegisterCallback((int)NFCPlayerLogic.Event.PlayerMove, OnPlayerMove);
	}

    public bool OnPlayerMove(NFIDataList valueList)
    {
        NFGUID tar = valueList.ObjectVal(0);

        if (tar == NFCPlayerLogic.Instance().mRoleID)
            return true;

        OtherPlayer player = transform.Find(tar.ToString()).GetComponent<OtherPlayer>();

        double fSpeed = valueList.FloatVal(1);
        NFVector3 pos = valueList.Vector3Val(2);

        player.MoveTo(new Vector3(pos.X(), pos.Y(), pos.Z()));
        Debug.Log("Player Move:" + new Vector3(pos.X(), pos.Y(), pos.Z()).ToString());

        return true;
    }

    private void OnClassPlayerEventHandler(NFGUID self, int nContainerID, int nGroupID, NFIObject.CLASS_EVENT_TYPE eType, string strClassName, string strConfigIndex)
    {
        if (eType == NFIObject.CLASS_EVENT_TYPE.OBJECT_CREATE)
        {
            Debug.Log("OBJECT_CREATE:" + self.ToString());
            string strConfigID = NFCKernelModule.Instance().QueryPropertyString(self, "ConfigID");
            Vector3 vec = new Vector3();
            vec.x = (float)NFCKernelModule.Instance().QueryPropertyFloat(self, "X");
            vec.y = (float)NFCKernelModule.Instance().QueryPropertyFloat(self, "Y");
            vec.z = (float)NFCKernelModule.Instance().QueryPropertyFloat(self, "Z");

            //　MainPlayer
            string strPrefabPath = "Player/AIThirdPersonController";
            if (self == NFCPlayerLogic.Instance().mRoleID)
            {
                strPrefabPath = "Player/ThirdPersonController";
            }
            //if (strConfigID.Length <= 0)
            //{
            //    strPrefabPath = NFCElementModule.Instance().QueryPropertyString("Player", "Prefab");
            //}
            //else
            //{
            //    strPrefabPath = NFCKernelModule.Instance.GetElementModule().QueryPropertyString(strConfigID, "Prefab");
            //}

            //CreateObject(self, strPrefabPath, vec, strClassName);

            GameObject perfb = Resources.Load<GameObject>(strPrefabPath);
            GameObject player = GameObject.Instantiate(perfb);
            player.name = self.ToString();
            player.transform.SetParent(transform);
            player.transform.position = vec;

            //　MainPlayer
            if (self == NFCPlayerLogic.Instance().mRoleID)
            {
                player.AddComponent<MainPlayer>();
            }
            else
            {
                var other = player.AddComponent<OtherPlayer>();
                other.MoveTo(vec);
            }
        }
        else if (eType == NFIObject.CLASS_EVENT_TYPE.OBJECT_DESTROY)
        {
            DestroyObject(transform.Find(self.ToString()));
        }

    }
    private void OnClassNPCEventHandler(NFGUID self, int nContainerID, int nGroupID, NFIObject.CLASS_EVENT_TYPE eType, string strClassName, string strConfigIndex)
    {
        //if (eType == NFIObject.CLASS_EVENT_TYPE.OBJECT_CREATE)
        //{
        //    string strConfigID = NFCKernelModule.Instance().QueryPropertyString(self, "ConfigID");
        //    Vector3 vec = new Vector3();
        //    vec.x = (float)NFCKernelModule.Instance().QueryPropertyFloat(self, "X");
        //    vec.y = (float)NFCKernelModule.Instance().QueryPropertyFloat(self, "Y");
        //    vec.z = (float)NFCKernelModule.Instance().QueryPropertyFloat(self, "Z");

        //    string strPrefabPath = "";
        //    if (strConfigID.Length <= 0)
        //    {
        //        strPrefabPath = NFCElementModule.Instance().QueryPropertyString("Player", "Prefab");
        //    }
        //    else
        //    {
        //        strPrefabPath = NFCElementModule.Instance()..QueryPropertyString(strConfigID, "Prefab");
        //    }

        //    strPrefabPath = "Player/MainPlayer";
        //    GameObject playerPerf = Resources.Load<GameObject>(strPrefabPath);
        //    GameObject player = Instantiate(playerPerf);
            


        //}
        //else if (eType == NFIObject.CLASS_EVENT_TYPE.OBJECT_DESTROY)
        //{
        //    //DestroyObject(self);
        //}

    }
}
