function I1330002_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--���ID�� ������ƷTypeID��������Ʒ�ʣ�����ģʽ��log��������Ϣö��
	role.AddRoleItem(MapID, InstanceID, TargetID, 1340001, 2, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 1330003, 1, -1, 8, 420)

	local temp = TargetID%3

	local sex = role.GetRoleSex(TargetID)

	if sex == 0 then

		if temp==0 then
			role.AddRoleItem(MapID, InstanceID, TargetID, 9140032, 1, 1, 8, 420)
		elseif temp==1 then
			role.AddRoleItem(MapID, InstanceID, TargetID, 9140222, 1, 1, 8, 420)
		elseif temp==2 then
			role.AddRoleItem(MapID, InstanceID, TargetID, 9140242, 1, 1, 8, 420)
		end

	else

		if temp==0 then
			role.AddRoleItem(MapID, InstanceID, TargetID, 9140022, 1, 1, 8, 420)
		elseif temp==1 then
			role.AddRoleItem(MapID, InstanceID, TargetID, 9140212, 1, 1, 8, 420)
		elseif temp==2 then
			role.AddRoleItem(MapID, InstanceID, TargetID, 9140232, 1, 1, 8, 420)
		end
	end
end

function I1330002_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 3) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(1330002, 1, "I1330002_GiftBag")
aux.RegisterItemEvent(1330002, 0, "I1330002_CanUseGiftBag")
