--���֮�ƣ�13271
--����1��������������׮[3040233, 3040234, 3040235]�Ի���ֱ�ɾ��һ������[2613141]

--����NPC�Ի�����
function x13271_OnNPCTalk(mapID, instanceID, questID, ownerID, npcid, npctypeid)
	if npctypeid == 3040233 then
		role.RemoveFromRole(mapID, instanceID, ownerID, 2613141, 1, 102)
	elseif npctypeid == 3040234 then
		role.RemoveFromRole(mapID, instanceID, ownerID, 2613141, 1, 102)
	elseif npctypeid == 3040235 then
		role.RemoveFromRole(mapID, instanceID, ownerID, 2613141, 1, 102)
	end
end

--ע��
aux.RegisterQuestEvent(13271, 6, "x13271_OnNPCTalk")




