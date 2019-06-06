--����1�����볡�������[m101]�����buff���������[2017701]
--����5��ֻ���ڳ��������[m101]�У��ſ�ʹ�õ�������ɢ[3200405]

function m101_OnPlayerEnter(MapID, InstanceID,RoleID)
	--unit.AddBuff(MapID, InstanceID,RoleID, 2018101,RoleID)
	local bhave = role.IsRoleHaveBuff(MapID, InstanceID, RoleID, 20177)
	if bhave == false then
	    unit.AddBuff(MapID, InstanceID, RoleID, 2017701, RoleID)
	end
end

--ע��
aux.RegisterMapEvent("m101", 2, "m101_OnPlayerEnter")

function m101_CanSetSafeguard(MapID, InstanceID)
    return false
end

aux.RegisterMapEvent("m101", 19, "m101_CanSetSafeguard")


function I3200405_CanUse(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	if MapID ~= 2291722097 then
	bRet = 43
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(3200405, 0, "I3200405_CanUse")

function m101_OnPlayerLeave(MapID, InstanceID, RoleID)
	local bhave = role.IsRoleHaveBuff(MapID, InstanceID, RoleID, 20202)
	if bhave == true then
	    unit.CancelBuff(MapID, InstanceID, RoleID, 2020201, RoleID)
		unit.AddBuff(MapID, InstanceID, RoleID, 2024701, RoleID)
	end
end

--ע��
aux.RegisterMapEvent("m101", 3, "m101_OnPlayerLeave")


--��Ҹ���ű�
function m101_RoloRevive(MapID, InstanceID, RoleID, Type, ReviveHP, ReviveMP, x, y, z, RebornMapID)
	if Type == 0 then
		ReviveHP = role.GetRoleAttValue(MapID, InstanceID, RoleID, 8)
		ReviveMP = role.GetRoleAttValue(MapID, InstanceID, RoleID, 10)
	    return ReviveHP, ReviveMP, 251, 17136, 700, MapID
	end
	return ReviveHP, ReviveMP, x, y, z, MapID
end

aux.RegisterMapEvent("m101", 11, "m101_RoloRevive")
