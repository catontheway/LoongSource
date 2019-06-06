function I1330207_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--���ID�� ������ƷTypeID��������Ʒ�ʣ�����ģʽ��log��������Ϣö��
	role.AddRoleItem(MapID, InstanceID, TargetID, 8800020, 1, 4, 8, 420)
	local sex = role.GetRoleSex(TargetID)
	if sex == 0 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 9130023, 1, 1, 8, 420)
	elseif sex == 1 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 9130033, 1, 1, 8, 420)
	end
end

function I1330207_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 2) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(1330207, 1, "I1330207_GiftBag")
aux.RegisterItemEvent(1330207, 0, "I1330207_CanUseGiftBag")
