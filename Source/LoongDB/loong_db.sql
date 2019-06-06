CREATE TABLE `account_common` (
	`AccountID` int(10) unsigned NOT NULL COMMENT '�˺�ID',
	`AccountName` char(36) NOT NULL COMMENT '�˺���',
	`SafeCodeCrc` int(10) unsigned NOT NULL DEFAULT '4294967295' COMMENT '��ȫ��Crc32',
	`ResetTime` int(10) unsigned NOT NULL DEFAULT '4294967295' COMMENT '���ð�ȫ��',
	`BagPsdCrc` int(10) unsigned NOT NULL DEFAULT '4294967295' COMMENT '��������CRCֵ',
	`BaiBaoYuanBao` int(10) NOT NULL DEFAULT '0' COMMENT '�ٱ�����Ԫ��',
	`WareSize` smallint(6) NOT NULL DEFAULT '24' COMMENT '�ֿ��С,Ĭ��ֵΪ24',
	`WareSilver` bigint(20) NOT NULL DEFAULT '0' COMMENT '�ֿ��н�Ǯ��',
	PRIMARY KEY (`AccountID`),
	KEY `AccountName` (`AccountName`)
	) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='��Ϸ�������˺��������';

CREATE TABLE `roledata` (
	`AccountID` int(10) unsigned NOT NULL,
	
	`RoleID` int(10) unsigned NOT NULL,
	`RoleName` varchar(32) NOT NULL,
	`RoleNameCrc` int(10) unsigned NOT NULL,

	`Sex` tinyint(3) NOT NULL COMMENT '�Ա�(0.Ů;1.��)',

	`HairModelID` tinyint(3) unsigned NOT NULL COMMENT '����',
	`HairColorID` tinyint(3) unsigned NOT NULL COMMENT '��ɫ',
	`FaceModelID` tinyint(3) unsigned NOT NULL COMMENT '�沿���',
	`FaceDetailID` tinyint(3) unsigned NOT NULL COMMENT '�沿ϸ��',
	`DressModelID` tinyint(3) unsigned NOT NULL COMMENT '��װ��ʽ',
	
	`AvatarEquip` tinyblob COMMENT 'װ�����',
	`DisplaySet` tinyint(3) NOT NULL default '1' COMMENT '�����ʾ����(Ĭ����ʾʱװģʽ)',

	`MapID` int(10) unsigned NOT NULL COMMENT '��ɫ���ڵ�ͼID',
	`X` float(10) NOT NULL COMMENT '��ɫ���ڵ�ͼ�е�X����',
	`Y` float(10) NOT NULL COMMENT '��ɫ���ڵ�ͼ�е�Y����',
	`Z` float(10) NOT NULL COMMENT '��ɫ���ڵ�ͼ�е�Z����',
	
	`FaceX` float(10) NOT NULL COMMENT '��ɫ�����X����',
	`FaceY` float(10) NOT NULL COMMENT '��ɫ�����Y����',
	`FaceZ` float(10) NOT NULL COMMENT '��ɫ�����Z����',
	
	`RebornMapID` int(10) unsigned NOT NULL default '4294967295' COMMENT '��ɫ�����ͼID',
	
	`Class` tinyint(3) NOT NULL default '1' COMMENT 'ר��ְҵ',
	`ClassEx` tinyint(3) NOT NULL default '0' COMMENT 'Ӣ��ְҵ',

	`Level` smallint(3) NOT NULL default '1' COMMENT '��ǰ�ȼ�',
	`ExpCurLevel` int(10) unsigned NOT NULL default '0' COMMENT '��ǰ�������������',
	
	`HP` mediumint(8) NOT NULL default '-1' COMMENT 'Ѫ��',
	`MP` mediumint(8) NOT NULL default '-1' COMMENT '������',
	`Rage` smallint(6) NOT NULL default '0' COMMENT 'ŭ��',
	`Endurance` smallint(6) NOT NULL default '-1' COMMENT '�־���',
	`Vitality` smallint(5) NOT NULL default '-1' COMMENT '����(0��999)',
	`Injury` mediumint(8) NOT NULL default '0' COMMENT '����',
	`Knowledge` mediumint(8) NOT NULL default '0' COMMENT '����',
	`Morale` smallint(4) NOT NULL default '100' COMMENT 'ʿ��',
	`Morality` mediumint(8) NOT NULL default '0' COMMENT '����',
	`Culture` mediumint(8) NOT NULL default '0' COMMENT '��ѧ��Ϊ',
	`Credit` mediumint(8) NOT NULL default '0' COMMENT '���ö�',
	`Identity` tinyint(3) NOT NULL default '0' COMMENT '���',
	`VIPPoint` mediumint(8) NOT NULL default '0' COMMENT '��Ա����',
	
	`AttPtAvail` smallint(6) NOT NULL default '0' COMMENT '��Ͷ���Ե�',
	`TalentPtAvail` smallint(6) NOT NULL default '0' COMMENT '��Ͷ���ʵ���',
	
	`PhysiqueAdded` smallint(6) NOT NULL default '0' COMMENT '���',
	`StrengthAdded` smallint(6) NOT NULL default '0' COMMENT '����',
	`PneumaAdded` smallint(6) NOT NULL default '0' COMMENT 'Ԫ��',
	`InnerforceAdded` smallint(6) NOT NULL default '0' COMMENT '����',
	`TechniqueAdded` smallint(6) NOT NULL default '0' COMMENT '����',
	`AgilityAdded` smallint(6) NOT NULL default '0' COMMENT '��',
	
	`Strength` mediumint(8) NOT NULL default '0' COMMENT 'ʵ��ֵ',
	`StrengthNum` smallint(6) NOT NULL default '0' COMMENT 'ʵ������',
	`Heroism` mediumint(8) NOT NULL default '0' COMMENT 'Ӣ��ֵ',
	`Wit` mediumint(8) NOT NULL default '0' COMMENT 'ı��ֵ',
	`Errantry` mediumint(8) NOT NULL default '0' COMMENT '����ֵ',
	`Valor` mediumint(8) NOT NULL default '0' COMMENT '��սֵ',
	
	`TalentType1` tinyint(2) NOT NULL default '-1' COMMENT '��������',
	`TalentType2` tinyint(2) NOT NULL default '-1' COMMENT '��������',
	`TalentType3` tinyint(2) NOT NULL default '-1' COMMENT '��������',
	`TalentType4` tinyint(2) NOT NULL default '-1' COMMENT '��������',
	
	`TalentVal1` smallint(4) NOT NULL default '0' COMMENT '����Ͷ��ֵ',
	`TalentVal2` smallint(4) NOT NULL default '0' COMMENT '����Ͷ��ֵ',
	`TalentVal3` smallint(4) NOT NULL default '0' COMMENT '����Ͷ��ֵ',
	`TalentVal4` smallint(4) NOT NULL default '0' COMMENT '����Ͷ��ֵ',
	
	`SGFlag` tinyint(1) NOT NULL default '1' COMMENT '�Ƿ�PK����',
	`CloseSGTime` char(20) NOT NULL default '2000-01-01 00:00:00' COMMENT '�ϴιر�PK������ʱ��',
	
	`BagSize` smallint(6) NOT NULL default '20' COMMENT '������С,Ĭ��Ϊ20',
	`BagGold` int(10) NOT NULL default '0' COMMENT '�����н�����',
	`BagSilver` int(10) NOT NULL default '0' COMMENT '������Ӭ����',
	`BagYuanBao` int(10) NOT NULL default '0' COMMENT '������Ԫ����',
	`ExchangeVolume` int(10) NOT NULL default '0' COMMENT '��ҽ�ɫ���������',
	
	`GuildID` int(10) unsigned NOT NULL default '4294967295' COMMENT '������ڰ���id',
	`TotalTax` int(10) NOT NULL default '0' COMMENT '��˰�ܶ�',
	
	`RemoteOpenSet` int(10) unsigned NOT NULL default '4294967295' COMMENT '�Ƿ��Զ����ҹ��������Ϣ',
	`CurTitleID` smallint(6) unsigned NOT NULL default '65535' COMMENT '��ǰ�ƺ�',
	
	`GetMallFreeTime` char(20) NOT NULL default '2000-00-00 00:00:00' COMMENT '�ϴδ��̳���ȡ�����Ʒʱ��',
	
	`CreateTime` char(20) NOT NULL COMMENT '����ʱ��',
	`LoginTime` char(20) NOT NULL default '2000-00-00 00:00:00' COMMENT '����ʱ��',
	`LogoutTime` char(20) NOT NULL default '2000-00-00 00:00:00' COMMENT '����ʱ��',
	`OnlineTime` int(10) NOT NULL default '-1' COMMENT '������ʱ��(��λ:��)',
	`CurOnlineTime` int(10) NOT NULL default '0' COMMENT '��ǰ����ʱ��(��λ:��)',
	
	`RemoveFlag` tinyint(1) NOT NULL default '0' COMMENT '�Ƿ���ɾ��״̬(��״̬��Ҳ��ᱻ������Ϸ����)',
	`RemoveTime` int(10) unsigned NOT NULL default '4294967295' COMMENT 'ɾ��ʱ��',
	`ScriptData` blob COMMENT '�ű�����',
	`TreasureSum` tinyint(3) unsigned NOT NULL DEFAULT '0' COMMENT '��ɫ����������',

        `StallLevel` tinyint(3) unsigned NOT NULL DEFAULT '1' COMMENT '̯λ�ȼ�',                                                                                                                                                                                                    
        `StallDailyExp` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '̯λ���쾭��',                                                                                                                                                                                              
        `StallCurExp` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '̯λ��ǰ�ȼ�����', 
	`StallLastTime` int(10) unsigned NOT NULL DEFAULT '4325376' COMMENT '̯λ�������ʱ��(Ĭ��2000��1��1��)',
	`Hostility` tinyint(1) unsigned NOT NULL DEFAULT '0' COMMENT '����ֵ',
	`DeadUnSetSafeGuardCountDown` smallint(5) signed NOT NULL DEFAULT '-100' COMMENT '�ر��������Ϊ0ʱ�سǸ��������ϵ�pk��������tick',
	`ItemTransportMapID` int(10) unsigned NOT NULL DEFAULT '3017298127' COMMENT '��¼ʽ���ͷ�mapid',
	`ItemTransportX` float unsigned NOT NULL DEFAULT '110000' COMMENT '��¼ʽ���ͷ�X���� 2200*50',
	`ItemTransportZ` float unsigned NOT NULL DEFAULT '110000' COMMENT '��¼ʽ���ͷ�Z���� 2200*50',
	`ItemTransportY` float unsigned NOT NULL DEFAULT '332300' COMMENT '��¼ʽ���ͷ�Y���� 6646*50',
	
	`LoverID` int(10) unsigned NOT NULL DEFAULT '4294967295' COMMENT '���˶�Ӧ��roleidû������4294967295',
	`HaveWedding` tinyint(3) unsigned NOT NULL DEFAULT '0' COMMENT '�Ƿ���й�����',
	`LastLessingTime` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '�ϴ���ȡ�����͸���ʱ��',
	`LastLessingLevel` tinyint(2) unsigned NOT NULL DEFAULT '0' COMMENT '��ȡ���˶��ټ�', 
	`NeedPrisonRevive` tinyint(3) unsigned NOT NULL DEFAULT '0' COMMENT '�Ƿ���Ҫ��������', 
	PRIMARY KEY (`RoleID`),
	KEY `AccountID` (`AccountID`),
	UNIQUE KEY `RoleNameCrc` (`RoleNameCrc`)
	) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='��ɫ���ݱ� -- ���ı�';
	

