--RWY231ǰ����˸�
--������󣬻��������Ծ������BUFF2016501

function q14231_OnComplete(MapID, InstanceID, QuestID, OwnerID, NpcID)
	unit.AddBuff(MapID, InstanceID, OwnerID, 2016501, OwnerID)
end
aux.RegisterQuestEvent(14231, 1, "q14231_OnComplete")



