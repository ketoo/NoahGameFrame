using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using NFCoreEx;

public class NFObjectElement
{
	
	public NFIDENTID targetIdent = new NFIDENTID();
	private string strTableName = "";
    private string strInfo = "";
    private string strCommand = "";
	
	private Vector2 scrollPositionFirst = Vector2.zero;
	private Vector2 scrollPositionSecond = Vector2.zero;
	private Vector2 scrollPositionThird = Vector2.zero;
	
	GUIStyle buttonLeft;
    public void OnGUI(NFIKernel kernel, int nHeight, int nWidth)
	{
		if (buttonLeft == null)
		{
	
	        buttonLeft = GUI.skin.button;
	
	        buttonLeft.alignment = TextAnchor.MiddleLeft;
	
	    }

		
		int nElementWidth = 150;
		int nElementHeight = 20;

        GUI.color = Color.red;
        strInfo = GUI.TextField(new Rect(0, nHeight - 20, nElementWidth * 3f + 120, 20), strInfo);
        strCommand = GUI.TextField(new Rect(nElementWidth * 3f + 120, nHeight - 20, 350, 20), strCommand);
        if (GUI.Button(new Rect(nWidth - 100, nHeight - 20, 100, 20),  "cmd"))
        {

        }
        GUI.color = Color.white;


        NFIValueList objectList = kernel.GetObjectList();

        scrollPositionFirst = GUI.BeginScrollView(new Rect(0, nElementHeight, nElementWidth / 2 + 20, nHeight), scrollPositionFirst, new Rect(0, 0, nElementWidth, objectList.Count() * (nElementHeight)));


		//all object
		for (int i = 0; i < objectList.Count(); i++)
		{
			NFIDENTID ident = objectList.ObjectVal(i);
			int nSerial = (int)(ident.nData64 >>32 & 0xFFFFFFFF);
			int nIdent = (int)(ident.nData64 & 0xFFFFFFFF);
			if(GUI.Button(new Rect(0, i*nElementHeight, nElementWidth/2, nElementHeight), ident.nData64.ToString()))
			{
                targetIdent = ident;
				strTableName = "";
			}
			
		}
		
		GUI.EndScrollView();
			
		////////////////
		if(!targetIdent.IsNull())
		{
			NFIObject go = kernel.GetObject(targetIdent);
			
			
			NFIValueList recordLlist = go.GetRecordManager().GetRecordList();
			NFIValueList propertyList = go.GetPropertyManager().GetPropertyList();
			
			int nAllElement = 1;
			for(int j = 0; j < recordLlist.Count(); j++)
			{
				string strRecordName = recordLlist.StringVal(j);
				if(strRecordName.Length > 0)
				{
					nAllElement++;
				}
			}
			for(int j = 0; j < propertyList.Count(); j++)
			{
				string strPropertyName = propertyList.StringVal(j);
				if(strPropertyName.Length > 0)
				{
					nAllElement++;
				}
			}
			//////////////////
            scrollPositionSecond = GUI.BeginScrollView(new Rect(nElementWidth / 2 + 20, nElementHeight, nElementWidth+20, nHeight/2), scrollPositionSecond, new Rect(0, 0, nElementWidth, (nAllElement+1) * (nElementHeight) + 1));

			int nElementIndex = 0;
			GUI.Button(new Rect(0, nElementIndex*nElementHeight, nElementWidth, nElementHeight), targetIdent.nData64.ToString());
			nElementIndex++;
			//all record
			for(int j = 0; j < recordLlist.Count(); j++)
			{
				string strRecordName = recordLlist.StringVal(j);
				if(strRecordName.Length > 0)
				{
					if(GUI.Button(new Rect(0, nElementIndex*nElementHeight, nElementWidth, nElementHeight), "++" + strRecordName))
					{
						strTableName = strRecordName;
					}
					
					nElementIndex++;
				}
				
			}
	
			
			///////////////////////////////
			//all property 


            for (int k = 0; k < propertyList.Count(); k++)
            {
                string strPropertyValue = null;
                string strPropertyName = propertyList.StringVal(k);
                NFIProperty property = go.GetPropertyManager().GetProperty(strPropertyName);
                NFIValueList.VARIANT_TYPE eType = property.GetType();
                switch (eType)
                {
                    case NFIValueList.VARIANT_TYPE.VTYPE_DOUBLE:
                        strPropertyValue = property.QueryDouble().ToString();
                        break;
                    case NFIValueList.VARIANT_TYPE.VTYPE_FLOAT:
                        strPropertyValue = property.QueryFloat().ToString();
                        break;
                    case NFIValueList.VARIANT_TYPE.VTYPE_INT:
                        strPropertyValue = property.QueryInt().ToString();
                        break;
                    case NFIValueList.VARIANT_TYPE.VTYPE_OBJECT:
                        strPropertyValue = property.QueryObject().nData64.ToString();
                        break;
                    case NFIValueList.VARIANT_TYPE.VTYPE_STRING:
                        strPropertyValue = property.QueryString();
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
			
			
			if(strTableName.Length > 0)
			{
				NFIRecord record = go.GetRecordManager().GetRecord(strTableName);
				if(null != record)
				{
					int nRow = record.GetRows();
					int nCol = record.GetCols();
					int nOffest = 30;

                    scrollPositionThird = GUI.BeginScrollView(new Rect(nElementWidth * 1.5f + 40, nElementHeight, nElementWidth * 2, nHeight/2), scrollPositionThird, new Rect(0, 0, nElementWidth * nCol + nOffest, nRow * nElementHeight + nOffest));
					
						
					string selString = null;
					
					
					for(int row = 0; row < nRow; row++)
					{
						GUI.Button(new Rect(0, row*nElementHeight+nOffest, nOffest, nElementHeight), row.ToString());//row
						for(int col = 0; col < nCol; col++)
						{
							if(0 == row)
							{
                                GUI.Button(new Rect(col * nElementWidth + nOffest, 0, nElementWidth, nElementHeight), col.ToString() + "  [" + record.GetColType(col) + "]");
							}
							
							if(record.IsUsed(row))
							{
								NFIValueList.VARIANT_TYPE eType = record.GetColType(col);
								switch(eType)
								{								
								case NFIValueList.VARIANT_TYPE.VTYPE_INT:
									selString = record.QueryInt(row, col).ToString();
									break;
									
								case NFIValueList.VARIANT_TYPE.VTYPE_FLOAT:
									selString = record.QueryFloat(row, col).ToString();
									break;
									
								case NFIValueList.VARIANT_TYPE.VTYPE_DOUBLE:
									selString = record.QueryDouble(row, col).ToString();
									break;
									
								case NFIValueList.VARIANT_TYPE.VTYPE_STRING:
									selString = record.QueryString(row, col).ToString();
									break;
									
								case NFIValueList.VARIANT_TYPE.VTYPE_OBJECT:
									selString = record.QueryObject(row, col).nData64.ToString();
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
							
							if(GUI.Button(new Rect(col*nElementWidth+nOffest, row*nElementHeight+nOffest, nElementWidth, nElementHeight), selString))
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
