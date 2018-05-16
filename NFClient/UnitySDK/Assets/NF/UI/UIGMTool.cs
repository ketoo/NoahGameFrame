//-----------------------------------------------------------------------
// <copyright file="NFObjectElement.cs">
//     Copyright (C) 2015-2015 lvsheng.huang <https://github.com/ketoo/NFrame>
// </copyright>
//-----------------------------------------------------------------------

using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using NFSDK;

public class UIGMTool : UIDialog 
{

    private NFGUID xTargetIdent = new NFGUID();
    private string strTableName = "";
    private string strInfo = "";
    private string strCommand = "";

    private Vector2 scrollPositionFirst = Vector2.zero;
    private Vector2 scrollPositionSecond = Vector2.zero;
    private Vector2 scrollPositionThird = Vector2.zero;

	private NFIKernelModule mKernelModule;

	void Start()
    {
        NFIPluginManager xPluginManager = NFCPluginManager.Instance();
		mKernelModule = xPluginManager.FindModule<NFIKernelModule>();
    }

    GUIStyle buttonLeft;
	void OnGMGUI(int nHeight, int nWidth)
	{
		if (buttonLeft == null)
        {

            buttonLeft = GUI.skin.button;

            buttonLeft.alignment = TextAnchor.MiddleLeft;

        }


        int nElementWidth = 300;
        int nElementHeight = 20;

        GUI.color = Color.red;
        strInfo = GUI.TextField(new Rect(0, nHeight - 20, nElementWidth, 20), strInfo);
        strCommand = GUI.TextField(new Rect(nElementWidth, nHeight - 20, 350, 20), strCommand);
        if (GUI.Button(new Rect(nElementWidth + 350, nHeight - 20, 100, 20), "cmd"))
        {

        }
        GUI.color = Color.white;


        NFDataList objectList = mKernelModule.GetObjectList();

        scrollPositionFirst = GUI.BeginScrollView(new Rect(0, nElementHeight, nElementWidth / 2 + 20, nHeight - 100), scrollPositionFirst, new Rect(0, 0, nElementWidth, objectList.Count() * (nElementHeight)));


        //all object
        for (int i = 0; i < objectList.Count(); i++)
        {
            NFGUID ident = objectList.ObjectVal(i);

            if (GUI.Button(new Rect(0, i * nElementHeight, nElementWidth, nElementHeight), ident.nHead64.ToString() + "_" + ident.nData64.ToString()))
            {
                xTargetIdent = ident;
                strTableName = "";
                strInfo = ident.ToString();
            }

        }

        GUI.EndScrollView();

        ////////////////
        if (!xTargetIdent.IsNull())
        {
            NFIObject go = mKernelModule.GetObject(xTargetIdent);
            if (null != go)
            {
                NFDataList recordLlist = go.GetRecordManager().GetRecordList();
                NFDataList propertyList = go.GetPropertyManager().GetPropertyList();

                int nAllElement = 1;
                for (int j = 0; j < recordLlist.Count(); j++)
                {
                    string strRecordName = recordLlist.StringVal(j);
                    if (strRecordName.Length > 0)
                    {
                        nAllElement++;
                    }
                }
                for (int j = 0; j < propertyList.Count(); j++)
                {
                    string strPropertyName = propertyList.StringVal(j);
                    if (strPropertyName.Length > 0)
                    {
                        nAllElement++;
                    }
                }
                //////////////////
                scrollPositionSecond = GUI.BeginScrollView(new Rect(nElementWidth / 2 + 25, nElementHeight, nElementWidth / 2 + 25, nHeight - 100), scrollPositionSecond, new Rect(0, 0, nElementWidth, (nAllElement + 1) * (nElementHeight) + 1));

                int nElementIndex = 0;
                GUI.Button(new Rect(0, nElementIndex * nElementHeight, nElementWidth, nElementHeight), xTargetIdent.ToString());
                nElementIndex++;
                //all record
                List<string> xRecordList = new List<string>();
                for (int j = 0; j < recordLlist.Count(); j++)
                {
                    xRecordList.Add(recordLlist.StringVal(j));
                }
                xRecordList.Sort();
                for (int j = 0; j < xRecordList.Count; j++)
                {
                    string strRecordName = xRecordList[j];
                    if (strRecordName.Length > 0)
                    {
                        if (GUI.Button(new Rect(0, nElementIndex * nElementHeight, nElementWidth, nElementHeight), "++" + strRecordName))
                        {
                            strTableName = strRecordName;
                        }

                        nElementIndex++;
                    }

                }


                ///////////////////////////////
                //all property 
                List<string> xPropertyList = new List<string>();
                for (int k = 0; k < propertyList.Count(); k++)
                {
                    xPropertyList.Add(propertyList.StringVal(k));
                }
                xPropertyList.Sort();

                for (int k = 0; k < xPropertyList.Count; k++)
                {
                    string strPropertyValue = null;
                    string strPropertyName = xPropertyList[k];
                    NFIProperty property = go.GetPropertyManager().GetProperty(strPropertyName);
                    NFDataList.VARIANT_TYPE eType = property.GetType();
                    switch (eType)
                    {
                        case NFDataList.VARIANT_TYPE.VTYPE_FLOAT:
                            strPropertyValue = property.QueryFloat().ToString();
                            break;
                        case NFDataList.VARIANT_TYPE.VTYPE_INT:
                            strPropertyValue = property.QueryInt().ToString();
                            break;
                        case NFDataList.VARIANT_TYPE.VTYPE_OBJECT:
                            strPropertyValue = property.QueryObject().ToString();
                            break;
                        case NFDataList.VARIANT_TYPE.VTYPE_STRING:
                            strPropertyValue = property.QueryString();
                            break;
                        case NFDataList.VARIANT_TYPE.VTYPE_VECTOR2:
                            strPropertyValue = property.QueryVector2().ToString();
                            break;
                        case NFDataList.VARIANT_TYPE.VTYPE_VECTOR3:
                            strPropertyValue = property.QueryVector3().ToString();
                            break;
                        default:
                            strPropertyValue = "?";
                            break;
                    }

                    if (strPropertyName.Length > 0)
                    {
                        if (GUI.Button(new Rect(0, nElementIndex * nElementHeight, nElementWidth, nElementHeight), strPropertyName + ":" + strPropertyValue))
                        {
                            strTableName = "";
                            strInfo = strPropertyName + ":" + strPropertyValue;
                        }
                        nElementIndex++;
                    }

                }


                GUI.EndScrollView();
                ////////////////////////


                if (strTableName.Length > 0)
                {
                    NFIRecord record = go.GetRecordManager().GetRecord(strTableName);
                    if (null != record)
                    {
                        int nRow = record.GetRows();
                        int nCol = record.GetCols();
                        int nOffest = 30;

                        scrollPositionThird = GUI.BeginScrollView(new Rect(nElementWidth + 50, nElementHeight, nElementWidth * 3, nHeight / 2), scrollPositionThird, new Rect(0, 0, nElementWidth * nCol + nOffest, nRow * nElementHeight + nOffest));


                        string selString = null;


                        for (int row = 0; row < nRow; row++)
                        {
                            GUI.Button(new Rect(0, row * nElementHeight + nOffest, nOffest, nElementHeight), row.ToString());//row
                            for (int col = 0; col < nCol; col++)
                            {
                                if (0 == row)
                                {
                                    GUI.Button(new Rect(col * nElementWidth + nOffest, 0, nElementWidth, nElementHeight), col.ToString() + "  [" + record.GetColType(col) + "]" + record.GetColTag(col));
                                }

                                if (record.IsUsed(row))
                                {
                                    NFDataList.VARIANT_TYPE eType = record.GetColType(col);
                                    switch (eType)
                                    {
                                        case NFDataList.VARIANT_TYPE.VTYPE_INT:
                                            selString = record.QueryInt(row, col).ToString();
                                            break;

                                        case NFDataList.VARIANT_TYPE.VTYPE_FLOAT:
                                            selString = record.QueryFloat(row, col).ToString();
                                            break;

                                        case NFDataList.VARIANT_TYPE.VTYPE_STRING:
                                            selString = record.QueryString(row, col);
                                            break;

                                        case NFDataList.VARIANT_TYPE.VTYPE_OBJECT:
                                            selString = record.QueryObject(row, col).ToString();
                                            break;

                                        case NFDataList.VARIANT_TYPE.VTYPE_VECTOR2:
                                            selString = record.QueryObject(row, col).ToString();
                                            break;
                                        case NFDataList.VARIANT_TYPE.VTYPE_VECTOR3:
                                            selString = record.QueryObject(row, col).ToString();
                                            break;

                                        default:
                                            selString = "UnKnowType";
                                            break;

                                    }
                                }
                                else
                                {
                                    selString = "NoUse";
                                }

                                if (GUI.Button(new Rect(col * nElementWidth + nOffest, row * nElementHeight + nOffest, nElementWidth, nElementHeight), selString))
                                {
                                    strInfo = "Row:" + row.ToString() + " Col:" + col.ToString() + " " + selString;
                                }

                            }
                        }

                        GUI.EndScrollView();
                    }
                }
            }
        }
	}
    public void OnGUI()
    {
		OnGMGUI(800, 600);
    }
}
