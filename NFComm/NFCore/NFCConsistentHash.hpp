// -------------------------------------------------------------------------
//    @FileName         :    NFCConsistentHash.hpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-03-01
//    @Module           :    NFCConsistentHash.hpp
//
// -------------------------------------------------------------------------
#ifndef NFC_CONSISTENT_HASH_H
#define NFC_CONSISTENT_HASH_H

#include <map>
#include <string>
#include <list>
#include <functional> 
#include <algorithm>
#include "NFComm/NFPluginModule/NFPlatform.h"
#include "common/crc32.hpp"

//虚拟节点
class NFIVirtualNode 
{
public:

    //主机IP，此主机的第几个虚节点序号
    NFIVirtualNode(const int nVirID)
        :nVirtualIndex(nVirID)
    {

    }

	NFIVirtualNode()
	{
		nVirtualIndex = 0;
	}

	virtual ~NFIVirtualNode()
	{
		nVirtualIndex = 0;
	}

	virtual std::string GetDataStr() const
	{
		return "";
	}
	virtual int GetDataID() const
	{
		return 0;
	}
	virtual bool Candidate() const
	{
		return false;
	};

    std::string ToStr() const 
    {
        std::ostringstream strInfo;
        strInfo << lexical_cast<std::string>(GetDataID()) << "-" << GetDataStr() << "-" << nVirtualIndex;
        return strInfo.str();
        //return boost::str(boost::format("%1%-%2%-%3%") % lexical_cast<std::string>(GetDataID()) % GetDataStr() % nVirtualIndex);
    }

private:
    int nVirtualIndex;//虚拟节点序号
};

class NFCMachineNode : public NFIVirtualNode
{
public:
	NFCMachineNode(const int nVirID) : NFIVirtualNode(nVirID)
	{
		strIP = "";
		nPort = 0;
		nWeight = 0;//总共多少权重即是多少虚拟节点
		nMachineID = 0;
		bCandidate = false;
	}

	NFCMachineNode()
	{
		strIP = "";
		nPort = 0;
		nWeight = 0;//总共多少权重即是多少虚拟节点
		nMachineID = 0;
		bCandidate = false;
	}

public:

	virtual std::string GetDataStr() const 
	{
		return strIP;
	}

	virtual int GetDataID() const
	{
		return nMachineID;
	}

	virtual bool Candidate() const
	{
		return bCandidate;
	}

	std::string strIP;
	int nPort;
	int nWeight;
	int nMachineID;
	bool bCandidate;

	//如果是候选主机，则在没启用之前，他会指向真实主机
	std::list<NFIVirtualNode> xRealMachine;
};

class NFIHasher
{
public:
    virtual uint32_t GetHashValue(const NFIVirtualNode& vNode) = 0;
};

class NFCHasher : public NFIHasher
{
public:
    virtual uint32_t GetHashValue(const NFIVirtualNode& vNode)
    {
        //boost::crc_32_type ret;
        //std::string vnode = vNode.ToStr();
        //ret.process_bytes(vnode.c_str(), vnode.size());
        
        //return ret.checksum();

        std::string vnode = vNode.ToStr();
        return NFrame::CRC32(vnode);
    }
};

class NFIConsistentHash
{
	virtual std::size_t Size() const = 0;
	virtual bool Empty() const = 0;

	virtual void Insert(const int nID, const std::string& strIP, int nPort) = 0;
	virtual void Insert(const NFCMachineNode& xNode) = 0;

	virtual bool Exist(const NFCMachineNode& xInNode) = 0;
	virtual std::size_t Erase(const NFCMachineNode& xNode)  = 0;

	virtual bool GetSuitNode(NFCMachineNode& node) = 0;
	virtual bool GetSuitNode(const std::string& str, NFCMachineNode& node) = 0;
	virtual bool GetSuitNode(uint32_t hashValue, NFCMachineNode& node) = 0;

	virtual bool GetNodeList(std::list<NFCMachineNode>& nodeList) = 0;
};

class NFCConsistentHash
{
public:

public:

    NFCConsistentHash()
    {
        m_pHasher = new NFCHasher();
    }

    virtual ~NFCConsistentHash()
    {
        delete m_pHasher;
        m_pHasher = NULL;
    }

public:
    std::size_t Size() const
    {
        return mxNodes.size();
    }

    bool Empty() const 
    {
        return mxNodes.empty();
    }

	void Insert(const int nID, const std::string& strIP, int nPort) 
	{
		NFCMachineNode xNode;
		xNode.nMachineID = nID;
		xNode.strIP = strIP;
		xNode.nPort = nPort;

		Insert(xNode);
	}

    void Insert(const NFCMachineNode& xNode) 
    {
        uint32_t hash = m_pHasher->GetHashValue(xNode);
        auto it = mxNodes.find(hash);
        if (it == mxNodes.end())
        {
            mxNodes.insert(std::map<uint32_t, NFCMachineNode>::value_type(hash, xNode));
        }
    }

	bool Exist(const NFCMachineNode& xInNode)
	{
		uint32_t hash = m_pHasher->GetHashValue(xInNode);
		std::map<uint32_t, NFCMachineNode>::iterator it = mxNodes.find(hash);
		if (it != mxNodes.end())
		{
			return true;
		}

		return false;
	}

    std::size_t Erase(const NFCMachineNode& xNode) 
    {
        uint32_t hash = m_pHasher->GetHashValue(xNode);
        return mxNodes.erase(hash);
    }

	bool GetSuitNode(NFCMachineNode& node)
	{
		int nID = 0;
		return GetSuitNode(nID, node);
	}

	bool GetSuitNode(const std::string& str, NFCMachineNode& node)
	{
		uint32_t nCRC32 = NFrame::CRC32(str);
        return GetSuitNode(nCRC32, node);
	}

	bool GetSuitNode(uint32_t hashValue, NFCMachineNode& node)
	{
		if(mxNodes.empty())
		{
			return false;
		}

		std::map<uint32_t, NFCMachineNode>::iterator it = mxNodes.lower_bound(hashValue);

		if (it == mxNodes.end())
		{
			it = mxNodes.begin();
		}

		node = it->second;

		return true;
	}

	bool GetNodeList(std::list<NFCMachineNode>& nodeList)
	{
		for (std::map<uint32_t, NFCMachineNode>::iterator it = mxNodes.begin(); it != mxNodes.end(); ++it)
		{
			nodeList.push_back(it->second);
		}

		return true;
	}

private:
	std::map<uint32_t, NFCMachineNode> mxNodes;
    NFIHasher* m_pHasher;
};


#endif
