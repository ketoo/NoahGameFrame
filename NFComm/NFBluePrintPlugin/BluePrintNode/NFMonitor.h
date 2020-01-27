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

#include "NFComm/NFPluginModule/NFIElementModule.h"
#include "NFComm/NFPluginModule/NFIClassModule.h"
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
class NFGameEventMonitor : public NFIMonitor
{
public:

	NFGameEventMonitor(NFIPluginManager* p, const NFGUID& blockID, const NFGUID& id, const std::string& name)
	{
		this->id = id;
		this->name = name;
		this->pPluginManager = p;
		this->logicBlockId = blockID;

		monitorType = NFMonitorType::GameEvent;

		Init();
	}


	virtual void InitInputArgs()
	{
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFGameEventMonitorInputArg::toString(NFGameEventMonitorInputArg::EventID);
			var->valueType = NFValueType::Int;
			var->fromType = NFIODataComFromType::BOTH;

			inputArgs.push_back(var);
		}
	}

	virtual void InitOutputArgs()
	{
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFGameEventMonitorOutputArg::toString(NFGameEventMonitorOutputArg::NextNode);
			var->valueType = NFValueType::Node;

			outputArgs.push_back(var);
		} 
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFGameEventMonitorOutputArg::toString(NFGameEventMonitorOutputArg::ObjectID);
			var->valueType = NFValueType::Object;

			outputArgs.push_back(var);
		}
	}

	virtual void UpdateOutputData()
	{
		NF_SHARE_PTR<NFIOData> inputData = GetInputArg(0);
		NF_SHARE_PTR<NFIOData> outputData = GetOutputArg(0);
		outputData->varData = inputData->varData;
	}

public:
	
};

class NFNetworkEventMonitor : public NFIMonitor
{
public:

	NFNetworkEventMonitor(NFIPluginManager* p, const NFGUID& blockID, const NFGUID& id, const std::string& name)
	{
		this->id = id;
		this->name = name;
		this->pPluginManager = p;
		this->logicBlockId = blockID;

		monitorType = NFMonitorType::NetworkEvent;

		Init();

	}


	virtual void InitInputArgs()
	{
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFNetworkEventMonitorInputArg::toString(NFNetworkEventMonitorInputArg::EventID);
			var->valueType = NFValueType::Int;
			var->fromType = NFIODataComFromType::BOTH;

			inputArgs.push_back(var);
		}
	}

	virtual void InitOutputArgs()
	{
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFNetworkEventMonitorOutputArg::toString(NFNetworkEventMonitorOutputArg::NextNode);
			var->valueType = NFValueType::Node;

			outputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFGameEventMonitorOutputArg::toString(NFNetworkEventMonitorOutputArg::ObjectID);
			var->valueType = NFValueType::Object;

			outputArgs.push_back(var);
		}
	}

	virtual void UpdateOutputData()
	{
		NF_SHARE_PTR<NFIOData> inputData = GetInputArg(0);
		NF_SHARE_PTR<NFIOData> outputData = GetOutputArg(0);
		outputData->varData = inputData->varData;
	}

public:

};


class NFNetworkMsgEventMonitor : public NFIMonitor
{
public:

	NFNetworkMsgEventMonitor(NFIPluginManager* p, const NFGUID& blockID, const NFGUID& id, const std::string& name)
	{
		this->id = id;
		this->name = name;
		this->pPluginManager = p;
		this->logicBlockId = blockID;

		monitorType = NFMonitorType::NetworkMsgEvent;

		Init();

	}

	virtual void InitInputArgs()
	{
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFNetworkMsgMonitorInputArg::toString(NFNetworkMsgMonitorInputArg::NetMsgID);
			var->valueType = NFValueType::Int;
			var->fromType = NFIODataComFromType::INTERNAL;

			inputArgs.push_back(var);
		}
	}

	virtual void InitOutputArgs()
	{
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFNetworkkMsgMonitorOutputArg::toString(NFNetworkkMsgMonitorOutputArg::NextNode);
			var->valueType = NFValueType::Node;

			outputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFNetworkkMsgMonitorOutputArg::toString(NFNetworkkMsgMonitorOutputArg::ObjectID);
			var->valueType = NFValueType::Object;

			outputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFNetworkkMsgMonitorOutputArg::toString(NFNetworkkMsgMonitorOutputArg::Dictionary);
			var->valueType = NFValueType::Dictionary;

			outputArgs.push_back(var);
		}
	}


	virtual void UpdateOutputData()
	{
	}

