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
	m_pNavigationDataModule = pPluginManager->FindModule<NFINavigationDataModule>();
	
	m_pKernelModule->RegisterCommonClassEvent(this, &NFGodView::OnClassCommonEvent);

	m_pNodeView = NF_NEW NFNodeView(p);
	m_pTreeView = NF_NEW NFTreeView(p);

    m_pNodeView->ResetOffset(NFVector2::Zero());
	m_pNodeView->SetHoverNodeCallBack(std::bind(&NFGodView::HandlerNodeHovered, this, std::placeholders::_1));
	m_pNodeView->SetBeginRenderCallBack(std::bind(&NFGodView::HandlerForBeginRender, this));

	m_pTreeView->SetSelectedNodeFunctor(std::bind(&NFGodView::HandlerSelected, this, std::placeholders::_1, std::placeholders::_2));
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
       m_pNodeView->ResetOffset(NFVector2::Zero());
   }

	//draw map data as the background
   //1. let the walkable area empty
   //2. draw the unwalkable area with unfill rectangle (should be a cross in the center of the rectangle)
   //3. draw the item area with unfill circle (should be a cross in the center of the rectangle)
   //4. draw the stair with a sign
   //5 how to tell the user about the height of the map

	m_pNodeView->Execute();

	UpdateSceneObjectNodePosition(mSceneID, mGroupID);


	return true;
}

void NFGodView::HandlerSelected(const NFGUID& id, const bool doubleClick)
{
	SetCurrentObjectID(id);

	if (doubleClick)
	{
		NFNodeView* pView = (NFNodeView*)m_pNodeView;
		pView->MoveToNode(mCurrentObjectID);
	}

	//occupy inspectorview
	NF_SHARE_PTR<NFIView> pHierachyView = m_pUIModule->GetView(NFViewType::HierachyView);
	NF_SHARE_PTR<NFIView> pInspectorView = m_pUIModule->GetView(NFViewType::InspectorView);
	if (pHierachyView && pInspectorView)
	{
		pInspectorView->OccupySubRender(pHierachyView.get());
	}
}

void NFGodView::HandlerForBeginRender()
{
	if (mSceneID > 0 && mGroupID > 0)
	{
		//draw
		auto data = m_pNavigationDataModule->GetMapData(mSceneID, 0);
		if (data)
		{
			for (int i = 0; i < data->tileConfig.mapSize; ++i)
			{
				for (int j = 0; j < data->tileConfig.mapSize; ++j)
				{
					auto voxel = data->data.GetElement(NFGUID(i, j));
					if (voxel && voxel->movable <= 0)
					{
						if (voxel->layer > 1)
						{
							int color = NFColor::DEFAULT;
							//int size = imnodes::GetStyle().grid_spacing / 2;

							if (voxel->layer == 3)
							{
								color = NFColor::WORKFLOW;
							}
							else if (voxel->layer == 4)
							{
								color = NFColor::PININ;
							}

							if ( i < 5 && j < 5)
							{
								m_pNodeView->SetCurrentContext();

								int cellSize = imnodes::GetStyle().grid_spacing;
								ImVec2 v(i * cellSize, j * -cellSize);
								imnodes::AddRectFilled(v, ImVec2(imnodes::GetStyle().grid_spacing, imnodes::GetStyle().grid_spacing), color);
							}
						}
					}
				}
			}
		}
	}
}

bool NFGodView::HandlerNodeHovered(const NFGUID& id)
{
	const std::string& className = m_pKernelModule->GetPropertyString(id, NFrame::IObject::ClassName());
	const std::string& name = m_pKernelModule->GetPropertyString(id, NFrame::IObject::Name());
	const NFVector3& pos = m_pKernelModule->GetPropertyVector3(id, NFrame::IObject::Position());

	std::string propertyTitle = className;
	if (name.length() > 0)
	{
		propertyTitle  = className +"[" + name + "]";
	}

	std::string propertyPos =  + "Position:" + std::to_string((int)pos.X()) + "," + std::to_string((int)pos.Y()) + "," + std::to_string((int)pos.Z());

	//pop up windows

	ImGui::BeginTooltip();
	ImGui::Text(propertyTitle.c_str());
	ImGui::Text(id.ToString().c_str());
	ImGui::Separator();
	ImGui::Text(propertyPos.c_str());
	ImGui::EndTooltip();

	return true;
}

