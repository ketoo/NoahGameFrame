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

#include "NFComm/NFPluginModule/NFIBluePrintModule.h"

class NFCreateObjectExecuter : public NFIExecuter
{
private:
	NFCreateObjectExecuter() {}
public:
	NFCreateObjectExecuter(NFIPluginManager* p, const NFGUID& blockID, const NFGUID& id, const std::string& name)
	{
		this->id = id;
		this->name = name;
		this->logicBlockId = blockID;
		this->pPluginManager = p;

		executerType = NFExecuterType::CreateObject;

		Init();
	}

	virtual void InitInputArgs()
	{
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFExecuterCreateObjectInputArg::toString(NFExecuterCreateObjectInputArg::LastNode);
			var->valueType = NFValueType::Node;

			inputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFExecuterCreateObjectInputArg::toString(NFExecuterCreateObjectInputArg::ConfigID);
			var->valueType = NFValueType::String;
			var->fromType = NFIODataComFromType::EXTERNAL;

			inputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFExecuterCreateObjectInputArg::toString(NFExecuterCreateObjectInputArg::SceneID);
			var->valueType = NFValueType::Int;
			var->fromType = NFIODataComFromType::EXTERNAL;

			inputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFExecuterCreateObjectInputArg::toString(NFExecuterCreateObjectInputArg::GroupID);
			var->valueType = NFValueType::Int;
			var->fromType = NFIODataComFromType::EXTERNAL;

			inputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFExecuterCreateObjectInputArg::toString(NFExecuterCreateObjectInputArg::Position);
			var->valueType = NFValueType::Vector3;
			var->fromType = NFIODataComFromType::EXTERNAL;

			inputArgs.push_back(var);
		}
	}

	virtual void InitOutputArgs()
	{
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFExecuterCreateObjectOutputputArg::toString(NFExecuterCreateObjectOutputputArg::NextNode);
			var->valueType = NFValueType::Node;

			outputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFExecuterCreateObjectOutputputArg::toString(NFExecuterCreateObjectOutputputArg::ObjectID);
			var->valueType = NFValueType::Object;

			outputArgs.push_back(var);
		}
	}


	// Inherited via NFIExecuter
	virtual void PrepareInputData(const NFGUID& runTimeOnwer, const bool iteration) override;

	virtual void UpdateOutputData(const NFGUID& runTimeOnwer, const bool iteration) override;


	// Inherited via NFIExecuter
	virtual NF_SHARE_PTR<NFIOData> FindOutputNodeIOData() override;

};

class NFDestroyObjectExecuter : public NFIExecuter
{
private:
	NFDestroyObjectExecuter() {}
public:
	NFDestroyObjectExecuter(NFIPluginManager* p, const NFGUID& blockID, const NFGUID& id, const std::string& name)
	{
		this->id = id;
		this->name = name;
		this->logicBlockId = blockID;
		this->pPluginManager = p;

		executerType = NFExecuterType::DestroyObject;

		Init();

	};


	virtual void InitInputArgs()
	{
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFExecuterDestroyObjectInputArg::toString(NFExecuterDestroyObjectInputArg::LastNode);
			var->valueType = NFValueType::Node;

			inputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFExecuterDestroyObjectInputArg::toString(NFExecuterDestroyObjectInputArg::ObjectID);
			var->valueType = NFValueType::Object;
			var->fromType = NFIODataComFromType::EXTERNAL;

			inputArgs.push_back(var);
		}
	}

	virtual void InitOutputArgs()
	{

		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFExecuterDestroyObjectOutputputArg::toString(NFExecuterDestroyObjectOutputputArg::NextNode);
			var->valueType = NFValueType::Node;

			outputArgs.push_back(var);
		}
	}


	// Inherited via NFIExecuter

	virtual void PrepareInputData(const NFGUID& runTimeOnwer, const bool iteration) override;

	virtual void UpdateOutputData(const NFGUID& runTimeOnwer, const bool iteration) override;


	// Inherited via NFIExecuter
	virtual NF_SHARE_PTR<NFIOData> FindOutputNodeIOData() override;

};

class NFGameEventExecuter : public NFIExecuter
{
private:
	NFGameEventExecuter() {}
public:
	NFGameEventExecuter(NFIPluginManager* p, const NFGUID& blockID, const NFGUID& id, const std::string& name)
	{
		this->id = id;
		this->name = name;
		this->logicBlockId = blockID;
		this->pPluginManager = p;

		executerType = NFExecuterType::GameEvent;

		Init();
	}

