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

class NFIntBranch : public NFIBranch
{
private:
	NFIntBranch() {}

public:
	NFIntBranch(NFIPluginManager* p, const NFGUID& blockID, const NFGUID& id, const std::string& name)
	{
		this->name = name;
		this->id = id;
		this->logicBlockId = blockID;
		this->pPluginManager = p;

		branchType = NFBranchType::IntBranch;
		comparatorType = NFComparatorType::Equal;

		Init();
	}

	virtual void InitInputArgs() override
	{
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFBranchInputArg::toString(NFBranchInputArg::PreNode);
			var->SetValueType(NFValueType::Node);

			inputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFBranchInputArg::toString(NFBranchInputArg::LeftInput);
			var->SetValueType(NFValueType::Int);
			var->fromType = NFIODataComFromType::EXTERNAL;

			inputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFBranchInputArg::toString(NFBranchInputArg::Comparator);
			var->SetValueType(NFValueType::UNKNOW);
			var->fromType = NFIODataComFromType::INTERNAL;

			inputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFBranchInputArg::toString(NFBranchInputArg::RightInput);
			var->SetValueType(NFValueType::Int);
			var->fromType = NFIODataComFromType::EXTERNAL;

			inputArgs.push_back(var);
		}
	}

	virtual void InitOutputArgs() override
	{
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFBranchOutputArg::toString(NFBranchOutputArg::TrueOut);
			var->SetValueType(NFValueType::Node);

			outputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFBranchOutputArg::toString(NFBranchOutputArg::FalseOut);
			var->SetValueType(NFValueType::Node);

			outputArgs.push_back(var);
		}
	}

	// Inherited via NFIBranch
	virtual void UpdateOutputData(const NFGUID& runTimeOwner) override;
};


class NFStringBranch : public NFIBranch
{
private:
	NFStringBranch() {}

public:
	NFStringBranch(NFIPluginManager* p, const NFGUID& blockID, const NFGUID& id, const std::string& name)
	{
		this->name = name;
		this->id = id;
		this->logicBlockId = blockID;
		this->pPluginManager = p;

		branchType = NFBranchType::StringBranch;
		comparatorType = NFComparatorType::Equal;

		Init();

	}

	virtual void InitInputArgs()
	{
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFBranchInputArg::toString(NFBranchInputArg::PreNode);
			var->SetValueType(NFValueType::Node);

			inputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFBranchInputArg::toString(NFBranchInputArg::LeftInput);
			var->SetValueType(NFValueType::UNKNOW);
			var->fromType = NFIODataComFromType::EXTERNAL;

			inputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFBranchInputArg::toString(NFBranchInputArg::Comparator);
			var->SetValueType(NFValueType::String);
			var->fromType = NFIODataComFromType::INTERNAL;

			inputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFBranchInputArg::toString(NFBranchInputArg::RightInput);
			var->SetValueType(NFValueType::String);
			var->fromType = NFIODataComFromType::EXTERNAL;

			inputArgs.push_back(var);
		}
	}

	virtual void InitOutputArgs()
	{
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFBranchOutputArg::toString(NFBranchOutputArg::TrueOut);
			var->SetValueType(NFValueType::Node);

			outputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFBranchOutputArg::toString(NFBranchOutputArg::FalseOut);
			var->SetValueType(NFValueType::Node);

			outputArgs.push_back(var);
		}
	}

	virtual void UpdateOutputData(const NFGUID& runTimeOwner) override;
};


class NFFloatBranch : public NFIBranch
{
private:
	NFFloatBranch() {}

public:
	NFFloatBranch(NFIPluginManager* p, const NFGUID& blockID, const NFGUID& id, const std::string& name)
	{
		this->name = name;
		this->id = id;
		this->logicBlockId = blockID;
		this->pPluginManager = p;

		branchType = NFBranchType::FloatBranch;
		comparatorType = NFComparatorType::Equal;

		Init();

	}

	virtual void InitInputArgs()
	{
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFBranchInputArg::toString(NFBranchInputArg::PreNode);
			var->SetValueType(NFValueType::Node);

			inputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFBranchInputArg::toString(NFBranchInputArg::LeftInput);
			var->SetValueType(NFValueType::Float);
			var->fromType = NFIODataComFromType::EXTERNAL;

			inputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFBranchInputArg::toString(NFBranchInputArg::Comparator);
			var->SetValueType(NFValueType::UNKNOW);
			var->fromType = NFIODataComFromType::INTERNAL;

			inputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFBranchInputArg::toString(NFBranchInputArg::RightInput);
			var->SetValueType(NFValueType::Float);
			var->fromType = NFIODataComFromType::EXTERNAL;

			inputArgs.push_back(var);
		}
	}

	virtual void InitOutputArgs()
	{
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFBranchOutputArg::toString(NFBranchOutputArg::TrueOut);
			var->SetValueType(NFValueType::Node);

			outputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFBranchOutputArg::toString(NFBranchOutputArg::FalseOut);
			var->SetValueType(NFValueType::Node);

			outputArgs.push_back(var);
		}
	}

	virtual void UpdateOutputData(const NFGUID& runTimeOwner) override;
};


