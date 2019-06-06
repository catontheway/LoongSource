function I1330108_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--���ID�� ������ƷTypeID��������Ʒ�ʣ�����ģʽ��log��������Ϣö��
	role.AddRoleItem(MapID, InstanceID, TargetID, 1330313, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 1330314, 1, -1, 8, 420)
end

function I1330108_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 2) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(1330108, 1, "I1330108_GiftBag")
aux.RegisterItemEvent(1330108, 0, "I1330108_CanUseGiftBag")
