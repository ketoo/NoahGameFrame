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

NF_SHARE_PTR<NFIOData> NFArithmetic::FindOutputNodeIOData()
{
    return GetOutputArg(NFArithmeticOutputArg::NextNode);
}

void NFArithmetic::UpdateOutputData(const NFGUID& runTimeOwner)
{
    NF_SHARE_PTR<NFIOData> arithmeticType = GetInputArg(NFArithmeticInputArg::ArithmeticType);
    NF_SHARE_PTR<NFIOData> valueType = GetInputArg(NFArithmeticInputArg::ValueType);
    NF_SHARE_PTR<NFIOData> leftData = GetInputArg(NFArithmeticInputArg::LeftInput);
    NF_SHARE_PTR<NFIOData> rightData = GetInputArg(NFArithmeticInputArg::RightInput);

    NF_SHARE_PTR<NFIOData> outputData = GetOutputArg(NFArithmeticOutputArg::Output);
    if (valueType->GetValueType() == NFValueType::Int)
    {
        if (arithmeticType->GetInt() == NFArithmeticType::Add)
        {
            outputData->SetInt(leftData->GetInt() + rightData->GetInt());
        }
        else if (arithmeticType->GetInt() == NFArithmeticType::Sub)
        {
            outputData->SetInt(leftData->GetInt() - rightData->GetInt());
        }
        else if (arithmeticType->GetInt() == NFArithmeticType::Multiply)
        {
            outputData->SetInt(leftData->GetInt() * rightData->GetInt());
        }
        else if (arithmeticType->GetInt() == NFArithmeticType::Division)
        {
            if (rightData->GetInt() != 0)
            {
                outputData->SetInt(leftData->GetInt() / rightData->GetInt());
            }
            else
            {
                m_pLogModule->LogError("Denominator can't be 0!");
            }
        }
    }
    else if (leftData->GetValueType() == NFValueType::Float)
    {
        if (arithmeticType->GetInt() == NFArithmeticType::Add)
        {
            outputData->SetFloat(leftData->GetFloat() + rightData->GetFloat());
        }
        else if (arithmeticType->GetInt() == NFArithmeticType::Sub)
        {
            outputData->SetFloat(leftData->GetFloat() - rightData->GetFloat());
        }
        else if (arithmeticType->GetInt() == NFArithmeticType::Multiply)
        {
            outputData->SetFloat(leftData->GetFloat() * rightData->GetFloat());
        }
        else if (arithmeticType->GetInt() == NFArithmeticType::Division)
        {
            if (rightData->GetFloat() != 0)
            {
                outputData->SetFloat(leftData->GetFloat() / rightData->GetFloat());
            }
            else
            {
                //debug log
            }
        }
    }
    else if (leftData->GetValueType() == NFValueType::String)
    {
        if (arithmeticType->GetInt() == NFArithmeticType::Add)
        {
            outputData->SetString(leftData->GetString() + rightData->GetString());
        }
        else if (arithmeticType->GetInt() == NFArithmeticType::Sub)
        {
            //outputData->SetFloat(leftData->GetFloat() - rightData->GetFloat());
        }
    }
    else if (leftData->GetValueType() == NFValueType::Vector2)
    {
        if (arithmeticType->GetInt() == NFArithmeticType::Add)
        {

        }
        else if (arithmeticType->GetInt() == NFArithmeticType::Sub)
        {

        }
        else if (arithmeticType->GetInt() == NFArithmeticType::Multiply)
        {

        }
        else if (arithmeticType->GetInt() == NFArithmeticType::Division)
        {

        }
    }
    else if (leftData->GetValueType() == NFValueType::Vector3)
    {
        if (arithmeticType->GetInt() == NFArithmeticType::Add)
        {

        }
        else if (arithmeticType->GetInt() == NFArithmeticType::Sub)
        {

        }
        else if (arithmeticType->GetInt() == NFArithmeticType::Multiply)
        {

        }
        else if (arithmeticType->GetInt() == NFArithmeticType::Division)
        {

        }
    }
}