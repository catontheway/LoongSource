function I1330209_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--���ID�� ������ƷTypeID��������Ʒ�ʣ�����ģʽ��log��������Ϣö��
	role.AddRoleItem(MapID, InstanceID, TargetID, 1330311, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 1330312, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8800035, 1, 2, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8800036, 1, 2, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8800037, 1, 2, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8800038, 1, 2, 8, 420)
end

function I1330209_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 6) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(1330209, 1, "I1330209_GiftBag")
aux.RegisterItemEvent(1330209, 0, "I1330209_CanUseGiftBag")