CREATE TABLE `skill` (                     
	`RoleID` int(10) unsigned NOT NULL,
	`ID` int(10) unsigned NOT NULL,
	`BiddenLevel` tinyint(3) NOT NULL default '0' COMMENT 'Ͷ��ȼ�',
	`SelfLevel` tinyint(3) NOT NULL default '0' COMMENT '���еȼ�',
	`Proficiency` mediumint(8) NOT NULL default '0' COMMENT '���������̶�',
	`CoolDown` int(10) NOT NULL COMMENT 'buff����ʱ��(��λ:����)',
	PRIMARY KEY  (`RoleID`, `ID`)
	) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='��ɫ����Ĺ���';


CREATE TABLE `buff` (                     
	`RoleID` int(10) unsigned NOT NULL COMMENT '��˭����',
	`SrcUnitID` int(10) unsigned NOT NULL COMMENT 'ԴUnit��ID',
	`SrcSkillID` int(10) unsigned NOT NULL COMMENT '���ĸ����ܲ�������ID��',
	`ItemTypeID` int(10) unsigned NOT NULL COMMENT '��Ʒ��װ��������ID',
	`ItemSerialID` bigint(10) NOT NULL COMMENT '���ĸ���Ʒ��װ������',
	
	`BuffID` int(10) unsigned NOT NULL COMMENT 'Buff ID',
	`CurTick` int(10) unsigned NOT NULL COMMENT '��ǰ����ʱ��(��λ:tick)',
	`Level` tinyint(3) NOT NULL COMMENT '�ȼ�',
	`CurLapTimes` tinyint(3) NOT NULL COMMENT '��ǰ���Ӵ���',
	
	`EffectSkillID` blob COMMENT 'Ӱ���buff�ļ���ID',

	PRIMARY KEY  (`RoleID`, `BuffID`)
	) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='��ɫ���ϵ�buff';


