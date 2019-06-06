############################## 1.1.25 start #########################################

##2009-06-23
#account_common�����AccountName�ֶ�
alter table `account_common` add column `AccountName` char (36)   NOT NULL  COMMENT '�˺���' after `AccountID`;

############################## 1.1.25 end ###########################################

############################## 1.2.0 start ###########################################

##2009-07-06
#������Ʒ������
CREATE TABLE `bill_item` (                 
             `token_id` char(50) NOT NULL COMMENT '����ID',          
             `account_id` int(10) unsigned NOT NULL COMMENT '�ʺ�ID',  
             `item_id` int(10) unsigned NOT NULL COMMENT '��ƷID',     
             `item_num` int(11) NOT NULL COMMENT '��Ʒ����',             
             PRIMARY KEY (`token_id`)                 
           ) ENGINE=MyISAM DEFAULT CHARSET=utf8;       

##2009-07-06
#����Ԫ��������
CREATE TABLE `bill_yuanbao` (                       
                `token_id` char(50) NOT NULL COMMENT '����ID',  
                `account_id` int(10) unsigned NOT NULL COMMENT '�ʺ�ID',          
                `yuanbao` int(11) NOT NULL COMMENT 'Ԫ������',                       
                PRIMARY KEY (`token_id`)                          
              ) ENGINE=MyISAM DEFAULT CHARSET=utf8;
              
##2009-07-07
#���ӳ������ݱ�
CREATE TABLE `pet_data` (                                                                                       
	`pet_id` int(11) unsigned NOT NULL COMMENT '����id',                                                        
	`pet_name` varchar(32) NOT NULL COMMENT '��������',                                                       
	`master_id` int(11) unsigned NOT NULL COMMENT '����id',                                                     
	`type_id` int(11) unsigned NOT NULL COMMENT 'ԭ��id',                                                       
	`quality` tinyint(1) unsigned NOT NULL COMMENT '����Ʒ��',                                                
	`aptitude` tinyint(3) unsigned NOT NULL COMMENT '��������',                                               
	`cur_spirit` int(4) NOT NULL COMMENT '��ǰ����',                                                          
	`mountable` tinyint(1) NOT NULL COMMENT '�ܷ����',                                                       
	`cur_exp` int(20) NOT NULL DEFAULT '0' COMMENT '��ǰ����',                                                
	`step` tinyint(1) NOT NULL DEFAULT '0' COMMENT '��',                                                         
	`grade` tinyint(1) NOT NULL DEFAULT '1' COMMENT '��',                                                        
	`talent_count` tinyint(4) NOT NULL DEFAULT '0' COMMENT '��ǰ���ʼ���',                                  
	`wuxing_energy` tinyint(4) NOT NULL DEFAULT '0' COMMENT '������',                                          
	`pet_state` tinyint(1) NOT NULL DEFAULT '0' COMMENT '����״̬',                                           
	PRIMARY KEY (`pet_id`)                                                                                        
) ENGINE=MyISAM DEFAULT CHARSET=utf8 CHECKSUM=1 DELAY_KEY_WRITE=1 ROW_FORMAT=DYNAMIC COMMENT='�����' ;

##2009-07-08
#����VIP̯λ��
CREATE TABLE `vip_stall` (                                       
             `StallID` tinyint(2) unsigned NOT NULL COMMENT 'VIP̯λ���',  
             `RoleID` int(10) unsigned NOT NULL COMMENT '������ID',         
             `RemainTime` int(10) NOT NULL COMMENT 'ʣ��ʱ��',              
             PRIMARY KEY (`StallID`)                                        
           ) ENGINE=MyISAM DEFAULT CHARSET=utf8;

##2009-07-10
#�������ݱ�����Ǳ���ֶ�
alter table `pet_data` add column `potential` smallint(4) UNSIGNED NOT NULL COMMENT '��ǰǱ��' after `aptitude`;

