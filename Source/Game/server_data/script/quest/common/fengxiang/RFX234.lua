--ը��ˮկ��11234
--����1��ʹ����Ʒ���Ͳ���-��[2611123]���ȡ����ը��ˮկ[11234]

--��Ʒ���Ͳ���ʹ��Ч������
function i2611123_QuestGiver(MapID, InstanceID, TypeID, TargetID)
	role.AddQuest(TargetID, 11234)
end

--ע��
aux.RegisterItemEvent(2611123, 1, "i2611123_QuestGiver")