CREATE TABLE `title` (                     
	`RoleID` int(10) unsigned NOT NULL COMMENT '��ɫID',
	`ID` smallint(5) unsigned NOT NULL COMMENT '�ƺ�ID',
	`Count` int(10) unsigned NOT NULL COMMENT '����',
	PRIMARY KEY  (`RoleID`, `ID`)
	) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='��ɫ�ƺű�';


CREATE TABLE `item` (                                                                                                 
          `SerialNum` bigint(20) NOT NULL COMMENT '64λid',                                                                  
          `Num` smallint(5) NOT NULL DEFAULT '1' COMMENT '����',                                                            
          `TypeID` int(10) unsigned NOT NULL COMMENT '����id',                                                              
          `Bind` tinyint(4) NOT NULL COMMENT '�Ƿ��',                                                                  
          `LockStat` tinyint(4) NOT NULL DEFAULT '0' COMMENT '��Ʒ����״̬(1:����״̬)',                            
          `UseTimes` int(10) NOT NULL DEFAULT '0' COMMENT '��Ʒʹ�ô���',                                               
          `FirstGainTime` char(20) NOT NULL DEFAULT '2000-00-00 00:00:00' COMMENT '��һ����һ�øõ��ߵ�ʱ��',  
          `CreateMode` tinyint(2) NOT NULL COMMENT '������ʽ��GM��NPC���������ҵ�',                            
          `CreateID` int(10) unsigned NOT NULL COMMENT '������ʽ�ж�Ӧ��id',                                          
          `CreatorID` int(10) unsigned NOT NULL DEFAULT '4294967295' COMMENT '������id',                                   
          `CreateTime` char(20) NOT NULL COMMENT '����ʱ��',                                                              
          `OwnerID` int(10) unsigned NOT NULL COMMENT '��ɫID',                                                             
          `AccountID` int(10) unsigned NOT NULL COMMENT '�ʺ�ID(����ɫ������Ʒ��)',                                 
          `ContainerTypeID` tinyint(4) unsigned NOT NULL COMMENT '������������',                                        
          `Suffix` tinyint(4) unsigned NOT NULL COMMENT '������λ��',                                                    
          `NameID` int(10) unsigned NOT NULL DEFAULT '4294967295' COMMENT '����ID',                                         
          PRIMARY KEY (`SerialNum`),                                                                                          
          KEY `OwnerID` (`OwnerID`),                                                                                          
          KEY `AccountID` (`AccountID`,`ContainerTypeID`)                                                                     
        ) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='��Ʒ��';                                                      

	
CREATE TABLE `equip` (
	`SerialNum` bigint(20) NOT NULL,

	# װ������ʱȷ��
	`Quality` tinyint(4) unsigned NOT NULL default '0' COMMENT 'Ʒ��(255��ʾδ����)',

	`MinUseLevel` tinyint(4) unsigned NOT NULL default '0' COMMENT '�ȼ�����',
	`MaxUseLevel` tinyint(4) unsigned NOT NULL default '0' COMMENT '�ȼ�����',

	`WuHun` smallint(5) NOT NULL COMMENT '���:�ڹ��˺�������',
	`MinDmg` smallint(5) NOT NULL COMMENT '������С�˺�',
	`MaxDmg` smallint(5) NOT NULL COMMENT '��������˺�',
	`Armor` smallint(5) NOT NULL COMMENT '���߻���',

	`PotVal` int(10) NOT NULL COMMENT '��ǰװ��Ǳ��ֵ',
	`PotValUsed` int(10) NOT NULL default '0' COMMENT '�Ѿ����ĵ�װ��Ǳ��ֵ',
	# װ��Ǳ��ֵ��������
	`PotIncTimes` smallint(10) UNSIGNED DEFAULT '0' NOT NULL,
	# ��ɫƷ���������� 2009-12-2 Jason
	`PurpleQltyIdfPct` int(10) UNSIGNED DEFAULT '0' NOT NULL,

	`RoleAttEffect` tinyblob COMMENT 'һ������Ӱ��',
	
	`LongInnerID` int(10) unsigned NOT NULL default '4294967295' COMMENT '����������ID',
	`LongOuterID` int(10) unsigned NOT NULL default '4294967295' COMMENT '����������ID',

	`SpecAtt` tinyint(2) unsigned NOT NULL default '255' COMMENT '��������',

	# ʱװ����/����ʱȷ��
	`Appearance` smallint(5) NOT NULL default '0' COMMENT '����',
	`Rein` tinyint(3) unsigned NOT NULL default '0' COMMENT 'ͳ��',
	`Savvy` tinyint(3) unsigned NOT NULL default '0' COMMENT '����',
	`Fortune` tinyint(3) unsigned NOT NULL default '0' COMMENT '��Ե',
	`ColorID` tinyint(3) NOT NULL default '0' COMMENT '��ɫ����',
	
	# ����ʱӰ�������
	`AttALimMod` smallint(5) NOT NULL default '0' COMMENT 'һ����������ֵ',
	`AttALimModPct` smallint(5) NOT NULL default '0' COMMENT 'һ���������ưٷֱ�',
	
	`PosyTimes` tinyint(4) NOT NULL default '0' COMMENT '���Ĵ���',
	`PosyEffect` tinyblob COMMENT '����Ӱ��(����,����,���⹥��)',
	
	`EngraveTimes` tinyint(4) NOT NULL default '0' COMMENT '�Կ̴���',
	`EngraveAtt` tinyblob COMMENT '�Կ�Ӱ��(����:����/���;����:����)',
	
	`HoleNum` tinyint(4) NOT NULL default '0' COMMENT '��Ƕ�׸���',
	`HoleGemID` tinyblob COMMENT '5��������Ƕ�ı�ʯID',
	
	`BrandLevel` tinyint(4) NOT NULL default '0' COMMENT '��ӡ�ȼ�',
	`DerateVal` tinyblob COMMENT '�˺�����ֵ(����������ö�����˺�����˳�����,��ô�Ҳ�����)',
	
	`LongFuLevel` tinyint(4) NOT NULL default '0' COMMENT '�����ȼ�',
	`WuXing` tinyblob COMMENT '���еȼ�: ��ľˮ����',
	
	`CanCut` tinyint(2) NOT NULL default '1' COMMENT '�Ƿ�ɿ���(0:��; 1:��)',

	`FlareVal` tinyint(3) unsigned NOT NULL default '0' COMMENT '����ֵ',

	# װ������ʱȷ��
	`QltyModPct` smallint(10) NOT NULL default '0' COMMENT 'һ������Ʒ��������(-10000,10000)',
	`QltyModPctEx` smallint(10) unsigned NOT NULL default '0' COMMENT '��������Ʒ��������[0,10000)',
	`PotValModPct` smallint(10) unsigned NOT NULL DEFAULT '10000' COMMENT 'װ��Ǳ��ֵӰ��ϵ��',

	PRIMARY KEY  (`SerialNum`)
	) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='װ����(װ�� = װ���� + ��Ʒ��)';
	
	
