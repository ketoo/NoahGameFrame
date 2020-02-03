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

#include "NFBluePrintView.h"
#include "NFUIModule.h"

char* variableIntImage = "..//NFDataCfg//Fonts//Icon//Variable//int.png";
char* variableFloatImage = "..//NFDataCfg//Fonts//Icon//Variable//float.png";
char* variableBoolImage = "..//NFDataCfg//Fonts//Icon//Variable//bool.png";
char* variableStringImage = "..//NFDataCfg//Fonts//Icon//Variable//string.png";
char* variableObjectImage = "..//NFDataCfg//Fonts//Icon//Variable//object.png";
char* variableVector2Image = "..//NFDataCfg//Fonts//Icon//Variable//vector2.png";
char* variableVector3Image = "..//NFDataCfg//Fonts//Icon//Variable//vector3.png";
char* variableArrayImage = "..//NFDataCfg//Fonts//Icon//Variable//list.png";
char* variableDictionaryImage = "..//NFDataCfg//Fonts//Icon//Variable//dictionary.png";
char* variableDefaultImage = "..//NFDataCfg//Fonts//Icon//Variable//unknow.png";

typedef std::function<bool(const NFGUID&, const NFGUID&, const NFGUID&, const NFGUID&)> LINK_EVENT_FUNCTOR;

template < typename T >
class SmartBind
{
public:
	SmartBind()
	{
		name = typeid(T).name();
	}

	T t;
	std::string name;
};