#2009-7-15 roledata�����ӽű�����blob
alter table roledata add column `ScriptData` blob NULL COMMENT '�ű�����' after `RemoveTime`;

##2009-7-17
#Ϊװ������Ǳ��ֵӰ��ϵ��
alter table `equip` add column `PotValModPct` smallint(10) UNSIGNED DEFAULT '10000' NOT NULL COMMENT 'װ��Ǳ��ֵӰ��ϵ��' after `QltyModPctEx`;
alter table `equip_baibao` add column `PotValModPct` smallint(10) UNSIGNED DEFAULT '10000' NOT NULL COMMENT 'װ��Ǳ��ֵӰ��ϵ��' after `QltyModPctEx`;
alter table `equip_del` add column `PotValModPct` smallint(10) UNSIGNED DEFAULT '10000' NOT NULL COMMENT 'װ��Ǳ��ֵӰ��ϵ��' after `QltyModPctEx`;

##2009-7-21
#���ӳ��＼�ܱ��
create table `pet_skill`( 
	`petskill_typeid` int(11) NOT NULL COMMENT '���＼��id', 
	`petid` int(11) NOT NULL COMMENT '����id', 
	`para1` int(11) ZEROFILL COMMENT '����1', 
	`para2` int(11) ZEROFILL COMMENT '����2', 
	PRIMARY KEY (`petskill_typeid`, `petid`)
	)  ;

##2009-7-22
#�޸ĳ������ݱ��
alter table `pet_data` drop column `mountable`;
						

############################## 1.2.0 end ############################################


############################## 1.2.1 start ##########################################
##2009-7-23
#�޸Ľ�ɫ������
alter table `visiting_card` change `Level` `Level` tinyint(3) UNSIGNED default '1' NOT NULL comment '�ȼ�';

##2009-7-28
#�޸ĳ��＼�ܱ�
alter table `pet_skill` change `petskill_typeid` `petskill_typeid` int(11) UNSIGNED NOT NULL comment '���＼��id', 
						change `petid` `petid` int(11) UNSIGNED NOT NULL comment '����id',
						change `para1` `para1` int(11) UNSIGNED NULL  comment '����1', 
						change `para2` `para2` int(11) UNSIGNED NULL  comment '����2';

#2009-7-29 roledata�����ӽ�ɫ�����������
alter table roledata add column `TreasureSum` tinyint(3) unsigned default '0' NOT NULL COMMENT '��ɫ����������' after `ScriptData`;

##2009-08-03
#�޸ĲֿⱣ���Ǯ��ʽ
alter table `account_common` drop column `WareGold`;
alter table `account_common` change `WareSilver` `WareSilver` int(20) default '0' NOT NULL;

#׷�Ӱ��������ֶ�
alter table `guild` add column `LeaderID` int(10) UNSIGNED NOT NULL COMMENT '����ID' after `FounderNameID`;
alter table `guild` add column `RemainSpreadTimes` int(3) DEFAULT '0' NOT NULL COMMENT '���񷢲�ʣ�����';
alter table `guild` add column `Commendation` bool DEFAULT '0' NOT NULL COMMENT '���̼ν�״̬';

#׷�Ӱ��ɳ�Ա����
alter table `guild_member` add column `CanUseGuildWare` bool DEFAULT '0' NOT NULL COMMENT '�������ɲֿ�Ȩ��';

#������������������
CREATE TABLE `guild_commodity` (                                                                 
                   `role_id` int(10) unsigned NOT NULL COMMENT '���ID',                                        
                   `guild_id` int(10) unsigned NOT NULL COMMENT '���ڰ���ID',                                 
                   `role_level` smallint(3) unsigned NOT NULL DEFAULT '1' COMMENT '��ʼ����ʱ��ҵȼ�',  
                   `tael` int(10) NOT NULL DEFAULT '0' COMMENT '��ǰ��������',                              
                   `goods` tinyblob COMMENT '�̻���Ϣ',                                                       
                   PRIMARY KEY (`role_id`)                                                                        
                 ) ENGINE=MyISAM DEFAULT CHARSET=utf8;

