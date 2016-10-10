//-----------------------------------------------------------------------
// <copyright file="NFCObject.cs">
//     Copyright (C) 2015-2015 lvsheng.huang <https://github.com/ketoo/NFrame>
// </copyright>
//-----------------------------------------------------------------------
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace NFrame
{
    public class NFCObject : NFIObject
    {
		public NFCObject(NFGUID self, int nContainerID, int nGroupID, string strClassName, string strConfigIndex)
		{
			mSelf = self;
            mstrClassName = strClassName;
            mstrConfigIndex = strConfigIndex;
			mnContainerID = nContainerID;
			mnGroupID = nGroupID;
			Init();
		}

		~NFCObject()
		{
			Shut();
		}

        public override void Init()
        {
			mRecordManager = new NFCRecordManager(mSelf);
			mPropertyManager = new NFCPropertyManager(mSelf);
			mHeartManager = new NFCHeartBeatManager(mSelf);

            return;
        }

        public override void Shut()
        {
            NFIDataList xRecordList = mRecordManager.GetRecordList();
            if (null != xRecordList)
            {
                for(int i = 0; i < xRecordList.Count(); ++i)
                {
                    string strRecordName = xRecordList.StringVal(i);
                    NFIRecord xRecord = mRecordManager.GetRecord(strRecordName);
                    if (null !=  xRecord)
                    {
                        xRecord.Clear();
                    }
                }
            }

			mRecordManager = null;
			mPropertyManager = null;
            mHeartManager = null;

            return;
        }

        public override void AddHeartBeat(string strHeartBeatName, float fTime, int nCount, NFIHeartBeat.HeartBeatEventHandler handler)
        {
            GetHeartBeatManager().AddHeartBeat(strHeartBeatName, fTime, nCount, handler);
        }

        public override bool FindHeartBeat(string strHeartBeatName)
        {
            return GetHeartBeatManager().FindHeartBeat(strHeartBeatName);
        }

        public override void RemoveHeartBeat(string strHeartBeatName)
        {
            GetHeartBeatManager().RemoveHeartBeat(strHeartBeatName);
        }

        public override bool UpData(float fLastTime, float fAllTime)
        {
            GetHeartBeatManager().Update(fLastTime);

            return true;
        }

        ///////////////////////////////////////////////////////////////////////
        public override NFGUID Self()
        {
			return mSelf;
        }
		
		public override int ContainerID()
        {
			return mnContainerID;
        }
		
		public override int GroupID()
        {
			return mnGroupID;
        }
		
        public override string ClassName()
        {
            return mstrClassName;
        }

        public override string ConfigIndex()
        {
            return mstrConfigIndex;
        }

        public override bool FindProperty(string strPropertyName)
        {
			if (null != mPropertyManager.GetProperty(strPropertyName))
			{
				return true;
			}

            return false;
        }

        public override bool SetPropertyInt(string strPropertyName, Int64 nValue)
        {
			NFIProperty property = mPropertyManager.GetProperty(strPropertyName);
			if (null == property)
			{
                NFIDataList valueList = new NFCDataList();
                valueList.AddInt(0);
                property = mPropertyManager.AddProperty(strPropertyName, valueList);
            }

			property.SetInt(nValue);
			return true;
        }

        public override bool SetPropertyFloat(string strPropertyName, double fValue)
        {
			NFIProperty property = mPropertyManager.GetProperty(strPropertyName);
			if (null == property)
			{
                NFIDataList valueList = new NFCDataList();
                valueList.AddFloat(0.0f);
                property = mPropertyManager.AddProperty(strPropertyName, valueList);
            }
				
            property.SetFloat(fValue);
			return true;
        }

        public override bool SetPropertyString(string strPropertyName, string strValue)
        {
			NFIProperty property = mPropertyManager.GetProperty(strPropertyName);
			if (null == property)
			{
                NFIDataList valueList = new NFCDataList();
                valueList.AddString("");
                property = mPropertyManager.AddProperty(strPropertyName, valueList);
            }

			property.SetString(strValue);
			return true;
        }

        public override bool SetPropertyObject(string strPropertyName, NFGUID obj)
        {
			NFIProperty property = mPropertyManager.GetProperty(strPropertyName);
			if (null == property)
			{
                NFIDataList valueList = new NFCDataList();
                valueList.AddObject(new NFGUID());
                property = mPropertyManager.AddProperty(strPropertyName, valueList);
            }

			property.SetObject(obj);
			return true;

        }

        public override bool SetPropertyVector2(string strPropertyName, NFVector2 obj)
        {
            NFIProperty property = mPropertyManager.GetProperty(strPropertyName);
            if (null == property)
            {
                NFIDataList valueList = new NFCDataList();
                valueList.AddVector2(new NFVector2());
                property = mPropertyManager.AddProperty(strPropertyName, valueList);
            }

            property.SetVector2(obj);
            return true;

        }

        public override bool SetPropertyVector3(string strPropertyName, NFVector3 obj)
        {
            NFIProperty property = mPropertyManager.GetProperty(strPropertyName);
            if (null == property)
            {
                NFIDataList valueList = new NFCDataList();
                valueList.AddVector3(new NFVector3());
                property = mPropertyManager.AddProperty(strPropertyName, valueList);
            }

            property.SetVector3(obj);
            return true;

        }

        public override Int64 QueryPropertyInt(string strPropertyName)
        {
			NFIProperty property = mPropertyManager.GetProperty(strPropertyName);
			if (null != property)
			{
				return property.QueryInt();
			}

            return 0;
        }

        public override double QueryPropertyFloat(string strPropertyName)
        {
			NFIProperty property = mPropertyManager.GetProperty(strPropertyName);
			if (null != property)
			{
				return property.QueryFloat();
			}

            return 0.0;
        }

        public override string QueryPropertyString(string strPropertyName)
        {
			NFIProperty property = mPropertyManager.GetProperty(strPropertyName);
			if (null != property)
			{
				return property.QueryString();
			}

            return NFIDataList.NULL_STRING;
        }

        public override NFGUID QueryPropertyObject(string strPropertyName)
        {
			NFIProperty property = mPropertyManager.GetProperty(strPropertyName);
			if (null != property)
			{
				return property.QueryObject();
			}

            return NFIDataList.NULL_OBJECT;
        }

        public override NFVector2 QueryPropertyVector2(string strPropertyName)
        {
            NFIProperty property = mPropertyManager.GetProperty(strPropertyName);
            if (null != property)
            {
                return property.QueryVector2();
            }

            return NFIDataList.NULL_VECTOR2;
        }

        public override NFVector3 QueryPropertyVector3(string strPropertyName)
        {
            NFIProperty property = mPropertyManager.GetProperty(strPropertyName);
            if (null != property)
            {
                return property.QueryVector3();
            }

            return NFIDataList.NULL_VECTOR3;
        }

        public override bool FindRecord(string strRecordName)
        {
			NFIRecord record = mRecordManager.GetRecord(strRecordName);
			if (null != record)
			{
				return true;
			}
            return false;
        }

        public override bool SetRecordInt(string strRecordName, int nRow, int nCol, Int64 nValue)
        {
			NFIRecord record = mRecordManager.GetRecord(strRecordName);
			if (null != record)
			{
				record.SetInt(nRow, nCol, nValue);
				return true;
			}

			return false;
        }

        public override bool SetRecordFloat(string strRecordName, int nRow, int nCol, double fValue)
        {
			NFIRecord record = mRecordManager.GetRecord(strRecordName);
			if (null != record)
			{
				record.SetFloat(nRow, nCol, fValue);
				return true;
			}

			return false;
        }

        public override bool SetRecordString(string strRecordName, int nRow, int nCol, string strValue)
        {
			NFIRecord record = mRecordManager.GetRecord(strRecordName);
			if (null != record)
			{
				record.SetString(nRow, nCol, strValue);
				return true;
			}

			return false;
        }

        public override bool SetRecordObject(string strRecordName, int nRow, int nCol, NFGUID obj)
        {
			NFIRecord record = mRecordManager.GetRecord(strRecordName);
			if (null != record)
			{
				record.SetObject(nRow, nCol, obj);
				return true;
			}

			return false;
        }

        public override bool SetRecordVector2(string strRecordName, int nRow, int nCol, NFVector2 obj)
        {
            NFIRecord record = mRecordManager.GetRecord(strRecordName);
            if (null != record)
            {
                record.SetVector2(nRow, nCol, obj);
                return true;
            }

            return false;
        }

        public override bool SetRecordVector3(string strRecordName, int nRow, int nCol, NFVector3 obj)
        {
            NFIRecord record = mRecordManager.GetRecord(strRecordName);
            if (null != record)
            {
                record.SetVector3(nRow, nCol, obj);
                return true;
            }

            return false;
        }

        public override Int64 QueryRecordInt(string strRecordName, int nRow, int nCol)
        {
			NFIRecord record = mRecordManager.GetRecord(strRecordName);
			if (null != record)
			{
				return record.QueryInt(nRow, nCol);
			}

            return 0;
        }

        public override double QueryRecordFloat(string strRecordName, int nRow, int nCol)
        {
			NFIRecord record = mRecordManager.GetRecord(strRecordName);
			if (null != record)
			{
				return record.QueryFloat(nRow, nCol);
			}

            return 0.0;
        }

        public override string QueryRecordString(string strRecordName, int nRow, int nCol)
        {
			NFIRecord record = mRecordManager.GetRecord(strRecordName);
			if (null != record)
			{
				return record.QueryString(nRow, nCol);
			}

            return NFIDataList.NULL_STRING;
        }

        public override NFGUID QueryRecordObject(string strRecordName, int nRow, int nCol)
        {
			NFIRecord record = mRecordManager.GetRecord(strRecordName);
			if (null != record)
			{
				return record.QueryObject(nRow, nCol);
			}

            return null;
        }

        public override NFVector2 QueryRecordVector2(string strRecordName, int nRow, int nCol)
        {
            NFIRecord record = mRecordManager.GetRecord(strRecordName);
            if (null != record)
            {
                return record.QueryVector2(nRow, nCol);
            }

            return null;
        }

        public override NFVector3 QueryRecordVector3(string strRecordName, int nRow, int nCol)
        {
            NFIRecord record = mRecordManager.GetRecord(strRecordName);
            if (null != record)
            {
                return record.QueryVector3(nRow, nCol);
            }

            return null;
        }

        public override NFIRecordManager GetRecordManager()
        {
			return mRecordManager;
        }

        public override NFIHeartBeatManager GetHeartBeatManager()
        {
			return mHeartManager;
        }

        public override NFIPropertyManager GetPropertyManager()
        {
			return mPropertyManager;
        }

		NFGUID mSelf;
		int mnContainerID;
		int mnGroupID;
		
		string mstrClassName;
		string mstrConfigIndex;

		NFIRecordManager mRecordManager;
		NFIPropertyManager mPropertyManager;
		NFIHeartBeatManager mHeartManager;
	}
}