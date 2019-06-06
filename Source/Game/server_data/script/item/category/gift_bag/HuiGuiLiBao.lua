function I1330601_LiBao(MapID, InstanceID, TypeID, TargetID)
	--ϴ�赤��������
	role.AddRoleItem(MapID, InstanceID, TargetID, 1300003, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 1300103, 1, -1, 8, 420)
	--�Ƹ���
	role.AddRoleItem(MapID, InstanceID, TargetID, 1330403, 1, -1, 8, 420)
	--����ֵ����Ʒ
	role.AddRoleSilver(MapID, InstanceID, TargetID, 100000, 102)
	role.AddRoleExp(MapID, InstanceID, TargetID, 246100)
end

function I1330601_CanUse(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 3) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
	end
	return bRet, bIgnore
end

function I1330602_LiBao(MapID, InstanceID, TypeID, TargetID)
	--ϴ�赤��������
	role.AddRoleItem(MapID, InstanceID, TargetID, 1300003, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 1300103, 1, -1, 8, 420)
	--�Ƹ���
	role.AddRoleItem(MapID, InstanceID, TargetID, 1330403, 1, -1, 8, 420)
	--����ֵ����Ʒ
	role.AddRoleSilver(MapID, InstanceID, TargetID, 100000, 102)
	role.AddRoleExp(MapID, InstanceID, TargetID, 800100)
end

function I1330602_CanUse(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 3) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
	end
	return bRet, bIgnore
end

function I1330603_LiBao(MapID, InstanceID, TypeID, TargetID)
	--ϴ�赤��������
	role.AddRoleItem(MapID, InstanceID, TargetID, 1300003, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 1300103, 1, -1, 8, 420)
	--�Ƹ���
	role.AddRoleItem(MapID, InstanceID, TargetID, 1330403, 1, -1, 8, 420)
	--����ֵ����Ʒ
	role.AddRoleSilver(MapID, InstanceID, TargetID, 200000, 102)
	role.AddRoleExp(MapID, InstanceID, TargetID, 1706800)
end

function I1330603_CanUse(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 3) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
	end
	return bRet, bIgnore
end

function I1330604_LiBao(MapID, InstanceID, TypeID, TargetID)
	--ϴ�赤��������
	role.AddRoleItem(MapID, InstanceID, TargetID, 1300003, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 1300103, 1, -1, 8, 420)
	--�Ƹ���
	role.AddRoleItem(MapID, InstanceID, TargetID, 1330403, 1, -1, 8, 420)
	--����ֵ����Ʒ
	role.AddRoleSilver(MapID, InstanceID, TargetID, 200000, 102)
	role.AddRoleExp(MapID, InstanceID, TargetID, 2072500)
end

function I1330604_CanUse(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 3) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(1330601, 1, "I1330601_LiBao")
aux.RegisterItemEvent(1330601, 0, "I1330601_CanUse")

aux.RegisterItemEvent(1330602, 1, "I1330602_LiBao")
aux.RegisterItemEvent(1330602, 0, "I1330602_CanUse")

aux.RegisterItemEvent(1330603, 1, "I1330603_LiBao")
aux.RegisterItemEvent(1330603, 0, "I1330603_CanUse")

aux.RegisterItemEvent(1330604, 1, "I1330604_LiBao")
aux.RegisterItemEvent(1330604, 0, "I1330604_CanUse")
