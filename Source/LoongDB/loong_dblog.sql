CREATE TABLE `log_money` (
	`ID` int(10) unsigned NOT NULL AUTO_INCREMENT COMMENT '��ˮ��',
	`AccountID` int(10) unsigned NOT NULL COMMENT '�˺�ID',
	`RoleID` int(10) unsigned NOT NULL COMMENT '��ɫID',
	`CmdID` int(10) unsigned NOT NULL COMMENT '����',
	`ConType` tinyint(3) NOT NULL COMMENT '��������',
	`Num` bigint(10) NOT NULL COMMENT '����(+����ʾ���룻-����ʾȡ��)',
	`TotalNum` bigint(20) NOT NULL COMMENT 'ָ����������������',
	`RoleIDRel` int(10) unsigned NOT NULL default '4294967295' COMMENT '�������ɫID',
	`Time` datetime NOT NULL COMMENT 'ʱ��',
	PRIMARY KEY  (`ID`)
	) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT '�������ֿ��Ǯ�仯(��λ����)';
	
	
CREATE TABLE `log_yuanbao` (
	`ID` int(10) unsigned NOT NULL AUTO_INCREMENT COMMENT '��ˮ��',
	`AccountID` int(10) unsigned NOT NULL COMMENT '�˺�ID',
	`RoleID` int(10) unsigned NOT NULL COMMENT '��ɫID',
	`CmdID` int(10) unsigned NOT NULL COMMENT '����',
	`ConType` tinyint(3) NOT NULL COMMENT '��������',
	`Num` int(10) NOT NULL COMMENT '����(+����ʾ���룻-����ʾȡ��)',
	`TotalNum` int(20) NOT NULL COMMENT 'ָ����������������',
	`Time` datetime NOT NULL COMMENT 'ʱ��',
	PRIMARY KEY  (`ID`)
	) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT '�������ٱ���Ԫ���仯';


CREATE TABLE `log_exvolume` (                                       
                `ID` int(10) unsigned NOT NULL AUTO_INCREMENT COMMENT '��ˮ��',   
                `AccountID` int(10) unsigned NOT NULL COMMENT '�˺�ID',           
                `RoleID` int(10) unsigned NOT NULL COMMENT '��ɫID',              
                `CmdID` int(10) unsigned NOT NULL COMMENT '����',                 
                `Num` int(10) NOT NULL COMMENT '����(+����ʾ���룻-����ʾȡ��)',  
                `TotalNum` int(20) NOT NULL COMMENT '����',         
                `Time` datetime NOT NULL COMMENT 'ʱ��',                          
                PRIMARY KEY (`ID`)                                                
              ) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='����仯';
	

CREATE TABLE `log_time_stat` (
	`ID` int(10) unsigned NOT NULL AUTO_INCREMENT COMMENT '��ˮ��',
	`AccountID` int(10) unsigned NOT NULL COMMENT '�˺�ID',
	`RoleID` int(10) unsigned NOT NULL COMMENT '��ɫID',
	`BagSilver` bigint(10) NOT NULL COMMENT '������������',
	`WareSilver` bigint(20) NOT NULL COMMENT '��ɫ�ֿ���������',
	`BagYB` int(10) NOT NULL COMMENT '����Ԫ������',
	`BaiBaoYB` int(10) NOT NULL COMMENT '��ɫ�ֿ�Ԫ������',
	`ExVolume` int(10) NOT NULL COMMENT '��ҽ�ɫ��������',
	`Time` datetime NOT NULL COMMENT 'ʱ��',
	PRIMARY KEY  (`ID`)
	) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT '������Ҷ�ʱ��ǮԪ����¼';
	
	
CREATE TABLE `log_item` (
	`ID` int(10) unsigned NOT NULL AUTO_INCREMENT COMMENT '��ˮ��',
	
	`RoleID` int(10) unsigned NOT NULL COMMENT 'role id',
	`CmdID` int(10) unsigned NOT NULL COMMENT '����',
	`TypeID` int(10) unsigned NOT NULL COMMENT 'type id',	
	`OptNum` smallint(5) NOT NULL COMMENT '�仯����(+����ʾ��ã�-����ʾʧȥ)',

	`Serial1` bigint(20) NOT NULL COMMENT '��Ʒ64λ���',
	`ConType1` tinyint(1) NOT NULL COMMENT '�������(0��ʾû����������)',
	`ResNum1` smallint(5) NOT NULL COMMENT '���ո���',
	
	`Serial2` bigint(20) NOT NULL COMMENT '��Ʒ64λ���(0��ʾû��)',
	`ConType2` tinyint(1) NOT NULL COMMENT '�������(0��ʾû����������)',
	`ResNum2` smallint(5) NOT NULL COMMENT '���ո���',
	
	`RoleIDRel` int(10) unsigned NOT NULL default '4294967295' COMMENT '�������ɫID',

	`Time` datetime NOT NULL COMMENT 'ʱ��',
	PRIMARY KEY  (`ID`)
	) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT '��Ʒ��¼';
	
	
CREATE TABLE `log_itemtimes` (
	`ID` int(10) unsigned NOT NULL AUTO_INCREMENT COMMENT '��ˮ��',
		
	`RoleID` int(10) unsigned NOT NULL COMMENT 'role id',
	`CmdID` int(10) unsigned NOT NULL COMMENT '����',

	`Serial` bigint(20) NOT NULL COMMENT '��Ʒ64λ���',
	`TypeID` int(10) unsigned NOT NULL COMMENT 'type id',
	`ConType` tinyint(1) NOT NULL COMMENT '�������(0��ʾû����������)',
	`UsedTimes` int(10) NOT NULL COMMENT '��ʹ�ô���',
	`MaxUseTimes` int(10) NOT NULL COMMENT '����ʹ�ô���',

	`Time` datetime NOT NULL COMMENT 'ʱ��',
	PRIMARY KEY  (`ID`)
	) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT '��Ʒ��¼';
	
	
