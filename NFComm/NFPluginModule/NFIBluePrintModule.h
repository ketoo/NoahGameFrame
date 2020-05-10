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
#ifndef NFI_BLUE_PRINT_MODULE_H
#define NFI_BLUE_PRINT_MODULE_H

#include <iostream>
#include <map>
#include <list>
#include "NFIModule.h"
#include "NFIKernelModule.h"
#include "NFIClassModule.h"
#include "NFIElementModule.h"
#include "NFILogModule.h"
#include "NFIEventModule.h"

class NFBranchType;
class NFMonitorType;
class NFVariableType;
class NFExecutorType;
class NFModifierType;
class NFArithmeticType;

class NFBluePrintNodeBase;
class NFLogicBlock;
class NFIBranch;
class NFIMonitor;
class NFIVariable;
class NFIExecutor;
class NFIModifier;
class NFIArithmetic;
class NFIDebugger;

class NFDataLink;

class NFIBluePrintModule
	: public NFIModule
{
public:

	////////////////////////////////////////
	virtual void SetNodeModifyEventFunctor(std::function<void(const NFGUID&, const bool)> functor) = 0;
	virtual void SetLinkModifyEventFunctor(std::function<void(const NFGUID&, const bool)> functor) = 0;

	virtual NF_SHARE_PTR<NFLogicBlock> CreateLogicBlock(const NFGUID& logicBlockId, const std::string& name) = 0;
	virtual const std::list<NF_SHARE_PTR<NFLogicBlock>>& GetLogicBlocks() = 0;
	virtual NF_SHARE_PTR<NFLogicBlock>  GetLogicBlock(const NFGUID& logicBlockId) = 0;

	virtual NF_SHARE_PTR<NFBluePrintNodeBase>  FindNode(const NFGUID& id) = 0;

	virtual NF_SHARE_PTR<NFIBranch> AddBranch(const NFGUID& logicBlockId, const NFBranchType type, const NFGUID& id, const std::string& name) = 0;
	virtual NF_SHARE_PTR<NFIMonitor> AddMonitor(const NFGUID& logicBlockId, const NFMonitorType type, const NFGUID& id, const std::string& name) = 0;
	virtual NF_SHARE_PTR<NFIVariable> AddVariable(const NFGUID& logicBlockId, const NFVariableType type, const NFGUID& id, const std::string& name) = 0;
	virtual NF_SHARE_PTR<NFIExecutor> AddExecutor(const NFGUID& logicBlockId, const NFExecutorType TYPE, const NFGUID& id, const std::string& name) = 0;
	virtual NF_SHARE_PTR<NFIModifier> AddModifier(const NFGUID& logicBlockId, const NFModifierType type, const NFGUID& id, const std::string& name) = 0;
	virtual NF_SHARE_PTR<NFIArithmetic> AddArithmetic(const NFGUID& logicBlockId, const NFArithmeticType type, const NFGUID& id, const std::string& name) = 0;
	virtual NF_SHARE_PTR<NFIDebugger> AddDebugger(const NFGUID& logicBlockId, const NFGUID& id, const std::string& name) = 0;

	virtual void AddLink(const NFGUID& logicBlockId, const NFGUID& id, const NFGUID& startNode, const NFGUID& endNode, const NFGUID& startPin, const NFGUID& endPin) = 0;
	virtual NF_SHARE_PTR<NFDataLink> GetLink(const NFGUID& logicBlockId, const NFGUID& startNode, const NFGUID& endNode, const NFGUID& startPin, const NFGUID& endPin) = 0;
	virtual NF_SHARE_PTR<NFDataLink> GetLink(const NFGUID& logicBlockId, const NFGUID& id) = 0;
	virtual NF_SHARE_PTR<NFDataLink> GetLink(const NFGUID& id) = 0;
	virtual bool DeleteLink(const NFGUID& logicBlockId, const NFGUID& startNode, const NFGUID& endNode, const NFGUID& startPin, const NFGUID& endPin) = 0;
	virtual bool DeleteLink(const NFGUID& logicBlockId, const NFGUID& id) = 0;
	virtual bool DeleteLink(const NFGUID& id) = 0;
	virtual bool CheckLink(const NFGUID& id) = 0;
	virtual std::list<NF_SHARE_PTR<NFDataLink>> GetLinks(const NFGUID& logicBlockId) = 0;

	virtual bool DeleteMonitor(const NFGUID& id) = 0;
	virtual bool DeleteJudgement(const NFGUID& id) = 0;
	virtual bool DeleteExecutor(const NFGUID& id) = 0;
};

