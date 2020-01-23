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
	int BLACK = IM_COL32(0, 0, 0, 255);
	int WHITE = IM_COL32(255, 255, 255, 255);
	int RED = IM_COL32(255, 0, 0, 255);
	int LIME = IM_COL32(0, 255, 0, 255);
	int BLUE = IM_COL32(0, 0, 255, 255);
	int YELLOW = IM_COL32(255, 255, 0, 255);
	int CYAN = IM_COL32(0, 255, 255, 255);
	int MAGENTA = IM_COL32(255, 0, 255, 255);
	int SILVER = IM_COL32(192, 192, 192, 255);
	int GRAY = IM_COL32(128, 128, 128, 255);
	int MAROON = IM_COL32(128, 0, 0, 255);
	int OLIVE = IM_COL32(128, 128, 0, 255);
	int GREEN = IM_COL32(0, 128, 0, 255);
	int PURPLE = IM_COL32(128, 0, 128, 255);
	int TEAL = IM_COL32(0, 128, 128, 255);
	int NAVY = IM_COL32(0, 0, 128, 255);


   m_pNodeView = NF_NEW NFNodeView(p);
   m_pTreeView = NF_NEW NFTreeView(p);

   m_pNodeView->ResetOffest(NFVector2::Zero());
   m_pNodeView->SetUpNewLinkCallBack(std::bind(&NFBluePrintView::TryNewLinkEvent, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));
   m_pNodeView->SetUpDeleteLinkCallBack(std::bind(&NFBluePrintView::TryDeleteLinkEvent, this, std::placeholders::_1));
   m_pNodeView->SetPinRenderCallBack(std::bind(&NFBluePrintView::PinRender, this, std::placeholders::_1));

   m_pTreeView->SetSelectedNodeFunctor(std::bind(&NFBluePrintView::HandlerSelected, this, std::placeholders::_1));
   m_pTreeView->SetName(GET_CLASS_NAME(NFBluePrintView));

   m_pUIModule = pPluginManager->FindModule<NFIUIModule>();
   m_pBluePrintModule = pPluginManager->FindModule<NFIBluePrintModule>();
   m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
   m_pClassModule = pPluginManager->FindModule<NFIClassModule>();
   m_pElementModule = pPluginManager->FindModule<NFIElementModule>();

   m_pBluePrintModule->SetNodeModifyEventFunctor(std::bind(&NFBluePrintView::NodeModifyEvent, this, std::placeholders::_1, std::placeholders::_2));
   m_pBluePrintModule->SetLinkModifyEventFunctor(std::bind(&NFBluePrintView::LinkModifyEvent, this, std::placeholders::_1, std::placeholders::_2));
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
   
   CreateLogicBlock();
   CreateMonitor();
   CreateBranch();
   CreateExecuter();
   CreateModifier();
   CreateVariable();
   CreateArithmetic();

   if (ImGui::IsWindowFocused())
   {
      NF_SHARE_PTR<NFIView> pView = m_pUIModule->GetView(NFViewType::HierachyView);
      if (pView)
      {
         pView->OccupySubRender(this);
      }
   }

	return false;
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
		}

		for (auto variable : logicBlock->variables)
		{
			m_pNodeView->AddNode(variable->id, variable->name);
		}

		for (auto branch : logicBlock->branches)
		{
			m_pNodeView->AddNode(branch->id, branch->name);
		}

		for (auto executer : logicBlock->executers)
		{
			m_pNodeView->AddNode(executer->id, executer->name);
		}
		for (auto modifier : logicBlock->modifiers)
		{
			m_pNodeView->AddNode(modifier->id, modifier->name);
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
	auto node = m_pBluePrintModule->FindNode(id);
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
		auto node = m_pBluePrintModule->FindNode(id);
		if (node->blueprintType == NFBlueprintType::LOGICBLOCK)
		{
			m_pTreeView->AddTreeNode(node->id, node->name);
		}
		else
		{
			m_pTreeView->AddSubTreeNode(node->logicBlockId, node->id, node->name);
			NFPinColor color = GetBackGroundColor(node);

			m_pNodeView->AddNode(node->id, node->name, color, NFVector2(0, 0));

			for (int i = 0; i < node->GetInputArgCount(); ++i)
			{
				auto variableArg = node->GetInputArg(i);
				NFPinColor pinColor = NFPinColor::BLUE;
				if (variableArg->valueType == NFValueType::Node)
				{
					pinColor = NFPinColor::YELLOW;
				}
				else
				{
					if (variableArg->fromType == NFIODataComFromType::INTERNAL)
					{
						pinColor = NFPinColor::GRAY;
					}
					else
					{
						pinColor = NFPinColor::RED;
					}
				}

				m_pNodeView->AddPinIn(node->id, variableArg->id, variableArg->name, pinColor);
			}

			for (int i = 0; i < node->GetOutputArgCount(); ++i)
			{
				auto variableArg = node->GetOutputArg(i);
				NFPinColor pinColor = NFPinColor::WHITE;

				if (variableArg->valueType == NFValueType::Node)
				{
					pinColor = NFPinColor::YELLOW;
				}
				else
				{
					pinColor = NFPinColor::WHITE;
				}

				m_pNodeView->AddPinOut(node->id, variableArg->id, variableArg->name, pinColor);
			}
		}
	}
	else
	{
		m_pTreeView->DeleteTreeNode(id);
		m_pNodeView->DeleteNode(id);
	}
}

