function I1330021_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--���ID�� ������ƷTypeID��������Ʒ�ʣ�����ģʽ��log��������Ϣö��
	role.AddRoleItem(MapID, InstanceID, TargetID, 1000311, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 1000316, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4008005, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4009003, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 1360133, 10, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 1360123, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 1340001, 10, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3200464, 50, -1, 8, 420)
end

function I1330021_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 8) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(1330021, 1, "I1330021_GiftBag")
aux.RegisterItemEvent(1330021, 0, "I1330021_CanUseGiftBag")