NFBluePrintView::NFBluePrintView(NFIPluginManager* p, NFViewType vt) : NFIView(p, vt, GET_CLASS_NAME(NFBluePrintView))
{
	int DEFAULT = IM_COL32(20, 20, 20, 255);
   int EXECUTER = IM_COL32(220, 220, 93, 255);
   int VARIABLE = IM_COL32(175, 175, 175, 255);
   int MODIFIER = IM_COL32(110, 210, 90, 255);
   int BRANCH = IM_COL32(140, 210, 190, 255);
   int MONITOR = IM_COL32(140, 210, 190, 255);
   int LOGGER = IM_COL32(240, 190, 100, 255);
   int ARITHMETIC = IM_COL32(78, 80, 84, 255);
   int CUSTOM = IM_COL32(104, 176, 224, 255);


   m_pNodeView = NF_NEW NFNodeView(p);
   m_pTreeView = NF_NEW NFTreeView(p);


  
   m_pNodeView->ResetOffest(NFVector2::Zero());

   SmartBind<LINK_EVENT_FUNCTOR> linkBind;
   auto bindTest1 = std::bind(&NFBluePrintView::TryNewLinkEvent, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
   auto bindTest2 = std::bind(&NFBluePrintView::TryNewLinkEvent, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);

   linkBind.t = bindTest1;
   //hash value
   std::cout << linkBind.name << std::endl;

   m_pNodeView->SetUpNewLinkCallBack(std::bind(&NFBluePrintView::TryNewLinkEvent, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));
   m_pNodeView->SetUpDeleteLinkCallBack(std::bind(&NFBluePrintView::TryDeleteLinkEvent, this, std::placeholders::_1));
   
   m_pNodeView->SetPinRenderCallBack(std::bind(&NFBluePrintView::PinRender, this, std::placeholders::_1));

   m_pNodeView->SetNodeRenderBeforePinInCallBack(std::bind(&NFBluePrintView::NodeRenderBeforePinIn, this, std::placeholders::_1));
   m_pNodeView->SetNodeRenderAfterPinInCallBack(std::bind(&NFBluePrintView::NodeRenderAfterPinIn, this, std::placeholders::_1));
   m_pNodeView->SetNodeRenderBeforePinOutCallBack(std::bind(&NFBluePrintView::NodeRenderBeforePinOut, this, std::placeholders::_1));
   m_pNodeView->SetNodeRenderAfterPinOutCallBack(std::bind(&NFBluePrintView::NodeRenderAfterPinOut, this, std::placeholders::_1));




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
	if (id == mCurrentLogicBlockID)
	{
		return;
	}

	mCurrentLogicBlockID = id;

	m_pNodeView->CleanNodes();

	auto logicBlock = m_pBluePrintModule->GetLogicBlock(mCurrentLogicBlockID);
	if (logicBlock)
	{
		for (auto node : logicBlock->monitors)
		{
			AddNode(node);
		}

		for (auto node : logicBlock->variables)
		{
			AddNode(node);
		}

		for (auto node : logicBlock->branches)
		{
			AddNode(node);
		}

		for (auto node : logicBlock->executers)
		{
			AddNode(node);
		}
		for (auto node : logicBlock->modifiers)
		{
			AddNode(node);
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
		if (node->blueprintType == NFBlueprintType::LOGICBLOCK)
		{
			SetCurrentLogicBlockID(id);
		}
		else
		{
			SetCurrentObjectID(id);
		}
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

			AddNode(node);
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
			NFColor color = NFColor::DEFAULT;
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
							color = NFColor::WORKFLOW;
						}
						else if (inputAgr->valueType == NFValueType::UNKNOW)
						{
							color = NFColor::DEFAULT;
						}
						else
						{
							m_pNodeView->ModifyPinColor(inputAgr->id, NFColor::PININ);
							color = NFColor::PININ;
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

void NFBluePrintView::AddNode(NF_SHARE_PTR<NFBluePrintNodeBase> node)
{
	if (node->blueprintType != NFBlueprintType::LOGICBLOCK)
	{
		NFColor color = GetBackGroundColor(node);

		m_pNodeView->AddNode(node->id, node->name, color, NFVector2(0, 0));

		for (int i = 0; i < node->GetInputArgCount(); ++i)
		{
			auto variableArg = node->GetInputArg(i);
			NFColor pinColor = NFColor::DEFAULT;
			if (variableArg->valueType == NFValueType::Node)
			{
				pinColor = NFColor::WORKFLOW;
			}
			else
			{
				if (variableArg->fromType == NFIODataComFromType::BOTH
					|| variableArg->fromType == NFIODataComFromType::EXTERNAL)
				{
					pinColor = NFColor::PININ;
				}
				else if (variableArg->fromType == NFIODataComFromType::INTERNAL)
				{
					pinColor = NFColor::DEFAULT;
				}
			}

			std::string imageName = variableDefaultImage;
			switch (variableArg->valueType)
			{
			case NFValueType::Int:
				imageName = variableIntImage;
				break;
			case NFValueType::Float:
				imageName = variableFloatImage;
				break;
			case NFValueType::String:
				imageName = variableStringImage;
				break;
			case NFValueType::Vector2:
				imageName = variableVector2Image;
				break;
			case NFValueType::Vector3:
				imageName = variableVector3Image;
				break;
			case NFValueType::Object:
				imageName = variableObjectImage;
				break;
			case NFValueType::Array:
				imageName = variableArrayImage;
				break;
			case NFValueType::Dictionary:
				imageName = variableDictionaryImage;
				break;
			default:
				break;
			}
			

			m_pNodeView->AddPinIn(node->id, variableArg->id, variableArg->name, imageName, pinColor);
		}

		for (int i = 0; i < node->GetOutputArgCount(); ++i)
		{
			auto variableArg = node->GetOutputArg(i);
			NFColor pinColor = NFColor::DEFAULT;

			if (variableArg->valueType == NFValueType::Node)
			{
				pinColor = NFColor::WORKFLOW;
			}
			else
			{
				pinColor = NFColor::PINOUT;
			}

			m_pNodeView->AddPinOut(node->id, variableArg->id, variableArg->name, "", pinColor);
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
						if (inputArg->fromType != NFIODataComFromType::INTERNAL)
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

void NFBluePrintView::NodeRenderBeforePinIn(NFNode* node)
{

}

void NFBluePrintView::NodeRenderAfterPinIn(NFNode* node)
{
	auto blueprintNode = m_pBluePrintModule->FindNode(node->guid);
	switch (blueprintNode->blueprintType)
	{
	case NFBlueprintType::VARIABLE:
	{
		auto variableNode = std::dynamic_pointer_cast<NFIVariable>(blueprintNode);
		switch (variableNode->variableType)
		{
		case NFVariableType::Dictionary:
		{
			if (ImGui::Button("+"))
			{

			}
		}
		break;
		default:
			break;
		}
	}
	break;
	default:
		break;
	}
}

void NFBluePrintView::NodeRenderBeforePinOut(NFNode* node)
{

}

void NFBluePrintView::NodeRenderAfterPinOut(NFNode* node)
{
	auto blueprintNode = m_pBluePrintModule->FindNode(node->guid);
	switch (blueprintNode->blueprintType)
	{
	case NFBlueprintType::VARIABLE:
	{
		auto variableNode = std::dynamic_pointer_cast<NFIVariable>(blueprintNode);
		switch (variableNode->variableType)
		{
		case NFVariableType::Dictionary:
		{
			if (ImGui::Button("+"))
			{

			}
		}
		break;
		default:
			break;
		}
	}
	break;
	default:
		break;
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
	int itemWidth = 70;
	auto variable = std::dynamic_pointer_cast<NFIVariable>(m_pBluePrintModule->FindNode(pin->nodeId));

	static char str0[128] = "";

	if (pin->inputPin)
	{
		ImGui::PushItemWidth(itemWidth);

		auto inputArg = variable->GetInputArg(0);

		ImGui::SameLine();
		if (ImGui::BeginCombo(" ", inputArg->valueType.toString().c_str()))
		{
			for (auto x : NFValueType::allValues())
			{
				if (x == NFValueType::Node
					|| x == NFValueType::UNKNOW
					|| x == NFValueType::Array
					|| x == NFValueType::Dictionary)
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

		ImGui::SameLine();
		if (ImGui::InputText("", str0, IM_ARRAYSIZE(str0)))
		{
			inputArg->varData = str0;

			variable->UpdateOutputData();
		}

		ImGui::PopItemWidth();
	}
	else
	{
		ImGui::SameLine();
		auto outputData = variable->GetOutputArg(0);
		ImGui::TextColored(ImVec4(1.0f, 0.4f, 0.4f, 1.0f), outputData->varData.c_str());
	}
}

void NFBluePrintView::PinRenderForElementVariable(NFNodePin* pin)
{
	int itemWidth = 80;

	auto variable = std::dynamic_pointer_cast<NFIVariable>(m_pBluePrintModule->FindNode(pin->nodeId));

	auto classNameArg = variable->GetInputArg(NFElementVariableInputArg::toString(NFElementVariableInputArg::ClassName));
	auto elementIDArg = variable->GetInputArg(NFElementVariableInputArg::toString(NFElementVariableInputArg::ConfigID));
	auto propertyNameArg = variable->GetInputArg(NFElementVariableInputArg::toString(NFElementVariableInputArg::PropName));

	ImGui::PushItemWidth(itemWidth);
	//class anme && element id && property name
	if (pin->name == NFElementVariableInputArg::toString(NFElementVariableInputArg::ClassName))
	{
		ImGui::SameLine();
		if (ImGui::BeginCombo("", classNameArg->varData.c_str()))
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
	}
	else if (pin->name == NFElementVariableInputArg::toString(NFElementVariableInputArg::ConfigID))
	{
		auto currentClassObject = m_pClassModule->GetElement(classNameArg->varData);
		if (currentClassObject)
		{

			auto idList = currentClassObject->GetIDList();

			ImGui::SameLine();
			if (ImGui::BeginCombo("", elementIDArg->varData.c_str()))
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

		}
	}
	else if (pin->name == NFElementVariableInputArg::toString(NFElementVariableInputArg::PropName))
	{
		auto currentClassObject = m_pClassModule->GetElement(classNameArg->varData);
		if (currentClassObject && !elementIDArg->varData.empty())
		{

			ImGui::SameLine();
			if (ImGui::BeginCombo("", propertyNameArg->varData.c_str()))
			{
				auto property = currentClassObject->GetPropertyManager()->First();
				while (property)
				{
					if (ImGui::Selectable(property->GetKey().c_str()))
					{
						propertyNameArg->varData = property->GetKey();

						switch (property->GetType())
						{
						case NFDATA_TYPE::TDATA_INT:
							propertyNameArg->valueType = NFValueType::Int;
							break;
						case NFDATA_TYPE::TDATA_FLOAT:
							propertyNameArg->valueType = NFValueType::Float;
							break;
						case NFDATA_TYPE::TDATA_STRING:
							propertyNameArg->valueType = NFValueType::String;
							break;
						case NFDATA_TYPE::TDATA_OBJECT:
							propertyNameArg->valueType = NFValueType::Object;
							break;
						case NFDATA_TYPE::TDATA_VECTOR2:
							propertyNameArg->valueType = NFValueType::Vector2;
							break;
						case NFDATA_TYPE::TDATA_VECTOR3:
							propertyNameArg->valueType = NFValueType::Vector3;
							break;
						default:
							break;
						}

						variable->UpdateOutputData();
					}

					property = currentClassObject->GetPropertyManager()->Next();
				}

				ImGui::EndCombo();
			}

		}
	}
	else
	{
		//output
		if (!pin->inputPin)
		{
			ImGui::SameLine();
			auto outputData = variable->GetOutputArg(0);
			ImGui::TextColored(ImVec4(1.0f, 0.4f, 0.4f, 1.0f), outputData->varData.c_str());
		}
	}

	ImGui::PopItemWidth();
}

void NFBluePrintView::PinRenderForPropertyVariable(NFNodePin* pin)
{
	int itemWidth = 80;

	auto variable = std::dynamic_pointer_cast<NFIVariable>(m_pBluePrintModule->FindNode(pin->nodeId));

	auto onwerID = variable->GetInputArg(NFPropertyVariableInputArg::toString(NFPropertyVariableInputArg::ObjectID));
	auto classNameArg = variable->GetInputArg(NFPropertyVariableInputArg::toString(NFPropertyVariableInputArg::ClassName));
	auto propertyNameArg = variable->GetInputArg(NFPropertyVariableInputArg::toString(NFPropertyVariableInputArg::PropName));

	ImGui::PushItemWidth(itemWidth);
	if (pin->inputPin)
	{
		if (pin->name == NFPropertyVariableInputArg::toString(NFPropertyVariableInputArg::ObjectID))
		{
		}
		else if (pin->name == NFPropertyVariableInputArg::toString(NFPropertyVariableInputArg::ClassName))
		{
			ImGui::SameLine();
			if (ImGui::BeginCombo("", classNameArg->varData.c_str()))
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
		}
		else if (pin->name == NFPropertyVariableInputArg::toString(NFPropertyVariableInputArg::PropName))
		{
			auto currentClassObject = m_pClassModule->GetElement(classNameArg->varData);
			if (currentClassObject)
			{
				ImGui::SameLine();
				if (ImGui::BeginCombo("", propertyNameArg->varData.c_str()))
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
			}
		}
	}
	else
	{
	}

	ImGui::PopItemWidth();
}

void NFBluePrintView::PinRenderForRecordVariable(NFNodePin* pin)
{
	int itemWidth = 80;

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
		ImGui::SameLine();

		if (ImGui::BeginCombo("", classNameArg->varData.c_str()))
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

			ImGui::SameLine();
			if (ImGui::BeginCombo("", recordNameArg->varData.c_str()))
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
	int itemWidth = 50;
	auto monitor = std::dynamic_pointer_cast<NFIMonitor>(m_pBluePrintModule->FindNode(pin->nodeId));

	static char str0[128] = "";

	if (pin->inputPin)
	{
		ImGui::PushItemWidth(itemWidth);

		if (pin->name == NFGameEventMonitorInputArg::toString(NFGameEventMonitorInputArg::EventID))
		{
			auto inputArg = monitor->GetInputArg(0);

			ImGui::SameLine();
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

void NFBluePrintView::PinRenderForNetworkEventMonitor(NFNodePin* pin)
{
	int itemWidth = 50;
	auto monitor = std::dynamic_pointer_cast<NFIMonitor>(m_pBluePrintModule->FindNode(pin->nodeId));

	static char str0[128] = "";

	if (pin->inputPin)
	{
		ImGui::PushItemWidth(itemWidth);

		if (pin->name == NFNetworkEventMonitorInputArg::toString(NFNetworkEventMonitorInputArg::EventID))
		{
			auto inputArg = monitor->GetInputArg(0);

			ImGui::SameLine();
			if (ImGui::InputText("", str0, IM_ARRAYSIZE(str0)))
			{
				inputArg->varData = str0;

				monitor->UpdateOutputData();
			}

		}
		ImGui::PopItemWidth();
	}
}

void NFBluePrintView::PinRenderForNetworkMsgMonitor(NFNodePin* pin)
{
	int itemWidth = 50;
	auto monitor = std::dynamic_pointer_cast<NFIMonitor>(m_pBluePrintModule->FindNode(pin->nodeId));

	static char str0[128] = "";

	if (pin->inputPin)
	{
		ImGui::PushItemWidth(itemWidth);

		if (pin->name == NFNetworkMsgMonitorInputArg::toString(NFNetworkMsgMonitorInputArg::NetMsgID))
		{
			auto inputArg = monitor->GetInputArg(0);

			ImGui::SameLine();
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
	int itemWidth = 80;
	auto monitor = std::dynamic_pointer_cast<NFIMonitor>(m_pBluePrintModule->FindNode(pin->nodeId));

	if (pin->inputPin)
	{
		if (pin->name == NFMonitorObjectEventInputArg::toString(NFMonitorObjectEventInputArg::ClassName))
		{

			ImGui::PushItemWidth(itemWidth);
			auto inputArg = monitor->GetInputArg(NFMonitorObjectEventInputArg::ClassName);

			ImGui::SameLine();
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
	int itemWidth = 80;
	auto monitor = std::dynamic_pointer_cast<NFIMonitor>(m_pBluePrintModule->FindNode(pin->nodeId));

	static char str0[128] = "";

	if (pin->inputPin)
	{

		auto classNameArg = monitor->GetInputArg(NFMonitorPropertyEventInputArg::ClassName);
		auto propertyNameArg = monitor->GetInputArg(NFMonitorPropertyEventInputArg::PropName);

		if (pin->name == NFMonitorPropertyEventInputArg::toString(NFMonitorPropertyEventInputArg::ClassName))
		{

			ImGui::PushItemWidth(itemWidth);

			ImGui::SameLine();
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
		else if(pin->name == NFMonitorPropertyEventInputArg::toString(NFMonitorPropertyEventInputArg::PropName))
		{
			if (!classNameArg->varData.empty())
			{
				auto currentClassObject = m_pClassModule->GetElement(classNameArg->varData);
				{

					ImGui::PushItemWidth(itemWidth);

					ImGui::SameLine();
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
	int itemWidth = 80;

	ImGui::PushItemWidth(itemWidth);
	auto branch = std::dynamic_pointer_cast<NFIBranch>(m_pBluePrintModule->FindNode(pin->nodeId));
	if (branch)
	{
		if (pin->name == NFBranchInputArg::toString(NFBranchInputArg::Comparator))
		{
			auto comparatorArg = branch->GetInputArg(NFBranchInputArg::Comparator);

			ImGui::SameLine();
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
		auto currentLogicBlock = m_pBluePrintModule->FindNode(mCurrentLogicBlockID);
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
						m_pBluePrintModule->AddMonitor(mCurrentLogicBlockID, monitorType, m_pKernelModule->CreateGUID(), str0);
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
		auto currentObject = m_pBluePrintModule->FindNode(mCurrentLogicBlockID);
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
					m_pBluePrintModule->AddBranch(mCurrentLogicBlockID, branchType, m_pKernelModule->CreateGUID(), str0);

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
		auto currentObject = m_pBluePrintModule->FindNode(mCurrentLogicBlockID);
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
					m_pBluePrintModule->AddExecuter(mCurrentLogicBlockID, executerType, m_pKernelModule->CreateGUID(), str0);

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
		auto currentObject = m_pBluePrintModule->FindNode(mCurrentLogicBlockID);
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

					m_pBluePrintModule->AddModifier(mCurrentLogicBlockID, modifierType, m_pKernelModule->CreateGUID(), str0);

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
		auto currentObject = m_pBluePrintModule->FindNode(mCurrentLogicBlockID);
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
					
					m_pBluePrintModule->AddVariable(mCurrentLogicBlockID, valueType, m_pKernelModule->CreateGUID(), str0);

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
		auto currentObject = m_pBluePrintModule->FindNode(mCurrentLogicBlockID);
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

					m_pBluePrintModule->AddArithmetic(mCurrentLogicBlockID, arithmeticType, m_pKernelModule->CreateGUID(), str0);

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

NFColor NFBluePrintView::GetBackGroundColor(NF_SHARE_PTR<NFBluePrintNodeBase> node)
{
	NFColor color = NFColor::DEFAULT;
	switch (node->blueprintType)
	{
	case NFBlueprintType::MONITOR:
		color = NFColor::MONITOR;
		break;
	case NFBlueprintType::EXECUTER:
		color = NFColor::EXECUTER;
		break;
	case NFBlueprintType::VARIABLE:
		color = NFColor::VARIABLE;
		break;
	case NFBlueprintType::MODIFIER:
		color = NFColor::MODIFIER;
		break;
	case NFBlueprintType::ARITHMETIC:
		color = NFColor::ARITHMETIC;
		break;
	case NFBlueprintType::LOGGER:
		color = NFColor::LOGGER;
		break;
	case NFBlueprintType::BRANCH:
		color = NFColor::BRANCH;
		break;
	
	default:
		break;
	}

	return color;
}