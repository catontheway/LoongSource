--����2�����볡��Ǭ���[m102]�����buffǬ�������[2017801]
--����6��ֻ���ڳ���Ǭ���[m102]�У��ſ�ʹ�õ�����������ɢ[3200415]

function c03_1_OnPlayerEnter(MapID, InstanceID,RoleID)
	local bhave = role.IsRoleHaveBuff(MapID, InstanceID, RoleID, 20278)
	if bhave == false then
		unit.AddBuff(MapID, InstanceID, RoleID, 2027801, RoleID)
	end
end

--ע��
aux.RegisterMapEvent("c03_1", 2, "c03_1_OnPlayerEnter")
