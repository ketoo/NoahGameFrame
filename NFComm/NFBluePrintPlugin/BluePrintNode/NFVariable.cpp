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

void NFInputVariable::UpdateOutputData(const NFGUID& runTimeOwner)
{
	NF_SHARE_PTR<NFIOData> inputData = GetInputArg(0);
	NF_SHARE_PTR<NFIOData> outputData = GetOutputArg(0);

	outputData->SetValueType(inputData->GetValueType());
	outputData->SetData(inputData->GetData());
}

NF_SHARE_PTR<NFIOData> NFInputVariable::FindOutputNodeIOData()
{
	return GetOutputArg(0);
}

void NFRecordVariable::UpdateOutputData(const NFGUID& runTimeOwner)
{
}

NF_SHARE_PTR<NFIOData> NFRecordVariable::FindOutputNodeIOData()
{
	return NF_SHARE_PTR<NFIOData>();
}

void NFElementVariable::UpdateOutputData(const NFGUID& runTimeOwner)
{
	NF_SHARE_PTR<NFIOData> className = GetInputArg(NFElementVariableInputArg::ClassName);
	NF_SHARE_PTR<NFIOData> configID = GetInputArg(NFElementVariableInputArg::ConfigID);
	NF_SHARE_PTR<NFIOData> propertyName = GetInputArg(NFElementVariableInputArg::PropName);

	NF_SHARE_PTR<NFIOData> outputClassName = GetOutputArg(NFElementVariableOutputArg::ClassName);
	NF_SHARE_PTR<NFIOData> outputConfigID = GetOutputArg(NFElementVariableOutputArg::ConfigID);
	NF_SHARE_PTR<NFIOData> outputPropName = GetOutputArg(NFElementVariableOutputArg::PropName);
	NF_SHARE_PTR<NFIOData> outputPropValue = GetOutputArg(NFElementVariableOutputArg::PropValue);

	if (className->GetString().empty() || configID->GetString().empty() || propertyName->GetString().empty())
	{
		return;
	}

	auto elementModule = this->pPluginManager->FindModule<NFIElementModule>();
	auto classModule = this->pPluginManager->FindModule<NFIClassModule>();
	if (elementModule->ExistElement(configID->GetString()))
	{
		auto classObject = classModule->GetElement(className->GetString());
		if (classObject)
		{
			auto classProperty = classObject->GetPropertyManager()->GetElement(propertyName->GetString());
			if (classProperty)
			{
				switch (classProperty->GetType())
				{
				case NFDATA_TYPE::TDATA_INT:
				{
					outputPropValue->SetValueType(NFValueType::Int);
					int64_t value = elementModule->GetPropertyInt(configID->GetString(), propertyName->GetString());
					outputPropValue->SetInt(value);
				}
				break;
				case NFDATA_TYPE::TDATA_FLOAT:
				{
					outputPropValue->SetValueType(NFValueType::Float);
					double value = elementModule->GetPropertyFloat(configID->GetString(), propertyName->GetString());
					outputPropValue->SetFloat(value);
				}
				break;
				case NFDATA_TYPE::TDATA_STRING:
				{
					outputPropValue->SetValueType(NFValueType::String);
					std::string value = elementModule->GetPropertyString(configID->GetString(), propertyName->GetString());
					outputPropValue->SetString(value);
				}
				break;
				case NFDATA_TYPE::TDATA_VECTOR2:
				{
					outputPropValue->SetValueType(NFValueType::Vector2);
					NFVector2 value = elementModule->GetPropertyVector2(configID->GetString(), propertyName->GetString());
					outputPropValue->SetVector2(value);
				}
				break;
				case NFDATA_TYPE::TDATA_VECTOR3:
				{
					outputPropValue->SetValueType(NFValueType::Vector3);
					NFVector3 value = elementModule->GetPropertyVector3(configID->GetString(), propertyName->GetString());
					outputPropValue->SetVector3(value);
				}
				break;
				default:
					break;
				}

			}
		}
	}
}

NF_SHARE_PTR<NFIOData> NFElementVariable::FindOutputNodeIOData()
{
	return NF_SHARE_PTR<NFIOData>();
}

