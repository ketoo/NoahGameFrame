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

class NFCreateObjectExecutor : public NFIExecutor
{
private:
	NFCreateObjectExecutor() {}
public:
	NFCreateObjectExecutor(NFIPluginManager* p, const NFGUID& blockID, const NFGUID& id, const std::string& name)
	{
		this->id = id;
		this->name = name;
		this->logicBlockId = blockID;
		this->pPluginManager = p;

		executerType = NFExecutorType::CreateObject;

		Init();
	}

	virtual void InitInputArgs()
	{
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFExecutorCreateObjectInputArg::toString(NFExecutorCreateObjectInputArg::PreNode);
			var->SetValueType(NFValueType::Node);

			inputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFExecutorCreateObjectInputArg::toString(NFExecutorCreateObjectInputArg::ClassName);
			var->SetValueType(NFValueType::String);
			var->fromType = NFIODataComFromType::EXTERNAL;

			inputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFExecutorCreateObjectInputArg::toString(NFExecutorCreateObjectInputArg::ConfigID);
			var->SetValueType(NFValueType::String);
			var->fromType = NFIODataComFromType::EXTERNAL;

			inputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFExecutorCreateObjectInputArg::toString(NFExecutorCreateObjectInputArg::SceneID);
			var->SetValueType(NFValueType::Int);
			var->fromType = NFIODataComFromType::EXTERNAL;

			inputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFExecutorCreateObjectInputArg::toString(NFExecutorCreateObjectInputArg::GroupID);
			var->SetValueType(NFValueType::Int);
			var->fromType = NFIODataComFromType::EXTERNAL;

			inputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFExecutorCreateObjectInputArg::toString(NFExecutorCreateObjectInputArg::Position);
			var->SetValueType(NFValueType::Vector3);
			var->fromType = NFIODataComFromType::EXTERNAL;

			inputArgs.push_back(var);
		}
	}

	virtual void InitOutputArgs()
	{
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFExecutorCreateObjectOutputputArg::toString(NFExecutorCreateObjectOutputputArg::NextNode);
			var->SetValueType(NFValueType::Node);

			outputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFExecutorCreateObjectOutputputArg::toString(NFExecutorCreateObjectOutputputArg::ObjectID);
			var->SetValueType(NFValueType::Object);

			outputArgs.push_back(var);
		}
	}


	virtual void UpdateOutputData(const NFGUID& runTimeOwner) override;

	virtual NF_SHARE_PTR<NFIOData> FindOutputNodeIOData() override;

};

class NFDestroyObjectExecutor : public NFIExecutor
{
private:
	NFDestroyObjectExecutor() {}
public:
	NFDestroyObjectExecutor(NFIPluginManager* p, const NFGUID& blockID, const NFGUID& id, const std::string& name)
	{
		this->id = id;
		this->name = name;
		this->logicBlockId = blockID;
		this->pPluginManager = p;

		executerType = NFExecutorType::DestroyObject;

		Init();

	};


	virtual void InitInputArgs()
	{
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFExecutorDestroyObjectInputArg::toString(NFExecutorDestroyObjectInputArg::PreNode);
			var->SetValueType(NFValueType::Node);

			inputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFExecutorDestroyObjectInputArg::toString(NFExecutorDestroyObjectInputArg::ObjectID);
			var->SetValueType(NFValueType::Float);
			var->fromType = NFIODataComFromType::INTERNAL;

			inputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFExecutorDestroyObjectInputArg::toString(NFExecutorDestroyObjectInputArg::ObjectID);
			var->SetValueType(NFValueType::Object);
			var->fromType = NFIODataComFromType::EXTERNAL;

			inputArgs.push_back(var);
		}
	}

	virtual void InitOutputArgs()
	{

		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFExecutorDestroyObjectOutputputArg::toString(NFExecutorDestroyObjectOutputputArg::NextNode);
			var->SetValueType(NFValueType::Node);

			outputArgs.push_back(var);
		}
	}


	virtual void UpdateOutputData(const NFGUID& runTimeOwner) override;
	virtual NF_SHARE_PTR<NFIOData> FindOutputNodeIOData() override;

};

