function I3200461_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--���ID�� ������ƷTypeID��������Ʒ�ʣ�����ģʽ��log��������Ϣö��
	role.AddRoleItem(MapID, InstanceID, TargetID, 3200463, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3200464, 20, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3200465, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3200466, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3200462, 1, -1, 8, 420)
end

function I3200461_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 5) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(3200461, 1, "I3200461_GiftBag")
aux.RegisterItemEvent(3200461, 0, "I3200461_CanUseGiftBag")

--��Ʒ��������������ʹ��

function CanUseXinShouLiPin(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	if MapID == 2291722097 or MapID == 2291721329 or MapID == 2291721585 or MapID == 2291720817 or MapID == 2291721073 or MapID == 2291720305 or MapID == 2291720561 then
	    return bRet, bIgnore
	else
	    bRet = 43
	end
	return bRet, bIgnore
end


aux.RegisterItemEvent(3200465, 0, "CanUseXinShouLiPin")
aux.RegisterItemEvent(3200466, 0, "CanUseXinShouLiPin")
