function I1330612_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--���ID�� ������ƷTypeID��������Ʒ�ʣ�����ģʽ��log��������Ϣö��
	role.AddRoleItem(MapID, InstanceID, TargetID, 1340002, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 1220021, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3200709, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 1360101, 1, -1, 8, 420)

	local Exp_Give = role.GetRoleLevelUpExp(MapID, InstanceID, TargetID)
	local Level = role.GetRoleLevel(MapID, InstanceID, TargetID)
	local UseTimes = 7

	Exp_Give = Exp_Give * 30000 / (Level+42)^2 / UseTimes

	role.AddRoleExp(MapID, InstanceID, TargetID, Exp_Give)
end

function I1330612_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 4) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(1330612, 1, "I1330612_GiftBag")
aux.RegisterItemEvent(1330612, 0, "I1330612_CanUseGiftBag")
