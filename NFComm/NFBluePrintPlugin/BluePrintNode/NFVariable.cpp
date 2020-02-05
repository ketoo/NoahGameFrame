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

#include "NFVariable.h"

void NFInputVariable::PrepareInputData()
{
}

void NFInputVariable::UpdateOutputData()
{
	NF_SHARE_PTR<NFIOData> inputData = GetInputArg(0);
	NF_SHARE_PTR<NFIOData> outputData = GetOutputArg(0);
	outputData->valueType = inputData->valueType;
	outputData->varData = inputData->varData;
}

NF_SHARE_PTR<NFBluePrintNodeBase> NFInputVariable::FindNextNode()
{
	return NF_SHARE_PTR<NFBluePrintNodeBase>();
}

void NFRecordVariable::PrepareInputData()
{
}

void NFRecordVariable::UpdateOutputData()
{
}

NF_SHARE_PTR<NFBluePrintNodeBase> NFRecordVariable::FindNextNode()
{
	return NF_SHARE_PTR<NFBluePrintNodeBase>();
}

void NFElementVariable::PrepareInputData()
{
}

void NFElementVariable::UpdateOutputData()
{
	NF_SHARE_PTR<NFIOData> className = GetInputArg(NFElementVariableInputArg::toString(NFElementVariableInputArg::ClassName));
	NF_SHARE_PTR<NFIOData> configID = GetInputArg(NFElementVariableInputArg::toString(NFElementVariableInputArg::ConfigID));
	NF_SHARE_PTR<NFIOData> propertyName = GetInputArg(NFElementVariableInputArg::toString(NFElementVariableInputArg::PropName));

	NF_SHARE_PTR<NFIOData> outputClassName = GetOutputArg(NFElementVariableInputArg::toString(NFElementVariableOutputArg::ClassName));
	NF_SHARE_PTR<NFIOData> outputConfigID = GetOutputArg(NFElementVariableInputArg::toString(NFElementVariableOutputArg::ConfigID));
	NF_SHARE_PTR<NFIOData> outputPropName = GetOutputArg(NFElementVariableInputArg::toString(NFElementVariableOutputArg::PropName));
	NF_SHARE_PTR<NFIOData> outputPropValue = GetOutputArg(NFElementVariableInputArg::toString(NFElementVariableOutputArg::PropValue));

	if (className->varData.GetString().empty() || configID->varData.GetString().empty() || propertyName->varData.GetString().empty())
	{
		return;
	}

	auto elementModule = this->pPluginManager->FindModule<NFIElementModule>();
	auto classModule = this->pPluginManager->FindModule<NFIClassModule>();
	if (elementModule->ExistElement(configID->varData.GetString()))
	{
		auto classObject = classModule->GetElement(className->varData.GetString());
		if (classObject)
		{
			auto classProperty = classObject->GetPropertyManager()->GetElement(propertyName->varData.GetString());
			if (classProperty)
			{
				switch (classProperty->GetType())
				{
				case NFDATA_TYPE::TDATA_INT:
				{
					outputPropValue->valueType = NFValueType::Int;
					int64_t value = elementModule->GetPropertyInt(configID->varData.GetString(), propertyName->varData.GetString());
					outputPropValue->varData.SetInt(value);
				}
				break;
				case NFDATA_TYPE::TDATA_FLOAT:
				{
					outputPropValue->valueType = NFValueType::Float;
					double value = elementModule->GetPropertyFloat(configID->varData.GetString(), propertyName->varData.GetString());
					outputPropValue->varData.SetFloat(value);
				}
				break;
				case NFDATA_TYPE::TDATA_STRING:
				{
					outputPropValue->valueType = NFValueType::String;
					std::string value = elementModule->GetPropertyString(configID->varData.GetString(), propertyName->varData.GetString());
					outputPropValue->varData.SetString(value);
				}
				break;
				case NFDATA_TYPE::TDATA_VECTOR2:
				{
					outputPropValue->valueType = NFValueType::Vector2;
					NFVector2 value = elementModule->GetPropertyVector2(configID->varData.GetString(), propertyName->varData.GetString());
					outputPropValue->varData.SetVector2(value);
				}
				break;
				case NFDATA_TYPE::TDATA_VECTOR3:
				{
					outputPropValue->valueType = NFValueType::Vector3;
					NFVector3 value = elementModule->GetPropertyVector3(configID->varData.GetString(), propertyName->varData.GetString());
					outputPropValue->varData.SetVector3(value);
				}
				break;
				default:
					break;
				}

			}
		}
	}
}

NF_SHARE_PTR<NFBluePrintNodeBase> NFElementVariable::FindNextNode()
{
	return NF_SHARE_PTR<NFBluePrintNodeBase>();
}

void NFPropertyVariable::PrepareInputData()
{
}

