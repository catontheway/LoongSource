function I1330202_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--���ID�� ������ƷTypeID��������Ʒ�ʣ�����ģʽ��log��������Ϣö��
	role.AddRoleItem(MapID, InstanceID, TargetID, 1421008, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 1421012, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 1421016, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 1421020, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 1421032, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 1421036, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 1230016, 6, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 1300002, 1, -1, 8, 420)
end

function I1330202_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 8) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(1330202, 1, "I1330202_GiftBag")
aux.RegisterItemEvent(1330202, 0, "I1330202_CanUseGiftBag")
