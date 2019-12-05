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
   m_pNodeView = NF_NEW NFNodeView(p);
   m_pTreeView = NF_NEW NFTreeView(p);

   m_pTreeView->SetName("BluePrint LogicBlocks");

   m_pUIModule = pPluginManager->FindModule<NFIUIModule>();
   m_pBluePrintModule = pPluginManager->FindModule<NFIBluePrintModule>();
   m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();

   NFGUID testID1 = m_pKernelModule->CreateGUID();
   NFGUID testID2 = m_pKernelModule->CreateGUID();
   NFGUID testID3 = m_pKernelModule->CreateGUID();
   NFGUID testID4 = m_pKernelModule->CreateGUID();


   NFGUID tree111 = m_pKernelModule->CreateGUID();
   NFGUID tree222 = m_pKernelModule->CreateGUID();
   NFGUID tree333 = m_pKernelModule->CreateGUID();
   NFGUID tree444 = m_pKernelModule->CreateGUID();


   m_pTreeView->AddTreeNode(tree111, "tree111");
   m_pTreeView->AddTreeNode(tree222, "tree222");
   m_pTreeView->AddTreeNode(tree333, "tree333");
   m_pTreeView->AddTreeNode(tree444, "tree444");

   m_pNodeView->AddNode(testID1, "testnode1111111", NFVector2(300, 300));
   m_pNodeView->AddNode(testID2, "testnode2222222", NFVector2(3, 0));
   //m_pNodeView->AddNode(testID3, "testnode333333", NFVector2(200, 0));
   //m_pNodeView->AddNode(testID4, "testnode4444444", NFVector2(300, 0));

   //m_pNodeView->AddNodeAttrOut(testID1, m_pKernelModule->CreateGUID(), "Out1");
   //m_pNodeView->AddNodeAttrOut(testID1, m_pKernelModule->CreateGUID(), "Out2");
  // m_pNodeView->AddNodeAttrIn(testID2, m_pKernelModule->CreateGUID(), "In12");
   //m_pNodeView->AddNodeAttrIn(testID2, m_pKernelModule->CreateGUID(), "In123222");
   /*
   m_pNodeView->AddNodeAttrOut(testID2, m_pKernelModule->CreateGUID(), "Out1");
   m_pNodeView->AddNodeAttrOut(testID2, m_pKernelModule->CreateGUID(), "Out2");
   m_pNodeView->AddNodeAttrOut(testID3, m_pKernelModule->CreateGUID(), "Out1");
   m_pNodeView->AddNodeAttrOut(testID3, m_pKernelModule->CreateGUID(), "Out2");

   m_pNodeView->AddNodeAttrIn(testID1, m_pKernelModule->CreateGUID(), "In1");
   m_pNodeView->AddNodeAttrIn(testID1, m_pKernelModule->CreateGUID(), "In2");
   m_pNodeView->AddNodeAttrIn(testID2, m_pKernelModule->CreateGUID(), "In12");
   m_pNodeView->AddNodeAttrIn(testID2, m_pKernelModule->CreateGUID(), "In123222");
   m_pNodeView->AddNodeAttrIn(testID3, m_pKernelModule->CreateGUID(), "In13");
   m_pNodeView->AddNodeAttrIn(testID3, m_pKernelModule->CreateGUID(), "In133333");
   */
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
	//1. add monitor
   //2. add judgement
   //3. add executor
   //4. delete links
   //4. delete nodes

	//5. return canvas's center  

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
	   m_pNodeView->ResetOffest(NFVector2(-5, -109));
	   //m_pNodeView->ResetOffest(NFVector2::Zero());
   }


   m_pNodeView->Execute();
   
   CreateLogicBlockWindow();

   if (ImGui::IsWindowFocused())
   {
      NF_SHARE_PTR<NFIView> pView = m_pUIModule->GetView(NFViewType::HierachyView);
      if (pView)
      {
         pView->OccupySubRender(this);
      }
   }

   ImGuiIO& io = ImGui::GetIO();
   if (io.WantCaptureMouse)
   {

   }

	return false;
}

