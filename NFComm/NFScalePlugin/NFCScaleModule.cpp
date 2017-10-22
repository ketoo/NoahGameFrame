// -------------------------------------------------------------------------
//    @FileName			:    NFCScaleModule.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2017-10-23 12:22
//    @Module           :    NFCScaleModule
//    @Desc             :
// -------------------------------------------------------------------------

#include "NFCScaleModule.h"

bool NFCScaleModule::Init()
{
	m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
	m_pClassModule = pPluginManager->FindModule<NFIClassModule>();
	m_pLogModule = pPluginManager->FindModule<NFILogModule>();
	m_pElementModule = pPluginManager->FindModule<NFIElementModule>();
	
	return true;
}

bool NFCScaleModule::AfterInit()
{


	return true;
}

bool NFCScaleModule::Shut()
{

	return true;
}

bool NFCScaleModule::Execute()
{
	return true;
}
