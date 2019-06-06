--����3�����볡�������[m103]�����buff���������[2017901]
--����7��ֻ���ڳ��������[m103]�У��ſ�ʹ�õ�����������ɢ[3200425]

function m103_OnPlayerEnter(MapID, InstanceID,RoleID)
	--unit.AddBuff(MapID, InstanceID,RoleID, 2018301,RoleID)
	local bhave = role.IsRoleHaveBuff(MapID, InstanceID, RoleID, 20179)
	if bhave == false then
	unit.AddBuff(MapID, InstanceID, RoleID, 2017901, RoleID)
	end
end

--ע��
aux.RegisterMapEvent("m103", 2, "m103_OnPlayerEnter")

function m103_CanSetSafeguard(MapID, InstanceID)
    return false
end

aux.RegisterMapEvent("m103", 19, "m103_CanSetSafeguard")


function I3200425_CanUse(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	if MapID ~= 2291721585 then
	bRet = 43
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(3200425, 0, "I3200425_CanUse")

function m103_OnPlayerLeave(MapID, InstanceID, RoleID)
	local bhave = role.IsRoleHaveBuff(MapID, InstanceID, RoleID, 20202)
	if bhave == true then
	    unit.CancelBuff(MapID, InstanceID, RoleID, 2020201, RoleID)
		unit.AddBuff(MapID, InstanceID, RoleID, 2024701, RoleID)
	end
end

--ע��
aux.RegisterMapEvent("m103", 3, "m103_OnPlayerLeave")

--��Ҹ���ű�
function m103_RoloRevive(MapID, InstanceID, RoleID, Type, ReviveHP, ReviveMP, x, y, z, RebornMapID)

	if Type == 0 then
		ReviveHP = role.GetRoleAttValue(MapID, InstanceID, RoleID, 8)
		ReviveMP = role.GetRoleAttValue(MapID, InstanceID, RoleID, 10)
	    return ReviveHP, ReviveMP, 251, 17136, 700, 2291721585
	end
	return ReviveHP, ReviveMP, x, y, z, MapID
end

aux.RegisterMapEvent("m103", 11, "m103_RoloRevive")
