// -------------------------------------------------------------------------
//    @FileName         :    NFCAddConsistentHash.hpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-03-01
//    @Module           :    NFCAddConsistentHash.hpp
//
// -------------------------------------------------------------------------

#include <map>
#include <string>
#include <list>
#include <functional> 
#include <algorithm>
#include "NFCConsistentHash.hpp"

#ifndef NFC_ADDCONSISTENT_HASH_H
#define NFC_ADDCONSISTENT_HASH_H


class NFCAddConsistentHash : public NFCConsistentHash
{
public:

    NFCAddConsistentHash()
    {
    }

    virtual ~NFCAddConsistentHash()
    {
    }

public:
   //添加备胎
	void AddCandidateMachine(const int nServerID) 
	{
		std::list<NFCMachineNode> xNodeList;
		GetNodeList(xNodeList);

		//把所有的真实主机作为备份
		NFCMachineNode xNode;
		xNode.nMachineID = nServerID;
		xNode.strIP = "";
		xNode.nPort = 0;
		xNode.bCandidate = true;

		for (std::list<NFCMachineNode>::iterator it = xNodeList.begin(); it != xNodeList.end(); ++it)
		{
			NFIVirtualNode&  xRealNode = *it;
			if (!xRealNode.Candidate())
			{
				xNode.xRealMachine.push_back(xRealNode);
			}
		}

		Insert(xNode);
	}
	//实例话一个备胎
	void IntanceCandidateMachine()
	{

	}

};


#endif