public:

};

class NFObjectEventMonitor : public NFIMonitor
{
public:

	NFObjectEventMonitor(NFIPluginManager* p, const NFGUID& blockID, const NFGUID& id, const std::string& name)
	{
		this->id = id;
		this->name = name;
		this->pPluginManager = p;
		this->logicBlockId = blockID;

		monitorType = NFMonitorType::ObjectEvent;

		Init();

	}


	virtual void InitInputArgs()
	{
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFMonitorObjectEventInputArg::toString(NFMonitorObjectEventInputArg::ClassName);
			var->valueType = NFValueType::String;
			var->fromType = NFIODataComFromType::INTERNAL;

			inputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFMonitorObjectEventInputArg::toString(NFMonitorObjectEventInputArg::ClassEvent);
			var->valueType = NFValueType::Int;
			var->fromType = NFIODataComFromType::INTERNAL;

			inputArgs.push_back(var);
		}
	}

	virtual void InitOutputArgs()
	{
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFMonitorObjectEventOutputArg::toString(NFMonitorObjectEventOutputArg::NextNode);
			var->valueType = NFValueType::Node;

			outputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFMonitorObjectEventOutputArg::toString(NFMonitorObjectEventOutputArg::ObjectID);
			var->valueType = NFValueType::Object;

			outputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFMonitorObjectEventOutputArg::toString(NFMonitorObjectEventOutputArg::ClassName);
			var->valueType = NFValueType::String;

			outputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFMonitorObjectEventOutputArg::toString(NFMonitorObjectEventOutputArg::ConfigID);
			var->valueType = NFValueType::String;

			outputArgs.push_back(var);
		}

		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFMonitorObjectEventOutputArg::toString(NFMonitorObjectEventOutputArg::SceneID);
			var->valueType = NFValueType::Int;

			outputArgs.push_back(var);
		}

		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFMonitorObjectEventOutputArg::toString(NFMonitorObjectEventOutputArg::GroupID);
			var->valueType = NFValueType::Int;

			outputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFMonitorObjectEventOutputArg::toString(NFMonitorObjectEventOutputArg::Position);
			var->valueType = NFValueType::Vector3;

			outputArgs.push_back(var);
		}
	}

	virtual void UpdateOutputData()
	{
	}

public:

};

class NFPropertyEventMonitor : public NFIMonitor
{
public:

	NFPropertyEventMonitor(NFIPluginManager* p, const NFGUID& blockID, const NFGUID& id, const std::string& name)
	{
		this->id = id;
		this->name = name;
		this->pPluginManager = p;
		this->logicBlockId = blockID;

		monitorType = NFMonitorType::PropertyEvent;

		Init();

	}


	virtual void InitInputArgs()
	{
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFMonitorPropertyEventInputArg::toString(NFMonitorPropertyEventInputArg::ClassName);
			var->valueType = NFValueType::String;
			var->fromType = NFIODataComFromType::INTERNAL;

			inputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFMonitorPropertyEventInputArg::toString(NFMonitorPropertyEventInputArg::PropertyName);
			var->valueType = NFValueType::String;
			var->fromType = NFIODataComFromType::INTERNAL;

			inputArgs.push_back(var);
		}
	}

	virtual void InitOutputArgs()
	{
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFMonitorPropertyEventOutputArg::toString(NFMonitorPropertyEventOutputArg::NextNode);
			var->valueType = NFValueType::Node;

			outputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFMonitorPropertyEventOutputArg::toString(NFMonitorPropertyEventOutputArg::ObjectID);
			var->valueType = NFValueType::Object;

			outputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFMonitorPropertyEventOutputArg::toString(NFMonitorPropertyEventOutputArg::PropertyName);
			var->valueType = NFValueType::String;

			outputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFMonitorPropertyEventOutputArg::toString(NFMonitorPropertyEventOutputArg::PropertyValue);
			var->valueType = NFValueType::UNKNOW;//depend the property's type

			outputArgs.push_back(var);
		}
	}

	virtual void UpdateOutputData()
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

public:

};


