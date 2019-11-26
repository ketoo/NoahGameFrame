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

   m_pUIModule = pPluginManager->FindModule<NFIUIModule>();
   m_pBluePrintModule = pPluginManager->FindModule<NFIBluePrintModule>();
   m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();

   NFGUID testID1 = m_pKernelModule->CreateGUID();
   NFGUID testID2 = m_pKernelModule->CreateGUID();
   NFGUID testID3 = m_pKernelModule->CreateGUID();
   NFGUID testID4 = m_pKernelModule->CreateGUID();

   m_pNodeView->AddNode(testID1, "testnode1111111", NFVector2(0, 0));
   m_pNodeView->AddNode(testID2, "testnode2222222", NFVector2(2, 0));
   m_pNodeView->AddNode(testID3, "testnode333333", NFVector2(4, 0));
   m_pNodeView->AddNode(testID4, "testnode4444444", NFVector2(6, 0));

   m_pNodeView->AddNodeAttrOut(testID1, m_pKernelModule->CreateGUID(), "Out1");
   m_pNodeView->AddNodeAttrOut(testID1, m_pKernelModule->CreateGUID(), "Out2");
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
}

NFBluePrintView::~NFBluePrintView()
{
   delete m_pNodeView;
   m_pNodeView = nullptr;
}

bool NFBluePrintView::Execute()
{
	//1. add monitor
   //2. add judgement
   //3. add executor
   //4. delete links
   //4. delete nodes

	//5. return canvas's center  
   static std::string selectedBlock;
   if (ImGui::Button("LogicBlocks"))
   {
      ImGui::OpenPopup("my_select_LogicBlocks");
     
      if (ImGui::BeginPopup("my_select_LogicBlocks"))
      {
         std::vector<std::string> strIdList;
         auto& logicBlocks = m_pBluePrintModule->GetLogicBlocks();
         for (auto it : logicBlocks)
         {
            strIdList.push_back(it->name);
         }

         for (int n = 0; n < strIdList.size(); n++)
         {
            if (ImGui::Selectable(strIdList[n].c_str()))
            {
                selectedBlock = strIdList[n];
            }
         }
          
         ImGui::EndPopup();
      }
   }


   if (ImGui::Button("+ NFMonitor"))
   {
      if (mCurrentLogicBlock)
      {
         mCurrentLogicBlock->AddMonitor(NFIBluePrintModule::NFMonitorType::PropertyEvent, "", NFDataList::Empty());
      }
   }

   ImGui::SameLine();
   if (ImGui::Button("+ NFJudgement"))
   {
   }
   
   ImGui::SameLine();
   if (ImGui::Button("+ NFExecutor"))
   {
   }
   
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
      m_pNodeView->ResetOffestZero();
   }


   m_pNodeView->Execute();
   
   if (bCreatingLogicBlock)
   {
      if (!ImGui::BeginChild("Create Logic Block"))
      {
         static char str0[128] = "Hello, world!";
         ImGui::InputText("input text", str0, IM_ARRAYSIZE(str0));

         if (ImGui::Button("Cancel"))
         {
            bCreatingLogicBlock = false;
         }
         
         ImGui::SameLine();

         if (ImGui::Button("OK"))
         {
            m_pBluePrintModule->CreateLogicBlock(str0);
            bCreatingLogicBlock = false;
         }

         ImGui::EndChild();
      }
   }

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
}

void NFBluePrintView::SubRender()
{
   ImGui::Text(this->name.c_str());

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