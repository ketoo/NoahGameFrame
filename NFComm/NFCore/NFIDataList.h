// -------------------------------------------------------------------------
//    @FileName         :    NFIDataList.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-03-01
//    @Module           :    NFIDataList
//
// -------------------------------------------------------------------------

#ifndef _NFI_DATALIST_H_
#define _NFI_DATALIST_H_

#include <string>
#include <cstring>
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <list>
#include <vector>
#include <map>
#include <algorithm>
#include <assert.h>
#include <map>
#include <typeinfo>
#include <iomanip>
#include <functional>
#include <iostream>
#include <fstream>
#include <memory>
#include "NFComm/NFPluginModule/NFPlatform.h"
#include "NFCMemory.h"
#include <boost/lexical_cast.hpp>
#include <boost/variant.hpp>
#include <boost/algorithm/string.hpp>
#include "NFComm/NFPluginModule/NFIdentID.h"

//变量类型
enum TDATA_TYPE
{
    TDATA_UNKNOWN,  // 未知
    TDATA_INT,              // 32位整数
    TDATA_FLOAT,            // 单精度浮点数
    TDATA_DOUBLE,       // 双精度浮点数
    TDATA_STRING,       // 字符串
    TDATA_OBJECT,       // 对象ID
    //TDATA_POINTER,      // 指针
    TDATA_MAX,
};

const static std::string NULL_STR = "";

//类型接口
class NFIDataList
{
public:
    struct TData
    {
    public:
        TData()
        {
            nType = TDATA_UNKNOWN;
        }

        ~TData()
        {
            nType = TDATA_UNKNOWN;
        }

        TDATA_TYPE nType;
        boost::variant<NFINT64, float, double, std::string, NFIDENTID, void*> variantData;
    };

    NFIDataList()
    {
        mnCapacity = STACK_SIZE;
        mnNextOrderCapacity = mnCapacity * 2;
        mnSize = 0;
        mnOrder = 0;
    }

    virtual ~NFIDataList() = 0;

    virtual std::string StringValEx(const int index, const bool bForce) const = 0;
    virtual bool ToString(std::string& str, const char* strSplit) const = 0;
	virtual bool FormString(std::string& str, const char* strSplit) = 0;

 protected:
    template<typename T>
    T NumberVal(const int index) const
    {
        T result = 0;
        if (index < mnSize && index >= 0)
        {
            TDATA_TYPE type =  Type(index);
            if (type == TDATA_DOUBLE
                || type == TDATA_FLOAT
                || type == TDATA_INT
                /*|| type == TDATA_OBJECT*/
                /*|| type == TDATA_POINTER*/)
            {
                const TData* var = GetStackConst(index);
                result = boost::get<T>(var->variantData);
            }
        }

        return result;
    }

    template<typename T>
    bool SetNumber(const int index, const T& value)
    {
        if (index < mnSize && index >= 0)
        {
            TDATA_TYPE type =  Type(index);
            if (type == TDATA_DOUBLE
                || type == TDATA_FLOAT
                || type == TDATA_INT
                || type == TDATA_OBJECT
                /*|| type == TDATA_POINTER*/)
            {
                TData* var = GetStack(index);
                var->variantData = value;
                return true;
            }
        }

        return false;
    }

    template<typename T>
    bool AddNumber(const TDATA_TYPE type, const T& value)
    {
        if (type == TDATA_DOUBLE
            || type == TDATA_FLOAT
            || type == TDATA_INT
            || type == TDATA_OBJECT
            /*|| type == TDATA_POINTER*/)
        {
            TData* var = GetStack(mnSize);
            if (var)
            {
                var->nType = type;
                var->variantData = value;
                mnSize++;

                return true;
            }
        }
        return false;
    }

protected:
    //根据序号得到阶,8->16->32
    int GetOrder(const int index, int& nNewIndex) const
    {
        int nOrder = -1;
        int nLastStackSize = STACK_SIZE;
        nNewIndex = index;

        while (nNewIndex - nLastStackSize >= 0)
        {
            nOrder++;

            nNewIndex -= nLastStackSize;
            nLastStackSize *= 2;
        }

        return nOrder;
    }

