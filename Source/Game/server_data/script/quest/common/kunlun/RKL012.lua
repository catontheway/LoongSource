--RKL012
--����1������������[3070005]�Ի������BUFF����è��[2020201]

--NPC�Ի�����
function x15012_OnNPCTalk(MapID, InstanceID, QuestID, OwnerID, NpcID, NpcTypeID)
    --����Ƿ�Ϊ��������
	if npctypeid == 3070005 then
		unit.AddBuff(MapID, InstanceID, OwnerID, 2020001, OwnerID)
	end
end

--ע��
aux.RegisterQuestEvent(15012, 6, "x15012_OnNPCTalk")

--�������ʱ
function q15012_OnComplete(mapID, instanceID, questID, ownerID, enderID)
	unit.CancelBuff(mapID, instanceID, ownerID, 2020001)
end

--ע��
aux.RegisterQuestEvent(15012, 1, "q15012_OnComplete")