void NFBluePrintView::LinkModifyEvent(const NFGUID& id, const bool create)
{
	//
	auto linkData = m_pBluePrintModule->GetLink(id);
	if (linkData)
	{
		if (create)
		{
			NFPinColor color = NFPinColor::WHITE;
			auto endNpde =m_pBluePrintModule->FindNode(linkData->endNode);
			if (endNpde)
			{
				for (int i = 0; i < endNpde->GetInputArgCount(); ++i)
				{
					auto inputAgr = endNpde->GetInputArg(i);
					if (inputAgr->id == linkData->endAttr)
					{
						if (inputAgr->valueType == NFValueType::Node)
						{
							color = NFPinColor::YELLOW;
						}
						else if (inputAgr->valueType == NFValueType::UNKNOW)
						{
							color = NFPinColor::BLACK;
						}
						else
						{
							m_pNodeView->ModifyPinColor(inputAgr->id, NFPinColor::BLUE);
							color = NFPinColor::BLUE;
						}

						break;
					}
				}
				
			}

			m_pNodeView->AddLink(id, linkData->startNode, linkData->endNode, linkData->startAttr, linkData->endAttr, color);
		}
		else
		{
			m_pNodeView->DeleteLink(linkData->startNode, linkData->endNode, linkData->startAttr, linkData->endAttr);
		}
	}
}

bool NFBluePrintView::TryNewLinkEvent(const NFGUID& startNode, const NFGUID& endNode, const NFGUID& startPin, const NFGUID& endPin)
{
	auto startNodeObject = m_pBluePrintModule->FindNode(startNode);
	auto endNodeObject = m_pBluePrintModule->FindNode(endNode);

	if (startNodeObject && endNodeObject)
	{
		for (int i = 0; i < startNodeObject->GetOutputArgCount(); ++i)
		{
			auto outputArg = startNodeObject->GetOutputArg(i);
			if (outputArg->id == startPin)
			{
				for (int i = 0; i < endNodeObject->GetInputArgCount(); ++i)
				{
					auto inputArg = endNodeObject->GetInputArg(i);
					if (inputArg->id == endPin)
					{
						if (inputArg->fromType == NFIODataComFromType::EXTERNAL)
						{
							if (inputArg->valueType == outputArg->valueType)
							{
								//if linked
								if (!inputArg->GetLinkID().IsNull())
								{
									//show a error message
									return false;
								}

								if (outputArg->valueType == NFValueType::Node
									&& !outputArg->GetLinkID().IsNull())
								{
									//show a error message
									return false;
								}

								if (inputArg->valueType == NFValueType::UNKNOW
									|| outputArg->valueType == NFValueType::UNKNOW)
								{
									//show a error message
									return false;
								}

								if (endNodeObject->blueprintType == NFBlueprintType::BRANCH)
								{
									auto comparetorArg = endNodeObject->GetInputArg(NFBranchInputArg::Comparator);
									if (inputArg->id == comparetorArg->id)
									{
										//show a error message
										return false;
									}
								}

								m_pBluePrintModule->AddLink(startNodeObject->logicBlockId, m_pKernelModule->CreateGUID(), startNodeObject->id, endNodeObject->id, startPin, endPin);
								return true;
							}
						}
					}
				}
			}
		}
	}

	return false;
}

bool NFBluePrintView::TryDeleteLinkEvent(const NFGUID& id)
{
	if (m_pBluePrintModule->DeleteLink(id))
	{
		m_pNodeView->DeleteLink(id);
	}

	return true;
}

void NFBluePrintView::PinRender(NFNodePin* pin)
{
	NFGUID nodeID = m_pNodeView->GetNodeByAttriId(pin->nodeId);
	auto node = m_pBluePrintModule->FindNode(pin->nodeId);
	if (node)
	{
		switch (node->blueprintType)
		{
		case NFBlueprintType::VARIABLE:
			PinRenderForVariable(pin);
			break;
		case NFBlueprintType::MONITOR:
			PinRenderForMonitor(pin);
			break;
		case NFBlueprintType::BRANCH:
			PinRenderForBranch(pin);
			break;
		case NFBlueprintType::MODIFIER:
			PinRenderForModifier(pin);
			break;
		case NFBlueprintType::LOGGER:
			PinRenderForLogger(pin);
			break;
		case NFBlueprintType::EXECUTER:
			PinRenderForExecuter(pin);
			break;
		default:
			break;
		}
	}
}

