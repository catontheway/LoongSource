--�����¹㲥���ƹ���
---------------------------------------------------------------------------------------------
function I7999901_OnUse (MapID, InstanceID, TypeID, RoleID)	--ʹ����Ʒ���Թ㲥1[7999901]
	local MsgID = msg.BeginMsgEvent();
	msg.AddMsgEvent(MsgID,102,100001);
	msg.AddMsgEvent(MsgID,2,RoleID);
	msg.DispatchBroadcast(MsgID,-1,-1,-1);	--ȫ���㲥
end
---------------------------------------------------------------------------------------------
function I7999902_OnUse (MapID, InstanceID, TypeID, RoleID)	--ʹ����Ʒ���Թ㲥2[7999902]
	local MsgID = msg.BeginMsgEvent();
	msg.AddMsgEvent(MsgID,102,100002);
	msg.AddMsgEvent(MsgID,2,RoleID);
	msg.AddMsgEvent(MsgID,4,TypeID);
	msg.DispatchBroadcast(MsgID,-1,-1,-1);
end
---------------------------------------------------------------------------------------------
function I7999903_OnUse (MapID, InstanceID, TypeID, RoleID)	--ʹ����Ʒ���Թ㲥3[7999903]
	local MsgID = msg.BeginMsgEvent();
	msg.AddMsgEvent(MsgID,102,100003);
	msg.AddMsgEvent(MsgID,2,RoleID);
	msg.AddMsgEvent(MsgID,2,RoleID);
	msg.AddMsgEvent(MsgID,4,TypeID);
	msg.DispatchBroadcast(MsgID,-1,-1,-1);
end
---------------------------------------------------------------------------------------------
function I7999904_OnUse (MapID, InstanceID, TypeID, RoleID)	--ʹ����Ʒ���Թ㲥4[7999904]
	local MsgID = msg.BeginMsgEvent();
	msg.AddMsgEvent(MsgID,102,100004);
	msg.DispatchBroadcast(MsgID,-1,-1,-1);
end
---------------------------------------------------------------------------------------------
function I7999905_OnUse (MapID, InstanceID, TypeID, RoleID)	--ʹ����Ʒ���Թ㲥5[7999905]
	local MsgID = msg.BeginMsgEvent();
	msg.AddMsgEvent(MsgID,102,100005);
	msg.DispatchBroadcast(MsgID,-1,-1,-1);
end
---------------------------------------------------------------------------------------------
function I7999906_OnUse (MapID, InstanceID, TypeID, RoleID)	--ʹ����Ʒ���Թ㲥6[7999906]
	local MsgID = msg.BeginMsgEvent();
	msg.AddMsgEvent(MsgID,102,100006);
	msg.AddMsgEvent(MsgID,2,RoleID);
	msg.AddMsgEvent(MsgID,2,RoleID);
	msg.DispatchBroadcast(MsgID,-1,-1,-1);	--ȫ���㲥
end
---------------------------------------------------------------------------------------------
function I7999907_OnUse (MapID, InstanceID, TypeID, RoleID)	--ʹ����Ʒ���Թ㲥7[7999907]
	local MsgID = msg.BeginMsgEvent();
	msg.AddMsgEvent(MsgID,102,100007);
	msg.DispatchBroadcast(MsgID,-1,-1,-1);
end
---------------------------------------------------------------------------------------------
function I7999908_OnUse (MapID, InstanceID, TypeID, RoleID)	--ʹ����Ʒ���Թ㲥8[7999908]
	local MsgID = msg.BeginMsgEvent();
	msg.AddMsgEvent(MsgID,102,100008);
	msg.DispatchBroadcast(MsgID,-1,-1,-1);
end
---------------------------------------------------------------------------------------------


--����������
---------------------------------------------------------------------------------------------
function Bqc11017 (MapID, InstanceID, QuestID, RoleID, NPCID)	--���������� ������-��ƪ[11017]
	local MsgID = msg.BeginMsgEvent();
	msg.AddMsgEvent(MsgID,102,100028);
	msg.AddMsgEvent(MsgID,2,RoleID);
	msg.DispatchBroadcast(MsgID,3017298383--[[����]],InstanceID,-1);
end
---------------------------------------------------------------------------------------------




---------------------------------------------------------------------------------------------
--******************************************�ָ���***************************************************--
--����Ϊ�¼�ע�����ݣ�
--�¼�ע�᣻

--�����¹㲥���ƹ�����ص��¼�ע��
aux.RegisterItemEvent(7999901,1,"I7999901_OnUse");	--ʹ����Ʒ���Թ㲥1[7999901]
aux.RegisterItemEvent(7999902,1,"I7999902_OnUse");	--ʹ����Ʒ���Թ㲥2[7999902]
aux.RegisterItemEvent(7999903,1,"I7999903_OnUse");	--ʹ����Ʒ���Թ㲥3[7999903]
aux.RegisterItemEvent(7999904,1,"I7999904_OnUse");	--ʹ����Ʒ���Թ㲥4[7999904]
aux.RegisterItemEvent(7999905,1,"I7999905_OnUse");	--ʹ����Ʒ���Թ㲥5[7999905]
aux.RegisterItemEvent(7999906,1,"I7999906_OnUse");	--ʹ����Ʒ���Թ㲥6[7999906]
aux.RegisterItemEvent(7999907,1,"I7999907_OnUse");	--ʹ����Ʒ���Թ㲥7[7999907]
aux.RegisterItemEvent(7999908,1,"I7999908_OnUse");	--ʹ����Ʒ���Թ㲥8[7999908]


--������������ص��¼�ע��
aux.RegisterQuestEvent(11017,1 ,"Bqc11017"); --���������� ������-��ƪ[11017]
