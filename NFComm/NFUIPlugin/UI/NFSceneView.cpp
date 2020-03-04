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

#include "NFSceneView.h"
#include "NFUIModule.h"

NFSceneView::NFSceneView(NFIPluginManager* p, NFViewType vt) : NFIView(p, vt, GET_CLASS_NAME(NFSceneView))
{

}

bool NFSceneView::Execute()
{
    /*
    // My Game has a different viewport than the editor's one:
    const int W = 1080 / 2;
    const int H = 1920 / 2;
    // We set the same viewport size (plus margin) to the next window (if first use)
    ImGui::SetNextWindowSize(ImVec2(W + 10, H + 10),
        ImGuiSetCond_FirstUseEver);
    ImGui::Begin("Game rendering");
    {
        // Get the current cursor position (where your window is)
        ImVec2 pos = ImGui::GetCursorScreenPos();

        // A boolean to allow me to stop the game rendering
        if (runApp) {
            glViewport(0, 0, W, H);
            // Render the scene into an FBO
            game->render(time);
            // Restore previous viewport
            glViewport(0, 0, w, h);
        }
        // Get the texture associated to the FBO
        auto tex = game->getRendered();

        // Ask ImGui to draw it as an image:
        // Under OpenGL the ImGUI image type is GLuint
        // So make sure to use "(void *)tex" but not "&tex"
        ImGui::GetWindowDrawList()->AddImage((void*)tex, ImVec2(ImGui::GetItemRectMin().x + pos.x, ImGui::GetItemRectMin().y + pos.y), ImVec2(pos.x + h / 2, pos.y + w / 2), ImVec2(0, 1), ImVec2(1, 0));
    }
    ImGui::End();
    */
	//1. get all scene from static config data
	//2. let the user choose one scene
	//3. get all objects of this level
	//4. draw object by imgui
	//5. show the props and records if the user picked an object
	//6. use can modify the value of props to trigger the saving job

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

void NFSceneView::SubRender()
{
   ImGui::Text(this->name.c_str());
}
