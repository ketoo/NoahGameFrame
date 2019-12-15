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
   ImGui::Checkbox("Debug", &logDebug);
   ImGui::SameLine();
   ImGui::Checkbox("Info", &logInfo);
   ImGui::SameLine();
   ImGui::Checkbox("Waring", &logWaring);
   ImGui::SameLine();
   ImGui::Checkbox("Error", &logError);
   ImGui::SameLine();
   ImGui::Checkbox("Fatal", &logFatal);

   ImGui::SameLine();
   if (ImGui::Button("Clear log"))
   {
      mLogData.clear();
   }

   ImGui::SameLine();
   static std::string searchLog;
   static char str0[64] = "";
   if (ImGui::InputText("", str0, IM_ARRAYSIZE(str0)))
   {
      searchLog = str0;
   }

   ImGui::SetNextItemWidth(-1.0f);
   if (ImGui::ListBoxHeader("##", -1, -1))
   {
      static int selected = 0;
      for (int i = 0; i < mLogData.size(); i++)
      {
         const LogData& item_name = mLogData[i];
         if (item_name.level == NFILogModule::NLL_DEBUG_NORMAL && logDebug)
         {
            //colour-->white
            bool render = true;
            if (!searchLog.empty())
            {
               if (item_name.data.find_first_of(searchLog) < 0)
               {
                  render = false;
               } 
            }
 
            if (render)
            {
               if (ImGui::Selectable(item_name.data.c_str(), selected == i))
               {
                  selected = i;
                  // handle selection
               }
            }
         }
         else if (item_name.level == NFILogModule::NLL_INFO_NORMAL && logInfo)
         {
            //colour-->white
            bool render = true;
            if (!searchLog.empty())
            {
               if (item_name.data.find_first_of(searchLog) < 0)
               {
                  render = false;
               } 
            }
 
            if (render)
            {
               if (ImGui::Selectable(item_name.data.c_str(), selected == i))
               {
                  selected = i;
                  // handle selection
               }
            }
         }
         else if (item_name.level == NFILogModule::NLL_WARING_NORMAL && logWaring)
         {
            //colour-->yellow
            bool render = true;
            if (!searchLog.empty())
            {
               if (item_name.data.find_first_of(searchLog) < 0)
               {
                  render = false;
               } 
            }
 
            if (render)
            {
               if (ImGui::Selectable(item_name.data.c_str(), selected == i))
               {
                  selected = i;
                  // handle selection
               }
            }
         }
         else if (item_name.level == NFILogModule::NLL_ERROR_NORMAL && logError)
         {
            //colour-->red
            bool render = true;
            if (!searchLog.empty())
            {
               if (item_name.data.find_first_of(searchLog) < 0)
               {
                  render = false;
               } 
            }
 
            if (render)
            {
               if (ImGui::Selectable(item_name.data.c_str(), selected == i))
               {
                  selected = i;
                  // handle selection
               }
            }
         }
         else if (item_name.level == NFILogModule::NLL_FATAL_NORMAL && logFatal)
         {
            //colour-->red
            bool render = true;
            if (!searchLog.empty())
            {
               if (item_name.data.find_first_of(searchLog) < 0)
               {
                  render = false;
               } 
            }
 
            if (render)
            {
               if (ImGui::Selectable(item_name.data.c_str(), selected == i))
               {
                  selected = i;
                  // handle selection
               }
            }
         }
         //ImGui::Text("%s", mLogData[n].c_str());
      }
         
      ImGui::ListBoxFooter();
   //ImGui::PushItemWidth(-1);
   //ImGui::PopItemWidth();

   }

	return false;
}

void NFConsoleView::Hooker(const NFILogModule::NF_LOG_LEVEL level, const std::string& str)
{
   mLogData.push_back(LogData(level, str));
}