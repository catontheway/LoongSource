--�ϳ�100������ ����������8200616

--�ɷ�ʹ�ú���
function i3200625_CanUse(MapID, InstanceID, TypeID, TargetID)
    local bRet, bIgnore = 0, false
    local i = role.GetRoleItemNum(TargetID, 3200617)
    local k = role.GetRoleItemNum(TargetID, 3200637)
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
function i3200625_QuestUsable(MapID, InstanceID, TypeID, TargetID)
       role.RemoveFromRole(MapID, InstanceID, TargetID, 3200617, 1, 420)
       role.RemoveFromRole(MapID, InstanceID, TargetID, 3200637, 30, 420)
       role.AddRoleItem(MapID, InstanceID, TargetID, 8200616, 1, -1, 8, 420)
       --֪ͨȫ����������
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 13, 1)
	msg.AddMsgEvent(MsgID, 1, 327)
        msg.AddMsgEvent(MsgID, 2, TargetID)
	msg.AddMsgEvent(MsgID, 4, 8200616)
	msg.DispatchWorldMsgEvent(MsgID)
end

--ע��
aux.RegisterItemEvent(3200625, 1, "i3200625_QuestUsable")
aux.RegisterItemEvent(3200625, 0, "i3200625_CanUse")

