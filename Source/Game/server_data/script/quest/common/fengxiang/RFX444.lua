--���ص���ı��11444
--����1��ʹ����Ʒ�򶾽��ܺ�[2611230]���ȡ�������ص���ı[11444]

--��Ʒ�򶾽��ܺ�ʹ��Ч������
function i2611230_QuestGiver(MapID, InstanceID, TypeID, TargetID)
	role.AddQuest(TargetID, 11444)
end

--ע��
aux.RegisterItemEvent(2611230, 1, "i2611230_QuestGiver")
