--RZL902
--����1���붫����Ԫ[3010082]�Ի������񱳰������Ʒ���˷���[3200510]

--NPC�Ի�����
function x12902_OnNPCTalk(mapID, instanceID, questID, ownerID, npcid, npctypeid)
    --����Ƿ�Ϊ������Ԫ
	if npctypeid == 3010082 then
		role.QuestAddRoleItem(mapID, instanceID, ownerID, questID, 3200510, 1, 0, 102)
	end
end

--ע��
aux.RegisterQuestEvent(12902, 6, "x12902_OnNPCTalk")