CREATE TABLE `item_cdtime` (                                                        
               `RoleID` int(10) unsigned NOT NULL COMMENT '��ɫid',                            
               `CDTime` blob COMMENT '��ȴʱ��blob����ʽΪ��typeid����ȴʱ�䣩',  
               PRIMARY KEY (`RoleID`)                                                            
             ) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='��Ʒ&װ����ȴʱ��' ;
	
CREATE TABLE `friend` (                                              
          `RoleID` int(10) unsigned NOT NULL COMMENT '���id',             
          `FriendID` int(10) unsigned NOT NULL COMMENT '����id',           
          `GroupID` tinyint(2) NOT NULL COMMENT '��������Ⱥ��',        
          PRIMARY KEY (`RoleID`,`FriendID`)                                  
        ) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='�����б�(����)';
	
	
CREATE TABLE `friendship` (               
	`RoleID` int(10) unsigned NOT NULL COMMENT '������id��С��',  
	`FriendID` int(10) unsigned NOT NULL COMMENT '������id�ϴ��',
	`FriVal` mediumint(6) NOT NULL COMMENT '�Ѻö�',
	PRIMARY KEY  (`RoleID`, `FriendID`),
	KEY `FriendID` (`FriendID`)
	) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='�Ѻö�(˫��)';
	
	
CREATE TABLE `enemy` (                                     
          `RoleID` int(10) unsigned NOT NULL COMMENT '���id',   
          `EnemyID` int(10) unsigned NOT NULL COMMENT '���id',  
          PRIMARY KEY (`RoleID`,`EnemyID`)                         
        ) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='���';
	
CREATE TABLE `blacklist` (                                                        
             `RoleID` int(10) unsigned NOT NULL COMMENT '��ɫid',                          
             `BlackID` int(10) unsigned NOT NULL COMMENT '����ɫid�����������id',  
             PRIMARY KEY (`RoleID`,`BlackID`)                                                
           ) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='������';

	
CREATE TABLE `task_done` (                                                                           
             `RoleID` int(10) unsigned NOT NULL,                                                                
             `TaskID` int(10) unsigned NOT NULL COMMENT '����id',                                             
             `Times` smallint(6) unsigned NOT NULL COMMENT 'һ��ʱ������ɵ��������',              
             `StartTime` int(10) unsigned NOT NULL DEFAULT '4294967295' COMMENT '��һ������ʼʱ��',  
             PRIMARY KEY (`RoleID`,`TaskID`)                                                                    
           ) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='����������';

	
	
CREATE TABLE `task` (                                                                 
          `RoleID` int(10) unsigned NOT NULL,                                                 
          `TaskID` int(10) unsigned NOT NULL,                                                 
          `TaskFlag` int(10) NOT NULL DEFAULT '0' COMMENT '�������״̬',               
          `Monster0Num` int(10) NOT NULL DEFAULT '0' COMMENT '�Ѵ������1',              
          `Monster1Num` int(10) NOT NULL DEFAULT '0',                                         
          `Monster2Num` int(10) NOT NULL DEFAULT '0',                                         
          `Monster3Num` int(10) NOT NULL DEFAULT '0',                                         
          `Monster4Num` int(10) NOT NULL DEFAULT '0',                                         
          `Monster5Num` int(10) NOT NULL DEFAULT '0',                                         
          `StartTime` int(10) unsigned NOT NULL DEFAULT '4294967295' COMMENT '��ʼʱ��',  
          `ScriptData` blob COMMENT '�ű�����',                                           
          `DynamicTarget` blob,                                                               
          PRIMARY KEY (`RoleID`,`TaskID`)                                                     
        ) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='�ѽ�����';

CREATE TABLE `role_del` (  
	`AccountID` int(10) unsigned NOT NULL,
	`RoleID` int(10) unsigned NOT NULL,
	`RoleName` varchar(32) NOT NULL,
	`IP` varchar(32),   
	`DeleteTime` datetime NOT NULL
	) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='��ɫɾ����ؼ�¼'; 
	
	
CREATE TABLE `ybaccount` (
	`RoleID` int(10) unsigned NOT NULL,
	`YuanBao` int(10) NOT NULL default '0',
	`Gold` int(10) NOT NULL default '0',
	`Silver` int(10) NOT NULL default '0',
	`bSellOrder` tinyint(2) NOT NULL default '0' COMMENT '�Ƿ��ύ�����۶���',
	`bBuyOrder` tinyint(2) NOT NULL default '0' COMMENT ' �Ƿ��ύ���չ�����',
	PRIMARY KEY (`RoleID`)
	) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='Ԫ�������˻�';
	
	