NF_SMART_ENUM(NFBlueprintType,
	LOGICBLOCK,
	MONITOR,
	BRANCH,
	EXECUTER,
	VARIABLE,
	MODIFIER,
	ARITHMETIC,
	DEBUGER,
	CUSTOM,
)

//--------FOR VARIABLE----------------
NF_SMART_ENUM(NFElementVariableInputArg,
	ClassName,
	ConfigID,
	PropName,
	)

	NF_SMART_ENUM(NFElementVariableOutputArg,
		ClassName,
		ConfigID,
		PropName,
		PropValue,
		)

	NF_SMART_ENUM(NFPropertyVariableInputArg,
		ObjectID,
		ClassName,
		PropName,
		)

	NF_SMART_ENUM(NFPropertyVariableOutputArg,
		ObjectID,
		ClassName,
		ConfigID,
		SceneID,
		GroupID,
		Position,
		PropValue,
		)

	NF_SMART_ENUM(NFRecordVariableInputArg,
		ObjectID,
		ClassName,
		RecordName,
		RecordRow,
		RecordCol,
		)

//------------------------
//-------FOR MONITOR-----------------

NF_SMART_ENUM(NFMonitorType,
	GameEvent,
	NetworkEvent,
	NetworkMsgEvent,
	ObjectEvent,
	PropertyEvent,
	RecordEvent,
	SceneEvent,
	)

NF_SMART_ENUM(NFClassEventType,
	OBJECT_NO_DATA,
	OBJECT_HAS_DATA,
	OBJECT_FINISHED,
	BEFORE_DESTROY,
	DESTROY,
	)

	//------------------------------
	NF_SMART_ENUM(NFGameEventMonitorInputArg,
		EventID,
		Dictionary,
		)

	NF_SMART_ENUM(NFGameEventMonitorOutputArg,
		NextNode,
		EventID,
		ObjectID,
		Dictionary,
		)
	//------------------------------
	NF_SMART_ENUM(NFNetworkEventMonitorInputArg,
		EventID,
		)

	NF_SMART_ENUM(NFNetworkEventMonitorOutputArg,
		NextNode,
		EventID,
		ObjectID,
		)
	//------------------------------
	NF_SMART_ENUM(NFNetworkMsgMonitorInputArg,
		NetMsgID,
		)

	NF_SMART_ENUM(NFNetworkkMsgMonitorOutputArg,
		NextNode,
		ObjectID,
		NetMsgID,
		Dictionary,
		)
	//message data will includes a hashmap, developers could add more args to get the data by key
	//------------------------------
	NF_SMART_ENUM(NFMonitorObjectEventInputArg,
		ClassName,
		ClassEvent,
		)

	NF_SMART_ENUM(NFMonitorObjectEventOutputArg,
		NextNode,
		ObjectID,
		ClassName,
		ConfigID,
		SceneID,
		GroupID,
		Position,
		)

	//------------------------------
	NF_SMART_ENUM(NFMonitorPropertyEventInputArg,
		ClassName,
		PropName,
		)

	NF_SMART_ENUM(NFMonitorPropertyEventOutputArg,
		NextNode,
		ObjectID,
		PropName,
		)

	//------------------------------
	NF_SMART_ENUM(NFMonitorRecordEventInputArg,
		ClassName,
		RecordName,
		OperateType,
		)

	NF_SMART_ENUM(NFMonitorRecordEventOutArg,
		NextNode,
		ClassName,
		RecordName,
		Row,
		Col,
		)

	//------------------------------
	NF_SMART_ENUM(NFMonitorSceneEventInputArg,
		ClassName,
		SceneID,
		OperateType,
		)

	NF_SMART_ENUM(NFMonitorSceneEventOutArg,
		NextNode,
		ClassName,
		SceneID,
		GroupID,
		ObjectID,
		)

	//------------------------------
//------------------------
//----------FOR VARIABLE--------------
NF_SMART_ENUM(NFValueType,
	UNKNOW,
	Node,
	Int,
	Float,
	String,
	Vector2,
	Vector3,
	Object,
	Array,
	Dictionary,
	)

