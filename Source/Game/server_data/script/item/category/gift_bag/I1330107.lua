function I1330107_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--���ID�� ������ƷTypeID��������Ʒ�ʣ�����ģʽ��log��������Ϣö��
	local sex = role.GetRoleSex(TargetID)
	if sex == 0 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 9130022, 1, 1, 8, 420)
	elseif sex == 1 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 9130032, 1, 1, 8, 420)
	end
end

function I1330107_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 1) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(1330107, 1, "I1330107_GiftBag")
aux.RegisterItemEvent(1330107, 0, "I1330107_CanUseGiftBag")
