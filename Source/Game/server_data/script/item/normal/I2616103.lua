--�ɷ�ʹ�ú���
function i2616103_CanUse(MapID, InstanceID, TypeID, TargetID)
    local bRet, bIgnore = 0, false
    local i = role.GetRoleItemNum(TargetID, 2616101)
    local k = role.GetRoleItemNum(TargetID, 2616102)
	if i < 1 then
           bRet = 32
	end
       if k < 1 then
	    bRet = 32
	end
	--����
	return bRet, bIgnore
end

--ʹ��Ч��
function i2616103_QuestUsable(MapID, InstanceID, TypeID, TargetID)
       role.RemoveFromRole(MapID, InstanceID, TargetID, 2616101, 1, 420)
       role.RemoveFromRole(MapID, InstanceID, TargetID, 2616102, 1, 420)
       role.AddRoleItem(MapID, InstanceID, TargetID, 8970043, 1, -1, 8, 420)
end

--ע��
aux.RegisterItemEvent(2616103, 1, "i2616103_QuestUsable")
aux.RegisterItemEvent(2616103, 0, "i2616103_CanUse")

