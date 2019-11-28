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

#include "NFGodView.h"
#include "NFNodeView.h"
#include "NFUIModule.h"

NFGodView::NFGodView(NFIPluginManager* p, NFViewType vt) : NFIView(p, vt, GET_CLASS_NAME(NFGodView))
{
   m_pUIModule = pPluginManager->FindModule<NFIUIModule>();
   m_pClassModule = pPluginManager->FindModule<NFIClassModule>();
   m_pElementModule = pPluginManager->FindModule<NFIElementModule>();
   m_pSceneModule = pPluginManager->FindModule<NFISceneModule>();
   m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();


   m_pNodeView = NF_NEW NFNodeView(p);
}

NFGodView::~NFGodView()
{
   delete m_pNodeView;
   m_pNodeView = nullptr;
}

bool NFGodView::Execute()
{
	//1. get all scene from NF
	//2. let the user choose one scene with the group
	//3. get all objects from that group
	//4. draw object by imgui
	//5. show the props and records if the user picked an object
	//6. use can try to change the value of props to trigger the logic block unit to have a test
   if (ImGui::IsWindowFocused())
   {
      NF_SHARE_PTR<NFIView> pView = m_pUIModule->GetView(NFViewType::HierachyView);
      if (pView)
      {
         pView->OccupySubRender(this);
      }
   }

	NF_SHARE_PTR<NFIClass> xLogicClass = m_pClassModule->GetElement(NFrame::Scene::ThisName());
	if (xLogicClass)
	{
		const std::vector<std::string>& strIdList = xLogicClass->GetIDList();

      static int selected = -1;
      if (ImGui::Button("Select Scene"))
      {
         ImGui::OpenPopup("my_select_scene");
      }
      ImGui::SameLine();
      ImGui::TextUnformatted(selected < 0 ? "<None>" : strIdList[selected].c_str());

      if (ImGui::BeginPopup("my_select_scene"))
      {
         for (int n = 0; n < strIdList.size(); n++)
         {
            if (ImGui::Selectable(strIdList[n].c_str(), selected == n))
            {
                selected = n;
                mnSceneID = lexical_cast<int>(strIdList[n]);
                mnGroupID= -1;
                mCurrentObjectID = NFGUID();
            }
         }
          
         ImGui::EndPopup();
      }
	}

   if (mnSceneID > 0)
   {
      const std::vector<int>& groups = m_pSceneModule->GetGroups(mnSceneID);

      static int selected = -1;

      ImGui::SameLine();
      if (ImGui::Button("Select Group"))
      {
         ImGui::OpenPopup("my_select_group");
      }
      ImGui::SameLine();
      ImGui::TextUnformatted(selected < 0 ? "<None>" : std::to_string(mnGroupID).c_str());

      if (ImGui::BeginPopup("my_select_group"))
      {
         ImGui::Separator();
         for (int n = 0; n < groups.size(); n++)
         {
            if (ImGui::Selectable(std::to_string(groups[n]).c_str(), selected == n))
            {
                selected = n;
                mnGroupID = groups[n];
            }
         }
         
         ImGui::EndPopup();
      }

      ImGui::SameLine();
      if (ImGui::Button("  -  "))
      {
         //occupy inspectorview
      }
      
      ImGui::SameLine();
      if (ImGui::Button(" + "))
      {
         //occupy inspectorview
      }

      
   }

   ImGui::SameLine();
   if (ImGui::Button("return to center"))
   {
      m_pNodeView->ResetOffest(NFVector2::Zero());
   }

   RenderSceneObjectNode(mnSceneID, mnGroupID);

	return true;
}

void NFGodView::RenderScene(const int sceneID)
{
   if (mnGroupID >= 0)
   {
      RenderScene(sceneID, mnGroupID);
   }
   else
   {
      const std::vector<int>& groups = m_pSceneModule->GetGroups(sceneID);
      for (int j = 0; j < groups.size(); j++)
      {
         RenderScene(sceneID, groups[j]);
      }
   }
}

void NFGodView::SubRender()
{
   ImGui::Text(this->name.c_str());
   
   if (mnSceneID <= 0)
   {
      NF_SHARE_PTR<NFIClass> xLogicClass = m_pClassModule->GetElement(NFrame::Scene::ThisName());
      if (xLogicClass)
      {
         const std::vector<std::string>& strIdList = xLogicClass->GetIDList();
         for (int i = 0; i < strIdList.size(); i++)
         {
            std::string sceneTreeNodeName = "Scene<" + strIdList[i] + ">";
            if (ImGui::TreeNode(sceneTreeNodeName.c_str()))
            {
               RenderScene(lexical_cast<int>(strIdList[i]));
               
               ImGui::TreePop();
               ImGui::Separator();
            }
         }
      }
   }
   else
   {
      RenderScene(mnSceneID);
   }
}

void NFGodView::RenderScene(const int sceneID, const int groupID)
{
   std::string groupTreeNodeName = "Group<" + std::to_string(groupID) + ">";
   if (ImGui::TreeNode(groupTreeNodeName.c_str()))
   {
      //objects
      NFDataList list;
      m_pKernelModule->GetGroupObjectList(sceneID, groupID, list);
      for (int k = 0; k < list.GetCount(); ++k)
      {
         const NFGUID& guid = list.Object(k);
         const std::string& strClassName = m_pKernelModule->GetPropertyString(guid, NFrame::IObject::ClassName());
         std::string buttonName = strClassName + "<" + guid.ToString() + ">";
         if (ImGui::Button(buttonName.c_str()))
         {
            SetCurrentObjectID(guid);

            //occupy inspectorview
            NF_SHARE_PTR<NFIView> pHierachyView = m_pUIModule->GetView(NFViewType::HierachyView);
            NF_SHARE_PTR<NFIView> pInspectorView = m_pUIModule->GetView(NFViewType::InspectorView);
            if (pHierachyView && pInspectorView)
            {
               pInspectorView->OccupySubRender(pHierachyView.get());
            }
         }
      }

      ImGui::TreePop();
      ImGui::Separator();
   }
}


NFGUID NFGodView::GetCurrentObjectID()
{
   return mCurrentObjectID;
}

void NFGodView::SetCurrentObjectID(const NFGUID& id)
{
   mCurrentObjectID = id;
   const std::string& strName = m_pKernelModule->GetPropertyString(mCurrentObjectID, NFrame::IObject::Name());
   const NFVector3& pos = m_pKernelModule->GetPropertyVector3(mCurrentObjectID, NFrame::IObject::Position());

   NFNodeView* pView = (NFNodeView*)m_pNodeView;
   pView->ResetOffest(NFVector2(pos.X(), pos.Z()));
}

void NFGodView::RenderSceneObjectNode(const int sceneID, const int groupID)
{
   NFNodeView* pView = (NFNodeView*)m_pNodeView;
   pView->CleanNodes();

   if (sceneID > 0 && groupID > 0)
   {
      //objects
      NFDataList list;
      m_pKernelModule->GetGroupObjectList(sceneID, groupID, list);
      for (int k = 0; k < list.GetCount(); ++k)
      {
         const NFGUID& guid = list.Object(k);
         const std::string& strName = m_pKernelModule->GetPropertyString(guid, NFrame::IObject::Name());
         const NFVector3& pos = m_pKernelModule->GetPropertyVector3(guid, NFrame::IObject::Position());
         pView->AddNode(guid, strName, NFVector2(pos.X(), pos.Z()));
         //pView->AddNode(guid, guid.ToString(), NFVector2());
         pView->AddNodeAttrIn(guid, m_pKernelModule->CreateGUID(), strName);
      }
   }

   m_pNodeView->Execute();
}