void NFBluePrintView::PinRenderForVariable(NFNodePin* pin)
{
	auto variable = std::dynamic_pointer_cast<NFIVariable>(m_pBluePrintModule->FindNode(pin->nodeId));
	if (variable)
	{
		switch (variable->variableType)
		{
		case NFVariableType::Input:
		{
			PinRenderForInputVariable(pin);
		}
		break;
		case NFVariableType::ElementSystem:
		{
			PinRenderForElementVariable(pin);
		}
		break;
		case NFVariableType::PropertySystem:
		{
			PinRenderForPropertyVariable(pin);
		}
		break;
		case NFVariableType::RecordSystem:
		{
			PinRenderForRecordVariable(pin);

		}
		break;
		default:
			break;
		}
	}

}

void NFBluePrintView::PinRenderForInputVariable(NFNodePin* pin)
{
	int itemWidth = 100;
	auto variable = std::dynamic_pointer_cast<NFIVariable>(m_pBluePrintModule->FindNode(pin->nodeId));

	static char str0[128] = "";

	if (pin->inputPin)
	{
		ImGui::PushItemWidth(itemWidth);

		auto inputArg = variable->GetInputArg(0);
		if (ImGui::BeginCombo(pin->name.c_str(), NFValueType::toString(inputArg->valueType).c_str()))
		{
			for (auto x : NFValueType::allValues())
			{
				if (x == NFValueType::Node
					|| x == NFValueType::UNKNOW)
				{
					continue;
				}

				if (ImGui::Selectable(x.toString().c_str(), false))
				{
					auto outputArg = variable->GetOutputArg(0);
					if (!outputArg->GetLinkID().IsNull())
					{
						break;
					}

					inputArg->valueType = x;
					inputArg->varData = "";
					memset(str0, 0, sizeof(str0));
					variable->UpdateOutputData();
				}
			}
			ImGui::EndCombo();
		}

		if (ImGui::InputText("", str0, IM_ARRAYSIZE(str0)))
		{
			inputArg->varData = str0;

			variable->UpdateOutputData();
		}

		ImGui::PopItemWidth();
	}
	else
	{
		auto outputData = variable->GetOutputArg(0);
		ImGui::Indent(itemWidth);
		ImGui::TextColored(ImVec4(1.0f, 0.4f, 0.4f, 1.0f), outputData->varData.c_str());
	}
}

void NFBluePrintView::PinRenderForElementVariable(NFNodePin* pin)
{
	int itemWidth = 100;

	auto variable = std::dynamic_pointer_cast<NFIVariable>(m_pBluePrintModule->FindNode(pin->nodeId));

	auto classNameArg = variable->GetInputArg(NFElementVariableInputArg::toString(NFElementVariableInputArg::ClassName));
	auto elementIDArg = variable->GetInputArg(NFElementVariableInputArg::toString(NFElementVariableInputArg::ConfigID));
	auto propertyNameArg = variable->GetInputArg(NFElementVariableInputArg::toString(NFElementVariableInputArg::PropertyName));

	//class anme && element id && property name
	if (pin->name == NFElementVariableInputArg::toString(NFElementVariableInputArg::ClassName))
	{
		ImGui::PushItemWidth(itemWidth);

		if (ImGui::BeginCombo(pin->name.c_str(), classNameArg->varData.c_str()))
		{
			auto classObject = m_pClassModule->First();
			while (classObject)
			{
				if (classObject->GetIDList().size() > 0)
				{
					if (ImGui::Selectable(classObject->GetClassName().c_str()))
					{
						classNameArg->varData = classObject->GetClassName();
						elementIDArg->varData = "";
						propertyNameArg->varData = "";
					}
				}

				classObject = m_pClassModule->Next();
			}

			ImGui::EndCombo();
		}

		ImGui::PopItemWidth();
	}
	else if (pin->name == NFElementVariableInputArg::toString(NFElementVariableInputArg::ConfigID))
	{
		auto currentClassObject = m_pClassModule->GetElement(classNameArg->varData);
		if (currentClassObject)
		{
			ImGui::PushItemWidth(itemWidth);

			auto idList = currentClassObject->GetIDList();
			if (ImGui::BeginCombo(pin->name.c_str(), elementIDArg->varData.c_str()))
			{
				for (auto id : idList)
				{
					if (ImGui::Selectable(id.c_str()))
					{
						elementIDArg->varData = id;
						variable->UpdateOutputData();
					}
				}

				ImGui::EndCombo();
			}

			ImGui::PopItemWidth();
		}
	}
	else if (pin->name == NFElementVariableInputArg::toString(NFElementVariableInputArg::PropertyName))
	{
		//for all properties
		NFDATA_TYPE dataType = NFDATA_TYPE::TDATA_UNKNOWN;
		switch (propertyNameArg->valueType)
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

		auto currentClassObject = m_pClassModule->GetElement(classNameArg->varData);
		if (currentClassObject && !elementIDArg->varData.empty())
		{
			ImGui::PushItemWidth(itemWidth);

			if (ImGui::BeginCombo(pin->name.c_str(), propertyNameArg->varData.c_str()))
			{
				auto property = currentClassObject->GetPropertyManager()->First();
				while (property)
				{
					if (property->GetType() == dataType)
					{
						if (ImGui::Selectable(property->GetKey().c_str()))
						{
							propertyNameArg->varData = property->GetKey();
							variable->UpdateOutputData();
						}
					}

					property = currentClassObject->GetPropertyManager()->Next();
				}

				ImGui::EndCombo();
			}

			ImGui::PopItemWidth();
		}
	}
	else
	{
		//output
		if (!pin->inputPin)
		{
			auto outputData = variable->GetOutputArg(0);
			ImGui::Indent(itemWidth);
			ImGui::TextColored(ImVec4(1.0f, 0.4f, 0.4f, 1.0f), outputData->varData.c_str());
		}
	}
}

