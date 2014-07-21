// -------------------------------------------------------------------------
//    @FileName      ：    NFMsgDefine.cpp
//    @Author      ：    LvSheng.Huang
//    @Date     ：    2010-10-5 12:05
//    @Module    ：    消息协议声明
//
// -------------------------------------------------------------------------

#include "NFMsgDefine.h"

//bool RecordToString( NFIRecord* pRecord, NFMsg::ObjectRecordBase& recordBase, E_CHECK_TYPE nCheckType, const bool bCheck )
//{
//	if (!pRecord)
//	{
//		return false;
//	}
//
//    // check if this record need to save/private/public
//    bool bCheckType = false;
//    switch (nCheckType)
//    {
//    case ECT_SAVE:
//        bCheckType = pRecord->GetSave();
//        break;
//    case ECT_PRIVATE:
//        bCheckType = pRecord->GetPrivate();
//        break;
//    case ECT_PUBLIC:
//        bCheckType = pRecord->GetPublic();
//        break;
//    default:
//        return true;
//    }
//
//    if (!bCheckType || bCheckType != bCheck)
//    {
//        return true;
//    }
//
//	recordBase.set_record_name( pRecord->GetName() );
//    for ( int i = 0; i < pRecord->GetRows(); i++ )
//	{
//		if ( pRecord->IsUsed( i ) )
//		{
//			for ( int j = 0; j < pRecord->GetCols(); j++ )
//			{
//				TDATA_TYPE varType = pRecord->GetColType( j );
//				switch ( varType )
//				{
//				case TDATA_INT:
//					{
//						NFMsg::RecordInt* recordInt = recordBase.add_record_int_list();
//						recordInt->set_row( i );
//						recordInt->set_col( j );
//						recordInt->set_data( pRecord->QueryInt( i, j ) );
//					}
//
//					break;
//				case TDATA_FLOAT:
//					{
//						NFMsg::RecordFloat* recordFloat = recordBase.add_record_float_list();
//						recordFloat->set_row( i );
//						recordFloat->set_col( j );
//						recordFloat->set_data( pRecord->QueryFloat( i, j ) );
//					}
//					break;
//				case TDATA_STRING:
//					{
//						NFMsg::RecordString* recordString = recordBase.add_record_string_list();
//						recordString->set_row( i );
//						recordString->set_col( j );
//						recordString->set_data( pRecord->QueryString( i, j ) );
//					}
//					break;
//				case TDATA_OBJECT:
//					{
//						NFMsg::RecordObject* recordObejct = recordBase.add_record_object_list();
//						recordObejct->set_row( i );
//						recordObejct->set_col( j );
//						recordObejct->set_data( pRecord->QueryObject( i, j ).nData64 );
//					}
//					break;
//				default:
//					break;
//				}
//			}
//		}
//	}
//
//	return true;
//}
//
//bool RecordToString( NFIRecordManager* pRecordManager, NFMsg::ObjectRecordList& recordList, E_CHECK_TYPE nCheckType, const bool bCheck )
//{
//	if (!pRecordManager)
//	{
//		return false;
//	}
//
//	NFIRecord* pRecord = pRecordManager->First();
//	while (pRecord)
//	{
//		NFMsg::ObjectRecordBase* pRecordBase = recordList.add_record_list();
//		RecordToString( pRecord, *pRecordBase, nCheckType, bCheck );
//
//		pRecord = pRecordManager->Next();
//	}
//
//	return true;
//}
//
//
//bool RecordFormString( NFIRecord* pRecord, const NFMsg::ObjectRecordBase& xRecordBase )
//{
//	if (!pRecord)
//	{
//		return false;
//	}
//
//	const std::string& strName = xRecordBase.record_name();
//
//	if (pRecord->GetName() != strName)
//	{
//		return false;
//	}
//
//	for ( int i = 0; i < xRecordBase.record_int_list_size(); i++ )
//	{
//		const NFMsg::RecordInt& recordInt = xRecordBase.record_int_list( i );
//		int nRow = recordInt.row();
//		int nCol = recordInt.col();
//		int nData = recordInt.data();
//		pRecord->SetUsed( nRow, 1 );
//		pRecord->SetInt( nRow, nCol, nData );
//	}
//
//	for ( int i = 0; i < xRecordBase.record_float_list_size(); i++ )
//	{
//		const NFMsg::RecordFloat& recordFloat = xRecordBase.record_float_list( i );
//		int nRow = recordFloat.row();
//		int nCol = recordFloat.col();
//		float fData = recordFloat.data();
//		pRecord->SetUsed( nRow, 1 );
//		pRecord->SetFloat( nRow, nCol, fData );
//	}
//
//	for ( int i = 0; i < xRecordBase.record_string_list_size(); i++ )
//	{
//		const NFMsg::RecordString& recordString = xRecordBase.record_string_list( i );
//		int nRow = recordString.row();
//		int nCol = recordString.col();
//		const std::string& strData = recordString.data();
//		pRecord->SetUsed( nRow, 1 );
//		pRecord->SetString( nRow, nCol, strData.c_str() );
//	}
//
//	for ( int i = 0; i < xRecordBase.record_object_list_size(); i++ )
//	{
//		const NFMsg::RecordObject& recordObject = xRecordBase.record_object_list( i );
//		int nRow = recordObject.row();
//		int nCol = recordObject.col();
//		int64_t nData = recordObject.data();
//		pRecord->SetUsed( nRow, 1 );
//		pRecord->SetObject( nRow, nCol, ( NFIDENTID )nData );
//	}
//	return true;
//
//}
//
//bool RecordFormString( NFIRecordManager* pRecordManager, const NFMsg::ObjectRecordList& recordList )
//{
//	if (!pRecordManager)
//	{
//		return false;
//	}
//
//	for (int i = 0; i < recordList.record_list_size(); ++i)
//	{
//		const NFMsg::ObjectRecordBase& recordBase = recordList.record_list(i);
//		const std::string& strName = recordBase.record_name();
//		NFIRecord* pRecord = pRecordManager->GetElement(strName);
//		if (pRecord)
//		{
//			RecordFormString(pRecord, recordBase);
//		}
//	}
//
//	return true;
//}
//
//bool PropertyToString( NFIPropertyManager* pProManager, NFMsg::ObjectPropertyList& propertyDB, E_CHECK_TYPE nCheckType, const bool bCheck )
//{
//	if (!pProManager)
//	{
//		return false;
//	}
//
//	//保存的数据才这样
//	NFIProperty* pProperty = pProManager->First();
//	while ( pProperty )
//	{
//        // check if this record need to save/private/public
//        bool bCheckType = false;
//        switch (nCheckType)
//        {
//        case ECT_SAVE:
//            bCheckType = pProperty->GetSave();
//            break;
//        case ECT_PRIVATE:
//            bCheckType = pProperty->GetPrivate();
//            break;
//        case ECT_PUBLIC:
//            bCheckType = pProperty->GetPublic();
//            break;
//        default:
//            break;
//        }
//
//        if (bCheckType && bCheckType == bCheck)
//        {
//            TDATA_TYPE varType = pProperty->GetType();
//            switch ( varType )
//            {
//            case TDATA_INT:
//                {
//                    NFMsg::PropertyInt* propertyInt = propertyDB.add_property_int_list();
//                    propertyInt->set_property_name( pProperty->GetKey() );
//                    propertyInt->set_data( pProperty->QueryInt() );
//                }
//
//                break;
//            case TDATA_FLOAT:
//                {
//                    NFMsg::PropertyFloat* propertyFloat = propertyDB.add_property_float_list();
//                    propertyFloat->set_property_name( pProperty->GetKey() );
//                    propertyFloat->set_data( pProperty->QueryFloat() );
//                }
//                break;
//            case TDATA_STRING:
//                {
//                    NFMsg::PropertyString* propertyString = propertyDB.add_property_string_list();
//                    propertyString->set_property_name( pProperty->GetKey() );
//                    propertyString->set_data( pProperty->QueryString() );
//                }
//                break;
//            case TDATA_OBJECT:
//                {
//                    NFMsg::PropertyObject* propertyObject = propertyDB.add_property_object_list();
//                    propertyObject->set_property_name( pProperty->GetKey() );
//                    propertyObject->set_data( pProperty->QueryObject().nData64 );
//                }
//                break;
//            default:
//                break;
//            }
//        }
//
//		pProperty = pProManager->Next();
//	}
//
//	return true;
//
//}
//
//bool PropertyFormString( NFIPropertyManager* pProManager, const NFMsg::ObjectPropertyList& propertyDB )
//{
//	for ( int i = 0; i < propertyDB.property_int_list_size(); i++ )
//	{
//		const NFMsg::PropertyInt& propertyInt = propertyDB.property_int_list( i );
//		const std::string& strName = propertyInt.property_name();
//		int nData = propertyInt.data();
//
//		NFIValueList::TData TData;
//		TData.nType = TDATA_TYPE::TDATA_INT;
//		TData.variantData = nData;
//		pProManager->SetProperty( strName, TData );
//	}
//
//	for ( int i = 0; i < propertyDB.property_float_list_size(); i++ )
//	{
//		const NFMsg::PropertyFloat& propertyFloat = propertyDB.property_float_list( i );
//		const std::string& strName = propertyFloat.property_name();
//		float fData = propertyFloat.data();
//
//		NFIValueList::TData TData;
//		TData.nType = TDATA_TYPE::TDATA_FLOAT;
//		TData.variantData = fData;
//		pProManager->SetProperty( strName, TData );
//	}
//
//	for ( int i = 0; i < propertyDB.property_string_list_size(); i++ )
//	{
//		const NFMsg::PropertyString& propertyString = propertyDB.property_string_list( i );
//		const std::string& strName = propertyString.property_name();
//		const std::string& strData = propertyString.data();
//
//		NFIValueList::TData TData;
//		TData.nType = TDATA_TYPE::TDATA_STRING;
//		TData.variantData = strData;
//		pProManager->SetProperty( strName, TData );
//	}
//
//	for ( int i = 0; i < propertyDB.property_object_list_size(); i++ )
//	{
//		const NFMsg::PropertyObject& propertyObject = propertyDB.property_object_list( i );
//		const std::string& strName = propertyObject.property_name();
//		int64_t nData = propertyObject.data();
//
//		NFIValueList::TData TData;
//		TData.nType = TDATA_TYPE::TDATA_OBJECT;
//		TData.variantData = nData;
//		pProManager->SetProperty( strName, TData );
//	}
//
//	return true;
//}
//
//bool PropertyListToString( NFList<NFIPropertyManager*>& pProManagerList, NFMsg::MultiObjectPropertyList& xPropertyDB, E_CHECK_TYPE nCheckType, const bool bCheck )
//{
//	if (pProManagerList.Count() <= 0)
//	{
//		return false;
//	}
//
//	NFIPropertyManager* pProManager = NULL;
//	bool bProRet = pProManagerList.First(pProManager);
//	while (bProRet && pProManager)
//	{
//		NFIDENTID ident = pProManager->Self();
//
//		NFMsg::ObjectPropertyList* pPropertyList = xPropertyDB.add_multi_player_property();
//		pPropertyList->set_player_id(ident.nData64);
//		PropertyToString(pProManager, *pPropertyList, nCheckType, bCheck);
//
//		bProRet = pProManagerList.Next(pProManager);
//	}
//
//	return true;
//}