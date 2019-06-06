################################# 1.2.0 start ########################################

##2009-07-03
#�޸�login_log���ʽ
alter table `login_log` drop column `accountName`, drop column `id`;

#�޸�login_log���ʽ
alter table `login_log` add column `accountName` varchar(32) CHARSET utf8 COLLATE utf8_general_ci NOT NULL after `accountID`;

#�޸�game_guarder_log���ʽ
alter table `game_guarder_log` add column `name` varchar(32) CHARSET utf8 COLLATE utf8_general_ci NOT NULL after `account_id`;

##2009-07-11
#����world״̬��
CREATE TABLE `world_state` (
  `worldid` bigint(11) NOT NULL DEFAULT '0' COMMENT '����ID',
  `rolenum` int(11) NOT NULL DEFAULT '0' COMMENT '��ǰ��������',
  `worldstate` tinyint(3) NOT NULL DEFAULT '0' COMMENT '(��ǰ����/��߸���)*100',
  `time` char(20) DEFAULT NULL COMMENT '����ʱ��',
  PRIMARY KEY (`worldid`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='����״̬ ��ͣ��update';


##2009-07-20
#����world״̬log��
CREATE TABLE `world_state_log` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `worldid` bigint(11) NOT NULL DEFAULT '0' COMMENT '����ID',
  `rolenum` int(11) NOT NULL DEFAULT '0' COMMENT '��ǰ��������',
  `worldstate` tinyint(3) NOT NULL DEFAULT '0' COMMENT '(��ǰ����/��߸���)*100',
  `time` char(20) DEFAULT NULL COMMENT '����ʱ��',
  PRIMARY KEY (`id`)  
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='����״̬ ��ͣ��insert';

##2009-07-23
#�޸ĵ���ǳ�log�ı�ṹ
alter table `login_log` change `loginTime` `action` varchar(6) character set utf8 collate utf8_general_ci NOT NULL comment '����', 
						change `logoutTime` `time` datetime NULL  comment 'ʱ��';

################################# 1.2.0 end ##########################################

################################# 1.2.1 start ########################################

##2009-08-28
#�޸ķ�ͣ��ʾλ						
alter table `account` change `forbid` `forbid_mask` tinyint(4) UNSIGNED NOT NULL DEFAULT 0 comment '��ӦĿǰ���ַ�ͣ���ͣ����롣';

##2009-08-28
#����gm��ͣ��Ӧ��Ϸ����ı�
CREATE TABLE `world_forbid` (
  `accountid` int(11) unsigned NOT NULL DEFAULT '0' COMMENT '�˺�id',
  `worldname_crc` int(11) unsigned NOT NULL DEFAULT '0' COMMENT '��Ϸ��������crc',
  PRIMARY KEY (`accountid`,`worldname_crc`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='��¼gm���߷�ͣ�˺�';

##2009-09-07
#����ip��ͣ������
CREATE TABLE `black_list` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `ip` varchar(20) DEFAULT NULL COMMENT '�����ip',
  PRIMARY KEY (`id`),
  UNIQUE KEY `ip` (`ip`)
) ENGINE=MyISAM AUTO_INCREMENT=2 DEFAULT CHARSET=utf8 COMMENT='������';

##2009-09-18
#��ӷ�����ʱ���
CREATE TABLE `fatigue_time` (                                           
                `account_id` int(11) NOT NULL COMMENT '�˺�id',                     
                `acc_online_time` int(11) DEFAULT '0' COMMENT '�ۼ�����ʱ��',   
                `acc_offline_time` int(11) DEFAULT '0' COMMENT '�ۼ�����ʱ��',  
                PRIMARY KEY (`account_id`)                                            
              ) ENGINE=MyISAM DEFAULT CHARSET=utf8;
              
#������ʱ������
INSERT INTO fatigue_time(account_id)  (SELECT id FROM account ) on DUPLICATE key update acc_offline_time = 0, acc_online_time=0; 

################################# 1.2.1 end ##########################################

################################# 1.2.2 start ########################################

##2009-10-09
#�޸Ĳ�ѯ�ϴε�¼ip��time����bug
alter table `account` add column `ip` varchar(20) DEFAULT NULL COMMENT '�ϴε�¼ip' after `mibao`;
alter table `account` add column `time` datetime DEFAULT NULL COMMENT '�ϴε�¼time' after `ip`;

##2009-10-10
#��ӷ�����ʱ���
drop table `fatigue_time`;

##2009-10-12
#��ӷ�����ʱ���
CREATE TABLE `fatigue_time` (                                           
                `accountname_crc` int(11) NOT NULL COMMENT '�ʺ���crc',            
                `acc_online_time` int(11) DEFAULT '0' COMMENT '�ۼ�����ʱ��',   
                `acc_offline_time` int(11) DEFAULT '0' COMMENT '�ۼ�����ʱ��',  
                PRIMARY KEY (`accountname_crc`)                                       
              ) ENGINE=MyISAM DEFAULT CHARSET=utf8;

##2009-10-14
#�޸���accountname_crc������
alter table `fatigue_time` change `accountname_crc` `accountname_crc` int(11) UNSIGNED NOT NULL comment '�ʺ���crc';

################################# 1.2.2 end ##########################################
 


