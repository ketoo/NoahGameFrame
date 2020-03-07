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

#include "NFArithmetic.h"

void NFArithmetic::PrepareInputData(const NFGUID& runTimeOnwer, const bool iteration)
{
}

NF_SHARE_PTR<NFIOData> NFArithmetic::FindOutputNodeIOData()
{
	return NF_SHARE_PTR<NFIOData>();
}

void NFArithmetic::UpdateOutputData(const NFGUID& runTimeOnwer, const bool iteration)
{
    if (this->arithmeticType == NFArithmeticType::Add)
    {

    }
    else if (this->arithmeticType == NFArithmeticType::Sub)
    {

    }
    else if (this->arithmeticType == NFArithmeticType::Multiply)
    {

    }
    else if (this->arithmeticType == NFArithmeticType::Division)
    {

    }
}