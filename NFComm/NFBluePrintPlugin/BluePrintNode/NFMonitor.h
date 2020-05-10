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
			var->SetValueType(NFValueType::Int);
			var->fromType = NFIODataComFromType::BOTH;

			inputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFGameEventMonitorInputArg::toString(NFGameEventMonitorInputArg::Dictionary);
			var->SetValueType(NFValueType::Dictionary);
			var->fromType = NFIODataComFromType::EXTERNAL;

			inputArgs.push_back(var);
		}
	}

	virtual void InitOutputArgs()
	{
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFGameEventMonitorOutputArg::toString(NFGameEventMonitorOutputArg::NextNode);
			var->SetValueType(NFValueType::Node);

			outputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFGameEventMonitorOutputArg::toString(NFGameEventMonitorOutputArg::EventID);
			var->SetValueType(NFValueType::Int);

			outputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFGameEventMonitorOutputArg::toString(NFGameEventMonitorOutputArg::ObjectID);
			var->SetValueType(NFValueType::Object);

			outputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFGameEventMonitorOutputArg::toString(NFGameEventMonitorOutputArg::Dictionary);
			var->SetValueType(NFValueType::Dictionary);

			outputArgs.push_back(var);
		}
	}

	virtual void UpdateOutputData(const NFGUID& runTimeOwner) override;

	virtual NF_SHARE_PTR<NFIOData> FindOutputNodeIOData() override;
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
			var->SetValueType(NFValueType::Int);
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
			var->SetValueType(NFValueType::Node);

			outputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFNetworkEventMonitorOutputArg::toString(NFNetworkEventMonitorOutputArg::EventID);
			var->SetValueType(NFValueType::Int);

			outputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFNetworkEventMonitorOutputArg::toString(NFNetworkEventMonitorOutputArg::ObjectID);
			var->SetValueType(NFValueType::Object);

			outputArgs.push_back(var);
		}
	}


	virtual void UpdateOutputData(const NFGUID& runTimeOwner) override;


	// Inherited via NFIMonitor
	virtual NF_SHARE_PTR<NFIOData> FindOutputNodeIOData() override;

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
			var->SetValueType(NFValueType::Int);
			var->fromType = NFIODataComFromType::BOTH;

			inputArgs.push_back(var);
		}
	}

	virtual void InitOutputArgs()
	{
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFNetworkkMsgMonitorOutputArg::toString(NFNetworkkMsgMonitorOutputArg::NextNode);
			var->SetValueType(NFValueType::Node);

			outputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFNetworkkMsgMonitorOutputArg::toString(NFNetworkkMsgMonitorOutputArg::ObjectID);
			var->SetValueType(NFValueType::Object);

			outputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFNetworkkMsgMonitorOutputArg::toString(NFNetworkkMsgMonitorOutputArg::NetMsgID);
			var->SetValueType(NFValueType::Int);

			outputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFNetworkkMsgMonitorOutputArg::toString(NFNetworkkMsgMonitorOutputArg::Dictionary);
			var->SetValueType(NFValueType::Dictionary);

			outputArgs.push_back(var);
		}
	}

	virtual void UpdateOutputData(const NFGUID& runTimeOwner) override;

	// Inherited via NFIMonitor
	virtual NF_SHARE_PTR<NFIOData> FindOutputNodeIOData() override;
};

class NFObjectEventMonitor : public NFIMonitor
{
private:
	NFIKernelModule* m_pKernelModule;
public:

	NFObjectEventMonitor(NFIPluginManager* p, const NFGUID& blockID, const NFGUID& id, const std::string& name)
	{
		this->id = id;
		this->name = name;
		this->pPluginManager = p;
		this->logicBlockId = blockID;

		monitorType = NFMonitorType::ObjectEvent;
		m_pKernelModule = this->pPluginManager->FindModule<NFIKernelModule>();

		Init();

	}


	virtual void InitInputArgs()
	{
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFMonitorObjectEventInputArg::toString(NFMonitorObjectEventInputArg::ClassName);
			var->SetValueType(NFValueType::String);
			var->fromType = NFIODataComFromType::INTERNAL;

			inputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFMonitorObjectEventInputArg::toString(NFMonitorObjectEventInputArg::ClassEvent);
			var->SetValueType(NFValueType::Int);
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
			var->SetValueType(NFValueType::Node);

			outputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFMonitorObjectEventOutputArg::toString(NFMonitorObjectEventOutputArg::ObjectID);
			var->SetValueType(NFValueType::Object);

			outputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFMonitorObjectEventOutputArg::toString(NFMonitorObjectEventOutputArg::ClassName);
			var->SetValueType(NFValueType::String);

			outputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFMonitorObjectEventOutputArg::toString(NFMonitorObjectEventOutputArg::ConfigID);
			var->SetValueType(NFValueType::String);

			outputArgs.push_back(var);
		}

		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFMonitorObjectEventOutputArg::toString(NFMonitorObjectEventOutputArg::SceneID);
			var->SetValueType(NFValueType::Int);

