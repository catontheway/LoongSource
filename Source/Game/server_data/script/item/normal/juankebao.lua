--2000041	[��]ϡ��ʯ��
--2000044	[��]ϡ����ʯ
--2000047	[��]ϡ������
--2000050	[��]ϡ������
--1330005	�Կ̲��ϰ�

--2000042	[��]��Ʒʯ��
--2000045	[��]��Ʒ��ʯ
--2000048	[��]��Ʒ����
--2000051	[��]��Ʒ����
--1330006	��Ʒ�Կ̲��ϰ�


--��ͨ�Կ̰�
function I1330005_OnUse(MapID, InstanceID, TypeID, RoleID)
	--���ID�� ������ƷTypeID��������Ʒ�ʣ�����ģʽ��log��������Ϣö��
	local jinli = role.GetRoleAttValue(MapID, InstanceID, RoleID, 1)
	local neili = role.GetRoleAttValue(MapID, InstanceID, RoleID, 3)
	local k = math.random(1,2)
	if  jinli > neili then
		role.AddRoleItem(MapID, InstanceID, RoleID, 2000047, 18, -1, 8, 420)
		role.AddRoleItem(MapID, InstanceID, RoleID, 2000050, 4, -1, 8, 420)
	elseif jinli < neili then
		role.AddRoleItem(MapID, InstanceID, RoleID, 2000041, 18, -1, 8, 420)
		role.AddRoleItem(MapID, InstanceID, RoleID, 2000044, 4, -1, 8, 420)
	elseif jinli == neili then
		if  k == 1 then
		role.AddRoleItem(MapID, InstanceID, RoleID, 2000047, 18, -1, 8, 420)
		role.AddRoleItem(MapID, InstanceID, RoleID, 2000050, 4, -1, 8, 420)
		else
		role.AddRoleItem(MapID, InstanceID, RoleID, 2000041, 18, -1, 8, 420)
		role.AddRoleItem(MapID, InstanceID, RoleID, 2000044, 4, -1, 8, 420)
		end
	end
	return 1
end


function I1330005_CanUse(MapID, InstanceID, TypeID, RoleID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(RoleID)
	if(FreeSize < 3) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
		return bRet, bIgnore
	end

end

aux.RegisterItemEvent(1330005, 1, "I1330005_OnUse")
aux.RegisterItemEvent(1330005, 0, "I1330005_CanUse")

--��Ʒ�Կ̰�
function I1330006_OnUse(MapID, InstanceID, TypeID, RoleID)
	--���ID�� ������ƷTypeID��������Ʒ�ʣ�����ģʽ��log��������Ϣö��
	local jinli = role.GetRoleAttValue(MapID, InstanceID, RoleID, 1)
	local neili = role.GetRoleAttValue(MapID, InstanceID, RoleID, 3)
	local k = math.random(1,2)
	if  jinli > neili then
		role.AddRoleItem(MapID, InstanceID, RoleID, 2000048, 18, -1, 8, 420)
		role.AddRoleItem(MapID, InstanceID, RoleID, 2000051, 4, -1, 8, 420)
	elseif jinli < neili then
		role.AddRoleItem(MapID, InstanceID, RoleID, 2000042, 18, -1, 8, 420)
		role.AddRoleItem(MapID, InstanceID, RoleID, 2000045, 4, -1, 8, 420)
	elseif jinli == neili then
		if  k == 1 then
		role.AddRoleItem(MapID, InstanceID, RoleID, 2000048, 18, -1, 8, 420)
		role.AddRoleItem(MapID, InstanceID, RoleID, 2000051, 4, -1, 8, 420)
		else
		role.AddRoleItem(MapID, InstanceID, RoleID, 2000042, 18, -1, 8, 420)
		role.AddRoleItem(MapID, InstanceID, RoleID, 2000045, 4, -1, 8, 420)
		end
	end
	return 1
end


function I1330006_CanUse(MapID, InstanceID, TypeID, RoleID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(RoleID)
	if(FreeSize < 3) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
		return bRet, bIgnore
	end

end

aux.RegisterItemEvent(1330006, 1, "I1330006_OnUse")
aux.RegisterItemEvent(1330006, 0, "I1330006_CanUse")





