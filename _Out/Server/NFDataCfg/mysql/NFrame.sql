CREATE TABLE `BB_Build` (
	`ID` varchar(128) NOT NULL,
	PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;



CREATE TABLE `BB_Player` (
	`ID` varchar(128) NOT NULL,
	PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

ALTER TABLE `BB_Player` ADD `RoleName` varchar(128) DEFAULT '' COMMENT '角色名';
ALTER TABLE `BB_Player` ADD `Sex` bigint(11) DEFAULT '0' COMMENT '性别';
ALTER TABLE `BB_Player` ADD `Race` bigint(11) DEFAULT '0' COMMENT '种族';
ALTER TABLE `BB_Player` ADD `Camp` bigint(11) DEFAULT '0' COMMENT '阵营';
ALTER TABLE `BB_Player` ADD `LastContainerID` bigint(11) DEFAULT '0' COMMENT '玩家下线的时候需要保存上次在线的场景';
ALTER TABLE `BB_Player` ADD `Level` bigint(11) DEFAULT '0' COMMENT '等级,属性名不能超过20字符,重要的是，分层的属性，不能保存，因为表会自动保存';
ALTER TABLE `BB_Player` ADD `Gold` bigint(11) DEFAULT '0' COMMENT '拥有的金钱';
ALTER TABLE `BB_Player` ADD `Stone` bigint(11) DEFAULT '0' COMMENT '拥有的石头';
ALTER TABLE `BB_Player` ADD `Steel` bigint(11) DEFAULT '0' COMMENT '拥有的铁';
ALTER TABLE `BB_Player` ADD `Diamond` bigint(11) DEFAULT '0' COMMENT '拥有的钻石';
ALTER TABLE `BB_Player` ADD `LoadPropertyFinish` bigint(11) DEFAULT '0' COMMENT '';
ALTER TABLE `BB_Player` ADD `BuildingList` BLOB COMMENT '';
ALTER TABLE `BB_Player` ADD `BuildingProduce` BLOB COMMENT '';


CREATE TABLE `Buff` (
	`ID` varchar(128) NOT NULL,
	PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

ALTER TABLE `Buff` ADD `EffectType` bigint(11) DEFAULT '0' COMMENT '影响属性类型(效果类型)  生命，法力(可组合,叠加)';
ALTER TABLE `Buff` ADD `EffectValueType` bigint(11) DEFAULT '0' COMMENT '影响值类型 百分比 绝对值--如果是控制属性开关，则只能是绝对值';
ALTER TABLE `Buff` ADD `EffectValueReferType` bigint(11) DEFAULT '0' COMMENT '影响值参考类型';
ALTER TABLE `Buff` ADD `EffectTimeValue` bigint(11) DEFAULT '0' COMMENT '总共产生几次';
ALTER TABLE `Buff` ADD `EffectTimeInterval` float(11,3) DEFAULT '0' COMMENT '每次间隔时间0.5f倍数';
ALTER TABLE `Buff` ADD `WashGroupID` bigint(11) DEFAULT '0' COMMENT '每次间隔时间0.5f倍数';
ALTER TABLE `Buff` ADD `ReverseReferType` bigint(11) DEFAULT '0' COMMENT '是否还原，不还原的在计算的时候不能保存在runTimeInfo,只能累计在最终值内';
ALTER TABLE `Buff` ADD `EffectClearOnDead` bigint(11) DEFAULT '0' COMMENT '死亡是否清除';
ALTER TABLE `Buff` ADD `DownSaveType` bigint(11) DEFAULT '0' COMMENT '下线是否保存';


CREATE TABLE `ChatGroup` (
	`ID` varchar(128) NOT NULL,
	PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

ALTER TABLE `ChatGroup` ADD `Name` varchar(128) DEFAULT '' COMMENT '组名字';
ALTER TABLE `ChatGroup` ADD `CreateObject` varchar(128) DEFAULT '' COMMENT '创建者';
ALTER TABLE `ChatGroup` ADD `GroupMemberList` BLOB COMMENT '';
ALTER TABLE `ChatGroup` ADD `ChatList` BLOB COMMENT '';


CREATE TABLE `Cost` (
	`ID` varchar(128) NOT NULL,
	PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;



CREATE TABLE `EffectData` (
	`ID` varchar(128) NOT NULL,
	PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;



CREATE TABLE `Equip` (
	`ID` varchar(128) NOT NULL,
	PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;



CREATE TABLE `Guild` (
	`ID` varchar(128) NOT NULL,
	PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

ALTER TABLE `Guild` ADD `Name` varchar(128) DEFAULT '' COMMENT '工会名';
ALTER TABLE `Guild` ADD `PresidentID` varchar(128) DEFAULT '' COMMENT '会长ID';
ALTER TABLE `Guild` ADD `PresidentName` varchar(128) DEFAULT '' COMMENT '会长名';
ALTER TABLE `Guild` ADD `Rank` bigint(11) DEFAULT '0' COMMENT '公会排名';
ALTER TABLE `Guild` ADD `GuildAD` varchar(128) DEFAULT '' COMMENT '工会公告';
ALTER TABLE `Guild` ADD `GuildDesc` varchar(128) DEFAULT '' COMMENT '工会描述';
ALTER TABLE `Guild` ADD `GuildMoney` bigint(11) DEFAULT '0' COMMENT '工会财富';
ALTER TABLE `Guild` ADD `GuildLevel` bigint(11) DEFAULT '0' COMMENT '工会等级';
ALTER TABLE `Guild` ADD `GuildContinueDay` bigint(11) DEFAULT '0' COMMENT '工会霸占排行榜天数';
ALTER TABLE `Guild` ADD `GuilID` varchar(128) DEFAULT '' COMMENT '工会ID';
ALTER TABLE `Guild` ADD `GuilIDIcon` bigint(11) DEFAULT '0' COMMENT '公会头像';
ALTER TABLE `Guild` ADD `GuildMemeberCount` bigint(11) DEFAULT '0' COMMENT '成员数量(增加个字段，不然遍历Record太麻烦)';
ALTER TABLE `Guild` ADD `GuildMemeberMaxCount` bigint(11) DEFAULT '0' COMMENT '成员最大数量';
ALTER TABLE `Guild` ADD `GuildHonor` bigint(11) DEFAULT '0' COMMENT '公会荣誉';
ALTER TABLE `Guild` ADD `GuildBoss` BLOB COMMENT 'guild';
ALTER TABLE `Guild` ADD `GuildMemberList` BLOB COMMENT 'guild';
ALTER TABLE `Guild` ADD `GuildEvent` BLOB COMMENT 'guild';
ALTER TABLE `Guild` ADD `GuildHouse` BLOB COMMENT 'guild';
ALTER TABLE `Guild` ADD `GuildSkill` BLOB COMMENT 'guild';


CREATE TABLE `GuildName` (
	`ID` varchar(128) NOT NULL,
	PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

ALTER TABLE `GuildName` ADD `GuildID` varchar(128) DEFAULT '' COMMENT '工会ID';


CREATE TABLE `InitProperty` (
	`ID` varchar(128) NOT NULL,
	PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;



CREATE TABLE `Item` (
	`ID` varchar(128) NOT NULL,
	PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;



CREATE TABLE `Language` (
	`ID` varchar(128) NOT NULL,
	PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;



CREATE TABLE `NPC` (
	`ID` varchar(128) NOT NULL,
	PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

ALTER TABLE `NPC` ADD `EXP` bigint(11) DEFAULT '0' COMMENT '经验获得,如果是怪物，则是掉落经验';
ALTER TABLE `NPC` ADD `SP` bigint(11) DEFAULT '0' COMMENT '体力';
ALTER TABLE `NPC` ADD `MP` bigint(11) DEFAULT '0' COMMENT '法力值';
ALTER TABLE `NPC` ADD `Money` bigint(11) DEFAULT '0' COMMENT '钱';
ALTER TABLE `NPC` ADD `X` float(11,3) DEFAULT '0' COMMENT '';
ALTER TABLE `NPC` ADD `Y` float(11,3) DEFAULT '0' COMMENT '';
ALTER TABLE `NPC` ADD `Z` float(11,3) DEFAULT '0' COMMENT '';
ALTER TABLE `NPC` ADD `PosList` BLOB COMMENT '';


CREATE TABLE `Player` (
	`ID` varchar(128) NOT NULL,
	PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

ALTER TABLE `Player` ADD `Name` varchar(128) DEFAULT '' COMMENT '角色名';
ALTER TABLE `Player` ADD `Sex` bigint(11) DEFAULT '0' COMMENT '性别';
ALTER TABLE `Player` ADD `Race` bigint(11) DEFAULT '0' COMMENT '种族';
ALTER TABLE `Player` ADD `Camp` bigint(11) DEFAULT '0' COMMENT '阵营';
ALTER TABLE `Player` ADD `LastContainerID` bigint(11) DEFAULT '0' COMMENT '玩家下线的时候需要保存上次在线的场景';
ALTER TABLE `Player` ADD `Level` bigint(11) DEFAULT '0' COMMENT '等级,属性名不能超过20字符,重要的是，分层的属性，不能保存，因为表会自动保存';
ALTER TABLE `Player` ADD `ShowName` varchar(128) DEFAULT '' COMMENT '显示的名字';
ALTER TABLE `Player` ADD `PrefabPath` varchar(128) DEFAULT '' COMMENT '预设路径';
ALTER TABLE `Player` ADD `FirstTarget` varchar(128) DEFAULT '' COMMENT '首要目标';
ALTER TABLE `Player` ADD `State` bigint(11) DEFAULT '0' COMMENT '0呆着1跑2走3闪烁4技能飞移5死6平常技能中';
ALTER TABLE `Player` ADD `LogicState` bigint(11) DEFAULT '0' COMMENT '逻辑状态，或者说模块独占状态';
ALTER TABLE `Player` ADD `CharType` bigint(11) DEFAULT '0' COMMENT '角色类型';
ALTER TABLE `Player` ADD `Job` bigint(11) DEFAULT '0' COMMENT '职业';
ALTER TABLE `Player` ADD `EXP` bigint(11) DEFAULT '0' COMMENT '经验获得,如果是怪物，则是掉落经验';
ALTER TABLE `Player` ADD `HP` bigint(11) DEFAULT '0' COMMENT '生命值';
ALTER TABLE `Player` ADD `SP` bigint(11) DEFAULT '0' COMMENT '体力';
ALTER TABLE `Player` ADD `MP` bigint(11) DEFAULT '0' COMMENT '法力值';
ALTER TABLE `Player` ADD `Money` bigint(11) DEFAULT '0' COMMENT '钱';
ALTER TABLE `Player` ADD `Account` varchar(128) DEFAULT '' COMMENT '玩家的帐号';
ALTER TABLE `Player` ADD `ConnectKey` varchar(128) DEFAULT '' COMMENT '玩家连接服务器的KEY';
ALTER TABLE `Player` ADD `Gold` bigint(11) DEFAULT '0' COMMENT '玩家金钱';
ALTER TABLE `Player` ADD `MAXEXP` bigint(11) DEFAULT '0' COMMENT '升级需要的经验';
ALTER TABLE `Player` ADD `DEAD_COUNT` bigint(11) DEFAULT '0' COMMENT '死亡次数--需要保存的都是因为不分层的属性，分层属性不需要保存，保存各层的值即可';
ALTER TABLE `Player` ADD `RELIVE_SOUL` bigint(11) DEFAULT '0' COMMENT '转生次数';
ALTER TABLE `Player` ADD `GUILD_NAME` varchar(128) DEFAULT '' COMMENT '工会ID名字';
ALTER TABLE `Player` ADD `SPOUSE_NAME` varchar(128) DEFAULT '' COMMENT '配偶名字';
ALTER TABLE `Player` ADD `TEAM_CARD` varchar(128) DEFAULT '' COMMENT '佩戴的队旗卡ID';
ALTER TABLE `Player` ADD `PVP_ATK_CARD` varchar(128) DEFAULT '' COMMENT '佩戴的PVP攻击卡';
ALTER TABLE `Player` ADD `PVP_DEF_CARD` varchar(128) DEFAULT '' COMMENT '佩戴的PVP防御卡';
ALTER TABLE `Player` ADD `LUCKLY_CARD` varchar(128) DEFAULT '' COMMENT '佩戴的幸运卡ID';
ALTER TABLE `Player` ADD `ATK_PVP` bigint(11) DEFAULT '0' COMMENT 'PVP攻击力';
ALTER TABLE `Player` ADD `DEF_PVP` bigint(11) DEFAULT '0' COMMENT 'PVP防御力';
ALTER TABLE `Player` ADD `CreditValue` bigint(11) DEFAULT '0' COMMENT '信用值';
ALTER TABLE `Player` ADD `SoulValue` bigint(11) DEFAULT '0' COMMENT '武魂值';
ALTER TABLE `Player` ADD `OnlineCount` bigint(11) DEFAULT '0' COMMENT '上线次数';
ALTER TABLE `Player` ADD `TotalTime` bigint(11) DEFAULT '0' COMMENT '在线时间';
ALTER TABLE `Player` ADD `Citizenship` bigint(11) DEFAULT '0' COMMENT '创建帐号服务器';
ALTER TABLE `Player` ADD `LastOfflineTime` varchar(128) DEFAULT '' COMMENT '上次下线时间';
ALTER TABLE `Player` ADD `OnlineTime` varchar(128) DEFAULT '' COMMENT '本次上线时间';
ALTER TABLE `Player` ADD `TotalLineTime` varchar(128) DEFAULT '' COMMENT '今日在线时长{截至至本次上线时间}';
ALTER TABLE `Player` ADD `GMLevel` bigint(11) DEFAULT '0' COMMENT 'GM等级';
ALTER TABLE `Player` ADD `LastAttacker` varchar(128) DEFAULT '' COMMENT '';
ALTER TABLE `Player` ADD `X` float(11,3) DEFAULT '0' COMMENT '';
ALTER TABLE `Player` ADD `Y` float(11,3) DEFAULT '0' COMMENT '';
ALTER TABLE `Player` ADD `Z` float(11,3) DEFAULT '0' COMMENT '';
ALTER TABLE `Player` ADD `TargetX` float(11,3) DEFAULT '0' COMMENT '';
ALTER TABLE `Player` ADD `TargetY` float(11,3) DEFAULT '0' COMMENT '';
ALTER TABLE `Player` ADD `TargetZ` float(11,3) DEFAULT '0' COMMENT '';
ALTER TABLE `Player` ADD `PathStep` bigint(11) DEFAULT '0' COMMENT '';
ALTER TABLE `Player` ADD `GameID` bigint(11) DEFAULT '0' COMMENT '登陆的服ID';
ALTER TABLE `Player` ADD `GateID` bigint(11) DEFAULT '0' COMMENT '登陆的网关ID';
ALTER TABLE `Player` ADD `GuildID` varchar(128) DEFAULT '' COMMENT '公会ID';
ALTER TABLE `Player` ADD `PlayerViewItem` BLOB COMMENT '';
ALTER TABLE `Player` ADD `BagItemList` BLOB COMMENT '普通背包';
ALTER TABLE `Player` ADD `EctypeList` BLOB COMMENT '副本列表';
ALTER TABLE `Player` ADD `DropItemList` BLOB COMMENT '怪物掉落表';
ALTER TABLE `Player` ADD `SkillTable` BLOB COMMENT '';
ALTER TABLE `Player` ADD `TaskMonsterList` BLOB COMMENT '任务杀怪表';
ALTER TABLE `Player` ADD `TaskList` BLOB COMMENT '任务表';


CREATE TABLE `Scene` (
	`ID` varchar(128) NOT NULL,
	PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;



CREATE TABLE `Server` (
	`ID` varchar(128) NOT NULL,
	PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;



CREATE TABLE `Shop` (
	`ID` varchar(128) NOT NULL,
	PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

ALTER TABLE `Shop` ADD `Type` bigint(11) DEFAULT '0' COMMENT '';
ALTER TABLE `Shop` ADD `ItemID` varchar(128) DEFAULT '' COMMENT '';
ALTER TABLE `Shop` ADD `Gold` bigint(11) DEFAULT '0' COMMENT '';
ALTER TABLE `Shop` ADD `Steel` bigint(11) DEFAULT '0' COMMENT '';
ALTER TABLE `Shop` ADD `Stone` bigint(11) DEFAULT '0' COMMENT '';
ALTER TABLE `Shop` ADD `Diamond` bigint(11) DEFAULT '0' COMMENT '';
ALTER TABLE `Shop` ADD `Level` bigint(11) DEFAULT '0' COMMENT '';


CREATE TABLE `Skill` (
	`ID` varchar(128) NOT NULL,
	PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;



CREATE TABLE `SkillRef` (
	`ID` varchar(128) NOT NULL,
	PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;



CREATE TABLE `SqlServer` (
	`ID` varchar(128) NOT NULL,
	PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;



CREATE TABLE `StateFuncResources` (
	`ID` varchar(128) NOT NULL,
	PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

ALTER TABLE `StateFuncResources` ADD `Atlas_ResID` varchar(128) DEFAULT '' COMMENT '';


CREATE TABLE `StateFunction` (
	`ID` varchar(128) NOT NULL,
	PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

ALTER TABLE `StateFunction` ADD `EFT_INFO` bigint(11) DEFAULT '0' COMMENT '';
ALTER TABLE `StateFunction` ADD `EFT_BOOOST` bigint(11) DEFAULT '0' COMMENT '';
ALTER TABLE `StateFunction` ADD `EFT_LVLUP` bigint(11) DEFAULT '0' COMMENT '';
ALTER TABLE `StateFunction` ADD `EFT_CREATE_SOLDER` bigint(11) DEFAULT '0' COMMENT '';
ALTER TABLE `StateFunction` ADD `EFT_CREATE_SPEEL` bigint(11) DEFAULT '0' COMMENT '';
ALTER TABLE `StateFunction` ADD `EFT_RESEARCH` bigint(11) DEFAULT '0' COMMENT '';
ALTER TABLE `StateFunction` ADD `EFT_COLLECT_GOLD` bigint(11) DEFAULT '0' COMMENT '';
ALTER TABLE `StateFunction` ADD `EFT_COLLECT_STONE` bigint(11) DEFAULT '0' COMMENT '';
ALTER TABLE `StateFunction` ADD `EFT_COLLECT_STEEL` bigint(11) DEFAULT '0' COMMENT '';
ALTER TABLE `StateFunction` ADD `EFT_COLLECT_DIAMOND` bigint(11) DEFAULT '0' COMMENT '';
ALTER TABLE `StateFunction` ADD `EFT_SELL` bigint(11) DEFAULT '0' COMMENT '';
ALTER TABLE `StateFunction` ADD `EFT_REPAIR` bigint(11) DEFAULT '0' COMMENT '';
ALTER TABLE `StateFunction` ADD `EFT_CANCEL` bigint(11) DEFAULT '0' COMMENT '';
ALTER TABLE `StateFunction` ADD `EFT_FINISH` bigint(11) DEFAULT '0' COMMENT '';


CREATE TABLE `Task` (
	`ID` varchar(128) NOT NULL,
	PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;