void NFBluePrintView::PinRenderForPropertyVariable(NFNodePin* pin)
{
	int itemWidth = 100;

	auto variable = std::dynamic_pointer_cast<NFIVariable>(m_pBluePrintModule->FindNode(pin->nodeId));

	auto onwerID = variable->GetInputArg(NFPropertyVariableInputArg::toString(NFPropertyVariableInputArg::ObjectID));
	auto classNameArg = variable->GetInputArg(NFPropertyVariableInputArg::toString(NFPropertyVariableInputArg::ClassName));
	auto propertyNameArg = variable->GetInputArg(NFPropertyVariableInputArg::toString(NFPropertyVariableInputArg::PropertyName));
	
	/*
	ImGui::PushItemWidth(itemWidth);

	if (ImGui::BeginCombo("ValueType", variable->valueType.toString().c_str()))
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
	ImGui::PopItemWidth();
	*/

	if (pin->name == NFPropertyVariableInputArg::toString(NFPropertyVariableInputArg::ObjectID))
	{
		ImGui::Button("None");
	}
	else if (pin->name == NFPropertyVariableInputArg::toString(NFPropertyVariableInputArg::ClassName))
	{
		ImGui::PushItemWidth(itemWidth);

		if (ImGui::BeginCombo(pin->name.c_str(), classNameArg->varData.c_str()))
		{
			auto classObject = m_pClassModule->First();
			while (classObject)
			{
				if (ImGui::Selectable(classObject->GetClassName().c_str()))
				{
					auto outputArg = variable->GetOutputArg(0);
					if (!outputArg->GetLinkID().IsNull())
					{
						break;
					}

					classNameArg->varData = classObject->GetClassName();
					propertyNameArg->varData = "";
					propertyNameArg->valueType = NFValueType::UNKNOW;
					variable->UpdateOutputData();
				}

				classObject = m_pClassModule->Next();
			}

			ImGui::EndCombo();
		}

		ImGui::PopItemWidth();
	}
	else if (pin->name == NFPropertyVariableInputArg::toString(NFPropertyVariableInputArg::PropertyName))
	{
		auto currentClassObject = m_pClassModule->GetElement(classNameArg->varData);
		if (currentClassObject)
		{
			ImGui::PushItemWidth(itemWidth);
			if (ImGui::BeginCombo(pin->name.c_str(), propertyNameArg->varData.c_str()))
			{
				auto property = currentClassObject->GetPropertyManager()->First();
				while (property)
				{
					if (ImGui::Selectable(property->GetKey().c_str()))
					{
						auto outputArg = variable->GetOutputArg(0);
						if (!outputArg->GetLinkID().IsNull())
						{
							break;
						}

						propertyNameArg->varData = property->GetKey();
						variable->UpdateOutputData();
					}

					property = currentClassObject->GetPropertyManager()->Next();
				}

				ImGui::EndCombo();
			}
			ImGui::PopItemWidth();
		}
	}
	else
	{
		if (!pin->inputPin)
		{
			auto outputData = variable->GetOutputArg(0);
			ImGui::Indent(itemWidth);
			ImGui::TextColored(ImVec4(1.0f, 0.4f, 0.4f, 1.0f), outputData->varData.c_str());
		}
	}

}

