function GaoJiBaoShiJingHua(MapID, InstanceID, TypeID, TargetID)
	--���ID�� ������ƷTypeID��������Ʒ�ʣ�����ģʽ��log��������Ϣö��
	role.AddRoleItem(MapID, InstanceID, TargetID, 3200727, 1, -1, 8, 420)
end

function GaoJiBaoShiJingHua_CanUse(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 1) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(3200721, 1, "GaoJiBaoShiJingHua")
aux.RegisterItemEvent(3200721, 0, "GaoJiBaoShiJingHua_CanUse")

aux.RegisterItemEvent(3200722, 1, "GaoJiBaoShiJingHua")
aux.RegisterItemEvent(3200722, 0, "GaoJiBaoShiJingHua_CanUse")

aux.RegisterItemEvent(3200723, 1, "GaoJiBaoShiJingHua")
aux.RegisterItemEvent(3200723, 0, "GaoJiBaoShiJingHua_CanUse")

aux.RegisterItemEvent(3200724, 1, "GaoJiBaoShiJingHua")
aux.RegisterItemEvent(3200724, 0, "GaoJiBaoShiJingHua_CanUse")

aux.RegisterItemEvent(3200725, 1, "GaoJiBaoShiJingHua")
aux.RegisterItemEvent(3200725, 0, "GaoJiBaoShiJingHua_CanUse")

aux.RegisterItemEvent(3200726, 1, "GaoJiBaoShiJingHua")
aux.RegisterItemEvent(3200726, 0, "GaoJiBaoShiJingHua_CanUse")
