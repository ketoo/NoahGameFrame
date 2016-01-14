// -------------------------------------------------------------------------
//    @FileName         :    NFCConsistentHash.hpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-03-01
//    @Module           :    NFCConsistentHash.hpp
//
// -------------------------------------------------------------------------


#include <map>
#include <string>
#include <list>
#include <functional> 
#include <algorithm>
#include "boost/crc.hpp"
#include "boost/lexical_cast.hpp"
#include "boost/format.hpp"

#ifndef NFC_CONSISTENT_HASH_H
#define NFC_CONSISTENT_HASH_H

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

	virtual std::string GetDataStr() const = 0;
	virtual int GetDataID() const = 0;

    std::string ToStr() const 
    {
        return boost::str(boost::format("%1%-%2%-%3%") % boost::lexical_cast<std::string>(GetDataID()) % GetDataStr() % nVirtualIndex);
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
	}

	NFCMachineNode()
	{
		strIP = "";
		nPort = 0;
		nWeight = 0;//总共多少权重即是多少虚拟节点
		nMachineID = 0;
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

    void Insert(const NFCMachineNode& xNode) 
    {
        uint32_t hash = m_pHasher->GetHashValue(xNode);
        auto it = mxNodes.find(hash);
        if (it == mxNodes.end())
        {
            mxNodes.insert(TMAP_TYPE::value_type(hash,xNode));
        }
    }

	bool Exist(const NFCMachineNode& xInNode)
	{
		uint32_t hash = m_pHasher->GetHashValue(xInNode);
		TMAP_TYPE::iterator it = mxNodes.find(hash);
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
		boost::crc_32_type ret;
		ret.process_bytes(str.c_str(), str.length());
		uint32_t nCRC32 = ret.checksum();

		if (GetSuitNode(nCRC32, node))
		{
			return true;
		}

		return false;
	}

	bool GetSuitNode(uint32_t hashValue, NFCMachineNode& node)
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

// 	bool GetMasterNodeReverse(NFCMachineNode& node)
// 	{
// 		if(mxNodes.empty())
// 		{
// 			return false;
// 		}
// 
// 		int nID = 0;
// 		TMAP_TYPE::iterator it = mxNodes.begin();
// 		for (; it != mxNodes.end(); ++it)
// 		{
// 			if (it->second.GetDataID() > nID)
// 			{
// 				node = it->second;
// 				nID = it->second.GetDataID();
// 			}
// 		}
// 
// 		return true;
// 	}
// 
// 	bool GetMasterNodeSequence(NFCMachineNode& node)
// 	{
// 		if(mxNodes.empty())
// 		{
// 			return false;
// 		}
// 
// 		int nID = mxNodes.begin()->second.GetDataID();
// 		TMAP_TYPE::iterator it = mxNodes.begin();
// 		for (; it != mxNodes.end(); ++it)
// 		{
// 			if (it->second.GetDataID() < nID)
// 			{
// 				node = it->second;
// 				nID = it->second.GetDataID();
// 			}
// 		}
// 
// 		return true;
// 	}

private:
	typedef std::map<uint32_t, NFCMachineNode> TMAP_TYPE;
	typedef TMAP_TYPE::iterator iterator;

    NFIHasher* m_pHasher;
    TMAP_TYPE mxNodes;
};


#endif
