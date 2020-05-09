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

		executerType = NFExecuterType::CreateObject;

		Init();
	}

	virtual void InitInputArgs()
	{
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFExecuterCreateObjectInputArg::toString(NFExecuterCreateObjectInputArg::LastNode);
			var->SetValueType(NFValueType::Node);

			inputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFExecuterCreateObjectInputArg::toString(NFExecuterCreateObjectInputArg::ClassName);
			var->SetValueType(NFValueType::String);
			var->fromType = NFIODataComFromType::EXTERNAL;

			inputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFExecuterCreateObjectInputArg::toString(NFExecuterCreateObjectInputArg::ConfigID);
			var->SetValueType(NFValueType::String);
			var->fromType = NFIODataComFromType::EXTERNAL;

			inputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFExecuterCreateObjectInputArg::toString(NFExecuterCreateObjectInputArg::SceneID);
			var->SetValueType(NFValueType::Int);
			var->fromType = NFIODataComFromType::EXTERNAL;

			inputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFExecuterCreateObjectInputArg::toString(NFExecuterCreateObjectInputArg::GroupID);
			var->SetValueType(NFValueType::Int);
			var->fromType = NFIODataComFromType::EXTERNAL;

			inputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFExecuterCreateObjectInputArg::toString(NFExecuterCreateObjectInputArg::Position);
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
			var->name = NFExecuterCreateObjectOutputputArg::toString(NFExecuterCreateObjectOutputputArg::NextNode);
			var->SetValueType(NFValueType::Node);

			outputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFExecuterCreateObjectOutputputArg::toString(NFExecuterCreateObjectOutputputArg::ObjectID);
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

		executerType = NFExecuterType::DestroyObject;

		Init();

	};


	virtual void InitInputArgs()
	{
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFExecuterDestroyObjectInputArg::toString(NFExecuterDestroyObjectInputArg::LastNode);
			var->SetValueType(NFValueType::Node);

			inputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFExecuterDestroyObjectInputArg::toString(NFExecuterDestroyObjectInputArg::ObjectID);
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
			var->name = NFExecuterDestroyObjectOutputputArg::toString(NFExecuterDestroyObjectOutputputArg::NextNode);
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

		executerType = NFExecuterType::GameEvent;

		Init();
	}

	virtual void InitInputArgs()
	{
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFExecuterGameEventInputArg::toString(NFExecuterGameEventInputArg::LastNode);
			var->SetValueType(NFValueType::Node);

			inputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFExecuterGameEventInputArg::toString(NFExecuterGameEventInputArg::ObjectID);
			var->SetValueType(NFValueType::Object);
			var->fromType = NFIODataComFromType::EXTERNAL;

			inputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFExecuterGameEventInputArg::toString(NFExecuterGameEventInputArg::EventID);
			var->SetValueType(NFValueType::Int);
			var->fromType = NFIODataComFromType::EXTERNAL;

			inputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFExecuterGameEventInputArg::toString(NFExecuterGameEventInputArg::Dctionary);
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
			var->name = NFExecuterGameEventOutputArg::toString(NFExecuterGameEventOutputArg::NextNode);
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

		executerType = NFExecuterType::MoveObject;

		Init();
	}

	virtual void InitInputArgs()
	{
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFExecuterMoveObjectInputArg::toString(NFExecuterMoveObjectInputArg::LastNode);
			var->SetValueType(NFValueType::Node);

			inputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFExecuterMoveObjectInputArg::toString(NFExecuterMoveObjectInputArg::ObjectID);
			var->SetValueType(NFValueType::Object);
			var->fromType = NFIODataComFromType::EXTERNAL;

			inputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFExecuterMoveObjectInputArg::toString(NFExecuterMoveObjectInputArg::Position);
			var->SetValueType(NFValueType::Vector3);
			var->fromType = NFIODataComFromType::EXTERNAL;

			inputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFExecuterMoveObjectInputArg::toString(NFExecuterMoveObjectInputArg::MoveType);
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
			var->name = NFExecuterMoveObjectOutputArg::toString(NFExecuterMoveObjectOutputArg::NextNode);
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

		executerType = NFExecuterType::SendMessage;

		Init();
	}

	virtual void InitInputArgs()
	{
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFExecuterSendMessageInputArg::toString(NFExecuterSendMessageInputArg::LastNode);
			var->SetValueType(NFValueType::Node);

			inputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFExecuterSendMessageInputArg::toString(NFExecuterSendMessageInputArg::ObjectID);
			var->SetValueType(NFValueType::Object);
			var->fromType = NFIODataComFromType::BOTH;

			inputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFExecuterSendMessageInputArg::toString(NFExecuterSendMessageInputArg::MessageID);
			var->SetValueType(NFValueType::Int);
			var->fromType = NFIODataComFromType::BOTH;

			inputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFExecuterSendMessageInputArg::toString(NFExecuterSendMessageInputArg::Data);
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
			var->name = NFExecuterMoveObjectOutputArg::toString(NFExecuterMoveObjectOutputArg::NextNode);
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

		executerType = NFExecuterType::EnterScene;

		Init();
	}

	virtual void InitInputArgs()
	{
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFExecuterEnterSceneInputArg::toString(NFExecuterEnterSceneInputArg::LastNode);
			var->SetValueType(NFValueType::Node);

			inputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFExecuterEnterSceneInputArg::toString(NFExecuterEnterSceneInputArg::ObjectID);
			var->SetValueType(NFValueType::Object);
			var->fromType = NFIODataComFromType::EXTERNAL;

			inputArgs.push_back(var);
		}

		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFExecuterEnterSceneInputArg::toString(NFExecuterEnterSceneInputArg::SceneID);
			var->SetValueType(NFValueType::Int);
			var->fromType = NFIODataComFromType::EXTERNAL;

			inputArgs.push_back(var);
		}

		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFExecuterEnterSceneInputArg::toString(NFExecuterEnterSceneInputArg::GroupID);
			var->SetValueType(NFValueType::Int);
			var->fromType = NFIODataComFromType::EXTERNAL;

			inputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFExecuterEnterSceneInputArg::toString(NFExecuterEnterSceneInputArg::Position);
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
			var->name = NFExecuterEnterSceneOutputArg::toString(NFExecuterEnterSceneOutputArg::NextNode);
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

		executerType = NFExecuterType::Sleep;

		Init();
	}

	virtual void InitInputArgs()
	{
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFExecuterSleepInputArg::toString(NFExecuterSleepInputArg::LastNode);
			var->SetValueType(NFValueType::Node);

			inputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFExecuterSleepInputArg::toString(NFExecuterSleepInputArg::ObjectID);
			var->SetValueType(NFValueType::Object);
			var->fromType = NFIODataComFromType::EXTERNAL;

			inputArgs.push_back(var);
		}

		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFExecuterSleepInputArg::toString(NFExecuterSleepInputArg::SleepTime);
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
			var->name = NFExecuterSleepOutputArg::toString(NFExecuterSleepOutputArg::NextNode);
			var->SetValueType(NFValueType::Node);

			outputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFExecuterSleepOutputArg::toString(NFExecuterSleepOutputArg::ObjectID);
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

		executerType = NFExecuterType::AddHeartBeat;

		Init();
	}

	virtual void InitInputArgs()
	{
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFExecuterAddHeartBeatInputArg::toString(NFExecuterAddHeartBeatInputArg::LastNode);
			var->SetValueType(NFValueType::Node);

			inputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFExecuterAddHeartBeatInputArg::toString(NFExecuterAddHeartBeatInputArg::ObjectID);
			var->SetValueType(NFValueType::Object);
			var->fromType = NFIODataComFromType::EXTERNAL;

			inputArgs.push_back(var);
		}

		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFExecuterAddHeartBeatInputArg::toString(NFExecuterAddHeartBeatInputArg::BeatName);
			var->SetValueType(NFValueType::String);
			var->fromType = NFIODataComFromType::INTERNAL;

			inputArgs.push_back(var);
		}

		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFExecuterAddHeartBeatInputArg::toString(NFExecuterAddHeartBeatInputArg::BeatTime);
			var->SetValueType(NFValueType::Int);
			var->fromType = NFIODataComFromType::INTERNAL;

			inputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFExecuterAddHeartBeatInputArg::toString(NFExecuterAddHeartBeatInputArg::BeatCount);
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
			var->name = NFExecuterAddHeartBeatOutputArg::toString(NFExecuterAddHeartBeatOutputArg::NextNode);
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
			var->SetValueType(NFValueType::Node);

			inputArgs.push_back(var);
		}
	}

	virtual void UpdateOutputData(const NFGUID& runTimeOwner) override;
	virtual NF_SHARE_PTR<NFIOData> FindOutputNodeIOData() override;
};
