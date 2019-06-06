function I1330502_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--���ID�� ������ƷTypeID��������Ʒ�ʣ�����ģʽ��log��������Ϣö��
	role.AddRoleItem(MapID, InstanceID, TargetID, 1320001, 5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 1000601, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 1320021, 2, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 1330503, 1, -1, 8, 420)
end

function I1330502_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 4) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(1330502, 1, "I1330502_GiftBag")
aux.RegisterItemEvent(1330502, 0, "I1330502_CanUseGiftBag")
