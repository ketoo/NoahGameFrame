// -------------------------------------------------------------------------
//    @FileName         :    NFCDataList.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-03-01
//    @Module           :    NFCDataList
//
// -------------------------------------------------------------------------

#include <cstdarg>
#include "NFCDataList.h"
#include "NFIDataList.h"

NFCDataList::NFCDataList()
: NFIDataList()
{
}

NFCDataList::NFCDataList(const char* str, const char* strSplit)
{
    Clear();

    Split(str, strSplit);
}

NFCDataList::NFCDataList(const NFCDataList& src)
{
    Clear();

    InnerAppendEx(src, 0, src.GetCount());
}

NFCDataList::NFCDataList(const NFIDataList& src)
{
    Clear();

    InnerAppendEx(src, 0, src.GetCount());
}

NFCDataList::~NFCDataList()
{
    Clear();
};

/*
NFCDataList& NFCDataList::operator=(const NFCDataList& src)
{
    Clear();
    InnerAppendEx(src, 0, src.GetCount());

    return *this;
}

NFCDataList& NFCDataList::operator=(const NFIDataList& src)
{
    Clear();
    InnerAppendEx(src, 0, src.GetCount());

    return *this;
}

*/
// 添加
bool NFCDataList::Append(const NFIDataList& src, const int start, const int count)
{
    if (start >= src.GetCount())
    {
        return false;
    }

    int end = start + count;

    if (end > src.GetCount())
    {
        return false;
    }

    InnerAppendEx(src, start, end);

    return true;
}

bool NFCDataList::Append(const NFIDataList::TData& xData)
{
    if (xData.GetType() <= TDATA_UNKNOWN
        || xData.GetType() >= TDATA_MAX)
    {
        return false;
    }

	switch (xData.GetType())
	{
	case TDATA_INT:
		AddInt(xData.GetInt());
		break;
	case TDATA_FLOAT:
		AddFloat(xData.GetFloat());
		break;
	case TDATA_OBJECT:
		AddObject(xData.GetObject());
		break;
	case TDATA_STRING:
		AddString(xData.GetString());
		break;
	default:
		break;
	}

    return true;
}

bool NFCDataList::Append( const NFIDataList& src )
{
	return Append(src, 0, src.GetCount());
}

bool NFCDataList::Add(const NFINT64 value)
{
	if (GetCount() == mvList.size())
	{
		AddStatck();
	}
	
	NF_SHARE_PTR<TData> var = GetStack(GetCount());
	if (var)
	{
		var->SetInt(value);
		mnUseSize++;

		return true;
	}

    return false;
}

bool NFCDataList::Add(const double value)
{
	if (GetCount() == mvList.size())
	{
		AddStatck();
	}

	NF_SHARE_PTR<TData> var = GetStack(GetCount());
	if (var)
	{
		var->SetFloat(value);
		mnUseSize++;

		return true;
	}

	return false;
}

bool NFCDataList::Add(const char* value)
{
	if (GetCount() == mvList.size())
	{
		AddStatck();
	}

	NF_SHARE_PTR<TData> var = GetStack(GetCount());
	if (var)
	{
		var->SetString(value);
		mnUseSize++;

		return true;
	}

	return false;
}

bool NFCDataList::Add(const std::string& value)
{
	if (GetCount() == mvList.size())
	{
		AddStatck();
	}

	NF_SHARE_PTR<TData> var = GetStack(GetCount());
	if (var)
	{
		var->SetString(value);
		mnUseSize++;

		return true;
	}

	return false;
}

bool NFCDataList::Add(const NFGUID& value)
{
	if (GetCount() == mvList.size())
	{
		AddStatck();
	}

	NF_SHARE_PTR<TData> var = GetStack(GetCount());
	if (var)
	{
		var->SetObject(value);
		mnUseSize++;

		return true;
	}

	return false;
}

bool NFCDataList::Set(const int index, const NFINT64 value)
{
    if (index < GetCount()
		&& index >= 0
		&& Type(index) == TDATA_INT)
    {
		NF_SHARE_PTR<TData> var = GetStack(GetCount());
		if (var)
		{
			var->SetInt(value);

			return true;
		}
    }

    return false;
}
bool NFCDataList::Set(const int index, const double value)
{
	if (index < GetCount()
		&& index >= 0
		&& Type(index) == TDATA_FLOAT)
	{
		NF_SHARE_PTR<TData> var = GetStack(GetCount());
		if (var)
		{
			var->SetFloat(value);

			return true;
		}
	}

    return false;
}
bool NFCDataList::Set(const int index, const char* value)
{
	if (index < GetCount()
		&& index >= 0
		&& Type(index) == TDATA_STRING)
	{
		NF_SHARE_PTR<TData> var = GetStack(GetCount());
		if (var)
		{
			var->SetString(value);

			return true;
		}
	}

	return false;
}

bool NFCDataList::Set(const int index, const NFGUID& value)
{
	if (index < GetCount()
		&& index >= 0
		&& Type(index) == TDATA_OBJECT)
	{
		NF_SHARE_PTR<TData> var = GetStack(GetCount());
		if (var)
		{
			var->SetObject(value);

			return true;
		}
	}

	return false;
}

NFINT64 NFCDataList::Int(const int index) const
{
    if (index < GetCount() && index >= 0)
    {
		if (Type(index) == TDATA_INT)
		{
			const NF_SHARE_PTR<TData> var = GetStack(index);
			return var->GetInt();
		}
    }

    return 0;
}

