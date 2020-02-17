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

class NFPropertyModifier : public NFIModifier
{
private:
	NFPropertyModifier() {}

public:
	NFPropertyModifier(NFIPluginManager* p, const NFGUID& blockID, const NFGUID& id, const std::string& name)
	{
		this->name = name;
		this->id = id;
		this->logicBlockId = blockID;
		this->pPluginManager = p;

		modifierType = NFModifierType::SetProperty;

		Init();
	}

	virtual void InitInputArgs()
	{
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFPropertyModifierInputArg::toString(NFPropertyModifierInputArg::LastNode);
			var->valueType = NFValueType::Node;

			inputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFPropertyModifierInputArg::toString(NFPropertyModifierInputArg::ObjectID);
			var->valueType = NFValueType::Object;
			var->fromType = NFIODataComFromType::EXTERNAL;

			inputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFPropertyModifierInputArg::toString(NFPropertyModifierInputArg::ClassName);
			var->valueType = NFValueType::String;
			var->fromType = NFIODataComFromType::INTERNAL;

			inputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFPropertyModifierInputArg::toString(NFPropertyModifierInputArg::PropName);
			var->valueType = NFValueType::String;
			var->fromType = NFIODataComFromType::INTERNAL;

			inputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFPropertyModifierInputArg::toString(NFPropertyModifierInputArg::PropValue);
			var->valueType = NFValueType::UNKNOW;
			var->fromType = NFIODataComFromType::EXTERNAL;

			inputArgs.push_back(var);
		}
	}

	virtual void InitOutputArgs()
	{
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFPropertyModifierOutputArg::toString(NFPropertyModifierOutputArg::NextNode);
			var->valueType = NFValueType::Node;

			outputArgs.push_back(var);
		}
	}

	virtual void UpdateOutputData()
	{
		NF_SHARE_PTR<NFIOData> inputClassName = GetInputArg(NFPropertyModifierInputArg::ClassName);
		NF_SHARE_PTR<NFIOData> inputPropName = GetInputArg(NFPropertyModifierInputArg::PropName);
		NF_SHARE_PTR<NFIOData> inputPropValue = GetInputArg(NFPropertyModifierInputArg::PropValue);

		std::string className = inputClassName->varData.GetString();
		std::string propName = inputPropName->varData.GetString();

		NFIClassModule* classModule = this->pPluginManager->FindModule<NFIClassModule>();
		auto classObject = classModule->GetElement(className);
		auto propObject = classObject->GetPropertyManager()->GetElement(propName);

		switch (propObject->GetType())
		{
		case NFDATA_TYPE::TDATA_INT:
			inputPropValue->valueType = NFValueType::Int;
			break;
		case NFDATA_TYPE::TDATA_FLOAT:
			inputPropValue->valueType = NFValueType::Float;
			break;
		case NFDATA_TYPE::TDATA_STRING:
			inputPropValue->valueType = NFValueType::String;
			break;
		case NFDATA_TYPE::TDATA_OBJECT:
			inputPropValue->valueType = NFValueType::Object;
			break;
		case NFDATA_TYPE::TDATA_VECTOR2:
			inputPropValue->valueType = NFValueType::Vector2;
			break;
		case NFDATA_TYPE::TDATA_VECTOR3:
			inputPropValue->valueType = NFValueType::Vector3;
			break;
		default:
			break;
		}
	}

	// Inherited via NFIModifier
	virtual void PrepareInputData() override;

	// Inherited via NFIModifier
	virtual NF_SHARE_PTR<NFBluePrintNodeBase> FindNextNode() override;
};

class NFRecordModifier : public NFIModifier
{
private:
	NFRecordModifier() {}

public:
	NFRecordModifier(NFIPluginManager* p, const NFGUID& blockID, const NFGUID& id, const std::string& name)
	{
		this->name = name;
		this->id = id;
		this->logicBlockId = blockID;
		this->pPluginManager = p;

		modifierType = NFModifierType::SetRecord;

		Init();
	}

