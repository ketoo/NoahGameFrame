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

#include "NFBluePrintView.h"
#include "NFUIModule.h"

NFBluePrintView::NFBluePrintView(NFIPluginManager* p, NFViewType vt) : NFIView(p, vt, GET_CLASS_NAME(NFBluePrintView))
{
   m_pNodeView = NF_NEW NFNodeView(p);
   m_pTreeView = NF_NEW NFTreeView(p);

   m_pNodeView->ResetOffest(NFVector2::Zero());
   m_pNodeView->SetUpNewLinkCallBack(std::bind(&NFBluePrintView::TryNewLinkEvent, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));
   m_pNodeView->SetUpNodeAttriRenderCallBack(std::bind(&NFBluePrintView::NodeAttriRender, this, std::placeholders::_1));

   m_pTreeView->SetSelectedNodeFunctor(std::bind(&NFBluePrintView::HandlerSelected, this, std::placeholders::_1));
   m_pTreeView->SetName(GET_CLASS_NAME(NFBluePrintView));

   m_pUIModule = pPluginManager->FindModule<NFIUIModule>();
   m_pBluePrintModule = pPluginManager->FindModule<NFIBluePrintModule>();
   m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
   m_pClassModule = pPluginManager->FindModule<NFIClassModule>();
   m_pElementModule = pPluginManager->FindModule<NFIElementModule>();

   m_pBluePrintModule->SetNodeModifyEventFunctor(std::bind(&NFBluePrintView::NodeModifyEvent, this, std::placeholders::_1, std::placeholders::_2));
   m_pBluePrintModule->SetLinkModifyEventFunctor(std::bind(&NFBluePrintView::LinkModifyEvent, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5));
}

NFBluePrintView::~NFBluePrintView()
{
   delete m_pNodeView;
   m_pNodeView = nullptr;

   delete m_pTreeView;
   m_pTreeView = nullptr;
}

bool NFBluePrintView::Execute()
{
   ImGui::SameLine();
   if (ImGui::Button("- nodes"))
   {
   }

   ImGui::SameLine();
   if (ImGui::Button("- links"))
   {
   }

	ImGui::SameLine();
	if (ImGui::Button("return to center"))
	{
		m_pNodeView->ResetOffest(NFVector2::Zero());
	}

   m_pNodeView->Execute();
   
   CreateLogicBlockWindow();
   CreateMonitor();
   CreateJudgment();
   CreateExecuter();
   CreateVariable();

   CreateComparator();

   if (ImGui::IsWindowFocused())
   {
      NF_SHARE_PTR<NFIView> pView = m_pUIModule->GetView(NFViewType::HierachyView);
      if (pView)
      {
         pView->OccupySubRender(this);
      }
   }

   ImGuiIO& io = ImGui::GetIO();
   if (io.KeyCtrl)
   {

   }

	return false;
}

void AddJudgementNode(NFTreeView* pTreeView, const NFGUID& parentID, NF_SHARE_PTR<NFJudgement> judgement);

void AddExecuterNode(NFTreeView* pTreeView, const NFGUID& id, NF_SHARE_PTR<NFExecuter> executer)
{
	pTreeView->AddSubTreeNode(id, executer->id, executer->name);
	if (executer->nextExecuter)
	{
		AddExecuterNode(pTreeView, executer->id, executer->nextExecuter);
	}
	
	for (auto it : executer->judgements)
	{
		AddJudgementNode(pTreeView, executer->id, it);
	}
}

void AddJudgementNode(NFTreeView* pTreeView, const NFGUID& parentID, NF_SHARE_PTR<NFJudgement> judgement)
{
	pTreeView->AddSubTreeNode(parentID, judgement->id, judgement->name);

	if (judgement->trueBlueprintNode)
	{
		if (judgement->trueBlueprintNode->blueprintType == NFBlueprintType::JUDGEMENT)
		{
			AddJudgementNode(pTreeView, judgement->id, std::dynamic_pointer_cast<NFJudgement>(judgement->trueBlueprintNode));
		}
		else if (judgement->trueBlueprintNode->blueprintType == NFBlueprintType::EXECUTER)
		{
			AddExecuterNode(pTreeView, judgement->id, std::dynamic_pointer_cast<NFExecuter>(judgement->trueBlueprintNode));
		}
	}

	if (judgement->falseBlueprintNode)
	{
		if (judgement->falseBlueprintNode->blueprintType == NFBlueprintType::JUDGEMENT)
		{
			AddJudgementNode(pTreeView, judgement->id, std::dynamic_pointer_cast<NFJudgement>(judgement->falseBlueprintNode));
		}
		else if (judgement->falseBlueprintNode->blueprintType == NFBlueprintType::EXECUTER)
		{
			AddExecuterNode(pTreeView, judgement->id, std::dynamic_pointer_cast<NFExecuter>(judgement->falseBlueprintNode));
		}
	}
}

