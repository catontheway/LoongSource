function Marriage_GiftBag(MapID, InstanceID, TypeID, TargetID)
    local sex = role.GetRoleSex(TargetID)
	if sex == 1 then
	--���ID�� ������ƷTypeID��������Ʒ�ʣ�����ģʽ��log��������Ϣö��
	role.AddRoleItem(MapID, InstanceID, TargetID, 9140081, 1, 1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 9140082, 1, 1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 9140083, 1, 1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 9140084, 1, 1, 8, 420)
	elseif sex == 0 then
	--���ID�� ������ƷTypeID��������Ʒ�ʣ�����ģʽ��log��������Ϣö��
	role.AddRoleItem(MapID, InstanceID, TargetID, 9140091, 1, 1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 9140092, 1, 1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 9140093, 1, 1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 9140094, 1, 1, 8, 420)
	end
end

function Marriage_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 4) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(1361111, 1, "Marriage_GiftBag")
aux.RegisterItemEvent(1361111, 0, "Marriage_CanUseGiftBag")
aux.RegisterItemEvent(1361112, 1, "Marriage_GiftBag")
aux.RegisterItemEvent(1361112, 0, "Marriage_CanUseGiftBag")


