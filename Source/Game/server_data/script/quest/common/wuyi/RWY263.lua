--RWY263������������ս
--����1����Ի�����ըҩͰ[3050123]�Ի��� ˢ��4������ʿ��[1007117]
function x14263_OnNPCTalk(mapID, instanceID, questID, ownerID, npcid, npctypeid)
	if npctypeid == 3050123 then
	local x, y ,z = unit.GetPosition(mapID, instanceID, ownerID)
	map.MapCreateCreature(mapID, instanceID, 1007117, x+4, y, z)
	map.MapCreateCreature(mapID, instanceID, 1007117, x-4, y, z)
	map.MapCreateCreature(mapID, instanceID, 1007117, x+2, y, z+2)
	map.MapCreateCreature(mapID, instanceID, 1007117, x-2, y, z+2)
	end
end

aux.RegisterQuestEvent(14263, 6, "x14263_OnNPCTalk")
