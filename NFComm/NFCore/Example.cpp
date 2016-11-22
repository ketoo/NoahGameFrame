#include "stdafx.h"
#include <stdint.h>
#include <iostream>
#include <string>
#include <boost/functional/hash.hpp>
#include <boost/format.hpp>
#include <boost/crc.hpp>
#include "NFCConsistentHash.hpp"

void InitMainNodes(std::vector<MachineData>& vNodes)
{

    MachineData xData;
    xData.strIP = "192.168.1.100";
    xData.nWeight = 1000;
    xData.nPort = 5555;
    vNodes.push_back(xData);

    xData.strIP = "192.168.1.101";
    xData.nWeight = 1000;
    xData.nPort = 6666;
    vNodes.push_back(xData);

    xData.strIP = "192.168.1.102";
    xData.nWeight = 1000;
    xData.nPort = 7777;
    vNodes.push_back(xData);

    xData.strIP = "192.168.1.103";
    xData.nWeight = 1000;
    xData.nPort = 8888;
    vNodes.push_back(xData);

    xData.strIP = "192.168.1.104";
    xData.nWeight = 1000;
    xData.nPort = 9999;
    vNodes.push_back(xData);
}

void InitVirtualNodes(const std::vector<MachineData>& vMainNodes, std::vector<NFCNode>& vVirNodes)
{
    for (int i = 0; i < vMainNodes.size(); ++i)
    {
        MachineData xData = vMainNodes[i];
        for (int j = 0; j < xData.nWeight; ++j)
        {
            NFCNode vNode(xData, j);
            vVirNodes.push_back(vNode);
        }
    }
}


void TestOneValue(NFCConsistentHash& xCH, uint32_t nValue)
{
    NFCConsistentHash::iterator it = xCH.Find(nValue);
    std::cout << it->second.xNodeData.strIP << " Index: " << it->second.nVirtualIndex << " Hash: " <<  it->first << std::endl;
}

void TestMulValue(NFCConsistentHash& xCH)
{
    std::map<std::string, int> xTest;

    for (int i = 0; i < 1000000; ++i)
    {
        boost::crc_32_type ret;
        char sz[100] = { 0 };
        sprintf_s(sz, "%d", i);
        ret.process_bytes(sz, 5);

        NFCConsistentHash::iterator itTest = xCH.Find(ret.checksum());

        std::map<std::string, int>::iterator itRet = xTest.find(itTest->second.xNodeData.strIP);
        if (itRet != xTest.end())
        {
            itRet->second++;
        }
        else
        {
            xTest.insert(std::map<std::string, int>::value_type(itTest->second.xNodeData.strIP, 0));
        }
    }

    for (std::map<std::string, int>::iterator it = xTest.begin(); it != xTest.end(); ++it)
    {
        std::cout << it->first << " : " << it->second << std::endl;

    }
}

int _tmain(int argc, _TCHAR* argv[])
{
    std::vector<MachineData> nodes;
    InitMainNodes(nodes);

    std::vector<NFCNode> vVirNodes;
    InitVirtualNodes(nodes, vVirNodes);

    NFCConsistentHash xConsistentHash;
    for (int i = 0; i < vVirNodes.size(); ++i)
    {
        xConsistentHash.Insert(vVirNodes[i]);
    }

    boost::crc_32_type ret;
    ret.process_bytes("213123123eewrwet34fertytrer", 5);
    uint32_t nTestValue = ret.checksum();
    TestOneValue(xConsistentHash, nTestValue);


    TestMulValue(xConsistentHash);

    MachineData xRemoveData = nodes[3];
    for (int i = 0; i < xRemoveData.nWeight; ++i)
    {
        NFCNode vNode(xRemoveData, i);
        xConsistentHash.Erase(vNode);
    }
    TestOneValue(xConsistentHash, nTestValue);

    TestMulValue(xConsistentHash);

    return 0;
}