double NFCDataList::Float(const int index) const
{
    if (index < GetCount() && index >= 0)
    {
		const NF_SHARE_PTR<TData> var = mvList[index];
		if (var && TDATA_FLOAT == var->GetType())
		{
			return var->GetFloat();
		}
    }

    return 0.0f;
}

const std::string& NFCDataList::String(const int index) const
{
    if (index < GetCount() && index >= 0)
    {
        const NF_SHARE_PTR<TData> var = mvList[index];
        if (var && TDATA_STRING == var->GetType())
        {
            return var->GetString();
        }
    }

    return NULL_STR;
}

const NFGUID& NFCDataList::Object(const int index) const
{
    if (index < GetCount() && index >= 0)
    {
		TDATA_TYPE type = Type(index);
		if (TDATA_OBJECT == type)
		{
			NF_SHARE_PTR<TData> var = GetStack(index);
			if (var.get())
			{
				return var->GetObject();
			}
		}
    }

    return NULL_OBJECT;
}

bool NFCDataList::Split(const char* str, const char* strSplit)
{
	Clear();

    std::string strData(str);
    if (strData.empty())
    {
        return true;
    }

    std::string temstrSplit(strSplit);
    std::string::size_type pos;
    strData += temstrSplit;
    std::string::size_type size = strData.length();

    for (std::string::size_type i = 0; i < size; i++)
    {
        pos = int(strData.find(temstrSplit, i));
        if (pos < size)
        {
            std::string strSub = strData.substr(i, pos - i);
            Add(strSub.c_str());

            i = pos + temstrSplit.size() - 1;
        }
    }

    return true;
}

TDATA_TYPE NFCDataList::Type(const int index) const
{
    if (index >= GetCount() || index < 0)
    {
        return TDATA_UNKNOWN;
    }

    if (index < STACK_SIZE)
    {
        return mvList[index]->GetType();
    }
    else
    {
        const NF_SHARE_PTR<TData> pData = GetStack(index);
        if (pData)
        {
            return pData->GetType();
        }
    }

    return TDATA_UNKNOWN;
}

bool NFCDataList::TypeEx(const int nType, ...) const
{
    bool bRet = true;

    if (TDATA_UNKNOWN == nType)
    {
        bRet = false;
        return bRet;
    }

    TDATA_TYPE pareType = (TDATA_TYPE)nType;
    va_list arg_ptr;
    va_start(arg_ptr, nType);
    int index = 0;

    while (pareType != TDATA_UNKNOWN)
    {
        //比较
        TDATA_TYPE varType = Type(index);
        if (varType != pareType)
        {
            bRet = false;
            break;
        }

        ++index;
        pareType = (TDATA_TYPE)va_arg(arg_ptr, int);   //获取下一个参数
    }

    va_end(arg_ptr); //结束

    return bRet;
}

bool NFCDataList::Concat(const NFIDataList& src)
{
    InnerAppendEx(src, 0, src.GetCount());
    return true;
}

void NFCDataList::Clear()
{
	mnUseSize = 0;
	//8个以后的清除掉
	if (mvList.size() > STACK_SIZE)
	{
		for (int i = 0; i < STACK_SIZE; ++i)
		{
			mvList[i]->Reset();
		}

		mvList.erase(mvList.begin() + 8, mvList.end());
	}
}

bool NFCDataList::IsEmpty() const
{
    return (0 == mnUseSize);
}

int NFCDataList::GetCount() const
{
    return mnUseSize;
}

void NFCDataList::InnerAppendEx(const NFIDataList& src, const int start, const int end)
{
    for (int i = start; i < end; ++i)
    {
        TDATA_TYPE vType = src.Type(i);
        switch (vType)
        {
            case TDATA_INT:
				AddInt(src.Int(i));
                break;
            case TDATA_FLOAT:
				AddFloat(src.Float(i));
                break;
            case TDATA_STRING:
                AddString(src.String(i));
                break;
            case TDATA_OBJECT:
                AddObject(src.Object(i));
                break;
            default:
                break;
        }
    }
}

std::string NFCDataList::StringValEx(const int index) const
{
    if (index < GetCount() && index >= 0)
    {
		std::string strData;

		const TDATA_TYPE eType =  Type(index);
		switch (eType)
		{
		case TDATA_INT:
			strData = lexical_cast<std::string> (Int(index));
			break;

		case TDATA_FLOAT:
			strData = lexical_cast<std::string> (Float(index));
			break;

		case TDATA_STRING:
			strData = String(index);
			break; 

        case TDATA_OBJECT:
			strData = Object(index).ToString();
			break;

        default:
			strData = NULL_STR;
			break;
		}
    }

    return NULL_STR;
}

bool NFCDataList::ToString(std::string& str, const char* strSplit) const
{
    for (int i = 0; i < GetCount(); ++i)
    {
        std::string strVal = StringValEx(i);
        str += strVal;
        str += strSplit;
    }

    std::string strTempSplit(strSplit);
    std::string::size_type nPos = str.rfind(strSplit);
    if (nPos == str.length() - strTempSplit.length())
    {
        str = str.substr(0, nPos);
    }

    return true;
}

void NFCDataList::AddStatck()
{
	for (int i = 0; i < STACK_SIZE; ++i )
	{
		NF_SHARE_PTR<TData> pData(NF_NEW TData());
		mvList.push_back(pData);
	}
}

const NF_SHARE_PTR<NFIDataList::TData> NFCDataList::GetStack( const int index ) const
{
	if (index < mvList.size())
	{
		return mvList[index];
	}

	return NF_SHARE_PTR<TData>();
}