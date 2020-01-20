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

#include "NFComm/NFPluginModule/NFIBluePrintModule.h"
#include "NFComm/NFPluginModule/NFIElementModule.h"
#include "NFComm/NFPluginModule/NFIClassModule.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"


class NFInputVariable : public NFIVariable
{
private:
	NFInputVariable() {}
public:
	NFInputVariable(NFIPluginManager* p, const NFGUID& blockID, const NFGUID& id, const std::string& name)
	{
		this->id = id;
		this->name = name;
		this->pPluginManager = p;
		this->logicBlockId = blockID;

		variableType = NFVariableType::Input;

		Init();
	}


	virtual void InitInputArgs()
	{
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = "DataType";
			var->valueType = NFValueType::String;

			inputArgs.push_back(var);
		}
	}

	virtual void InitOutputArgs()
	{
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = "";
			var->valueType = NFValueType::UNKNOW;

			outputArgs.push_back(var);
		}
	}

	virtual void UpdateOutputData()
	{
		NF_SHARE_PTR<NFIOData> inputData = GetInputArg(0);
		NF_SHARE_PTR<NFIOData> outputData = GetOutputArg(0);
		outputData->valueType = inputData->valueType;
		outputData->varData = inputData->varData;
	}
};

//for Element Data <class anme && element id && property name>
class NFElementVariable : public NFIVariable
{
private:
	NFElementVariable() {}
public:
	NFElementVariable(NFIPluginManager* p, const NFGUID& blockID, const NFGUID& id, const std::string& name)
	{
		this->id = id;
		this->name = name;
		this->pPluginManager = p;
		this->logicBlockId = blockID;

		variableType = NFVariableType::ElementSystem;

		Init();

	}


	virtual void InitInputArgs()
	{
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFElementVariableInputArg::toString(NFElementVariableInputArg::ClassName);
			var->valueType = NFValueType::String;

			inputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFElementVariableInputArg::toString(NFElementVariableInputArg::ConfigID);
			var->valueType = NFValueType::String;

			inputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFElementVariableInputArg::toString(NFElementVariableInputArg::PropertyName);
			var->valueType = NFValueType::String;

			inputArgs.push_back(var);
		}
	}

	virtual void InitOutputArgs()
	{
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = "";
			var->valueType = NFValueType::UNKNOW;

			outputArgs.push_back(var);
		}
	}

	virtual void UpdateOutputData()
	{
		NF_SHARE_PTR<NFIOData> className = GetInputArg(NFElementVariableInputArg::toString(NFElementVariableInputArg::ClassName));
		NF_SHARE_PTR<NFIOData> configID = GetInputArg(NFElementVariableInputArg::toString(NFElementVariableInputArg::ConfigID));
		NF_SHARE_PTR<NFIOData> propertyName = GetInputArg(NFElementVariableInputArg::toString(NFElementVariableInputArg::PropertyName));

		NF_SHARE_PTR<NFIOData> outputData = GetOutputArg(0);
		outputData->varData = "";

		if (className->varData.empty() || configID->varData.empty() || propertyName->varData.empty())
		{
			return;
		}

		auto elementModule = this->pPluginManager->FindModule<NFIElementModule>();
		auto classModule = this->pPluginManager->FindModule<NFIClassModule>();
		if (elementModule->ExistElement(configID->varData))
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
						outputData->valueType = NFValueType::Int;
						int64_t value = elementModule->GetPropertyInt(configID->varData, propertyName->varData);
						outputData->varData = std::to_string(value);
					}
						break;
					case NFDATA_TYPE::TDATA_FLOAT:
					{
						outputData->valueType = NFValueType::Float;
						double value = elementModule->GetPropertyFloat(configID->varData, propertyName->varData);
						outputData->varData = std::to_string(value);
					}
						break;
					case NFDATA_TYPE::TDATA_STRING:
					{
						outputData->valueType = NFValueType::String;
						std::string value = elementModule->GetPropertyString(configID->varData, propertyName->varData);
						outputData->varData = value;
					}
						break;
					case NFDATA_TYPE::TDATA_VECTOR2:
					{
						outputData->valueType = NFValueType::Vector2;
						NFVector2 value = elementModule->GetPropertyVector2(configID->varData, propertyName->varData);
						outputData->varData = value.ToString();
					}
						break;
					case NFDATA_TYPE::TDATA_VECTOR3:
					{
						outputData->valueType = NFValueType::Vector3;
						NFVector3 value = elementModule->GetPropertyVector3(configID->varData, propertyName->varData);
						outputData->varData = value.ToString();
					}
						break;
					default:
						break;
					}
					
				}
			}
		}
	}