void NFPropertyVariable::UpdateOutputData()
{
	NF_SHARE_PTR<NFIOData> className = GetInputArg(NFPropertyVariableInputArg::toString(NFPropertyVariableInputArg::ClassName));
	NF_SHARE_PTR<NFIOData> propertyName = GetInputArg(NFPropertyVariableInputArg::toString(NFPropertyVariableInputArg::PropName));

	NF_SHARE_PTR<NFIOData> outputData = GetOutputArg(0);
	outputData->varData.SetString("");

	if (className->varData.GetString().empty() || propertyName->varData.GetString().empty())
	{
		return;
	}

	auto elementModule = this->pPluginManager->FindModule<NFIElementModule>();
	auto classModule = this->pPluginManager->FindModule<NFIClassModule>();
	{
		auto classObject = classModule->GetElement(className->varData.GetString());
		if (classObject)
		{
			auto classProperty = classObject->GetPropertyManager()->GetElement(propertyName->varData.GetString());
			if (classProperty)
			{
				switch (classProperty->GetType())
				{
				case NFDATA_TYPE::TDATA_INT:
				{
					outputData->valueType = NFValueType::Int;
					outputData->varData.Reset();
					outputData->varData.SetInt(0);
				}
				break;
				case NFDATA_TYPE::TDATA_FLOAT:
				{
					outputData->valueType = NFValueType::Float;
					outputData->varData.Reset();
					outputData->varData.SetFloat(0);
				}
				break;
				case NFDATA_TYPE::TDATA_OBJECT:
				{
					outputData->valueType = NFValueType::Object;
					outputData->varData.Reset();
					outputData->varData.SetObject(NFGUID());
				}
				break;
				case NFDATA_TYPE::TDATA_STRING:
				{
					outputData->valueType = NFValueType::String;
					outputData->varData.Reset();
					outputData->varData.SetString("");
				}
				break;
				case NFDATA_TYPE::TDATA_VECTOR2:
				{
					outputData->valueType = NFValueType::Vector2;
					outputData->varData.Reset();
					outputData->varData.SetVector2(NFVector2());
				}
				break;
				case NFDATA_TYPE::TDATA_VECTOR3:
				{
					outputData->valueType = NFValueType::Vector3;
					outputData->varData.Reset();
					outputData->varData.SetVector3(NFVector3());
				}
				break;
				default:
					break;
				}

			}
		}
	}
}

NF_SHARE_PTR<NFBluePrintNodeBase> NFPropertyVariable::FindNextNode()
{
	return NF_SHARE_PTR<NFBluePrintNodeBase>();
}

void NFPropertyListVariable::PrepareInputData()
{
}

void NFPropertyListVariable::UpdateOutputData()
{

	NF_SHARE_PTR<NFIOData> className = GetInputArg(NFPropertyListVariableInputArg::toString(NFPropertyListVariableInputArg::ClassName));

	NF_SHARE_PTR<NFIOData> intputObjectID = GetOutputArg(NFPropertyListVariableInputArg::ObjectID);
	NF_SHARE_PTR<NFIOData> intputClassName = GetOutputArg(NFPropertyListVariableInputArg::ClassName);

	NF_SHARE_PTR<NFIOData> outputObjectID = GetOutputArg(NFPropertyListVariableOutputArg::ObjectID);
	NF_SHARE_PTR<NFIOData> outputClassName = GetOutputArg(NFPropertyListVariableOutputArg::ClassName);

	NF_SHARE_PTR<NFIOData> outputConfigID = GetOutputArg(NFPropertyListVariableOutputArg::ConfigID);
	NF_SHARE_PTR<NFIOData> outputSceneID = GetOutputArg(NFPropertyListVariableOutputArg::SceneID);
	NF_SHARE_PTR<NFIOData> outputGroupID = GetOutputArg(NFPropertyListVariableOutputArg::GroupID);
	NF_SHARE_PTR<NFIOData> outputPosition = GetOutputArg(NFPropertyListVariableOutputArg::Position);

	//NFGUID objectID(intputObjectID->varData);
	//std::string className(intputClassName->varData);

	NFIKernelModule* kernelModule = this->pPluginManager->FindModule<NFIKernelModule>();
	{
		//std::string classObject = kernelModule->

	}
}

NF_SHARE_PTR<NFBluePrintNodeBase> NFPropertyListVariable::FindNextNode()
{
	return NF_SHARE_PTR<NFBluePrintNodeBase>();
}

void NFArrayVariable::PrepareInputData()
{
}

void NFArrayVariable::UpdateOutputData()
{
}

NF_SHARE_PTR<NFBluePrintNodeBase> NFArrayVariable::FindNextNode()
{
	return NF_SHARE_PTR<NFBluePrintNodeBase>();
}

void NFDictionaryVariable::PrepareInputData()
{
}

void NFDictionaryVariable::UpdateOutputData()
{
}

NF_SHARE_PTR<NFBluePrintNodeBase> NFDictionaryVariable::FindNextNode()
{
	return NF_SHARE_PTR<NFBluePrintNodeBase>();
}
