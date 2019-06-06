############################## 1.1.25 start #########################################

##2009-06-24
#�޸�Ȩ�ޱ����ֶ���
alter table `privilege` change `pri_s` `pri_3` int (4)  DEFAULT '1' NOT NULL;

############################## 1.1.25 end ###########################################

############################## 1.1.26 start #########################################
##2009-06-25
#�޸�Ȩ�ޱ����ֶ�
alter table `privilege` 
	drop column `pri_0`,
	add column `pri_4` int (4)  DEFAULT '0' NOT NULL  after `pri_3`, 
	add column `pri_5` int (4)  DEFAULT '0' NOT NULL  after `pri_4`,
	add column `pri_6` int (4)  DEFAULT '0' NOT NULL  after `pri_5`, 
	add column `pri_7` int (4)  DEFAULT '0' NOT NULL  after `pri_6`,
	change `pri_1` `pri_1` int (4)  DEFAULT '0' NOT NULL , 
	change `pri_2` `pri_2` int (4)  DEFAULT '0' NOT NULL , 
	change `pri_3` `pri_3` int (4)  DEFAULT '0' NOT NULL ;

##2009-06-26
#�����ά�ͻ���ip�󶨱�
CREATE TABLE `rt_iptable` (                   
	`name` char(32) NOT NULL COMMENT '�˺���',  
	`ip` char(30) NOT NULL COMMENT '��ip',    
	PRIMARY KEY (`name`)                        
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

##2009-06-29
#���GM����Log��
CREATE TABLE `gm_log` (                                                                                                                                                                                                                                                                                                                                              
          `AccountID` int(10) unsigned NOT NULL COMMENT '�������˺�ID',                                                                                                                                                                                                                                                                                                 
          `Name` char(32) NOT NULL COMMENT '�������˺�����',                                                                                                                                                                                                                                                                                                          
          `Time` datetime NOT NULL COMMENT 'Log��¼ʱ��',                                                                                                                                                                                                                                                                                                                
          `OperIP` char(32) NOT NULL COMMENT '������IP',                                                                                                                                                                                                                                                                                                                  
          `Action` enum('����GM����','����GM�޸�','����GMɾ��','��ɫ�ָ�','�����޸�','��������','����ɾ��','����˫��','˫��ɾ��','�Զ�ѭ������','�Զ�ѭ������ɾ��','��ɫ����','���½ǹ���','���½ǹ���ɾ��','����������','�رշ�����','ǿ�ƹرշ�����','��IP') NOT NULL COMMENT '��������',  
          `Details` varchar(512) NOT NULL COMMENT '��������'                                                                                                                                                                                                                                                                                                             
        ) ENGINE=MyISAM DEFAULT CHARSET=utf8;


############################## 1.1.26 end ###########################################

############################## 1.2.0 start ###########################################

##2009-07-01
#׷�Ӱ�IP��Ȩ��
insert into `privilege` values ('NRTC_BindIP',0,0,0,0,0,0,1);

#�޸�Ȩ�ޱ����ֶ�
alter table `privilege` 
	add column `pri_8` int (4)  DEFAULT '0' NOT NULL  after `pri_7`;

############################## 1.2.0 end #############################################

############################## 1.2.1 start ###########################################

##2009-08-20
#����������Ϣ
insert into `privilege` values ('NRTC_SystemCmd',0,0,0,0,0,0,1,0);
insert into `privilege` values ('NRTC_MaxNum',0,0,0,0,0,0,0,1);

#�޸�Ȩ�ޱ����ֶ�,����9���˺�
alter table `privilege` 
	add column `pri_9` int (4)  DEFAULT '0' NOT NULL  after `pri_8`;
	
#������Ϣ
insert into `privilege` values ('NRTC_UpdateMall',0,0,0,0,0,0,0,0,1);

############################## 1.2.1 end #############################################

############################## 1.2.2 start ###########################################

# log�е�Actionλ����һ���¼�
alter table `gm_log` change `Action` `Action` enum ('����GM����','����GM�޸�','����GMɾ��','��ɫ�ָ�','�����޸�','��������','����ɾ��','����˫��','˫��ɾ��','�Զ�ѭ������','�Զ�ѭ������ɾ��','��ɫ����','���½ǹ���','���½ǹ���ɾ��','����������','�رշ�����','ǿ�ƹرշ�����','��IP','����ϵͳ����');

# ���ӵ��߻ָ�����Ϣ
insert into `privilege` values ('NGMTC_ItemResume',0,0,0,1,0,0,0,0,0);

# ����������Ϣ
insert into `privilege` values ('NGMTC_AccountForbid',0,1,0,0,0,0,0,0,0);
insert into `privilege` values ('NGMTC_Blacklist',0,0,1,0,0,0,0,0,0);

############################## 1.2.2 end #############################################

############################## 1.2.3 start ###########################################

# ����ȷ�ϸ���Ϣ��Ȩ��
truncate table privilege��

insert into `privilege` values 

('NGMTC_AccountForbid',0,1,1,1,1,0,0,0,0),
('NGMTC_AutoNotice',1,1,1,1,1,0,0,0,0),
('NGMTC_Blacklist',0,0,1,1,1,0,0,0,0),
('NGMTC_Double',0,1,1,1,1,0,0,0,0),
('NGMTC_GMCreate',0,0,0,0,1,0,1,0,0),
('NGMTC_GMDelete',0,0,0,0,1,0,1,0,0),
('NGMTC_GMModify',0,0,0,0,1,0,1,0,0),
('NGMTC_ItemCreate',0,0,0,1,1,0,0,0,0),
('NGMTC_ItemDelete',0,0,1,1,1,0,0,0,0),
('NGMTC_ItemResume',0,0,0,1,1,0,0,0,0),
('NGMTC_ProModify',0,0,0,1,1,0,0,0,0),
('NGMTC_RightNotice',1,1,1,1,1,0,0,0,0),
('NGMTC_RoleResume',0,0,1,1,1,0,0,0,0),
('NRTC_BindIP',0,0,0,0,0,0,1,0,0),
('NRTC_Challenge',1,1,1,1,1,1,1,1,1),
('NRTC_Close',0,0,0,0,0,1,1,0,0),
('NRTC_MaxNum',0,0,0,0,0,0,0,1,0),
('NRTC_Start',0,0,0,0,0,1,1,0,0),
('NRTC_SystemCmd',0,0,0,0,0,0,1,0,0),
('NRTC_UpdateMall',0,0,0,0,0,0,0,0,1);

# �����趨��ȡ����ʱ����Ƶ��������Ϣ���Լ�ȡ�����½ǹ�����Ϣ
insert into `privilege` values ('NGMTC_AutoChatNotice',0,1,1,1,1,0,0,0,0);
insert into `privilege` values ('NGMTC_CancelAutoChatNotice',0,1,1,1,1,0,0,0,0);
insert into `privilege` values ('NGMTC_CancelRightNotice',1,1,1,1,1,0,0,0,0);

############################## 1.2.3 end #############################################

############################## 1.2.4 start ###########################################
# log�е�Actionλ����һ���¼�
alter table `gm_log` change `Action` `Action` enum ('����GM����','����GM�޸�','����GMɾ��','��ɫ�ָ�','�����޸�','��������','����ɾ��','����˫��','˫��ɾ��','�Զ�ѭ������','�Զ�ѭ������ɾ��','���½ǹ���','���½ǹ���ɾ��','����Ƶ������','ȡ������Ƶ������','����������','�رշ�����','ǿ�ƹرշ�����','��IP','����ϵͳ����');
alter table `gm_log` change `Action` `Action` enum ('create GM account','modify GM account','delete GM account','recover role','modify role attribute','create object','delete object','double','cancel double','auto notice','cancel auto notice','right notice','cancel right notice','auto chat notice','cancel auto chat notice','start server','close server','close server forced','IP bind','send system cmd');

############################## 1.2.4 end #############################################

############################## 1.2.5 start #############################################

insert into `privilege` values 

('NGMTS_GMSetFackNumer',0,0,0,0,0,0,0,1,0)

############################## 1.2.5 end #############################################