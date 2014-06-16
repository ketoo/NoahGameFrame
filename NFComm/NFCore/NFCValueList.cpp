// -------------------------------------------------------------------------
//    @FileName         :    NFCValueList.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-03-01
//    @Module           :    NFCValueList
//
// -------------------------------------------------------------------------

#include "NFCValueList.h"

NFCValueList::NFCValueList()
{
    mnCapacity = STACK_SIZE;
    mnNextOrderCapacity = mnCapacity * 2;
    mnSize = 0;
    mnOrder = 0;
    Clear();

}

NFCValueList::NFCValueList(const char* str, const char* strSplit)
{
    mnSize = 0;
    mnOrder = 0;
    mnCapacity = STACK_SIZE;
    mnNextOrderCapacity = mnCapacity * 2;

    Clear();

    Split(str, strSplit);
}

NFCValueList::NFCValueList(const NFIValueList& src)
{
    mnSize = 0;
    mnOrder = 0;
    mnCapacity = STACK_SIZE;
    mnNextOrderCapacity = mnCapacity * 2;

    Clear();

    InnerAppendEx(src, 0, src.GetCount());
}

NFCValueList::~NFCValueList()
{
    Clear();
};

NFCValueList& NFCValueList::operator=(const NFIValueList& src)
{
    Clear();
    InnerAppendEx(src, 0, src.GetCount());

    return *this;
}

// 添加
bool NFCValueList::Append(const NFIValueList& src, const int start, const int count)
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

bool NFCValueList::Append(const NFIValueList::VarData& varData)
{
    if (varData.nType <= VTYPE_UNKNOWN
        || varData.nType >= VTYPE_MAX)
    {
        return false;
    }

    VarData* pVar = GetStack(mnSize);
    if (pVar)
    {
        pVar->nType = varData.nType;
        pVar->variantData = varData.variantData;
        mnSize++;

        return true;
    }

    return false;
}

bool NFCValueList::AddInt(const int value)
{
    return NFIValueList::AddNumber<int>(VTYPE_INT, value);
}

bool NFCValueList::AddFloat(const float value)
{
    return AddNumber<float>(VTYPE_FLOAT, value);
}

bool NFCValueList::AddDouble(const double value)
{
    return AddNumber<double>(VTYPE_DOUBLE, value);
}

bool NFCValueList::AddString(const char* value)
{
    VarData* pVar = GetStack(mnSize);
    if (pVar)
    {
        pVar->nType = VTYPE_STRING;
        pVar->variantData = (std::string)value;
        mnSize++;
    }

    return true;
    //return AddStringEx(value);
}

bool NFCValueList::AddObject(const NFIDENTID& value)
{
    return AddNumber<NFINT64>(VTYPE_OBJECT, value.nData64);
}

bool NFCValueList::AddPointer(const void* value)
{
    return AddNumber<const void*>(VTYPE_POINTER, value);
}

bool NFCValueList::SetInt(const int index, const int value)
{
    return SetNumber(index, value);
}
bool NFCValueList::SetFloat(const int index, const float value)
{
    return SetNumber(index, value);
}
bool NFCValueList::SetDouble(const int index, const double value)
{
    return SetNumber(index, value);
}
bool NFCValueList::SetString(const int index, const char* value)
{
    if (index < mnSize && index > 0)
    {
        VarData* var = GetStack(index);
        if (var && VTYPE_STRING == var->nType)
        {
            var->variantData = (std::string)value;
            return true;
        }
    }

    return false;
}
bool NFCValueList::SetObject(const int index, const NFIDENTID& value)
{
    return SetNumber<NFINT64>(index, value.nData64);
}

bool NFCValueList::SetPointer(const int index, const void* value)
{
    return SetNumber(index, value);
}

int NFCValueList::IntVal(const int index) const
{
    return NumberVal<int>(index);
}

float NFCValueList::FloatVal(const int index) const
{
    return NumberVal<float>(index);
}


double NFCValueList::DoubleVal(const int index) const
{
    return NumberVal<double>(index);
}

const std::string& NFCValueList::StringVal(const int index) const
{
    if (index < mnSize)
    {
        const VarData* var = GetStackConst(index);
        if (var && VTYPE_STRING == var->nType)
        {
            return boost::get<const std::string&>(var->variantData);
        }
    }

    return NULL_STR;
    //return StringValEx(index);
}

NFIDENTID NFCValueList::ObjectVal(const int index) const
{
    return NumberVal<NFINT64>(index);
}