CREATE TABLE `yuanbaoorder` (
	`dwID` int(10) unsigned NOT NULL COMMENT '����ID',
	`RoleID` int(10) unsigned NOT NULL COMMENT '���ID',
	`OrderType` tinyint(6) NOT NULL default '0' COMMENT '�������ͣ��չ����ۣ�',
	`Price` int(10) NOT NULL default '0' COMMENT '������Ԫ���۸�',
	`Number` int(10) NOT NULL default '0' COMMENT '������Ԫ������',
	`OrderMode` int(6) NOT NULL default '0' COMMENT '��������״̬��ί���г����رգ�',
	`StartTime` int(10) unsigned NOT NULL default '0' COMMENT '�������ύʱ��',
	`EndTime` int(10) unsigned NOT NULL default '4294967295' COMMENT '�����Ľ���ʱ��',
	`AvgPrice` int(10) NOT NULL default '0' COMMENT '���ճɽ���ƽ���۸�',
	`DealNum` int (10) NOT NULL default '0' COMMENT '���յĳɽ�����',
	PRIMARY KEY (`dwID`)
	) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='Ԫ�����׶���';

CREATE TABLE `visiting_card` (                                                 
                 `RoleID` int(11) unsigned NOT NULL COMMENT '��ɫid',                       
                 `Level` tinyint(3) unsigned NOT NULL DEFAULT '1' COMMENT '�ȼ�',                    
                 `Job` tinyint(3) NOT NULL DEFAULT '0' COMMENT 'ְҵ',                      
                 `MateRoleID` int(11) NOT NULL DEFAULT '-1' COMMENT '��ż',                 
                 `FactionID` int(11) NOT NULL DEFAULT '-1' COMMENT '����',                  
                 `Position` int(3) NOT NULL DEFAULT '-1' COMMENT '����ְλ',              
                 `Visibility` tinyint(1) NOT NULL DEFAULT '1' COMMENT '�ɼ���',            
                 `Sex` tinyint(2) unsigned NOT NULL DEFAULT '3' COMMENT '�Ա�',             
                 `Constellation` tinyint(4) unsigned NOT NULL DEFAULT '13' COMMENT '����',  
                 `ChineseZodiac` tinyint(4) unsigned NOT NULL DEFAULT '13' COMMENT '��Ф',  
                 `Area` tinyint(4) unsigned NOT NULL DEFAULT '0' COMMENT '����',            
                 `Province` tinyint(6) unsigned NOT NULL DEFAULT '0' COMMENT 'ʡ��',        
                 `City` char(6) NOT NULL DEFAULT 'N/A' COMMENT '����',                     
                 `HeadUrl` varchar(100) DEFAULT NULL COMMENT 'ͷ��',              
                 `Signature` varchar(200) NOT NULL DEFAULT 'N/A' COMMENT 'ǩ��',            
                 PRIMARY KEY (`RoleID`)                                                       
               ) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='��ɫ������';
               
CREATE TABLE `clan_data` (                                                                       
             `RoleID` int(11) unsigned NOT NULL,                                                            
             `FameMask` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '�����ñ�ʶ',                    
             `RepRstTimeStamp` int(11) unsigned NOT NULL DEFAULT '0' COMMENT '��������ʱ��',          
             `RepXuanYuan` int(11) unsigned NOT NULL DEFAULT '0' COMMENT '��ԯ����',                    
             `ConXuanYuan` int(11) unsigned NOT NULL DEFAULT '0' COMMENT '��ԯ����',                    
             `ActCntXuanYuan` tinyint(2) unsigned NOT NULL DEFAULT '0' COMMENT '��ԯʣ�༤�����',  
             `RepShenNong` int(11) unsigned NOT NULL DEFAULT '0' COMMENT '��ũ����',                    
             `ConShenNong` int(11) unsigned NOT NULL DEFAULT '0' COMMENT '��ũ����',                    
             `ActCntShenNong` tinyint(2) unsigned NOT NULL DEFAULT '0' COMMENT '��ũʣ�༤�����',  
             `RepFuXi` int(11) unsigned NOT NULL DEFAULT '0' COMMENT '��������',                        
             `ConFuXi` int(11) unsigned NOT NULL DEFAULT '0' COMMENT '���˹���',                        
             `ActCntFuXi` tinyint(2) unsigned NOT NULL DEFAULT '0' COMMENT '����ʣ�༤�����',      
             `RepSanMiao` int(11) unsigned NOT NULL DEFAULT '0' COMMENT '��������',                     
             `ConSanMiao` int(11) unsigned NOT NULL DEFAULT '0' COMMENT '���繱��',                     
             `ActCntSanMiao` tinyint(2) unsigned NOT NULL DEFAULT '0' COMMENT '����ʣ�༤�����',   
             `RepJiuLi` int(11) unsigned NOT NULL DEFAULT '0' COMMENT '��������',                       
             `ConJiuLi` int(11) unsigned NOT NULL DEFAULT '0' COMMENT '���蹱��',                       
             `ActCntJiuLi` tinyint(2) unsigned NOT NULL DEFAULT '0' COMMENT '����ʣ�༤�����',     
             `RepYueZhi` int(11) unsigned NOT NULL DEFAULT '0' COMMENT '��������',                      
             `ConYueZhi` int(11) unsigned NOT NULL DEFAULT '0' COMMENT '���Ϲ���',                      
             `ActCntYueZhi` tinyint(2) unsigned NOT NULL DEFAULT '0' COMMENT '����ʣ�༤�����',    
             `RepNvWa` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'Ů�����',                        
             `ConNvWa` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'Ů洹���',                        
             `ActCntNvWa` tinyint(2) unsigned NOT NULL DEFAULT '0' COMMENT 'Ů�ʣ�༤�����',      
             `RepGongGong` int(11) unsigned NOT NULL DEFAULT '0' COMMENT '��������',                    
             `ConGongGong` int(11) unsigned NOT NULL DEFAULT '0' COMMENT '��������',                    
             `ActCntGongGong` tinyint(2) unsigned NOT NULL DEFAULT '0' COMMENT '����ʣ�༤�����',  
             PRIMARY KEY (`RoleID`),                                                                        
             KEY `RepFuXi` (`RepFuXi`),                                                                     
             KEY `RepXuanYuan` (`RepXuanYuan`),                                                             
             KEY `RepSanMiao` (`RepSanMiao`),                                                               
             KEY `RepShenNong` (`RepShenNong`),                                                             
             KEY `RepJiuLi` (`RepJiuLi`),                                                                   
             KEY `RepGongGong` (`RepGongGong`),                                                             
             KEY `RepNvWa` (`RepNvWa`),                                                                     
             KEY `RepYueZhi` (`RepYueZhi`)                                                                  
           ) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='����������ݱ�';                                          

