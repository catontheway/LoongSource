
--RWYMR07������ָ��
--����1����ɱ[��Ӱɱ��][1007111]��10%�ĸ���ˢ�³�NPC[��·����][3060017]
--����2����[��·����][3060017]�Ի�����һ��[����ָ��][2614043]

--����1����ɱ[��Ӱɱ��][1007111]��10%�ĸ���ˢ�³�NPC[��·����][3060017]
function x1007111_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	local Rate = math.random(1, 100)
	local x, y ,z = unit.GetPosition(MapID, InstanceID, TargetID)
	if Rate < 11 then
	map.MapCreateCreature(MapID, InstanceID, 3060017, x+2, y, z+2)
	end
end


--����2����[��·����][3060017]�Ի�����һ��[����ָ��][2614043]
function x14307_OnNPCTalk(mapID, instanceID, questID, ownerID, npcid, npctypeid)
	if npctypeid == 3060017 then
		role.RemoveFromRole(mapID, instanceID, ownerID, 2614043, 1, 101)
	end
end

aux.RegisterQuestEvent(14307, 6, "x14307_OnNPCTalk")
aux.RegisterCreatureEvent(1007111, 4, "x1007111_OnDie")
