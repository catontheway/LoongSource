
--��������ѧϰ����

--����

function s1120104_onchange(MapID,InstanceID,RoleID,LastLevel,CurLevel)

	if LastLevel == 4 and CurLevel == 5 then
	local MsgID = msg.BeginMsgEvent()		--���##�ɹ��Ľ������еĻ���Ѩ��ȫ��ͨ�������õ������Ե�������
	msg.AddMsgEvent(MsgID, 100, 3501)
	msg.AddMsgEvent(MsgID, 2, RoleID)
	msg.DispatchWorldMsgEvent(MsgID)
	end
end
aux.RegisterSkillEvent(1120104, 4, "s1120104_onchange")



function s1120209_onchange(MapID,InstanceID,RoleID,LastLevel,CurLevel)

	if LastLevel == 9 and CurLevel == 10 then
	local MsgID = msg.BeginMsgEvent()		--���##�ɹ��Ľ������еĹ�ԪѨ��ȫ��ͨ�������õ��˴���ȵ�������
	msg.AddMsgEvent(MsgID, 100, 3502)
	msg.AddMsgEvent(MsgID, 2, RoleID)
	msg.DispatchWorldMsgEvent(MsgID)
	end
end
aux.RegisterSkillEvent(1120209, 4, "s1120209_onchange")



function s1120609_onchange(MapID,InstanceID,RoleID,LastLevel,CurLevel)
	if LastLevel == 9 and CurLevel == 10 then
	local MsgID = msg.BeginMsgEvent()		--���##�ɹ��Ľ������еĽ���Ѩ��ȫ��ͨ�������õ��˴���ȵ�������
	msg.AddMsgEvent(MsgID, 100, 3503)
	msg.AddMsgEvent(MsgID, 2, RoleID)
	msg.DispatchWorldMsgEvent(MsgID)
	end
end
aux.RegisterSkillEvent(1120609, 4, "s1120609_onchange")


function s1121009_onchange(MapID,InstanceID,RoleID,LastLevel,CurLevel)
	if LastLevel == 9 and CurLevel == 10 then
	local MsgID = msg.BeginMsgEvent()		--���##�ɹ��Ľ������е��βѨ��ȫ��ͨ�������õ��˴���ȵ�������
	msg.AddMsgEvent(MsgID, 100, 3504)
	msg.AddMsgEvent(MsgID, 2, RoleID)
	msg.DispatchWorldMsgEvent(MsgID)
	end
end
aux.RegisterSkillEvent(1121009, 4, "s1121009_onchange")


function s1121304_onchange(MapID,InstanceID,RoleID,LastLevel,CurLevel)
	if LastLevel == 4 and CurLevel == 5 then
	local MsgID = msg.BeginMsgEvent()		--���##�ɹ��Ľ������еĻ���Ѩ��ȫ��ͨ�������õ��˴���ȵ�������
	msg.AddMsgEvent(MsgID, 100, 3505)
	msg.AddMsgEvent(MsgID, 2, RoleID)
	msg.DispatchWorldMsgEvent(MsgID)
	end
end
aux.RegisterSkillEvent(1121304, 4, "s1121304_onchange")


function s1121607_onchange(MapID,InstanceID,RoleID,LastLevel,CurLevel)
	if LastLevel == 7 and CurLevel == 8 then
	local MsgID = msg.BeginMsgEvent()		--���##�ɹ��Ľ������е���ȪѨ��ȫ��ͨ�������õ��˴���ȵ�������
	msg.AddMsgEvent(MsgID, 100, 3506)
	msg.AddMsgEvent(MsgID, 2, RoleID)
	msg.DispatchWorldMsgEvent(MsgID)
	end
end
aux.RegisterSkillEvent(1121607, 4, "s1121607_onchange")


---����

function s1121704_onchange(MapID,InstanceID,RoleID,LastLevel,CurLevel)
	if LastLevel == 4 and CurLevel == 5 then
	local MsgID = msg.BeginMsgEvent()		--���##�ɹ��Ľ������еĳ�ǿѨ��ȫ��ͨ�������õ������Ե�������
	msg.AddMsgEvent(MsgID, 100, 3507)
	msg.AddMsgEvent(MsgID, 2, RoleID)
	msg.DispatchWorldMsgEvent(MsgID)
	end
end
aux.RegisterSkillEvent(1121704, 4, "s1121704_onchange")