void NFBluePrintView::PinRenderForRecordVariable(NFNodePin* pin)
{
	int itemWidth = 100;

	auto variable = std::dynamic_pointer_cast<NFIVariable>(m_pBluePrintModule->FindNode(pin->nodeId));

	auto onwerID = variable->GetInputArg(NFRecordVariableInputArg::toString(NFRecordVariableInputArg::ObjectID));
	auto classNameArg = variable->GetInputArg(NFRecordVariableInputArg::toString(NFRecordVariableInputArg::ClassName));
	auto recordNameArg = variable->GetInputArg(NFRecordVariableInputArg::toString(NFRecordVariableInputArg::RecordName));
	auto recordRowArg = variable->GetInputArg(NFRecordVariableInputArg::toString(NFRecordVariableInputArg::RecordRow));
	auto recordColArg = variable->GetInputArg(NFRecordVariableInputArg::toString(NFRecordVariableInputArg::RecordCol));

	/*
	ImGui::PushItemWidth(itemWidth);

	if (ImGui::BeginCombo("ValueType", variable->valueType.toString().c_str()))
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
	ImGui::PopItemWidth();
	*/

	if (pin->name == NFRecordVariableInputArg::toString(NFRecordVariableInputArg::ObjectID))
	{
		ImGui::Button(onwerID->varData.c_str());
	}
	else if (pin->name == NFRecordVariableInputArg::toString(NFRecordVariableInputArg::ClassName))
	{
		ImGui::PushItemWidth(itemWidth);

		if (ImGui::BeginCombo(pin->name.c_str(), classNameArg->varData.c_str()))
		{
			auto classObject = m_pClassModule->First();
			while (classObject)
			{
				if (ImGui::Selectable(classObject->GetClassName().c_str()))
				{
					classNameArg->varData = classObject->GetClassName();
					recordNameArg->varData = "";
				}

				classObject = m_pClassModule->Next();
			}

			ImGui::EndCombo();
		}

		ImGui::PopItemWidth();
	}
	else if (pin->name == NFRecordVariableInputArg::toString(NFRecordVariableInputArg::RecordName))
	{
		auto currentClassObject = m_pClassModule->GetElement(classNameArg->varData);
		if (currentClassObject)
		{
			//for all properties
			NFDATA_TYPE dataType = NFDATA_TYPE::TDATA_UNKNOWN;
			switch (recordNameArg->valueType)
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

			ImGui::PushItemWidth(itemWidth);
			if (ImGui::BeginCombo(pin->name.c_str(), recordNameArg->varData.c_str()))
			{
				auto property = currentClassObject->GetPropertyManager()->First();
				while (property)
				{
					if (property->GetType() == dataType)
					{
						if (ImGui::Selectable(property->GetKey().c_str()))
						{
							recordNameArg->varData = property->GetKey();
						}
					}

					property = currentClassObject->GetPropertyManager()->Next();
				}

				ImGui::EndCombo();
			}
			ImGui::PopItemWidth();
		}
	}

}

void NFBluePrintView::PinRenderForMonitor(NFNodePin* pin)
{
	auto monitor = std::dynamic_pointer_cast<NFIMonitor>(m_pBluePrintModule->FindNode(pin->nodeId));
	if (monitor)
	{
		switch (monitor->monitorType)
		{
		case NFMonitorType::GameEvent:
		{
			PinRenderForGameEventMonitor(pin);
		}
		break;
		case NFMonitorType::NetworkEvent:
		{
			PinRenderForNetworkEventMonitor(pin);
		}
		break;
		case NFMonitorType::NetworkMsgEvent:
		{
			PinRenderForNetworkMsgMonitor(pin);
		}
		break;
		case NFMonitorType::ObjectEvent:
		{
			PinRenderForObjectEventMonitor(pin);
		}
		break;
		case NFMonitorType::PropertyEvent:
		{
			PinRenderForPropertyEventMonitor(pin);
		}
		break;
		case NFMonitorType::RecordEvent:
		{
			PinRenderForRecordEventMonitor(pin);
		}
		break;
		case NFMonitorType::SceneEvent:
		{
			PinRenderForSceneEventMonitor(pin);
		}
		break;
		case NFMonitorType::ItemEvent:
		{
			PinRenderForItemEventMonitor(pin);
		}
		break;
		case NFMonitorType::SkillEvent:
		{
			PinRenderForSkillEventMonitor(pin);
		}
		break;
		case NFMonitorType::BuffEvent:
		{
			PinRenderForBuffEventMonitor(pin);
		}
		break;
		default:
			break;
		}
	}
}

void NFBluePrintView::PinRenderForGameEventMonitor(NFNodePin* pin)
{
	int itemWidth = 100;
	auto monitor = std::dynamic_pointer_cast<NFIMonitor>(m_pBluePrintModule->FindNode(pin->nodeId));

	static char str0[128] = "";

	if (pin->inputPin)
	{
		ImGui::PushItemWidth(itemWidth);

		if (pin->name == NFGameEventMonitorInputArg::toString(NFGameEventMonitorInputArg::EventID))
		{
			ImGui::PushItemWidth(itemWidth);
			auto inputArg = monitor->GetInputArg(0);

			if (ImGui::InputText("", str0, IM_ARRAYSIZE(str0)))
			{
				inputArg->varData = str0;

				monitor->UpdateOutputData();
			}

			ImGui::PopItemWidth();
		}
		ImGui::PopItemWidth();
	}
	else
	{
	}
}

void NFBluePrintView::PinRenderForNetworkEventMonitor(NFNodePin* pin)
{
	int itemWidth = 100;
	auto monitor = std::dynamic_pointer_cast<NFIMonitor>(m_pBluePrintModule->FindNode(pin->nodeId));

	static char str0[128] = "";

	if (pin->inputPin)
	{
		ImGui::PushItemWidth(itemWidth);

		if (pin->name == NFNetworkEventMonitorInputArg::toString(NFNetworkEventMonitorInputArg::NetEventID))
		{
			ImGui::PushItemWidth(itemWidth);
			auto inputArg = monitor->GetInputArg(0);

			if (ImGui::InputText("", str0, IM_ARRAYSIZE(str0)))
			{
				inputArg->varData = str0;

				monitor->UpdateOutputData();
			}

			ImGui::PopItemWidth();
		}
		ImGui::PopItemWidth();
	}
}

