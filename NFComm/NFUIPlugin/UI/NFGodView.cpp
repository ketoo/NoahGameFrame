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
   m_pTreeView = NF_NEW NFTreeView(p);

   m_pNodeView->ResetOffest(NFVector2::Zero());

   m_pTreeView->SetSelectedNodeFunctor(std::bind(&NFGodView::HandlerSelected, this, std::placeholders::_1));
   m_pTreeView->SetName(GET_CLASS_NAME(NFGodView));
}

NFGodView::~NFGodView()
{
   delete m_pTreeView;
   m_pTreeView = nullptr;

   delete m_pNodeView;
   m_pNodeView = nullptr;
}

bool NFGodView::Execute()
{
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
					mSceneID = lexical_cast<int>(strIdList[n]);
					mGroupID= -1;

					//render
					RenderScene(mSceneID, mGroupID);
					RenderSceneObjectNode(mSceneID, mGroupID);
				}
			}
          
			ImGui::EndPopup();
		}
	}

   if (mSceneID > 0)
   {
      const std::vector<int>& groups = m_pSceneModule->GetGroups(mSceneID);

      static int selected = -1;

      ImGui::SameLine();
      if (ImGui::Button("Select Group"))
      {
         ImGui::OpenPopup("my_select_group");
      }
      ImGui::SameLine();
      ImGui::TextUnformatted(selected < 0 ? "<None>" : std::to_string(mGroupID).c_str());

      if (ImGui::BeginPopup("my_select_group"))
      {
         ImGui::Separator();
         for (int n = 0; n < groups.size(); n++)
         {
            if (ImGui::Selectable(std::to_string(groups[n]).c_str(), selected == n))
            {
                selected = n;
				mGroupID = groups[n];

				//render
				RenderScene(mSceneID, mGroupID);
				RenderSceneObjectNode(mSceneID, mGroupID);
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

	m_pNodeView->Execute();

	UpdateSceneObjectNodePosition(mSceneID, mGroupID);

	return true;
}

void NFGodView::HandlerSelected(const NFGUID& id)
{
	SetCurrentObjectID(id);

	//occupy inspectorview
	NF_SHARE_PTR<NFIView> pHierachyView = m_pUIModule->GetView(NFViewType::HierachyView);
	NF_SHARE_PTR<NFIView> pInspectorView = m_pUIModule->GetView(NFViewType::InspectorView);
	if (pHierachyView && pInspectorView)
	{
		pInspectorView->OccupySubRender(pHierachyView.get());
	}
}

NFVector2 NFGodView::ToNodeSpacePos(const NFVector2& v)
{
	return NFVector2(v.X() * 10, v.Y() * 10);
	//return NFVector2(v.X(), v.Y());
}

void NFGodView::RenderScene(const int sceneID, const int groupID)
{
	m_pTreeView->Clear();

	if (groupID < 0)
	{
		const std::vector<int>& groups = m_pSceneModule->GetGroups(sceneID);
		for (int j = 0; j < groups.size(); j++)
		{
			int group = groups[j];

			NFGUID nodeId = NFGUID(sceneID, group);
			m_pTreeView->AddTreeNode(nodeId, nodeId.ToString());

			//objects
			NFDataList list;
			m_pKernelModule->GetGroupObjectList(sceneID, group, list);
			for (int k = 0; k < list.GetCount(); ++k)
			{
				const NFGUID& guid = list.Object(k);
				const std::string& strClassName = m_pKernelModule->GetPropertyString(guid, NFrame::IObject::ClassName());
				std::string buttonName = strClassName + "<" + guid.ToString() + ">";

				m_pTreeView->AddSubTreeNode(nodeId, guid, buttonName.c_str());
			}
		}
	}
	else
	{
		NFGUID nodeId = NFGUID(sceneID, groupID);
		m_pTreeView->AddTreeNode(nodeId, nodeId.ToString());

		//objects
		NFDataList list;
		m_pKernelModule->GetGroupObjectList(sceneID, groupID, list);
		for (int k = 0; k < list.GetCount(); ++k)
		{
			const NFGUID& guid = list.Object(k);
			const std::string& strClassName = m_pKernelModule->GetPropertyString(guid, NFrame::IObject::ClassName());
			std::string buttonName = strClassName + "<" + guid.ToString() + ">";

			m_pTreeView->AddSubTreeNode(nodeId, guid, buttonName.c_str());
		}
	}
}

void NFGodView::SubRender()
{
	m_pTreeView->Execute();
	/*
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
	*/

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

   pView->MoveToNode(mCurrentObjectID);
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
		 const std::string& className = m_pKernelModule->GetPropertyString(guid, NFrame::IObject::ClassName());
		 const std::string& name = m_pKernelModule->GetPropertyString(guid, NFrame::IObject::Name());
         const NFVector3& pos = m_pKernelModule->GetPropertyVector3(guid, NFrame::IObject::Position());
		 std::string barTile = className + "(" + std::to_string((int)pos.X()) + "," + std::to_string((int)pos.Z()) + ")";
         
		 pView->AddNode(guid, barTile, ToNodeSpacePos(NFVector2(pos.X(), pos.Z())));
		 pView->SetNodeDraggable(guid, false);
         //pView->AddNodeAttrIn(guid, m_pKernelModule->CreateGUID(), NFrame::IObject::Name(), strName);
         //pView->AddNodeAttrIn(guid, m_pKernelModule->CreateGUID(), NFrame::IObject::Position(), pos.ToString());
      }
   }
}

void NFGodView::UpdateSceneObjectNodePosition(const int sceneID, const int groupID)
{
	NFNodeView* pView = (NFNodeView*)m_pNodeView;

	if (sceneID > 0 && groupID > 0)
	{
		//objects
		NFDataList list;
		m_pKernelModule->GetGroupObjectList(sceneID, groupID, list);
		for (int k = 0; k < list.GetCount(); ++k)
		{
			const NFGUID& guid = list.Object(k);
			const std::string& className = m_pKernelModule->GetPropertyString(guid, NFrame::IObject::ClassName());
			const std::string& name = m_pKernelModule->GetPropertyString(guid, NFrame::IObject::Name());
			const NFVector3& pos = m_pKernelModule->GetPropertyVector3(guid, NFrame::IObject::Position());
			std::string barTile = className + "(" + std::to_string((int)pos.X()) + "," + std::to_string((int)pos.Z()) + ")";

			pView->SetNodePosition(guid, ToNodeSpacePos(NFVector2(pos.X(), pos.Z())));
		}
	}
}
