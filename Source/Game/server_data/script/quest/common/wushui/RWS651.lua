--����˼����13651
--����1��ʹ����Ʒ����˼���ס�[2613017]��ӵ�������˼��[13651]

--��Ʒ��˼ʹ��Ч������
function i2613017_QuestGiver(MapID, InstanceID, TypeID, TargetID)
	role.AddQuest(TargetID, 13651)
end

--ע��
aux.RegisterItemEvent(2613017, 1, "i2613017_QuestGiver")
