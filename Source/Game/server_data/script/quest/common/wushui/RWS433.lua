--ο����ʿ��13433
--����1����������ʿ�ף��ң�������[3040315, 3040316, 3040317, 3040318]�Ի���ֱ�ɾ��һ����ɫ��[2613222]

--NPC�Ի�����
function x13433_OnNPCTalk(mapID, instanceID, questID, ownerID, npcid, npctypeid)
	if npctypeid == 3040315 then
		role.RemoveFromRole(mapID, instanceID, ownerID, 2613222, 1, 102)
	elseif npctypeid == 3040316 then
		role.RemoveFromRole(mapID, instanceID, ownerID, 2613222, 1, 102)
	elseif npctypeid == 3040317 then
		role.RemoveFromRole(mapID, instanceID, ownerID, 2613222, 1, 102)
	elseif npctypeid == 3040318 then
		role.RemoveFromRole(mapID, instanceID, ownerID, 2613222, 1, 102)
	end
end

--ע��
aux.RegisterQuestEvent(13433, 6, "x13433_OnNPCTalk")

