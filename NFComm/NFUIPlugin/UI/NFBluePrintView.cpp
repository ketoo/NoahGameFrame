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

   int testID1 = m_pNodeView->GenerateId();
   int testID2 = m_pNodeView->GenerateId();
   int testID3 = m_pNodeView->GenerateId();
   int testID4 = m_pNodeView->GenerateId();

   m_pNodeView->AddNode(testID1, "testnode1111111");
   m_pNodeView->AddNode(testID2, "testnode2222222");
   m_pNodeView->AddNode(testID3, "testnode333333");
   m_pNodeView->AddNode(testID4, "testnode4444444");

   for (int i = 0; i < 4; ++i)
   {
      m_pNodeView->AddNodeAttrOut(testID1, m_pNodeView->GenerateId(), "Out1");
      m_pNodeView->AddNodeAttrOut(testID2, m_pNodeView->GenerateId(), "Out1");
      m_pNodeView->AddNodeAttrOut(testID3, m_pNodeView->GenerateId(), "Out1");

      m_pNodeView->AddNodeAttrIn(testID1, m_pNodeView->GenerateId(), "In1");
      m_pNodeView->AddNodeAttrIn(testID2, m_pNodeView->GenerateId(), "In12");
      m_pNodeView->AddNodeAttrIn(testID3, m_pNodeView->GenerateId(), "In13");
   }
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
   if (ImGui::Button("+ NFBluePrint Block"))
   {
      auto pLogicBlock = m_pBluePrintModule->CreateLogicBlock();
      if (pLogicBlock)
      {
         
      }
   }

   ImGui::SameLine();
   if (ImGui::Button("+ NFMonitor"))
   {

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
   if (ImGui::Button("- links"))
   {
   }

   ImGui::SameLine();
   if (ImGui::Button("- nodes"))
   {
   }

   ImGui::SameLine();
   if (ImGui::Button("return center"))
   {
      m_pNodeView->ResetOffestZero();
   }

   m_pNodeView->Execute();
   
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

void NFBluePrintView::SubRender()
{
   ImGui::Text(this->name.c_str());
}