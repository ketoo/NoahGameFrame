// -------------------------------------------------------------------------
//    @FileName         :    NFCDataList.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-03-01
//    @Module           :    NFCDataList
//
// -------------------------------------------------------------------------

#include "NFCDataList.h"

NFCDataList::NFCDataList()
{
    mnCapacity = STACK_SIZE;
    mnNextOrderCapacity = mnCapacity * 2;
    mnSize = 0;
    mnOrder = 0;
    Clear();

}

NFCDataList::NFCDataList(const char* str, const char* strSplit)
{
    mnSize = 0;
    mnOrder = 0;
    mnCapacity = STACK_SIZE;
    mnNextOrderCapacity = mnCapacity * 2;

    Clear();

    Split(str, strSplit);
}

NFCDataList::NFCDataList(const NFIDataList& src)
{
    mnSize = 0;
    mnOrder = 0;
    mnCapacity = STACK_SIZE;
    mnNextOrderCapacity = mnCapacity * 2;

    Clear();

    InnerAppendEx(src, 0, src.GetCount());
}

NFCDataList::~NFCDataList()
{
    Clear();
};

NFCDataList& NFCDataList::operator=(const NFIDataList& src)
{
    Clear();
    InnerAppendEx(src, 0, src.GetCount());

    return *this;
}

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

bool NFCDataList::Append(const NFIDataList::TData& TData)
{
    if (TData.nType <= TDATA_UNKNOWN
        || TData.nType >= TDATA_MAX)
    {
        return false;
    }

    NFIDataList::TData* pVar = GetStack(mnSize);
    if (pVar)
    {
        pVar->nType = TData.nType;
        pVar->variantData = TData.variantData;
        mnSize++;

        return true;
    }

    return false;
}

bool NFCDataList::Add(const int value)
{
    return NFIDataList::AddNumber<int>(TDATA_INT, value);
}

bool NFCDataList::Add(const float value)
{
    return AddNumber<float>(TDATA_FLOAT, value);
}

bool NFCDataList::Add(const double value)
{
    return AddNumber<double>(TDATA_DOUBLE, value);
}

bool NFCDataList::Add(const char* value)
{
    TData* pVar = GetStack(mnSize);
    if (pVar)
    {
        pVar->nType = TDATA_STRING;
        pVar->variantData = (std::string)value;
        mnSize++;
    }

    return true;
    //return AddStringEx(value);
}

bool NFCDataList::Add(const NFIDENTID& value)
{
    return AddNumber<NFINT64>(TDATA_OBJECT, value.nData64);
}

bool NFCDataList::Add(const void* value)
{
    return AddNumber<const void*>(TDATA_POINTER, value);
}

bool NFCDataList::Set(const int index, const int value)
{
    if (index < mnSize && index >= 0)
    {
        return SetNumber(index, value);
    }

    return false;
}
bool NFCDataList::Set(const int index, const float value)
{
    if (index < mnSize && index >= 0)
    {
        return SetNumber(index, value);
    }

    return false;
}
bool NFCDataList::Set(const int index, const double value)
{
    if (index < mnSize && index >= 0)
    {
        return SetNumber(index, value);
    }

    return false;
}
bool NFCDataList::Set(const int index, const char* value)
{
    if (index < mnSize && index > 0)
    {
        TData* var = GetStack(index);
        if (var && TDATA_STRING == var->nType)
        {
            var->variantData = (std::string)value;
            return true;
        }
    }

    return false;
}
bool NFCDataList::Set(const int index, const NFIDENTID& value)
{
    if (index < mnSize && index >= 0)
    {
        return SetNumber<NFINT64>(index, value.nData64);
    }

    return false;
}

bool NFCDataList::Set(const int index, const void* value)
{
    if (index < mnSize && index >= 0)
    {
        return SetNumber(index, value);
    }

    return false;
}

int NFCDataList::Int(const int index) const
{
    if (index < mnSize && index >= 0)
    {
        return NumberVal<int>(index);
    }

    return 0;
}

