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
#include "NFCMemory.h"
#include <boost/lexical_cast.hpp>
#include <boost/variant.hpp>
#include <boost/algorithm/string.hpp>
#include "NFComm/NFPluginModule/NFIdentID.h"
#include "NFComm/NFPluginModule/NFPlatform.h"

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
		mnUseSize = 0;
		for (int i = 0; i < STACK_SIZE; ++i)
		{
			mvList.push_back(NF_SHARE_PTR<TData>(NF_NEW TData()));
		}
    }

    virtual ~NFIDataList() = 0;

    virtual std::string StringValEx(const int index, const bool bForce) const = 0;
    virtual bool ToString(std::string& str, const char* strSplit) const = 0;

 protected:
    template<typename T>
    T NumberVal(const int index) const
    {
        T result(0);
        if (index < GetCount() && index >= 0)
        {
            TDATA_TYPE type =  Type(index);
            if (type == TDATA_DOUBLE
                || type == TDATA_FLOAT
                || type == TDATA_INT
                || type == TDATA_OBJECT)
            {
                const NF_SHARE_PTR<TData> var = GetStack(index);
                result = boost::get<T>(var->variantData);
            }
        }

        return result;
    }

    template<typename T>
    bool SetValue(const int index, const T& value)
    {
        if (index < GetCount() && index >= 0)
        {
            TDATA_TYPE type =  Type(index);
            if (type == TDATA_DOUBLE
                || type == TDATA_FLOAT
                || type == TDATA_INT
                || type == TDATA_OBJECT
				|| type == TDATA_STRING)
            {
                NF_SHARE_PTR<TData> var = GetStack(index);
                var->variantData = value;
                return true;
            }
        }

        return false;
    }

    template<typename T>
    bool AddValue(const TDATA_TYPE type, const T& value)
    {
		if (GetCount() == mvList.size())
		{
			AddStatck();
		}

        if (type == TDATA_DOUBLE
            || type == TDATA_FLOAT
            || type == TDATA_INT
            || type == TDATA_OBJECT
            || type == TDATA_STRING)
        {
            NF_SHARE_PTR<TData> var = GetStack(GetCount());
            if (var)
            {
                var->nType = type;
                var->variantData = value;
				mnUseSize++;

                return true;
            }
        }

        return false;
    }

	void AddStatck()
	{
		for (int i = 0; i < STACK_SIZE; ++i )
		{
			NF_SHARE_PTR<TData> pData(NF_NEW TData());
			mvList.push_back(pData);
		}
	}

public:
	const NF_SHARE_PTR<TData> GetStack(const int index) const
	{
		if (index < mvList.size())
		{
			return mvList[index];
		}

		return NF_SHARE_PTR<TData>();
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

	NFIDataList& NFIDataList::operator=(const NFIDataList& src)
	{
		Clear();
		Append(src, 0, src.GetCount());

		return *this;
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
	int mnUseSize;
    std::vector<NF_SHARE_PTR<TData>> mvList;
};

inline NFIDataList::~NFIDataList() {}

#endif