void NFBluePrintView::SetCurrentLogicBlock(NF_SHARE_PTR<NFIBluePrintModule::NFLogicBlock> logicBlock)
{
   mCurrentLogicBlock = logicBlock;

   mCurrentMonitor = nullptr;
   mCurrentExecuter = nullptr;
   mCurrentJudgement = nullptr;
   //occupy 
}

void NFBluePrintView::SetCurrentMonitor(NF_SHARE_PTR<NFIBluePrintModule::NFMonitor> monitor)
{
	mCurrentMonitor = monitor;
	mCurrentExecuter = nullptr;
	mCurrentJudgement = nullptr;
	//occupy 
}

void NFBluePrintView::SetCurrentJudgement(NF_SHARE_PTR<NFIBluePrintModule::NFJudgement> judgement)
{
	mCurrentMonitor = nullptr;
	mCurrentJudgement = judgement;
	//occupy 
}

void NFBluePrintView::SetCurrentExecuter(NF_SHARE_PTR<NFIBluePrintModule::NFExecuter> executer)
{
	mCurrentExecuter = executer;

	//occupy 
}

void NFBluePrintView::SubRender()
{
   ImGui::Text(this->name.c_str());

   m_pTreeView->Execute();

   const std::list<NF_SHARE_PTR<NFIBluePrintModule::NFLogicBlock>>& logicBlocks = m_pBluePrintModule->GetLogicBlocks();
   for (auto it : logicBlocks)
   {
      std::string logicBlockNodeName = "LogicBlock<" + it->name + ">";
      if (ImGui::TreeNode(logicBlockNodeName.c_str()))
      {
         for (auto monitor : it->monitors)
         {
            SubMonitorRender(monitor);
         }

         ImGui::TreePop();
      }
   }
   if (ImGui::TreeNode("Basic trees"))
   {
	   for (int i = 0; i < 5; i++)
	   {
		   // Use SetNextItemOpen() so set the default state of a node to be open.
		   // We could also use TreeNodeEx() with the ImGuiTreeNodeFlags_DefaultOpen flag to achieve the same thing!
		   if (i == 0)
			   ImGui::SetNextItemOpen(true, ImGuiCond_Once);

		   if (ImGui::TreeNode((void*)(intptr_t)i, "Child %d", i))
		   {
			   ImGui::Text("blah blah");
			   ImGui::SameLine();
			   if (ImGui::SmallButton("button")) {};
			   ImGui::TreePop();
		   }
	   }
	   ImGui::TreePop();
   }

   if (ImGui::TreeNode("Advanced, with Selectable nodes"))
   {
	   static int selection_mask = (1 << 2); // Dumb representation of what may be user-side selection state. You may carry selection state inside or outside your objects in whatever format you see fit.
	   int node_clicked = -1;                // Temporary storage of what node we have clicked to process selection at the end of the loop. May be a pointer to your own node type, etc.
	   for (int i = 0; i < 6; i++)
	   {
		   // Disable the default open on single-click behavior and pass in Selected flag according to our selection state.
		   ImGuiTreeNodeFlags node_flags = 0;
		   const bool is_selected = (selection_mask & (1 << i)) != 0;
		   if (is_selected)
			   node_flags |= ImGuiTreeNodeFlags_Selected;
		   if (i < 3)
		   {
			   // Items 0..2 are Tree Node
			   bool node_open = ImGui::TreeNodeEx((void*)(intptr_t)i, node_flags, "Selectable Node %d", i);
			   if (ImGui::IsItemClicked())
			   {
				   node_clicked = i;
			   }

			   if (node_open)
			   {
				   ImGui::BulletText("Blah blah\nBlah Blah");
				   ImGui::TreePop();
			   }
		   }
		   else
		   {
			   // Items 3..5 are Tree Leaves
			   // The only reason we use TreeNode at all is to allow selection of the leaf.
			   // Otherwise we can use BulletText() or advance the cursor by GetTreeNodeToLabelSpacing() and call Text().
			   node_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen; // ImGuiTreeNodeFlags_Bullet
			   ImGui::TreeNodeEx((void*)(intptr_t)i, node_flags, "Selectable Leaf %d", i);
			   if (ImGui::IsItemClicked())
				   node_clicked = i;
		   }
	   }
	   if (node_clicked != -1)
	   {
		   // Update selection state. Process outside of tree loop to avoid visual inconsistencies during the clicking-frame.
		   if (ImGui::GetIO().KeyCtrl)
			   selection_mask ^= (1 << node_clicked);          // CTRL+click to toggle
		   else //if (!(selection_mask & (1 << node_clicked))) // Depending on selection behavior you want, this commented bit preserve selection when clicking on item that is part of the selection
			   selection_mask = (1 << node_clicked);           // Click to single-select
	   }

	   ImGui::TreePop();
   }

	if (ImGui::TreeNode("Collapsing Headers"))
	{
		static bool closable_group = true;
		ImGui::Checkbox("Show 2nd header", &closable_group);
		if (ImGui::CollapsingHeader("Header", ImGuiTreeNodeFlags_None))
		{
			ImGui::Text("IsItemHovered: %d", ImGui::IsItemHovered());
			for (int i = 0; i < 5; i++)
				ImGui::Text("Some content %d", i);
		}
		if (ImGui::CollapsingHeader("Header with a close button", &closable_group))
		{
			ImGui::Text("IsItemHovered: %d", ImGui::IsItemHovered());
			for (int i = 0; i < 5; i++)
				ImGui::Text("More content %d", i);
		}
		/*
		if (ImGui::CollapsingHeader("Header with a bullet", ImGuiTreeNodeFlags_Bullet))
			ImGui::Text("IsItemHovered: %d", ImGui::IsItemHovered());
		*/
		ImGui::TreePop();
	}
}

