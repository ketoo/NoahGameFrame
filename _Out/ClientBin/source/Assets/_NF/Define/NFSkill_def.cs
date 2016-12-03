using System;


public class NFSkill_def
{
	public NFSkill_def()
	{
	}
	
	public enum EGameSkillType
	{
		EGST_BRIEF = 0,//普通0短刃类
		
		EGST_NORMAL_BULLET = 1,//1普通子弹类(不穿透)
		
		EGST_NORMAL_BULLET_PIERCE = 2,//2普通子弹类穿
		
		EGST_BULLET_SINGLE = 3,//3普通子弹类单体目标范围(每人头顶闪电那样)
		
		EGST_BULLET_POSITION = 4,//3普通子弹类爆炸目标范围(火矛从天而降那样position)
		
	}
	
	//攻击结果类型
	
	public enum EGameSkillResultsType
	{
		EART_MISS = 0,        //MISS
		
	    EART_NORMAL = 1,        //普通攻击正常
		
	    EART_THUMP_BACK= 2,      //重击(击退)
		
		EART_THUMP_DOWN = 3,      //重击(击倒)
		
	    EART_RESIST = 4,        //格挡
		
	    EART_SUBSECTION2 = 5,   //分成2次连击
		
	    EART_KILLEDMISS = 6,	//怪物被打没了。。。只能在怪物打了这下就死的情况下才用,客户端先隐藏
		
	};
	
	public enum NFSkillTestSkillResult
	{
		//返回0:可以 1:距离不够 2:CD不到
		
		NFSTSR_OK,
		NFSTSR_DISTANCE,
		NFSTSR_CD,
	};

}

