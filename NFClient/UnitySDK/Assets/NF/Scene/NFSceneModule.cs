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
		private NFIEventModule mEventModule;


		private Dictionary<NFGUID, GameObject> mGameObjectMap = new Dictionary<NFGUID, GameObject>();

        public NFSceneModule(NFIPluginManager pluginManager)
        {
            mPluginManager = pluginManager;
        }
  

		public override void Awake() 
		{ 
		}
		public override void Init()
		{ 
		}

		public override void AfterInit() 
		{
			mPlayerModule = FindModule<NFPlayerModule>();
			mNetModule = FindModule<NFNetModule>();
			mKernelModule = FindModule<NFIKernelModule>();
			mEventModule = FindModule<NFIEventModule>();

            mKernelModule.RegisterClassCallBack(NFrame.Player.ThisName, OnClassPlayerEventHandler);
            mKernelModule.RegisterClassCallBack(NFrame.NPC.ThisName, OnClassNPCEventHandler);

			mEventModule.RegisterCallback((int)NFPlayerModule.Event.SwapScene, SwapSceneEventHandler);
			mEventModule.RegisterCallback((int)NFPlayerModule.Event.PlayerMove, OnPlayerMove);

		}

		public override void Execute() {}
		public override void BeforeShut() {  }
		public override void Shut() {}

		protected void SwapSceneEventHandler(NFDataList valueList)
		{
			string strSceneID = valueList.StringVal(0);
			NFVector3 vPos = valueList.Vector3Val(1);

            //TODO
			Application.LoadLevel(1);
		}

		GameObject GetObject(NFGUID id)
		{
			GameObject gameObject = null;
			mGameObjectMap.TryGetValue(id, out gameObject);

			return gameObject;
		}
  

		public void OnPlayerMove(NFDataList valueList)
        {
            NFGUID tar = valueList.ObjectVal(0);

			if (tar == mPlayerModule.mRoleID)
                return;

			GameObject player = GetObject(tar);
			OtherPlayer otherPlayer = player.GetComponent<OtherPlayer>();

			double fSpeed = valueList.FloatVal(1);
			long nType = valueList.IntVal(2);
            NFVector3 pos = valueList.Vector3Val(3);

			if (nType > 0)
			{
				otherPlayer.JumpTo(new Vector3(pos.X(), pos.Y(), pos.Z()));
			}
			else
			{
				otherPlayer.MoveTo(new Vector3(pos.X(), pos.Y(), pos.Z()));
			}

            Debug.Log("Player Move:" + new Vector3(pos.X(), pos.Y(), pos.Z()).ToString());
        }

        private void OnClassPlayerEventHandler(NFGUID self, int nContainerID, int nGroupID, NFIObject.CLASS_EVENT_TYPE eType, string strClassName, string strConfigIndex)
        {
            if (eType == NFIObject.CLASS_EVENT_TYPE.OBJECT_CREATE)
            {
                Debug.Log("OBJECT_CREATE:" + self.ToString());

                string strConfigID = mKernelModule.QueryPropertyString(self, "ConfigID");
                Vector3 vec = new Vector3();
                vec.x = (float)mKernelModule.QueryPropertyFloat(self, "X");
                vec.y = (float)mKernelModule.QueryPropertyFloat(self, "Y");
                vec.z = (float)mKernelModule.QueryPropertyFloat(self, "Z");

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

				mGameObjectMap.Add(self, player);

				GameObject.DontDestroyOnLoad(player);

                player.name = self.ToString();
                player.transform.position = vec;

                //MainPlayer
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
				GameObject go = GetObject(self);
				if (go != null)
				{
					mGameObjectMap.Remove(self);

					GameObject.DestroyObject(go);
				}
            }
		}

        private void OnClassNPCEventHandler(NFGUID self, int nContainerID, int nGroupID, NFIObject.CLASS_EVENT_TYPE eType, string strClassName, string strConfigIndex)
        {
            if (eType == NFIObject.CLASS_EVENT_TYPE.OBJECT_CREATE)
            {
                string strConfigID = mKernelModule.QueryPropertyString(self, "ConfigID");
                Vector3 vec = new Vector3();
                vec.x = (float)mKernelModule.QueryPropertyFloat(self, "X");
                vec.y = (float)mKernelModule.QueryPropertyFloat(self, "Y");
                vec.z = (float)mKernelModule.QueryPropertyFloat(self, "Z");

                string strPrefabPath = "";
            //    if (strConfigID.Length <= 0)
            //    {
            //        strPrefabPath = NFCElementModule.Instance().QueryPropertyString("Player", "Prefab");
            //    }
            //    else
            //    {
            //        strPrefabPath = NFCElementModule.Instance()..QueryPropertyString(strConfigID, "Prefab");
            //    }

                strPrefabPath = "Player/MainPlayer";
                GameObject playerPerf = Resources.Load<GameObject>(strPrefabPath);
				GameObject player = GameObject.Instantiate(playerPerf);


                mGameObjectMap.Add(self, player);

            }
            else if (eType == NFIObject.CLASS_EVENT_TYPE.OBJECT_DESTROY)
            {
				GameObject go = GetObject(self);
                if (go != null)
                {
                    mGameObjectMap.Remove(self);

                    GameObject.DestroyObject(go);
                }
            }

        }
	}
}
