function I1330103_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--���ID�� ������ƷTypeID��������Ʒ�ʣ�����ģʽ��log��������Ϣö��
	role.AddRoleItem(MapID, InstanceID, TargetID, 1330303, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 1330304, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8800015, 1, 2, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8800016, 1, 2, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8800017, 1, 2, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8800018, 1, 2, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 1340001, 2, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 1320001, 10, -1, 8, 420)
end

function I1330103_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 8) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(1330103, 1, "I1330103_GiftBag")
aux.RegisterItemEvent(1330103, 0, "I1330103_CanUseGiftBag")
