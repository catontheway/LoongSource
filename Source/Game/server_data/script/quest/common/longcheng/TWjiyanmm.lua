--�o����ü��ף�� 




--�����ύʱ�ӿ�
function x23001_OnComplete(mapID, instanceID, questID, ownerID, accepterID)
    --����ף��BUFF
	unit.AddBuff(mapID, instanceID, ownerID, 2025001, ownerID)
end

--ע��
aux.RegisterQuestEvent(23001, 1, "x23001_OnComplete")