#�������ɼ��ܱ�
CREATE TABLE `guild_skill` (                                         
               `guild_id` int(10) unsigned NOT NULL COMMENT '����ID',           
               `skill_id` int(10) unsigned NOT NULL COMMENT '����ID',           
               `progress` int(10) unsigned NOT NULL COMMENT '��������',       
               `level` int(2) unsigned NOT NULL COMMENT '��ǰ�ȼ�',           
               `researching` tinyint(1) NOT NULL COMMENT '��ǰ�����о���'  
             ) ENGINE=MyISAM DEFAULT CHARSET=utf8;

#����������ʩ������
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
               
#���������������а��
CREATE TABLE `commerce_rank` (                                             
                 `role_id` int(10) unsigned NOT NULL COMMENT '���ID',                  
                 `guild_id` int(10) unsigned NOT NULL COMMENT '����ID',                 
                 `times` int(10) NOT NULL DEFAULT '0' COMMENT '������̴���',       
                 `tael` int(10) NOT NULL DEFAULT '0' COMMENT 'Ϊ���ɹ��׵�����',  
                 PRIMARY KEY (`role_id`)                                                  
               ) ENGINE=MyISAM DEFAULT CHARSET=utf8;
               
						
						alter table `blacklist` change `RoleID` `RoleID` int(10) UNSIGNED NOT NULL comment '��ɫid', 
						change `BlackID` `BlackID` int(10) UNSIGNED NOT NULL comment '����ɫid�����������id';


##2009-8-5
#�������ݿ��ע��
alter table `clan_data` change `RepXuanYuan` `RepXuanYuan` int(11) UNSIGNED default '0' NOT NULL comment '��ԯ����', 
						change `ConXuanYuan` `ConXuanYuan` int(11) UNSIGNED default '0' NOT NULL comment '��ԯ����', 
						change `ActCntXuanYuan` `ActCntXuanYuan` tinyint(2) UNSIGNED default '0' NOT NULL comment '��ԯʣ�༤�����', 
						change `RepShenNong` `RepShenNong` int(11) UNSIGNED default '0' NOT NULL comment '��ũ����', 
						change `ConShenNong` `ConShenNong` int(11) UNSIGNED default '0' NOT NULL comment '��ũ����', 
						change `ActCntShenNong` `ActCntShenNong` tinyint(2) UNSIGNED default '0' NOT NULL comment '��ũʣ�༤�����', 
						change `RepFuXi` `RepFuXi` int(11) UNSIGNED default '0' NOT NULL comment '��������', 
						change `ConFuXi` `ConFuXi` int(11) UNSIGNED default '0' NOT NULL comment '���˹���', 
						change `ActCntFuXi` `ActCntFuXi` tinyint(2) UNSIGNED default '0' NOT NULL comment '����ʣ�༤�����', 
						change `RepSanMiao` `RepSanMiao` int(11) UNSIGNED default '0' NOT NULL comment '��������', 
						change `ConSanMiao` `ConSanMiao` int(11) UNSIGNED default '0' NOT NULL comment '���繱��', 
						change `ActCntSanMiao` `ActCntSanMiao` tinyint(2) UNSIGNED default '0' NOT NULL comment '����ʣ�༤�����', 
						change `RepJiuLi` `RepJiuLi` int(11) UNSIGNED default '0' NOT NULL comment '��������', 
						change `ConJiuLi` `ConJiuLi` int(11) UNSIGNED default '0' NOT NULL comment '���蹱��', 
						change `ActCntJiuLi` `ActCntJiuLi` tinyint(2) UNSIGNED default '0' NOT NULL comment '����ʣ�༤�����', 
						change `RepYueZhi` `RepYueZhi` int(11) UNSIGNED default '0' NOT NULL comment '��������', 
						change `ConYueZhi` `ConYueZhi` int(11) UNSIGNED default '0' NOT NULL comment '���Ϲ���', 
						change `ActCntYueZhi` `ActCntYueZhi` tinyint(2) UNSIGNED default '0' NOT NULL comment '����ʣ�༤�����', 
						change `RepNvWa` `RepNvWa` int(11) UNSIGNED default '0' NOT NULL comment 'Ů�����', 
						change `ConNvWa` `ConNvWa` int(11) UNSIGNED default '0' NOT NULL comment 'Ů洹���', 
						change `ActCntNvWa` `ActCntNvWa` tinyint(2) UNSIGNED default '0' NOT NULL comment 'Ů�ʣ�༤�����', 
						change `RepGongGong` `RepGongGong` int(11) UNSIGNED default '0' NOT NULL comment '��������', 
						change `ConGongGong` `ConGongGong` int(11) UNSIGNED default '0' NOT NULL comment '��������', 
						change `ActCntGongGong` `ActCntGongGong` tinyint(2) UNSIGNED default '0' NOT NULL comment '����ʣ�༤�����';
						