	virtual void InitInputArgs()
	{
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFRecordModifierInputArg::toString(NFRecordModifierInputArg::LastNode);
			var->valueType = NFValueType::Node;

			inputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFRecordModifierInputArg::toString(NFRecordModifierInputArg::ObjectID);
			var->valueType = NFValueType::Object;
			var->fromType = NFIODataComFromType::EXTERNAL;

			inputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFRecordModifierInputArg::toString(NFRecordModifierInputArg::RecordName);
			var->valueType = NFValueType::String;
			var->fromType = NFIODataComFromType::INTERNAL;

			inputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFRecordModifierInputArg::toString(NFRecordModifierInputArg::RecordRow);
			var->valueType = NFValueType::Int;
			var->fromType = NFIODataComFromType::BOTH;

			inputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFRecordModifierInputArg::toString(NFRecordModifierInputArg::RecordCol);
			var->valueType = NFValueType::Int;
			var->fromType = NFIODataComFromType::INTERNAL;

			inputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFRecordModifierInputArg::toString(NFRecordModifierInputArg::RecordValue);
			var->valueType = NFValueType::UNKNOW;
			var->fromType = NFIODataComFromType::EXTERNAL;

			inputArgs.push_back(var);
		}
	}

	virtual void InitOutputArgs()
	{
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFRecordModifierOutputArg::toString(NFRecordModifierOutputArg::NextNode);
			var->valueType = NFValueType::Node;

			outputArgs.push_back(var);
		}
	}

	// Inherited via NFIModifier
	virtual void PrepareInputData() override;
	virtual void UpdateOutputData() override;

	// Inherited via NFIModifier
	virtual NF_SHARE_PTR<NFBluePrintNodeBase> FindNextNode() override;
};


class NFRecordAdder : public NFIModifier
{
private:
	NFRecordAdder() {}

public:
	NFRecordAdder(NFIPluginManager* p, const NFGUID& blockID, const NFGUID& id, const std::string& name)
	{
		this->name = name;
		this->id = id;
		this->logicBlockId = blockID;
		this->pPluginManager = p;

		modifierType = NFModifierType::AddRecordRow;

		Init();
	}

	virtual NF_SHARE_PTR<NFBluePrintNodeBase> FindNode(const NFGUID& id)
	{

		return nullptr;
	}

	virtual void InitInputArgs()
	{
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFRecordRemoverrInputArg::toString(NFRecordRemoverrInputArg::LastNode);
			var->valueType = NFValueType::Node;

			inputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFRecordRemoverrInputArg::toString(NFRecordRemoverrInputArg::ObjectID);
			var->valueType = NFValueType::Object;
			var->fromType = NFIODataComFromType::EXTERNAL;

			inputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFRecordRemoverrInputArg::toString(NFRecordRemoverrInputArg::RecordName);
			var->valueType = NFValueType::String;
			var->fromType = NFIODataComFromType::INTERNAL;

			inputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFRecordRemoverrInputArg::toString(NFRecordRemoverrInputArg::RecordRow);
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
			var->name = NFRecordRemoverOutputArg::toString(NFRecordRemoverOutputArg::NextNode);
			var->valueType = NFValueType::Node;

			outputArgs.push_back(var);
		}
	}

	// Inherited via NFIModifier
	virtual void PrepareInputData() override;
	virtual void UpdateOutputData() override;
};

class NFRecordRemover : public NFIModifier
{
private:
	NFRecordRemover() {}

public:
	NFRecordRemover(NFIPluginManager* p, const NFGUID& blockID, const NFGUID& id, const std::string& name)
	{
		this->name = name;
		this->id = id;
		this->logicBlockId = blockID;
		this->pPluginManager = p;

		modifierType = NFModifierType::SetRecord;

		Init();
	}

	virtual void InitInputArgs()
	{
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFRecordRemoverrInputArg::toString(NFRecordRemoverrInputArg::LastNode);
			var->valueType = NFValueType::Node;

			inputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFRecordRemoverrInputArg::toString(NFRecordRemoverrInputArg::ObjectID);
			var->valueType = NFValueType::Object;
			var->fromType = NFIODataComFromType::EXTERNAL;

			inputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFRecordRemoverrInputArg::toString(NFRecordRemoverrInputArg::RecordName);
			var->valueType = NFValueType::String;
			var->fromType = NFIODataComFromType::INTERNAL;

			inputArgs.push_back(var);
		}
		{
			NF_SHARE_PTR<NFIOData> var = NF_SHARE_PTR<NFIOData>(NF_NEW NFIOData());
			var->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
			var->name = NFRecordRemoverrInputArg::toString(NFRecordRemoverrInputArg::RecordRow);
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
			var->name = NFRecordRemoverOutputArg::toString(NFRecordRemoverOutputArg::NextNode);
			var->valueType = NFValueType::Node;

			outputArgs.push_back(var);
		}
	}

	// Inherited via NFIModifier
	virtual void PrepareInputData() override;
	virtual void UpdateOutputData() override;

	// Inherited via NFIModifier
	virtual NF_SHARE_PTR<NFBluePrintNodeBase> FindNextNode() override;
};