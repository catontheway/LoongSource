--�ַ�������12001
--����1����NPCɣ��[3030002]�������[3030003]���ſ���[3030004]�Ի���ֱ���һ��������Ʒ����[2612023]

--NPC�Ի�����
function x12001_OnNPCTalk(mapID, instanceID, questID, ownerID, npcid, npctypeid)
	if npctypeid == 3030002 or npctypeid == 3030003 or npctypeid == 3030004 then
		role.QuestAddRoleItem(mapID, instanceID, ownerID, questID, 2612023, 1, 0, 102)
	end
end

--ע��
aux.RegisterQuestEvent(12001, 6, "x12001_OnNPCTalk")