    TData* GetStack(const int index)
    {
        //mnNewSize是8的阶层
        if (index < STACK_SIZE)
        {
            return &mvStack[index];
        }
        else if (index < mnCapacity)
        {
            int nOrderIndex = 0;
            int nOrder = GetOrder(index, nOrderIndex);
            if (nOrder >= 0)
            {
                TData* pData = mvList[nOrder];
                return &pData[nOrderIndex];
            }
        }
        else if (index == mnCapacity)
        {
            TData* pData = NF_NEW TData[mnNextOrderCapacity];
            mvList.push_back(pData);
            mvList[mnOrder] = pData;
            mnOrder += 1;
            mnCapacity += mnNextOrderCapacity;
            mnNextOrderCapacity *= 2;

            return GetStack(index);
        }

        return NULL;
    }

public:
    const TData* GetStackConst(const int index) const
    {
        //mnNewSize是8的阶层
        if (index < STACK_SIZE)
        {
            return &mvStack[index];
        }
        else if (index < mnCapacity)
        {
            int nOrderIndex = 0;
            int nOrder = GetOrder(index, nOrderIndex);
            if (nOrder >= 0)
            {
                TData* pData = mvList[nOrder];
                return &pData[nOrderIndex];
            }
        }

        return NULL;
    }


    // 合并
    virtual bool Concat(const NFIDataList& src) = 0;
    // 部分添加
    virtual bool Append(const NFIDataList& src, int start, int count) = 0;
    // 部分添加
    virtual bool Append(const NFIDataList::TData& sTData) = 0;
    // 清空
    virtual void Clear() = 0;
    // 是否为空
    virtual bool IsEmpty() const = 0;
    // 数据数量
    virtual int GetCount() const = 0;
    // 数据类型
    virtual TDATA_TYPE Type(const int index) const = 0;
    //数据类型检测
    virtual bool TypeEx(const  int nType, ...) const = 0;
    //新进入拆分
    virtual bool Split(const char* str, const char* strSplit) = 0;

    // 添加数据
    virtual bool Add(const NFINT64 value) = 0;
    virtual bool Add(const float value) = 0;
    virtual bool Add(const double value) = 0;
    virtual bool Add(const char* value) = 0;

    virtual bool Add(const std::string& value) = 0;
    virtual bool Add(const NFIDENTID& value) = 0;
    virtual bool Add(const void* value) = 0;

    virtual bool Set(const int index, const NFINT64 value) = 0;
    virtual bool Set(const int index, const float value) = 0;
    virtual bool Set(const int index, const double value) = 0;
    virtual bool Set(const int index, const char* value) = 0;
    virtual bool Set(const int index, const NFIDENTID& value) = 0;
    virtual bool Set(const int index, const void* value) = 0;

    // 获得数据
    virtual NFINT64 Int(const int index) const = 0;
    virtual float Float(const int index) const = 0;
    virtual double Double(const int index) const = 0;
    virtual const std::string& String(const int index) const = 0;
    virtual NFIDENTID Object(const int index) const = 0;
    virtual void* Pointer(const int index) const = 0;

    bool AddInt(const NFINT64 value)
    {
        return Add(value);
    }
    bool AddFloat(const float value)
    {
        return Add(value);
    }
    bool AddDouble(const double value)
    {
        return Add(value);
    }
    bool AddString(const char* value)
    {
        return Add(value);
    }
    bool AddString(const std::string& value)
    {
        return Add(value);
    }
    bool AddObject(const NFIDENTID& value)
    {
        return Add(value);
    }
    bool AddPoint(const void* value)
    {
        return Add(value);
    }

    bool SetInt(const int index, const NFINT64 value)
    {
        return Set(index, value);
    }
    bool SetFloat(const int index, const float value)
    {
        return Set(index, value);
    }
    bool SetDouble(const int index, const double value)
    {
        return Set(index, value);
    }
    bool SetString(const int index, const char* value)
    {
        return Set(index, value);
    }
    bool SetObject(const int index, const NFIDENTID& value)
    {
        return Set(index, value);
    }
    bool SetPoint(const int index, const void* value)
    {
        return Set(index, value);
    }

