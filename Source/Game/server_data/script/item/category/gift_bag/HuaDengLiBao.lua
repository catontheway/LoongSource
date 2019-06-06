function I3303136_GiftBag(MapID, InstanceID, TypeID, TargetID)

	role.AddRoleItem(MapID, InstanceID, TargetID, 3303130, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3303129, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3303006, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3303001, 1, -1, 8, 420)

end

function I3303136_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 4) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(3303136, 1, "I3303136_GiftBag")
aux.RegisterItemEvent(3303136, 0, "I3303136_CanUseGiftBag")
