/*
			This file is part of:
				NoahFrame
			https://github.com/ketoo/NoahGameFrame

   Copyright 2009 - 2019 NoahFrame(NoahGameFrame)

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
#ifndef NF_BLUE_PRINT_VIEW_H
#define NF_BLUE_PRINT_VIEW_H

#include "NFNodeView.h"
#include "NFTreeView.h"
#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"
#include "NFComm/NFPluginModule/NFIUIModule.h"
#include "NFComm/NFPluginModule/NFIBluePrintModule.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIElementModule.h"
#include "NFComm/NFPluginModule/NFIClassModule.h"

class NFBluePrintView : public NFIView
{
public:
	NFBluePrintView(NFIPluginManager* p, NFViewType vt);
	virtual ~NFBluePrintView();

	virtual bool Execute();

   	virtual void SubRender();

	void TryToCreateBluePrintBlock();
	void TryToCreateMonitor(NFMonitorType type);
	void TryToCreateJudgement();
	void TryToCreateExecuter();
	void TryToCreateVariable(NFVariableType type);

	void TryToCreateComparator();

	NFGUID GetCurrentObjectID();
	void SetCurrentObjectID(const NFGUID& id);

	void SetCurrentLogicBlockID(const NFGUID& id);

	NFTreeView* GetTreeView();
	NFNodeView* GetNodeView();
	
private:
	void HandlerSelected(const NFGUID& id);
	void NodeModifyEvent(const NFGUID& id, const bool create);
	void LinkModifyEvent(const NFGUID& startNode, const const NFGUID& endNode, const NFGUID& startPin, const const NFGUID& endPin, const bool create);

	bool TryNewLinkEvent(const NFGUID& startNode, const const NFGUID& endNode, const NFGUID& startPin, const const NFGUID& endPin);
	bool TryNewLinkEventForVariableToMonitor(NF_SHARE_PTR<NFBluePrintNodeBase> startNode, NF_SHARE_PTR<NFBluePrintNodeBase> endNode, const NFGUID& startPin, const const NFGUID& endPin);


	void NodeAttriRender(NFNodeAttri* nodeAttri);

	void NodeAttriRenderForVariable(NFNodeAttri* nodeAttri);
	void NodeAttriRenderForInputVariable(NFNodeAttri* nodeAttri);
	void NodeAttriRenderForElementVariable(NFNodeAttri* nodeAttri);
	void NodeAttriRenderForPropertyVariable(NFNodeAttri* nodeAttri);
	void NodeAttriRenderForRecordVariable(NFNodeAttri* nodeAttri);

	void NodeAttriRenderForMonitor(NFNodeAttri* nodeAttri);


	void CreateLogicBlockWindow();
	void CreateMonitor();
	void CreateJudgment();
	void CreateExecuter();
	void CreateVariable();

	void CreateComparator();
   

private:
	void AddMonitorNode(NF_SHARE_PTR<NFIMonitor> monitor);
	void AddVariableNode(NF_SHARE_PTR<NFIVariable> variable);

private:

	NFGUID mCurrentObjectID;
	NFGUID mCurrentLogicBlickID;

private:
	bool bCreatingLogicBlock = false;

	NFMonitorType monitorType;
	bool bCreatingMonitor = false;

	bool bCreatingJudgment = false;
	bool bCreatingExecuter = false;

	NFVariableType valueType;
	bool bCreatingVariable = false;

	bool bCreatingComparator = false;

	NFTreeView* m_pTreeView;
	NFNodeView* m_pNodeView;

private:
	NFIBluePrintModule* m_pBluePrintModule;
	NFIUIModule* m_pUIModule;
	NFIKernelModule* m_pKernelModule;
	NFIClassModule* m_pClassModule;
	NFIElementModule* m_pElementModule;
};

#endif