CREATE TABLE `log_mallsell` (
	`ID` int(10) unsigned NOT NULL AUTO_INCREMENT COMMENT '��ˮ��',
	
	`CmdID` int(10) unsigned NOT NULL COMMENT '����',
	`Serial` bigint(20) NOT NULL COMMENT '64Ϊ���',
	`TypeID` int(10) unsigned NOT NULL COMMENT '��Ʒid',
	`RoleID` int(10) unsigned NOT NULL COMMENT '������id',
	`Num` smallint(5) NOT NULL COMMENT '����',
	`FstGainTime` char(20) NOT NULL default '2000-00-00 00:00:00' COMMENT '��һ����һ�øõ��ߵ�ʱ��',
	`ExistTime` int(10) unsigned NOT NULL COMMENT 'ʱ��(��λ����)',
	`MaxUseTimes` int(10) NOT NULL COMMENT '����ʹ�ô���',
	`CostYB` int(10) NOT NULL COMMENT '����۸�(�ܼۣ�Ԫ��)',
	`CostExVolume` int(10) NOT NULL COMMENT '�һ����ѵ���������',
	`Time` datetime NOT NULL COMMENT 'ʱ��',
	PRIMARY KEY  (`ID`)
	) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT '�̳���Ʒ���ۼ�¼';
	
CREATE TABLE `log_fund` (                                      
            `guild_id` int(10) unsigned NOT NULL COMMENT '����ID',     
            `account_id` int(10) unsigned NOT NULL COMMENT '�˺�ID',   
            `role_id` int(10) unsigned NOT NULL COMMENT '��ɫID',      
            `cmd_id` int(10) NOT NULL COMMENT '����',                  
            `fund` int(10) NOT NULL COMMENT '�����ʽ�仯',        
            `total_fund` int(10) NOT NULL COMMENT '���ɵ�ǰ�ʽ�',  
            `time` time NOT NULL COMMENT 'logʱ��'                     
          ) ENGINE=MyISAM DEFAULT CHARSET=utf8;

CREATE TABLE `log_material` (                                      
                `guild_id` int(10) unsigned NOT NULL COMMENT '����ID',         
                `account_id` int(10) unsigned NOT NULL COMMENT '�˺�ID',       
                `role_id` int(10) unsigned NOT NULL COMMENT '��ɫID',          
                `cmd_id` int(10) NOT NULL COMMENT '����',                      
                `material` int(10) NOT NULL COMMENT '�����ʲı仯',        
                `total_material` int(10) NOT NULL COMMENT '���ɵ�ǰ�ʲ�',  
                `time` time NOT NULL COMMENT 'logʱ��'                         
              ) ENGINE=MyISAM DEFAULT CHARSET=utf8;

CREATE TABLE `log_reputation` (                                      
                  `guild_id` int(10) unsigned NOT NULL COMMENT '����ID',           
                  `account_id` int(10) unsigned NOT NULL COMMENT '�˺�ID',         
                  `role_id` int(10) unsigned NOT NULL COMMENT '��ɫID',            
                  `cmd_id` int(10) NOT NULL COMMENT '����',                        
                  `reputation` int(10) NOT NULL COMMENT '���������仯',        
                  `total_reputation` int(10) NOT NULL COMMENT '���ɵ�ǰ����',  
                  `time` time NOT NULL COMMENT 'logʱ��'                           
                ) ENGINE=MyISAM DEFAULT CHARSET=utf8;	


CREATE TABLE `log_mallsell_pack` (
	`ID` int(10) unsigned NOT NULL AUTO_INCREMENT COMMENT '��ˮ��',

	`PackID` int(10) unsigned NOT NULL COMMENT '��Ʒ��id',
	`BuyID` int(10) unsigned NOT NULL COMMENT '������role id',
	`ToID` int(10) unsigned NOT NULL COMMENT '�����role id',
	`CostYB` int(10) NOT NULL COMMENT '����۸�(�ܼۣ�Ԫ��)',

	`Time` datetime NOT NULL COMMENT 'ʱ��',
	PRIMARY KEY  (`ID`)
	) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT '�̳ǳ��۵���Ʒ����¼';

CREATE TABLE `log_GMCmd` (
	`ID` int(10) unsigned NOT NULL AUTO_INCREMENT COMMENT '��ˮ��',

	`role_id` int(10) unsigned NOT NULL COMMENT '��ɫid',
	`gm_cmd` varchar(256) NOT NULL COMMENT 'Gm����',
	`error_code` int(10) unsigned NOT NULL COMMENT 'ִ������ķ�����',
	`time` datetime NOT NULL COMMENT 'ʱ��',
	PRIMARY KEY  (`ID`)
	) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT '��ɫʹ��GM�����log';


CREATE TABLE `log_Pet` (
	`ID` int(10) unsigned NOT NULL AUTO_INCREMENT COMMENT '��ˮ��',
	`master_id` int(10) unsigned NOT NULL COMMENT '��ɫid',
	`pet_id` int(10) unsigned NOT NULL COMMENT '����id',
	`cmd_id` int(10) unsigned NOT NULL COMMENT '����',
	PRIMARY KEY  (`ID`)
	) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT '��ɫ�Գ��������log';
