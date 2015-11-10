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
#include "NFComm/NFPluginModule/NFIdentID.h"
#include "NFComm/NFPluginModule/NFPlatform.h"

//��������
enum TDATA_TYPE
{
    TDATA_UNKNOWN,  // δ֪
    TDATA_INT,              // 32λ����
    TDATA_FLOAT,       // ������(˫���ȣ���double����ʵ��)
    TDATA_STRING,       // �ַ���
    TDATA_OBJECT,       // ����ID
    //TDATA_POINTER,      // ָ��
    TDATA_MAX,
};

const static std::string NULL_STR = "";

//���ͽӿ�
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

        template<typename T>
        T Value() const
        {
            T result = 0;
            if (nType == TDATA_FLOAT
                || nType == TDATA_INT)
            {
                result = boost::get<T>(variantData);
            }

            return result;
        }

        const std::string& String() const
        {
            if (TDATA_STRING == nType)
            {
                return boost::get<const std::string&>(variantData);
            }

            return NULL_STR;
        }

        const NFIDENTID& Object() const
        {
            NFIDENTID result;
            if (TDATA_STRING == nType)
            {
                return boost::get<const NFIDENTID&>(variantData);
            }

            return result;
        }

        TDATA_TYPE nType;
        boost::variant<NFINT64, double, std::string, NFIDENTID, void*> variantData;
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

    virtual std::string StringValEx(const int index) const = 0;
    virtual bool ToString(std::string& str, const char* strSplit) const = 0;

 protected:
    template<typename T>
    T NumberVal(const int index) const
    {
        T result = 0;
        if (index < GetCount() && index >= 0)
        {
            TDATA_TYPE type =  Type(index);
            if (type == TDATA_FLOAT
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
            if (type == TDATA_FLOAT
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

        if (type == TDATA_FLOAT
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
    // �ϲ�
    virtual bool Concat(const NFIDataList& src) = 0;
    // �������
	virtual bool Append(const NFIDataList& src) = 0;
    virtual bool Append(const NFIDataList& src, const int start, const int count) = 0;
    // �������
    virtual bool Append(const NFIDataList::TData& sTData) = 0;
    // ���
    virtual void Clear() = 0;
    // �Ƿ�Ϊ��
    virtual bool IsEmpty() const = 0;
    // ��������
    virtual int GetCount() const = 0;
    // ��������
    virtual TDATA_TYPE Type(const int index) const = 0;
    //�������ͼ��
    virtual bool TypeEx(const  int nType, ...) const = 0;
    //�½�����
    virtual bool Split(const char* str, const char* strSplit) = 0;

    // �������
    virtual bool Add(const NFINT64 value) = 0;
    virtual bool Add(const double value) = 0;
    virtual bool Add(const char* value) = 0;

    virtual bool Add(const std::string& value) = 0;
    virtual bool Add(const NFIDENTID& value) = 0;
    virtual bool Add(const void* value) = 0;

    virtual bool Set(const int index, const NFINT64 value) = 0;
    virtual bool Set(const int index, const double value) = 0;
    virtual bool Set(const int index, const char* value) = 0;
    virtual bool Set(const int index, const NFIDENTID& value) = 0;
    virtual bool Set(const int index, const void* value) = 0;

    // �������
    virtual NFINT64 Int(const int index) const = 0;
    virtual double Float(const int index) const = 0;
    virtual const std::string& String(const int index) const = 0;
    virtual NFIDENTID Object(const int index) const = 0;
    virtual void* Pointer(const int index) const = 0;

    bool AddInt(const NFINT64 value)
    {
        return Add(value);
    }
    bool AddFloat(const double value)
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
    bool SetFloat(const int index, const double value)
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

	virtual bool Add(const std::string& str, const NFINT64 value) = 0;
	virtual bool Add(const std::string& str, const double value) = 0;
	virtual bool Add(const std::string& str, const char* value) = 0;
	virtual bool Add(const std::string& str, const std::string& value) = 0;
	virtual bool Add(const std::string& str, const NFIDENTID& value) = 0;

	virtual bool Set(const std::string& str, const NFINT64 value) = 0;
	virtual bool Set(const std::string& str, const double value) = 0;
	virtual bool Set(const std::string& str, const char* value) = 0;
	virtual bool Set(const std::string& str, const std::string& value) = 0;
	virtual bool Set(const std::string& str, const NFIDENTID& value) = 0;

	// �������
	virtual NFINT64 Int(const std::string& str) const = 0;
	virtual double Float(const std::string& str) const = 0;
	virtual const std::string& String(const std::string& str) const = 0;
	virtual NFIDENTID Object(const std::string& str) const = 0;
/*
	NFIDataList& NFIDataList::operator = (const NFIDataList& src)
	{
		Clear();
		Append(src, 0, src.GetCount());

		return *this;
	}
*/
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

    inline NFIDataList& operator<<(const NFINT64& value)
    {
        Add(value);
        return *this;
    }
	inline NFIDataList& operator<<(const int value)
	{
		Add((NFINT64)value);
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
    std::vector< NF_SHARE_PTR<TData> > mvList;
	std::map<std::string, NF_SHARE_PTR<TData> > mxMap;
};

inline NFIDataList::~NFIDataList() {}

#endif