public:
	
};

//for Property Data <self id && property name>
class NFPropertyVariable : public NFIVariable
{
public:
	NFPropertyVariable(NFIPluginManager* p, const NFGUID& blockID, const NFGUID& id, const std::string& name)
	{
		this->id = id;
		this->name = name;
		this->pPluginManager = p;
		this->logicBlockId = blockID;

		variableType = NFVariableType::PropertySystem;

		Init();

	}


	virtual void InitInputArgs()
	{
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFPropertyVariableInputArg::toString(NFPropertyVariableInputArg::OnwerID);
			var->valueType = NFValueType::Object;

			inputArgs.push_back(var);
		} 
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFPropertyVariableInputArg::toString(NFPropertyVariableInputArg::ClassName);
			var->valueType = NFValueType::String;

			inputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFPropertyVariableInputArg::toString(NFPropertyVariableInputArg::PropertyName);
			var->valueType = NFValueType::String;

			inputArgs.push_back(var);
		}
	}
	virtual void InitOutputArgs()
	{
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = "";
			var->valueType = NFValueType::UNKNOW;

			outputArgs.push_back(var);
		}
	}
	virtual void UpdateOutputData()
	{

		NF_SHARE_PTR<NFIOData> className = GetInputArg(NFPropertyVariableInputArg::toString(NFPropertyVariableInputArg::ClassName));
		NF_SHARE_PTR<NFIOData> propertyName = GetInputArg(NFPropertyVariableInputArg::toString(NFPropertyVariableInputArg::PropertyName));

		NF_SHARE_PTR<NFIOData> outputData = GetOutputArg(0);
		outputData->varData = "";

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
						outputData->valueType = NFValueType::Int;
						outputData->varData = NFValueType::toString(NFValueType::Int);
					}
					break;
					case NFDATA_TYPE::TDATA_FLOAT:
					{
						outputData->valueType = NFValueType::Float;
						outputData->varData = NFValueType::toString(NFValueType::Float);
					}
					break;
					case NFDATA_TYPE::TDATA_OBJECT:
					{
						outputData->valueType = NFValueType::Object;
						outputData->varData = NFValueType::toString(NFValueType::Object);
					}
					break;
					case NFDATA_TYPE::TDATA_STRING:
					{
						outputData->valueType = NFValueType::String;
						outputData->varData = NFValueType::toString(NFValueType::String);
					}
					break;
					case NFDATA_TYPE::TDATA_VECTOR2:
					{
						outputData->valueType = NFValueType::Vector2;
						outputData->varData = NFValueType::toString(NFValueType::Vector2);
					}
					break;
					case NFDATA_TYPE::TDATA_VECTOR3:
					{
						outputData->valueType = NFValueType::Vector3;
						outputData->varData = NFValueType::toString(NFValueType::Vector3);
					}
					break;
					default:
						break;
					}

				}
			}
		}
	}

public:

};

	//for Record Data <self id && record name && row && col>
class NFRecordVariable : public NFIVariable
{
public:
	NFRecordVariable(NFIPluginManager* p, const NFGUID& blockID, const NFGUID& id, const std::string& name)
	{
		this->id = id;
		this->name = name;
		this->pPluginManager = p;
		this->logicBlockId = blockID;

		variableType = NFVariableType::RecordSystem;

		Init();
	}


	virtual void InitInputArgs()
	{
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFPropertyVariableInputArg::toString(NFPropertyVariableInputArg::OnwerID);
			var->valueType = NFValueType::Object;

			inputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFRecordVariableInputArg::ClassName;
			var->valueType = NFValueType::String;

			inputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFRecordVariableInputArg::RecordName;
			var->valueType = NFValueType::String;

			inputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFRecordVariableInputArg::RecordRow;
			var->valueType = NFValueType::Int;

			inputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFRecordVariableInputArg::RecordCol;
			var->valueType = NFValueType::Int;

			inputArgs.push_back(var);
		}
	}

	virtual void InitOutputArgs()
	{
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = "";
			var->valueType = NFValueType::UNKNOW;

			outputArgs.push_back(var);
		}
	}
	virtual void UpdateOutputData()
	{

	}

public:

};
