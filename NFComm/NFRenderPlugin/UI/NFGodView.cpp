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

	//m_pNodeView->SetHoverNodeCallBack(std::bind(&NFGodView::HandlerNodeHovered, this, std::placeholders::_1));
	mNodeSystem.SetBeginRenderCallBack(std::bind(&NFGodView::DrawMapData, this));

	mTreeView.SetSelectedNodeFunctor(std::bind(&NFGodView::HandlerSelected, this, std::placeholders::_1, std::placeholders::_2));
	mTreeView.SetName(GET_CLASS_NAME(NFGodView));

	for (int i = 0; i <= 10; ++i)
	{
		mLayerColor.push_back(ImColor(255, 255, 255, 100));
	}
	//mLayerColor[0] = 0;
	//mLayerColor[1] = 0;
	mLayerColor[2] = ImColor(255, 255, 255, 100);
	mLayerColor[3] = ImColor(255, 255, 255, 170);
	mLayerColor[4] = ImColor(241, 170, 0, 100);
	mLayerColor[5] = ImColor(241, 170, 0, 200);
	mLayerColor[6] = ImColor(228, 110, 0, 200);
	mLayerColor[7] = ImColor(241, 170, 170, 200);
	mLayerColor[8] = ImColor(228, 110, 110, 200);
	mLayerColor[9] = ImColor(219, 78, 78, 200);
	mLayerColor[10] = ImColor(255, 78, 0, 255);

	mStairColor = ImColor(0, 255, 255, 255);
	mTextColor = ImColor(255, 255, 255, 100);
	mImmovable = ImColor(255, 0, 0, 255);
}

NFGodView::~NFGodView()
{
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
	   mNodeSystem.ResetToCenter();
   }

	//draw map data as the background
   //1. let the walkable area empty
   //2. draw the unwalkable area with unfill rectangle (should be a cross in the center of the rectangle)
   //3. draw the item area with unfill circle (should be a cross in the center of the rectangle)
   //4. draw the stair with a sign
   //5 how to tell the user about the height of the map

	mNodeSystem.Execute();

	UpdateSceneObjectNodePosition(mSceneID, mGroupID);

	DrawToolBar();

	if (mSceneID > 0 && mGroupID)
	{
		if (ImGui::IsWindowHovered() && !ImGui::IsAnyItemActive() && ImGui::IsMouseClicked(0))
		{
			if (!mCurrentObjectID.IsNull())
			{
				ImVec2 wPos = ImGui::GetWindowPos();
				ImVec2 wSize = ImGui::GetWindowSize();
				NFVector2 offset = mNodeSystem.GetOffset();
				ImGuiIO& io = ImGui::GetIO();


				NFVector2 displayOffset(mNodeSystem.GetNodeSize() / 2, mNodeSystem.GetNodeSize() / 2);
				NFVector2 vec(io.MousePos.x - wPos.x - offset.X() - displayOffset.X(), io.MousePos.y - offset.Y() - displayOffset.Y());

				mClickedPos = NFVector2(vec.X() / mNodeSystem.GetNodeSize(), vec.Y() / mNodeSystem.GetNodeSize());

				m_pKernelModule->SetPropertyVector3(mCurrentObjectID, NFrame::IObject::GMMoveTo(), NFVector3(mClickedPos.X(), 0, -mClickedPos.Y()));
			}
		}
	}

	auto posNode = mNodeSystem.FindNode(NFGUID());
	if (!posNode)
	{
		NFVector2 pos = ToMapGridPos(NFVector3(mClickedPos.X(), 0, -mClickedPos.Y()));
		mNodeSystem.AddNode(NFGUID(), "Pos", pos, mStairColor);
	}
	else
	{
		posNode->pos = ToMapGridPos(NFVector3(mClickedPos.X(), 0, mClickedPos.Y()));
		std::string nodeName = "Pos(" + std::to_string((int)posNode->pos.X()) + "," + std::to_string((int)posNode->pos.Y()) + ")";
		posNode->name = nodeName;

	}

	return true;
}

void NFGodView::HandlerSelected(const NFGUID& id, const bool doubleClick)
{
	SetCurrentObjectID(id);

	if (doubleClick)
	{
		mNodeSystem.MoveToNode(mCurrentObjectID);
	}
	else
	{
		mNodeSystem.SelectNode(mCurrentObjectID);
	}

	//occupy inspectorview
	NF_SHARE_PTR<NFIView> pHierachyView = m_pUIModule->GetView(NFViewType::HierachyView);
	NF_SHARE_PTR<NFIView> pInspectorView = m_pUIModule->GetView(NFViewType::InspectorView);
	if (pHierachyView && pInspectorView)
	{
		pInspectorView->OccupySubRender(pHierachyView.get());
	}
}

