function I3200006_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--���ID�� ������ƷTypeID��������Ʒ�ʣ�����ģʽ��log��������Ϣö��
	local rolelevel = role.GetRoleLevel(MapID, InstanceID, TargetID)

	rolelevel = math.floor(rolelevel/10)+1


	role.AddRoleItem(MapID, InstanceID, TargetID, 1000900+rolelevel, 5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 1000506, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 1210001, 1, -1, 8, 420)
end

function I3200006_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 3) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(3200006, 1, "I3200006_GiftBag")
aux.RegisterItemEvent(3200006, 0, "I3200006_CanUseGiftBag")
