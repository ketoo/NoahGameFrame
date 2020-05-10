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

#include "NFUIModule.h"
#include "NFHierachyView.h"
#include "NFGodView.h"
#include "NFBluePrintView.h"
#include "NFProjectView.h"
#include "NFGameView.h"

NFHierachyView::NFHierachyView(NFIPluginManager* p, NFViewType vt) : NFIView(p, vt, GET_CLASS_NAME(NFHierachyView))
{
	m_pClassModule = pPluginManager->FindModule<NFIClassModule>();
	m_pElementModule = pPluginManager->FindModule<NFIElementModule>();
	m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
	m_pEventModule = pPluginManager->FindModule<NFIEventModule>();
	m_pBPVirtualMachineModule = pPluginManager->FindModule<NFIBPVirtualMachineModule>();
	
	m_pBluePrintModule = pPluginManager->FindModule<NFIBluePrintModule>();
}

bool NFHierachyView::Execute()
{
	//1. the project root folder is NFDataCfg

	return false;
}


void NFHierachyView::SubRender()
{
   if (m_pOccupyView)
   {
      switch (m_pOccupyView->viewType)
      {
      case NFViewType::GodView:
         GodViewSubRender();
         break;
      case NFViewType::BluePrintView:
         BluePrintViewSubRender();
         break;
      case NFViewType::ProjectView:
         ProjectViewSubRender();
         break;
      case NFViewType::GameView:
         GameViewSubRender();
         break;
      
      default:
         break;
      }
   }
}

void NFHierachyView::GodViewSubRender()
{
   NFGUID objectID = ((NFGodView*)m_pOccupyView)->GetCurrentObjectID();
   NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject(objectID);
   std::string name = "GUID: " + objectID.ToString();
   ImGui::Text(name.c_str());
   if (ImGui::Button("Create"))
   {
	   const int scene = m_pKernelModule->GetPropertyInt(objectID, NFrame::IObject::SceneID());
	   const int group = m_pKernelModule->GetPropertyInt(objectID, NFrame::IObject::GroupID());
	   const std::string& className = m_pKernelModule->GetPropertyString(objectID, NFrame::IObject::ClassName());
	   const std::string& configID = m_pKernelModule->GetPropertyString(objectID, NFrame::IObject::ConfigID());
	   const NFVector3& position = m_pKernelModule->GetPropertyVector3(objectID, NFrame::IObject::Position());

	   NFDataList dataList;
	   dataList << NFrame::IObject::Position();
	   dataList << position;

	   m_pKernelModule->CreateObject(NFGUID(), scene, group, className, configID, dataList);
   }

   ImGui::SameLine();
   if (ImGui::Button("Destroy"))
   {
	   m_pKernelModule->DestroyObject(objectID);
   }

   if (pObject == nullptr)
   {
	   ImGui::SameLine();
	   if (ImGui::Button("Disable AI"))
	   {
		   NFDataList data;
		   m_pKernelModule->GetGroupObjectList(objectID.nHead64, objectID.nData64, data);
		   for (int i = 0; i < data.GetCount(); ++i)
		   {
			   m_pKernelModule->SetPropertyInt(data.Object(i), NFrame::NPC::Disable(), 1);
		   }
	   }
   }

   ImGui::Separator();

   ImGui::BeginGroup();

   if (pObject)
   {
	   auto element = m_pClassModule->GetElement(NFrame::IObject::ThisName());
	   NF_SHARE_PTR<NFIProperty> property = element->GetPropertyManager()->First();
	   while (property)
	   {
		   if (property->GetKey() == NFrame::IObject::ClassName()
			   || property->GetKey() == NFrame::IObject::ConfigID()
			   || property->GetKey() == NFrame::IObject::ID()
			   || property->GetKey() == NFrame::IObject::GroupID()
			   || property->GetKey() == NFrame::IObject::SceneID())
		   {
			   ImGui::Button(property->GetKey().c_str());

		   }
		   else
		   {
			   if (ImGui::Button(property->GetKey().c_str()))
			   {
				   modifyPropertyName = property->GetKey();
				   strcpy(modifyPropertyValue, property->ToString().c_str());
			   }
		   }
			ImGui::SameLine();

			auto p = pObject->GetPropertyManager()->GetElement(property->GetKey());
			ImGui::Text(p->ToString().c_str());

			property = element->GetPropertyManager()->Next();
	   }

		ImGui::Separator();
		ImGui::Separator();

		NF_SHARE_PTR<NFIProperty> pProperty = pObject->GetPropertyManager()->First();
		while(pProperty)
		{
			if (!element->GetPropertyManager()->GetElement(pProperty->GetKey()))
			{
				if (ImGui::Button(pProperty->GetKey().c_str()))
				{
					modifyPropertyName = pProperty->GetKey();
					strcpy(modifyPropertyValue, pProperty->ToString().c_str());
				}
				ImGui::SameLine();
				ImGui::Text(pProperty->ToString().c_str());
			}
         
         	pProperty = pObject->GetPropertyManager()->Next();
      	}
   }


   ImGui::EndGroup();

   RenderForModifyProperty();
}

