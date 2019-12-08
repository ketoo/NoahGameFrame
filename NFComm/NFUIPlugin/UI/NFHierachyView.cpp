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
         ImGui::Text(pProperty->GetString().c_str());

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
      auto treeNode = pBluePrintView->GetTreeView()->FindTreeNode(objectID);
      auto blueprintNode = m_pBluePrintModule->FindBaseNode(objectID);
      if (treeNode && blueprintNode)
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
         if (blueprintNode->blueprintType == NFBlueprintType::MONITOR)
         {
            NF_SHARE_PTR<NFMonitor> pBluePrintView = std::dynamic_pointer_cast<NFMonitor>(blueprintNode);
            ImGui::Text("MonitorType");
            ImGui::SameLine();
            if (ImGui::Button(pBluePrintView->operatorType.toString().c_str()))
            {
               ImGui::OpenPopup("my_select_group");
            }
 
            if (ImGui::BeginPopup("my_select_group"))
            {
               ImGui::Separator();
               for( auto x : NFMonitorType::allValues() )
               {
                  if (ImGui::Selectable(x.toString().c_str(), false))
                  {
                     pBluePrintView->operatorType = x;
                  }
               }
               
               ImGui::EndPopup();
            }

            switch (pBluePrintView->operatorType)
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
                  NFMonitorObjectEventType subType(pBluePrintView->subType);
                  if (ImGui::Button(subType.toString().c_str()))
                  {
                     ImGui::OpenPopup("my_select_sub_type_group");
                  }

                  if (ImGui::BeginPopup("my_select_sub_type_group"))
                  {
                     ImGui::Separator();
                     for( auto x : NFMonitorObjectEventType::allValues() )
                     {
                        if (ImGui::Selectable(x.toString().c_str(), false))
                        {
                           pBluePrintView->subType = x;
                        }
                     }
                     
                     ImGui::EndPopup();
                  }


                  //arg: class name


               }
               break;
            case NFMonitorType::PropertyEvent:
               //1 arg
               break;
            case NFMonitorType::RecordEvent:
               {
                  //sub type

                  ImGui::Text("RecordEventSubType");
                  ImGui::SameLine();
                  NFMonitorRecordEventType subType(pBluePrintView->subType);
                  if (ImGui::Button(subType.toString().c_str()))
                  {
                     ImGui::OpenPopup("my_select_sub_type_group");
                  }

                  if (ImGui::BeginPopup("my_select_sub_type_group"))
                  {
                     ImGui::Separator();
                     for( auto x : NFMonitorRecordEventType::allValues() )
                     {
                        if (ImGui::Selectable(x.toString().c_str(), false))
                        {
                           pBluePrintView->subType = x;
                        }
                     }
                     
                     ImGui::EndPopup();
                  }

                  //arg: recordName
                  ImGui::Text("RecordName");
                  ImGui::SameLine();
                  std::string recordName = pBluePrintView->arg.String(0);
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
                     ImGui::Separator();
                     for( auto x : NFMonitorRecordEventType::allValues() )
                     {
                        if (ImGui::Selectable(x.toString().c_str(), false))
                        {
                           pBluePrintView->subType = x;
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
         else if (blueprintNode->blueprintType == NFBlueprintType::JUDGEMENT)
         {

         }
         else if (blueprintNode->blueprintType == NFBlueprintType::EXECUTER)
         {

         }

         ImGui::EndGroup();

      }
   }
}