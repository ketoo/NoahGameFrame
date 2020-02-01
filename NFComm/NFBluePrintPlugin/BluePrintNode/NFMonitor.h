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
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFGameEventMonitorOutputArg::toString(NFGameEventMonitorOutputArg::Dictionary);
			var->valueType = NFValueType::Dictionary;

			outputArgs.push_back(var);
		}
	}

	// Inherited via NFIMonitor
	virtual void PrepareInputData() override;
	virtual void UpdateOutputData() override;

	// Inherited via NFIMonitor
	virtual NF_SHARE_PTR<NFBluePrintNodeBase> FindNextNode() override;
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
			var->name = NFNetworkEventMonitorOutputArg::toString(NFNetworkEventMonitorOutputArg::ObjectID);
			var->valueType = NFValueType::Object;

			outputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFNetworkEventMonitorOutputArg::toString(NFNetworkEventMonitorOutputArg::Dictionary);
			var->valueType = NFValueType::Dictionary;

			outputArgs.push_back(var);
		}
	}


	// Inherited via NFIMonitor
	virtual void PrepareInputData() override;

	virtual void UpdateOutputData() override;


	// Inherited via NFIMonitor
	virtual NF_SHARE_PTR<NFBluePrintNodeBase> FindNextNode() override;

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
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFNetworkkMsgMonitorOutputArg::toString(NFNetworkkMsgMonitorOutputArg::Dictionary);
			var->valueType = NFValueType::Dictionary;

			outputArgs.push_back(var);
		}
	}

	// Inherited via NFIMonitor
	virtual void PrepareInputData() override;
	virtual void UpdateOutputData() override;

	// Inherited via NFIMonitor
	virtual NF_SHARE_PTR<NFBluePrintNodeBase> FindNextNode() override;
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


	// Inherited via NFIMonitor
	virtual void PrepareInputData() override;

	virtual void UpdateOutputData() override;


	// Inherited via NFIMonitor
	virtual NF_SHARE_PTR<NFBluePrintNodeBase> FindNextNode() override;

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


	// Inherited via NFIMonitor
	virtual void PrepareInputData() override;

	virtual void UpdateOutputData() override;


	// Inherited via NFIMonitor
	virtual NF_SHARE_PTR<NFBluePrintNodeBase> FindNextNode() override;

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


	// Inherited via NFIMonitor
	virtual void PrepareInputData() override;

	virtual void UpdateOutputData() override;


	// Inherited via NFIMonitor
	virtual NF_SHARE_PTR<NFBluePrintNodeBase> FindNextNode() override;

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


	// Inherited via NFIMonitor
	virtual void PrepareInputData() override;

	virtual void UpdateOutputData() override;


	// Inherited via NFIMonitor
	virtual NF_SHARE_PTR<NFBluePrintNodeBase> FindNextNode() override;

};

