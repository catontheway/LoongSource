--���ԭ����10440
--ʹ����Ʒ�����ͷ��[2610173]������������ԭ��[10440]

--��Ʒ�����ͷ��ʹ��Ч���ű�
function i2610173_QuestGiver(MapID, InstanceID, TypeID, TargetID)
	role.AddQuest(TargetID, 10440)
end

--ע��
aux.RegisterItemEvent(2610173, 1, "i2610173_QuestGiver")