class NFObjectBranch : public NFIBranch
{
private:
	NFObjectBranch() {}

public:
	NFObjectBranch(NFIPluginManager* p, const NFGUID& blockID, const NFGUID& id, const std::string& name)
	{
		this->name = name;
		this->id = id;
		this->logicBlockId = blockID;
		this->pPluginManager = p;

		branchType = NFBranchType::ObjectBranch;
		comparatorType = NFComparatorType::Equal;

		Init();

	}

	virtual void InitInputArgs()
	{
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFBranchInputArg::toString(NFBranchInputArg::PreNode);
			var->SetValueType(NFValueType::Node);

			inputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFBranchInputArg::toString(NFBranchInputArg::LeftInput);
			var->SetValueType(NFValueType::Object);
			var->fromType = NFIODataComFromType::EXTERNAL;

			inputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFBranchInputArg::toString(NFBranchInputArg::Comparator);
			var->SetValueType(NFValueType::UNKNOW);
			var->fromType = NFIODataComFromType::INTERNAL;

			inputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFBranchInputArg::toString(NFBranchInputArg::RightInput);
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
			var->name = NFBranchOutputArg::toString(NFBranchOutputArg::TrueOut);
			var->SetValueType(NFValueType::Node);

			outputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFBranchOutputArg::toString(NFBranchOutputArg::FalseOut);
			var->SetValueType(NFValueType::Node);

			outputArgs.push_back(var);
		}
	}

	virtual void UpdateOutputData(const NFGUID& runTimeOwner) override;
};



class NFVector2Branch : public NFIBranch
{
private:
	NFVector2Branch() {}

public:
	NFVector2Branch(NFIPluginManager* p, const NFGUID& blockID, const NFGUID& id, const std::string& name)
	{
		this->name = name;
		this->id = id;
		this->logicBlockId = blockID;
		this->pPluginManager = p;

		branchType = NFBranchType::ObjectBranch;
		comparatorType = NFComparatorType::Equal;

		Init();

	}

	virtual void InitInputArgs()
	{
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFBranchInputArg::toString(NFBranchInputArg::PreNode);
			var->SetValueType(NFValueType::Node);

			inputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFBranchInputArg::toString(NFBranchInputArg::LeftInput);
			var->SetValueType(NFValueType::Vector2);
			var->fromType = NFIODataComFromType::EXTERNAL;

			inputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFBranchInputArg::toString(NFBranchInputArg::Comparator);
			var->SetValueType(NFValueType::UNKNOW);
			var->fromType = NFIODataComFromType::INTERNAL;

			inputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFBranchInputArg::toString(NFBranchInputArg::RightInput);
			var->SetValueType(NFValueType::Vector2);
			var->fromType = NFIODataComFromType::EXTERNAL;

			inputArgs.push_back(var);
		}
	}

	virtual void InitOutputArgs()
	{
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFBranchOutputArg::toString(NFBranchOutputArg::TrueOut);
			var->SetValueType(NFValueType::Node);

			outputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFBranchOutputArg::toString(NFBranchOutputArg::FalseOut);
			var->SetValueType(NFValueType::Node);

			outputArgs.push_back(var);
		}
	}

	virtual void UpdateOutputData(const NFGUID& runTimeOwner) override;
};

class NFVector3Branch : public NFIBranch
{
private:
	NFVector3Branch() {}

public:
	NFVector3Branch(NFIPluginManager* p, const NFGUID& blockID, const NFGUID& id, const std::string& name)
	{
		this->name = name;
		this->id = id;
		this->logicBlockId = blockID;
		this->pPluginManager = p;

		branchType = NFBranchType::ObjectBranch;
		comparatorType = NFComparatorType::Equal;

		Init();

	}

	virtual void InitInputArgs()
	{
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFBranchInputArg::toString(NFBranchInputArg::PreNode);
			var->SetValueType(NFValueType::Node);

			inputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFBranchInputArg::toString(NFBranchInputArg::LeftInput);
			var->SetValueType(NFValueType::Vector3);
			var->fromType = NFIODataComFromType::EXTERNAL;

			inputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFBranchInputArg::toString(NFBranchInputArg::Comparator);
			var->SetValueType(NFValueType::UNKNOW);
			var->fromType = NFIODataComFromType::INTERNAL;

			inputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFBranchInputArg::toString(NFBranchInputArg::RightInput);
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
			var->name = NFBranchOutputArg::toString(NFBranchOutputArg::TrueOut);
			var->SetValueType(NFValueType::Node);

			outputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFBranchOutputArg::toString(NFBranchOutputArg::FalseOut);
			var->SetValueType(NFValueType::Node);

			outputArgs.push_back(var);
		}
	}

	virtual void UpdateOutputData(const NFGUID& runTimeOwner) override;
};