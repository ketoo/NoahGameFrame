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

#include "NFConsoleView.h"
#include "NFUIModule.h"
#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"

NFConsoleView::NFConsoleView(NFIPluginManager* p, NFViewType vt) : NFIView(p, vt, GET_CLASS_NAME(NFConsoleView))
{
	m_pLogModule = pPluginManager->FindModule<NFILogModule>();
   m_pElementModule = pPluginManager->FindModule<NFIElementModule>();
   m_pClassModule = pPluginManager->FindModule<NFIClassModule>();
   m_pUIModule = pPluginManager->FindModule<NFIUIModule>();

   m_pLogModule->SetHooker(this, &NFConsoleView::Hooker);
}

bool NFConsoleView::Execute()
{
	//1. the project root folder is NFDataCfg

      ImGui::SetNextItemWidth(-1.0f);
      if (ImGui::ListBoxHeader("##", -1, -1))
      {
         for (int n = 0; n < mLogData.size(); n++)
         {
            const std::string& item_name = mLogData[n];
            if (ImGui::Selectable(item_name.c_str(), true))
            {
               // handle selection
            }

            //ImGui::Text("%s", mLogData[n].c_str());
         }
            
         ImGui::ListBoxFooter();
      //ImGui::PushItemWidth(-1);
      //ImGui::PopItemWidth();

      }

	return false;
}

void NFConsoleView::Hooker(const std::string& str)
{
   mLogData.push_back(str);
}