NFGUID NFBluePrintView::GetCurrentObjectID()
{
	return mCurrentObjectID;
}

void NFBluePrintView::SetCurrentObjectID(const NFGUID& id)
{
	mCurrentObjectID = id;

	m_pNodeView->MoveToNode(mCurrentObjectID);
}

void NFBluePrintView::SetCurrentLogicBlockID(const NFGUID& id)
{
	mCurrentLogicBlickID = id;

	m_pNodeView->CleanNodes();

	auto logicBlock = m_pBluePrintModule->GetLogicBlock(mCurrentLogicBlickID);
	if (logicBlock)
	{
		for (auto monitor : logicBlock->monitors)
		{
			m_pNodeView->AddNode(monitor->id, monitor->name);

			for (int i = 0; i  < monitor->GetMonitorJudgementCount(); ++i)
			{
				auto judgement = monitor->GetJudgement(i);

				m_pNodeView->AddNode(judgement->id, judgement->name);
			}
		}

		for (auto variable : logicBlock->variables)
		{
			m_pNodeView->AddNode(variable->id, variable->name);
		}
	}
	//add all nodes
	//add all links

}

NFTreeView* NFBluePrintView::GetTreeView()
{
	return m_pTreeView;
}

NFNodeView* NFBluePrintView::GetNodeView()
{
	return m_pNodeView;
}

void NFBluePrintView::SubRender()
{
	//ImGui::GetIO().KeyCtrl)
   m_pTreeView->Execute();
}

void NFBluePrintView::HandlerSelected(const NFGUID& id)
{
	mCurrentObjectID = NFGUID();
	auto node = m_pBluePrintModule->FindBaseNode(id);
	if (node)
	{
		SetCurrentObjectID(id);
	}

	//referesh for sub window
	//occupy inspectorview
	NF_SHARE_PTR<NFIView> pHierachyView = m_pUIModule->GetView(NFViewType::HierachyView);
	NF_SHARE_PTR<NFIView> pInspectorView = m_pUIModule->GetView(NFViewType::InspectorView);
	if (pHierachyView && pInspectorView)
	{
		pInspectorView->OccupySubRender(pHierachyView.get());
	}
}

void NFBluePrintView::NodeModifyEvent(const NFGUID& id, const bool create)
{
	if (create)
	{
		auto node = m_pBluePrintModule->FindBaseNode(id);
		if (node->parent)
		{
			m_pTreeView->AddSubTreeNode(node->parent->id, node->id, node->name);

			switch (node->blueprintType)
			{
			case NFBlueprintType::MONITOR:
			{
				auto monitor = std::dynamic_pointer_cast<NFIMonitor>(node);
				AddMonitorNode(monitor);
			}
				break;
			case NFBlueprintType::VARIABLE:
			{
				auto variable = std::dynamic_pointer_cast<NFIVariable>(node);
				AddVariableNode(variable);
			}
			break;
			default:
				break;
			}
		}
		else
		{
			m_pTreeView->AddTreeNode(node->id, node->name);
		}
	}
	else
	{
		m_pTreeView->DeleteTreeNode(id);
		m_pNodeView->DeleteNode(id);
	}
	/*
	//m_pTreeView->Clear();
	//m_pNodeView->CleanNodes();

	const std::list<NF_SHARE_PTR<NFLogicBlock>>& logicBlocks = m_pBluePrintModule->GetLogicBlocks();
	for (auto block : logicBlocks)
	{
		m_pTreeView->AddTreeNode(block->id, block->name);

		for (auto monitor : block->monitors)
		{
			m_pTreeView->AddSubTreeNode(block->id, monitor->id, monitor->name);
			m_pNodeView->AddNode(monitor->id, monitor->name);

			for (auto judgement : monitor->judgements)
			{
				AddJudgementNode(m_pTreeView, monitor->id, judgement);
			}
		}

		for (auto variable : block->variables)
		{
			m_pTreeView->AddSubTreeNode(block->id, variable->id, variable->name);
			m_pNodeView->AddNode(variable->id, variable->name);
		}
	}
	*/

}

