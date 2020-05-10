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

//#if NF_PLATFORM == NF_PLATFORM_WIN

char* variableNodeImage = "..//NFDataCfg//Fonts//Icon//Variable//node.png";
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

char* nodeMonitorImage = "..//NFDataCfg//Fonts//Icon//Node//monitor.png";
char* nodeBranchImage = "..//NFDataCfg//Fonts//Icon//Node//branch.png";
char* nodeExecutorImage = "..//NFDataCfg//Fonts//Icon//Node//executer.png";
char* nodeVariableImage = "";
char* nodeModifierImage = "..//NFDataCfg//Fonts//Icon//Node//modifier.png";
char* nodeArithmeticImage = "..//NFDataCfg//Fonts//Icon//Node//arithmetic.png";
char* nodeDebugerImage = "..//NFDataCfg//Fonts//Icon//Node//debug.png";
char* nodeCustomImage = "..//NFDataCfg//Fonts//Icon//Node//custom.png";
char* nodeUnknowImage = "..//NFDataCfg//Fonts//Icon//Node//unknow.png";

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
	int DEFAULT = IM_COL32(122, 182, 228, 255);
	int WORKFLOW = IM_COL32(220, 220, 93, 255);
	//int LINK = IM_COL32(175, 175, 175, 255);
	int PININ = IM_COL32(122, 182, 228, 255);
	int PININ_GOOD = IM_COL32(150, 193, 60, 255);
	int PINOUT = IM_COL32(175, 175, 175, 255);

	int EXECUTER = IM_COL32(211, 188, 73, 255);
	int VARIABLE = IM_COL32(200, 141, 206, 255);
	int MODIFIER = IM_COL32(102, 186, 75, 255);
	int BRANCH = IM_COL32(46, 96, 50, 255);
	int MONITOR = IM_COL32(140, 210, 190, 255);
	int DEBUGER = IM_COL32(242, 165, 91, 255);
	int ARITHMETIC = IM_COL32(78, 80, 84, 255);
	int CUSTOM = IM_COL32(104, 176, 224, 255);


    mNodeView.ResetOffset(NFVector2::Zero());

   SmartBind<LINK_EVENT_FUNCTOR> linkBind;
   auto bindTest1 = std::bind(&NFBluePrintView::TryNewLinkEvent, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
   auto bindTest2 = std::bind(&NFBluePrintView::TryNewLinkEvent, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);

   linkBind.t = bindTest1;
   //hash value
   std::cout << linkBind.name << std::endl;

   mNodeView.SetUpNewLinkCallBack(std::bind(&NFBluePrintView::TryNewLinkEvent, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));
   mNodeView.SetUpDeleteLinkCallBack(std::bind(&NFBluePrintView::TryDeleteLinkEvent, this, std::placeholders::_1));
   
   mNodeView.SetPinRenderCallBack(std::bind(&NFBluePrintView::PinRender, this, std::placeholders::_1));

   mNodeView.SetNodeRenderBeforePinInCallBack(std::bind(&NFBluePrintView::NodeRenderBeforePinIn, this, std::placeholders::_1));
   mNodeView.SetNodeRenderAfterPinInCallBack(std::bind(&NFBluePrintView::NodeRenderAfterPinIn, this, std::placeholders::_1));
   mNodeView.SetNodeRenderBeforePinOutCallBack(std::bind(&NFBluePrintView::NodeRenderBeforePinOut, this, std::placeholders::_1));
   mNodeView.SetNodeRenderAfterPinOutCallBack(std::bind(&NFBluePrintView::NodeRenderAfterPinOut, this, std::placeholders::_1));




   mTreeView.SetSelectedNodeFunctor(std::bind(&NFBluePrintView::HandlerSelected, this, std::placeholders::_1, std::placeholders::_2));
   mTreeView.SetName(GET_CLASS_NAME(NFBluePrintView));

   m_pUIModule = pPluginManager->FindModule<NFIUIModule>();
   m_pBluePrintModule = pPluginManager->FindModule<NFIBluePrintModule>();
   m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
   m_pClassModule = pPluginManager->FindModule<NFIClassModule>();
	m_pElementModule = pPluginManager->FindModule<NFIElementModule>();
	m_pLogModule = pPluginManager->FindModule<NFILogModule>();

   m_pBluePrintModule->SetNodeModifyEventFunctor(std::bind(&NFBluePrintView::NodeModifyEvent, this, std::placeholders::_1, std::placeholders::_2));
   m_pBluePrintModule->SetLinkModifyEventFunctor(std::bind(&NFBluePrintView::LinkModifyEvent, this, std::placeholders::_1, std::placeholders::_2));
}