float NFCDataList::Float(const int index) const
{
    if (index < mnSize && index >= 0)
    {
        return NumberVal<float>(index);
    }

    return 0.0f;
}


double NFCDataList::Double(const int index) const
{
    if (index < mnSize && index >= 0)
    {
        return NumberVal<double>(index);
    }

    return 0.0f;
}

const std::string& NFCDataList::String(const int index) const
{
    if (index < mnSize && index >= 0)
    {
        const TData* var = GetStackConst(index);
        if (var && TDATA_STRING == var->nType)
        {
            return boost::get<const std::string&>(var->variantData);
        }
    }

    return NULL_STR;
    //return StringValEx(index);
}

NFIDENTID NFCDataList::Object(const int index) const
{
    if (index < mnSize && index >= 0)
    {
        return NumberVal<NFINT64>(index);
    }

    return 0;
}

void* NFCDataList::Pointer(const int index) const
{
    if (index < mnSize && index >= 0)
    {
        return NumberVal<void*>(index);
    }

    return NULL;
}

// bool NFCDataList::Split(const char* pstr, const char* pstrSplit)
// {
//     Clear();
// 
//     int nLen = strlen(pstr);
//     char* szInput = new char[nLen + 1];
//     memset(szInput, 0, nLen + 1);
// 
//     strcpy_s(szInput, nLen + 1, pstr);
// 
//     char* p = NULL;
//     p = strtok(szInput, pstrSplit);
//     while (p != NULL)
//     {
//         Add(p);
//         p = strtok(NULL, pstrSplit);
//     }
// 
//     delete[] szInput;
// 
//     return true;
// }

bool NFCDataList::Split( const char* str, const char* strSplit )
{
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
    if (index >= mnSize || index < 0)
    {
        return TDATA_UNKNOWN;
    }

    if (index < STACK_SIZE)
    {
        return mvStack[index].nType;
    }
    else
    {
        const TData* pData = GetStackConst(index);
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
    for (int i = 0; i < STACK_SIZE; i++)
    {
        mvStack[i].nType = TDATA_UNKNOWN;
    }

    for (int i = 0; i < mvList.size(); i++)
    {
        TData* data = mvList[i];
        delete[] data;
        data = NULL;
    }

    mnOrder = 0;
    mnSize = 0;
    mnCapacity = STACK_SIZE;
    mnNextOrderCapacity = mnCapacity * 2;

    mvList.clear();
}

bool NFCDataList::IsEmpty() const
{
    return (0 == mnSize);
}

int NFCDataList::GetCount() const
{
    return mnSize;
}

void NFCDataList::InnerAppendEx(const NFIDataList& src, const int start, const int end)
{
    for (int i = start; i < end; ++i)
    {
        TDATA_TYPE vType = src.Type(i);
        switch (vType)
        {
            case TDATA_INT:
                AddNumber<int>(vType, src.NumberVal<int>(i));
                break;
            case TDATA_FLOAT:
                AddNumber<float>(vType, src.NumberVal<float>(i));
                break;
            case TDATA_DOUBLE:
                AddNumber<double>(vType, src.NumberVal<double>(i));
                break;
            case TDATA_STRING:
                Add(src.String(i).c_str());
                break;
            case TDATA_OBJECT:
                AddNumber<NFINT64>(vType, src.NumberVal<NFINT64>(i));
                break;
            case TDATA_POINTER:
                AddNumber<void*>(vType, src.NumberVal<void*>(i));
                break;
            default:
                //Assert(0);
                break;
        }
    }
}

std::string NFCDataList::StringValEx(const int index, const bool bForce) const
{
    if (index < mnSize && index >= 0)
    {
        TDATA_TYPE type =  Type(index);
        if (type == TDATA_STRING)
        {
            return String(index);
        }

        const TData* var = GetStackConst(index);
        if (var)
        {
            return boost::lexical_cast<std::string>(var->variantData);
        }
    }

    return NULL_STR;
}

bool NFCDataList::ToString(OUT std::string& str, const char* strSplit)
{
    for (int i = 0; i < GetCount(); ++i)
    {
        std::string strVal = StringValEx(i, true);
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