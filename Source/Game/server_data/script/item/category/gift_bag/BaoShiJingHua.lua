function BaoShiJingHua(MapID, InstanceID, TypeID, TargetID)
	--���ID�� ������ƷTypeID��������Ʒ�ʣ�����ģʽ��log��������Ϣö��
	role.AddRoleItem(MapID, InstanceID, TargetID, 3200707, 1, -1, 8, 420)
end

function BaoShiJingHua_CanUse(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 1) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(3200701, 1, "BaoShiJingHua")
aux.RegisterItemEvent(3200701, 0, "BaoShiJingHua_CanUse")

aux.RegisterItemEvent(3200702, 1, "BaoShiJingHua")
aux.RegisterItemEvent(3200702, 0, "BaoShiJingHua_CanUse")

aux.RegisterItemEvent(3200703, 1, "BaoShiJingHua")
aux.RegisterItemEvent(3200703, 0, "BaoShiJingHua_CanUse")

aux.RegisterItemEvent(3200704, 1, "BaoShiJingHua")
aux.RegisterItemEvent(3200704, 0, "BaoShiJingHua_CanUse")

aux.RegisterItemEvent(3200705, 1, "BaoShiJingHua")
aux.RegisterItemEvent(3200705, 0, "BaoShiJingHua_CanUse")

aux.RegisterItemEvent(3200706, 1, "BaoShiJingHua")
aux.RegisterItemEvent(3200706, 0, "BaoShiJingHua_CanUse")
