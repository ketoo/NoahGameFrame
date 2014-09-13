#include <map>
#include <string>
#include <list>
#include <functional> 
#include <algorithm>



#ifndef __CONSISTENT_HASH_H__
#define __CONSISTENT_HASH_H__

struct MachineData
{
    MachineData()
    {
        strIP = "";
        nPort = 0;
        nWeight = 0;//总共多少权重即是多少虚拟节点
    }

    std::string strIP;
    int nPort;
    int nWeight;
};

//虚拟节点
class NFCNode 
{
public:

    //主机IP，此主机的第几个虚节点序号
    NFCNode(const MachineData& xData, int nVirID)
        :xNodeData(xData),nVirtualIndex(nVirID)
    {

    }

    std::string ToStr() const 
    {
        return boost::str(boost::format("%1%-%2%") % xNodeData.strIP % nVirtualIndex);
    }

    MachineData xNodeData;//主机IP,主机在主机列表中的索引
    int nVirtualIndex;//虚拟节点序号
};

class NFIHasher
{
public:
    virtual uint32_t GetHashValue(const NFCNode& vNode) = 0;
};

class NFCHasher : public NFIHasher
{
public:
    virtual uint32_t GetHashValue(const NFCNode& vNode)
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
    typedef std::map<uint32_t, NFCNode> TMAP_TYPE;
    typedef TMAP_TYPE::iterator iterator;

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

    void Insert(const NFCNode& node) 
    {
        uint32_t hash = m_pHasher->GetHashValue(node);
        auto it = mxNodes.find(hash);
        if (it == mxNodes.end())
        {
            mxNodes.insert(TMAP_TYPE::value_type(hash,node));
        }
    }

    void Erase(iterator it)
    {
        mxNodes.erase(it);
    }

    std::size_t Erase(const NFCNode& node) 
    {
        uint32_t hash = m_pHasher->GetHashValue(node);
        return mxNodes.erase(hash);
    }

    iterator Find(uint32_t hashValue) 
    {
        if(mxNodes.empty()) 
        {
            return mxNodes.end();
        }

        iterator it = mxNodes.lower_bound(hashValue);

        if (it == mxNodes.end())
        {
            it = mxNodes.begin();
        }

        return it;
    }

    iterator Next(const iterator& it)
    {
        iterator itNext = it;
        if (itNext == mxNodes.end())
        {
            itNext = mxNodes.begin();
        }
        else
        {
            ++itNext;
        }

        return itNext;
        
    }

    iterator Begin()
    { 
        return mxNodes.begin(); 
    }

    iterator End()
    { 
        return mxNodes.end(); 
    }

private:

    NFIHasher* m_pHasher;
    TMAP_TYPE mxNodes;
};


#endif
