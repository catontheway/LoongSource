function I1330210_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--���ID�� ������ƷTypeID��������Ʒ�ʣ�����ģʽ��log��������Ϣö��
	role.AddRoleItem(MapID, InstanceID, TargetID, 1230019, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 1240007, 2, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 1240008, 2, -1, 8, 420)
end

function I1330210_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 3) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(1330210, 1, "I1330210_GiftBag")
aux.RegisterItemEvent(1330210, 0, "I1330210_CanUseGiftBag")