alter table `enemy` change `RoleID` `RoleID` int(10) UNSIGNED NOT NULL comment '���id', 
					change `EnemyID` `EnemyID` int(10) UNSIGNED NOT NULL comment '���id';

alter table `friend` change `RoleID` `RoleID` int(10) UNSIGNED NOT NULL comment '���id', 
					change `FriendID` `FriendID` int(10) UNSIGNED NOT NULL comment '����id', 
					change `GroupID` `GroupID` tinyint(2) NOT NULL comment '��������Ⱥ��';
					
alter table `item` change `SerialNum` `SerialNum` bigint(20) NOT NULL comment '64λid', 
					change `Num` `Num` smallint(5) default '1' NOT NULL comment '����', 
					change `TypeID` `TypeID` int(10) UNSIGNED NOT NULL comment '����id', 
					change `Bind` `Bind` tinyint(4) NOT NULL comment '�Ƿ��', 
					change `CreatorID` `CreatorID` int(10) UNSIGNED default '4294967295' NOT NULL comment '������id';
					
alter table `item_cdtime` change `RoleID` `RoleID` int(10) UNSIGNED NOT NULL comment '��ɫid', 
					change `CDTime` `CDTime` blob NULL  comment '��ȴʱ��blob����ʽΪ��typeid����ȴʱ�䣩';
					
alter table `task` change `Monster0Num` `Monster0Num` int(10) default '0' NOT NULL comment '�Ѵ������1', 
					change `StartTime` `StartTime` int(10) UNSIGNED default '4294967295' NOT NULL comment '��ʼʱ��', 
					change `ScriptData` `ScriptData` blob NULL  comment '�ű�����', 
					change `DynamicTarget` `DynamicTarget` blob NULL ;
					
alter table `task_done` change `TaskID` `TaskID` int(10) UNSIGNED NOT NULL comment '����id', 
					change `Times` `Times` smallint(6) UNSIGNED NOT NULL comment 'һ��ʱ������ɵ��������', 
					change `StartTime` `StartTime` int(10) UNSIGNED default '4294967295' NOT NULL comment '��һ������ʼʱ��';

alter table `visiting_card` change `City` `City` char(6) character set utf8 collate utf8_general_ci default 'N/A' NOT NULL comment '����';

##2009-8-5
#���ӽ����������ݿ��
CREATE TABLE `vip_netbar` (                                                                                  
              `accountid` int(11) unsigned NOT NULL COMMENT '�˺�id',                                                  
              `login_time` int(11) unsigned NOT NULL COMMENT '�ϴ��ڽ������ɵ�¼ʱ��',                        
              PRIMARY KEY (`accountid`)                                                                                  
            ) ENGINE=InnoDB DEFAULT CHARSET=utf8 CHECKSUM=1 DELAY_KEY_WRITE=1 ROW_FORMAT=DYNAMIC COMMENT='��������'  ;

