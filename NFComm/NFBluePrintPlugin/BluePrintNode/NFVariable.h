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


class NFInputVariable : public NFIVariable
{
private:
	NFInputVariable() {}
public:
	NFInputVariable(NFIPluginManager* p, const NFGUID& blockID, const NFGUID& id, const std::string& name)
	{
		this->id = id;
		this->name = name;
		this->pPluginManager = p;
		this->logicBlockId = blockID;

		variableType = NFVariableType::Input;

		Init();
	}


	virtual void InitInputArgs()
	{
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = "";
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
			var->name = "";
			var->valueType = NFValueType::UNKNOW;

			outputArgs.push_back(var);
		}
	}

	// Inherited via NFIVariable
	virtual void PrepareInputData(const NFGUID& runTimeOnwer, const bool iteration) override;
	virtual void UpdateOutputData(const NFGUID& runTimeOnwer, const bool iteration) override;

	// Inherited via NFIVariable
	virtual NF_SHARE_PTR<NFIOData> FindOutputNodeIOData() override;
};

//for Element Data <class anme && element id && property name>
class NFElementVariable : public NFIVariable
{
private:
	NFElementVariable() {}
public:
	NFElementVariable(NFIPluginManager* p, const NFGUID& blockID, const NFGUID& id, const std::string& name)
	{
		this->id = id;
		this->name = name;
		this->pPluginManager = p;
		this->logicBlockId = blockID;

		variableType = NFVariableType::ElementSystem;

		Init();

	}


	virtual void InitInputArgs()
	{
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFElementVariableInputArg::toString(NFElementVariableInputArg::ClassName);
			var->valueType = NFValueType::String;
			var->fromType = NFIODataComFromType::INTERNAL;

			inputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFElementVariableInputArg::toString(NFElementVariableInputArg::ConfigID);
			var->valueType = NFValueType::String;
			var->fromType = NFIODataComFromType::INTERNAL;

			inputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFElementVariableInputArg::toString(NFElementVariableInputArg::PropName);
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
			var->name = NFElementVariableInputArg::toString(NFElementVariableOutputArg::ClassName);
			var->valueType = NFValueType::String;

			outputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFElementVariableInputArg::toString(NFElementVariableOutputArg::ConfigID);
			var->valueType = NFValueType::String;

			outputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFElementVariableInputArg::toString(NFElementVariableOutputArg::PropName);
			var->valueType = NFValueType::String;

			outputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFElementVariableInputArg::toString(NFElementVariableOutputArg::PropValue);
			var->valueType = NFValueType::UNKNOW;

			outputArgs.push_back(var);
		}
	}

	// Inherited via NFIVariable
	virtual void PrepareInputData(const NFGUID& runTimeOnwer, const bool iteration) override;
	virtual void UpdateOutputData(const NFGUID& runTimeOnwer, const bool iteration) override;

	// Inherited via NFIVariable
	virtual NF_SHARE_PTR<NFIOData> FindOutputNodeIOData() override;
};

//for Property Data <self id && property name>
class NFPropertyVariable : public NFIVariable
{
public:
	NFPropertyVariable(NFIPluginManager* p, const NFGUID& blockID, const NFGUID& id, const std::string& name)
	{
		this->id = id;
		this->name = name;
		this->pPluginManager = p;
		this->logicBlockId = blockID;

		variableType = NFVariableType::PropertySystem;

		Init();

	}


	virtual void InitInputArgs()
	{
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFPropertyVariableInputArg::toString(NFPropertyVariableInputArg::ObjectID);
			var->valueType = NFValueType::Object;
			var->fromType = NFIODataComFromType::EXTERNAL;

			inputArgs.push_back(var);
		} 
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFPropertyVariableInputArg::toString(NFPropertyVariableInputArg::ClassName);
			var->valueType = NFValueType::String;
			var->fromType = NFIODataComFromType::INTERNAL;

			inputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFPropertyVariableInputArg::toString(NFPropertyVariableInputArg::PropName);
			var->valueType = NFValueType::String;
			var->fromType = NFIODataComFromType::BOTH;

			inputArgs.push_back(var);
		}
	}
	virtual void InitOutputArgs()
	{
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFPropertyVariableOutputArg::toString(NFPropertyVariableOutputArg::ClassName);
			var->valueType = NFValueType::String;

			outputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFPropertyVariableOutputArg::toString(NFPropertyVariableOutputArg::PropName);
			var->valueType = NFValueType::String;

			outputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFPropertyVariableOutputArg::toString(NFPropertyVariableOutputArg::PropValue);
			var->valueType = NFValueType::UNKNOW;

			outputArgs.push_back(var);
		}
	}


	// Inherited via NFIVariable
	virtual void PrepareInputData(const NFGUID& runTimeOnwer, const bool iteration) override;

	virtual void UpdateOutputData(const NFGUID& runTimeOnwer, const bool iteration) override;


	// Inherited via NFIVariable
	virtual NF_SHARE_PTR<NFIOData> FindOutputNodeIOData() override;

};

class NFPropertyListVariable : public NFIVariable
{
public:
	NFPropertyListVariable(NFIPluginManager* p, const NFGUID& blockID, const NFGUID& id, const std::string& name)
	{
		this->id = id;
		this->name = name;
		this->pPluginManager = p;
		this->logicBlockId = blockID;

		variableType = NFVariableType::PropertyList;

		Init();

	}

