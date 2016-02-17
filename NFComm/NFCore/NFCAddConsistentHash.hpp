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
   //��ӱ�̥
	void AddCandidateMachine(const int nServerID) 
	{
		std::list<NFCMachineNode> xNodeList;
		GetNodeList(xNodeList);

		//�����е���ʵ������Ϊ����
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
	//ʵ����һ����̥
	void IntanceCandidateMachine()
	{

	}

};


#endif