void NFBluePrintView::LinkModifyEvent(const NFGUID& startNode, const const NFGUID& endNode, const NFGUID& startPin, const const NFGUID& endPin, const bool create)
{
	if (create)
	{
		m_pNodeView->AddLink(startNode, endNode, startPin, endPin);
	}
	else
	{
		m_pNodeView->DeleteLink(startNode, endNode, startPin, endPin);
	}
}

bool NFBluePrintView::TryNewLinkEvent(const NFGUID& startNode, const const NFGUID& endNode, const NFGUID& startPin, const const NFGUID& endPin)
{
	bool ret = false;

	auto start = m_pBluePrintModule->FindBaseNode(startNode);
	auto end = m_pBluePrintModule->FindBaseNode(endNode);
	if (start && end)
	{
		if (start->blueprintType == NFBlueprintType::VARIABLE)
		{
			switch (end->blueprintType)
			{
			case NFBlueprintType::MONITOR:
			{
				ret = TryNewLinkEventForVariableToMonitor(start, end, startPin, endPin);
				
			}
				break;
			case NFBlueprintType::VARIABLE:
				break;
			default:
				break;
			}
		}


		if (ret)
		{
		}
	}

	///log error

	return ret;


}

bool NFBluePrintView::TryNewLinkEventForVariableToMonitor(NF_SHARE_PTR<NFBluePrintNodeBase> startNode, NF_SHARE_PTR<NFBluePrintNodeBase> endNode, const NFGUID& startPin, const const NFGUID& endPin)
{
	if (startNode->logicBlockId != endNode->logicBlockId)
	{
		return false;
	}

	auto variable = std::dynamic_pointer_cast<NFIVariable>(startNode);
	auto monitor = std::dynamic_pointer_cast<NFIMonitor>(endNode);
	switch (monitor->GetMonitorType())
	{
	case NFMonitorType::GameEvent:
	{
		for (int i = 0; i < monitor->GetInputArgCount(); ++i)
		{
			auto inputData = monitor->GetInputArg(i);
			if (inputData->id == endPin)
			{
				if (inputData->valueType == variable->valueType)
				{
					m_pBluePrintModule->AddLink(startNode->logicBlockId, startNode->id, endNode->id, startPin, endPin);
					return true;
				}
			}
		}
		
	}
		break;

	default:
		break;
	}
	return false;
}

void NFBluePrintView::NodeAttriRender(NFNodeAttri* nodeAttri)
{
	NFGUID nodeID = m_pNodeView->GetNodeByAttriId(nodeAttri->nodeId);
	auto node = m_pBluePrintModule->FindBaseNode(nodeAttri->nodeId);
	if (node)
	{
		switch (node->blueprintType)
		{
		case NFBlueprintType::VARIABLE:
			NodeAttriRenderForVariable(nodeAttri);
			break;
		case NFBlueprintType::MONITOR:
			NodeAttriRenderForMonitor(nodeAttri);
			break;
		default:
			break;
		}
	}
}

void NFBluePrintView::NodeAttriRenderForVariable(NFNodeAttri* nodeAttri)
{
	auto variable = std::dynamic_pointer_cast<NFIVariable>(m_pBluePrintModule->FindBaseNode(nodeAttri->nodeId));
	if (variable)
	{
		if (nodeAttri->desc == GET_CLASS_NAME(NFVariableType))
		{
			ImGui::Button(variable->name.c_str());
			/*
			if (ImGui::BeginCombo("NFVariableType", variable->variableType.toString().c_str()))
			{
				for (auto x : NFVariableType::allValues())
				{
					if (ImGui::Selectable(x.toString().c_str(), false))
					{
						variable->variableType = x;
					}
				}
				ImGui::EndCombo();
			}
			*/

		}
		else if (nodeAttri->desc == GET_CLASS_NAME(NFValueType))
		{
			switch (variable->variableType)
			{
			case NFVariableType::Input:
			{
				NodeAttriRenderForInputVariable(nodeAttri);
			}
			break;
			case NFVariableType::ElementSystem:
			{
				NodeAttriRenderForElementVariable(nodeAttri);
			}
			break;
			case NFVariableType::PropertySystem:
			{
				NodeAttriRenderForPropertyVariable(nodeAttri);
			}
			break;
			case NFVariableType::RecordSystem:
			{
				NodeAttriRenderForRecordVariable(nodeAttri);

			}
			break;
			default:
				break;
			}
		}
	}

}