	virtual void InitInputArgs()
	{
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFPropertyListVariableInputArg::toString(NFPropertyListVariableInputArg::ObjectID);
			var->valueType = NFValueType::Object;
			var->fromType = NFIODataComFromType::EXTERNAL;

			inputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFPropertyListVariableInputArg::toString(NFPropertyListVariableInputArg::ClassName);
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
			var->name = NFPropertyListVariableOutputArg::toString(NFPropertyListVariableOutputArg::ObjectID);
			var->valueType = NFValueType::Object;

			outputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFPropertyListVariableOutputArg::toString(NFPropertyListVariableOutputArg::ClassName);
			var->valueType = NFValueType::String;

			outputArgs.push_back(var);
		}

		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFPropertyListVariableOutputArg::toString(NFPropertyListVariableOutputArg::ConfigID);
			var->valueType = NFValueType::String;

			outputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFPropertyListVariableOutputArg::toString(NFPropertyListVariableOutputArg::SceneID);
			var->valueType = NFValueType::Int;

			outputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFPropertyListVariableOutputArg::toString(NFPropertyListVariableOutputArg::GroupID);
			var->valueType = NFValueType::Int;

			outputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFPropertyListVariableOutputArg::toString(NFPropertyListVariableOutputArg::Position);
			var->valueType = NFValueType::Vector3;

			outputArgs.push_back(var);
		}
	}


	// Inherited via NFIVariable
	virtual void PrepareInputData(const NFGUID& runTimeOnwer, const bool iteration) override;

	virtual void UpdateOutputData(const NFGUID& runTimeOnwer, const bool iteration) override;


	// Inherited via NFIVariable
	virtual NF_SHARE_PTR<NFIOData> FindOutputNodeIOData() override;

};
//for Record Data <self id && record name && row && col>
class NFRecordVariable : public NFIVariable
{
public:
	NFRecordVariable(NFIPluginManager* p, const NFGUID& blockID, const NFGUID& id, const std::string& name)
	{
		this->id = id;
		this->name = name;
		this->pPluginManager = p;
		this->logicBlockId = blockID;

		variableType = NFVariableType::RecordSystem;

		Init();
	}


	virtual void InitInputArgs()
	{
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFPropertyVariableInputArg::toString(NFPropertyVariableInputArg::ObjectID);
			var->valueType = NFValueType::Object;
			var->fromType = NFIODataComFromType::EXTERNAL;

			inputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFRecordVariableInputArg::ClassName;
			var->valueType = NFValueType::String;
			var->fromType = NFIODataComFromType::INTERNAL;

			inputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFRecordVariableInputArg::RecordName;
			var->valueType = NFValueType::String;
			var->fromType = NFIODataComFromType::INTERNAL;

			inputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFRecordVariableInputArg::RecordRow;
			var->valueType = NFValueType::Int;
			var->fromType = NFIODataComFromType::INTERNAL;

			inputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFRecordVariableInputArg::RecordCol;
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
			var->name = "";
			var->valueType = NFValueType::UNKNOW;

			outputArgs.push_back(var);
		}
	}

	// Inherited via NFIVariable
	virtual void PrepareInputData(const NFGUID& runTimeOnwer, const bool iteration) override;


	// Inherited via NFIVariable
	virtual void UpdateOutputData(const NFGUID& runTimeOnwer, const bool iteration) override;


	// Inherited via NFIVariable
	virtual NF_SHARE_PTR<NFIOData> FindOutputNodeIOData() override;

};

class NFArrayVariable : public NFIVariable
{
public:
	NFArrayVariable(NFIPluginManager* p, const NFGUID& blockID, const NFGUID& id, const std::string& name)
	{
		this->id = id;
		this->name = name;
		this->pPluginManager = p;
		this->logicBlockId = blockID;

		variableType = NFVariableType::Array;

		Init();
	}


	virtual void InitInputArgs()
	{
	
	}

	virtual void InitOutputArgs()
	{
	
	}


	// Inherited via NFIVariable
	virtual void PrepareInputData(const NFGUID& runTimeOnwer, const bool iteration) override;

	virtual void UpdateOutputData(const NFGUID& runTimeOnwer, const bool iteration) override;


	// Inherited via NFIVariable
	virtual NF_SHARE_PTR<NFIOData> FindOutputNodeIOData() override;

};

class NFDictionaryVariable : public NFIVariable
{
public:
	NFDictionaryVariable(NFIPluginManager* p, const NFGUID& blockID, const NFGUID& id, const std::string& name)
	{
		this->id = id;
		this->name = name;
		this->pPluginManager = p;
		this->logicBlockId = blockID;

		variableType = NFVariableType::Dictionary;

		Init();
	}


	virtual void InitInputArgs()
	{

	}

	virtual void InitOutputArgs()
	{

	}


	// Inherited via NFIVariable
	virtual void PrepareInputData(const NFGUID& runTimeOnwer, const bool iteration) override;

	virtual void UpdateOutputData(const NFGUID& runTimeOnwer, const bool iteration) override;


	// Inherited via NFIVariable
	virtual NF_SHARE_PTR<NFIOData> FindOutputNodeIOData() override;

};
