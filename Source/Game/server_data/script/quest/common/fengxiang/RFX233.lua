--���ͳ�����11233
--����1��ʹ����Ʒ���Ͳ���-Ҽ[2611122]���ȡ�������ͳ���[11233]

--��Ʒ���Ͳ���ʹ��Ч������
function i2611122_QuestGiver(MapID, InstanceID, TypeID, TargetID)
	role.AddQuest(TargetID, 11233)
end

--ע��
aux.RegisterItemEvent(2611122, 1, "i2611122_QuestGiver")
