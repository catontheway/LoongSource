--������ͼ��11623
--����1������������BUFF���[2011701]

--�������ʱ����
function x11623_OnComplete(mapID, instanceID, questID, ownerID, enderID)
	unit.AddBuff(mapID, instanceID, ownerID, 2011701, ownerID)
end

--ע��
aux.RegisterQuestEvent(11623, 1, "x11623_OnComplete")