NFBluePrintView::~NFBluePrintView()
{
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
        mNodeView.ResetOffset(NFVector2::Zero());
	}

   mNodeView.Execute();
   
   CreateLogicBlock();
   CreateMonitor();
   CreateBranch();
   CreateExecutor();
   CreateModifier();
   CreateVariable();
	CreateDebugger();
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

	mNodeView.MoveToNode(mCurrentObjectID);
}

NFGUID NFBluePrintView::GetCurrentLogicBlockID()
{
	return mCurrentLogicBlockID;
}

void NFBluePrintView::SetCurrentLogicBlockID(const NFGUID& id)
{
	if (id == mCurrentLogicBlockID)
	{
		return;
	}

	mCurrentLogicBlockID = id;

	mNodeView.CleanNodes();

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

const NFTreeView& NFBluePrintView::GetTreeView()
{
	return mTreeView;
}

void NFBluePrintView::SubRender()
{
	//ImGui::GetIO().KeyCtrl)
   mTreeView.Execute();
}

void NFBluePrintView::HandlerSelected(const NFGUID& id, const bool doubleClick)
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
			mTreeView.AddTreeNode(node->id, node->name);
		}
		else
		{
			mTreeView.AddSubTreeNode(node->logicBlockId, node->id, node->name);

			AddNode(node);
		}
	}
	else
	{
		mTreeView.DeleteTreeNode(id);
		mNodeView.DeleteNode(id);
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
						color = GetPinColor(inputAgr->GetValueType());
						mNodeView.ModifyPinColor(inputAgr->id, color);

						break;
					}
				}
				
			}

			mNodeView.AddLink(id, linkData->startNode, linkData->endNode, linkData->startAttr, linkData->endAttr, color);
		}
		else
		{
			mNodeView.DeleteLink(linkData->startNode, linkData->endNode, linkData->startAttr, linkData->endAttr);
		}
	}
}