	virtual void InitInputArgs()
	{
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFExecuterGameEventInputArg::toString(NFExecuterGameEventInputArg::LastNode);
			var->valueType = NFValueType::Node;

			inputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFExecuterGameEventInputArg::toString(NFExecuterGameEventInputArg::ObjectID);
			var->valueType = NFValueType::Object;
			var->fromType = NFIODataComFromType::EXTERNAL;

			inputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFExecuterGameEventInputArg::toString(NFExecuterGameEventInputArg::EventID);
			var->valueType = NFValueType::Int;
			var->fromType = NFIODataComFromType::EXTERNAL;

			inputArgs.push_back(var);
		}
	}

	virtual void InitOutputArgs()
	{
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFExecuterGameEventOutputArg::toString(NFExecuterGameEventOutputArg::NextNode);
			var->valueType = NFValueType::Node;

			outputArgs.push_back(var);
		}
	}


	// Inherited via NFIExecuter

	virtual void PrepareInputData(const NFGUID& runTimeOnwer, const bool iteration) override;

	virtual void UpdateOutputData(const NFGUID& runTimeOnwer, const bool iteration) override;


	// Inherited via NFIExecuter
	virtual NF_SHARE_PTR<NFIOData> FindOutputNodeIOData() override;

};

class NFMoveObjectExecuter : public NFIExecuter
{
private:
	NFMoveObjectExecuter() {}
public:
	NFMoveObjectExecuter(NFIPluginManager* p, const NFGUID& blockID, const NFGUID& id, const std::string& name)
	{
		this->id = id;
		this->name = name;
		this->logicBlockId = blockID;
		this->pPluginManager = p;

		executerType = NFExecuterType::MoveObject;

		Init();
	}

	virtual void InitInputArgs()
	{
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFExecuterMoveObjectInputArg::toString(NFExecuterMoveObjectInputArg::LastNode);
			var->valueType = NFValueType::Node;

			inputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFExecuterMoveObjectInputArg::toString(NFExecuterMoveObjectInputArg::ObjectID);
			var->valueType = NFValueType::Object;
			var->fromType = NFIODataComFromType::EXTERNAL;

			inputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFExecuterMoveObjectInputArg::toString(NFExecuterMoveObjectInputArg::Position);
			var->valueType = NFValueType::Vector3;
			var->fromType = NFIODataComFromType::EXTERNAL;

			inputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFExecuterMoveObjectInputArg::toString(NFExecuterMoveObjectInputArg::MoveType);
			var->valueType = NFValueType::Int;
			var->fromType = NFIODataComFromType::EXTERNAL;

			inputArgs.push_back(var);
		}
	}

	virtual void InitOutputArgs()
	{
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFExecuterMoveObjectOutputArg::toString(NFExecuterMoveObjectOutputArg::NextNode);
			var->valueType = NFValueType::Node;

			outputArgs.push_back(var);
		}
	}


	// Inherited via NFIExecuter

	virtual void PrepareInputData(const NFGUID& runTimeOnwer, const bool iteration) override;

	virtual void UpdateOutputData(const NFGUID& runTimeOnwer, const bool iteration) override;


	// Inherited via NFIExecuter
	virtual NF_SHARE_PTR<NFIOData> FindOutputNodeIOData() override;

};

class NFSendMessageExecuter : public NFIExecuter
{
private:
	NFSendMessageExecuter() {}
public:
	NFSendMessageExecuter(NFIPluginManager* p, const NFGUID& blockID, const NFGUID& id, const std::string& name)
	{
		this->id = id;
		this->name = name;
		this->logicBlockId = blockID;
		this->pPluginManager = p;

		executerType = NFExecuterType::SendMessage;

		Init();
	}

	virtual void InitInputArgs()
	{
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFExecuterSendMessageInputArg::toString(NFExecuterSendMessageInputArg::LastNode);
			var->valueType = NFValueType::Node;

			inputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFExecuterSendMessageInputArg::toString(NFExecuterSendMessageInputArg::ObjectID);
			var->valueType = NFValueType::Object;
			var->fromType = NFIODataComFromType::BOTH;

			inputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFExecuterSendMessageInputArg::toString(NFExecuterSendMessageInputArg::MessageID);
			var->valueType = NFValueType::Int;
			var->fromType = NFIODataComFromType::BOTH;

			inputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFExecuterSendMessageInputArg::toString(NFExecuterSendMessageInputArg::Data);
			var->valueType = NFValueType::Dictionary;
			var->fromType = NFIODataComFromType::BOTH;

			inputArgs.push_back(var);
		}
	}

	virtual void InitOutputArgs()
	{
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFExecuterMoveObjectOutputArg::toString(NFExecuterMoveObjectOutputArg::NextNode);
			var->valueType = NFValueType::Node;

			outputArgs.push_back(var);
		}
	}


	// Inherited via NFIExecuter

	virtual void PrepareInputData(const NFGUID& runTimeOnwer, const bool iteration) override;

	virtual void UpdateOutputData(const NFGUID& runTimeOnwer, const bool iteration) override;


	// Inherited via NFIExecuter
	virtual NF_SHARE_PTR<NFIOData> FindOutputNodeIOData() override;

};

