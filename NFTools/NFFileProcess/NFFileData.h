//
// Created by James on 4/02/21.
//

#ifndef NFFILEDATA_H
#define NFFILEDATA_H

#include <map>
#include <list>

class NFClassProperty
{
public:
	NFClassProperty()
	{
	}

	std::map<std::string, std::string> descList;//tag, value
	std::string name;
	std::string type;
};

class NFClassRecord
{
public:
	NFClassRecord()
	{
	}

	struct RecordColDesc
	{
		int index;
		std::string type;
		std::string desc;
	};

	std::string className;
	std::map<std::string, std::string> descList;//tag, value
	std::map<std::string, RecordColDesc*> colList;//tag, desc
};

class NFClassStruct
{
public:
	NFClassStruct()
	{
	}
	std::string className;
	std::map<std::string, NFClassProperty*> xPropertyList;//key, desc
	std::map<std::string, NFClassRecord*> xRecordList;//name, desc
};

class NFClassElement
{
public:
	NFClassElement()
	{
	}

	class ElementData
	{
	public:
		std::map<std::string, std::string> xPropertyList;
	};

	std::map<std::string, ElementData*> xElementList;//key, iniList
};

class ClassData
{
public:
	NFClassStruct xStructData;
	NFClassElement xIniData;
	bool beIncluded = false;
	bool beParted = false;
	std::string filePath;
	std::string fileFolder;
	std::list<std::string> includes;
	std::list<std::string> parts;
	std::list<std::string> ref;
	std::list<std::string> parents;
};

#endif //NFFILEDATA_H
