
--RWS107
--����1�����ջ�ϰ�[3040412]�Ի������񱳰������Ʒһ������[2613624]

--NPC�Ի�����
function x13107_OnNPCTalk(mapID, instanceID, questID, ownerID, npcid, npctypeid)
    --����Ƿ�Ϊ��ջ�ϰ�
	if npctypeid == 3040412 then
		role.QuestAddRoleItem(mapID, instanceID, ownerID, questID, 2613624, 1, 0, 102)
	end
end

--ע��
aux.RegisterQuestEvent(13107, 6, "x13107_OnNPCTalk")

