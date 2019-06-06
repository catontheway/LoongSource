function I1330111_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--���ID�� ������ƷTypeID��������Ʒ�ʣ�����ģʽ��log��������Ϣö��
	role.AddRoleItem(MapID, InstanceID, TargetID, 8800039, 1, 2, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3200604, 1, -1, 8, 420)
end

function I1330111_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 2) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(1330111, 1, "I1330111_GiftBag")
aux.RegisterItemEvent(1330111, 0, "I1330111_CanUseGiftBag")
