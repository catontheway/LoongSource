function ZhiYeXiangLian_Use(MapID, InstanceID, TypeID, TargetID)
    local ItemID = ( TypeID - 1361011 ) * 100 + 8520003
	--���ID�� ������ƷTypeID��������Ʒ�ʣ�����ģʽ��log��������Ϣö��
	role.AddRoleItem(MapID, InstanceID, TargetID, ItemID, 1, -1, 8, 420)
end

function ZhiYeXiangLian_CanUse(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 1) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(1361011, 1, "ZhiYeXiangLian_Use")
aux.RegisterItemEvent(1361011, 0, "ZhiYeXiangLian_CanUse")
aux.RegisterItemEvent(1361012, 1, "ZhiYeXiangLian_Use")
aux.RegisterItemEvent(1361012, 0, "ZhiYeXiangLian_CanUse")
aux.RegisterItemEvent(1361013, 1, "ZhiYeXiangLian_Use")
aux.RegisterItemEvent(1361013, 0, "ZhiYeXiangLian_CanUse")
aux.RegisterItemEvent(1361014, 1, "ZhiYeXiangLian_Use")
aux.RegisterItemEvent(1361014, 0, "ZhiYeXiangLian_CanUse")
aux.RegisterItemEvent(1361015, 1, "ZhiYeXiangLian_Use")
aux.RegisterItemEvent(1361015, 0, "ZhiYeXiangLian_CanUse")
aux.RegisterItemEvent(1361016, 1, "ZhiYeXiangLian_Use")
aux.RegisterItemEvent(1361016, 0, "ZhiYeXiangLian_CanUse")
aux.RegisterItemEvent(1361017, 1, "ZhiYeXiangLian_Use")
aux.RegisterItemEvent(1361017, 0, "ZhiYeXiangLian_CanUse")
aux.RegisterItemEvent(1361018, 1, "ZhiYeXiangLian_Use")
aux.RegisterItemEvent(1361018, 0, "ZhiYeXiangLian_CanUse")