void NFHierachyView::GameViewSubRender()
{
   ImGui::Text("GameView");
}

void NFHierachyView::ProjectViewSubRender()
{
   ImGui::Text("ProjectView");
}

void NFHierachyView::BluePrintViewSubRender()
{
   NF_SHARE_PTR<NFBluePrintView> pBluePrintView = std::dynamic_pointer_cast<NFBluePrintView>(m_pUIModule->GetView(NFViewType::BluePrintView));
   NFGUID objectID = pBluePrintView->GetCurrentObjectID();
   NFGUID blockID = pBluePrintView->GetCurrentLogicBlockID();

	if(objectID.IsNull() && !blockID.IsNull()) 
	{
		objectID = blockID;
	}


   if (!objectID.IsNull())
   {
	   bool selected = pBluePrintView->GetTreeView().GetSelectedNode() == objectID;
	   if (!selected)
	   {
		   objectID = blockID;
	   }

      auto blueprintNode = m_pBluePrintModule->FindNode(objectID);
      if (blueprintNode)
      {
      		ImGui::BeginGroup();
         
	      ImGui::Checkbox("", &selected);
          ImGui::SameLine();

         char blueprintName[128] = {0};
         strcpy(blueprintName, blueprintNode->name .c_str());
		 if (ImGui::InputText("", blueprintName, IM_ARRAYSIZE(blueprintName)))
         {
            blueprintNode->name = blueprintName;
         }

         ImGui::EndGroup();
         
         ImGui::Text("ID");
         ImGui::SameLine();
         ImGui::Text(objectID.ToString().c_str());

         ImGui::Text("Type");
         ImGui::SameLine();
         ImGui::Text(blueprintNode->blueprintType.toString().c_str());

		  if (ImGui::Button("Print All Data"))
		  {
			  blueprintNode->Print();
		  }

         ImGui::BeginGroup();

         if (blueprintNode->blueprintType == NFBlueprintType::LOGICBLOCK)
         {
            BluePrintViewSubRenderForLogicBlock();
         }
         else if (blueprintNode->blueprintType == NFBlueprintType::MONITOR)
         {
			   BluePrintViewSubRenderForMonitor();
         }
         else if (blueprintNode->blueprintType == NFBlueprintType::BRANCH)
         {
			   BluePrintViewSubRenderForBranch();
         }
         else if (blueprintNode->blueprintType == NFBlueprintType::EXECUTER)
         {
			   BluePrintViewSubRenderForExecutor();
         }
		 else if (blueprintNode->blueprintType == NFBlueprintType::VARIABLE)
		 {
			 BluePrintViewSubRenderForVariable();
		 }
		 else if (blueprintNode->blueprintType == NFBlueprintType::MODIFIER)
		 {
			 BluePrintViewSubRenderForVariable();
		 }
		 else if (blueprintNode->blueprintType == NFBlueprintType::ARITHMETIC)
		 {
			 BluePrintViewSubRenderForVariable();
		 }
		 else if (blueprintNode->blueprintType == NFBlueprintType::DEBUGER)
		 {
			 BluePrintViewSubRenderForVariable();
		 }
         ImGui::EndGroup();
      }
   }
}

