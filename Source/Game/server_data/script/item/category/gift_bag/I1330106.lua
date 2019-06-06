function I1330106_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--���ID�� ������ƷTypeID��������Ʒ�ʣ�����ģʽ��log��������Ϣö��
	role.AddRoleItem(MapID, InstanceID, TargetID, 1410011, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 1410023, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 1410035, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 1410047, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 1410059, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 1410071, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 1410083, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 1410087, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 1240011, 1, -1, 8, 420)
	local sex = role.GetRoleSex(TargetID)
	if sex == 0 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 9130021, 1, 1, 8, 420)
	elseif sex == 1 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 9130031, 1, 1, 8, 420)
	end
end

function I1330106_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 10) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(1330106, 1, "I1330106_GiftBag")
aux.RegisterItemEvent(1330106, 0, "I1330106_CanUseGiftBag")
