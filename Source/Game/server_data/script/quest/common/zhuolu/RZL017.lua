--�ػ�ƽ����12017
--����1�������ʯ��[3030017]�Ի���ɾ��������Ʒ����[2612012]

--NPC�Ի�����
function x12017_OnNPCTalk(mapID, instanceID, questID, ownerID, npcid, npctypeid)
	if npctypeid == 3030017 then
		role.RemoveFromRole(mapID, instanceID, ownerID, 2612012, 1, 101)
	end
end

--ע��
aux.RegisterQuestEvent(12017, 6, "x12017_OnNPCTalk")