function s1121904_onchange(MapID,InstanceID,RoleID,LastLevel,CurLevel)
	if LastLevel == 4 and CurLevel == 5 then
	local MsgID = msg.BeginMsgEvent()		--���##�ɹ��Ľ������е�����Ѩ��ȫ��ͨ�������õ������Ե�������
	msg.AddMsgEvent(MsgID, 100, 3508)
	msg.AddMsgEvent(MsgID, 2, RoleID)
	msg.DispatchWorldMsgEvent(MsgID)
	end
end
aux.RegisterSkillEvent(1121904, 4, "s1121904_onchange")


function s1122107_onchange(MapID,InstanceID,RoleID,LastLevel,CurLevel)
	if LastLevel == 7 and CurLevel == 8 then
	local MsgID = msg.BeginMsgEvent()		--���##�ɹ��Ľ������еļ���Ѩ��ȫ��ͨ�������õ��˴���ȵ�������
	msg.AddMsgEvent(MsgID, 100, 3509)
	msg.AddMsgEvent(MsgID, 2, RoleID)
	msg.DispatchWorldMsgEvent(MsgID)
	end
end
aux.RegisterSkillEvent(1122107, 4, "s1122107_onchange")


function s1122204_onchange(MapID,InstanceID,RoleID,LastLevel,CurLevel)
	if LastLevel == 4 and CurLevel == 5 then
	local MsgID = msg.BeginMsgEvent()		--���##�ɹ��Ľ������е�����Ѩ��ȫ��ͨ�������õ������Ե�������
	msg.AddMsgEvent(MsgID, 100, 3510)
	msg.AddMsgEvent(MsgID, 2, RoleID)
	msg.DispatchWorldMsgEvent(MsgID)
	end
end
aux.RegisterSkillEvent(1122204, 4, "s1122204_onchange")

function s1122609_onchange(MapID,InstanceID,RoleID,LastLevel,CurLevel)
	if LastLevel == 9 and CurLevel == 10 then
	local MsgID = msg.BeginMsgEvent()		--���##�ɹ��Ľ������е�����Ѩ��ȫ��ͨ�������õ��˴���ȵ�������
	msg.AddMsgEvent(MsgID, 100, 3511)
	msg.AddMsgEvent(MsgID, 2, RoleID)
	msg.DispatchWorldMsgEvent(MsgID)
	end
end
aux.RegisterSkillEvent(1122609, 4, "s1122609_onchange")



function s1122704_onchange(MapID,InstanceID,RoleID,LastLevel,CurLevel)
	if LastLevel == 4 and CurLevel == 5 then
	local MsgID = msg.BeginMsgEvent()		--���##�ɹ��Ľ������еĴ�׵Ѩ��ȫ��ͨ�������õ������Ե�������
	msg.AddMsgEvent(MsgID, 100, 3512)
	msg.AddMsgEvent(MsgID, 2, RoleID)
	msg.DispatchWorldMsgEvent(MsgID)
	end
end
aux.RegisterSkillEvent(1122704, 4, "s1122704_onchange")


function s1122804_onchange(MapID,InstanceID,RoleID,LastLevel,CurLevel)
	if LastLevel == 4 and CurLevel == 5 then
	local MsgID = msg.BeginMsgEvent()		--���##�ɹ��Ľ������е�����Ѩ��ȫ��ͨ�������õ��˴���ȵ�������
	msg.AddMsgEvent(MsgID, 100, 3513)
	msg.AddMsgEvent(MsgID, 2, RoleID)
	msg.DispatchWorldMsgEvent(MsgID)
	end
end
aux.RegisterSkillEvent(1122804, 4, "s1122804_onchange")


function s1122909_onchange(MapID,InstanceID,RoleID,LastLevel,CurLevel)
	if LastLevel == 9 and CurLevel == 10 then
	local MsgID = msg.BeginMsgEvent()		--���##�ɹ��Ľ������еİٻ�Ѩ��ȫ��ͨ�������õ��˴���ȵ�������
	msg.AddMsgEvent(MsgID, 100, 3514)
	msg.AddMsgEvent(MsgID, 2, RoleID)
	msg.DispatchWorldMsgEvent(MsgID)
	end
end
aux.RegisterSkillEvent(1122909, 4, "s1122909_onchange")



function s1123209_onchange(MapID,InstanceID,RoleID,LastLevel,CurLevel)
	if LastLevel == 9 and CurLevel == 10 then
	local MsgID = msg.BeginMsgEvent()		--���##�ɹ��Ľ������е���ͥѨ��ȫ��ͨ�������õ��˴���ȵ�������
	msg.AddMsgEvent(MsgID, 100, 3515)
	msg.AddMsgEvent(MsgID, 2, RoleID)
	msg.DispatchWorldMsgEvent(MsgID)
	end
end
aux.RegisterSkillEvent(1123209, 4, "s1123209_onchange")











