--RWYMR01:����֮��
--����1������Ʒ[���л���][2614023]��ɱ���׽���Ʊ�[1007005][1007006]��ˢ��[�׽��ױ�][1007030]��ɾ��[���л���][2614023]
--����2������Ʒ[�׽���][2614024]��ɱ�����л�[1007003][1007004], ˢ��[�����ͻ�][1007031], ɾ��[�׽���][2614024]




--����1������Ʒ[���л���][2614023]��ɱ���׽�[1007005][1007006]��ˢ��[�׽��ױ�][1007030]��ɾ��[���л���][2614023]
function c1007005_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	local x, y ,z = unit.GetPosition(MapID, InstanceID, TargetID)
	local num = role.GetRoleItemNum(RoleID, 2614023)
	if num > 0 then
	map.MapCreateCreature(MapID, InstanceID, 1007030, x+2, y, z+2)
	role.RemoveFromRole(MapID, InstanceID, RoleID, 2614023, 1, 101)
	end
end

aux.RegisterCreatureEvent(1007005, 4, "c1007005_OnDie")

function c1007006_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	local x, y ,z = unit.GetPosition(MapID, InstanceID, TargetID)
	local num = role.GetRoleItemNum(RoleID, 2614023)
	if num > 0 then
	map.MapCreateCreature(MapID, InstanceID, 1007030, x+2, y, z+2)
	role.RemoveFromRole(MapID, InstanceID, RoleID, 2614023, 1, 101)
	end
end

aux.RegisterCreatureEvent(1007006, 4, "c1007006_OnDie")


--����2������Ʒ[�׽���][2614024]��ɱ�����л�[1007003][1007004], ˢ��[�����ͻ�][1007031], ɾ��[�׽���][2614024]
function c1007003_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	local x, y ,z = unit.GetPosition(MapID, InstanceID, TargetID)
	local num = role.GetRoleItemNum(RoleID, 2614024)
	if num > 0 then
	map.MapCreateCreature(MapID, InstanceID, 1007031, x+2, y, z+2)
	role.RemoveFromRole(MapID, InstanceID, RoleID, 2614024, 1, 101)
	end
end

aux.RegisterCreatureEvent(1007003, 4, "c1007003_OnDie")

function c1007004_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	local x, y ,z = unit.GetPosition(MapID, InstanceID, TargetID)
	local num = role.GetRoleItemNum(RoleID, 2614024)
	if num > 0 then
	map.MapCreateCreature(MapID, InstanceID, 1007031, x+2, y, z+2)
	role.RemoveFromRole(MapID, InstanceID, RoleID, 2614024, 1, 101)
	end
end

aux.RegisterCreatureEvent(1007004, 4, "c1007004_OnDie")
