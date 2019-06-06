
--������ʯ
function i3304002_CanUse(MapID, InstanceID, TypeID, TargetID)
    local bRet, bIgnore = 0, false

    local i = role.GetRoleItemNum(TargetID, 3304002)
	if i < 10 then
        bRet = 32
	end

	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 1) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
	end

	--����
	return bRet, bIgnore

end

--ʹ��Ч��
function i3304002_OnUse(MapID, InstanceID, TypeID, TargetID)

    role.RemoveFromRole(MapID, InstanceID, TargetID, 3304002, 9, 420)
    role.AddRoleItem(MapID, InstanceID, TargetID, 3304003, 1, -1, 8, 420)

end

--ע��
aux.RegisterItemEvent(3304002, 1, "i3304002_OnUse")
aux.RegisterItemEvent(3304002, 0, "i3304002_CanUse")

