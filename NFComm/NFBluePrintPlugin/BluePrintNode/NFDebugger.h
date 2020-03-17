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
#include "NFComm/NFPluginModule/NFIElementModule.h"
#include "NFComm/NFPluginModule/NFIClassModule.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"


class NFDebugger : public NFIDebugger
{
private:
	NFDebugger() {}
public:
	NFDebugger(NFIPluginManager* p, const NFGUID& blockID, const NFGUID& id, const std::string& name)
	{
		this->id = id;
		this->name = name;
		this->pPluginManager = p;
		this->logicBlockId = blockID;

		m_pLogModule = this->pPluginManager->FindModule<NFILogModule>();

		Init();
	}


	virtual void InitInputArgs()
	{
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFDebuggerInputArg::toString(NFDebuggerInputArg::LastNode);
			var->valueType = NFValueType::Node;
			var->fromType = NFIODataComFromType::EXTERNAL;

			inputArgs.push_back(var);
		} 
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFDebuggerInputArg::toString(NFDebuggerInputArg::LogLevel);
			var->valueType = NFValueType::Int;
			var->fromType = NFIODataComFromType::INTERNAL;

			inputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFDebuggerInputArg::toString(NFDebuggerInputArg::LogData);
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
			var->name = NFDebuggerOutputArg::toString(NFDebuggerOutputArg::NextNode);
			var->valueType = NFValueType::Node;

			outputArgs.push_back(var);
		}
	}

	// Inherited via NFIVariable
	virtual void PrepareInputData(const NFGUID& runTimeOnwer, const bool iteration) override;
	virtual void UpdateOutputData(const NFGUID& runTimeOnwer, const bool iteration) override;

	// Inherited via NFIVariable
	virtual NF_SHARE_PTR<NFIOData> FindOutputNodeIOData() override;

private:

	NFILogModule* m_pLogModule;
};