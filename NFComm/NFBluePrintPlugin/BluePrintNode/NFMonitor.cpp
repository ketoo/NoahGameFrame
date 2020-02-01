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

#include "NFMonitor.h"


void NFGameEventMonitor::PrepareInputData()
{
}

void NFGameEventMonitor::UpdateOutputData()
{
	NF_SHARE_PTR<NFIOData> inputData = GetInputArg(0);
	NF_SHARE_PTR<NFIOData> outputData = GetOutputArg(0);
	outputData->varData = inputData->varData;
}

NF_SHARE_PTR<NFBluePrintNodeBase> NFGameEventMonitor::FindNextNode()
{
	return NF_SHARE_PTR<NFBluePrintNodeBase>();
}

void NFNetworkEventMonitor::PrepareInputData()
{
}

void NFNetworkEventMonitor::UpdateOutputData()
{
	NF_SHARE_PTR<NFIOData> inputData = GetInputArg(0);
	NF_SHARE_PTR<NFIOData> outputData = GetOutputArg(0);
	outputData->varData = inputData->varData;
}

NF_SHARE_PTR<NFBluePrintNodeBase> NFNetworkEventMonitor::FindNextNode()
{
	return NF_SHARE_PTR<NFBluePrintNodeBase>();
}

void NFNetworkMsgEventMonitor::PrepareInputData()
{
}

void NFNetworkMsgEventMonitor::UpdateOutputData()
{
}

NF_SHARE_PTR<NFBluePrintNodeBase> NFNetworkMsgEventMonitor::FindNextNode()
{
	return NF_SHARE_PTR<NFBluePrintNodeBase>();
}


void NFObjectEventMonitor::PrepareInputData()
{
}

void NFObjectEventMonitor::UpdateOutputData()
{
}

NF_SHARE_PTR<NFBluePrintNodeBase> NFObjectEventMonitor::FindNextNode()
{
	return NF_SHARE_PTR<NFBluePrintNodeBase>();
}

void NFPropertyEventMonitor::PrepareInputData()
{
}

void NFPropertyEventMonitor::UpdateOutputData()
{
	NF_SHARE_PTR<NFIOData> className = GetInputArg(NFMonitorPropertyEventInputArg::toString(NFMonitorPropertyEventInputArg::ClassName));
	NF_SHARE_PTR<NFIOData> propertyName = GetInputArg(NFMonitorPropertyEventInputArg::toString(NFMonitorPropertyEventInputArg::PropertyName));

	NF_SHARE_PTR<NFIOData> outputSelf = GetOutputArg(NFMonitorPropertyEventOutputArg::toString(NFMonitorPropertyEventOutputArg::ObjectID));
	NF_SHARE_PTR<NFIOData> outputPropertyName = GetOutputArg(NFMonitorPropertyEventOutputArg::toString(NFMonitorPropertyEventOutputArg::PropertyName));
	NF_SHARE_PTR<NFIOData> outputPropertyValue = GetOutputArg(NFMonitorPropertyEventOutputArg::toString(NFMonitorPropertyEventOutputArg::PropertyValue));

	outputSelf->varData = "";
	outputPropertyName->varData = propertyName->varData;
	outputPropertyValue->varData = "";

	if (className->varData.empty() || propertyName->varData.empty())
	{
		return;
	}

	auto elementModule = this->pPluginManager->FindModule<NFIElementModule>();
	auto classModule = this->pPluginManager->FindModule<NFIClassModule>();
	{
		auto classObject = classModule->GetElement(className->varData);
		if (classObject)
		{
			auto classProperty = classObject->GetPropertyManager()->GetElement(propertyName->varData);
			if (classProperty)
			{
				switch (classProperty->GetType())
				{
				case NFDATA_TYPE::TDATA_INT:
				{
					outputPropertyValue->valueType = NFValueType::Int;
					outputPropertyValue->varData = NFValueType::toString(NFValueType::Int);
				}
				break;
				case NFDATA_TYPE::TDATA_FLOAT:
				{
					outputPropertyValue->valueType = NFValueType::Float;
					outputPropertyValue->varData = NFValueType::toString(NFValueType::Float);
				}
				break;
				case NFDATA_TYPE::TDATA_OBJECT:
				{
					outputPropertyValue->valueType = NFValueType::Object;
					outputPropertyValue->varData = NFValueType::toString(NFValueType::Object);
				}
				break;
				case NFDATA_TYPE::TDATA_STRING:
				{
					outputPropertyValue->valueType = NFValueType::String;
					outputPropertyValue->varData = NFValueType::toString(NFValueType::String);
				}
				break;
				case NFDATA_TYPE::TDATA_VECTOR2:
				{
					outputPropertyValue->valueType = NFValueType::Vector2;
					outputPropertyValue->varData = NFValueType::toString(NFValueType::Vector2);
				}
				break;
				case NFDATA_TYPE::TDATA_VECTOR3:
				{
					outputPropertyValue->valueType = NFValueType::Vector3;
					outputPropertyValue->varData = NFValueType::toString(NFValueType::Vector3);
				}
				break;
				default:
					break;
				}

			}
		}
	}
}

NF_SHARE_PTR<NFBluePrintNodeBase> NFPropertyEventMonitor::FindNextNode()
{
	return NF_SHARE_PTR<NFBluePrintNodeBase>();
}

void NFRecordEventMonitor::PrepareInputData()
{
}

void NFRecordEventMonitor::UpdateOutputData()
{
}

NF_SHARE_PTR<NFBluePrintNodeBase> NFRecordEventMonitor::FindNextNode()
{
	return NF_SHARE_PTR<NFBluePrintNodeBase>();
}

void NFSceneEventMonitor::PrepareInputData()
{
}

void NFSceneEventMonitor::UpdateOutputData()
{
}

NF_SHARE_PTR<NFBluePrintNodeBase> NFSceneEventMonitor::FindNextNode()
{
	return NF_SHARE_PTR<NFBluePrintNodeBase>();
}
