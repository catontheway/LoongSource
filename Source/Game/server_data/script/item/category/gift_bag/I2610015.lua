function I2610015_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--���ID�� ������ƷTypeID��������Ʒ�ʣ�����ģʽ��log��������Ϣö��
	role.AddRoleItem(MapID, InstanceID, TargetID, 1320001, 5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 1330001, 1, -1, 8, 420)

	local temp = TargetID%3

	local sex = role.GetRoleSex(TargetID)

	if sex == 0 then

		if temp==0 then
			role.AddRoleItem(MapID, InstanceID, TargetID, 9140033, 1, 1, 8, 420)
		elseif temp==1 then
			role.AddRoleItem(MapID, InstanceID, TargetID, 9140223, 1, 1, 8, 420)
		elseif temp==2 then
			role.AddRoleItem(MapID, InstanceID, TargetID, 9140243, 1, 1, 8, 420)
		end

	else

		if temp==0 then
			role.AddRoleItem(MapID, InstanceID, TargetID, 9140023, 1, 1, 8, 420)
		elseif temp==1 then
			role.AddRoleItem(MapID, InstanceID, TargetID, 9140213, 1, 1, 8, 420)
		elseif temp==2 then
			role.AddRoleItem(MapID, InstanceID, TargetID, 9140233, 1, 1, 8, 420)
		end
	end
end

function I2610015_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 3) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(2610015, 1, "I2610015_GiftBag")
aux.RegisterItemEvent(2610015, 0, "I2610015_CanUseGiftBag")