NF_SMART_ENUM(NFVariableType,
	BasicVariable,
	ElementSystem,
	PropertySystem,
	PropertyList,
	RecordSystem,
	Array,
	Dictionary,
	)

//----------FOR DEBUGER--------------

NF_SMART_ENUM(NFDebuggerLevel,
	Debug_Level,
	Info_Level,
	Waring_Level,
	Error_Level,
	)

NF_SMART_ENUM(NFDebuggerInputArg,
	PreNode,
	LogLevel,
	LogObject,
	LogString,
	LogInt,
	LogFloat,
	LogVector2,
	LogVector3,
	LogDictionary,
	LogData,
)

NF_SMART_ENUM(NFDebuggerOutputArg,
	NextNode,
)
///////////FOR MODIFIER BEGIN///////////////////////////
//SetProperty 3 args: NFGUID objectID, string propertyName, int value
//SetRecord 5 args: NFGUID objectID, string recordName, int row, int col, int value
NF_SMART_ENUM(NFModifierType,
	SetProperty,
	SetRecord,
	AddRecordRow,
	RemRecordRow,
)

NF_SMART_ENUM(NFPropertyModifierInputArg,
	PreNode,
	ObjectID,
	ClassName,
	PropName,
	PropValue
	)

NF_SMART_ENUM(NFPropertyModifierOutputArg,
		NextNode,
		)

NF_SMART_ENUM(NFRecordModifierInputArg,
	PreNode,
	ObjectID,
	RecordName,
	RecordRow,
	RecordCol,
	RecordValue
	)
NF_SMART_ENUM(NFRecordModifierOutputArg,
	NextNode,
	)

NF_SMART_ENUM(NFRecordRemoverrInputArg,
	PreNode,
	ObjectID,
	RecordName,
	RecordRow,
)

NF_SMART_ENUM(NFRecordRemoverOutputArg,
	NextNode,
	)


///////////FOR EXECUTER BEGIN///////////////////////////
NF_SMART_ENUM(NFExecutorType,
	CreateObject,
	DestroyObject,
	GameEvent,
	MoveObject,
	Sleep,
	SendMessage,
	EnterScene,
	AddHeartBeat,
	AttackObject,
)

NF_SMART_ENUM(NFExecutorCreateObjectInputArg,
	PreNode,
	ClassName,
	ConfigID,
	SceneID,
	GroupID,
	Position,
)
NF_SMART_ENUM(NFExecutorCreateObjectOutputputArg,
	NextNode,
	ObjectID,
	)

NF_SMART_ENUM(NFExecutorDestroyObjectInputArg,
	PreNode,
	DelayTime,
	ObjectID,
	)

NF_SMART_ENUM(NFExecutorDestroyObjectOutputputArg,
	NextNode,
	)

NF_SMART_ENUM(NFExecutorGameEventInputArg,
	PreNode,
	ObjectID,
	EventID,
	Dctionary,
	)

NF_SMART_ENUM(NFExecutorGameEventOutputArg,
	NextNode,
	)

NF_SMART_ENUM(NFExecutorSendMessageInputArg,
	PreNode,
	ObjectID,
	MessageID,
	Data,
	)

NF_SMART_ENUM(NFExecutorSendMessageOutputArg,
	NextNode,
	)


NF_SMART_ENUM(NFExecutorMoveObjectInputArg,
	PreNode,
	ObjectID,
	Position,
	MoveType,
	)

NF_SMART_ENUM(NFExecutorMoveObjectOutputArg,
	NextNode,
	)

NF_SMART_ENUM(NFExecutorSleepInputArg,
	PreNode,
	ObjectID,
	SleepTime,
	)

NF_SMART_ENUM(NFExecutorSleepOutputArg,
	NextNode,
	ObjectID,
	)

NF_SMART_ENUM(NFExecutorEnterSceneInputArg,
	PreNode,
	ObjectID,
	SceneID,
	GroupID,
	Position,
	)

NF_SMART_ENUM(NFExecutorEnterSceneOutputArg,
	NextNode,
	)

NF_SMART_ENUM(NFExecutorAddHeartBeatInputArg,
	PreNode,
	ObjectID,
	BeatName,
	BeatTime,
	BeatCount,
	)

NF_SMART_ENUM(NFExecutorAddHeartBeatOutputArg,
	NextNode,
	)

