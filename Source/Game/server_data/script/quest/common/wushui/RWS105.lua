
--RWS105
--����1���뺣��[3040207]�Ի������񱳰������Ʒ�㹳[2613617]

--NPC�Ի�����
function x13105_OnNPCTalk(mapID, instanceID, questID, ownerID, npcid, npctypeid)
    --����Ƿ�Ϊ����
	if npctypeid == 3040207 then
		role.QuestAddRoleItem(mapID, instanceID, ownerID, questID, 2613617, 1, 0, 102)
	end
end

--ע��
aux.RegisterQuestEvent(13105, 6, "x13105_OnNPCTalk")

