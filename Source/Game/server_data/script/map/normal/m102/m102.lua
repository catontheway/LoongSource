--����2�����볡��Ǭ���[m102]�����buffǬ�������[2017801]
--����6��ֻ���ڳ���Ǭ���[m102]�У��ſ�ʹ�õ�����������ɢ[3200415]

function m102_OnPlayerEnter(MapID, InstanceID,RoleID)
	--unit.AddBuff(MapID, InstanceID,RoleID, 2018201,RoleID)
	local bhave = role.IsRoleHaveBuff(MapID, InstanceID, RoleID, 20178)
	if bhave == false then
	unit.AddBuff(MapID, InstanceID, RoleID, 2017801, RoleID)
	end
end

--ע��
aux.RegisterMapEvent("m102", 2, "m102_OnPlayerEnter")

function m102_CanSetSafeguard(MapID, InstanceID)
    return false
end

aux.RegisterMapEvent("m102", 19, "m102_CanSetSafeguard")

function I3200415_CanUse(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	if MapID ~= 2291721329 then
	bRet = 43
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(3200415, 0, "I3200415_CanUse")

function m102_OnPlayerLeave(MapID, InstanceID, RoleID)
	local bhave = role.IsRoleHaveBuff(MapID, InstanceID, RoleID, 20202)
	if bhave == true then
	    unit.CancelBuff(MapID, InstanceID, RoleID, 2020201, RoleID)
		unit.AddBuff(MapID, InstanceID, RoleID, 2024701, RoleID)
	end
end

--ע��
aux.RegisterMapEvent("m102", 3, "m102_OnPlayerLeave")

--��Ҹ���ű�
function m102_RoloRevive(MapID, InstanceID, RoleID, Type, ReviveHP, ReviveMP, x, y, z, RebornMapID)


	if Type == 0 then
		ReviveHP = role.GetRoleAttValue(MapID, InstanceID, RoleID, 8)
		ReviveMP = role.GetRoleAttValue(MapID, InstanceID, RoleID, 10)
	    return ReviveHP, ReviveMP, 499, 0, 448, 2291721329
	end
	return ReviveHP, ReviveMP, x, y, z, MapID
end

aux.RegisterMapEvent("m102", 11, "m102_RoloRevive")
