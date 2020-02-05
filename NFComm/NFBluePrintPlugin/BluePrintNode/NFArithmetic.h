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

class NFArithmetic : public NFIArithmetic
{
private:
	NFArithmetic() {}

public:
	NFArithmetic(NFIPluginManager* p, const NFGUID& blockID, const NFGUID& id, const std::string& name)
	{
		this->name = name;
		this->id = id;
		this->logicBlockId = blockID;
		this->pPluginManager = p;


		Init();

	}

	virtual void InitInputArgs()
	{
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFArithmeticInputArg::toString(NFArithmeticInputArg::LeftInput);
			var->valueType = NFValueType::Object;

			inputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFArithmeticInputArg::toString(NFArithmeticInputArg::ALU);
			var->valueType = NFValueType::String;

			inputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFArithmeticInputArg::toString(NFArithmeticInputArg::RightInput);
			var->valueType = NFValueType::String;

			inputArgs.push_back(var);
		}
	}

	virtual void InitOutputArgs()
	{
		
	}

	virtual void UpdateOutputData() override;
	virtual void PrepareInputData() override;

	// Inherited via NFIArithmetic
	virtual NF_SHARE_PTR<NFBluePrintNodeBase> FindNextNode() override;
};