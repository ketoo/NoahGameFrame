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
// ���
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

bool NFCDataList::Append(const NFIDataList::TData& TData)
{
    if (TData.nType <= TDATA_UNKNOWN
        || TData.nType >= TDATA_MAX)
    {
        return false;
    }

	switch (TData.nType)
	{
	case TDATA_INT:
	case TDATA_FLOAT:
	case TDATA_OBJECT:
		{
			AddValue(TData.nType, TData.variantData);
		}
		break;
	case TDATA_STRING:
		{
			const std::string& strData = boost::get<std::string>(TData.variantData);
			AddString(strData);
		}
		break;
	default:
		break;
	}


    return false;
}

bool NFCDataList::Append( const NFIDataList& src )
{
	return Append(src, 0, src.GetCount());
}

bool NFCDataList::Add(const NFINT64 value)
{
    return NFIDataList::AddValue<NFINT64>(TDATA_INT, value);
}

bool NFCDataList::Add(const double value)
{
    return AddValue<double>(TDATA_FLOAT, value);
}

bool NFCDataList::Add(const char* value)
{
    return AddValue<std::string>(TDATA_STRING, std::string(value));
}

bool NFCDataList::Add(const std::string& value)
{
    return AddValue<std::string>(TDATA_STRING, value);
}

bool NFCDataList::Add(const NFIDENTID& value)
{
    return AddValue<NFIDENTID>(TDATA_OBJECT, value);
}

bool NFCDataList::Add(const void* value)
{
    //return AddNumber<const void*>(TDATA_POINTER, value);
    return true;
}

bool NFCDataList::Add( const std::string& str, const NFINT64 value )
{
	return false;
}

bool NFCDataList::Add( const std::string& str, const double value )
{
	return false;
}

bool NFCDataList::Add( const std::string& str, const char* value )
{
	return false;
}

bool NFCDataList::Add( const std::string& str, const std::string& value )
{
	return false;
}

bool NFCDataList::Add( const std::string& str, const NFIDENTID& value )
{
	return false;
}

bool NFCDataList::Set(const int index, const NFINT64 value)
{
    if (index < GetCount() && index >= 0)
    {
        return SetValue(index, value);
    }

    return false;
}
bool NFCDataList::Set(const int index, const double value)
{
    if (index < GetCount() && index >= 0)
    {
        return SetValue(index, value);
    }

    return false;
}
bool NFCDataList::Set(const int index, const char* value)
{
    if (index < GetCount() && index >= 0)
    {
		return SetString(index, value);
    }

    return false;
}

bool NFCDataList::Set(const int index, const NFIDENTID& value)
{
    if (index < GetCount() && index >= 0)
    {
        return SetValue<NFIDENTID>(index, value);
    }

    return false;
}

bool NFCDataList::Set(const int index, const void* value)
{
    if (index < GetCount() && index >= 0)
    {
        //return SetNumber(index, value);
    }

    return false;
}

bool NFCDataList::Set( const std::string& str, const NFINT64 value )
{
	return false;
}

bool NFCDataList::Set( const std::string& str, const double value )
{
	return false;
}

bool NFCDataList::Set( const std::string& str, const char* value )
{
	return false;
}

bool NFCDataList::Set( const std::string& str, const std::string& value )
{
	return false;
}

bool NFCDataList::Set( const std::string& str, const NFIDENTID& value )
{
	return false;
}

NFINT64 NFCDataList::Int(const int index) const
{
    if (index < GetCount() && index >= 0)
    {
        return NumberVal<NFINT64>(index);
    }

    return 0;
}

NFINT64 NFCDataList::Int( const std::string& str ) const
{
	return 0;
}

double NFCDataList::Float(const int index) const
{
    if (index < GetCount() && index >= 0)
    {
        return NumberVal<double>(index);
    }

    return 0.0f;
}

double NFCDataList::Float( const std::string& str ) const
{
	return 0.0;
}

const std::string& NFCDataList::String(const int index) const
{
    if (index < GetCount() && index >= 0)
    {
        const NF_SHARE_PTR<TData> var = mvList[index];
        if (var && TDATA_STRING == var->nType)
        {
            return boost::get<const std::string&>(var->variantData);
        }
    }

    return NULL_STR;
}

const std::string& NFCDataList::String( const std::string& str ) const
{
	return NULL_STR;
}

const NFIDENTID& NFCDataList::Object(const int index) const
{
    if (index < GetCount() && index >= 0)
    {
        if (index < GetCount() && index >= 0)
        {
            TDATA_TYPE type = Type(index);
            if (type == TDATA_OBJECT)
            {
                NF_SHARE_PTR<TData> var = GetStack(index);
                return boost::get<NFIDENTID>(var->variantData);
            }
        }
    }

    return NULL_OBJECT;
}

const NFIDENTID& NFCDataList::Object( const std::string& str ) const
{
	return NULL_OBJECT;
}

void* NFCDataList::Pointer(const int index) const
{
    if (index < GetCount() && index >= 0)
    {
        return NumberVal<void*>(index);
    }

    return NULL;
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
        return mvList[index]->nType;
    }
    else
    {
        const NF_SHARE_PTR<TData> pData = GetStack(index);
        if (pData)
        {
            return pData->nType;
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
        //�Ƚ�
        TDATA_TYPE varType = Type(index);
        if (varType != pareType)
        {
            bRet = false;
            break;
        }

        ++index;
        pareType = (TDATA_TYPE)va_arg(arg_ptr, int);   //��ȡ��һ������
    }

    va_end(arg_ptr); //����

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
    //mnCapacity = STACK_SIZE;
	//8���Ժ�������
	if (mvList.size() > STACK_SIZE)
	{
		for (int i = 0; i < STACK_SIZE; ++i)
		{
			mvList[i]->nType = TDATA_UNKNOWN;
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
                AddValue<NFINT64>(vType, src.Int(i));
                break;
            case TDATA_FLOAT:
                AddValue<double>(vType, src.Float(i));
                break;
            case TDATA_STRING:
                AddString(src.String(i).c_str());
                break;
            case TDATA_OBJECT:
                AddValue<NFIDENTID>(vType, src.Object(i));
                break;
                //case TDATA_POINTER:
                //    AddNumber<void*>(vType, src.NumberVal<void*>(i));
                //    break;
            default:
                //Assert(0);
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
			strData = boost::lexical_cast<std::string> (Int(index));
			break;

		case TDATA_FLOAT:
			strData = boost::lexical_cast<std::string> (Float(index));
			break;

		case TDATA_STRING:
			strData = boost::lexical_cast<std::string> (String(index));
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
