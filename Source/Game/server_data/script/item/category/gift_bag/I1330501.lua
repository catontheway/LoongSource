function I1330501_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--���ID�� ������ƷTypeID��������Ʒ�ʣ�����ģʽ��log��������Ϣö��
	role.AddRoleItem(MapID, InstanceID, TargetID, 1360003, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 2610158, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3200710, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 1330502, 1, -1, 8, 420)
end

function I1330501_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 4) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(1330501, 1, "I1330501_GiftBag")
aux.RegisterItemEvent(1330501, 0, "I1330501_CanUseGiftBag")
