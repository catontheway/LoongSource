function I1350041_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--���ID�� ������ƷTypeID��������Ʒ�ʣ�����ģʽ��log��������Ϣö��

	local jiyuid = 1350200
	role.AddRoleItem(MapID, InstanceID, TargetID, jiyuid+math.random(11), 1, -1, 8, 420)


end

function I1350041_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 1) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(1350041, 1, "I1350041_GiftBag")
aux.RegisterItemEvent(1350041, 0, "I1350041_CanUseGiftBag")