///////////FOR BRANCH BEGIN///////////////////////////
NF_SMART_ENUM(NFBranchType,
	IntBranch,
	StringBranch,
	FloatBranch,
	ObjectBranch,
	Vector2Branch,
	Vector3Branch,
)

NF_SMART_ENUM(NFComparatorType,
	Equal,
	EnEqual,
	MoreThen,
	LessThan,
	)

NF_SMART_ENUM(NFBranchInputArg,
	PreNode,
	LeftInput,
	Comparator,
	RightInput
)

NF_SMART_ENUM(NFBranchOutputArg,
	TrueOut,
	FalseOut,
)

///////////FOR ARITHMETIC BEGIN///////////////////////////

NF_SMART_ENUM(NFArithmeticType,
	Add,
	Sub,
	Multiply,
	Division,
	)

NF_SMART_ENUM(NFArithmeticInputArg,
	PreNode,
	ValueType,
	LeftInput,
	ArithmeticType,
	RightInput,
)

NF_SMART_ENUM(NFArithmeticOutputArg,
	NextNode,
	Output,
)
///////////////////////////////////////////////////////////////////////////////////
class NFLogicBlock;
class NFIMonitor;
class NFIBranch;
class NFIExecutor;
class NFIVariable;
class NFIOData;
class NFDataLink;

//BOTH MEANS: INTERNAL AND EXTERNAL
//PARAMETER
NF_SMART_ENUM(NFIODataComFromType,
	BOTH,
	INTERNAL,
	EXTERNAL
)

class NFIOData
{
public:
	NFGUID id;
	std::string name;//arg name

	std::map<std::string, NFData> dictionaryData;
	NFIODataComFromType fromType = NFIODataComFromType::EXTERNAL;

	NFGUID GetLinkID()
	{
		return linkID;
	}

	void SetLinkID(NFGUID id)
	{
		linkID = id;
	}

	void SetValueType(NFValueType type)
	{
		if (valueType != type)
		{
			valueType = type;
			varData.Reset();

			switch (valueType)
			{
				case NFValueType::Int:
					varData.SetInt(0);
					break;
				case NFValueType::Float:
					varData.SetFloat(0.0f);
					break;
				case NFValueType::String:
					varData.SetString("");
					break;
				case NFValueType::Object:
					varData.SetObject(NFGUID());
					break;
				case NFValueType::Vector2:
					varData.SetVector2(NFVector2::Zero());
					break;
				case NFValueType::Vector3:
					varData.SetVector3(NFVector3::Zero());
					break;
				default:
					break;
			}
		}
	}

	NFValueType GetValueType()
	{
		return valueType;
	}

	void SetData(const NFData& var)
	{
		varData = var;
	}

	const NFData& GetData()
	{
		return varData;
	}

	void FromString(const std::string& var)
	{
		varData.FromString(var);
	}

	void SetInt(const NFINT64 var)
	{
		SetValueType(NFValueType::Int);
		varData.SetInt(var);
	}

	void SetFloat(const double var)
	{
		SetValueType(NFValueType::Float);
		varData.SetFloat(var);
	}

	void SetString(const std::string& var)
	{
		SetValueType(NFValueType::String);
		varData.SetString(var);
	}

	void SetObject(const NFGUID var)
	{
		SetValueType(NFValueType::Object);
		varData.SetObject(var);
	}

	void SetVector2(const NFVector2 var)
	{
		SetValueType(NFValueType::Vector2);
		varData.SetVector2(var);
	}

	void SetVector3(const NFVector3 var)
	{
		SetValueType(NFValueType::Vector3);
		varData.SetVector3(var);
	}

	NFINT64 GetInt() const
	{
		return varData.GetInt();
	}

	int GetInt32() const
	{
		return varData.GetInt32();
	}

	double GetFloat() const
	{
		return varData.GetFloat();
	}

	const std::string& GetString() const
	{
		return varData.GetString();
	}


	const NFGUID& GetObject() const
	{
		return varData.GetObject();
	}

	const NFVector2& GetVector2() const
	{
		return varData.GetVector2();
	}

	const NFVector3& GetVector3() const
	{
		return varData.GetVector3();
	}

	std::string ToString() const
	{
		return varData.ToString();
	}

private:
	NFData varData;
	NFValueType valueType = NFValueType::UNKNOW;
	NFGUID linkID;
};