    static bool Valid(const NFIDataList::TData& var)
    {
        bool bChanged = false;

        switch (var.nType)
        {
            case TDATA_INT:
            {
                if (0 != boost::get<NFINT64>(var.variantData))
                {
                    bChanged = true;
                }
            }
            break;
            case TDATA_FLOAT:
            {
                float fValue = boost::get<float>(var.variantData);
                if (fValue > 0.001f  || fValue < -0.001f)
                {
                    bChanged = true;
                }
            }
            break;
            case TDATA_DOUBLE:
            {
                double fValue = boost::get<double>(var.variantData);
                if (fValue > 0.001f  || fValue < -0.001f)
                {
                    bChanged = true;
                }
            }
            break;
            case TDATA_STRING:
            {
                const std::string& strData = boost::get<const std::string&>(var.variantData);
                if (!strData.empty())
                {
                    bChanged = true;
                }
            }
            break;
            case TDATA_OBJECT:
            {
                if (NFIDENTID() != boost::get<NFIDENTID>(var.variantData))
                {
                    bChanged = true;
                }
            }
            break;
            //case TDATA_POINTER:
            //    {
            //        if (0 != boost::get<void*>(var.variantData))
            //        {
            //            bChanged = true;
            //        }
            //    }
            //    break;
            default:
                break;
        }

        return bChanged;
    }

    inline bool Compare(const int nPos, const NFIDataList& src) const
    {
        if (src.GetCount() > nPos
            && GetCount() > nPos
            && src.Type(nPos) == Type(nPos))
        {
            switch (src.Type(nPos))
            {
                case TDATA_INT:
                    return Int(nPos) == src.Int(nPos);
                    break;

                case TDATA_FLOAT:
                    return fabs(Float(nPos) - src.Float(nPos)) < 0.001f;
                    break;

                case TDATA_DOUBLE:
                    return fabs(Double(nPos) - src.Double(nPos)) < 0.001f;
                    break;

                case TDATA_STRING:
                    return String(nPos) == src.String(nPos);
                    break;

                case TDATA_OBJECT:
                    return Object(nPos) == src.Object(nPos);
                    break;

                    //case TDATA_POINTER:
                    //    return Pointer(nPos) == src.Pointer(nPos);
                    //    break;

                default:
                    return false;
                    break;
            }
        }

        return false;
    }

    inline bool operator==(const NFIDataList& src) const
    {
        if (src.GetCount() == GetCount())
        {
            for (int i = 0; i < GetCount(); i++)
            {
                if (!Compare(i, src))
                {
                    return false;
                }
            }

            return true;
        }

        return false;
    }

    inline bool operator!=(const NFIDataList& src)
    {
        return !(*this == src);
    }

    inline NFIDataList& operator<<(const char value)
    {
        Add((NFINT64)value);
        return *this;
    }
    inline NFIDataList& operator<<(const unsigned char value)
    {
        Add((NFINT64)value);
        return *this;
    }
    inline NFIDataList& operator<<(const short value)
    {
        Add((NFINT64)value);
        return *this;
    }
    inline NFIDataList& operator<<(const unsigned short value)
    {
        Add((NFINT64)value);
        return *this;
    }
    inline NFIDataList& operator<<(const int value)
    {
        Add((NFINT64)value);
        return *this;
    }
    inline NFIDataList& operator<<(const unsigned int value)
    {
        Add((NFINT64)value);
        return *this;
    }
    inline NFIDataList& operator<<(const long value)
    {
        Add((NFINT64)value);
        return *this;
    }
    inline NFIDataList& operator<<(const unsigned long value)
    {
        Add((NFINT64)value);
        return *this;
    }
    inline NFIDataList& operator<<(const float value)
    {
        Add(value);
        return *this;
    }
    inline NFIDataList& operator<<(const double value)
    {
        Add(value);
        return *this;
    }
    inline NFIDataList& operator<<(const char* value)
    {
        Add(value);
        return *this;
    }
    inline NFIDataList& operator<<(const std::string& value)
    {
        Add(value);
        return *this;
    }

    //inline NFIDataList& operator<<(const NFINT64& value)
    //{
    //    Add(NFIDENTID(value));
    //    return *this;
    //}
    inline NFIDataList& operator<<(const int64_t& value)
    {
        Add(NFINT64(value));
        return *this;
    }

    inline NFIDataList& operator<<(const NFIDENTID& value)
    {
        Add(value);
        return *this;
    }
    inline NFIDataList& operator<<(const void* value)
    {
        Add(value);
        return *this;
    }

    inline NFIDataList& operator<<(const NFIDataList& value)
    {
        Concat(value);
        return *this;
    }
    enum { STACK_SIZE = 8 };
protected:
    int mnCapacity; //容量
    int mnSize;         //当前使用了的对象数量
    int mnNextOrderCapacity;
    NFINT16 mnOrder;            //扩充了几阶
    boost::array<TData, STACK_SIZE> mvStack;
    std::vector<TData*> mvList;
};

inline NFIDataList::~NFIDataList() {}

#endif
