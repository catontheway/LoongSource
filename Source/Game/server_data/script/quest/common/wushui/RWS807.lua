--׽��13807
--����1����NPC����[3040608]������[3040609]��Ү����[3040610]�Ի���ֱ�ˢ�¹��ﱼ�˰�[1006612]�����˱�[1006613]���˱���[1006614]

--NPC�Ի�����
function x13807_OnNPCTalk(mapID, instanceID, questID, ownerID, npcid, npctypeid)
	if npctypeid == 3040608 then
		local x, y ,z = unit.GetPosition(mapID, instanceID, ownerID)
		map.MapCreateCreature(mapID, instanceID, 1006612, x+2, y, z+2)
	end

	if npctypeid == 3040609 then
		local x, y ,z = unit.GetPosition(mapID, instanceID, ownerID)
		map.MapCreateCreature(mapID, instanceID, 1006613, x+2, y, z+2)
	end

	if npctypeid == 3040610 then
		local x, y ,z = unit.GetPosition(mapID, instanceID, ownerID)
		map.MapCreateCreature(mapID, instanceID, 1006614, x+2, y, z+2)
	end
end

--ע��
aux.RegisterQuestEvent(13807, 6, "x13807_OnNPCTalk")
