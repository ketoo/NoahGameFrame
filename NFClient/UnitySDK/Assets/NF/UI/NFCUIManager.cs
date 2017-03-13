using System;
using System.Linq;
using System.Text;
using System.Collections;
using System.Collections.Generic;
using System.IO;

using UnityEngine;

namespace NFSDK
{
	public class NFCUIManager: NFIModule
    {
        private static NFCUIManager _instance = null;
        public static NFCUIManager Instance()
        {
            return _instance;
        }

        public override bool Awake() { return true; }
        public override bool AfterInit() { return true; }
        public override bool Execute() { return true; }
        public override bool BeforeShut() { return true; }
        public override bool Shut() { return true; }

        public NFCUIManager(NFIPluginManager pluginManager)
        {
            mPluginManager = pluginManager;
            _instance = this;
		}
        
        public override bool Init() 
        { 
            return true;
        }

        public void ShowUI<T>(bool bPushHistory = true, NFIDataList varList = null) where T : UIDialog
        {
            if (mCurrentDialog != null)
            {
                mCurrentDialog.gameObject.SetActive(false);
                mCurrentDialog = null;
            }

            string name = typeof(T).ToString();
            GameObject uiObject;
            if (!mAllUIs.TryGetValue(name, out uiObject))
            {
                GameObject perfb = Resources.Load<GameObject>("UI/" + typeof(T).ToString());
                uiObject = GameObject.Instantiate(perfb);
                uiObject.name = name;

                uiObject.transform.SetParent(NFCRoot.Instance().transform);

                mAllUIs.Add(name, uiObject);
            }
            else
            {
                uiObject.SetActive(true);
            }

            if (uiObject)
            {
                T panel = uiObject.GetComponent<T>();
                if (varList != null)
                    panel.mUserData = varList;
                
                mCurrentDialog = panel;
                
                uiObject.SetActive(true);

                if (bPushHistory)
                {
                    mDialogs.Enqueue(panel);
                }
            }
        }

        public void CloseUI()
        {
            if (mCurrentDialog)
            {
                mCurrentDialog.gameObject.SetActive(false);
                mCurrentDialog = null;
            }
            mDialogs.Clear();
        }

        Dictionary<string, GameObject> mAllUIs = new Dictionary<string,GameObject>();
        Queue<UIDialog> mDialogs = new Queue<UIDialog>();
        private UIDialog mCurrentDialog = null;
    }
}