void NFBluePrintView::SubMonitorRender(NF_SHARE_PTR<NFIBluePrintModule::NFMonitor> monitor)
{
   if (ImGui::TreeNode(monitor->name.c_str()))
   {
      for (auto judgement : monitor->judgements)
      {
         SubJudgementRender(judgement);
         
      }
      /*

      //occupy inspectorview
      NF_SHARE_PTR<NFIView> pHierachyView = m_pUIModule->GetView(NFViewType::HierachyView);
      NF_SHARE_PTR<NFIView> pInspectorView = m_pUIModule->GetView(NFViewType::InspectorView);
      if (pHierachyView && pInspectorView)
      {
         pInspectorView->OccupySubRender(pHierachyView.get());
      }
      */
         
      ImGui::TreePop();
   }
}

void NFBluePrintView::SubJudgementRender(NF_SHARE_PTR<NFIBluePrintModule::NFJudgement> judgement)
{
   if (ImGui::TreeNode(judgement->name.c_str()))
   {
      if (judgement->nextJudgement)
      {
         SubJudgementRender(judgement->nextJudgement);
      }

      for (auto executer : judgement->executers)
      {
         SubExecuterRender(executer.second);
      }

      ImGui::TreePop();
   }
}

void NFBluePrintView::SubExecuterRender(NF_SHARE_PTR<NFIBluePrintModule::NFExecuter> executer)
{
   if (ImGui::TreeNode(executer->name.c_str()))
   {
      for (auto executer1 : executer->executers)
      {
         SubExecuterRender(executer1.second);
      }

      ImGui::TreePop();
   }
}

void NFBluePrintView::TryToCreateBluePrintBlock()
{
   if (!bCreatingLogicBlock)
   {
      bCreatingLogicBlock = true;
   }
}

 void NFBluePrintView::CreateLogicBlockWindow()
 {
   if (bCreatingLogicBlock)
   {
      ImGui::OpenPopup("CreatingLogicBlock");
      ImGui::SetNextWindowSize(ImVec2(230, 150));

      if (ImGui::BeginPopupModal("CreatingLogicBlock"))
      {
         static char str0[128] = "Hello, world!";
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
            m_pBluePrintModule->CreateLogicBlock(str0);
            bCreatingLogicBlock = false;
            ImGui::CloseCurrentPopup();
         }

         ImGui::EndPopup();
      }
   }
 }