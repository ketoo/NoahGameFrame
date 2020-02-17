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

#include "NFBranch.h"

void NFIntBranch::PrepareInputData()
{
}

void NFIntBranch::UpdateOutputData()
{
}

NF_SHARE_PTR<NFBluePrintNodeBase> NFIntBranch::FindNextNode()
{
	return NF_SHARE_PTR<NFBluePrintNodeBase>();
}

void NFStringBranch::PrepareInputData()
{
}

void NFStringBranch::UpdateOutputData()
{
}

NF_SHARE_PTR<NFBluePrintNodeBase> NFStringBranch::FindNextNode()
{
	return NF_SHARE_PTR<NFBluePrintNodeBase>();
}

void NFFloatgBranch::PrepareInputData()
{
}

void NFFloatgBranch::UpdateOutputData()
{
}

NF_SHARE_PTR<NFBluePrintNodeBase> NFFloatgBranch::FindNextNode()
{
	return NF_SHARE_PTR<NFBluePrintNodeBase>();
}

void NFObjectBranch::PrepareInputData()
{
}

void NFObjectBranch::UpdateOutputData()
{
}

NF_SHARE_PTR<NFBluePrintNodeBase> NFObjectBranch::FindNextNode()
{
	return NF_SHARE_PTR<NFBluePrintNodeBase>();
}

void NFVector2Branch::PrepareInputData()
{
}

void NFVector2Branch::UpdateOutputData()
{
}

NF_SHARE_PTR<NFBluePrintNodeBase> NFVector2Branch::FindNextNode()
{
	return NF_SHARE_PTR<NFBluePrintNodeBase>();
}

void NFVector3Branch::PrepareInputData()
{
}

void NFVector3Branch::UpdateOutputData()
{
}

NF_SHARE_PTR<NFBluePrintNodeBase> NFVector3Branch::FindNextNode()
{
	return NF_SHARE_PTR<NFBluePrintNodeBase>();
}