			outputArgs.push_back(var);
		}

		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFMonitorObjectEventOutputArg::toString(NFMonitorObjectEventOutputArg::GroupID);
			var->SetValueType(NFValueType::Int);

			outputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFMonitorObjectEventOutputArg::toString(NFMonitorObjectEventOutputArg::Position);
			var->SetValueType(NFValueType::Vector3);

			outputArgs.push_back(var);
		}
	}


	virtual void UpdateOutputData(const NFGUID& runTimeOwner) override;

	virtual NF_SHARE_PTR<NFIOData> FindOutputNodeIOData() override;

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
			var->SetValueType(NFValueType::String);
			var->fromType = NFIODataComFromType::INTERNAL;

			inputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFMonitorPropertyEventInputArg::toString(NFMonitorPropertyEventInputArg::PropName);
			var->SetValueType(NFValueType::String);
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
			var->SetValueType(NFValueType::Node);

			outputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFMonitorPropertyEventOutputArg::toString(NFMonitorPropertyEventOutputArg::ObjectID);
			var->SetValueType(NFValueType::Object);

			outputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFMonitorPropertyEventOutputArg::toString(NFMonitorPropertyEventOutputArg::PropName);
			var->SetValueType(NFValueType::String);

			outputArgs.push_back(var);
		}
	}


	virtual void UpdateOutputData(const NFGUID& runTimeOwner) override;


	// Inherited via NFIMonitor
	virtual NF_SHARE_PTR<NFIOData> FindOutputNodeIOData() override;

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
			var->SetValueType(NFValueType::String);
			var->fromType = NFIODataComFromType::INTERNAL;

			inputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFMonitorRecordEventInputArg::toString(NFMonitorRecordEventInputArg::RecordName);
			var->SetValueType(NFValueType::String);
			var->fromType = NFIODataComFromType::INTERNAL;

			inputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFMonitorRecordEventInputArg::toString(NFMonitorRecordEventInputArg::OperateType);
			var->SetValueType(NFValueType::Int);
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
			var->SetValueType(NFValueType::Node);

			outputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFMonitorRecordEventOutArg::toString(NFMonitorRecordEventOutArg::ClassName);
			var->SetValueType(NFValueType::Object);

			outputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFMonitorRecordEventOutArg::toString(NFMonitorRecordEventOutArg::RecordName);
			var->SetValueType(NFValueType::String);

			outputArgs.push_back(var);
		}

		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFMonitorRecordEventOutArg::toString(NFMonitorRecordEventOutArg::Row);
			var->SetValueType(NFValueType::Int);//depend the property's type

			outputArgs.push_back(var);
		}

		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFMonitorRecordEventOutArg::toString(NFMonitorRecordEventOutArg::Col);
			var->SetValueType(NFValueType::Int);//depend the property's type

			outputArgs.push_back(var);
		}
	}


	virtual void UpdateOutputData(const NFGUID& runTimeOwner) override;


	// Inherited via NFIMonitor
	virtual NF_SHARE_PTR<NFIOData> FindOutputNodeIOData() override;

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
			var->SetValueType(NFValueType::String);
			var->fromType = NFIODataComFromType::INTERNAL;

			inputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFMonitorSceneEventInputArg::toString(NFMonitorSceneEventInputArg::SceneID);
			var->SetValueType(NFValueType::Int);
			var->fromType = NFIODataComFromType::INTERNAL;

			inputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFMonitorSceneEventInputArg::toString(NFMonitorSceneEventInputArg::OperateType);
			var->SetValueType(NFValueType::Int);
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
			var->SetValueType(NFValueType::Node);

			outputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFMonitorSceneEventOutArg::toString(NFMonitorSceneEventOutArg::ClassName);
			var->SetValueType(NFValueType::Object);

			outputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFMonitorSceneEventOutArg::toString(NFMonitorSceneEventOutArg::SceneID);
			var->SetValueType(NFValueType::String);

			outputArgs.push_back(var);
		}

		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFMonitorSceneEventOutArg::toString(NFMonitorSceneEventOutArg::GroupID);
			var->SetValueType(NFValueType::Int);

			outputArgs.push_back(var);
		}

		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFMonitorSceneEventOutArg::toString(NFMonitorSceneEventOutArg::ObjectID);
			var->SetValueType(NFValueType::Object);

			outputArgs.push_back(var);
		}
	}


	virtual void UpdateOutputData(const NFGUID& runTimeOwner) override;

	virtual NF_SHARE_PTR<NFIOData> FindOutputNodeIOData() override;

};