class NFEnterSceneExecuter : public NFIExecuter
{
private:
	NFEnterSceneExecuter() {}
public:
	NFEnterSceneExecuter(NFIPluginManager* p, const NFGUID& blockID, const NFGUID& id, const std::string& name)
	{
		this->id = id;
		this->name = name;
		this->logicBlockId = blockID;
		this->pPluginManager = p;

		executerType = NFExecuterType::EnterScene;

		Init();
	}

	virtual void InitInputArgs()
	{
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFExecuterEnterSceneInputArg::toString(NFExecuterEnterSceneInputArg::LastNode);
			var->valueType = NFValueType::Node;

			inputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFExecuterEnterSceneInputArg::toString(NFExecuterEnterSceneInputArg::ObjectID);
			var->valueType = NFValueType::Object;
			var->fromType = NFIODataComFromType::EXTERNAL;

			inputArgs.push_back(var);
		}

		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFExecuterEnterSceneInputArg::toString(NFExecuterEnterSceneInputArg::SceneID);
			var->valueType = NFValueType::Int;
			var->fromType = NFIODataComFromType::EXTERNAL;

			inputArgs.push_back(var);
		}

		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFExecuterEnterSceneInputArg::toString(NFExecuterEnterSceneInputArg::GroupID);
			var->valueType = NFValueType::Int;
			var->fromType = NFIODataComFromType::EXTERNAL;

			inputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFExecuterEnterSceneInputArg::toString(NFExecuterEnterSceneInputArg::Position);
			var->valueType = NFValueType::Vector3;
			var->fromType = NFIODataComFromType::EXTERNAL;

			inputArgs.push_back(var);
		}
	}

	virtual void InitOutputArgs()
	{
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFExecuterEnterSceneOutputArg::toString(NFExecuterEnterSceneOutputArg::NextNode);
			var->valueType = NFValueType::Node;

			outputArgs.push_back(var);
		}
	}


	// Inherited via NFIExecuter

	virtual void PrepareInputData(const NFGUID& runTimeOnwer, const bool iteration) override;

	virtual void UpdateOutputData(const NFGUID& runTimeOnwer, const bool iteration) override;


	// Inherited via NFIExecuter
	virtual NF_SHARE_PTR<NFIOData> FindOutputNodeIOData() override;

};


class NFSleepExecuter : public NFIExecuter
{
private:
	NFSleepExecuter() {}
public:
	NFSleepExecuter(NFIPluginManager* p, const NFGUID& blockID, const NFGUID& id, const std::string& name)
	{
		this->id = id;
		this->name = name;
		this->logicBlockId = blockID;
		this->pPluginManager = p;

		executerType = NFExecuterType::Sleep;

		Init();
	}

	virtual void InitInputArgs()
	{
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFExecuterSleepInputArg::toString(NFExecuterSleepInputArg::LastNode);
			var->valueType = NFValueType::Node;

			inputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFExecuterSleepInputArg::toString(NFExecuterSleepInputArg::ObjectID);
			var->valueType = NFValueType::Object;
			var->fromType = NFIODataComFromType::EXTERNAL;

			inputArgs.push_back(var);
		}

		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFExecuterSleepInputArg::toString(NFExecuterSleepInputArg::SleepTime);
			var->valueType = NFValueType::Float;
			var->fromType = NFIODataComFromType::BOTH;

			inputArgs.push_back(var);
		}
	}

	virtual void InitOutputArgs()
	{
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFExecuterSleepOutputArg::toString(NFExecuterSleepOutputArg::NextNode);
			var->valueType = NFValueType::Node;

			outputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFExecuterSleepOutputArg::toString(NFExecuterSleepOutputArg::ObjectID);
			var->valueType = NFValueType::Object;

			outputArgs.push_back(var);
		}
	}


	// Inherited via NFIExecuter

	virtual void PrepareInputData(const NFGUID& runTimeOnwer, const bool iteration) override;

	virtual void UpdateOutputData(const NFGUID& runTimeOnwer, const bool iteration) override;


	// Inherited via NFIExecuter
	virtual NF_SHARE_PTR<NFIOData> FindOutputNodeIOData() override;

};

class NFAddHeartBeatExecuter : public NFIExecuter
{
private:
	NFAddHeartBeatExecuter() {}
public:
	NFAddHeartBeatExecuter(NFIPluginManager* p, const NFGUID& blockID, const NFGUID& id, const std::string& name)
	{
		this->id = id;
		this->name = name;
		this->logicBlockId = blockID;
		this->pPluginManager = p;

		executerType = NFExecuterType::AddHeartBeat;

		Init();
	}

