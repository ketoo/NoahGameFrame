// -------------------------------------------------------------------------
//    文件名      :    NFCEventManager.cpp
//    创建者      :    LvSheng.Huang
//    创建时间    :    2012-07-31 22:14
//    功能描述    :    事件管理器
//
// -------------------------------------------------------------------------

#include "stdafx.h"
#include <algorithm>
#include "NFCEventManager.h"

/*
bool NFCEventManager::AddEvent( const NFIDENTID& objectID, const int nEventID )
{
    EVENT_MAP::iterator it = mtEventMap.find( objectID );
    if ( it == mtEventMap.end() )
    {
        EVENT_INFO_MAP* pEventInfoMap = new EVENT_INFO_MAP;
        mtEventMap.insert( EVENT_MAP::value_type( objectID, pEventInfoMap ) );

        return true;
    }

    return false;
}
*/

// bool NFCEventManager::AddEvent( const NFIDENTID& objectID, const int nEventID, const int nSamsaraTime, const float fProcessTime, const NFIValueList& valueList, const bool bSyschro )
// {
//
//  NFEventList info;
//  info.nEventID = nEventID;
//  info.nSamsaraTime = nSamsaraTime;
//  info.fProcessTime = fProcessTime;
//  info.varParamList = valueList;
//  info.self = objectID;
//
//  if( AddEvent( objectID, nEventID, info ) && bSyschro )
//  {
//      DoEvent( objectID, &info );
//  }
//
//  return true;
// }

// bool NFCEventManager::AddEvent( const NFIDENTID& objectID, const int nEventID, const NFEventList& info )
// {
//  NFCObjectEventInfo*pObjectEventInfo = GetElement( objectID );
//  if ( !pObjectEventInfo )
//  {
//      pObjectEventInfo = CreateElement( objectID );
//  }
//
//  NFEventList* pEventInfo = pObjectEventInfo->GetElement( nEventID );
//  if ( !pEventInfo )
//  {
//      pEventInfo = pObjectEventInfo->CreateElement( nEventID );
//  }
//
//  pEventInfo->nEventID = info.nEventID;
//  pEventInfo->nSamsaraTime = info.nSamsaraTime;
//  pEventInfo->fProcessTime = info.fProcessTime;
//  pEventInfo->varParamList = info.varParamList;
//  pEventInfo->self = info.self;
//
//     return true;
// }

bool NFCEventManager::AddEventCallBack(const NFIDENTID& objectID, const int nEventID, EVENT_PROCESS_FUNC cb)
{
    NFCObjectEventInfo* pObjectEventInfo = GetElement(objectID);
    if (!pObjectEventInfo)
    {
        pObjectEventInfo = CreateElement(objectID);
    }
    assert(NULL != pObjectEventInfo);

    NFEventList* pEventInfo = pObjectEventInfo->GetElement(nEventID);
    if (!pEventInfo)
    {
        pEventInfo = pObjectEventInfo->CreateElement(nEventID);
    }

    assert(NULL != pEventInfo);

    pEventInfo->Add(cb);

    return true;
}

bool NFCEventManager::Execute(const float fLasFrametime, const float fStartedTime)
{
    //隐形BUG,如果在触发器中执行添加删除事件，也许就会有问题，因为迭代器最大值不一样，而且可能是插座中间

    //  NFCObjectEventInfo*pObjectEventInfo = First();
    //  while ( pObjectEventInfo )
    //  {
    //      NFCValueList releaseList;
    //      NFEventList* pEventInfo = pObjectEventInfo->First();
    //      while ( pEventInfo )
    //      {
    //          if ( pEventInfo->Execute( fLasFrametime, fStartedTime ) )
    //          {
    //              //  time out and do event
    //              if ( DoEvent( pEventInfo->nEventID, pEventInfo ) )
    //              {
    //                  releaseList.AddPointer( pEventInfo );
    //              }
    //          }
    //
    //          pEventInfo = pObjectEventInfo->Next();
    //      }
    //
    //      int nCount = releaseList.GetCount();
    //      for ( int i = 0; i < nCount; i++ )
    //      {
    //          pObjectEventInfo->DeleteElement( pEventInfo->nEventID );
    //      }
    //
    //      pObjectEventInfo = Next();
    //  }


    //     EVENT_MAP::iterator it = mtEventMap.begin();
    //     for ( it; it != mtEventMap.end(); it++ )
    //     {
    //         EVENT_INFO_MAP* pInfoMap = it->second;
    //         EVENT_INFO_MAP::iterator itrInfo = pInfoMap->begin();
    //         for ( itrInfo; itrInfo != pInfoMap->end(); )
    //         {
    //             if ( 0 != itrInfo->second.nSamsaraTime )
    //             {
    //                 if ( itrInfo->second.fLapseTime < 0.1 )
    //                 {
    //                     if ( itrInfo->second.nSamsaraTime > 0 )
    //                     {
    //                         itrInfo->second.nSamsaraTime--;
    //                     }
    //
    //                     itrInfo->second.fLapseTime = itrInfo->second.fProcessTime;
    //
    //                     DoEvent( it->first, itrInfo->second );
    //                 }
    //                 else
    //                 {
    //                     itrInfo->second.fLapseTime -= fLastTime;
    //                 }
    //
    //                 itrInfo++;
    //             }
    //             else
    //             {
    //                 itrInfo = pInfoMap->erase( itrInfo );
    //             }
    //         }
    //     }

    return true;
}

