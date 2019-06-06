function I1330505_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--���ID�� ������ƷTypeID��������Ʒ�ʣ�����ģʽ��log��������Ϣö��
	role.AddRoleItem(MapID, InstanceID, TargetID, 4100309, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 1140002, 2, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 1350016, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 1350017, 1, -1, 8, 420)
end

function I1330505_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 4) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(1330505, 1, "I1330505_GiftBag")
aux.RegisterItemEvent(1330505, 0, "I1330505_CanUseGiftBag")
