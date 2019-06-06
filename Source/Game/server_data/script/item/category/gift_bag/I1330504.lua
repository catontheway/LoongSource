function I1330504_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--���ID�� ������ƷTypeID��������Ʒ�ʣ�����ģʽ��log��������Ϣö��
	role.AddRoleItem(MapID, InstanceID, TargetID, 1320011, 2, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 1360101, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 1340001, 3, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 1330505, 1, -1, 8, 420)
end

function I1330504_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 4) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(1330504, 1, "I1330504_GiftBag")
aux.RegisterItemEvent(1330504, 0, "I1330504_CanUseGiftBag")
