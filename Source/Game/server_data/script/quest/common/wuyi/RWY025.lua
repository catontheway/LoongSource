--RWY025 �Ѽ�֤��-��
--����1�������NPC�������ӶԻ���ɺ󣬻�õ���ħ���֤��

function x14025_OnNPCTalk(mapID, instanceID, questID, ownerID, npcid, npctypeid)
	if npctypeid == 3050022 then
		role.QuestAddRoleItem(mapID, instanceID, ownerID, questID, 2614016, 1, 0, 102)
	end
end

aux.RegisterQuestEvent(14025, 6, "x14025_OnNPCTalk")

