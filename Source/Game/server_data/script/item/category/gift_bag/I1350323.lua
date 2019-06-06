function I1350323_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--���ID�� ������ƷTypeID��������Ʒ�ʣ�����ģʽ��log��������Ϣö��
	local rolesex = role.GetRoleSex(TargetID)
	if rolesex == 0 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 9900052, 1, 4, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 9900053, 1, 4, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 9900054, 1, 4, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 9900055, 1, 4, 8, 420)
	elseif rolesex == 1 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 9900042, 1, 4, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 9900043, 1, 4, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 9900044, 1, 4, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 9900045, 1, 4, 8, 420)
	end
end

function I1350323_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 4) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(1350323, 1, "I1350323_GiftBag")
aux.RegisterItemEvent(1350323, 0, "I1350323_CanUseGiftBag")
