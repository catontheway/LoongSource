--������13805
--����1����NPC��è[3040611]�Ի����õ��߰�����[2613610]

--NPC�Ի�����
function x13805_OnNPCTalk(mapID, instanceID, questID, ownerID, npcid, npctypeid)
	if npctypeid == 3040611 then
		role.QuestAddRoleItem(mapID, instanceID, ownerID, questID, 2613610, 1, 0, 102)
	end
end

--ע��
aux.RegisterQuestEvent(13805, 6, "x13805_OnNPCTalk")

    