void NFGodView::DrawMapData()
{
	if (mSceneID > 0 && mGroupID > 0)
	{
		auto data = m_pNavigationDataModule->GetMapData(mSceneID, 0);
		if (data)
		{
			int nodeSize = mNodeSystem.GetNodeSize();

			//draw walkable

			for (int i = 0; i < data->tileConfig.mapSize; i++)
			{
				for (int j = 0; j < data->tileConfig.mapSize; j++)
				{
					auto voxel = data->data.GetElement(NFGUID(i, j));
					if (voxel)
					{
						NFVector2 v1(i * nodeSize * data->tileConfig.cellSizeX, j * -nodeSize * data->tileConfig.cellSizeZ);
						NFVector2 v2(v1.X() + nodeSize * data->tileConfig.cellSizeX, v1.Y() - nodeSize * data->tileConfig.cellSizeX);

						if (voxel->layer > 1)
						{
							int color = mLayerColor[voxel->layer];
							mNodeSystem.DrawRectFilled(v1, v2, color);
						}
						{
							
							//DrawRectFilled(v1, v2, color);
							if (voxel->movable <= 0)
							{
								mNodeSystem.DrawCircle((v1 + v2) / 2, mNodeSystem.GetNodeSize() * 0.75, mImmovable);
							}
							if (voxel->stair_h > 0)
							{
								mNodeSystem.DrawRect(v1, v2, mStairColor);
								mNodeSystem.DrawText(NFVector2(v1.X(), v2.Y()), mStairColor, " ||||");
							}
							if (voxel->stair_v > 0)
							{
								mNodeSystem.DrawRect(v1, v2, mStairColor);
								mNodeSystem.DrawText(NFVector2(v1.X(), v2.Y()), mStairColor, " ==");
							}
							if (!voxel->occupyObject.IsNull())
							{
								mNodeSystem.DrawText(NFVector2(v1.X(), v2.Y()), ImColor(255, 0, 0, 255), "X");
							}
							if (!voxel->item.empty())
							{
							}
						}
					}
				}
			}

			for (int i = 0; i < data->tileConfig.mapSize; i++)
			{
				if (i % 5 == 0)
				{
					NFVector2 v1(i * nodeSize * data->tileConfig.cellSizeX, 0);
					std::string text = std::to_string(i * data->tileConfig.cellSizeX);
					mNodeSystem.DrawText(v1, mTextColor, text.c_str());
				}
			}
			for (int j = 0; j < data->tileConfig.mapSize; j++)
			{
				if (j % 5 == 0)
				{
					NFVector2 v1(-nodeSize *2, j * nodeSize * -data->tileConfig.cellSizeX);
					std::string text = std::to_string(j * data->tileConfig.cellSizeX);
					mNodeSystem.DrawText(v1, mTextColor, text.c_str());
				}
			}

			//draw background
			NFVector2 v1(0, 0);
			NFVector2 v2(nodeSize * data->tileConfig.mapSize * data->tileConfig.cellSizeX, nodeSize * -data->tileConfig.mapSize * data->tileConfig.cellSizeZ);

			mNodeSystem.DrawRect(v1, v2, mImmovable);
		}
	}
}

void NFGodView::DrawToolBar()
{
	auto wPos = ImGui::GetWindowPos();
	auto wSize = ImGui::GetWindowSize();

	ImVec2 start(12, wPos.y + 50);
	ImVec2 end(start.x + 15, start.y + 15);
	ImGui::GetWindowDrawList()->AddRect(start, end, mStairColor);

	start = ImVec2(35, wPos.y + 50);
	ImGui::GetWindowDrawList()->AddText(start, mStairColor, "Stair");

	start = ImVec2(20, wPos.y + 80);
	ImGui::GetWindowDrawList()->AddCircle(start, 10, mImmovable);

	start = ImVec2(35, wPos.y + 70);
	ImGui::GetWindowDrawList()->AddText(start, mImmovable, "Immovable");

	ImGui::NewLine();
	ImGui::NewLine();

	for (int i = 2; i < mLayerColor.size(); ++i)
	{
		std::string name = "L " + std::to_string(i);
		ImGui::PushStyleColor(ImGuiCol_Button, mLayerColor[i]);
		ImGui::Button(name.c_str());
		ImGui::PopStyleColor();
	}


}