CREATE TABLE `famehall_act_treasure` (                               
	 `TreasureID` smallint(11) unsigned NOT NULL COMMENT '�����䱦ID',  
	 `ActNameID` int(11) unsigned NOT NULL COMMENT '������䱦��ɫ����ID',                             
	 `RoleID` int(11) unsigned NOT NULL COMMENT '��ɫID',                                
	 `ClanType` tinyint(1) unsigned NOT NULL COMMENT '��������',                           
	 `ActTime` int(11) unsigned NOT NULL COMMENT '����ʱ��',                               
	 PRIMARY KEY (`ClanType`,`TreasureID`),                              
	 KEY `ActTime` (`ActTime`)
	) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='�����䱦�����';

CREATE TABLE `famehall_entersnap` (         
	`RoleID` int(11) unsigned NOT NULL COMMENT '��ɫID',       
	`EnterNameID` int(11) unsigned NOT NULL COMMENT '��ɫ����ID',  
	`ClanType` tinyint(3) unsigned NOT NULL COMMENT '��������',  
	`EnterTime` int(11) unsigned NOT NULL COMMENT '����ʱ��',    
	PRIMARY KEY (`ClanType`,`EnterTime`),     
	KEY `ClanType` (`ClanType`)               
) ENGINE=MyISAM DEFAULT CHARSET=utf8 ;


CREATE TABLE `famehall_rep_rst_timestamp` (    
	`ClanType` tinyint(8) NOT NULL COMMENT '��������',              
	`ResetTimeStamp` int(11) unsigned NOT NULL COMMENT '����ʱ��',  
	PRIMARY KEY (`ClanType`)                     
) ENGINE=MyISAM DEFAULT CHARSET=utf8;   
	

