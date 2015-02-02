using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace NFCoreEx
{
    public class NFCObject : NFIObject
    {
		public NFCObject(NFIDENTID self, int nContainerID, int nGroupID, string strClassName, string strConfigIndex)
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

        public override bool Init()
        {
			mRecordManager = new NFCRecordManager(mSelf);
			mPropertyManager = new NFCPropertyManager(mSelf);
			mHeartManager = new NFCHeartBeatManager(mSelf);
			mEventManager = new NFCEventManager(mSelf);

            return true;
        }

        public override bool Shut()
        {
			mRecordManager = null;
			mPropertyManager = null;
			mHeartManager = null;
            return true;
        }

        public override bool UpData(float fLastTime, float fAllTime)
        {
			//mHeartManager
            return true;
        }

        ///////////////////////////////////////////////////////////////////////
        public override NFIDENTID Self()
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

        // public override bool AddHeartBeat(  string strHeartBeatName, HEART_BEAT_FUNC cb,  NFIValueList& var,  float fTime,  int nCount );

        public override bool FindHeartBeat(string strHeartBeatName)
        {
            return true;
        }

        public override bool RemoveHeartBeat(string strHeartBeatName)
        {
            return true;
        }

        //////////////////////////////////////////////////
        // 
        //     public override bool AddRecordCallBack(  string strRecordName,  RECORD_EVENT_FUNC cb );
        // 
        //     public override bool AddPropertyCallBack(  string strCriticalName,  PROPERTY_EVENT_FUNC cb );

        /////////////////////////////////////////////////////////////////

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
                NFIValueList valueList = new NFCValueList();
                valueList.AddInt(0);
                property = mPropertyManager.AddProperty(strPropertyName, valueList);
            }

			property.SetInt(nValue);
			return true;
        }

        public override bool SetPropertyFloat(string strPropertyName, float fValue)
        {
			NFIProperty property = mPropertyManager.GetProperty(strPropertyName);
			if (null == property)
			{
                NFIValueList valueList = new NFCValueList();
                valueList.AddFloat(0.0f);
                property = mPropertyManager.AddProperty(strPropertyName, valueList);
            }
				
            property.SetFloat(fValue);
			return true;
        }

        public override bool SetPropertyDouble(string strPropertyName, double dwValue)
        {
			NFIProperty property = mPropertyManager.GetProperty(strPropertyName);
			if (null == property)
			{
                NFIValueList valueList = new NFCValueList();
                valueList.AddDouble(0);
                property = mPropertyManager.AddProperty(strPropertyName, valueList);
            }
				
            property.SetDouble(dwValue);
            return true;
        }

        public override bool SetPropertyString(string strPropertyName, string strValue)
        {
			NFIProperty property = mPropertyManager.GetProperty(strPropertyName);
			if (null == property)
			{
                NFIValueList valueList = new NFCValueList();
                valueList.AddString("");
                property = mPropertyManager.AddProperty(strPropertyName, valueList);
            }

			property.SetString(strValue);
			return true;
        }

        public override bool SetPropertyObject(string strPropertyName, NFIDENTID obj)
        {
			NFIProperty property = mPropertyManager.GetProperty(strPropertyName);
			if (null == property)
			{
                NFIValueList valueList = new NFCValueList();
                valueList.AddObject(new NFIDENTID());
                property = mPropertyManager.AddProperty(strPropertyName, valueList);
            }

			property.SetObject(obj);
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

        public override float QueryPropertyFloat(string strPropertyName)
        {
			NFIProperty property = mPropertyManager.GetProperty(strPropertyName);
			if (null != property)
			{
				return property.QueryFloat();
			}

            return 0.0f;
        }

        public override double QueryPropertyDouble(string strPropertyName)
        {
			NFIProperty property = mPropertyManager.GetProperty(strPropertyName);
			if (null != property)
			{
				return property.QueryDouble();
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

            return "";
        }

        public override NFIDENTID QueryPropertyObject(string strPropertyName)
        {
			NFIProperty property = mPropertyManager.GetProperty(strPropertyName);
			if (null != property)
			{
				return property.QueryObject();
			}

            return new NFIDENTID();
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

        public override bool SetRecordFloat(string strRecordName, int nRow, int nCol, float fValue)
        {
			NFIRecord record = mRecordManager.GetRecord(strRecordName);
			if (null != record)
			{
				record.SetFloat(nRow, nCol, fValue);
				return true;
			}

			return false;
        }

        public override bool SetRecordDouble(string strRecordName, int nRow, int nCol, double dwValue)
        {
			NFIRecord record = mRecordManager.GetRecord(strRecordName);
			if (null != record)
			{
				record.SetDouble(nRow, nCol, dwValue);
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

        public override bool SetRecordObject(string strRecordName, int nRow, int nCol, NFIDENTID obj)
        {
			NFIRecord record = mRecordManager.GetRecord(strRecordName);
			if (null != record)
			{
				record.SetObject(nRow, nCol, obj);
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

        public override float QueryRecordFloat(string strRecordName, int nRow, int nCol)
        {
			NFIRecord record = mRecordManager.GetRecord(strRecordName);
			if (null != record)
			{
				return record.QueryFloat(nRow, nCol);
			}

            return 0.0f;
        }

        public override double QueryRecordDouble(string strRecordName, int nRow, int nCol)
        {
			NFIRecord record = mRecordManager.GetRecord(strRecordName);
			if (null != record)
			{
				return record.QueryDouble(nRow, nCol);
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

            return "";
        }

        public override NFIDENTID QueryRecordObject(string strRecordName, int nRow, int nCol)
        {
			NFIRecord record = mRecordManager.GetRecord(strRecordName);
			if (null != record)
			{
				return record.QueryObject(nRow, nCol);
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

		public override NFIEventManager GetEventManager()
		{
			return mEventManager;
		}

		NFIDENTID mSelf;
		int mnContainerID;
		int mnGroupID;
		
		string mstrClassName;
		string mstrConfigIndex;

		NFIRecordManager mRecordManager;
		NFIPropertyManager mPropertyManager;
		NFIHeartBeatManager mHeartManager;
		NFIEventManager mEventManager;
	}
}