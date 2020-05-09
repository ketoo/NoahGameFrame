/*
			This file is part of:
				NoahFrame
			https://github.com/ketoo/NoahGameFrame

   Copyright 2009 - 2020 NoahFrame(NoahGameFrame)

   File creator: lvsheng.huang

   NoahFrame is open-source software and you can redistribute it and/or modify
   it under the terms of the License; besides, anyone who use this file/software must include this copyright announcement.

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

	   http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/
#ifndef NF_HIERACHY_VIEW_H
#define NF_HIERACHY_VIEW_H

#include "NFComm/NFPluginModule/NFIUIModule.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIBluePrintModule.h"
#include "NFComm/NFPluginModule/NFIClassModule.h"
#include "NFComm/NFPluginModule/NFIElementModule.h"
#include "NFComm/NFPluginModule/NFIEventModule.h"
#include "NFComm/NFPluginModule/NFIBPVirtualMachineModule.h"

class NFHierachyView : public NFIView
{
public:
	NFHierachyView(NFIPluginManager* p, NFViewType vt);
	virtual bool Execute();

   virtual void SubRender();

private:
   void GodViewSubRender();
   void GameViewSubRender();
   void ProjectViewSubRender();
   void BluePrintViewSubRender();

   void BluePrintViewSubRenderForLogicBlock();
   void BluePrintViewSubRenderForMonitor();
   void BluePrintViewSubRenderForBranch();
   void BluePrintViewSubRenderForExecutor();
   void BluePrintViewSubRenderForVariable();
   void BluePrintViewSubRenderForModifier();
   void BluePrintViewSubRenderForLogger();
   void BluePrintViewSubRenderForArithmetic();

   void BluePrintViewSubRenderForMonitorHead(NF_SHARE_PTR<NFIMonitor> monitor);
   void BluePrintViewSubRenderForMonitorBody(NF_SHARE_PTR<NFIMonitor> monitor);
   void BluePrintViewSubRenderForMonitorBot(NF_SHARE_PTR<NFIMonitor> monitor);

   void BluePrintViewSubRenderForBranchHead(NF_SHARE_PTR<NFIBranch> branch);
   void BluePrintViewSubRenderForBranchBody(NF_SHARE_PTR<NFIBranch> branch);
   void BluePrintViewSubRenderForBranchBot(NF_SHARE_PTR<NFIBranch> branch);

   void InitBluePrintMonitorArgs(NF_SHARE_PTR<NFIMonitor> monitor);
   void InitBluePrintJudgementArgs(NF_SHARE_PTR<NFIBranch> branch);
   void InitBluePrintExecutorArgs(NF_SHARE_PTR<NFIExecutor> executer);

   void RenderForModifyProperty();

private:
   std::string modifyPropertyName;
   char modifyPropertyValue[128] = "";

private:
	NFIClassModule* m_pClassModule;
	NFIElementModule* m_pElementModule;
	NFIKernelModule* m_pKernelModule;
    NFIEventModule* m_pEventModule;
    NFIBluePrintModule* m_pBluePrintModule;
    NFIBPVirtualMachineModule* m_pBPVirtualMachineModule;
};

#endif