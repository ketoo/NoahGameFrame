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
   std::string name = "GodView" + objectID.ToString();
   ImGui::Text(name.c_str());

   ImGui::BeginGroup();

   NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject(objectID);
   if (pObject)
   {
      NF_SHARE_PTR<NFIProperty> pProperty = pObject->GetPropertyManager()->First();
      while(pProperty)
      {
         ImGui::Text(pProperty->GetKey().c_str());
         ImGui::SameLine();
         ImGui::Text(pProperty->ToString().c_str());

         //static char str0[128] = "Hello, world!";
         //ImGui::InputText("input text", str0, IM_ARRAYSIZE(str0));
         
         pProperty = pObject->GetPropertyManager()->Next();
      }
   }

   

   ImGui::EndGroup();
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
   if (!objectID.IsNull())
   {
      auto blueprintNode = m_pBluePrintModule->FindBaseNode(objectID);
      if (blueprintNode)
      {
         ImGui::BeginGroup();
         
         bool selected = pBluePrintView->GetTreeView()->GetSelectedNode() == objectID;
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

         ImGui::BeginGroup();

         if (blueprintNode->blueprintType == NFBlueprintType::LOGICBLOCK)
         {
            BluePrintViewSubRenderForLogicBlock();
         }
         else if (blueprintNode->blueprintType == NFBlueprintType::MONITOR)
         {
			   BluePrintViewSubRenderForMonitor();
         }
         else if (blueprintNode->blueprintType == NFBlueprintType::JUDGEMENT)
         {
			   BluePrintViewSubRenderForJudgement();
         }
         else if (blueprintNode->blueprintType == NFBlueprintType::EXECUTER)
         {
			   BluePrintViewSubRenderForExecuter();
         }

         ImGui::EndGroup();
      }
   }
}

void NFHierachyView::BluePrintViewSubRenderForLogicBlock()
{
	ImGui::Separator();
   if (ImGui::Button("+ Monitor"))
   {
      NF_SHARE_PTR<NFBluePrintView> blueprintView = std::dynamic_pointer_cast<NFBluePrintView>(m_pUIModule->GetView(NFViewType::BluePrintView));
      blueprintView->TryToCreateMonitor();
   }
}

void NFHierachyView::BluePrintViewSubRenderForMonitor()
{
	NF_SHARE_PTR<NFBluePrintView> pBluePrintView = std::dynamic_pointer_cast<NFBluePrintView>(m_pUIModule->GetView(NFViewType::BluePrintView));
	NFGUID objectID = pBluePrintView->GetCurrentObjectID();
	if (!objectID.IsNull())
	{
		auto blueprintNode = m_pBluePrintModule->FindBaseNode(objectID);
		if (blueprintNode)
		{
			NF_SHARE_PTR<NFMonitor> monitor = std::dynamic_pointer_cast<NFMonitor>(blueprintNode);

         BluePrintViewSubRenderForMonitorHead(monitor);
         BluePrintViewSubRenderForMonitorBody(monitor);
         BluePrintViewSubRenderForMonitorBot(monitor);
      }
	}
}

void NFHierachyView::BluePrintViewSubRenderForJudgement()
{
	NF_SHARE_PTR<NFBluePrintView> pBluePrintView = std::dynamic_pointer_cast<NFBluePrintView>(m_pUIModule->GetView(NFViewType::BluePrintView));
	NFGUID objectID = pBluePrintView->GetCurrentObjectID();
	if (!objectID.IsNull())
	{
		auto blueprintNode = m_pBluePrintModule->FindBaseNode(objectID);
		if (blueprintNode)
		{
			NF_SHARE_PTR<NFJudgement> judgement = std::dynamic_pointer_cast<NFJudgement>(blueprintNode);

         BluePrintViewSubRenderForJudgementHead(judgement);
         BluePrintViewSubRenderForJudgementBody(judgement);
         BluePrintViewSubRenderForJudgementBot(judgement);
		}
	}
}

void NFHierachyView::BluePrintViewSubRenderForExecuter()
{
	NF_SHARE_PTR<NFBluePrintView> pBluePrintView = std::dynamic_pointer_cast<NFBluePrintView>(m_pUIModule->GetView(NFViewType::BluePrintView));
	NFGUID objectID = pBluePrintView->GetCurrentObjectID();
	if (!objectID.IsNull())
	{
		auto blueprintNode = m_pBluePrintModule->FindBaseNode(objectID);
		if (blueprintNode)
		{
		}
	}
}

void NFHierachyView::BluePrintViewSubRenderForMonitorHead(NF_SHARE_PTR<NFMonitor> monitor)
{

}

