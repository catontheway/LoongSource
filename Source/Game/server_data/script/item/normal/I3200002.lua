function I3200002_OnUse(MapID, InstanceID, TypeID, TargetID)
	--���ID�� ������ƷTypeID��������Ʒ�ʣ�����ģʽ��log��������Ϣö��
	local k = math.random(1,100)
	if k >= 1 and k <= 50 then
		role.AddRoleSilver(MapID, InstanceID, TargetID, 1000)
	elseif k > 50 and k <= 80 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 1100001, 2, -1, 8, 420)
	elseif k > 80 and k <= 90 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 2610142, 2, -1, 8, 420)
	elseif k > 90 and k <= 95 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3200301, 1, -1, 8, 420)
	elseif k > 95 and k <= 100 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 1101001, 2, -1, 8, 420)
	end
end

function I3200002_CanUse(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false						--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 1) then							--��ʾ��ұ����ռ䲻��
		bRet = 40
		return bRet, bIgnore
	end
end

aux.RegisterItemEvent(3200002, 1, "I3200002_OnUse")
aux.RegisterItemEvent(3200002, 0, "I3200002_CanUse")
