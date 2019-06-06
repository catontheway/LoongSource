function I3303053_GiftBag(MapID, InstanceID, TypeID, TargetID)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3303001, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3303005, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3303004, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3303006, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3303009, 5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3303011, 5, -1, 8, 420)
end

function I3303053_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 6) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(3303053, 1, "I3303053_GiftBag")
aux.RegisterItemEvent(3303053, 0, "I3303053_CanUseGiftBag")