class NFGameEventExecutor : public NFIExecutor
{
private:
	NFGameEventExecutor() {}
public:
	NFGameEventExecutor(NFIPluginManager* p, const NFGUID& blockID, const NFGUID& id, const std::string& name)
	{
		this->id = id;
		this->name = name;
		this->logicBlockId = blockID;
		this->pPluginManager = p;

		executerType = NFExecutorType::GameEvent;

		Init();
	}

	virtual void InitInputArgs()
	{
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFExecutorGameEventInputArg::toString(NFExecutorGameEventInputArg::PreNode);
			var->SetValueType(NFValueType::Node);

			inputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFExecutorGameEventInputArg::toString(NFExecutorGameEventInputArg::ObjectID);
			var->SetValueType(NFValueType::Object);
			var->fromType = NFIODataComFromType::EXTERNAL;

			inputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFExecutorGameEventInputArg::toString(NFExecutorGameEventInputArg::EventID);
			var->SetValueType(NFValueType::Int);
			var->fromType = NFIODataComFromType::EXTERNAL;

			inputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFExecutorGameEventInputArg::toString(NFExecutorGameEventInputArg::Dctionary);
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
			var->name = NFExecutorGameEventOutputArg::toString(NFExecutorGameEventOutputArg::NextNode);
			var->SetValueType(NFValueType::Node);

			outputArgs.push_back(var);
		}
	}

	virtual void UpdateOutputData(const NFGUID& runTimeOwner) override;
	virtual NF_SHARE_PTR<NFIOData> FindOutputNodeIOData() override;
};

class NFMoveObjectExecutor : public NFIExecutor
{
private:
	NFMoveObjectExecutor() {}
public:
	NFMoveObjectExecutor(NFIPluginManager* p, const NFGUID& blockID, const NFGUID& id, const std::string& name)
	{
		this->id = id;
		this->name = name;
		this->logicBlockId = blockID;
		this->pPluginManager = p;

		executerType = NFExecutorType::MoveObject;

		Init();
	}

	virtual void InitInputArgs()
	{
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFExecutorMoveObjectInputArg::toString(NFExecutorMoveObjectInputArg::PreNode);
			var->SetValueType(NFValueType::Node);

			inputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFExecutorMoveObjectInputArg::toString(NFExecutorMoveObjectInputArg::ObjectID);
			var->SetValueType(NFValueType::Object);
			var->fromType = NFIODataComFromType::EXTERNAL;

			inputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFExecutorMoveObjectInputArg::toString(NFExecutorMoveObjectInputArg::Position);
			var->SetValueType(NFValueType::Vector3);
			var->fromType = NFIODataComFromType::EXTERNAL;

			inputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFExecutorMoveObjectInputArg::toString(NFExecutorMoveObjectInputArg::MoveType);
			var->SetValueType(NFValueType::Int);
			var->fromType = NFIODataComFromType::EXTERNAL;

			inputArgs.push_back(var);
		}
	}

	virtual void InitOutputArgs()
	{
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFExecutorMoveObjectOutputArg::toString(NFExecutorMoveObjectOutputArg::NextNode);
			var->SetValueType(NFValueType::Node);

			outputArgs.push_back(var);
		}
	}

	virtual void UpdateOutputData(const NFGUID& runTimeOwner) override;
	virtual NF_SHARE_PTR<NFIOData> FindOutputNodeIOData() override;
};

class NFSendMessageExecutor : public NFIExecutor
{
private:
	NFSendMessageExecutor() {}
public:
	NFSendMessageExecutor(NFIPluginManager* p, const NFGUID& blockID, const NFGUID& id, const std::string& name)
	{
		this->id = id;
		this->name = name;
		this->logicBlockId = blockID;
		this->pPluginManager = p;

		executerType = NFExecutorType::SendMessage;

		Init();
	}

