function I1350322_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--���ID�� ������ƷTypeID��������Ʒ�ʣ�����ģʽ��log��������Ϣö��
	local rolesex = role.GetRoleSex(TargetID)
	if rolesex == 0 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 9900032, 1, 3, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 9900033, 1, 3, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 9900034, 1, 3, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 9900035, 1, 3, 8, 420)
	elseif rolesex == 1 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 9900022, 1, 3, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 9900023, 1, 3, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 9900024, 1, 3, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 9900025, 1, 3, 8, 420)
	end
end

function I11350322_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 4) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(1350322, 1, "I1350322_GiftBag")
aux.RegisterItemEvent(1350322, 0, "I1350322_CanUseGiftBag")