class NFDataLink
{
private:
	NFDataLink() {}

public:
	NFDataLink(NFGUID selfID, NFGUID startNodeID, NFGUID endNodeID, NFGUID startAttr, NFGUID endAttr, const int index = -1, const int color = -1)
	{
		this->selfID = selfID;
		this->index = index;
		this->color = color;

		this->startNode = startNodeID;
		this->endNode = endNodeID;

		this->startAttr = startAttr;
		this->endAttr = endAttr;
	}

	int index;
	int color;
	NFGUID selfID;
	NFGUID startAttr;
	NFGUID endAttr;
	NFGUID startNode;
	NFGUID endNode;
};

class NFBluePrintNodeBase
{
protected:
	NFIPluginManager* pPluginManager;

	NFIBluePrintModule* pBluePrintModule;
	NFIElementModule* pElementModule;
	NFIClassModule* pClassModule;
	NFIKernelModule* m_pKernelModule;
	NFILogModule* m_pLogModule;
	NFIEventModule* m_pEventModule;

public:
	NFBluePrintNodeBase()
	{
	}

	virtual ~NFBluePrintNodeBase(){}

	void Init()
	{
		pBluePrintModule = pPluginManager->FindModule<NFIBluePrintModule>();
		pElementModule = this->pPluginManager->FindModule<NFIElementModule>();
		pClassModule = this->pPluginManager->FindModule<NFIClassModule>();
		m_pKernelModule = this->pPluginManager->FindModule<NFIKernelModule>();
		m_pLogModule = this->pPluginManager->FindModule<NFILogModule>();
		m_pEventModule = this->pPluginManager->FindModule<NFIEventModule>();

		InitInputArgs();
		InitOutputArgs();

		//check input args count and out args count
	}

	const int GetInputArgCount()
	{
		return inputArgs.size();
	}

	const int GetOutputArgCount()
	{
		return outputArgs.size();
	}

	NF_SHARE_PTR<NFIOData> GetInputArg(const NFGUID& id)
	{
		for (int i = 0; i < inputArgs.size(); ++i)
		{
			auto input = inputArgs[i];
			if (input->id == id)
			{
				return input;
			}
		}

		return nullptr;
	}

	NF_SHARE_PTR<NFIOData> GetOutputArg(const NFGUID& id)
	{
		for (int i = 0; i < outputArgs.size(); ++i)
		{
			auto output = outputArgs[i];
			if (output->id == id)
			{
				return output;
			}
		}

		return nullptr;
	}

	//start from 0
	NF_SHARE_PTR<NFIOData> GetInputArg(const int index)
	{
		if (inputArgs.size() > index)
		{
			return inputArgs[index];
		}

		return nullptr;
	}

	//start from 0
	NF_SHARE_PTR<NFIOData> GetInputArg(const std::string name)
	{
		for (int i = 0; i < inputArgs.size(); ++i)
		{
			auto input = inputArgs[i];
			if (input->name == name)
			{
				return input;
			}
		}

		return nullptr;
	}

	//start from 0
	NF_SHARE_PTR<NFIOData> GetOutputArg(const int index)
	{
		if (outputArgs.size() > index)
		{
			return outputArgs[index];
		}

		return nullptr;
	}

	//start from 0
	NF_SHARE_PTR<NFIOData> GetOutputArg(const std::string name)
	{
		for (int i = 0; i < outputArgs.size(); ++i)
		{
			auto input = outputArgs[i];
			if (input->name == name)
			{
				return input;
			}
		}

		return nullptr;
	}

	virtual NF_SHARE_PTR<NFIOData> FindOutputNodeIOData() = 0;

	virtual NF_SHARE_PTR<NFBluePrintNodeBase> FindNextNode()
	{
		auto outputIO = FindOutputNodeIOData();
		if (outputIO)
		{
			NFGUID linkID = outputIO->GetLinkID();
			if (!linkID.IsNull())
			{
				auto linkData = this->pBluePrintModule->GetLink(linkID);
				if (linkData)
				{
					return this->pBluePrintModule->FindNode(linkData->endNode);
				}
			}
		}

		return nullptr;
	}

	virtual void InitInputArgs() = 0;
	virtual void InitOutputArgs() = 0;