CREATE TABLE `guild` (                         
	`ID` int(10) unsigned NOT NULL COMMENT '����ID -- ����ת��Сд���CRC32ֵ',
	`Name` blob COMMENT '����',
	
	`FounderNameID` int(10) unsigned NOT NULL COMMENT '���ɴ�ʼ������ID',
	`LeaderID` int(10) unsigned NOT NULL COMMENT '���ΰ���ID',
	`SpecState` int(10) unsigned NOT NULL default '0' COMMENT '���ɵ�ǰ����״̬',
	
	`Level` tinyint(3) NOT NULL COMMENT '���ɵ�ǰ�ȼ�',
	`HoldCity0` tinyint(3) unsigned NOT NULL default '0' COMMENT '���ɵ�ǰϽ�����б��',
	`HoldCity1` tinyint(3) unsigned NOT NULL default '0' COMMENT '',
	`HoldCity2` tinyint(3) unsigned NOT NULL default '0' COMMENT '',
	
	`Fund` int(10) NOT NULL default '0' COMMENT '��ǰ�ʽ�',
	`Material` int(10) NOT NULL default '0' COMMENT '��ǰ�ʲ�',
	`Rep` int(10) NOT NULL default '0' COMMENT '��ǰ����',
	`DailyCost` int(10) NOT NULL default '0' COMMENT 'ÿ��ά������',
	
	`Peace` smallint(5) NOT NULL default '0' COMMENT '��ǰ������',
	`Rank` smallint(5) NOT NULL default '0' COMMENT '��ǰ����(0��ʾ��δ����)',
	
	`Tenet` blob COMMENT '��ּ',
	`Symbol` blob COMMENT '��־ULR',
	
	`CreateTime` datetime NOT NULL COMMENT '����ʱ��',
	`GroupPurchase` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '�����Ź�ָ��', 
	`RemainSpreadTimes` int(3) unsigned NOT NULL DEFAULT '0' COMMENT '����ʣ�෢������',                                                           
    `Commendation` tinyint(1) NOT NULL DEFAULT '0' COMMENT '���̼ν�״̬',
	PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='��������';
	

CREATE TABLE `guild_member` (
	`RoleID` int(10) unsigned NOT NULL COMMENT '��ԱID',
	`GuildID` int(10) unsigned NOT NULL COMMENT '����ID',
	`GuildPos` tinyint(3) unsigned NOT NULL COMMENT '����ְλ',
	
	`TotalContrib` int(10) NOT NULL default '0' COMMENT '�ۼư��ɹ���',
	`CurContrib` int(10) NOT NULL default '0' COMMENT '��ǰ���ɹ���',
	`Exploit` int(10) NOT NULL default '0' COMMENT '���ɹ�ѫ',
	`Salary` int(10) NOT NULL default '0' COMMENT 'ٺ»��׼',
	`CanUseGuildWare` tinyint(1) NOT NULL DEFAULT '0' COMMENT '�������ɲֿ�Ȩ��',
	`JoinTime` datetime NOT NULL COMMENT '����ʱ��',
	PRIMARY KEY (`RoleID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='���ɳ�Ա';
	

CREATE TABLE `log_baibao` (
	`AccountID` int(10) unsigned NOT NULL COMMENT '�˺�ID',         
	`RoleID` int(10) NOT NULL COMMENT '��Ʒ��Դ',                   
	`LogTime` char(20) NOT NULL COMMENT '�����Ʒʱ��',             
	`n16Type` tinyint(4) unsigned NOT NULL COMMENT '��Ʒ��÷�ʽ',  
	`TypeID` int(10) unsigned NOT NULL COMMENT '��Ʒ����',          
	`LeaveWords` tinyblob COMMENT '����'                            
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
            
CREATE TABLE `group_purchase` (                                       
	`GuildID` int(10) unsigned NOT NULL COMMENT '����ID',               
	`RoleID` int(10) NOT NULL COMMENT '������ID',                       
	`MallID` int(10) NOT NULL COMMENT '�̳���ƷID',                     
	`ItemPrice` int(10) unsigned NOT NULL COMMENT '��Ʒ�۸�',           
	`ParticipatorNum` tinyint(3) unsigned NOT NULL COMMENT '��Ӧ����',  
	`RequireNum` tinyint(3) unsigned NOT NULL COMMENT '�Ź���������',   
	`RemainTime` int(10) NOT NULL COMMENT '�Ź�ʣ��ʱ��',               
	`ParticipatorList` tinyblob COMMENT '��Ӧ���б�'                    
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
               
CREATE TABLE `pet_data` (                                                                                  
            `pet_id` int(11) unsigned NOT NULL COMMENT '����id',
            `pet_name` varchar(32) NOT NULL COMMENT  '��������',
            `master_id` int(11) unsigned NOT NULL COMMENT '����id',
            `type_id` int(11) unsigned NOT NULL COMMENT 'ԭ��id',
            `quality` tinyint(1) unsigned NOT NULL COMMENT '����Ʒ��',
            `aptitude` int(4) unsigned NOT NULL COMMENT '��������',
            `potential` int(4) unsigned NOT NULL COMMENT '��ǰǱ��',
            `cur_spirit` int(4) NOT NULL COMMENT '��ǰ����',
            `cur_exp` int(20) NOT NULL DEFAULT '0' COMMENT '��ǰ����',
            `step` tinyint(1) NOT NULL DEFAULT '0' COMMENT '��',
            `grade` tinyint(1) NOT NULL DEFAULT '1' COMMENT '��',
            `talent_count` int(4) NOT NULL DEFAULT '0' COMMENT '��ǰ���ʼ���',
            `wuxing_energy` int(4) NOT NULL DEFAULT '0' COMMENT '������',
            `pet_state` int(1) NOT NULL DEFAULT '0'COMMENT '����״̬',
            `pet_lock` tinyint(1) NOT NULL DEFAULT '0' COMMENT '��������',
            PRIMARY KEY (`pet_id`)
          ) ENGINE=MyISAM DEFAULT CHARSET=utf8 CHECKSUM=1 DELAY_KEY_WRITE=1 ROW_FORMAT=DYNAMIC COMMENT='�����';

CREATE TABLE `vip_stall` (                                       
             `StallID` tinyint(2) unsigned NOT NULL COMMENT 'VIP̯λ���',  
             `RoleID` int(10) unsigned NOT NULL COMMENT '������ID',         
             `RemainTime` int(10) NOT NULL COMMENT 'ʣ��ʱ��',              
             PRIMARY KEY (`StallID`)                                        
           ) ENGINE=MyISAM DEFAULT CHARSET=utf8;

CREATE TABLE `pet_skill` (                                               
             `petskill_typeid` int(11) unsigned NOT NULL COMMENT '���＼��id',  
             `petid` int(11) unsigned NOT NULL COMMENT '����id',                  
             `para1` int(11) unsigned DEFAULT NULL COMMENT '����1',      
             `para2` int(11) unsigned DEFAULT NULL COMMENT '����2',      
             PRIMARY KEY (`petskill_typeid`,`petid`)                                
             KEY `petid` (`petid`)                                              
           ) ENGINE=MyISAM DEFAULT CHARSET=utf8;                               

CREATE TABLE `guild_commodity` (                                                                 
                   `role_id` int(10) unsigned NOT NULL COMMENT '���ID',                                        
                   `guild_id` int(10) unsigned NOT NULL COMMENT '���ڰ���ID',                                 
                   `role_level` smallint(3) unsigned NOT NULL DEFAULT '1' COMMENT '��ʼ����ʱ��ҵȼ�',  
                   `tael` int(10) NOT NULL DEFAULT '0' COMMENT '��ǰ��������',                              
                   `goods` tinyblob COMMENT '�̻���Ϣ',                                                       
                   PRIMARY KEY (`role_id`)                                                                        
                 ) ENGINE=MyISAM DEFAULT CHARSET=utf8;
     
CREATE TABLE `guild_skill` (                                         
               `guild_id` int(10) unsigned NOT NULL COMMENT '����ID',           
               `skill_id` int(10) unsigned NOT NULL COMMENT '����ID',           
               `progress` int(10) unsigned NOT NULL COMMENT '��������',       
               `level` int(2) unsigned NOT NULL COMMENT '��ǰ�ȼ�',           
               `researching` tinyint(1) NOT NULL COMMENT '��ǰ�����о���'  
             ) ENGINE=MyISAM DEFAULT CHARSET=utf8;

CREATE TABLE `vip_netbar` (                                                                                  
              `accountid` int(11) unsigned NOT NULL COMMENT '�˺�id',                                                  
              `login_time` int(11) unsigned NOT NULL COMMENT '�ϴ��ڽ������ɵ�¼ʱ��',                        
              PRIMARY KEY (`accountid`)                                                                                  
            ) ENGINE=InnoDB DEFAULT CHARSET=utf8 CHECKSUM=1 DELAY_KEY_WRITE=1 ROW_FORMAT=DYNAMIC COMMENT='��������';


CREATE TABLE `guild_upgrade` (                                           
                 `guild_id` int(10) unsigned NOT NULL COMMENT '��ʩ��������',     
                 `type` int(2) unsigned NOT NULL COMMENT '������ʩ����',          
                 `level` int(2) unsigned NOT NULL COMMENT '��ʩ�ȼ�',               
                 `progress` int(10) unsigned NOT NULL COMMENT '��������',           
                 `item_type_1` int(10) unsigned NOT NULL COMMENT '������Ʒ����',  
                 `item_neednum_1` int(10) unsigned NOT NULL COMMENT '���������',  
                 `item_type_2` int(10) unsigned NOT NULL,                               
                 `item_neednum_2` int(10) unsigned NOT NULL,                            
                 `item_type_3` int(10) unsigned NOT NULL,                               
                 `item_neednum_3` int(10) unsigned NOT NULL,                            
                 `item_type_4` int(10) unsigned NOT NULL,                               
                 `item_neednum_4` int(10) unsigned NOT NULL                             
               ) ENGINE=MyISAM DEFAULT CHARSET=utf8;
               
CREATE TABLE `commerce_rank` (                                             
                 `role_id` int(10) unsigned NOT NULL COMMENT '���ID',                  
                 `guild_id` int(10) unsigned NOT NULL COMMENT '����ID',                 
                 `times` int(10) NOT NULL DEFAULT '0' COMMENT '������̴���',       
                 `tael` int(10) NOT NULL DEFAULT '0' COMMENT 'Ϊ���ɹ��׵�����',  
                 PRIMARY KEY (`role_id`)                                                  
               ) ENGINE=MyISAM DEFAULT CHARSET=utf8;    
               
create table `activity` (   
		 `dwID` int (10)   NOT NULL ,  
		 `ScriptData` blob   NULL  , 
		 PRIMARY KEY ( `dwID` )  
		) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='�̶������';           

CREATE TABLE `left_msg` (                                                                                    
            `msg_id` int(11) unsigned NOT NULL COMMENT '��������ʱ��',                                           
            `roleid` int(11) unsigned NOT NULL COMMENT '���id',                                                     
            `msg_data` tinyblob COMMENT '��������',                                                                
            PRIMARY KEY (`msg_id`,`roleid`)                                                                            
          ) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='��������';

CREATE TABLE `centralbilling` (                                    
                  `TOKENID` int(11) NOT NULL COMMENT '����ID',                   
                  `USERNAME` varchar(50) NOT NULL COMMENT '�ʺ�',                
                  `WORLDID` int(11) DEFAULT NULL COMMENT '��ID',                  
                  `ZONEID` int(11) NOT NULL COMMENT '��ID',                       
                  `YUANORITEM` tinyint(1) NOT NULL COMMENT '0 Ԫ����1 ����',  
                  `ITEMCODE` int(11) DEFAULT NULL COMMENT '���߱��',          
                  `QTY` int(11) NOT NULL DEFAULT '0' COMMENT '����',             
                  `STATE` int(11) NOT NULL DEFAULT '0' COMMENT '״̬',           
                  `INSERTDATE` datetime NOT NULL COMMENT '����ʱ��',           
                  PRIMARY KEY (`TOKENID`)                                          
                ) ENGINE=InnoDB DEFAULT CHARSET=utf8;
CREATE TABLE `centralbilling_log` LIKE `centralbilling`;

# ����ʵ�����м�¼��
CREATE TABLE `strength_rankings` (                     
	`Num` smallint(6) unsigned NOT NULL DEFAULT '0' COMMENT '����',
	`PreNum` smallint(6) unsigned NOT NULL DEFAULT '0' COMMENT '�ϴ�����',
	`Strength` mediumint(8) unsigned NOT NULL DEFAULT '0' COMMENT 'ʵ��ֵ',
	`RoleName` varchar(32) NOT NULL COMMENT '��ɫ����', 
	PRIMARY KEY  (`RoleName`)
	);


#���ݿ���������
#ALTER TABLE `item_del` ADD KEY (`SerialNum`);
ALTER TABLE `yuanbaoorder` ADD KEY (`OrderMode`);
ALTER TABLE `yuanbaoorder` ADD KEY (`RoleID`);
ALTER TABLE `guild_upgrade` ADD KEY (`guild_id`);	
ALTER TABLE `guild_skill` ADD KEY (`guild_id`);
ALTER TABLE `guild_commodity` ADD KEY (`guild_id`);
ALTER TABLE `commerce_rank` ADD KEY (`guild_id`);
ALTER TABLE `roledata` ADD KEY (`RemoveFlag`);
#ALTER TABLE `item_needlog` ADD KEY (`NeedLog`);
ALTER TABLE `group_purchase` ADD KEY (`GuildID`);
ALTER TABLE `group_purchase` ADD KEY (`RoleID`);
ALTER TABLE `group_purchase` ADD KEY (`MallID`);
ALTER TABLE `pet_data` ADD KEY (`master_id`);
ALTER TABLE `yuanbaoorder` ADD KEY (`StartTime`);
#ALTER TABLE `account` ADD KEY (`worldname_crc`);
ALTER TABLE `roledata` ADD KEY (`rolename`);
ALTER TABLE `account_common` ADD KEY (`AccountName`);
ALTER TABLE `task_done` ADD KEY (`RoleID`);
  
  
# Jason �ⲿ���ӱ�
CREATE TABLE `external_links` (
  `ID` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `LinkName` varchar(10) NOT NULL DEFAULT '"NoName"' COMMENT '??????',
  `LinkURL` varchar(256) DEFAULT NULL COMMENT '????URL',
  PRIMARY KEY (`ID`),
  KEY `LinkName` (`LinkName`)
) ENGINE=InnoDB AUTO_INCREMENT=6 DEFAULT CHARSET=utf8;
  

################## �����±��ڴ�֮�ϼ� #######################

	
CREATE TABLE `item_needlog` (               
	`TypeID` int(10) unsigned NOT NULL COMMENT '��Ʒ��TypeID(Ψһ)',
	`NeedLog` tinyint(2) unsigned NOT NULL default '1' COMMENT '�Ƿ��¼log(0:����¼;1:��¼)',
	`MinQlty` tinyint(2) NOT NULL default '0' COMMENT '���¼log����Ʒ�����Ʒ��',
	PRIMARY KEY  (`TypeID`)
	) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='����log����ƷTypeID������Ӫ��д'; 
	

#������Ʒ��װ����ṹ
CREATE TABLE `item_baibao` SELECT * FROM `item` WHERE 0;
ALTER TABLE `item_baibao` ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='��Ʒ��';
ALTER TABLE `item_baibao` ADD PRIMARY KEY  (`SerialNum`);
ALTER TABLE `item_baibao` ADD KEY (`AccountID`);

CREATE TABLE `equip_baibao` LIKE `equip`;

CREATE TABLE `item_del` LIKE `item`;
Alter table `item_del` drop primary key;

CREATE TABLE `equip_del` LIKE `equip`;
Alter table `equip_del` drop primary key;

CREATE TABLE `roledata_del` LIKE `roledata`;
ALTER TABLE `roledata_del` DROP PRIMARY KEY;
ALTER TABLE `roledata_del` DROP INDEX `RoleNameCrc`;


