--���������ۣ�32
--����1��ʹ����Ʒ�����Ĵ�����[1330402]��ӵ���������������[32]
--����2������������������������ָ�ȫ��BUFF[2013201]

--��Ʒ�����Ĵ�����ʹ��Ч������
function i1330402_QuestGiver(MapID, InstanceID, TypeID, TargetID)
	role.AddQuest(TargetID, 32)
end

--ע��
aux.RegisterItemEvent(1330402, 1, "i1330402_QuestGiver")

--�������ʱ
function x032_OnComplete(mapID, instanceID, questID, ownerID, enderID)
	unit.AddBuff(mapID, instanceID, ownerID, 2013201, ownerID)
end

--ע��
aux.RegisterQuestEvent(32, 1, "x031_OnComplete")

