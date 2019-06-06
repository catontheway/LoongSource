############################## 1.2.1 start ##########################################

##2009-07-28
#log_item, log_money
alter table `log_item` add `RoleIDRel` int(10) unsigned NOT NULL default '4294967295' COMMENT '�������ɫID' after `ResNum2`;
alter table `log_money` add `RoleIDRel` int(10) unsigned NOT NULL default '4294967295' COMMENT '�������ɫID' after `TotalNum`;

##2009-08-03
#log_fund, log_material, log_reputation
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

##2009-08-13
#������`log_mallsell_pack`
CREATE TABLE `log_mallsell_pack` (
	`ID` int(10) unsigned NOT NULL AUTO_INCREMENT COMMENT '��ˮ��',

	`PackID` int(10) unsigned NOT NULL COMMENT '��Ʒ��id',
	`BuyID` int(10) unsigned NOT NULL COMMENT '������role id',
	`ToID` int(10) unsigned NOT NULL COMMENT '�����role id',
	`CostYB` int(10) NOT NULL COMMENT '����۸�(�ܼۣ�Ԫ��)',

	`Time` datetime NOT NULL COMMENT 'ʱ��',
	PRIMARY KEY  (`ID`)
	) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT '�̳ǳ��۵���Ʒ����¼';

############################## 1.2.1 end ###########################################

############################## 1.2.3 start #########################################

CREATE TABLE `log_GMCmd` (
	`ID` int(10) unsigned NOT NULL AUTO_INCREMENT COMMENT '��ˮ��',

	`role_id` int(10) unsigned NOT NULL COMMENT '��ɫid',
	`gm_cmd` varchar(256) NOT NULL COMMENT 'Gm����',
	`error_code` int(10) unsigned NOT NULL COMMENT 'ִ������ķ�����',
	`time` datetime NOT NULL COMMENT 'ʱ��',
	PRIMARY KEY  (`ID`)
	) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT '��ɫʹ��GM�����log';

############################## 1.2.3 end ###########################################
############################## 1.2.5 start #########################################
CREATE TABLE `log_Pet` (
	`ID` int(10) unsigned NOT NULL AUTO_INCREMENT COMMENT '��ˮ��',
	`master_id` int(10) unsigned NOT NULL COMMENT '��ɫid',
	`pet_id` int(10) unsigned NOT NULL COMMENT '����id',
	`cmd_id` int(10) unsigned NOT NULL COMMENT '����',
	PRIMARY KEY  (`ID`)
	) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT '��ɫ�Գ��������log';
############################## 1.2.5 end ###########################################
