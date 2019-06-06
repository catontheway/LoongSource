function I1330104_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--���ID�� ������ƷTypeID��������Ʒ�ʣ�����ģʽ��log��������Ϣö��
	role.AddRoleItem(MapID, InstanceID, TargetID, 1160101, 10, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 1160102, 10, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 1160103, 10, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 1160104, 10, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 1160105, 10, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 1160106, 10, -1, 8, 420)
end

function I1330104_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 6) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(1330104, 1, "I1330104_GiftBag")
aux.RegisterItemEvent(1330104, 0, "I1330104_CanUseGiftBag")