##2009-8-11
#�̶���ű�����
create table `activity` (    `dwID` int (10)   NOT NULL ,  
			`ScriptData` blob   NULL  , 
			PRIMARY KEY ( `dwID` )  
			) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='�̶������';      

##2009-8-20
#�޸ĳ����
alter table `pet_data` change `potential` `potential` int(4) UNSIGNED NOT NULL comment '��ǰǱ��';

##2009-9-23
#�޸İ���Ĭ��ֵ
alter table `guild` change `HoldCity0` `HoldCity0` tinyint(3) UNSIGNED default '0' NOT NULL comment '���ɵ�ǰϽ�����б��';
alter table `guild` change `HoldCity1` `HoldCity1` tinyint(3) UNSIGNED default '0' NOT NULL;
alter table `guild` change `HoldCity2` `HoldCity2` tinyint(3) UNSIGNED default '0' NOT NULL;
update guild set HoldCity0=0,HoldCity1=0,HoldCity2=0;

############################## 1.2.1 end ############################################

############################## 1.2.2 start ##########################################


#���ӽ�ɫ̯λ�ɳ������ֶ�
alter table `roledata` add column `StallLevel` tinyint(3) UNSIGNED DEFAULT '1' NOT NULL COMMENT '̯λ�ȼ�';
alter table `roledata` add column `StallDailyExp` int(10) UNSIGNED DEFAULT '0' NOT NULL COMMENT '̯λ���쾭��';
alter table `roledata` add column `StallCurExp` int(10) UNSIGNED DEFAULT '0' NOT NULL COMMENT '̯λ��ǰ�ȼ�����';
alter table `roledata` add column `StallLastTime` int(10) UNSIGNED DEFAULT '4325376' NOT NULL COMMENT '̯λ�������ʱ��(Ĭ��2000��1��1��)';

##2009-09-11
#��ӳ��������ֶ�
alter table `pet_data` add column `pet_lock` bool DEFAULT '0' NOT NULL COMMENT '��������' after `pet_state`;


##2009-09-16
#���Ա�
CREATE TABLE `left_msg` (                                                                                    
            `msg_id` int(11) unsigned NOT NULL COMMENT '��������ʱ��',                                           
            `roleid` int(11) unsigned NOT NULL COMMENT '���id',                                                     
            `msg_data` tinyblob COMMENT '��������',                                                                
            PRIMARY KEY (`msg_id`,`roleid`)                                                                            
          ) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='��������' ;
          
##2009-09-21
#ɾ��equip_del����
Alter table `equip_del` drop primary key;

##2009-09-28
#�޸Ľ�ɫְҵ��Ӣ��ְҵ��Ĭ��ֵΪ0
alter table `roledata` drop column `Class`; 
alter table `roledata` add column `Class` tinyint (3) DEFAULT '1' NOT NULL after `RebornMapID`;
alter table `roledata` drop column `ClassEx`; 
alter table `roledata` add column `ClassEx` tinyint (3) DEFAULT '0' NOT NULL after `Class`;

##2009-10-14
#�޸Ľ�ɫְҵ��Ӣ��ְҵ��Ĭ��ֵΪ0
alter table `visiting_card` change `HeadUrl` `HeadUrl` varchar(100) character set utf8 collate utf8_general_ci NULL  comment 'ͷ��url';

#2009-10-14
# rebuild table 'item_del'
drop table `item_del`;
CREATE TABLE `item_del` LIKE `item`;
Alter table `item_del` drop primary key;

#2009-10-19
# ����һ���µ������ֶ�
alter table `roledata` add column `TalentType4` tinyint(2) NOT NULL default '-1' COMMENT '��������' after `TalentType3`;
alter table `roledata` add column `TalentVal4` smallint(4) NOT NULL default '0' COMMENT '����Ͷ��ֵ' after `TalentVal3`;