void NFBluePrintView::NodeAttriRenderForInputVariable(NFNodeAttri* nodeAttri)
{
	auto variable = std::dynamic_pointer_cast<NFIVariable>(m_pBluePrintModule->FindBaseNode(nodeAttri->nodeId));

	if (ImGui::BeginCombo("valueType", variable->valueType.toString().c_str()))
	{
		for (auto x : NFValueType::allValues())
		{
			if (ImGui::Selectable(x.toString().c_str(), false))
			{
				variable->valueType = x;
			}
		}
		ImGui::EndCombo();
	}

	ImGui::PushItemWidth(160);

	static char str0[128] = "";
	ImGui::InputText("", str0, IM_ARRAYSIZE(str0));
}

void NFBluePrintView::NodeAttriRenderForElementVariable(NFNodeAttri* nodeAttri)
{
	auto variable = std::dynamic_pointer_cast<NFIVariable>(m_pBluePrintModule->FindBaseNode(nodeAttri->nodeId));

	//class anme && element id && property name
	if (nodeAttri->name == NFElementVariableInputArg::toString(NFElementVariableInputArg::ClassName))
	{
		auto classNameArg = variable->GetInputArg(nodeAttri->name);
		if (ImGui::BeginCombo("currentClassName", classNameArg->varData.c_str()))
		{
			auto classObject = m_pClassModule->First();
			while (classObject)
			{
				if (ImGui::Selectable(classObject->GetClassName().c_str()))
				{
					classNameArg->varData = classObject->GetClassName();
				}

				classObject = m_pClassModule->Next();
			}

			ImGui::EndCombo();
		}
	}
	else if (nodeAttri->name == NFElementVariableInputArg::toString(NFElementVariableInputArg::ElementConfigID))
	{
		auto classNameArg = variable->GetInputArg(NFElementVariableInputArg::toString(NFElementVariableInputArg::ClassName));
		auto elementIDArg = variable->GetInputArg(NFElementVariableInputArg::toString(NFElementVariableInputArg::ElementConfigID));
		auto currentClassObject = m_pClassModule->GetElement(classNameArg->varData);
		if (currentClassObject)
		{
			auto idList = currentClassObject->GetIDList();
			if (ImGui::BeginCombo("currentElementID", elementIDArg->varData.c_str()))
			{
				for (auto id : idList)
				{
					if (ImGui::Selectable(id.c_str()))
					{
						elementIDArg->varData = id;
					}
				}

				ImGui::EndCombo();
			}
		}
	}
	else if (nodeAttri->name == NFElementVariableInputArg::toString(NFElementVariableInputArg::ElementPropertyName))
	{
		//for all properties
		NFDATA_TYPE dataType = NFDATA_TYPE::TDATA_UNKNOWN;
		switch (variable->valueType)
		{
		case NFValueType::Int:
			dataType = NFDATA_TYPE::TDATA_INT;
			break;
		case NFValueType::Float:
			dataType = NFDATA_TYPE::TDATA_FLOAT;
			break;
		case NFValueType::String:
			dataType = NFDATA_TYPE::TDATA_STRING;
			break;
		case NFValueType::Object:
			dataType = NFDATA_TYPE::TDATA_OBJECT;
			break;
		case NFValueType::Vector2:
			dataType = NFDATA_TYPE::TDATA_VECTOR2;
			break;
		case NFValueType::Vector3:
			dataType = NFDATA_TYPE::TDATA_VECTOR3;
			break;
		default:
			break;
		}

		auto classNameArg = variable->GetInputArg(NFElementVariableInputArg::toString(NFElementVariableInputArg::ClassName));
		auto elementIDArg = variable->GetInputArg(NFElementVariableInputArg::toString(NFElementVariableInputArg::ElementConfigID));
		auto propertyNameArg = variable->GetInputArg(NFElementVariableInputArg::toString(NFElementVariableInputArg::ElementPropertyName));
		auto currentClassObject = m_pClassModule->GetElement(classNameArg->varData);
		if (currentClassObject && !elementIDArg->varData.empty())
		{
			if (ImGui::BeginCombo("currentPropertyID", propertyNameArg->varData.c_str()))
			{
				auto property = currentClassObject->GetPropertyManager()->First();
				while (property)
				{
					if (property->GetType() == dataType)
					{
						if (ImGui::Selectable(property->GetKey().c_str()))
						{
							propertyNameArg->varData = property->GetKey();
						}
					}

					property = currentClassObject->GetPropertyManager()->Next();
				}

				ImGui::EndCombo();
			}
		}
	}
}

