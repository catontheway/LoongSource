--��������ۣ�94
--����1��ʹ����Ʒ����������[1330403]��ӵ���������������[31]
--����2������������������������ָ�ȫ��BUFF[2013201]

--��Ʒ��Ͼ��С����ʹ��Ч������
function i1330403_QuestGiver(MapID, InstanceID, TypeID, TargetID)
	role.AddQuest(TargetID, 94)
end

--ע��
aux.RegisterItemEvent(1330403, 1, "i1330403_QuestGiver")

--�������ʱ
function x094_OnComplete(mapID, instanceID, questID, ownerID, enderID)
	unit.AddBuff(mapID, instanceID, ownerID, 2013201, ownerID)
end

--ע��
aux.RegisterQuestEvent(94, 1, "x094_OnComplete")