	void Execute(const NFGUID& runTimeOwner)
	{
		PrepareInputData(runTimeOwner);
		UpdateOutputData(runTimeOwner);

		auto nextNode = FindNextNode();
		if (nextNode)
		{
			nextNode->Execute(runTimeOwner);
		}
	}

	virtual void UpdateOutputData(const NFGUID& runTimeOwner) = 0;

	//should for node to boost the runtime speed
	//maybe parent node not only one node, maybe two or three, collect all nodes and let it PrepareInputData && UpdateOutputData
	void PrepareInputData(const NFGUID& runTimeOwner)
	{
		std::vector<NFGUID> parentNodes;
		for (int i = 0; i < inputArgs.size(); ++i)
		{
			auto inputArg = inputArgs[i];
			if (inputArg && !inputArg->GetLinkID().IsNull() && inputArg->GetValueType() != NFValueType::Node && inputArg->GetValueType() != NFValueType::UNKNOW)
			{
				auto linkData = this->pBluePrintModule->GetLink(inputArg->GetLinkID());
				if (linkData && !linkData->startNode.IsNull())
				{
					bool find = false;
					for (int j = 0; j < parentNodes.size(); ++j)
					{
						if (parentNodes[j] == linkData->startNode)
						{
							find = true;
							break;
						}
					}

					if (!find)
					{
						parentNodes.push_back(linkData->startNode);
					}
				}
			}
		}

		for (int j = 0; j < parentNodes.size(); ++j)
		{
			auto node = this->pBluePrintModule->FindNode(parentNodes[j]);
			if (node)
			{
				node->PrepareInputData(runTimeOwner);
				node->UpdateOutputData(runTimeOwner);
			}
		}

		//get value from parent node's output
		for (int i = 0; i < inputArgs.size(); ++i)
		{
			auto inputArg = inputArgs[i];
			if (inputArg && !inputArg->GetLinkID().IsNull() && inputArg->GetValueType() != NFValueType::Node && inputArg->GetValueType() != NFValueType::UNKNOW)
			{
				auto linkData = this->pBluePrintModule->GetLink(inputArg->GetLinkID());
				if (linkData && !linkData->startAttr.IsNull())
				{
					auto startNode = this->pBluePrintModule->FindNode(linkData->startNode);
					if (startNode)
					{
						auto startAttr = startNode->GetOutputArg(linkData->startAttr);
						if (startAttr && startAttr->GetValueType() == inputArg->GetValueType())
						{
							switch (inputArg->GetValueType())
							{
								case NFValueType::Int:
									inputArg->SetInt(startAttr->GetInt());
									break;
								case NFValueType::Float:
									inputArg->SetFloat(startAttr->GetFloat());
									break;
								case NFValueType::String:
									inputArg->SetString(startAttr->GetString());
									break;
								case NFValueType::Object:
									inputArg->SetObject(startAttr->GetObject());
									break;
								case NFValueType::Vector2:
									inputArg->SetVector2(startAttr->GetVector2());
									break;
								case NFValueType::Vector3:
									inputArg->SetVector3(startAttr->GetVector3());
									break;
								case NFValueType::Array:
								case NFValueType::Dictionary:
									inputArg->dictionaryData = startAttr->dictionaryData;
									break;
								default:
									break;
							}
						}
						else
						{
							std::ostringstream os;
							os << "Start Node ID" << startNode->id.ToString() << " ";
							os << "Start Node Name " << startNode->name << " ";
							os << "End Node ID " << inputArg->id.ToString() << " ";
							os << "End Node Name " << inputArg->name << " ";

							m_pLogModule->LogError(os, __FUNCTION__, __LINE__);
						}
					}
				}
			}
		}
	}

	void Print()
	{
		if (!m_pLogModule)
		{
			return;
		}

		std::ostringstream os;
		os << "Start Print Node:";
		os << " ID" << this->id.ToString();
		os << " Name " << this->name;
		os << std::endl;

		for (int i = 0; i < inputArgs.size(); ++i)
		{
			auto input = inputArgs[i];
			os << " Input Parameter:" << i;
			os << " ID:" << input->id.ToString();
			os << " Name:" << input->name;
			os << " LinkID:" << input->GetLinkID().ToString();
			os << " FromType:" << input->fromType;

			os << " ValueType:" << input->GetValueType().toString();
			os << " Value:" << input->ToString();
			os << std::endl;
		}

		os << std::endl;

		for (int i = 0; i < outputArgs.size(); ++i)
		{
			auto output = outputArgs[i];
			os << " output Parameter " << i;
			os << " ID " << output->id.ToString();
			os << " Name " << output->name;
			os << " LinkID:" << output->GetLinkID().ToString();
			os << " FromType:" << output->fromType;

			os << " ValueType:" << output->GetValueType().toString();
			os << " Value:" << output->ToString();
			os << std::endl;
		}

		m_pLogModule->LogDebug(os, __FUNCTION__, __LINE__);
	}

