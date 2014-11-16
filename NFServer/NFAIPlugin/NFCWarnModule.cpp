// -------------------------------------------------------------------------
//    文件名      ：    NFCWarnModule.cpp
//    创建者      ：    LvSheng.Huang
//    创建时间    ：    2012-07-14 08:50
//    功能描述    ：    警戒对象管理模块
//
// -------------------------------------------------------------------------

////#include "stdafx.h"
#include "NFCWarnModule.h"
#include "NFCAIModule.h"

NFCWarnModule::NFCWarnModule()
{

}

NFCWarnModule::~NFCWarnModule()
{
    ClearWarnAll();
}

bool NFCWarnModule::CheckWarnObject(const NFIDENTID& self)
{
    //  static NFIAIModule* pControl = NFCAIModule::Intance();
    //  static NFIStateMachine* pMachine = pControl->GetStateMachine( self );
    //  static NFIKnowLedge* pKnowLedge = pControl->GetKnowLedge();
    //  static NFIHateModule* pHateModule = pControl->GetHateModule();
    //
    //  if ( pHateModule->Empty( self ) )
    //  {
    //      return false;
    //  }
    //
    //  //找到距离最近的开火?(战斗中是不会检查的，因为那时候有仇恨列表)
    //  TOBJECTWARN::iterator it = mtObjectWarn.find( self );
    //  if ( it != mtObjectWarn.end() )
    //  {
    //      TWARNLIST* pWarnList = it->second;
    //      TWARNLIST::iterator itWarn = pWarnList->begin();
    //      for ( itWarn; itWarn != pWarnList->end(); itWarn++ )
    //      {
    //          if ( pKnowLedge->CanUseAnySkill( self, *itWarn ) )
    //          {
    //              return true;
    //          }
    //      }
    //  }

    return false;
}

void NFCWarnModule::OnSpring(const NFIDENTID& self, const NFIDENTID& other)
{
    AddWarn(self, other);
}

void NFCWarnModule::OnEndSpring(const NFIDENTID& self, const NFIDENTID& other)
{
    ClearWarn(self, other);
}

void NFCWarnModule::OnBeKilled(const NFIDENTID& self, const NFIDENTID& other)
{
    ClearWarnAll(self);
    ClearWarn(other, self);
}

bool NFCWarnModule::OnCreateAIObject(const NFIDENTID& self)
{
    //     if ( !Exist( self ) )
    //     {
    //         mtObjectWarn.insert( TOBJECTWARN::value_type( self, new TWARNLIST ) );
    //     }

    return true;
}

bool NFCWarnModule::OnDelAIObject(const NFIDENTID& self)
{
    ClearWarnAll(self);

    return true;
}

bool NFCWarnModule::Exist(const NFIDENTID& self)
{
    //     TOBJECTWARN::iterator it = mtObjectWarn.find( self );
    //     if ( it != mtObjectWarn.end() )
    //     {
    //         return true;
    //     }

    return false;
}

bool NFCWarnModule::ClearWarn(const NFIDENTID& self, const NFIDENTID& other)
{
    //     TOBJECTWARN::iterator it = mtObjectWarn.find( self );
    //     if ( it != mtObjectWarn.end() )
    //     {
    //         TWARNLIST* pWarnList = it->second;
    //         TWARNLIST::iterator itWarn = std::find( pWarnList->begin(), pWarnList->end(), other );
    //         if ( itWarn != pWarnList->end() )
    //         {
    //             pWarnList->erase( itWarn );
    //         }
    //     }

    return true;
}

bool NFCWarnModule::ClearWarnAll(const NFIDENTID& self)
{
    //     TOBJECTWARN::iterator it = mtObjectWarn.find( self );
    //     if ( it != mtObjectWarn.end() )
    //     {
    //         TWARNLIST* pWarnList = it->second;
    //         pWarnList->clear();
    //         delete pWarnList;
    //         mtObjectWarn.erase( it );
    //     }

    return true;
}

bool NFCWarnModule::ClearWarnAll()
{
    //     TOBJECTWARN::iterator it = mtObjectWarn.begin();
    //     for ( it; it != mtObjectWarn.end(); /*it++ */ )
    //     {
    //         ClearWarnAll( it->first );
    //     }

    return true;
}

bool NFCWarnModule::AddWarn(const NFIDENTID& self, const NFIDENTID& other)
{
    //     TOBJECTWARN::iterator it = mtObjectWarn.find( self );
    //     if ( it != mtObjectWarn.end() )
    //     {
    //         TWARNLIST* pWarnList = it->second;
    //         TWARNLIST::iterator itWarn = std::find( pWarnList->begin(), pWarnList->end(), other );
    //         if ( itWarn == pWarnList->end() )
    //         {
    //             pWarnList->push_back( other );
    //         }
    //     }

    return true;
}

bool NFCWarnModule::Empty(const NFIDENTID& self)
{
    //     TOBJECTWARN::iterator it = mtObjectWarn.find( self );
    //     if ( it != mtObjectWarn.end() )
    //     {
    //         return it->second->empty();
    //     }

    return true;
}