void NFHierachyView::BluePrintViewSubRenderForLogicBlock()
{
	NF_SHARE_PTR<NFBluePrintView> pBluePrintView = std::dynamic_pointer_cast<NFBluePrintView>(m_pUIModule->GetView(NFViewType::BluePrintView));
	NFGUID objectID = pBluePrintView->GetCurrentLogicBlockID();
	if (!objectID.IsNull())
	{
		auto logicBlock = m_pBluePrintModule->GetLogicBlock(objectID);
		if (logicBlock)
		{
			if (logicBlock->running)
			{
				if (ImGui::Button("Stop"))
				{
					m_pBPVirtualMachineModule->StopLogicBlock(objectID);
				}
			}
			else
			{
				if (ImGui::Button("Start"))
				{
					m_pBPVirtualMachineModule->RunLogicBlock(objectID);
				}
			}
		}
	}
}

void NFHierachyView::BluePrintViewSubRenderForMonitor()
{
	NF_SHARE_PTR<NFBluePrintView> pBluePrintView = std::dynamic_pointer_cast<NFBluePrintView>(m_pUIModule->GetView(NFViewType::BluePrintView));
	NFGUID objectID = pBluePrintView->GetCurrentObjectID();
	if (!objectID.IsNull())
	{
		auto blueprintNode = m_pBluePrintModule->FindNode(objectID);
		if (blueprintNode)
		{
			NF_SHARE_PTR<NFIMonitor> monitor = std::dynamic_pointer_cast<NFIMonitor>(blueprintNode);

			ImGui::Separator();
			BluePrintViewSubRenderForMonitorHead(monitor);
			ImGui::Separator();
			BluePrintViewSubRenderForMonitorBody(monitor);
			ImGui::Separator();
			BluePrintViewSubRenderForMonitorBot(monitor);
		}
	}
}

void NFHierachyView::BluePrintViewSubRenderForBranch()
{
	NF_SHARE_PTR<NFBluePrintView> pBluePrintView = std::dynamic_pointer_cast<NFBluePrintView>(m_pUIModule->GetView(NFViewType::BluePrintView));
	NFGUID objectID = pBluePrintView->GetCurrentObjectID();
	if (!objectID.IsNull())
	{
		auto blueprintNode = m_pBluePrintModule->FindNode(objectID);
		if (blueprintNode)
		{
			NF_SHARE_PTR<NFIBranch> branch = std::dynamic_pointer_cast<NFIBranch>(blueprintNode);

			ImGui::Separator();
			BluePrintViewSubRenderForBranchHead(branch);
			ImGui::Separator();
			BluePrintViewSubRenderForBranchBody(branch);
			ImGui::Separator();
			BluePrintViewSubRenderForBranchBot(branch);
		}
	}
}

void NFHierachyView::BluePrintViewSubRenderForExecutor()
{
	NF_SHARE_PTR<NFBluePrintView> pBluePrintView = std::dynamic_pointer_cast<NFBluePrintView>(m_pUIModule->GetView(NFViewType::BluePrintView));
	NFGUID objectID = pBluePrintView->GetCurrentObjectID();
	if (!objectID.IsNull())
	{
		auto blueprintNode = m_pBluePrintModule->FindNode(objectID);
		if (blueprintNode)
		{
		}
	}
}

void NFHierachyView::BluePrintViewSubRenderForVariable()
{
	NF_SHARE_PTR<NFBluePrintView> pBluePrintView = std::dynamic_pointer_cast<NFBluePrintView>(m_pUIModule->GetView(NFViewType::BluePrintView));
	NFGUID objectID = pBluePrintView->GetCurrentObjectID();
	if (!objectID.IsNull())
	{
		auto blueprintNode = m_pBluePrintModule->FindNode(objectID);
		if (blueprintNode)
		{
		}
	}
}

void NFHierachyView::BluePrintViewSubRenderForModifier()
{

}

void NFHierachyView::BluePrintViewSubRenderForLogger()
{

}