	virtual void InitInputArgs()
	{
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFExecutorSendMessageInputArg::toString(NFExecutorSendMessageInputArg::PreNode);
			var->SetValueType(NFValueType::Node);

			inputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFExecutorSendMessageInputArg::toString(NFExecutorSendMessageInputArg::ObjectID);
			var->SetValueType(NFValueType::Object);
			var->fromType = NFIODataComFromType::BOTH;

			inputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFExecutorSendMessageInputArg::toString(NFExecutorSendMessageInputArg::MessageID);
			var->SetValueType(NFValueType::Int);
			var->fromType = NFIODataComFromType::BOTH;

			inputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFExecutorSendMessageInputArg::toString(NFExecutorSendMessageInputArg::Data);
			var->SetValueType(NFValueType::Dictionary);
			var->fromType = NFIODataComFromType::BOTH;

			inputArgs.push_back(var);
		}
	}

	virtual void InitOutputArgs()
	{
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFExecutorMoveObjectOutputArg::toString(NFExecutorMoveObjectOutputArg::NextNode);
			var->SetValueType(NFValueType::Node);

			outputArgs.push_back(var);
		}
	}

	virtual void UpdateOutputData(const NFGUID& runTimeOwner) override;
	virtual NF_SHARE_PTR<NFIOData> FindOutputNodeIOData() override;
};

class NFEnterSceneExecutor : public NFIExecutor
{
private:
	NFEnterSceneExecutor() {}
public:
	NFEnterSceneExecutor(NFIPluginManager* p, const NFGUID& blockID, const NFGUID& id, const std::string& name)
	{
		this->id = id;
		this->name = name;
		this->logicBlockId = blockID;
		this->pPluginManager = p;

		executerType = NFExecutorType::EnterScene;

		Init();
	}

	virtual void InitInputArgs()
	{
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFExecutorEnterSceneInputArg::toString(NFExecutorEnterSceneInputArg::PreNode);
			var->SetValueType(NFValueType::Node);

			inputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFExecutorEnterSceneInputArg::toString(NFExecutorEnterSceneInputArg::ObjectID);
			var->SetValueType(NFValueType::Object);
			var->fromType = NFIODataComFromType::EXTERNAL;

			inputArgs.push_back(var);
		}

		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFExecutorEnterSceneInputArg::toString(NFExecutorEnterSceneInputArg::SceneID);
			var->SetValueType(NFValueType::Int);
			var->fromType = NFIODataComFromType::EXTERNAL;

			inputArgs.push_back(var);
		}

		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFExecutorEnterSceneInputArg::toString(NFExecutorEnterSceneInputArg::GroupID);
			var->SetValueType(NFValueType::Int);
			var->fromType = NFIODataComFromType::EXTERNAL;

			inputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFExecutorEnterSceneInputArg::toString(NFExecutorEnterSceneInputArg::Position);
			var->SetValueType(NFValueType::Vector3);
			var->fromType = NFIODataComFromType::EXTERNAL;

			inputArgs.push_back(var);
		}
	}

	virtual void InitOutputArgs()
	{
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFExecutorEnterSceneOutputArg::toString(NFExecutorEnterSceneOutputArg::NextNode);
			var->SetValueType(NFValueType::Node);

			outputArgs.push_back(var);
		}
	}

	virtual void UpdateOutputData(const NFGUID& runTimeOwner) override;
	virtual NF_SHARE_PTR<NFIOData> FindOutputNodeIOData() override;
};


class NFSleepExecutor : public NFIExecutor
{
private:
	NFSleepExecutor() {}
public:
	NFSleepExecutor(NFIPluginManager* p, const NFGUID& blockID, const NFGUID& id, const std::string& name)
	{
		this->id = id;
		this->name = name;
		this->logicBlockId = blockID;
		this->pPluginManager = p;

		executerType = NFExecutorType::Sleep;

		Init();
	}