void NFBluePrintView::PinRenderForNetworkMsgMonitor(NFNodePin* pin)
{
	int itemWidth = 100;
	auto monitor = std::dynamic_pointer_cast<NFIMonitor>(m_pBluePrintModule->FindNode(pin->nodeId));

	static char str0[128] = "";

	if (pin->inputPin)
	{
		ImGui::PushItemWidth(itemWidth);

		if (pin->name == NFNetworkMsgMonitorInputArg::toString(NFNetworkMsgMonitorInputArg::NetMsgID))
		{
			auto inputArg = monitor->GetInputArg(0);

			if (ImGui::InputText("", str0, IM_ARRAYSIZE(str0)))
			{
				inputArg->varData = str0;

				monitor->UpdateOutputData();
			}

		}

		ImGui::PopItemWidth();
	}
	else
	{
		
	}
}

void NFBluePrintView::PinRenderForObjectEventMonitor(NFNodePin* pin)
{
	int itemWidth = 100;
	auto monitor = std::dynamic_pointer_cast<NFIMonitor>(m_pBluePrintModule->FindNode(pin->nodeId));

	if (pin->inputPin)
	{
		if (pin->name == NFMonitorObjectEventInputArg::toString(NFMonitorObjectEventInputArg::ClassName))
		{

			ImGui::PushItemWidth(itemWidth);
			auto inputArg = monitor->GetInputArg(NFMonitorObjectEventInputArg::ClassName);

			if (ImGui::BeginCombo("", inputArg->varData.c_str()))
			{
				auto classObject = m_pClassModule->First();
				while (classObject)
				{
					if (classObject->GetIDList().size() > 0)
					{
						if (ImGui::Selectable(classObject->GetClassName().c_str()))
						{
							inputArg->varData = classObject->GetClassName();
						}
					}

					classObject = m_pClassModule->Next();
				}

				ImGui::EndCombo();
			}

			ImGui::PopItemWidth();
		}
		else if (pin->name == NFMonitorObjectEventInputArg::toString(NFMonitorObjectEventInputArg::ClassEvent))
		{
			//object event: no_data, loading_data.....
		}
	}
}

void NFBluePrintView::PinRenderForPropertyEventMonitor(NFNodePin* pin)
{
	int itemWidth = 100;
	auto monitor = std::dynamic_pointer_cast<NFIMonitor>(m_pBluePrintModule->FindNode(pin->nodeId));

	static char str0[128] = "";

	if (pin->inputPin)
	{

		auto classNameArg = monitor->GetInputArg(NFMonitorPropertyEventInputArg::ClassName);
		auto propertyNameArg = monitor->GetInputArg(NFMonitorPropertyEventInputArg::PropertyName);

		if (pin->name == NFMonitorPropertyEventInputArg::toString(NFMonitorPropertyEventInputArg::ClassName))
		{

			ImGui::PushItemWidth(itemWidth);

			if (ImGui::BeginCombo("", classNameArg->varData.c_str()))
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

			ImGui::PopItemWidth();
		}
		else if(pin->name == NFMonitorPropertyEventInputArg::toString(NFMonitorPropertyEventInputArg::PropertyName))
		{
			if (!classNameArg->varData.empty())
			{
				auto currentClassObject = m_pClassModule->GetElement(classNameArg->varData);
				{

					ImGui::PushItemWidth(itemWidth);

					if (ImGui::BeginCombo("", propertyNameArg->varData.c_str()))
					{
						auto property = currentClassObject->GetPropertyManager()->First();
						while (property)
						{
							{
								if (ImGui::Selectable(property->GetKey().c_str()))
								{
									propertyNameArg->varData = property->GetKey();
									monitor->UpdateOutputData();
								}
							}

							property = currentClassObject->GetPropertyManager()->Next();
						}

						ImGui::EndCombo();
					}

					ImGui::PopItemWidth();
				}
			}
		}
	}
	else
	{
	}
}

void NFBluePrintView::PinRenderForRecordEventMonitor(NFNodePin* pin)
{
}

void NFBluePrintView::PinRenderForSceneEventMonitor(NFNodePin* pin)
{
}

void NFBluePrintView::PinRenderForItemEventMonitor(NFNodePin* pin)
{
}

void NFBluePrintView::PinRenderForSkillEventMonitor(NFNodePin* pin)
{
}

void NFBluePrintView::PinRenderForBuffEventMonitor(NFNodePin* pin)
{
}

void NFBluePrintView::PinRenderForBranch(NFNodePin* pin)
{
	int itemWidth = 100;

	ImGui::PushItemWidth(itemWidth);
	auto branch = std::dynamic_pointer_cast<NFIBranch>(m_pBluePrintModule->FindNode(pin->nodeId));
	if (branch)
	{
		if (pin->name == NFBranchInputArg::toString(NFBranchInputArg::Comparator))
		{
			auto comparatorArg = branch->GetInputArg(NFBranchInputArg::Comparator);

			if (ImGui::BeginCombo("", (comparatorArg->varData.c_str())))
			{
				for (auto x : NFComparatorType::allValues())
				{
					if (ImGui::Selectable(x.toString().c_str(), false))
					{
						comparatorArg->varData = x.toString();
					}
				}
				ImGui::EndCombo();
			}
		}
	}

	ImGui::PopItemWidth();
}