void NFHierachyView::BluePrintViewSubRenderForArithmetic()
{

}

void NFHierachyView::BluePrintViewSubRenderForMonitorHead(NF_SHARE_PTR<NFIMonitor> monitor)
{
	switch (monitor->monitorType)
	{
	case NFMonitorType::GameEvent:
	{
		auto inputEventID = monitor->GetInputArg(NFGameEventMonitorInputArg::EventID);
		int localEventID = inputEventID->GetInt();
		if (ImGui::InputInt("input int", &localEventID))
		{
			inputEventID->SetInt(localEventID);
		}

		if (ImGui::Button("DoGameEvent"))
		{
			m_pEventModule->DoEvent(monitor->id, localEventID, NFDataList::Empty());
		}
	}

		break;
	case NFMonitorType::NetworkEvent:
		if (ImGui::Button("DoNetworkEvent"))
		{
			//m_pEventModule->DoEvent(id, std::atoi(eventID), NFDataList::Empty());
		}
		break;
	case NFMonitorType::NetworkMsgEvent:
		if (ImGui::Button("DoNetworkMsgEvent"))
		{
			//m_pEventModule->DoEvent(id, std::atoi(eventID), NFDataList::Empty());
		}
		break;
	default:
		break;
	}
}

void NFHierachyView::BluePrintViewSubRenderForMonitorBody(NF_SHARE_PTR<NFIMonitor> monitor)
{
	/*
	ImGui::Text("MonitorType");
	ImGui::SameLine();
	if (ImGui::Button(monitor->operatorType.toString().c_str()))
	{
		ImGui::OpenPopup("my_select_group");
	}

	if (ImGui::BeginPopup("my_select_group"))
	{
		ImGui::Separator();
		for (auto x : NFMonitorType::allValues())
		{
			if (ImGui::Selectable(x.toString().c_str(), false))
			{
				monitor->operatorType = x;
				InitBluePrintMonitorArgs(monitor);
			}
		}

		ImGui::EndPopup();
	}

    ///////////////////////////////////
    
	switch (monitor->operatorType)
	{
	case NFMonitorType::NetworkEvent:
		//sub type
		break;
	case NFMonitorType::NetworkMsgEvent:
		//sub type
		break;
	case NFMonitorType::ObjectEvent:
	{
		//sub type
		ImGui::Text("ObjectEventSubType");
		ImGui::SameLine();
		NFMonitorObjectEventType subType(monitor->subType);
		if (ImGui::Button(subType.toString().c_str()))
		{
			ImGui::OpenPopup("my_select_sub_type_group");
		}

		if (ImGui::BeginPopup("my_select_sub_type_group"))
		{
			ImGui::Separator();
			for (auto x : NFMonitorObjectEventType::allValues())
			{
				if (ImGui::Selectable(x.toString().c_str(), false))
				{
					monitor->subType = x;
				}
			}

			ImGui::EndPopup();
		}

		//arg: class name
		ImGui::Text("ClassName");
		ImGui::SameLine();
		
		std::string className = monitor->arg.String(NFMonitorObjectEventArgType::ClassName);
		if (className.empty())
		{
			className = "your class name";
		}

		if (ImGui::Button(className.c_str()))
		{
			ImGui::OpenPopup("my_select_class_name_group");
		}

		if (ImGui::BeginPopup("my_select_class_name_group"))
		{
			ImGui::Separator();

			auto classObject = m_pClassModule->First();
			while (classObject)
			{
				if (ImGui::Selectable(classObject->GetClassName().c_str(), false))
				{
					//init for arg
				   InitBluePrintMonitorArgs(monitor);
					monitor->arg.SetString(NFMonitorObjectEventArgType::ClassName, classObject->GetClassName());
				}

				classObject = m_pClassModule->Next();
			}

			ImGui::EndPopup();
		}
	}
	break;
	case NFMonitorType::PropertyEvent:
	{
		//arg: class name
		ImGui::Text("ClassName");
		ImGui::SameLine();

		std::string className = monitor->arg.String(NFMonitorObjectEventArgType::ClassName);
		if (className.empty())
		{
			className = "your class name";
		}

		if (ImGui::Button(className.c_str()))
		{
			ImGui::OpenPopup("my_select_class_name_group");
		}

		if (ImGui::BeginPopup("my_select_class_name_group"))
		{
			auto classObject = m_pClassModule->First();
			while (classObject)
			{
				if (ImGui::Selectable(classObject->GetClassName().c_str(), false))
				{
					//init for arg
				   InitBluePrintMonitorArgs(monitor);
					monitor->arg.SetString(NFMonitorObjectEventArgType::ClassName, classObject->GetClassName());
				}

				classObject = m_pClassModule->Next();
			}

			ImGui::EndPopup();
		}

		std::string propertyName = monitor->arg.String(NFMonitorPropertyEventArgType::PropertyName);
		if (propertyName.empty())
		{
			propertyName = "your property name";
		}
		ImGui::Text("PropertyName");
		ImGui::SameLine();
		if (ImGui::Button(propertyName.c_str()))
		{
			ImGui::OpenPopup("my_select_property_name_group");
		}

		if (ImGui::BeginPopup("my_select_property_name_group"))
		{
			const std::string& className = monitor->arg.String(NFMonitorPropertyEventArgType::ClassName);
			auto classObject = m_pClassModule->GetElement(className);
			if (classObject)
			{
				auto propertyObject = classObject->GetPropertyManager()->First();
				while (propertyObject)
				{
					if (ImGui::Selectable(propertyObject->GetKey().c_str(), false))
					{
						monitor->arg.SetString(NFMonitorPropertyEventArgType::PropertyName, propertyObject->GetKey());
					}

					propertyObject = classObject->GetPropertyManager()->Next();
				}
			}

			ImGui::EndPopup();
		}
	}
	break;
	case NFMonitorType::RecordEvent:
	{
		//sub type

		ImGui::Text("RecordEventSubType");
		ImGui::SameLine();
		NFMonitorRecordEventType subType(monitor->subType);
		if (ImGui::Button(subType.toString().c_str()))
		{
			ImGui::OpenPopup("my_select_sub_type_group");
		}

		if (ImGui::BeginPopup("my_select_sub_type_group"))
		{
			for (auto x : NFMonitorRecordEventType::allValues())
			{
				if (ImGui::Selectable(x.toString().c_str(), false))
				{
					monitor->subType = x;
				}
			}

			ImGui::EndPopup();
		}

       ImGui::Text("ClassName");
		ImGui::SameLine();

		std::string className = monitor->arg.String(NFMonitorRecordEventArgType::ClassName);
		if (className.empty())
		{
			className = "your class name";
		}

		if (ImGui::Button(className.c_str()))
		{
			ImGui::OpenPopup("my_select_class_name_group");
		}

		if (ImGui::BeginPopup("my_select_class_name_group"))
		{
			auto classObject = m_pClassModule->First();
			while (classObject)
			{
				if (ImGui::Selectable(classObject->GetClassName().c_str(), false))
				{
					//init for arg
				   	InitBluePrintMonitorArgs(monitor);
					monitor->arg.SetString(NFMonitorRecordEventArgType::ClassName, classObject->GetClassName());
				}

				classObject = m_pClassModule->Next();
			}

			ImGui::EndPopup();
		}

		//arg: recordName
		ImGui::Text("RecordName");
		ImGui::SameLine();
		std::string recordName = monitor->arg.String(NFMonitorRecordEventArgType::RecordName);
		if (recordName.empty())
		{
			recordName = "input your record name";
		}
		if (ImGui::Button(recordName.c_str()))
		{
			ImGui::OpenPopup("my_select_arg_group");
		}

		if (ImGui::BeginPopup("my_select_arg_group"))
		{
			auto classObject = m_pClassModule->GetElement(className);
			if (classObject)
			{
				auto recordObject = classObject->GetRecordManager()->First();
				while (recordObject)
				{
					if (ImGui::Selectable(recordObject->GetName().c_str(), false))
					{
						monitor->arg.SetString(NFMonitorRecordEventArgType::RecordName, recordObject->GetName());
					}

					recordObject = classObject->GetRecordManager()->Next();
				}
			}
			ImGui::EndPopup();
		}
	}
	break;

	case NFMonitorType::HeartBeatEvent:
		//sub type
		break;
	case NFMonitorType::SceneEvent:
		//sub type
		break;
	case NFMonitorType::ItemEvent:
		//sub type
		break;
	case NFMonitorType::BuffEvent:
		//sub type
		break;

	default:
		break;
	}

	*/
}