void NFHierachyView::BluePrintViewSubRenderForMonitorBody(NF_SHARE_PTR<NFMonitor> monitor)
{
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
}

void NFHierachyView::BluePrintViewSubRenderForMonitorBot(NF_SHARE_PTR<NFMonitor> monitor)
{
	ImGui::Separator();
   if (ImGui::Button("+ Judgement"))
   {
      NF_SHARE_PTR<NFBluePrintView> blueprintView = std::dynamic_pointer_cast<NFBluePrintView>(m_pUIModule->GetView(NFViewType::BluePrintView));
      blueprintView->TryToCreateJudgement();
   }
}

void NFHierachyView::BluePrintViewSubRenderForJudgementHead(NF_SHARE_PTR<NFJudgement> judgement)
{

}

void NFHierachyView::BluePrintViewSubRenderForJudgementBody(NF_SHARE_PTR<NFJudgement> judgement)
{
	ImGui::Text("Judgement Type");
   /*
	ImGui::SameLine();
	if (ImGui::Button(judgement->judgementType.toString().c_str()))
	{
		ImGui::OpenPopup("my_select_group");
	}

	if (ImGui::BeginPopup("my_select_group"))
	{
		ImGui::Separator();
		for (auto x : NFComparatorType::allValues())
		{
			if (ImGui::Selectable(x.toString().c_str(), false))
			{
				judgement->judgementType = x;
			}
		}

		ImGui::EndPopup();
	}
   */
   ///////////////////////////////////
   ImGui::BeginGroup();
   for (int i = 0; i < judgement->comparators.size(); ++i)
   {
      auto comparator = judgement->comparators[i];

      
		ImGui::Text("comparator");

   }

   ImGui::EndGroup();
    ///////////////////////////////////
}

void NFHierachyView::BluePrintViewSubRenderForJudgementBot(NF_SHARE_PTR<NFJudgement> judgement)
{
	ImGui::Separator();
   if (ImGui::Button("+ Comparator"))
   {
      NF_SHARE_PTR<NFBluePrintView> blueprintView = std::dynamic_pointer_cast<NFBluePrintView>(m_pUIModule->GetView(NFViewType::BluePrintView));
      blueprintView->TryToCreateComparator();
   }
/*
   if (ImGui::Button("+ Accessor"))
   {
      NF_SHARE_PTR<NFBluePrintView> blueprintView = std::dynamic_pointer_cast<NFBluePrintView>(m_pUIModule->GetView(NFViewType::BluePrintView));
      blueprintView->TryToCreateAccessor();
   }
*/
}


void NFHierachyView::InitBluePrintMonitorArgs(NF_SHARE_PTR<NFMonitor> monitor)
{
	monitor->arg.Clear();

	switch (monitor->operatorType)
	{
	case NFMonitorType::NetworkEvent:
		break;
	case NFMonitorType::NetworkMsgEvent:
		break;
	case NFMonitorType::ObjectEvent:
	{
      if (monitor->arg.GetCount() <= 0)
      {
         for (auto it : NFMonitorObjectEventArgType::allValues())
         {
            monitor->arg.AddString("");
         }
      }
      else
      {
         int i = 0;
         for (auto it : NFMonitorObjectEventArgType::allValues())
         {
            monitor->arg.SetString(i++, "");
         }
      }
      
	}
	break;
	case NFMonitorType::PropertyEvent:
	{
      if (monitor->arg.GetCount() <= 0)
      {
         for (auto it : NFMonitorPropertyEventArgType::allValues())
         {
            monitor->arg.AddString("");
         }
      }
      else
      {
         int i = 0;
         for (auto it : NFMonitorPropertyEventArgType::allValues())
         {
            monitor->arg.SetString(i++, "");
         }
      }
	}
	break;
	case NFMonitorType::RecordEvent:
	{
      if (monitor->arg.GetCount() <= 0)
      {
         for (auto it : NFMonitorPropertyEventArgType::allValues())
         {
            monitor->arg.AddString("");
         }
      }
      else
      {
         int i = 0;
         for (auto it : NFMonitorPropertyEventArgType::allValues())
         {
            monitor->arg.SetString(i++, "");
         }
      }
	}
	break;

	case NFMonitorType::HeartBeatEvent:
		break;
	case NFMonitorType::SceneEvent:
		break;
	case NFMonitorType::ItemEvent:
		break;
	case NFMonitorType::BuffEvent:
		break;

	default:
		break;
	}
}

void NFHierachyView::InitBluePrintJudgementArgs(NF_SHARE_PTR<NFJudgement> judgement)
{

}

void NFHierachyView::InitBluePrintExecuterArgs(NF_SHARE_PTR<NFExecuter> executer)
{

}