void NFBluePrintView::PinRenderForModifier(NFNodePin* pin)
{
}

void NFBluePrintView::PinRenderForLogger(NFNodePin* pin)
{
}

void NFBluePrintView::PinRenderForExecuter(NFNodePin* pin)
{
}

void NFBluePrintView::TryToCreateBluePrintBlock()
{
   if (!bCreatingLogicBlock)
   {
      bCreatingLogicBlock = true;
   }
}

void NFBluePrintView::TryToCreateArithmetic(NFArithmeticType type)
{
	if (!bCreatingArithmetic)
	{
		bCreatingArithmetic = true;
		arithmeticType = type;
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

void NFBluePrintView::TryToCreateBranch(NFBranchType type)
{
	if (!bCreatingBranch)
	{
		bCreatingBranch = true;
		branchType = type;
	}
}

void NFBluePrintView::TryToCreateExecuter(NFExecuterType type)
{
	if (!bCreatingExecuter)
	{
		bCreatingExecuter = true;
		executerType = type;
	}
}

void NFBluePrintView::TryToCreateModifier(NFModifierType type)
{
	if (!bCreatingModifier)
	{
		bCreatingModifier = true;
		modifierType = type;
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

void NFBluePrintView::CreateLogicBlock()
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
		auto currentLogicBlock = m_pBluePrintModule->FindNode(GetCurrentObjectID());
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
					static char str0[128] = "";
					memset(str0, 0, IM_ARRAYSIZE(str0));
					std::string valueTypeData = NFMonitorType::toString(monitorType);
					memcpy(str0, valueTypeData.c_str(), valueTypeData.length());

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
						m_pBluePrintModule->AddMonitor(GetCurrentObjectID(), monitorType, m_pKernelModule->CreateGUID(), str0);
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

void NFBluePrintView::CreateBranch()
{
	if (bCreatingBranch)
	{
		auto currentObject = m_pBluePrintModule->FindNode(mCurrentObjectID);
		if (currentObject)
		{
			ImGui::OpenPopup("Creating Branch");
			ImGui::SetNextWindowSize(ImVec2(230, 150));

			ImGuiWindowFlags window_flags = 0;
			window_flags |= ImGuiWindowFlags_NoResize;
			bool open = true;
			if (ImGui::BeginPopupModal("Creating Branch"), &open, window_flags)
			{
				static char str0[128] = "";
				memset(str0, 0, IM_ARRAYSIZE(str0));
				std::string valueTypeData = NFBranchType::toString(branchType);
				memcpy(str0, valueTypeData.c_str(), valueTypeData.length());

				ImGui::InputText("Branch Name", str0, IM_ARRAYSIZE(str0));

				ImGui::Separator();

				ImGui::Text("All those beautiful files will be deleted.\nThis operation cannot be undone!\n\n");

				ImGui::Separator();

				if (ImGui::Button("Cancel", ImVec2(100, 30)))
				{
					bCreatingBranch = false;
					ImGui::CloseCurrentPopup();
				}

				ImGui::SameLine();

				if (ImGui::Button("OK", ImVec2(100, 30)))
				{
					m_pBluePrintModule->AddBranch(currentObject->logicBlockId, branchType, m_pKernelModule->CreateGUID(), str0);

					bCreatingBranch = false;
					ImGui::CloseCurrentPopup();
				}

				ImGui::EndPopup();
			}
        }
        else
        {
            bCreatingBranch = false;
        }
	}
}

void NFBluePrintView::CreateExecuter()
{
	if (bCreatingExecuter)
	{
		auto currentObject = m_pBluePrintModule->FindNode(mCurrentObjectID);
		if (currentObject)
		{
			ImGui::OpenPopup("Creating Executer");
			ImGui::SetNextWindowSize(ImVec2(230, 150));

			ImGuiWindowFlags window_flags = 0;
			window_flags |= ImGuiWindowFlags_NoResize;
			bool open = true;
			if (ImGui::BeginPopupModal("Creating Executer"), &open, window_flags)
			{
				static char str0[128] = "Hello, Executer!";
				memset(str0, 0, IM_ARRAYSIZE(str0));
				std::string valueTypeData = NFExecuterType::toString(executerType);
				memcpy(str0, valueTypeData.c_str(), valueTypeData.length());

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
					m_pBluePrintModule->AddExecuter(currentObject->logicBlockId, executerType, m_pKernelModule->CreateGUID(), str0);

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
}

void NFBluePrintView::CreateModifier()
{
	if (bCreatingModifier)
	{
		auto currentObject = m_pBluePrintModule->FindNode(mCurrentObjectID);
		if (currentObject)
		{
			ImGui::OpenPopup("Creating Modifier");
			ImGui::SetNextWindowSize(ImVec2(230, 150));

			ImGuiWindowFlags window_flags = 0;
			window_flags |= ImGuiWindowFlags_NoResize;
			bool open = true;
			if (ImGui::BeginPopupModal("Creating Modifier"), &open, window_flags)
			{
				static char str0[128] = "0";
				memset(str0, 0, IM_ARRAYSIZE(str0));
				std::string valueTypeData = NFModifierType::toString(modifierType);
				memcpy(str0, valueTypeData.c_str(), valueTypeData.length());

				ImGui::InputText("Modifier Name", str0, IM_ARRAYSIZE(str0));

				ImGui::Separator();

				ImGui::Text("All those beautiful files will be deleted.\nThis operation cannot be undone!\n\n");

				ImGui::Separator();

				if (ImGui::Button("Cancel", ImVec2(100, 30)))
				{
					bCreatingModifier = false;
					ImGui::CloseCurrentPopup();
				}

				ImGui::SameLine();

				if (ImGui::Button("OK", ImVec2(100, 30)))
				{

					m_pBluePrintModule->AddModifier(currentObject->logicBlockId, modifierType, m_pKernelModule->CreateGUID(), str0);

					bCreatingModifier = false;
					ImGui::CloseCurrentPopup();
				}

				ImGui::EndPopup();
			}
		}
		else
		{
			bCreatingModifier = false;
		}
	}
}

void NFBluePrintView::CreateVariable()
{
	if (bCreatingVariable)
	{
		auto currentObject = m_pBluePrintModule->FindNode(mCurrentObjectID);
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
					
					m_pBluePrintModule->AddVariable(currentObject->logicBlockId, valueType, m_pKernelModule->CreateGUID(), str0);

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

void NFBluePrintView::CreateArithmetic()
{

	if (bCreatingArithmetic)
	{
		auto currentObject = m_pBluePrintModule->FindNode(mCurrentObjectID);
		if (currentObject)
		{
			ImGui::OpenPopup("Creating Arithmetic");
			ImGui::SetNextWindowSize(ImVec2(230, 150));

			ImGuiWindowFlags window_flags = 0;
			window_flags |= ImGuiWindowFlags_NoResize;
			bool open = true;
			if (ImGui::BeginPopupModal("Creating Arithmetic"), &open, window_flags)
			{
				static char str0[128] = "0";
				memset(str0, 0, IM_ARRAYSIZE(str0));
				std::string valueTypeData = NFVariableType::toString(valueType);
				memcpy(str0, valueTypeData.c_str(), valueTypeData.length());

				ImGui::InputText("Arithmetic Name", str0, IM_ARRAYSIZE(str0));

				ImGui::Separator();

				ImGui::Text("All those beautiful files will be deleted.\nThis operation cannot be undone!\n\n");

				ImGui::Separator();

				if (ImGui::Button("Cancel", ImVec2(100, 30)))
				{
					bCreatingArithmetic = false;
					ImGui::CloseCurrentPopup();
				}

				ImGui::SameLine();

				if (ImGui::Button("OK", ImVec2(100, 30)))
				{

					m_pBluePrintModule->AddArithmetic(currentObject->logicBlockId, arithmeticType, m_pKernelModule->CreateGUID(), str0);

					bCreatingArithmetic = false;
					ImGui::CloseCurrentPopup();
				}

				ImGui::EndPopup();
			}
		}
		else
		{
			bCreatingArithmetic = false;
		}
	}
}

NFPinColor NFBluePrintView::GetBackGroundColor(NF_SHARE_PTR<NFBluePrintNodeBase> node)
{
	/*
		LOGICBLOCK,
	MONITOR,
	BRANCH,
	EXECUTER,
	VARIABLE,
	MODIFIER,
	ARITHMETIC,
	LOGGER,

	    BLACK = -16777216,
    WHITE = - 1,
    RED = -16776961,
    LIME = -16711936,
    BLUE = -65536,
    YELLOW = -16711681,
    CYAN = -256,
    MAGENTA = -65281,
    SILVER = -4144960,
    GRAY = -8355712,
    MAROON = -16777088,
    OLIVE = -16744320,
    GREEN = -16744448,
    PURPLE = -8388480,
    TEAL = -8355840,
    NAVY = -8388608,
	*/
	NFPinColor color = NFPinColor::BLACK;
	switch (node->blueprintType)
	{
	case NFBlueprintType::MONITOR:
		color = NFPinColor::MAROON;
		break;
	case NFBlueprintType::EXECUTER:
		color = NFPinColor::GREEN;
		break;
	case NFBlueprintType::VARIABLE:
		color = NFPinColor::SILVER;
		break;
	case NFBlueprintType::MODIFIER:
		color = NFPinColor::BLUE;
		break;
	case NFBlueprintType::ARITHMETIC:
		color = NFPinColor::OLIVE;
		break;
	case NFBlueprintType::LOGGER:
		color = NFPinColor::GRAY;
		break;
	case NFBlueprintType::BRANCH:
		color = NFPinColor::TEAL;
		break;
	
	default:
		break;
	}

	return color;
}