void* NFCValueList::PointerVal(const int index) const
{
    return NumberVal<void*>(index);
}

// bool NFCValueList::Split(const char* pstr, const char* pstrSplit)
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
//         AddString(p);
//         p = strtok(NULL, pstrSplit);
//     }
// 
//     delete[] szInput;
// 
//     return true;
// }

bool NFCValueList::Split( const char* str, const char* strSplit )
{
	std::string strData(str);
    if (strData.empty())
    {
        return true;
    }

	std::string temstrSplit(strSplit);
	std::string::size_type pos;
	strData += temstrSplit;
	int size = strData.length();

	for (int i = 0; i < size; i++)
	{
		pos = int(strData.find(temstrSplit, i));
		if (pos < size)
		{
			std::string strSub = strData.substr(i, pos - i);
			AddString(strSub.c_str());

			i = pos + temstrSplit.size() - 1;
		}
	}

	return true;
}

VARIANT_TYPE NFCValueList::Type(const int index) const
{
    if (index >= mnSize || index < 0)
    {
        return VTYPE_UNKNOWN;
    }

    if (index < STACK_SIZE)
    {
        return mvStack[index].nType;
    }
    else
    {
        const VarData* pData = GetStackConst(index);
        if (pData)
        {
            return pData->nType;
        }
    }

    return VTYPE_UNKNOWN;
}

bool NFCValueList::TypeEx(const int nType, ...) const
{
    bool bRet = true;

    if (VTYPE_UNKNOWN == nType)
    {
        bRet = false;
        return bRet;
    }

    VARIANT_TYPE pareType = (VARIANT_TYPE)nType;
    va_list arg_ptr;
    va_start(arg_ptr, nType);
    int index = 0;

    while (pareType != VTYPE_UNKNOWN)
    {
        //比较
        VARIANT_TYPE varType = Type(index);
        if (varType != pareType)
        {
            bRet = false;
            break;
        }

        ++index;
        pareType = (VARIANT_TYPE)va_arg(arg_ptr, int);   //获取下一个参数
    }

    va_end(arg_ptr); //结束

    return bRet;
}

bool NFCValueList::Concat(const NFIValueList& src)
{
    InnerAppendEx(src, 0, src.GetCount());
    return true;
}

void NFCValueList::Clear()
{
    for (int i = 0; i < STACK_SIZE; i++)
    {
        mvStack[i].nType = VTYPE_UNKNOWN;
    }

    for (int i = 0; i < mvList.size(); i++)
    {
        VarData* data = mvList[i];
        delete[] data;
        data = NULL;
    }

    mnOrder = 0;
    mnSize = 0;
    mnCapacity = STACK_SIZE;
    mnNextOrderCapacity = mnCapacity * 2;

    mvList.clear();
}

bool NFCValueList::IsEmpty() const
{
    return (0 == mnSize);
}

int NFCValueList::GetCount() const
{
    return mnSize;
}

void NFCValueList::InnerAppendEx(const NFIValueList& src, const int start, const int end)
{
    for (int i = start; i < end; ++i)
    {
        VARIANT_TYPE vType = src.Type(i);
        switch (vType)
        {
            case VTYPE_INT:
                AddNumber<int>(vType, src.NumberVal<int>(i));
                break;
            case VTYPE_FLOAT:
                AddNumber<float>(vType, src.NumberVal<float>(i));
                break;
            case VTYPE_DOUBLE:
                AddNumber<double>(vType, src.NumberVal<double>(i));
                break;
            case VTYPE_STRING:
                AddString(src.StringVal(i).c_str());
                break;
            case VTYPE_OBJECT:
                AddNumber<NFINT64>(vType, src.NumberVal<NFINT64>(i));
                break;
            case VTYPE_POINTER:
                AddNumber<void*>(vType, src.NumberVal<void*>(i));
                break;
            default:
                //Assert(0);
                break;
        }
    }
}

std::string NFCValueList::StringValEx(const int index, const bool bForce) const
{
    if (index < mnSize && index >= 0)
    {
        VARIANT_TYPE type =  Type(index);
        if (type == VTYPE_STRING)
        {
            return StringVal(index);
        }

        const VarData* var = GetStackConst(index);
        if (var)
        {
            return boost::lexical_cast<std::string>(var->variantData);
        }
    }

    return NULL_STR;
}

bool NFCValueList::ToString(OUT std::string& str, const char* strSplit)
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