	bool enable = true;//only for logic block
	bool running = false;//only for logic block
	NFGUID logicBlockId;
	NFGUID id;
	std::string name;
	NFBlueprintType blueprintType = NFBlueprintType::LOGICBLOCK;

	std::vector<NF_SHARE_PTR<NFIOData>> inputArgs;
	std::vector<NF_SHARE_PTR<NFIOData>> outputArgs;
};

class NFIArithmetic : public NFBluePrintNodeBase
{
public:
	NFIArithmetic()
	{
		blueprintType = NFBlueprintType::ARITHMETIC;
	}
};

class NFIBranch : public NFBluePrintNodeBase
{
public:
	NFIBranch()
	{
		blueprintType = NFBlueprintType::BRANCH;
	}

	NF_SHARE_PTR<NFIOData> FindOutputNodeIOData() override
	{
		NF_SHARE_PTR<NFIOData> output = nullptr;
		if (result)
		{
			output = GetOutputArg(NFBranchOutputArg::TrueOut);
		}
		else
		{
			output = GetOutputArg(NFBranchOutputArg::FalseOut);
		}

		return output;
	}

	NFBranchType branchType;
	NFComparatorType comparatorType;
	bool result = false;
};

class NFIVariable : public NFBluePrintNodeBase
{
public:
	NFIVariable()
	{
		blueprintType = NFBlueprintType::VARIABLE;
	}

	NFVariableType variableType;
};

class NFIMonitor : public NFBluePrintNodeBase
{
public:
	NFIMonitor()
	{
		blueprintType = NFBlueprintType::MONITOR;
	}

	NFMonitorType monitorType;

public:
};

class NFIModifier : public NFBluePrintNodeBase
{
public:
	NFIModifier()
	{
		blueprintType = NFBlueprintType::MODIFIER;
	}

	NFModifierType modifierType;
};

class NFIExecutor : public NFBluePrintNodeBase
{
public:
	NFIExecutor()
	{
		blueprintType = NFBlueprintType::EXECUTER;
	}
	NFExecutorType executerType;
};

class NFIDebugger : public NFBluePrintNodeBase
{
public:
	NFIDebugger()
	{
		blueprintType = NFBlueprintType::DEBUGER;
	}
	//NFExecutorType executerType;
};

class NFLogicBlock : public NFBluePrintNodeBase
{
private:
	NFLogicBlock() {}

public:
	NFLogicBlock(NFIPluginManager* p, const NFGUID& id, const std::string& name)
	{
		this->logicBlockId = id;
		this->id = id;
		this->name = name;
		this->pPluginManager = p;
		this->blueprintType = NFBlueprintType::LOGICBLOCK;
	}

	virtual NF_SHARE_PTR<NFBluePrintNodeBase> FindNode(const NFGUID& id);

	virtual void InitInputArgs()
	{

	}

	virtual void InitOutputArgs()
	{

	}


	virtual void UpdateOutputData(const NFGUID& runTimeOwner)
	{

	}

	virtual NF_SHARE_PTR<NFIOData> FindOutputNodeIOData() override
	{
		return nullptr;
	}

	std::list<NF_SHARE_PTR<NFIExecutor>> executers;
	std::list<NF_SHARE_PTR<NFIBranch>> branches;
	std::list<NF_SHARE_PTR<NFIMonitor>> monitors;
	std::list<NF_SHARE_PTR<NFIVariable>> variables;
	std::list<NF_SHARE_PTR<NFIModifier>> modifiers;
	std::list<NF_SHARE_PTR<NFIArithmetic>> arithmetics;
	std::list<NF_SHARE_PTR<NFIDebugger>> debuggers;

	std::list<NF_SHARE_PTR<NFDataLink>> dataLinks;

};

#endif