	virtual void InitInputArgs()
	{
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFExecutorSleepInputArg::toString(NFExecutorSleepInputArg::PreNode);
			var->SetValueType(NFValueType::Node);

			inputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFExecutorSleepInputArg::toString(NFExecutorSleepInputArg::ObjectID);
			var->SetValueType(NFValueType::Object);
			var->fromType = NFIODataComFromType::EXTERNAL;

			inputArgs.push_back(var);
		}

		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFExecutorSleepInputArg::toString(NFExecutorSleepInputArg::SleepTime);
			var->SetValueType(NFValueType::Float);
			var->fromType = NFIODataComFromType::BOTH;

			inputArgs.push_back(var);
		}
	}

	virtual void InitOutputArgs()
	{
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFExecutorSleepOutputArg::toString(NFExecutorSleepOutputArg::NextNode);
			var->SetValueType(NFValueType::Node);

			outputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFExecutorSleepOutputArg::toString(NFExecutorSleepOutputArg::ObjectID);
			var->SetValueType(NFValueType::Object);

			outputArgs.push_back(var);
		}
	}

	virtual void UpdateOutputData(const NFGUID& runTimeOwner) override;

	virtual NF_SHARE_PTR<NFIOData> FindOutputNodeIOData() override;
};

class NFAddHeartBeatExecutor : public NFIExecutor
{
private:
	NFAddHeartBeatExecutor() {}
public:
	NFAddHeartBeatExecutor(NFIPluginManager* p, const NFGUID& blockID, const NFGUID& id, const std::string& name)
	{
		this->id = id;
		this->name = name;
		this->logicBlockId = blockID;
		this->pPluginManager = p;

		executerType = NFExecutorType::AddHeartBeat;

		Init();
	}

	virtual void InitInputArgs()
	{
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFExecutorAddHeartBeatInputArg::toString(NFExecutorAddHeartBeatInputArg::PreNode);
			var->SetValueType(NFValueType::Node);

			inputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFExecutorAddHeartBeatInputArg::toString(NFExecutorAddHeartBeatInputArg::ObjectID);
			var->SetValueType(NFValueType::Object);
			var->fromType = NFIODataComFromType::EXTERNAL;

			inputArgs.push_back(var);
		}

		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFExecutorAddHeartBeatInputArg::toString(NFExecutorAddHeartBeatInputArg::BeatName);
			var->SetValueType(NFValueType::String);
			var->fromType = NFIODataComFromType::INTERNAL;

			inputArgs.push_back(var);
		}

		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFExecutorAddHeartBeatInputArg::toString(NFExecutorAddHeartBeatInputArg::BeatTime);
			var->SetValueType(NFValueType::Int);
			var->fromType = NFIODataComFromType::INTERNAL;

			inputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFExecutorAddHeartBeatInputArg::toString(NFExecutorAddHeartBeatInputArg::BeatCount);
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
			var->name = NFExecutorAddHeartBeatOutputArg::toString(NFExecutorAddHeartBeatOutputArg::NextNode);
			var->SetValueType(NFValueType::Node);

			outputArgs.push_back(var);
		}
	}

	virtual void UpdateOutputData(const NFGUID& runTimeOwner) override;

	virtual NF_SHARE_PTR<NFIOData> FindOutputNodeIOData() override;

};

class NFAttackObjectExecutor : public NFIExecutor
{
private:
	NFAttackObjectExecutor() {}
public:
	NFAttackObjectExecutor(NFIPluginManager* p, const NFGUID& blockID, const NFGUID& id, const std::string& name)
	{
		this->id = id;
		this->name = name;
		this->logicBlockId = blockID;
		this->pPluginManager = p;

		executerType = NFExecutorType::AttackObject;

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
			var->SetValueType(NFValueType::Node);

			inputArgs.push_back(var);
		}
	}

	virtual void UpdateOutputData(const NFGUID& runTimeOwner) override;
	virtual NF_SHARE_PTR<NFIOData> FindOutputNodeIOData() override;
};
