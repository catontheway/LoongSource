--˫�����飺10848
--����1���������ʱ���BUFF˫������[2010401]

--�����ύʱ�ӿ�
function x10848_OnComplete(mapID, instanceID, questID, ownerID, accepterID)
    --����˫������
	unit.AddBuff(mapID, instanceID, ownerID, 2010401, ownerID)
end

--ע��
aux.RegisterQuestEvent(10848, 1, "x10848_OnComplete")