void NFHierachyView::BluePrintViewSubRenderForMonitorBot(NF_SHARE_PTR<NFIMonitor> monitor)
{

}

void NFHierachyView::BluePrintViewSubRenderForBranchHead(NF_SHARE_PTR<NFIBranch> judgement)
{

}

void NFHierachyView::BluePrintViewSubRenderForBranchBody(NF_SHARE_PTR<NFIBranch> judgement)
{
	ImGui::Text("Judgement Type");
}

void NFHierachyView::BluePrintViewSubRenderForBranchBot(NF_SHARE_PTR<NFIBranch> judgement)
{

}


void NFHierachyView::InitBluePrintMonitorArgs(NF_SHARE_PTR<NFIMonitor> monitor)
{

}

void NFHierachyView::InitBluePrintJudgementArgs(NF_SHARE_PTR<NFIBranch> judgement)
{

}

void NFHierachyView::InitBluePrintExecutorArgs(NF_SHARE_PTR<NFIExecutor> executer)
{

}

void NFHierachyView::RenderForModifyProperty()
{
	if (modifyPropertyName.length() > 0)
	{
   		NFGUID objectID = ((NFGodView*)m_pOccupyView)->GetCurrentObjectID();
		NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject(objectID);
		if (pObject)
		{
			NF_SHARE_PTR<NFIProperty> pProperty = pObject->GetPropertyManager()->GetElement(modifyPropertyName);

			ImGui::OpenPopup("Modify Property Value");
			ImGui::SetNextWindowSize(ImVec2(230, 150));		
			if (ImGui::BeginPopupModal("Modify Property Value"))
			{
				ImGui::Text(modifyPropertyName.c_str());		
				ImGui::InputText("New Value", modifyPropertyValue, IM_ARRAYSIZE(modifyPropertyValue));		
				ImGui::Separator();		
				ImGui::Text("If you modified the value then the framework will trigger the property event!\n\n");		
				ImGui::Separator();		
				if (ImGui::Button("Cancel", ImVec2(100, 30)))
				{
					modifyPropertyName = "";
					ImGui::CloseCurrentPopup();
				}		
				ImGui::SameLine();		
				if (ImGui::Button("OK", ImVec2(100, 30)))
				{
					modifyPropertyName = "";

					switch (pProperty->GetType())
					{
						case NFDATA_TYPE::TDATA_INT:
						{
							pProperty->SetInt(lexical_cast<int>(modifyPropertyValue));
						}
						break;
						case NFDATA_TYPE::TDATA_FLOAT:
						{
							pProperty->SetFloat(lexical_cast<float>(modifyPropertyValue));
						}
						break;
						case NFDATA_TYPE::TDATA_STRING:
						{
							pProperty->SetString(modifyPropertyValue);
						}
						break;
						case NFDATA_TYPE::TDATA_OBJECT:
						{
							NFGUID id;
							if (id.FromString(modifyPropertyValue))
							{
								pProperty->SetObject(id);
							}
						}
						break;
						case NFDATA_TYPE::TDATA_VECTOR2:
						{
							NFVector2 vec;
							if (vec.FromString(modifyPropertyValue))
							{
								pProperty->SetVector2(vec);
							}
						}
						break;
						case NFDATA_TYPE::TDATA_VECTOR3:
						{
							NFVector3 vec;
							if (vec.FromString(modifyPropertyValue))
							{
								pProperty->SetVector3(vec);
							}
						}
						break;

						default:
						break;
					}
					ImGui::CloseCurrentPopup();
				}		
				ImGui::EndPopup();
			}
		}
	}
}