// bool NFCEventManager::RemoveEvent( const NFIDENTID& objectID, const int nEventID )
// {
//  NFCObjectEventInfo*pObjectEventInfo = GetElement( objectID );
//  if ( pObjectEventInfo )
//  {
//      NFEventList* pEventInfo = pObjectEventInfo->GetElement( nEventID );
//      if ( pEventInfo )
//      {
//          return DeleteElement( objectID );
//      }
//  }
//
//     return false;
// }

bool NFCEventManager::RemoveEvent(const NFIDENTID& objectID)
{
    return DeleteElement(objectID);
}

bool NFCEventManager::RemoveEventCallBack(const NFIDENTID& objectID, const int nEventID, EVENT_PROCESS_FUNC cb)
{
    NFCObjectEventInfo* pObjectEventInfo = GetElement(objectID);
    if (pObjectEventInfo)
    {
        NFEventList* pEventInfo = pObjectEventInfo->GetElement(nEventID);
        if (pEventInfo)
        {
            return pEventInfo->Remove(cb);
        }
    }

    return false;
}

// bool NFCEventManager::DoEvent( const NFIDENTID& objectID, NFEventList* info )
// {
//  NFCObjectEventInfo*pObjectEventInfo = GetElement( objectID );
//  if ( pObjectEventInfo )
//  {
//      if ( info )
//      {
//          EVENT_PROCESS_FUNC cb = NULL;
//          bool bRet = info->First( cb );
//          while ( bRet )
//          {
//              cb( objectID, info->nEventID,  info->varParamList );
//
//              bRet = info->Next( cb );
//          }
//
//          if ( 0 == info->nSamsaraTime )
//          {
//              return true;
//          }
//      }
//  }
//
//     return false;
// }

bool NFCEventManager::DoEvent(const NFIDENTID& objectID, const int nEventID, const NFIValueList& valueList)
{
    NFCObjectEventInfo* pObjectEventInfo = GetElement(objectID);
    if (!pObjectEventInfo)
    {
        pObjectEventInfo = CreateElement(objectID);
    }

    if (pObjectEventInfo)
    {
        NFEventList* pEventInfo = pObjectEventInfo->GetElement(nEventID);
        if (pEventInfo)
        {
            EVENT_PROCESS_FUNC cb = NULL;
            bool bRet = pEventInfo->First(cb);
            while (bRet)
            {
                cb(objectID, nEventID,  valueList);

                bRet = pEventInfo->Next(cb);
            }
        }
    }

    //////////////////////////////////////////////////////////////////////////
    pObjectEventInfo = GetElement(0);
    if (pObjectEventInfo)
    {
        NFEventList* pEventInfo = pObjectEventInfo->GetElement(nEventID);
        if (pEventInfo)
        {
            EVENT_PROCESS_FUNC cb = NULL;
            bool bRet = pEventInfo->First(cb);
            while (bRet)
            {
                cb(objectID, nEventID,  valueList);

                bRet = pEventInfo->Next(cb);
            }
        }
    }
    return true;
}

bool NFCEventManager::DoEvent(NFIDENTID& objectID, const std::string& strClassName, const int nClassEvent, const NFIValueList& valueList)
{
    NFEventList* pEventList = mClassEventInfo.GetElement(strClassName);
    if (pEventList)
    {

        CLASS_EVENT_FUNC cb = NULL;
        bool bRet = pEventList->First(cb);
        while (bRet)
        {
            cb(objectID, nClassEvent,  valueList);

            bRet = pEventList->Next(cb);
        }
    }

    return false;
}

// bool NFCEventManager::HasEvent( const NFIDENTID& objectID, const int nEventID )
// {
// //     EVENT_MAP::iterator it = mtEventMap.find( objectID );
// //     if ( it != mtEventMap.end() )
// //     {
// //         EVENT_INFO_MAP* pInfoMap = it->second;
// //
// //         EVENT_INFO_MAP::iterator itrInfo = pInfoMap->find( nEventID );
// //         if ( itrInfo != pInfoMap->end() )
// //         {
// //             return true;
// //         }
// //     }
//  NFCObjectEventInfo*pObjectEventInfo = GetElement( objectID );
//  if ( pObjectEventInfo )
//  {
//      NFEventList* pEventInfo = pObjectEventInfo->GetElement( nEventID );
//      if ( pEventInfo )
//      {
//          return true;
//      }
//  }
//     return false;
// }

bool NFCEventManager::HasEventCallBack(const NFIDENTID& objectID, const int nEventID, EVENT_PROCESS_FUNC cb)
{
    NFCObjectEventInfo* pObjectEventInfo = GetElement(objectID);
    if (pObjectEventInfo)
    {
        NFEventList* pEventInfo = pObjectEventInfo->GetElement(nEventID);
        if (pEventInfo)
        {
            return pEventInfo->Find(cb);
        }
    }
    return false;
}

bool NFCEventManager::AddClassCallBack(const std::string& strClassName, CLASS_EVENT_FUNC cb)
{
    NFEventList* pEventList = mClassEventInfo.GetElement(strClassName);
    if (!pEventList)
    {
        pEventList = mClassEventInfo.CreateElement(strClassName);
    }

    assert(NULL != pEventList);

    pEventList->Add(cb);

    return true;
}

bool NFCEventManager::RemoveClassCallBack(const std::string& strClassName, CLASS_EVENT_FUNC cb)
{
    NFEventList* pEventList = mClassEventInfo.GetElement(strClassName);
    if (pEventList)
    {
        return pEventList->Remove(cb);
    }

    return false;
}
