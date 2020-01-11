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

/*
	GameEvent,
	NetworkEvent,
	NetworkMsgEvent,
	ObjectEvent,
	PropertyEvent,
	RecordEvent,
	HeartBeatEvent,
	SceneEvent,
	ItemEvent,
	SkillEvent,
	BuffEvent,
*/

//for Element Data <class anme && element id && property name>
class NFElementVariable : public NFIVariable
{
private:
	NFElementVariable() {}
public:
	NFElementVariable(NFIPluginManager* p, const NFGUID& blockID, const NFGUID& id, const std::string& name, NF_SHARE_PTR<NFBluePrintNodeBase> parent)
	{
		this->id = id;
		this->name = name;
		this->pPluginManager = p;
		this->parent = parent;
		this->logicBlockId = blockID;

		variableType = NFVariableType::ElementSystem;
		blueprintType = NFBlueprintType::VARIABLE;

		InitInputArgs();
	}


	virtual void InitInputArgs()
	{
		{
			NF_SHARE_PTR<NFInputData> var = NF_SHARE_PTR<NFInputData>(NF_NEW NFInputData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->index = NFElementVariableInputArg::ClassName;
			var->name = NFElementVariableInputArg::toString(NFElementVariableInputArg::ClassName);
			var->valueType = NFValueType::String;

			inputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFInputData> var = NF_SHARE_PTR<NFInputData>(NF_NEW NFInputData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->index = NFElementVariableInputArg::ElementConfigID;
			var->name = NFElementVariableInputArg::toString(NFElementVariableInputArg::ElementConfigID);
			var->valueType = NFValueType::String;

			inputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFInputData> var = NF_SHARE_PTR<NFInputData>(NF_NEW NFInputData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->index = NFElementVariableInputArg::ElementPropertyName;
			var->name = NFElementVariableInputArg::toString(NFElementVariableInputArg::ElementPropertyName);
			var->valueType = NFValueType::String;

			inputArgs.push_back(var);
		}
	}


public:
	
};

//for Property Data <self id && property name>
class NFPropertyVariable : public NFIVariable
{
public:
	NFPropertyVariable(NFIPluginManager* p, const NFGUID& blockID, const NFGUID& id, const std::string& name, NF_SHARE_PTR<NFBluePrintNodeBase> parent)
	{
		this->id = id;
		this->name = name;
		this->pPluginManager = p;
		this->parent = parent;
		this->logicBlockId = blockID;

		variableType = NFVariableType::PropertySystem;
		blueprintType = NFBlueprintType::VARIABLE;

		InitInputArgs();
	}


	virtual void InitInputArgs()
	{
		{
			NF_SHARE_PTR<NFInputData> var = NF_SHARE_PTR<NFInputData>(NF_NEW NFInputData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->index = NFPropertyVariableInputArg::OnwerID;
			var->name = NFPropertyVariableInputArg::toString(NFPropertyVariableInputArg::OnwerID);
			var->valueType = NFValueType::Object;

			inputArgs.push_back(var);
		} 
		{
			NF_SHARE_PTR<NFInputData> var = NF_SHARE_PTR<NFInputData>(NF_NEW NFInputData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->index = NFPropertyVariableInputArg::ClassName;
			var->name = NFPropertyVariableInputArg::toString(NFPropertyVariableInputArg::ClassName);
			var->valueType = NFValueType::String;

			inputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFInputData> var = NF_SHARE_PTR<NFInputData>(NF_NEW NFInputData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->index = NFPropertyVariableInputArg::PropertyName;
			var->name = NFPropertyVariableInputArg::toString(NFPropertyVariableInputArg::PropertyName);
			var->valueType = NFValueType::String;

			inputArgs.push_back(var);
		}
	}


public:

};

	//for Record Data <self id && record name && row && col>
class NFRecordVariable : public NFIVariable
{
public:
	NFRecordVariable(NFIPluginManager* p, const NFGUID& blockID, const NFGUID& id, const std::string& name, NF_SHARE_PTR<NFBluePrintNodeBase> parent)
	{
		this->id = id;
		this->name = name;
		this->pPluginManager = p;
		this->parent = parent;
		this->logicBlockId = blockID;

		//operatorType = NFMonitorType::GameEvent;
		blueprintType = NFBlueprintType::VARIABLE;

		InitInputArgs();
	}


	virtual void InitInputArgs()
	{
		{
			NF_SHARE_PTR<NFInputData> var = NF_SHARE_PTR<NFInputData>(NF_NEW NFInputData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->index = NFRecordVariableInputArg::OnwerID;
			var->name = NFPropertyVariableInputArg::toString(NFPropertyVariableInputArg::OnwerID);
			var->valueType = NFValueType::Object;

			inputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFInputData> var = NF_SHARE_PTR<NFInputData>(NF_NEW NFInputData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFRecordVariableInputArg::RecordName;
			var->valueType = NFValueType::String;

			inputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFInputData> var = NF_SHARE_PTR<NFInputData>(NF_NEW NFInputData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFRecordVariableInputArg::RecordRow;
			var->valueType = NFValueType::Int;

			inputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFInputData> var = NF_SHARE_PTR<NFInputData>(NF_NEW NFInputData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFRecordVariableInputArg::RecordCol;
			var->valueType = NFValueType::Int;

			inputArgs.push_back(var);
		}
	}


public:

};