void NFBluePrintView::NodeAttriRenderForPropertyVariable(NFNodeAttri* nodeAttri)
{
	auto variable = std::dynamic_pointer_cast<NFIVariable>(m_pBluePrintModule->FindBaseNode(nodeAttri->nodeId));

	static std::string currentSelfID = "Self"; //or other propert belong this player<variable>
	//class anme && element id && property name
	static std::string currentClassName = "";
	static std::string currentPropertyName = "";

	if (ImGui::BeginCombo("valueType", variable->valueType.toString().c_str()))
	{
		for (auto x : NFValueType::allValues())
		{
			if (ImGui::Selectable(x.toString().c_str(), false))
			{
				variable->valueType = x;
				currentClassName = "";
			}
		}
		ImGui::EndCombo();
	}

	if (ImGui::BeginCombo("currentClassName", currentClassName.c_str()))
	{
		auto classObject = m_pClassModule->First();
		while (classObject)
		{
			bool is_selected = currentClassName == classObject->GetClassName();
			if (ImGui::Selectable(classObject->GetClassName().c_str(), is_selected))
			{
				currentClassName = classObject->GetClassName();
				currentPropertyName = "";
			}

			classObject = m_pClassModule->Next();
		}

		ImGui::EndCombo();
	}

	//for all element object id
	auto currentClassObject = m_pClassModule->GetElement(currentClassName);
	if (currentClassObject)
	{
		//for all properties
		NFDATA_TYPE dataType = NFDATA_TYPE::TDATA_UNKNOWN;
		switch (variable->valueType)
		{
		case NFValueType::Int:
			dataType = NFDATA_TYPE::TDATA_INT;
			break;
		case NFValueType::Float:
			dataType = NFDATA_TYPE::TDATA_FLOAT;
			break;
		case NFValueType::String:
			dataType = NFDATA_TYPE::TDATA_STRING;
			break;
		case NFValueType::Object:
			dataType = NFDATA_TYPE::TDATA_OBJECT;
			break;
		case NFValueType::Vector2:
			dataType = NFDATA_TYPE::TDATA_VECTOR2;
			break;
		case NFValueType::Vector3:
			dataType = NFDATA_TYPE::TDATA_VECTOR3;
			break;
		default:
			break;
		}

		if (ImGui::BeginCombo("currentPropertyID", currentPropertyName.c_str()))
		{
			auto property = currentClassObject->GetPropertyManager()->First();
			while (property)
			{
				if (property->GetType() == dataType)
				{
					if (ImGui::Selectable(property->GetKey().c_str()))
					{
						currentPropertyName = property->GetKey();
					}
				}

				property = currentClassObject->GetPropertyManager()->Next();
			}

			ImGui::EndCombo();
		}
	}
}

void NFBluePrintView::NodeAttriRenderForRecordVariable(NFNodeAttri* nodeAttri)
{
	auto variable = std::dynamic_pointer_cast<NFIVariable>(m_pBluePrintModule->FindBaseNode(nodeAttri->nodeId));

	//self id && record name && row && col
	ImGui::SameLine();
	if (ImGui::BeginCombo("valueType", variable->valueType.toString().c_str()))
	{
		for (auto x : NFValueType::allValues())
		{
			if (ImGui::Selectable(x.toString().c_str(), false))
			{
				variable->valueType = x;
			}
		}
		ImGui::EndCombo();
	}
}

void NFBluePrintView::NodeAttriRenderForMonitor(NFNodeAttri* nodeAttri)
{

}

void NFBluePrintView::TryToCreateBluePrintBlock()
{
   if (!bCreatingLogicBlock)
   {
      bCreatingLogicBlock = true;
   }
}

void NFBluePrintView::TryToCreateMonitor(NFMonitorType type)
{
	if (!bCreatingMonitor)
	{
		monitorType = type;
		bCreatingMonitor = true;
	}
}

