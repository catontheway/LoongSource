--�ϳ�70������ ��ԯ���佣 8200309

--�ɷ�ʹ�ú���
function i3200548_CanUse(MapID, InstanceID, TypeID, TargetID)
    local bRet, bIgnore = 0, false
    local i = role.GetRoleItemNum(TargetID, 3200522)
    local k = role.GetRoleItemNum(TargetID, 3200563)
	if i ~= 1 then
           bRet = 32
	end
       if k <=29 then
	    bRet = 32
	end
	--����
	return bRet, bIgnore
end

--ʹ��Ч��
function i3200548_QuestUsable(MapID, InstanceID, TypeID, TargetID)
       role.RemoveFromRole(MapID, InstanceID, TargetID, 3200522, 1, 420)
       role.RemoveFromRole(MapID, InstanceID, TargetID, 3200563, 30, 420)
       role.AddRoleItem(MapID, InstanceID, TargetID, 8200309, 1, -1, 8, 420)
       --֪ͨȫ����������
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 13, 1)
	msg.AddMsgEvent(MsgID, 1, 327)
        msg.AddMsgEvent(MsgID, 2, TargetID)
	msg.AddMsgEvent(MsgID, 4, 8200309)
	msg.DispatchWorldMsgEvent(MsgID)
end

--ע��
aux.RegisterItemEvent(3200548, 1, "i3200548_QuestUsable")
aux.RegisterItemEvent(3200548, 0, "i3200548_CanUse")

