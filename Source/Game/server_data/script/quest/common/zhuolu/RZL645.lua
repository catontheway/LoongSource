--è�ڶ��ģ�12645
--����1����NPC��è[3030603]�Ի�������Ʒ�����Ƶ�Ь��[2612704]

--��è�Ի��ű�����
function x12645_OnNPCTalk(mapID, instanceID, questID, ownerID, npcid, npctypeid)
	if npctypeid == 3030603 then
		role.QuestAddRoleItem(mapID, instanceID, ownerID, questID, 2612704, 1, 0, 102)
	end
end

--ע��
aux.RegisterQuestEvent(12645, 6, "x12645_OnNPCTalk")

