--15041 ��Ի����� ħ��[3070707] update�Ի��� �����Ʒħ��[2615015]
--��Ʒħ��[2615015]ʹ�ú��ٻ����� ����ҹħ[1009019]
--ħ��[2615015]ֻ���� ���� 3543-50<x<3543+50,3320-50<z<3320+50 ��Χ��ʹ��

--NPC�Ի�����
function x15041_OnNPCTalk(MapID, InstanceID, QuestID, RoleID, NpcID, NpcTypeID)
    --����Ƿ�Ϊħ��[3070707]
	if NpcTypeID == 3070707 then
		role.AddRoleItem(MapID, InstanceID, RoleID, 2615015, 1, -1, 8, 420)
	end
end

--ע��
aux.RegisterQuestEvent(15041, 6, "x15041_OnNPCTalk")


--�ɷ�ʹ�ú���
function i2615015_CanUse(MapID, InstanceID, TypeID, TargetID)
    local bRet, bIgnore = 0, false
	--����������أ�����ʹ��
	if MapID ~= 3017299407 then
	    return 43, false
	else
	--�������ָ���ص㣬����ʹ��
	    local x,y,z = unit.GetPosition(MapID, InstanceID, TargetID)
	    if x < 3493 or x > 3593 or z < 3270 or z > 3370 then
	          return 43, false
            end
	end
    return bRet, bIgnore
end

--ʹ��Ч��
function i2615015_QuestUsable(MapID, InstanceID, TypeID, TargetID)
       local x,y,z = unit.GetPosition(MapID, InstanceID, TargetID)
       map.MapCreateCreature(MapID, InstanceID, 1009019, x, y, z)
end

--ע��
aux.RegisterItemEvent(2615015, 1, "i2615015_QuestUsable")
aux.RegisterItemEvent(2615015, 0, "i2615015_CanUse")
