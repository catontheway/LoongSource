function I2610200_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--���ID�� ������ƷTypeID��������Ʒ�ʣ�����ģʽ��log��������Ϣö��
	role.AddRoleItem(MapID, InstanceID, TargetID, 2610012, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8900016, 1, -1, 8, 420)

end

function I2610200_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 2) then
		--��ʾ��ұ����ռ䲻��
		bRet, bIgnore = 40, false
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(2610200, 1, "I2610200_GiftBag")
aux.RegisterItemEvent(2610200, 0, "I2610200_CanUseGiftBag")
