using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace NFSDK
{
	public class NFSceneModule : NFIModule
	{
		private NFIKernelModule mKernelModule;
		private NFNetModule mNetModule;
		private NFPlayerModule mPlayerModule;

        public NFSceneModule(NFIPluginManager pluginManager)
        {
            mPluginManager = pluginManager;
        }
  

		public override bool Awake() { return true; }
		public override bool Init() { return true; }

		public override bool AfterInit() 
		{
			mPlayerModule = FindModule<NFPlayerModule>();
			mNetModule = FindModule<NFNetModule>();
			mKernelModule = FindModule<NFIKernelModule>();

            NFCKernelModule.Instance().RegisterClassCallBack(NFrame.Player.ThisName, OnClassPlayerEventHandler);
            NFCKernelModule.Instance().RegisterClassCallBack(NFrame.NPC.ThisName, OnClassNPCEventHandler);

			//mPlayerModule.RegisterCallback((int)NFPlayerModule.Event.PlayerMove, OnPlayerMove);

			return true;
		}

		public override bool Execute() { return true; }
		public override bool BeforeShut() { return true; }
		public override bool Shut() { return true; }


		GameObject GetObject(NFGUID id)
		{
			return null;
		}
  

        public bool OnPlayerMove(NFIDataList valueList)
        {
			/*
            NFGUID tar = valueList.ObjectVal(0);

            if (tar == NFPlayerModule.Instance().mRoleID)
                return true;

            OtherPlayer player = transform.Find(tar.ToString()).GetComponent<OtherPlayer>();

            double fSpeed = valueList.FloatVal(1);
            NFVector3 pos = valueList.Vector3Val(2);

            player.MoveTo(new Vector3(pos.X(), pos.Y(), pos.Z()));
            Debug.Log("Player Move:" + new Vector3(pos.X(), pos.Y(), pos.Z()).ToString());
*/
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

                //MainPlayer
                string strPrefabPath = "Player/AIThirdPersonController";
				if (self == mPlayerModule.mRoleID)
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

				//DontDestroyOnLoad(player);

                player.name = self.ToString();
                player.transform.position = vec;

                //　MainPlayer
				if (self == mPlayerModule.mRoleID)
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
                //DestroyObject(transform.Find(self.ToString()));
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
}
