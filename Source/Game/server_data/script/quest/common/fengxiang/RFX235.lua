--����֥��11235
--����1��ʹ����Ʒ���Ͳ���-��[2611124]���ȡ��������֥[11235]

--��Ʒ���Ͳ���ʹ��Ч������
function i2611124_QuestGiver(MapID, InstanceID, TypeID, TargetID)
	role.AddQuest(TargetID, 11235)
end

--ע��
aux.RegisterItemEvent(2611124, 1, "i2611124_QuestGiver")
