--�������죺11631
--����1��ʹ����Ʒ���˽׼�ͼ[2611315]���ȡ������������[11631]

--��Ʒ���˽׼�ͼЧ���ű�����
function i2611315_QuestGiver(MapID, InstanceID, TypeID, TargetID)
	role.AddQuest(TargetID, 11631)
end

--ע��
aux.RegisterItemEvent(2611315, 1, "i2611315_QuestGiver")