	virtual void InitInputArgs()
	{
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFExecuterAddHeartBeatInputArg::toString(NFExecuterAddHeartBeatInputArg::LastNode);
			var->valueType = NFValueType::Node;

			inputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFExecuterAddHeartBeatInputArg::toString(NFExecuterAddHeartBeatInputArg::ObjectID);
			var->valueType = NFValueType::Object;
			var->fromType = NFIODataComFromType::EXTERNAL;

			inputArgs.push_back(var);
		}

		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFExecuterAddHeartBeatInputArg::toString(NFExecuterAddHeartBeatInputArg::BeatName);
			var->valueType = NFValueType::String;
			var->fromType = NFIODataComFromType::INTERNAL;

			inputArgs.push_back(var);
		}

		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFExecuterAddHeartBeatInputArg::toString(NFExecuterAddHeartBeatInputArg::BeatTime);
			var->valueType = NFValueType::Int;
			var->fromType = NFIODataComFromType::INTERNAL;

			inputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFExecuterAddHeartBeatInputArg::toString(NFExecuterAddHeartBeatInputArg::BeatCount);
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
			var->name = NFExecuterAddHeartBeatOutputArg::toString(NFExecuterAddHeartBeatOutputArg::NextNode);
			var->valueType = NFValueType::Node;

			outputArgs.push_back(var);
		}
	}


	// Inherited via NFIExecuter

	virtual void PrepareInputData(const NFGUID& runTimeOnwer, const bool iteration) override;

	virtual void UpdateOutputData(const NFGUID& runTimeOnwer, const bool iteration) override;


	// Inherited via NFIExecuter
	virtual NF_SHARE_PTR<NFIOData> FindOutputNodeIOData() override;

};

class NFAttackObjectExecuter : public NFIExecuter
{
private:
	NFAttackObjectExecuter() {}
public:
	NFAttackObjectExecuter(NFIPluginManager* p, const NFGUID& blockID, const NFGUID& id, const std::string& name)
	{
		this->id = id;
		this->name = name;
		this->logicBlockId = blockID;
		this->pPluginManager = p;

		executerType = NFExecuterType::AttackObject;

		Init();
	}


	virtual void InitInputArgs()
	{
	}

	virtual void InitOutputArgs()
	{
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = "NextNode";
			var->valueType = NFValueType::Node;

			inputArgs.push_back(var);
		}
	}


	// Inherited via NFIExecuter

	virtual void PrepareInputData(const NFGUID& runTimeOnwer, const bool iteration) override;

	virtual void UpdateOutputData(const NFGUID& runTimeOnwer, const bool iteration) override;


	// Inherited via NFIExecuter
	virtual NF_SHARE_PTR<NFIOData> FindOutputNodeIOData() override;

};


class NFUseSkillExecuter : public NFIExecuter
{
private:
	NFUseSkillExecuter() {}
public:
	NFUseSkillExecuter(NFIPluginManager* p, const NFGUID& blockID, const NFGUID& id, const std::string& name)
	{
		this->id = id;
		this->name = name;
		this->logicBlockId = blockID;
		this->pPluginManager = p;

		executerType = NFExecuterType::UseSkill;

		Init();
	}


	virtual void InitInputArgs()
	{
	}

	virtual void InitOutputArgs()
	{
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = "NextNode";
			var->valueType = NFValueType::Node;

			inputArgs.push_back(var);
		}
	}

	// Inherited via NFIExecuter
	virtual void PrepareInputData(const NFGUID& runTimeOnwer, const bool iteration) override;
	virtual void UpdateOutputData(const NFGUID& runTimeOnwer, const bool iteration) override;

	// Inherited via NFIExecuter
	virtual NF_SHARE_PTR<NFIOData> FindOutputNodeIOData() override;
};


class NFUseItemExecuter : public NFIExecuter
{
private:
	NFUseItemExecuter() {}
public:
	NFUseItemExecuter(NFIPluginManager* p, const NFGUID& blockID, const NFGUID& id, const std::string& name)
	{
		this->id = id;
		this->name = name;
		this->logicBlockId = blockID;
		this->pPluginManager = p;

		executerType = NFExecuterType::UseItem;

		Init();
	}


	virtual void InitInputArgs()
	{
	}

	virtual void InitOutputArgs()
	{
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = "NextNode";
			var->valueType = NFValueType::Node;

			inputArgs.push_back(var);
		}
	}

	// Inherited via NFIExecuter
	virtual void PrepareInputData(const NFGUID& runTimeOnwer, const bool iteration) override;
	virtual void UpdateOutputData(const NFGUID& runTimeOnwer, const bool iteration) override;

	// Inherited via NFIExecuter
	virtual NF_SHARE_PTR<NFIOData> FindOutputNodeIOData() override;
};