void NFGodView::OnMapClicked(const NFVector3& pos)
{

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

void NFGodView::RenderScene(const int sceneID, const int groupID)
{
	mTreeView.Clear();

	if (groupID < 0)
	{
		const std::vector<int>& groups = m_pSceneModule->GetGroups(sceneID);
		for (int j = 0; j < groups.size(); j++)
		{
			int group = groups[j];

			NFGUID nodeId = NFGUID(sceneID, group);
			mTreeView.AddTreeNode(nodeId, nodeId.ToString());

			//objects
			NFDataList list;
			m_pKernelModule->GetGroupObjectList(sceneID, group, list);
			for (int k = 0; k < list.GetCount(); ++k)
			{
				const NFGUID& guid = list.Object(k);
				const std::string& strClassName = m_pKernelModule->GetPropertyString(guid, NFrame::IObject::ClassName());
				std::string buttonName = strClassName + "<" + guid.ToString() + ">";

				mTreeView.AddSubTreeNode(nodeId, guid, buttonName.c_str());
			}
		}
	}
	else
	{
		NFGUID nodeId = NFGUID(sceneID, groupID);
		mTreeView.AddTreeNode(nodeId, nodeId.ToString());

		//objects
		NFDataList list;
		m_pKernelModule->GetGroupObjectList(sceneID, groupID, list);
		for (int k = 0; k < list.GetCount(); ++k)
		{
			const NFGUID& guid = list.Object(k);
			const std::string& strClassName = m_pKernelModule->GetPropertyString(guid, NFrame::IObject::ClassName());
			std::string buttonName = strClassName + "<" + guid.ToString() + ">";

			mTreeView.AddSubTreeNode(nodeId, guid, buttonName.c_str());
		}
	}
}

void NFGodView::SubRender()
{
	mTreeView.Execute();
}

NFGUID NFGodView::GetCurrentObjectID()
{
   return mCurrentObjectID;
}

void NFGodView::SetCurrentObjectID(const NFGUID& id)
{
	mCurrentObjectID = id;
	const NFVector3& pos = m_pKernelModule->GetPropertyVector3(id, NFrame::IObject::GMMoveTo());
	mClickedPos = NFVector2(pos.X(), pos.Z());
}

void NFGodView::RenderSceneObjectNode(const int sceneID, const int groupID)
{
   mNodeSystem.CleanNodes();
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

		 auto node = mNodeSystem.AddNode(guid, nodeName, ToMapGridPos(pos));
      }
   }
}

void NFGodView::UpdateSceneObjectNodePosition(const int sceneID, const int groupID)
{
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
				//std::string barTile = className.substr(0, 1) + "(" + std::to_string((int)pos.X()) + "," + std::to_string((int)pos.Z()) + ")";
				std::string barTile = std::to_string(m_pKernelModule->GetPropertyInt(guid, NFrame::NPC::HP()));

				auto node = mNodeSystem.FindNode(guid);
				if (node)
				{
					node->pos = ToMapGridPos(pos);
					node->name = barTile;
				}

				if (guid == mCurrentObjectID)
				{
					const NFVector3& pos = m_pKernelModule->GetPropertyVector3(guid, NFrame::IObject::GMMoveTo());
					mClickedPos = NFVector2(pos.X(), pos.Z());
				}
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
				mTreeView.DeleteTreeNode(self);

				mNodeSystem.DeleteNode(self);
			}
			else
			{
				mTreeView.DeleteTreeNode(self);
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

			mTreeView.AddSubTreeNode(NFGUID(mSceneID, mGroupID), self, buttonName.c_str());

			//node

			const std::string& name = m_pKernelModule->GetPropertyString(self, NFrame::IObject::Name());
			const NFVector3& pos = m_pKernelModule->GetPropertyVector3(self, NFrame::IObject::Position());
			std::string nodeName = className.substr(0, 1) + "(" + std::to_string((int)pos.X()) + "," + std::to_string((int)pos.Z()) + ")";

			NFVector2 nodePos = ToMapGridPos(pos);
			auto node = mNodeSystem.AddNode(self, nodeName, nodePos);
		}
	}

	return 0;
}

NFVector2 NFGodView::ToMapGridPos(const NFVector3& pos)
{
	return NFVector2(pos.X() * mNodeSystem.GetNodeSize(), pos.Z() * -mNodeSystem.GetNodeSize());
}