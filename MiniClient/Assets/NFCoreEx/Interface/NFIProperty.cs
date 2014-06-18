using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace NFCoreEx
{
    public abstract class NFIProperty
    {
	public delegate void PropertyEventHandler(NFIDENTID self, string strProperty, NFIValueList oldVar, NFIValueList newVar);

    public abstract void SetValue(NFIValueList varData);

    public abstract NFIValueList GetValue();

    public abstract string GetKey();
		
	public abstract NFIValueList.VARIANT_TYPE GetType();
		
    public abstract int QueryInt();

    public abstract float QueryFloat();

    public abstract double QueryDouble();

    public abstract string QueryString();

    public abstract NFIDENTID QueryObject();

	public abstract bool SetInt(int value);

	public abstract bool SetFloat(float value);

	public abstract bool SetDouble(double value);

	public abstract bool SetString(string value);

	public abstract bool SetObject(NFIDENTID value);

	public abstract void RegisterCallback(PropertyEventHandler handler);
    }
}