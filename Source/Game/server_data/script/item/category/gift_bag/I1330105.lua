function I1330105_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--���ID�� ������ƷTypeID��������Ʒ�ʣ�����ģʽ��log��������Ϣö��
	role.AddRoleItem(MapID, InstanceID, TargetID, 1421009, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 1421013, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 1421017, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 1421021, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 1421033, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 1421037, 1, -1, 8, 420)
	local sex = role.GetRoleSex(TargetID)
	if sex == 0 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 9130024, 1, 1, 8, 420)
	elseif sex == 1 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 9130034, 1, 1, 8, 420)
	end
end

function I1330105_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 7) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(1330105, 1, "I1330105_GiftBag")
aux.RegisterItemEvent(1330105, 0, "I1330105_CanUseGiftBag")
