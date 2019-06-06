function I1330109_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--���ID�� ������ƷTypeID��������Ʒ�ʣ�����ģʽ��log��������Ϣö��
	role.AddRoleItem(MapID, InstanceID, TargetID, 1330309, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 1330310, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8800035, 1, 1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8800036, 1, 1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8800037, 1, 1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8800038, 1, 1, 8, 420)
end

function I1330109_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 6) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(1330109, 1, "I1330109_GiftBag")
aux.RegisterItemEvent(1330109, 0, "I1330109_CanUseGiftBag")