NFVector2 NFGodView::ToNodeSpacePos(const NFVector2& v)
{
	int size = imnodes::GetStyle().grid_spacing / 2;
	int offset = imnodes::GetStyle().grid_spacing / 4;
	return NFVector2(v.X() * size - offset, v.Y() * -size - offset);
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
		 std::string nodeName = className.substr(0, 1) + "(" + std::to_string((int)pos.X()) + "," + std::to_string((int)pos.Z()) + ")";
         
		 auto node = pView->AddNode(guid, nodeName, NFColor::DEFAULT, ToNodeSpacePos(NFVector2(pos.X(), -pos.Z())));
		 node->title = false;

		 pView->SetNodeDraggable(guid, false);
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
			if (m_pKernelModule->ExistObject(guid))
			{
				const std::string& className = m_pKernelModule->GetPropertyString(guid, NFrame::IObject::ClassName());
				const std::string& name = m_pKernelModule->GetPropertyString(guid, NFrame::IObject::Name());
				const NFVector3& pos = m_pKernelModule->GetPropertyVector3(guid, NFrame::IObject::Position());
				std::string barTile = className + "(" + std::to_string((int)pos.X()) + "," + std::to_string((int)pos.Z()) + ")";

				pView->SetNodePosition(guid, ToNodeSpacePos(NFVector2(pos.X(), pos.Z())));
			}
			else
			{
				//log
			}
		}
	}
}

int NFGodView::OnClassCommonEvent(const NFGUID& self, const std::string& strClassNames, const CLASS_OBJECT_EVENT eClassEvent, const NFDataList& var)
{
	if (eClassEvent == CLASS_OBJECT_EVENT::COE_DESTROY)
	{
		const int scene = m_pKernelModule->GetPropertyInt(self, NFrame::IObject::SceneID());
		const int group = m_pKernelModule->GetPropertyInt(self, NFrame::IObject::GroupID());

		if (scene == mSceneID)
		{
			if (group == mGroupID)
			{
				m_pTreeView->DeleteTreeNode(self);

				NFNodeView* pView = (NFNodeView*)m_pNodeView;
				pView->DeleteNode(self);
			}
			else
			{
				m_pTreeView->DeleteTreeNode(self);
			}
		}
	}
	else if (eClassEvent == CLASS_OBJECT_EVENT::COE_CREATE_FINISH)
	{
		int scene = m_pKernelModule->GetPropertyInt(self, NFrame::IObject::SceneID());
		int group = m_pKernelModule->GetPropertyInt(self, NFrame::IObject::GroupID());

		if (scene == mSceneID && group == mGroupID)
		{
			const std::string& className = m_pKernelModule->GetPropertyString(self, NFrame::IObject::ClassName());

			//tree
			std::string buttonName = className + "<" + self.ToString() + ">";

			m_pTreeView->AddSubTreeNode(NFGUID(mSceneID, mGroupID), self, buttonName.c_str());

			//node

			const std::string& name = m_pKernelModule->GetPropertyString(self, NFrame::IObject::Name());
			const NFVector3& pos = m_pKernelModule->GetPropertyVector3(self, NFrame::IObject::Position());
			std::string nodeName = className.substr(0, 1) + "(" + std::to_string((int)pos.X()) + "," + std::to_string((int)pos.Z()) + ")";

			NFNodeView* pView = (NFNodeView*)m_pNodeView;
			auto node = pView->AddNode(self, nodeName, NFColor::DEFAULT, ToNodeSpacePos(NFVector2(pos.X(), pos.Z())));
			node->title = false;

			pView->SetNodeDraggable(self, false);
		}
	}

	return 0;
}