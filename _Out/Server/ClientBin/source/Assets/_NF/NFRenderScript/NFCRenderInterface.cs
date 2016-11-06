//-----------------------------------------------------------------------
// <copyright file="NFCRenderInterface.cs">
//     Copyright (C) 2015-2015 lvsheng.huang <https://github.com/ketoo/NFrame>
// </copyright>
//-----------------------------------------------------------------------
using UnityEngine;
using System.Collections;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System;
using NFrame;
using NFTCPClient;
using System.Collections.Generic;

public class NFCRenderInterface
{
    #region Instance
    private static NFCRenderInterface _Instance = null;
    private static readonly object _syncLock = new object();
    public static NFCRenderInterface Instance
    {
        get
        {
            lock (_syncLock)
            {
                if (_Instance == null)
                {
                    _Instance = new NFCRenderInterface();
                    _Instance.Init();
                }
                return _Instance;
            }
        }
    }
    #endregion

    public void Init()
    {
        NFCKernelModule.Instance.RegisterClassCallBack("Player", OnClassPlayerEventHandler);
        NFCKernelModule.Instance.RegisterClassCallBack("NPC", OnClassNPCEventHandler);
    }

    private void OnClassPlayerEventHandler(NFGUID self, int nContainerID, int nGroupID, NFIObject.CLASS_EVENT_TYPE eType, string strClassName, string strConfigIndex)
    {
        if (eType == NFIObject.CLASS_EVENT_TYPE.OBJECT_CREATE)
        {
            string strConfigID = NFCKernelModule.Instance.QueryPropertyString(self, "ConfigID");
            Vector3 vec = new Vector3();
            vec.x = (float)NFCKernelModule.Instance.QueryPropertyFloat(self, "X");
            vec.y = (float)NFCKernelModule.Instance.QueryPropertyFloat(self, "Y");
            vec.z = (float)NFCKernelModule.Instance.QueryPropertyFloat(self, "Z");

            string strPrefabPath = "";
            if (strConfigID.Length <= 0)
            {
                strPrefabPath = NFCKernelModule.Instance.GetElementModule().QueryPropertyString("Player", "Prefab");
            }
            else
            {
                strPrefabPath = NFCKernelModule.Instance.GetElementModule().QueryPropertyString(strConfigID, "Prefab");
            }

            //CreateObject(self, strPrefabPath, vec, strClassName);
        }
        else if (eType == NFIObject.CLASS_EVENT_TYPE.OBJECT_DESTROY)
        {
            //DestroyObject(self);
        }
        
    }

    private void OnClassNPCEventHandler(NFGUID self, int nContainerID, int nGroupID, NFIObject.CLASS_EVENT_TYPE eType, string strClassName, string strConfigIndex)
    {
        if (eType == NFIObject.CLASS_EVENT_TYPE.OBJECT_CREATE)
        {
            string strConfigID = NFCKernelModule.Instance.QueryPropertyString(self, "ConfigID");
            Vector3 vec = new Vector3();
            vec.x = (float)NFCKernelModule.Instance.QueryPropertyFloat(self, "X");
            vec.y = (float)NFCKernelModule.Instance.QueryPropertyFloat(self, "Y");
            vec.z = (float)NFCKernelModule.Instance.QueryPropertyFloat(self, "Z");

            string strPrefabPath = "";
            if (strConfigID.Length <= 0)
            {
                strPrefabPath = NFCKernelModule.Instance.GetElementModule().QueryPropertyString("Player", "Prefab");
            }
            else
            {
                strPrefabPath = NFCKernelModule.Instance.GetElementModule().QueryPropertyString(strConfigID, "Prefab");
            }

            //CreateObject(self, strPrefabPath, vec, strClassName);
        }
        else if (eType == NFIObject.CLASS_EVENT_TYPE.OBJECT_DESTROY)
        {
            //DestroyObject(self);
        }
        
    }
    public enum RecordBuildingList
    {
        BuildingID = 0,
        BuildingGUID = 1,
    }

    /////////////////////////////////////////////////////////////////
    private void OnBuildRecordEventHandler(NFGUID self, string strRecordName, NFIRecord.eRecordOptype eType, int nRow, int nCol, NFIDataList oldVar, NFIDataList newVar)
    {
 
    }

   
    ///////////////////////////////////////////////////////////////////////////////////////
    Dictionary<NFGUID, GameObject> mhtObject = new Dictionary<NFGUID, GameObject>();
}