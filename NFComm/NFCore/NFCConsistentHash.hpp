#include <map>
#include <string>
#include <list>
#include <functional> 
#include <algorithm>
#include "boost/crc.hpp"
#include "boost/format/free_funcs.hpp"

#ifndef __CONSISTENT_HASH_H__
#define __CONSISTENT_HASH_H__

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

	virtual std::string GetDataStr() = 0;

    std::string ToStr() const 
    {
        return boost::str(boost::format("%1%-%2%") % GetDataStr() % nVirtualIndex);
    }

private:


    int nVirtualIndex;//虚拟节点序号
};

class NFCMachineNode : public NFIVirtualNode
{
	NFCMachineNode(const int nVirID) : NFIVirtualNode(nVirID)
	{
		strIP = "";
		nPort = 0;
		nWeight = 0;//总共多少权重即是多少虚拟节点
		nMachineID = 0;
	}

	NFCMachineNode()
	{
		strIP = "";
		nPort = 0;
		nWeight = 0;//总共多少权重即是多少虚拟节点
		nMachineID = 0;
	}

	virtual std::string GetDataStr()
	{
		return strIP;
	}

	std::string strIP;
	int nPort;
	int nWeight;
	int nMachineID;
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
        boost::crc_32_type ret;
        std::string vnode = vNode.ToStr();
        ret.process_bytes(vnode.c_str(), vnode.size());

        return ret.checksum();
    }
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

    void Insert(const NFIVirtualNode& xNode) 
    {
        uint32_t hash = m_pHasher->GetHashValue(xNode);
        auto it = mxNodes.find(hash);
        if (it == mxNodes.end())
        {
            mxNodes.insert(TMAP_TYPE::value_type(hash,xNode));
        }
    }


    std::size_t Erase(const NFIVirtualNode& xNode) 
    {
        uint32_t hash = m_pHasher->GetHashValue(xNode);
        return mxNodes.erase(hash);
    }

	bool SuitNode(uint32_t hashValue, NFIVirtualNode& node)
	{
		if(mxNodes.empty())
		{
			return false;
		}

		TMAP_TYPE::iterator it = mxNodes.lower_bound(hashValue);

		if (it == mxNodes.end())
		{
			it = mxNodes.begin();
		}

		node = it->second;

		return true;
	}
private:
	typedef std::map<uint32_t, NFIVirtualNode> TMAP_TYPE;
	typedef TMAP_TYPE::iterator iterator;

    NFIHasher* m_pHasher;
    TMAP_TYPE mxNodes;
};


#endif
