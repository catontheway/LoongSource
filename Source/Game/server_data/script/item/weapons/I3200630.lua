--�ϳ�90������ ������ɷ��8200515

--�ɷ�ʹ�ú���
function i3200630_CanUse(MapID, InstanceID, TypeID, TargetID)
    local bRet, bIgnore = 0, false
    local i = role.GetRoleItemNum(TargetID, 3200612)
    local k = role.GetRoleItemNum(TargetID, 3200636)
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
function i3200630_QuestUsable(MapID, InstanceID, TypeID, TargetID)
       role.RemoveFromRole(MapID, InstanceID, TargetID, 3200612, 1, 420)
       role.RemoveFromRole(MapID, InstanceID, TargetID, 3200636, 30, 420)
       role.AddRoleItem(MapID, InstanceID, TargetID, 8200515, 1, -1, 8, 420)
       --֪ͨȫ����������
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 13, 1)
	msg.AddMsgEvent(MsgID, 1, 327)
        msg.AddMsgEvent(MsgID, 2, TargetID)
	msg.AddMsgEvent(MsgID, 4, 8200515)
	msg.DispatchWorldMsgEvent(MsgID)
end

--ע��
aux.RegisterItemEvent(3200630, 1, "i3200630_QuestUsable")
aux.RegisterItemEvent(3200630, 0, "i3200630_CanUse")

