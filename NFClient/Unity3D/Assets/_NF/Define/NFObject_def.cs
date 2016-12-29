using System;


public class NFObject_def
{
	public NFObject_def ()
	{
	}
	
	//性别
	
	public enum NFSexType
	{
		NST_MAN,
		NST_FEMAN,
		NST_MAX,
	};

	//职业1:反抗者(刀客)2先驱者(枪客)3:征服者(弓手)4:术士(医生)5格斗家(刺客)
	public enum NFJobType
	{
		NJT_1,
		NJT_2,
		NJT_3,
		NJT_4,
		NJT_MAX,
	};
	//种族
	
	public enum NFRaceType
	{
		NRT_1,
		NRT_2,
		NRT_3,
		NRT_4,
		NRT_MAX,
	};
	//阵营
	
	public enum NFCampType
	{
		NCT_1,
		NCT_2,
		NCT_3,
		NCT_MAX,
	};
	
	
	
	//////////////////////////////////////////////////////////////////////////
	//0呆着1跑2走3闪烁4技能飞移5死6平常技能中
	
	public enum NFObjectStateType
	{
		NOST_IDEL,
		NOST_RUN,
		NOST_WALK,
		NOST_WILK,
		NOST_SKILLFLY,
		NOST_DEAD,
		NOST_SKILLUSE,
	};

}

