//
// Created by James on 4/02/21.
//

#ifndef NFIGENERATOR_H
#define NFIGENERATOR_H

#include "NFFileData.h"
#include "NFFileProcessHelp.hpp"

class NFIGenerator
{
public:
	virtual bool Generate(const std::map<std::string, ClassData*>& classData) = 0;

	std::string strExcelIniPath = "../Excel/";
	std::string strXMLStructPath = "../Struct/";
	std::string strXMLIniPath = "../Ini/";

};


#endif //NFIGENERATOR_H
