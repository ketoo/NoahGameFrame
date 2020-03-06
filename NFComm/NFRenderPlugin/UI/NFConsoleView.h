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
#ifndef NF_CONSOLE_VIEW_H
#define NF_CONSOLE_VIEW_H

#include "NFComm/NFPluginModule/NFIUIModule.h"
#include "NFComm/NFPluginModule/NFILogModule.h"
#include "NFComm/NFPluginModule/NFIElementModule.h"
#include "NFComm/NFPluginModule/NFIClassModule.h"

class NFConsoleView : public NFIView
{
public:
	NFConsoleView(NFIPluginManager* p, NFViewType vt);
	virtual bool Execute();

private:
   void Hooker(const NFILogModule::NF_LOG_LEVEL level, const std::string& str);

private: 
   class LogData
   {
   public:
      LogData(const NFILogModule::NF_LOG_LEVEL level, const std::string& str)
      {
         this->level = level;
         this->data = str;
      }

      NFILogModule::NF_LOG_LEVEL level;
      std::string data;
   };

   std::vector<LogData> mLogData;

   bool logDebug = true;
   bool logInfo = true;
   bool logWaring = true;
   bool logError = true;
   bool logFatal = true;

private:
    NFIUIModule* m_pUIModule;
	NFILogModule* m_pLogModule;
    NFIElementModule* m_pElementModule;
    NFIClassModule* m_pClassModule;
};

#endif