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

	void TryToCreateBranch(NFBranchType type);
	void TryToCreateArithmetic(NFArithmeticType type);
	void TryToCreateMonitor(NFMonitorType type);
	void TryToCreateExecutor(NFExecutorType type);
	void TryToCreateModifier(NFModifierType type);
	void TryToCreateVariable(NFVariableType type);
	void TryToCreateDebuger();

	NFGUID GetCurrentObjectID();
	void SetCurrentObjectID(const NFGUID& id);

	NFGUID GetCurrentLogicBlockID();
	void SetCurrentLogicBlockID(const NFGUID& id);

	const NFTreeView& GetTreeView();
	
private:
	void HandlerSelected(const NFGUID& id, const bool doubleClick);
	void NodeModifyEvent(const NFGUID& id, const bool create);
	void LinkModifyEvent(const NFGUID& id, const bool create);

	void AddNode(NF_SHARE_PTR<NFBluePrintNodeBase> node);

	bool TryNewLinkEvent(const NFGUID& startNode, const NFGUID& endNode, const NFGUID& startPin, const NFGUID& endPin);
	bool TryDeleteLinkEvent(const NFGUID& id);

	void PinRender(NFNodePin* pin);

	void NodeRenderBeforePinIn(NFNode* node);
	void NodeRenderAfterPinIn(NFNode* node);
	void NodeRenderBeforePinOut(NFNode* node);
	void NodeRenderAfterPinOut(NFNode* node);

	void PinRenderForVariable(NFNodePin* pin);
	void PinRenderForInputVariable(NFNodePin* pin);
	void PinRenderForElementVariable(NFNodePin* pin);
	void PinRenderForPropertyVariable(NFNodePin* pin);
	void PinRenderForRecordVariable(NFNodePin* pin);

	void PinRenderForMonitor(NFNodePin* pin);
	void PinRenderForGameEventMonitor(NFNodePin* pin);
	void PinRenderForNetworkEventMonitor(NFNodePin* pin);
	void PinRenderForNetworkMsgMonitor(NFNodePin* pin);
	void PinRenderForObjectEventMonitor(NFNodePin* pin);
	void PinRenderForPropertyEventMonitor(NFNodePin* pin);
	void PinRenderForRecordEventMonitor(NFNodePin* pin);
	void PinRenderForSceneEventMonitor(NFNodePin* pin);

	void PinRenderForBranch(NFNodePin* pin);

	void PinRenderForModifier(NFNodePin* pin);
	void PinRenderForPropertyModifier(NFNodePin* pin);
	void PinRenderForRecordModifier(NFNodePin* pin);
	void PinRenderForRecordAddModifier(NFNodePin* pin);
	void PinRenderForRecordRemModifier(NFNodePin* pin);

	void PinRenderForArithmetic(NFNodePin* pin);

	void PinRenderForDebugger(NFNodePin* pin);
	void PinRenderForExecutor(NFNodePin* pin);

	void CreateLogicBlock();
	void CreateMonitor();
	void CreateBranch();
	void CreateExecutor();
	void CreateModifier();
	void CreateVariable();
	void CreateArithmetic();
	void CreateDebugger();


private:
	NFColor GetBackGroundColor(NFBlueprintType type);
	std::string GetPinIcon(NFValueType type);
	NFPinShape GetPinShape(NFValueType type, NFIODataComFromType comeFromType);
	std::string GetNodeIcon(NFBlueprintType type);
	NFColor GetPinColor(NFValueType type);
	//NFColor GetLinkColor(NF_SHARE_PTR<NFBluePrintNodeBase> node);

private:

	NFGUID mCurrentObjectID;
	NFGUID mCurrentLogicBlockID;

private:
	bool bCreatingLogicBlock = false;

	NFArithmeticType arithmeticType;
	bool bCreatingArithmetic = false;

	NFMonitorType monitorType;
	bool bCreatingMonitor = false;

	NFBranchType branchType;
	bool bCreatingBranch = false;

	NFExecutorType executerType;
	bool bCreatingExecutor = false;

	NFModifierType modifierType;
	bool bCreatingModifier = false;

	NFVariableType valueType;
	bool bCreatingVariable = false;

	bool bCreatingDebuger = false;

	NFTreeView mTreeView;
	NFNodeView mNodeView;

private:
	NFIBluePrintModule* m_pBluePrintModule;
	NFIUIModule* m_pUIModule;
	NFIKernelModule* m_pKernelModule;
	NFIClassModule* m_pClassModule;
	NFILogModule* m_pLogModule;
	NFIElementModule* m_pElementModule;
};

#endif