--��ػ�ҩ��13260
--����1������������[3040228, 3040229, 3040230]�Ի���ֱ�ɾ��һ��һ����ҩ[2613133]

--����NPC�Ի�����
function x13260_OnNPCTalk(mapID, instanceID, questID, ownerID, npcid, npctypeid)
	if npctypeid == 3040228 then
		role.RemoveFromRole(mapID, instanceID, ownerID, 2613133, 1, 102)
	elseif npctypeid == 3040229 then
		role.RemoveFromRole(mapID, instanceID, ownerID, 2613133, 1, 102)
	elseif npctypeid == 3040230 then
		role.RemoveFromRole(mapID, instanceID, ownerID, 2613133, 1, 102)
	end
end

--ע��
aux.RegisterQuestEvent(13260, 6, "x13260_OnNPCTalk")

