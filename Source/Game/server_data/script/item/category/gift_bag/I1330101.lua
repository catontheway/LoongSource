function I1330101_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--���ID�� ������ƷTypeID��������Ʒ�ʣ�����ģʽ��log��������Ϣö��
	role.AddRoleItem(MapID, InstanceID, TargetID, 1330301, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 1330302, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8800019, 1, 2, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 1300102, 2, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 1340001, 2, -1, 8, 420)
end

function I1330101_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 5) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(1330101, 1, "I1330101_GiftBag")
aux.RegisterItemEvent(1330101, 0, "I1330101_CanUseGiftBag")
