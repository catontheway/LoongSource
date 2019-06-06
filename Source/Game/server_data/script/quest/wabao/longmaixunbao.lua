--�Ƿ����ʹ��̰��ͼ
function I1350003_CanUseItem(MapID, InstanceID, TypeID, TargetID)
    if role.IsRoleHaveQuest(MapID, InstanceID, TargetID, 20034) or role.IsRoleHaveQuest(MapID, InstanceID, TargetID, 20035) then
    	return 45, false
    elseif role.IsRoleHaveQuest(MapID, InstanceID, TargetID, 20036) then
        return 41, false
    end
    
    return 0, false
end

--ʹ��̰��ͼ��1350003���һ�������
function I1350003_UseItem(MapID, InstanceID, TypeID, TargetID)
	--ˢ��������������ˣ�3011065��
	local PosX,PosY,PosZ = unit.GetPosition(MapID, InstanceID, TargetID)
	local NPCID = map.MapCreateCreature(MapID, InstanceID, 3011065, PosX, PosY, PosZ)
	role.SetRoleScriptData(TargetID, 3, RoleDataType["LongMaiXunBao_NPC_X"], PosX, RoleDataType["LongMaiXunBao_NPC_Y"], PosY, RoleDataType["LongMaiXunBao_NPC_Z"],PosZ)
	
	--�����������˼�סʹ��̰��ͼ�Ľ�ɫ��ID
	cre.SetCreatureScriptData(MapID, InstanceID, NPCID, 1, 1, TargetID)
end

aux.RegisterItemEvent(1350003, 0, "I1350003_CanUseItem")
aux.RegisterItemEvent(1350003, 1, "I1350003_UseItem")