void NFBluePrintView::AddNode(NF_SHARE_PTR<NFBluePrintNodeBase> node)
{
	if (node->blueprintType != NFBlueprintType::LOGICBLOCK)
	{
		NFColor color = GetBackGroundColor(node->blueprintType);

		auto nodeUI = mNodeView.AddNode(node->id, node->name, color, NFVector2(0, 0));
		nodeUI->iconPath = GetNodeIcon(node->blueprintType);

		for (int i = 0; i < node->GetInputArgCount(); ++i)
		{
			auto variableArg = node->GetInputArg(i);
			NFColor pinColor = NFColor::DEFAULT;
			if (variableArg->GetValueType() == NFValueType::Node)
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

			std::string imageName = GetPinIcon(variableArg->GetValueType());
			NFPinShape shape = GetPinShape(variableArg->GetValueType(), variableArg->fromType);
			
			mNodeView.AddPinIn(node->id, variableArg->id, variableArg->name, imageName, pinColor, shape);
		}

		for (int i = 0; i < node->GetOutputArgCount(); ++i)
		{
			auto variableArg = node->GetOutputArg(i);
			NFColor pinColor = NFColor::DEFAULT;

			if (variableArg->GetValueType() == NFValueType::Node)
			{
				pinColor = NFColor::WORKFLOW;
			}
			else
			{
				pinColor = NFColor::PINOUT;
			}

			std::string imageName = GetPinIcon(variableArg->GetValueType());
			NFPinShape shape = GetPinShape(variableArg->GetValueType(), variableArg->fromType);

			mNodeView.AddPinOut(node->id, variableArg->id, variableArg->name, imageName, pinColor, shape);
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
				for (int j = 0; j < endNodeObject->GetInputArgCount(); ++j)
				{
					auto inputArg = endNodeObject->GetInputArg(j);
					if (inputArg->id == endPin)
					{
						if (inputArg->fromType != NFIODataComFromType::INTERNAL)
						{
							if (inputArg->GetValueType() == outputArg->GetValueType())
							{
								//if linked
								if (!inputArg->GetLinkID().IsNull())
								{
									//show a error message
									return false;
								}

								if (outputArg->GetValueType() == NFValueType::Node
									&& !outputArg->GetLinkID().IsNull())
								{
									//show a error message
									return false;
								}

								if (inputArg->GetValueType() == NFValueType::UNKNOW
									|| outputArg->GetValueType() == NFValueType::UNKNOW)
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
		mNodeView.DeleteLink(id);
	}

	return true;
}

void NFBluePrintView::PinRender(NFNodePin* pin)
{
	NFGUID nodeID = mNodeView.GetNodeByAttriId(pin->nodeId);
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
		case NFBlueprintType::ARITHMETIC:
			PinRenderForArithmetic(pin);
			break;
		case NFBlueprintType::DEBUGER:
			PinRenderForDebugger(pin);
			break;
		case NFBlueprintType::EXECUTER:
			PinRenderForExecutor(pin);
			break;
		default:
			break;
		}

		auto inputArg = node->GetInputArg(pin->name);
		auto outputArg = node->GetOutputArg(pin->name);
		if (inputArg)
		{
			pin->newImage = GetPinIcon(inputArg->GetValueType());

			NFGUID linkID = inputArg->GetLinkID();
			if (!m_pBluePrintModule->CheckLink(linkID))
			{
				m_pBluePrintModule->DeleteLink(linkID);
			}
		}
		if (outputArg)
		{
			pin->newImage = GetPinIcon(outputArg->GetValueType());

			NFGUID linkID = outputArg->GetLinkID();
			if (!m_pBluePrintModule->CheckLink(linkID))
			{
				m_pBluePrintModule->DeleteLink(linkID);
			}
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
		case NFVariableType::BasicVariable:
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


	if (pin->inputPin)
	{
		ImGui::PushItemWidth(itemWidth);

		auto inputArg = variable->GetInputArg(0);
		auto outputArg = variable->GetOutputArg(0);

		ImGui::SameLine();
		if (ImGui::BeginCombo(" ", inputArg->GetValueType().toString().c_str()))
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
					inputArg->SetValueType(x);
					outputArg->SetValueType(x);
					variable->UpdateOutputData(NFGUID());
				}
			}
			ImGui::EndCombo();
		}

		ImGui::SameLine();

		std::string argData = inputArg->ToString();
		char str[128] = "";
		strcpy(str, argData.c_str());

		if (ImGui::InputText("", str, IM_ARRAYSIZE(str)))
		{
			inputArg->FromString(str);

			variable->UpdateOutputData(NFGUID());
		}

		ImGui::PopItemWidth();
	}
	else
	{
		ImGui::SameLine();
		auto outputData = variable->GetOutputArg(0);
		ImGui::TextColored(ImVec4(1.0f, 0.4f, 0.4f, 1.0f), outputData->GetString().c_str());
	}
}

void NFBluePrintView::PinRenderForElementVariable(NFNodePin* pin)
{
	int itemWidth = 80;

	auto variable = std::dynamic_pointer_cast<NFIVariable>(m_pBluePrintModule->FindNode(pin->nodeId));

	auto classNameArg = variable->GetInputArg(NFElementVariableInputArg::ClassName);
	auto elementIDArg = variable->GetInputArg(NFElementVariableInputArg::ConfigID);
	auto propertyNameArg = variable->GetInputArg(NFElementVariableInputArg::PropName);
	auto propertyValueArg = variable->GetOutputArg(NFElementVariableOutputArg::PropValue);

	ImGui::PushItemWidth(itemWidth);
	if (pin->inputPin)
	{
		if (pin->name == NFElementVariableInputArg::toString(NFElementVariableInputArg::ClassName))
		{
			ImGui::SameLine();
			if (ImGui::BeginCombo("", classNameArg->GetString().c_str()))
			{
				auto classObject = m_pClassModule->First();
				while (classObject)
				{
					if (classObject->GetIDList().size() > 0)
					{
						if (ImGui::Selectable(classObject->GetClassName().c_str()))
						{
							classNameArg->SetString(classObject->GetClassName());
							elementIDArg->SetString("");
							propertyNameArg->SetString("");
						}
					}

					classObject = m_pClassModule->Next();
				}

				ImGui::EndCombo();
			}
		}
		else if (pin->name == NFElementVariableInputArg::toString(NFElementVariableInputArg::ConfigID))
		{
			auto currentClassObject = m_pClassModule->GetElement(classNameArg->GetString());
			if (currentClassObject)
			{

				auto idList = currentClassObject->GetIDList();

				ImGui::SameLine();
				if (ImGui::BeginCombo("", elementIDArg->GetString().c_str()))
				{
					for (auto id : idList)
					{
						if (ImGui::Selectable(id.c_str()))
						{
							elementIDArg->SetString(id);
							variable->UpdateOutputData(NFGUID());
						}
					}

					ImGui::EndCombo();
				}

			}
		}
		else if (pin->name == NFElementVariableInputArg::toString(NFElementVariableInputArg::PropName))
		{
			auto currentClassObject = m_pClassModule->GetElement(classNameArg->GetString());
			if (currentClassObject && !elementIDArg->GetString().empty())
			{

				ImGui::SameLine();
				if (ImGui::BeginCombo("", propertyNameArg->GetString().c_str()))
				{
					auto property = currentClassObject->GetPropertyManager()->First();
					while (property)
					{
						if (ImGui::Selectable(property->GetKey().c_str()))
						{
							propertyNameArg->SetString(property->GetKey());

							variable->UpdateOutputData(NFGUID());
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
		//ImGui::SameLine();
		//auto outputData = variable->GetOutputArg(0);
		//ImGui::TextColored(ImVec4(1.0f, 0.4f, 0.4f, 1.0f), outputData->GetString().c_str());
	}

	ImGui::PopItemWidth();
}

void NFBluePrintView::PinRenderForPropertyVariable(NFNodePin* pin)
{
	int itemWidth = 80;

	auto variable = std::dynamic_pointer_cast<NFIVariable>(m_pBluePrintModule->FindNode(pin->nodeId));

	auto onwerID = variable->GetInputArg(NFPropertyVariableInputArg::ObjectID);
	auto classNameArg = variable->GetInputArg(NFPropertyVariableInputArg::ClassName);
	auto propertyNameArg = variable->GetInputArg(NFPropertyVariableInputArg::PropName);

	ImGui::PushItemWidth(itemWidth);
	if (pin->inputPin)
	{
		if (pin->name == NFPropertyVariableInputArg::toString(NFPropertyVariableInputArg::ObjectID))
		{
		}
		else if (pin->name == NFPropertyVariableInputArg::toString(NFPropertyVariableInputArg::ClassName))
		{
			ImGui::SameLine();
			if (ImGui::BeginCombo("", classNameArg->GetString().c_str()))
			{
				auto classObject = m_pClassModule->First();
				while (classObject)
				{
					if (ImGui::Selectable(classObject->GetClassName().c_str()))
					{
						auto outputArg = variable->GetOutputArg(0);

						classNameArg->SetString(classObject->GetClassName());
						propertyNameArg->SetString("");
						propertyNameArg->SetValueType(NFValueType::UNKNOW);
						variable->UpdateOutputData(NFGUID());
					}

					classObject = m_pClassModule->Next();
				}

				ImGui::EndCombo();
			}
		}
		else if (pin->name == NFPropertyVariableInputArg::toString(NFPropertyVariableInputArg::PropName))
		{
			auto currentClassObject = m_pClassModule->GetElement(classNameArg->GetString());
			if (currentClassObject)
			{
				ImGui::SameLine();
				if (ImGui::BeginCombo("", propertyNameArg->GetString().c_str()))
				{
					auto property = currentClassObject->GetPropertyManager()->First();
					while (property)
					{
						if (ImGui::Selectable(property->GetKey().c_str()))
						{
							propertyNameArg->SetString(property->GetKey());

							variable->UpdateOutputData(NFGUID());
						}

						property = currentClassObject->GetPropertyManager()->Next();
					}

					ImGui::EndCombo();
				}
			}
		}
	}

	ImGui::PopItemWidth();
}

void NFBluePrintView::PinRenderForRecordVariable(NFNodePin* pin)
{
	int itemWidth = 80;

	auto variable = std::dynamic_pointer_cast<NFIVariable>(m_pBluePrintModule->FindNode(pin->nodeId));

	auto onwerID = variable->GetInputArg(NFRecordVariableInputArg::ObjectID);
	auto classNameArg = variable->GetInputArg(NFRecordVariableInputArg::ClassName);
	auto recordNameArg = variable->GetInputArg(NFRecordVariableInputArg::RecordName);
	auto recordRowArg = variable->GetInputArg(NFRecordVariableInputArg::RecordRow);
	auto recordColArg = variable->GetInputArg(NFRecordVariableInputArg::RecordCol);

	/*
	ImGui::PushItemWidth(itemWidth);

	if (ImGui::BeginCombo("ValueType", variable->GetValueType().toString().c_str()))
	{
		for (auto x : NFValueType::allValues())
		{
			if (ImGui::Selectable(x.toString().c_str(), false))
			{
				variable->SetValueType(x);
			}
		}
		ImGui::EndCombo();
	}
	ImGui::PopItemWidth();
	*/

	if (pin->name == NFRecordVariableInputArg::toString(NFRecordVariableInputArg::ObjectID))
	{
		ImGui::Button(onwerID->GetString().c_str());
	}
	else if (pin->name == NFRecordVariableInputArg::toString(NFRecordVariableInputArg::ClassName))
	{
		ImGui::SameLine();

		if (ImGui::BeginCombo("", classNameArg->GetString().c_str()))
		{
			auto classObject = m_pClassModule->First();
			while (classObject)
			{
				if (ImGui::Selectable(classObject->GetClassName().c_str()))
				{
					classNameArg->SetString(classObject->GetClassName());
					recordNameArg->SetString("");
				}

				classObject = m_pClassModule->Next();
			}

			ImGui::EndCombo();
		}

		ImGui::PopItemWidth();
	}
	else if (pin->name == NFRecordVariableInputArg::toString(NFRecordVariableInputArg::RecordName))
	{
		auto currentClassObject = m_pClassModule->GetElement(classNameArg->GetString());
		if (currentClassObject)
		{
			//for all properties
			NFDATA_TYPE dataType = NFDATA_TYPE::TDATA_UNKNOWN;
			switch (recordNameArg->GetValueType())
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
			if (ImGui::BeginCombo("", recordNameArg->GetString().c_str()))
			{
				auto property = currentClassObject->GetPropertyManager()->First();
				while (property)
				{
					if (property->GetType() == dataType)
					{
						if (ImGui::Selectable(property->GetKey().c_str()))
						{
							recordNameArg->SetString(property->GetKey());
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
		default:
			break;
		}
	}
}

void NFBluePrintView::PinRenderForGameEventMonitor(NFNodePin* pin)
{
	int itemWidth = 50;
	auto monitor = std::dynamic_pointer_cast<NFIMonitor>(m_pBluePrintModule->FindNode(pin->nodeId));
	if (pin->inputPin)
	{
		ImGui::PushItemWidth(itemWidth);

		if (pin->name == NFGameEventMonitorInputArg::toString(NFGameEventMonitorInputArg::EventID))
		{
			auto inputArg = monitor->GetInputArg(NFGameEventMonitorInputArg::EventID);

			ImGui::SameLine();

			int eventID = inputArg->GetInt();
			if (ImGui::InputInt("", &eventID))
			{
				inputArg->SetInt(eventID);

				monitor->UpdateOutputData(NFGUID());
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

	if (pin->inputPin)
	{
		ImGui::PushItemWidth(itemWidth);

		if (pin->name == NFNetworkEventMonitorInputArg::toString(NFNetworkEventMonitorInputArg::EventID))
		{
			auto inputArg = monitor->GetInputArg(NFNetworkEventMonitorInputArg::EventID);

			ImGui::SameLine();

			int eventID = inputArg->GetInt();
			if (ImGui::InputInt("", &eventID))
			{
				inputArg->SetInt(eventID);

				monitor->UpdateOutputData(NFGUID());
			}

		}
		ImGui::PopItemWidth();
	}
}

void NFBluePrintView::PinRenderForNetworkMsgMonitor(NFNodePin* pin)
{
	int itemWidth = 50;
	auto monitor = std::dynamic_pointer_cast<NFIMonitor>(m_pBluePrintModule->FindNode(pin->nodeId));

	if (pin->inputPin)
	{
		ImGui::PushItemWidth(itemWidth);

		if (pin->name == NFNetworkMsgMonitorInputArg::toString(NFNetworkMsgMonitorInputArg::NetMsgID))
		{
			auto inputArg = monitor->GetInputArg(NFNetworkMsgMonitorInputArg::NetMsgID);

			ImGui::SameLine();

			int eventID = inputArg->GetInt();
			if (ImGui::InputInt("", &eventID))
			{
				inputArg->SetInt(eventID);

				monitor->UpdateOutputData(NFGUID());
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
			if (ImGui::BeginCombo("", inputArg->GetString().c_str()))
			{
				auto classObject = m_pClassModule->First();
				while (classObject)
				{
					if (classObject->GetIDList().size() > 0)
					{
						if (ImGui::Selectable(classObject->GetClassName().c_str()))
						{
							inputArg->SetString(classObject->GetClassName());
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
			ImGui::PushItemWidth(itemWidth);
			auto inputArg = monitor->GetInputArg(NFMonitorObjectEventInputArg::ClassEvent);

			ImGui::SameLine();
			NFClassEventType lastEventType = inputArg->GetInt();
			if (ImGui::BeginCombo("", lastEventType.toString().c_str()))
			{
				auto classEvents = NFClassEventType::allValues();
				for (int i = 0; i < classEvents.size(); ++i)
				{
					if (ImGui::Selectable(classEvents[i].toString().c_str()))
					{
						inputArg->SetInt(classEvents[i]);
					}
				}

				ImGui::EndCombo();
			}

			ImGui::PopItemWidth();
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
			if (ImGui::BeginCombo("", classNameArg->GetString().c_str()))
			{
				auto classObject = m_pClassModule->First();
				while (classObject)
				{
					if (ImGui::Selectable(classObject->GetClassName().c_str()))
					{
						classNameArg->SetString(classObject->GetClassName());
					}

					classObject = m_pClassModule->Next();
				}

				ImGui::EndCombo();
			}

			ImGui::PopItemWidth();
		}
		else if(pin->name == NFMonitorPropertyEventInputArg::toString(NFMonitorPropertyEventInputArg::PropName))
		{
			if (!classNameArg->GetString().empty())
			{
				auto currentClassObject = m_pClassModule->GetElement(classNameArg->GetString());
				{
					ImGui::PushItemWidth(itemWidth);

					ImGui::SameLine();
					if (ImGui::BeginCombo("", propertyNameArg->GetString().c_str()))
					{
						auto property = currentClassObject->GetPropertyManager()->First();
						while (property)
						{
							{
								if (ImGui::Selectable(property->GetKey().c_str()))
								{
									propertyNameArg->SetString(property->GetKey());
									monitor->UpdateOutputData(NFGUID());
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
			if (ImGui::BeginCombo("", (comparatorArg->GetString().c_str())))
			{
				for (auto x : NFComparatorType::allValues())
				{
					if (ImGui::Selectable(x.toString().c_str(), false))
					{
						comparatorArg->SetString(x.toString());
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
	auto modifier = std::dynamic_pointer_cast<NFIModifier>(m_pBluePrintModule->FindNode(pin->nodeId));
	if (modifier)
	{
		switch (modifier->modifierType)
		{
		case NFModifierType::SetProperty:
		{
			PinRenderForPropertyModifier(pin);
		}
		break;
		case NFModifierType::SetRecord:
		{
			PinRenderForRecordModifier(pin);
		}
		break;
		case NFModifierType::AddRecordRow:
		{
			PinRenderForRecordAddModifier(pin);
		}
		break;
		case NFModifierType::RemRecordRow:
		{
			PinRenderForRecordRemModifier(pin);
		}
		break;
		default:
			break;
		}
	}
}

void NFBluePrintView::PinRenderForPropertyModifier(NFNodePin* pin)
{
	int itemWidth = 80;

	auto variable = std::dynamic_pointer_cast<NFIModifier>(m_pBluePrintModule->FindNode(pin->nodeId));

	auto ownerID = variable->GetInputArg(NFPropertyModifierInputArg::ObjectID);
	auto classNameArg = variable->GetInputArg(NFPropertyModifierInputArg::ClassName);
	auto propertyNameArg = variable->GetInputArg(NFPropertyModifierInputArg::PropName);
	auto propertyValueArg = variable->GetInputArg(NFPropertyModifierInputArg::PropValue);

	ImGui::PushItemWidth(itemWidth);
	if (pin->inputPin)
	{
		if (pin->name == NFPropertyModifierInputArg::toString(NFPropertyModifierInputArg::ObjectID))
		{
		}
		else if (pin->name == NFPropertyModifierInputArg::toString(NFPropertyModifierInputArg::ClassName))
		{
			ImGui::SameLine();
			if (ImGui::BeginCombo("", classNameArg->GetString().c_str()))
			{
				auto classObject = m_pClassModule->First();
				while (classObject)
				{
					if (ImGui::Selectable(classObject->GetClassName().c_str()))
					{
						classNameArg->SetString(classObject->GetClassName());
						propertyNameArg->SetValueType(NFValueType::UNKNOW);
						variable->UpdateOutputData(NFGUID());
					}

					classObject = m_pClassModule->Next();
				}

				ImGui::EndCombo();
			}
		}
		else if (pin->name == NFPropertyModifierInputArg::toString(NFPropertyModifierInputArg::PropName))
		{
			auto currentClassObject = m_pClassModule->GetElement(classNameArg->GetString());
			if (currentClassObject)
			{
				ImGui::SameLine();
				if (ImGui::BeginCombo("", propertyNameArg->GetString().c_str()))
				{
					auto property = currentClassObject->GetPropertyManager()->First();
					while (property)
					{
						if (ImGui::Selectable(property->GetKey().c_str()))
						{
							propertyNameArg->SetString(property->GetKey());
							switch (property->GetType())
							{
								case NFDATA_TYPE::TDATA_INT:
								{
									propertyValueArg->SetValueType(NFValueType::Int);
								}
									break;
								case NFDATA_TYPE::TDATA_FLOAT:
								{
									propertyValueArg->SetValueType(NFValueType::Float);
								}
									break;
								case NFDATA_TYPE::TDATA_OBJECT:
								{
									propertyValueArg->SetValueType(NFValueType::Object);
								}
									break;
								case NFDATA_TYPE::TDATA_STRING:
								{
									propertyValueArg->SetValueType(NFValueType::String);
								}
									break;
								case NFDATA_TYPE::TDATA_VECTOR2:
								{
									propertyValueArg->SetValueType(NFValueType::Vector2);
								}
									break;
								case NFDATA_TYPE::TDATA_VECTOR3:
								{
									propertyValueArg->SetValueType(NFValueType::Vector3);
								}
									break;
								default:
									break;
							}

							variable->UpdateOutputData(NFGUID());
						}

						property = currentClassObject->GetPropertyManager()->Next();
					}

					ImGui::EndCombo();
				}
			}
		}
	}

	ImGui::PopItemWidth();
}

void NFBluePrintView::PinRenderForRecordModifier(NFNodePin* pin)
{

}

void NFBluePrintView::PinRenderForRecordAddModifier(NFNodePin* pin)
{

}

void NFBluePrintView::PinRenderForRecordRemModifier(NFNodePin* pin)
{

}

void NFBluePrintView::PinRenderForArithmetic(NFNodePin* pin)
{
	int itemWidth = 80;

	ImGui::PushItemWidth(itemWidth);
	auto arithmetic = std::dynamic_pointer_cast<NFIArithmetic>(m_pBluePrintModule->FindNode(pin->nodeId));
	if (arithmetic)
	{
		if (pin->name == NFArithmeticInputArg::toString(NFArithmeticInputArg::ArithmeticType))
		{
			auto comparatorArg = arithmetic->GetInputArg(NFArithmeticInputArg::ArithmeticType);
			ImGui::SameLine();

			NFArithmeticType type = (NFArithmeticType)comparatorArg->GetInt();

			if (ImGui::BeginCombo("", type.toString().c_str()))
			{
				for (auto x : NFArithmeticType::allValues())
				{
					if (ImGui::Selectable(x.toString().c_str(), false))
					{
						comparatorArg->SetInt(x);
					}
				}
				ImGui::EndCombo();
			}
		}
		else if (pin->name == NFArithmeticInputArg::toString(NFArithmeticInputArg::ValueType))
		{
			auto comparatorArg = arithmetic->GetInputArg(NFArithmeticInputArg::ValueType);

			auto leftData = arithmetic->GetInputArg(NFArithmeticInputArg::LeftInput);
			auto rightData = arithmetic->GetInputArg(NFArithmeticInputArg::RightInput);

			auto outData = arithmetic->GetOutputArg(NFArithmeticOutputArg::Output);

			NFValueType nowType = comparatorArg->GetInt();

			ImGui::SameLine();
			if (ImGui::BeginCombo("", nowType.toString().c_str()))
			{
				for (auto x : NFValueType::allValues())
				{
					if (x == NFValueType::Int
						|| x == NFValueType::Float
					   || x == NFValueType::String
					   || x == NFValueType::Object
					   || x == NFValueType::Vector2
					   || x == NFValueType::Vector3)
					{
						if (ImGui::Selectable(x.toString().c_str(), false))
						{
							comparatorArg->SetInt(x);

							leftData->SetValueType(x);
							rightData->SetValueType(x);

							outData->SetValueType(x);
						}
					}
				}

				ImGui::EndCombo();
			}
		}
	}

	ImGui::PopItemWidth();
}

void NFBluePrintView::PinRenderForDebugger(NFNodePin* pin)
{
	int itemWidth = 80;

	ImGui::PushItemWidth(itemWidth);
	auto debugger = std::dynamic_pointer_cast<NFIDebugger>(m_pBluePrintModule->FindNode(pin->nodeId));
	if (debugger)
	{
		if (pin->name == NFDebuggerInputArg::toString(NFDebuggerInputArg::LogLevel))
		{
			auto logLevel = debugger->GetInputArg(NFDebuggerInputArg::LogLevel);

			NFDebuggerLevel lvl = logLevel->GetInt();

			ImGui::SameLine();
			if (ImGui::BeginCombo("", lvl.toString().c_str()))
			{
				for (auto x : NFDebuggerLevel::allValues())
				{
					if (ImGui::Selectable(x.toString().c_str(), false))
					{
						logLevel->SetInt(x);
					}
				}
				ImGui::EndCombo();
			}
		}
		else if (pin->name == NFDebuggerInputArg::toString(NFDebuggerInputArg::LogData))
		{
			ImGui::SameLine();

			static char str0[128] = "";
			if (ImGui::InputText("", str0, IM_ARRAYSIZE(str0)))
			{
				auto logData = debugger->GetInputArg(NFDebuggerInputArg::LogData);
				logData->SetString(str0);
			}
		}
	}

	ImGui::PopItemWidth();
}

void NFBluePrintView::PinRenderForExecutor(NFNodePin* pin)
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

void NFBluePrintView::TryToCreateExecutor(NFExecutorType type)
{
	if (!bCreatingExecutor)
	{
		bCreatingExecutor = true;
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

void NFBluePrintView::TryToCreateDebuger()
{
	if (!bCreatingDebuger)
	{
		bCreatingDebuger = true;
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

void NFBluePrintView::CreateExecutor()
{
	if (bCreatingExecutor)
	{
		auto currentObject = m_pBluePrintModule->FindNode(mCurrentLogicBlockID);
		if (currentObject)
		{
			ImGui::OpenPopup("Creating Executor");
			ImGui::SetNextWindowSize(ImVec2(230, 150));

			ImGuiWindowFlags window_flags = 0;
			window_flags |= ImGuiWindowFlags_NoResize;
			bool open = true;
			if (ImGui::BeginPopupModal("Creating Executor"), &open, window_flags)
			{
				static char str0[128] = "Hello, Executor!";
				memset(str0, 0, IM_ARRAYSIZE(str0));
				std::string valueTypeData = NFExecutorType::toString(executerType);
				memcpy(str0, valueTypeData.c_str(), valueTypeData.length());

				ImGui::InputText("Executor Name", str0, IM_ARRAYSIZE(str0));

				ImGui::Separator();

				ImGui::Text("All those beautiful files will be deleted.\nThis operation cannot be undone!\n\n");

				ImGui::Separator();

				if (ImGui::Button("Cancel", ImVec2(100, 30)))
				{
					bCreatingExecutor = false;
					ImGui::CloseCurrentPopup();
				}

				ImGui::SameLine();

				if (ImGui::Button("OK", ImVec2(100, 30)))
				{
					m_pBluePrintModule->AddExecutor(mCurrentLogicBlockID, executerType, m_pKernelModule->CreateGUID(), str0);

					bCreatingExecutor = false;
					ImGui::CloseCurrentPopup();
				}

				ImGui::EndPopup();
			}
		
		}
		else
		{
			bCreatingExecutor = false;
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
				std::string valueTypeData = "NFArithmeticType";//NFArithmeticType::toString(arithmeticType);
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

void NFBluePrintView::CreateDebugger()
{
	if (bCreatingDebuger)
	{
		m_pBluePrintModule->AddDebugger(mCurrentLogicBlockID, m_pKernelModule->CreateGUID(), "Debugger");

		bCreatingDebuger = false;
	}
}

NFColor NFBluePrintView::GetBackGroundColor(NFBlueprintType type)
{
	NFColor color = NFColor::DEFAULT;
	switch (type)
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
	case NFBlueprintType::DEBUGER:
		color = NFColor::DEBUGER;
		break;
	case NFBlueprintType::BRANCH:
		color = NFColor::BRANCH;
		break;
	default:
		break;
	}

	return color;
}

std::string NFBluePrintView::GetPinIcon(NFValueType type)
{
	std::string imageName = variableDefaultImage;
	switch (type)
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
	case NFValueType::Node:
		imageName = variableNodeImage;
		break;
	default:
		break;
	}

	return imageName;
}

NFPinShape NFBluePrintView::GetPinShape(NFValueType type, NFIODataComFromType comeFromType)
{
	if (comeFromType == NFIODataComFromType::INTERNAL)
	{
		return NFPinShape::PinShape_NONE;
	}

	NFPinShape shape;
	switch (type)
	{
	case NFValueType::UNKNOW:
		shape = NFPinShape::PinShape_Quad;
		break;
	case NFValueType::Node:
		shape = NFPinShape::PinShape_Triangle;
		break;
	default:
		shape = NFPinShape::PinShape_Circle;
		break;
	}

	return shape;
}

std::string NFBluePrintView::GetNodeIcon(NFBlueprintType type)
{
	std::string imageName = nodeUnknowImage;
	switch (type)
	{
		case NFBlueprintType::MONITOR:
			imageName = nodeMonitorImage;
			break;
		case NFBlueprintType::BRANCH:
			imageName = nodeBranchImage;
			break;
		case NFBlueprintType::EXECUTER:
			imageName = nodeExecutorImage;
			break;
		case NFBlueprintType::VARIABLE:
			imageName = nodeVariableImage;
			break;
		case NFBlueprintType::MODIFIER:
			imageName = nodeModifierImage;
			break;
		case NFBlueprintType::ARITHMETIC:
			imageName = nodeArithmeticImage;
			break;
		case NFBlueprintType::DEBUGER:
			imageName = nodeDebugerImage;
			break;
		case NFBlueprintType::CUSTOM:
			imageName = nodeCustomImage;
			break;
	default:
		imageName = nodeUnknowImage;
		break;
	}

	return imageName;
}

NFColor NFBluePrintView::GetPinColor(NFValueType type)
{
	NFColor color = NFColor::DEFAULT;
	switch (type)
	{
	case NFValueType::UNKNOW:
		color = NFColor::DEFAULT;
		break;
	case NFValueType::Node:
		color = NFColor::WORKFLOW;
		break;
	default:
		color = NFColor::DEFAULT;
		break;
	}

	return color;
}