#2009-10-19
# ����Ԫ��������־��
CREATE TABLE `bill_yuanbao_log` (
  `token_id` char(50) NOT NULL DEFAULT '' COMMENT '����ID',
  `account_id` int(11) unsigned DEFAULT NULL,
  `yuanbao` int(11) DEFAULT NULL,
  `time` char(50) DEFAULT NULL,
  PRIMARY KEY (`token_id`),
  UNIQUE KEY `account_id` (`account_id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='Ԫ������log';  

##2009-10-20
#�޸Ľ�ɫ������
update visiting_card set HeadUrl=null where HeadUrl='N/A';

##2009-10-26
#�޸ĳ���������ʵķ�Χ
alter table `pet_data` change `aptitude` `aptitude` int(4) UNSIGNED NOT NULL comment '��������';

##2009-10-29
#�޸���������ȥ��unique����
alter table `bill_yuanbao_log` drop key `account_id`, add key `account_id` ( `account_id` );

##2009-10-30
#����item����log
CREATE TABLE `bill_item_log` (                 
 `token_id` char(50) NOT NULL COMMENT '����ID',          
 `account_id` int(10) unsigned NOT NULL COMMENT '�ʺ�ID',  
 `item_id` int(10) unsigned NOT NULL COMMENT '��ƷID',     
 `item_num` int(11) NOT NULL COMMENT '��Ʒ����',
 `time` char(50) DEFAULT NULL,             
 PRIMARY KEY (`token_id`),
 KEY `account_id` (`account_id`)               
) ENGINE=MyISAM DEFAULT CHARSET=utf8; 

#ɾ��������(�����֮ǰ������,����!!!)
drop table `bill_item_log`;
drop table `bill_yuanbao_log`;
drop table `bill_item`;
drop table `bill_yuanbao`;

#�������뻮����
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

############################## 1.2.2 end ############################################

############################## 1.2.3 start ##########################################
##2009-11-5
alter table `roledata` change `CloseSGTime` `CloseSGTime` char (20)  DEFAULT '2000-01-01 00:00:00' NOT NULL  COMMENT '�ϴιر�PK������ʱ��'

############################## 1.2.3 end ############################################

############################## 1.2.4 start ##########################################

##2009-11-18
#�޸��˳�����м����ֶε�����
alter table `pet_data` change `talent_count` `talent_count` int(4) default '0' NOT NULL, change `wuxing_energy` `wuxing_energy` int(4) default '0' NOT NULL, change `pet_state` `pet_state` int(1) default '0' NOT NULL;
#���ݿ���������
ALTER TABLE `item_del` ADD KEY (`SerialNum`);
ALTER TABLE `yuanbaoorder` ADD KEY (`OrderMode`);
ALTER TABLE `yuanbaoorder` ADD KEY (`RoleID`);
ALTER TABLE `guild_upgrade` ADD KEY (`guild_id`);	
ALTER TABLE `guild_skill` ADD KEY (`guild_id`);
ALTER TABLE `guild_commodity` ADD KEY (`guild_id`);
ALTER TABLE `commerce_rank` ADD KEY (`guild_id`);
ALTER TABLE `roledata` ADD KEY (`RemoveFlag`);
ALTER TABLE `item_needlog` ADD KEY (`NeedLog`);
ALTER TABLE `group_purchase` ADD KEY (`GuildID`);
ALTER TABLE `group_purchase` ADD KEY (`RoleID`);
ALTER TABLE `group_purchase` ADD KEY (`MallID`);
ALTER TABLE `pet_data` ADD KEY (`master_id`);
ALTER TABLE `yuanbaoorder` ADD KEY (`StartTime`);
ALTER TABLE `account` ADD KEY (`worldname_crc`);
ALTER TABLE `roledata` ADD KEY (`rolename`);
ALTER TABLE `account_common` ADD KEY (`AccountName`);
ALTER TABLE `task_done` ADD KEY (`RoleID`);

############################## 1.2.4 end ############################################
############################## 1.2.5 start ##########################################
#2009-11-23 roledata�����ӽ�ɫ����ֵ
alter table roledata add column `Hostility` tinyint(1) unsigned default '0' NOT NULL COMMENT '����ֵ' after `StallLastTime`;

#2009-11-26 roledata�����ӹر��������Ϊ0ʱ�سǸ��������ϵ�pk��������tick
alter table roledata add column `DeadUnSetSafeGuardCountDown` smallint(4) unsigned default '0' NOT NULL COMMENT '�ر��������Ϊ0ʱ�سǸ��������ϵ�pk��������tick' after `Hostility`;

#2009-11-28 roledata�����Ӽ�¼ʽ���ͷ�
alter table roledata add column  `ItemTransportMapID` int(10) unsigned NOT NULL DEFAULT '3017298127' COMMENT '��¼ʽ���ͷ�mapid' after `DeadUnSetSafeGuardCountDown`;
alter table roledata add column  `ItemTransportX` float unsigned NOT NULL DEFAULT '110000' COMMENT '��¼ʽ���ͷ�X����2200*50' after `ItemTransportMapID`;
alter table roledata add column  `ItemTransportZ` float unsigned NOT NULL DEFAULT '110000' COMMENT '��¼ʽ���ͷ�Z����2200*50' after `ItemTransportX`;
alter table roledata add column  `ItemTransportY` float unsigned NOT NULL DEFAULT '332300' COMMENT '��¼ʽ���ͷ�Y����6646*50' after `ItemTransportZ`;

alter table roledata change `ItemTransportX` `ItemTransportX` float unsigned NOT NULL DEFAULT '110000' COMMENT '��¼ʽ���ͷ�X����2200*50';
alter table roledata change `ItemTransportZ` `ItemTransportZ` float unsigned NOT NULL DEFAULT '110000' COMMENT '��¼ʽ���ͷ�Z����2200*50';
alter table roledata change `ItemTransportY` `ItemTransportY` float unsigned NOT NULL DEFAULT '332300' COMMENT '��¼ʽ���ͷ�Y����6646*50';

#2009-12-15 account_common �޸Ŀ�������
alter table `account_common` change `WareSilver` `WareSilver` bigint(20) default '0' NOT NULL;

#2009-12-17 roledata���޸�DeadUnSetSafeGuardCountDown�ֶ�
alter table roledata change `DeadUnSetSafeGuardCountDown` `DeadUnSetSafeGuardCountDown` smallint(5) signed NOT NULL default '-100' COMMENT '�ر��������Ϊ0ʱ�سǸ��������ϵ�pk��������tick';
############################## 1.2.5 end ############################################



############################## 1.3.0 begin ############################################
#2009-09-17
#��ӷ����ֶ�
alter table `roledata` add column `LoverID` int(10) unsigned NOT NULL DEFAULT '4294967295' COMMENT '���˶�Ӧ��roleidû������4294967295'after `ItemTransportY`;
alter table `roledata` add column `HaveWedding` tinyint(3) unsigned NOT NULL DEFAULT '0' COMMENT '�Ƿ���й�����' after `LoverID`;

# װ������Ǳ��ֵ��������
alter table `equip` add column `PotIncTimes` smallint(10) UNSIGNED DEFAULT '0' NOT NULL COMMENT 'װ��Ǳ��ֵ��������' after `PotValUsed`;
alter table `equip_baibao` add column `PotIncTimes` smallint(10) UNSIGNED DEFAULT '0' NOT NULL COMMENT 'װ��Ǳ��ֵ��������' after `PotValUsed`;
alter table `equip_del` add column `PotIncTimes` smallint(10) UNSIGNED DEFAULT '0' NOT NULL COMMENT 'װ��Ǳ��ֵ��������' after `PotValUsed`;
##2009-12-03
#roledata��������ʵ��ֵ��ʵ�������ֶ�
alter table `roledata` add column `Strength` mediumint(8) DEFAULT '0' NOT NULL COMMENT 'ʵ��ֵ' after `AgilityAdded`;
alter table `roledata` add column `StrengthNum` smallint(6) DEFAULT '0' NOT NULL COMMENT 'ʵ������' after `Strength`;
alter table `roledata` add column `Heroism` mediumint(8) DEFAULT '0' NOT NULL COMMENT 'Ӣ��ֵ' after `StrengthNum`;
alter table `roledata` add column `Wit` mediumint(8) DEFAULT '0' NOT NULL COMMENT 'ı��ֵ' after `Heroism`;
alter table `roledata` add column `Errantry` mediumint(8) DEFAULT '0' NOT NULL COMMENT '����ֵ' after `Wit`;
alter table `roledata` add column `Valor` mediumint(8) DEFAULT '0' NOT NULL COMMENT '��սֵ' after `Errantry`;

# ����ʵ�����м�¼��
CREATE TABLE `strength_rankings` (                     
	`Num` smallint(6) unsigned NOT NULL DEFAULT '0' COMMENT '����',
	`PreNum` smallint(6) unsigned NOT NULL DEFAULT '0' COMMENT '�ϴ�����',
	`Strength` mediumint(8) unsigned NOT NULL DEFAULT '0' COMMENT 'ʵ��ֵ',
	`RoleName` varchar(32) NOT NULL COMMENT '��ɫ����', 
	PRIMARY KEY  (`RoleName`)
	);

# װ����ɫƷ����������
alter table `equip` add column `PurpleQltyIdfPct` int(10) UNSIGNED DEFAULT '0' NOT NULL COMMENT 'װ����ɫƷ����������' after `PotIncTimes`;
alter table `equip_baibao` add column `PurpleQltyIdfPct` int(10) UNSIGNED DEFAULT '0' NOT NULL COMMENT 'װ����ɫƷ����������' after `PotIncTimes`;
alter table `equip_del` add column `PurpleQltyIdfPct` int(10) UNSIGNED DEFAULT '0' NOT NULL COMMENT 'װ����ɫƷ����������' after `PotIncTimes`;
# 2009-12-06
# ��ճ��＼����ȴʱ��
update pet_skill set para1=4294967295 where para1<>4294967295;

# Jason 2009-12-7 ���������͸�������ֶ���roledata���ڡ�
alter table `roledata` add column `LastLessingTime` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '�ϴ���ȡ�����͸���ʱ��' after `HaveWedding`;
alter table `roledata` add column `LastLessingLevel` tinyint(2) unsigned NOT NULL DEFAULT '0' COMMENT '��ȡ���˶��ټ�' after `LastLessingTime`;

alter table `roledata` add column `NeedPrisonRevive` tinyint(3) unsigned NOT NULL DEFAULT '0' COMMENT '�Ƿ���Ҫ��������' after `LastLessingLevel`;

#
alter table `roledata` change `DeadUnSetSafeGuardCountDown` `DeadUnSetSafeGuardCountDown` smallint(5) default '-100' NOT NULL;

############################## 1.3.0 end   ############################################



############################## 1.3.1 begin ############################################
# Jason �ⲿ���ӱ�
/*Table structure for table `external_links` */

DROP TABLE IF EXISTS `external_links`;

CREATE TABLE `external_links` (
  `ID` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `LinkName` varchar(10) NOT NULL DEFAULT '"NoName"' COMMENT '??????',
  `LinkURL` varchar(256) DEFAULT NULL COMMENT '????URL',
  PRIMARY KEY (`ID`),
  KEY `LinkName` (`LinkName`)
) ENGINE=InnoDB AUTO_INCREMENT=6 DEFAULT CHARSET=utf8;


# ��pet_skill������������
alter table `pet_skill` add index `petid` (`petid`);
alter table `pet_skill` engine = MyISAM;
############################## 1.3.1 end   ############################################



