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

#include "NFGameView.h"
#include "NFUIModule.h"


NFGameView::NFGameView(NFIPluginManager* p, NFViewType vt) : NFIView(p, vt, GET_CLASS_NAME(NFGameView))
{

}

bool NFGameView::Execute()
{
	//1. the project root folder is NFDataCfg
    ImVec2 offset;

   if (ImGui::IsWindowFocused())
   {

   }

   if (ImGui::IsMouseDragging(0))
   {
       offset.x += ImGui::GetIO().MouseDelta.x;
       offset.y += ImGui::GetIO().MouseDelta.y;
   }

   int cellSize = 16;
   ImVec2 v1(0, 0);
   ImVec2 v2(v1.x + cellSize * 32, v1.y + cellSize * -32 - cellSize);

   ImColor color(255, 0, 0, 255);

    for (float x = fmodf(offset.x, cellSize); x < ImGui::GetWindowSize().x; x += cellSize)
    {
        ImGui::GetWindowDrawList()->AddLine(ImVec2(x + offset.x, 0.0f), ImVec2(x, ImGui::GetWindowSize().y), ImColor(255, 0, 0, 255));
    }

    for (float y = fmodf(offset.y, cellSize); y < ImGui::GetWindowSize().y; y += cellSize)
    {
        ImGui::GetWindowDrawList()->AddLine(ImVec2(y + offset.y, 0.0f), ImVec2(y + offset.y, ImGui::GetWindowSize().y), ImColor(255, 0, 0, 255));
    }

   
   //ImGui::GetWindowDrawList()->AddRect(ImVec2(0, 0), ImVec2(-ImGui::GetWindowSize().x, -ImGui::GetWindowSize().y), ImColor(255, 255, 0, 250));
   //ImGui::GetWindowDrawList()->AddRect(v1, v2, color);
   ImGui::GetWindowDrawList()->AddText(v1, color, "(0,0)");


	return false;
}