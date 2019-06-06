function I1330201_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--���ID�� ������ƷTypeID��������Ʒ�ʣ�����ģʽ��log��������Ϣö��
	role.AddRoleItem(MapID, InstanceID, TargetID, 1330305, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 1330306, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8800019, 1, 3, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 1300102, 1, -1, 8, 420)
end

function I1330201_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 4) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(1330201, 1, "I1330201_GiftBag")
aux.RegisterItemEvent(1330201, 0, "I1330201_CanUseGiftBag")