void NFPropertyVariable::UpdateOutputData(const NFGUID& runTimeOwner)
{
	NF_SHARE_PTR<NFIOData> objectID = GetInputArg(NFPropertyVariableInputArg::ObjectID);
	NF_SHARE_PTR<NFIOData> className = GetInputArg(NFPropertyVariableInputArg::ClassName);
	NF_SHARE_PTR<NFIOData> propertyName = GetInputArg(NFPropertyVariableInputArg::PropName);

	if (className->GetString().empty() || propertyName->GetString().empty())
	{
		return;
	}

	NF_SHARE_PTR<NFIOData> outputClassName = GetOutputArg(NFPropertyVariableOutputArg::ClassName);
	NF_SHARE_PTR<NFIOData> outputPropertyName = GetOutputArg(NFPropertyVariableOutputArg::PropName);
	NF_SHARE_PTR<NFIOData> outputPropertyValue = GetOutputArg(NFPropertyVariableOutputArg::PropValue);

	outputClassName->SetString(className->GetString());
	outputPropertyName->SetString(propertyName->GetString());

	auto elementModule = this->pPluginManager->FindModule<NFIElementModule>();
	auto classModule = this->pPluginManager->FindModule<NFIClassModule>();
	auto kernelModule = this->pPluginManager->FindModule<NFIKernelModule>();
	{
		auto classObject = classModule->GetElement(className->GetString());
		if (classObject)
		{
			auto classProperty = classObject->GetPropertyManager()->GetElement(propertyName->GetString());
			if (classProperty)
			{
				switch (classProperty->GetType())
				{
				case NFDATA_TYPE::TDATA_INT:
				{
					outputPropertyValue->SetValueType(NFValueType::Int);
					if (!objectID->GetObject().IsNull())
					{
						outputPropertyValue->SetInt(kernelModule->GetPropertyInt(objectID->GetObject(), propertyName->GetString()));
					}
				}
				break;
				case NFDATA_TYPE::TDATA_FLOAT:
				{
					outputPropertyValue->SetValueType(NFValueType::Float);
					if (!objectID->GetObject().IsNull())
					{
						outputPropertyValue->SetFloat(kernelModule->GetPropertyFloat(objectID->GetObject(), propertyName->GetString()));
					}
				}
				break;
				case NFDATA_TYPE::TDATA_OBJECT:
				{
					outputPropertyValue->SetValueType(NFValueType::Object);
					if (!objectID->GetObject().IsNull())
					{
						outputPropertyValue->SetObject(kernelModule->GetPropertyObject(objectID->GetObject(), propertyName->GetString()));
					}
				}
				break;
				case NFDATA_TYPE::TDATA_STRING:
				{
					outputPropertyValue->SetValueType(NFValueType::String);
					if (!objectID->GetObject().IsNull())
					{
						outputPropertyValue->SetString(kernelModule->GetPropertyString(objectID->GetObject(), propertyName->GetString()));
					}
				}
				break;
				case NFDATA_TYPE::TDATA_VECTOR2:
				{
					outputPropertyValue->SetValueType(NFValueType::Vector2);
					if (!objectID->GetObject().IsNull())
					{
						outputPropertyValue->SetVector2(kernelModule->GetPropertyVector2(objectID->GetObject(), propertyName->GetString()));
					}
				}
				break;
				case NFDATA_TYPE::TDATA_VECTOR3:
				{
					outputPropertyValue->SetValueType(NFValueType::Vector3);
					if (!objectID->GetObject().IsNull())
					{
						outputPropertyValue->SetVector3(kernelModule->GetPropertyVector3(objectID->GetObject(), propertyName->GetString()));
					}
				}
				break;
				default:
					break;
				}

			}
		}
	}
}

NF_SHARE_PTR<NFIOData> NFPropertyVariable::FindOutputNodeIOData()
{
	return NF_SHARE_PTR<NFIOData>();
}

void NFPropertyListVariable::UpdateOutputData(const NFGUID& runTimeOwner)
{
	NF_SHARE_PTR<NFIOData> intputObjectID = GetOutputArg(NFPropertyListVariableInputArg::ObjectID);

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

NF_SHARE_PTR<NFIOData> NFPropertyListVariable::FindOutputNodeIOData()
{
	return NF_SHARE_PTR<NFIOData>();
}

void NFArrayVariable::UpdateOutputData(const NFGUID& runTimeOwner)
{
}

NF_SHARE_PTR<NFIOData> NFArrayVariable::FindOutputNodeIOData()
{
	return NF_SHARE_PTR<NFIOData>();
}

void NFDictionaryVariable::UpdateOutputData(const NFGUID& runTimeOwner)
{
}

NF_SHARE_PTR<NFIOData> NFDictionaryVariable::FindOutputNodeIOData()
{
	return NF_SHARE_PTR<NFIOData>();
}
