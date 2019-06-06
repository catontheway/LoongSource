--�������(30��)
--�ɷ�ʹ�ú���
function i3303202_CanUse(MapID, InstanceID, TypeID, TargetID)
    local bRet, bIgnore = 0, false
    local i = role.GetRoleItemNum(TargetID, 3303201)
	if i < 1 then
           bRet = 32
	end
	--����
	return bRet, bIgnore
end

--ʹ��Ч��
function i3303202_QuestUsable(MapID, InstanceID, TypeID, TargetID)
	role.RemoveFromRole(MapID, InstanceID, TargetID, 3303201, 1, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3303221, 1, -1, 8, 420)

	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 13, 1)
	msg.AddMsgEvent(MsgID, 1, 2308)
	msg.AddMsgEvent(MsgID, 2, TargetID)
	msg.DispatchWorldMsgEvent(MsgID)
end



--�������(90��)
--�ɷ�ʹ�ú���
function i3303203_CanUse(MapID, InstanceID, TypeID, TargetID)
    local bRet, bIgnore = 0, false
    local i = role.GetRoleItemNum(TargetID, 3303201)
    local k = role.GetRoleItemNum(TargetID, 3303221)
	if i < 1 and k < 1 then
           bRet = 32
	end
	--����
	return bRet, bIgnore
end

--ʹ��Ч��
function i3303203_QuestUsable(MapID, InstanceID, TypeID, TargetID)
	role.RemoveFromRole(MapID, InstanceID, TargetID, 3303201, 1, 420)
	role.RemoveFromRole(MapID, InstanceID, TargetID, 3303221, 1, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3303222, 1, -1, 8, 420)
	--֪ͨȫ����������
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 13, 1)
	msg.AddMsgEvent(MsgID, 1, 2309)
	msg.AddMsgEvent(MsgID, 2, TargetID)
	msg.DispatchWorldMsgEvent(MsgID)
end

--ע��
aux.RegisterItemEvent(3303202, 1, "i3303202_QuestUsable")
aux.RegisterItemEvent(3303202, 0, "i3303202_CanUse")
aux.RegisterItemEvent(3303203, 1, "i3303203_QuestUsable")
aux.RegisterItemEvent(3303203, 0, "i3303203_CanUse")

