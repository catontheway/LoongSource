function I1330004_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--���ID�� ������ƷTypeID��������Ʒ�ʣ�����ģʽ��log��������Ϣö��
	role.AddRoleItem(MapID, InstanceID, TargetID, 1140001, 10, -1, 8, 420)

	local temp = TargetID%3

	local sex = role.GetRoleSex(TargetID)

	if sex == 0 then

		if temp==0 then
			role.AddRoleItem(MapID, InstanceID, TargetID, 9140031, 1, 1, 8, 420)
		elseif temp==1 then
			role.AddRoleItem(MapID, InstanceID, TargetID, 9140221, 1, 1, 8, 420)
		elseif temp==2 then
			role.AddRoleItem(MapID, InstanceID, TargetID, 9140241, 1, 1, 8, 420)
		end

	else

		if temp==0 then
			role.AddRoleItem(MapID, InstanceID, TargetID, 9140021, 1, 1, 8, 420)
		elseif temp==1 then
			role.AddRoleItem(MapID, InstanceID, TargetID, 9140211, 1, 1, 8, 420)
		elseif temp==2 then
			role.AddRoleItem(MapID, InstanceID, TargetID, 9140231, 1, 1, 8, 420)
		end
	end
end

function I1330004_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 2) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(1330004, 1, "I1330004_GiftBag")
aux.RegisterItemEvent(1330004, 0, "I1330004_CanUseGiftBag")
