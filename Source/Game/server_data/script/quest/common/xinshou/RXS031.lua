--��覵����ۣ�31
--����1��ʹ����Ʒ��覵�С����[1330401]��ӵ�������覵�����[31]
--����2������������������������ָ�ȫ��BUFF[2013201]

--��Ʒ��Ͼ��С����ʹ��Ч������
function i1330401_QuestGiver(MapID, InstanceID, TypeID, TargetID)
	role.AddQuest(TargetID, 31)
end

--ע��
aux.RegisterItemEvent(1330401, 1, "i1330401_QuestGiver")

--�������ʱ
function x031_OnComplete(mapID, instanceID, questID, ownerID, enderID)
	unit.AddBuff(mapID, instanceID, ownerID, 2013201, ownerID)
end

--ע��
aux.RegisterQuestEvent(31, 1, "x031_OnComplete")