void NFBluePrintView::TryToCreateJudgement()
{
	if (!bCreatingJudgment)
	{
		bCreatingJudgment = true;
	}
}

void NFBluePrintView::TryToCreateExecuter()
{
	if (!bCreatingExecuter)
	{
		bCreatingExecuter = true;
	}
}

void NFBluePrintView::TryToCreateVariable(NFVariableType type)
{
	if (!bCreatingVariable)
	{
		valueType = type;
		bCreatingVariable = true;
	}
}

void NFBluePrintView::TryToCreateComparator()
{
	if (!bCreatingComparator)
	{
		bCreatingComparator = true;
	}
}

void NFBluePrintView::CreateLogicBlockWindow()
{
	if (bCreatingLogicBlock)
	{
		ImGui::OpenPopup("CreatingLogicBlock");
		ImGui::SetNextWindowSize(ImVec2(230, 150));

		ImGuiWindowFlags window_flags = 0;
		window_flags |= ImGuiWindowFlags_NoResize;
		bool open = true;
		if (ImGui::BeginPopupModal("CreatingLogicBlock", &open, window_flags))
		{
			static char str0[128] = "Hello, Blueprint!";
			ImGui::InputText("LogicBlock Name", str0, IM_ARRAYSIZE(str0));

			ImGui::Separator();

			ImGui::Text("All those beautiful files will be deleted.\nThis operation cannot be undone!\n\n");

			ImGui::Separator();

			if (ImGui::Button("Cancel", ImVec2(100, 30)))
			{
			   bCreatingLogicBlock = false;
			   ImGui::CloseCurrentPopup();
			}

			ImGui::SameLine();

			if (ImGui::Button("OK", ImVec2(100, 30)))
			{
			   m_pBluePrintModule->CreateLogicBlock(m_pKernelModule->CreateGUID(), str0);
			   bCreatingLogicBlock = false;
			   ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}
	}
}

void NFBluePrintView::CreateMonitor()
{
	if (bCreatingMonitor)
	{
		auto currentLogicBlock = m_pBluePrintModule->FindBaseNode(GetCurrentObjectID());
		if (currentLogicBlock)
		{
			if (currentLogicBlock->blueprintType == NFBlueprintType::LOGICBLOCK)
			{
				ImGui::OpenPopup("Creating Monitor");
				ImGui::SetNextWindowSize(ImVec2(230, 150));

				ImGuiWindowFlags window_flags = 0;
				window_flags |= ImGuiWindowFlags_NoResize;
				bool open = true;
				if (ImGui::BeginPopupModal("Creating Monitor", &open, window_flags))
				{
					static char str0[128] = "Hello, monitor";
					ImGui::InputText("Monitor Name", str0, IM_ARRAYSIZE(str0));

					ImGui::Separator();

					ImGui::Text("All those beautiful files will be deleted.\nThis operation cannot be undone!\n\n");

					ImGui::Separator();

					if (ImGui::Button("Cancel", ImVec2(100, 30)))
					{
						bCreatingMonitor = false;
						ImGui::CloseCurrentPopup();
					}

					ImGui::SameLine();

					if (ImGui::Button("OK", ImVec2(100, 30)))
					{
						m_pBluePrintModule->AddMonitorForLogicBlock(GetCurrentObjectID(), monitorType, m_pKernelModule->CreateGUID(), str0);
						bCreatingMonitor = false;
						ImGui::CloseCurrentPopup();
					}

					ImGui::EndPopup();
				}
			}
         else
         {
            bCreatingMonitor = false;
         }
		}
      else
      {
		   bCreatingMonitor = false;
      }
	}
}

void NFBluePrintView::CreateJudgment()
{
	if (bCreatingJudgment)
	{
		auto currentObject = m_pBluePrintModule->FindBaseNode(mCurrentObjectID);
		if (currentObject)
		{
			if (currentObject->blueprintType == NFBlueprintType::MONITOR
				|| currentObject->blueprintType == NFBlueprintType::JUDGEMENT
				|| currentObject->blueprintType == NFBlueprintType::EXECUTER)
			{
				ImGui::OpenPopup("Creating Judgment");
				ImGui::SetNextWindowSize(ImVec2(230, 150));

				ImGuiWindowFlags window_flags = 0;
				window_flags |= ImGuiWindowFlags_NoResize;
				bool open = true;
				if (ImGui::BeginPopupModal("Creating Judgment"), &open, window_flags)
				{
					static char str0[128] = "Hello, Judgment!";
					ImGui::InputText("Judgment Name", str0, IM_ARRAYSIZE(str0));

					ImGui::Separator();

					ImGui::Text("All those beautiful files will be deleted.\nThis operation cannot be undone!\n\n");

					ImGui::Separator();

					if (ImGui::Button("Cancel", ImVec2(100, 30)))
					{
						bCreatingJudgment = false;
						ImGui::CloseCurrentPopup();
					}

					ImGui::SameLine();

					if (ImGui::Button("OK", ImVec2(100, 30)))
					{
						if (currentObject->blueprintType == NFBlueprintType::MONITOR)
						{
							m_pBluePrintModule->AddJudgementForMonitor(mCurrentObjectID, m_pKernelModule->CreateGUID(), str0);
						}
						else if (currentObject->blueprintType == NFBlueprintType::JUDGEMENT)
						{
							//m_pBluePrintModule->AddJudgementForJudgement(mCurrentObjectID, m_pKernelModule->CreateGUID(), str0);
						}
						else if (currentObject->blueprintType == NFBlueprintType::EXECUTER)
						{
							m_pBluePrintModule->AddJudgementForExecuter(mCurrentObjectID, m_pKernelModule->CreateGUID(), str0);
						}

						bCreatingJudgment = false;
						ImGui::CloseCurrentPopup();
					}

					ImGui::EndPopup();
				}
         }
         else
         {
            bCreatingJudgment = false;
         }
		}
      else
      {
		   bCreatingJudgment = false;
      }
	}
}

void NFBluePrintView::CreateExecuter()
{
	if (bCreatingExecuter)
	{
		auto currentObject = m_pBluePrintModule->FindBaseNode(mCurrentObjectID);
		if (currentObject)
		{
			if (currentObject->blueprintType == NFBlueprintType::JUDGEMENT
				|| currentObject->blueprintType == NFBlueprintType::EXECUTER)
			{
				ImGui::OpenPopup("Creating Executer");
				ImGui::SetNextWindowSize(ImVec2(230, 150));

				ImGuiWindowFlags window_flags = 0;
				window_flags |= ImGuiWindowFlags_NoResize;
				bool open = true;
				if (ImGui::BeginPopupModal("Creating Executer"), &open, window_flags)
				{
					static char str0[128] = "Hello, Executer!";
					ImGui::InputText("Executer Name", str0, IM_ARRAYSIZE(str0));

					ImGui::Separator();

					ImGui::Text("All those beautiful files will be deleted.\nThis operation cannot be undone!\n\n");

					ImGui::Separator();

					if (ImGui::Button("Cancel", ImVec2(100, 30)))
					{
						bCreatingExecuter = false;
						ImGui::CloseCurrentPopup();
					}

					ImGui::SameLine();

					if (ImGui::Button("OK", ImVec2(100, 30)))
					{
						if (currentObject->blueprintType == NFBlueprintType::JUDGEMENT)
						{
							//m_pBluePrintModule->AddExecuterForJudgement(mCurrentObjectID, m_pKernelModule->CreateGUID(), str0);
						}
						else if (currentObject->blueprintType == NFBlueprintType::EXECUTER)
						{
							m_pBluePrintModule->AddExecuterForExecuter(mCurrentObjectID, m_pKernelModule->CreateGUID(), str0);
						}

						bCreatingExecuter = false;
						ImGui::CloseCurrentPopup();
					}

					ImGui::EndPopup();
				}
			}
			else
			{
				bCreatingExecuter = false;
			}
		}
		else
		{
			bCreatingExecuter = false;
		}
	}
}

void NFBluePrintView::CreateVariable()
{
	if (bCreatingVariable)
	{
		auto currentObject = m_pBluePrintModule->FindBaseNode(mCurrentObjectID);
		if (currentObject)
		{
			ImGui::OpenPopup("Creating Variable");
			ImGui::SetNextWindowSize(ImVec2(230, 150));

			ImGuiWindowFlags window_flags = 0;
			window_flags |= ImGuiWindowFlags_NoResize;
			bool open = true;
			if (ImGui::BeginPopupModal("Creating Variable"), &open, window_flags)
			{
				static char str0[128] = "0";
				memset(str0, 0, IM_ARRAYSIZE(str0));
				std::string valueTypeData = NFVariableType::toString(valueType);
				memcpy(str0, valueTypeData.c_str(), valueTypeData.length());

				ImGui::InputText("Variable Name", str0, IM_ARRAYSIZE(str0));

				ImGui::Separator();

				ImGui::Text("All those beautiful files will be deleted.\nThis operation cannot be undone!\n\n");

				ImGui::Separator();

				if (ImGui::Button("Cancel", ImVec2(100, 30)))
				{
					bCreatingVariable = false;
					ImGui::CloseCurrentPopup();
				}

				ImGui::SameLine();

				if (ImGui::Button("OK", ImVec2(100, 30)))
				{
					
					m_pBluePrintModule->AddVariableForLogicBlock(currentObject->logicBlockId, valueType, m_pKernelModule->CreateGUID(), str0);

					bCreatingVariable = false;
					ImGui::CloseCurrentPopup();
				}

				ImGui::EndPopup();
			}
		}
		else
		{
			bCreatingVariable = false;
		}
	}
}

void NFBluePrintView::CreateComparator()
{
	if (bCreatingComparator)
	{
		auto currentObject = m_pBluePrintModule->FindBaseNode(mCurrentObjectID);
		if (currentObject)
		{
			if (currentObject->blueprintType == NFBlueprintType::JUDGEMENT)
			{
				ImGui::OpenPopup("Creating Comparator");
				ImGui::SetNextWindowSize(ImVec2(230, 150));

				ImGuiWindowFlags window_flags = 0;
				window_flags |= ImGuiWindowFlags_NoResize;
				bool open = true;
				if (ImGui::BeginPopupModal("Creating Comparator"), &open, window_flags)
				{
					static char str0[128] = "Hello, Comparator!";
					ImGui::InputText("Comparator Name", str0, IM_ARRAYSIZE(str0));

					ImGui::Separator();

					ImGui::Text("All those beautiful files will be deleted.\nThis operation cannot be undone!\n\n");

					ImGui::Separator();

					if (ImGui::Button("Cancel", ImVec2(100, 30)))
					{
						bCreatingComparator = false;
						ImGui::CloseCurrentPopup();
					}

					ImGui::SameLine();

					if (ImGui::Button("OK", ImVec2(100, 30)))
					{
						auto judgement = std::dynamic_pointer_cast<NFJudgement>(currentObject);

						auto compator = NF_SHARE_PTR<NFComparator>(NF_NEW NFComparator());
						judgement->comparators.push_back(compator);

						bCreatingComparator = false;
						ImGui::CloseCurrentPopup();
					}

					ImGui::EndPopup();
				}
			}
         else
         {
            bCreatingComparator = false;
         }
		}
      else
      {
         bCreatingComparator = false;
      }
	}
}

void NFBluePrintView::AddMonitorNode(NF_SHARE_PTR<NFIMonitor> monitor)
{
	m_pNodeView->AddNode(monitor->id, monitor->name, NFVector2(0, 0));

	switch (monitor->GetMonitorType())
	{
	case NFMonitorType::GameEvent:
	{
		//auto gameEventMonitor = std::dynamic_pointer_cast<NFGameEventMonitor>(monitor);
		for (int i = 0; i < monitor->GetInputArgCount(); ++i)
		{
			auto intputArg = monitor->GetInputArg(i);
			m_pNodeView->AddNodeAttrIn(monitor->id, intputArg->id, intputArg->name, "");
		}
	}
	break;
	default:
		break;
	}
}

void NFBluePrintView::AddVariableNode(NF_SHARE_PTR<NFIVariable> variable)
{
	m_pNodeView->AddNode(variable->id, variable->name, NFVector2(0, 0));

	//m_pNodeView->AddNodeAttrIn(variable->id, m_pKernelModule->CreateGUID(), GET_CLASS_NAME(NFVariableType), GET_CLASS_NAME(NFVariableType));
	for (int i = 0; i < variable->GetInputArgCount(); ++i)
	{
		auto variableArg = variable->GetInputArg(i);
		m_pNodeView->AddNodeAttrIn(variable->id, variableArg->id, variableArg->name, GET_CLASS_NAME(NFValueType));
	}

	m_pNodeView->AddNodeAttrOut(variable->id, m_pKernelModule->CreateGUID(), "True", "");
}
