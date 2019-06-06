function I1350321_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--���ID�� ������ƷTypeID��������Ʒ�ʣ�����ģʽ��log��������Ϣö��
	local rolesex = role.GetRoleSex(TargetID)
	if rolesex == 0 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 9900012, 1, 2, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 9900013, 1, 2, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 9900014, 1, 2, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 9900015, 1, 2, 8, 420)
	elseif rolesex == 1 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 9900002, 1, 2, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 9900003, 1, 2, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 9900004, 1, 2, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 9900005, 1, 2, 8, 420)
	end
end

function I11350321_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 4) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(1350321, 1, "I1350321_GiftBag")
aux.RegisterItemEvent(1350321, 0, "I1350321_CanUseGiftBag")
