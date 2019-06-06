--����4�����볡�������[m104]�����buff���������[2018001]
--����8��ֻ���ڳ��������[m104]�У��ſ�ʹ�õ��߾Ż�����ɢ[3200435]

function m104_OnPlayerEnter(MapID, InstanceID,RoleID)
	--unit.AddBuff(MapID, InstanceID,RoleID, 2018401,RoleID)
	local bhave = role.IsRoleHaveBuff(MapID, InstanceID, RoleID, 20180)
	if bhave == false then
	unit.AddBuff(MapID, InstanceID, RoleID, 2018001, RoleID)
	end
end

--ע��
aux.RegisterMapEvent("m104", 2, "m104_OnPlayerEnter")

function m104_CanSetSafeguard(MapID, InstanceID)
    return false
end

aux.RegisterMapEvent("m104", 19, "m104_CanSetSafeguard")


function I3200435_CanUse(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	if MapID ~= 2291720817 then
	bRet = 43
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(3200435, 0, "I3200435_CanUse")

function m104_OnPlayerLeave(MapID, InstanceID, RoleID)
	local bhave = role.IsRoleHaveBuff(MapID, InstanceID, RoleID, 20202)
	if bhave == true then
	    unit.CancelBuff(MapID, InstanceID, RoleID, 2020201, RoleID)
		unit.AddBuff(MapID, InstanceID, RoleID, 2024701, RoleID)
	end
end

--ע��
aux.RegisterMapEvent("m104", 3, "m104_OnPlayerLeave")

--��Ҹ���ű�
function m104_RoloRevive(MapID, InstanceID, RoleID, Type, ReviveHP, ReviveMP, x, y, z, RebornMapID)

	if Type == 0 then
		ReviveHP = role.GetRoleAttValue(MapID, InstanceID, RoleID, 8)
		ReviveMP = role.GetRoleAttValue(MapID, InstanceID, RoleID, 10)
	    return ReviveHP, ReviveMP, 499, 0, 448, 2291720817
	end
	return ReviveHP, ReviveMP, x, y, z, MapID
end

aux.RegisterMapEvent("m104", 11, "m104_RoloRevive")