class NFRecordEventMonitor : public NFIMonitor
{
public:

	NFRecordEventMonitor(NFIPluginManager* p, const NFGUID& blockID, const NFGUID& id, const std::string& name)
	{
		this->id = id;
		this->name = name;
		this->pPluginManager = p;
		this->logicBlockId = blockID;

		monitorType = NFMonitorType::RecordEvent;

		Init();

	}


	virtual void InitInputArgs()
	{
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFMonitorRecordEventInputArg::toString(NFMonitorRecordEventInputArg::ClassName);
			var->valueType = NFValueType::String;
			var->fromType = NFIODataComFromType::INTERNAL;

			inputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFMonitorRecordEventInputArg::toString(NFMonitorRecordEventInputArg::RecordName);
			var->valueType = NFValueType::String;
			var->fromType = NFIODataComFromType::INTERNAL;

			inputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFMonitorRecordEventInputArg::toString(NFMonitorRecordEventInputArg::OperateType);
			var->valueType = NFValueType::Int;
			var->fromType = NFIODataComFromType::INTERNAL;

			inputArgs.push_back(var);
		}
	}

	virtual void InitOutputArgs()
	{
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFMonitorRecordEventOutArg::toString(NFMonitorRecordEventOutArg::NextNode);
			var->valueType = NFValueType::Node;

			outputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFMonitorRecordEventOutArg::toString(NFMonitorRecordEventOutArg::ClassName);
			var->valueType = NFValueType::Object;

			outputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFMonitorRecordEventOutArg::toString(NFMonitorRecordEventOutArg::RecordName);
			var->valueType = NFValueType::String;

			outputArgs.push_back(var);
		}

		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFMonitorRecordEventOutArg::toString(NFMonitorRecordEventOutArg::Row);
			var->valueType = NFValueType::Int;//depend the property's type

			outputArgs.push_back(var);
		}

		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFMonitorRecordEventOutArg::toString(NFMonitorRecordEventOutArg::Col);
			var->valueType = NFValueType::Int;//depend the property's type

			outputArgs.push_back(var);
		}
	}

	virtual void UpdateOutputData()
	{
		
	}

public:

};


class NFSceneEventMonitor : public NFIMonitor
{
public:

	NFSceneEventMonitor(NFIPluginManager* p, const NFGUID& blockID, const NFGUID& id, const std::string& name)
	{
		this->id = id;
		this->name = name;
		this->pPluginManager = p;
		this->logicBlockId = blockID;

		monitorType = NFMonitorType::RecordEvent;

		Init();

	}


	virtual void InitInputArgs()
	{
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFMonitorSceneEventInputArg::toString(NFMonitorSceneEventInputArg::ClassName);
			var->valueType = NFValueType::String;
			var->fromType = NFIODataComFromType::INTERNAL;

			inputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFMonitorSceneEventInputArg::toString(NFMonitorSceneEventInputArg::SceneID);
			var->valueType = NFValueType::Int;
			var->fromType = NFIODataComFromType::INTERNAL;

			inputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFMonitorSceneEventInputArg::toString(NFMonitorSceneEventInputArg::OperateType);
			var->valueType = NFValueType::Int;
			var->fromType = NFIODataComFromType::INTERNAL;

			inputArgs.push_back(var);
		}
	}

	virtual void InitOutputArgs()
	{
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFMonitorSceneEventOutArg::toString(NFMonitorSceneEventOutArg::NextNode);
			var->valueType = NFValueType::Node;

			outputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFMonitorSceneEventOutArg::toString(NFMonitorSceneEventOutArg::ClassName);
			var->valueType = NFValueType::Object;

			outputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFMonitorSceneEventOutArg::toString(NFMonitorSceneEventOutArg::SceneID);
			var->valueType = NFValueType::String;

			outputArgs.push_back(var);
		}

		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFMonitorSceneEventOutArg::toString(NFMonitorSceneEventOutArg::GroupID);
			var->valueType = NFValueType::Int;

			outputArgs.push_back(var);
		}

		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFMonitorSceneEventOutArg::toString(NFMonitorSceneEventOutArg::ObjectID);
			var->valueType = NFValueType::Object;

			outputArgs.push_back(var);
		}
	}

	virtual void UpdateOutputData()
	{

	}

public:

};

