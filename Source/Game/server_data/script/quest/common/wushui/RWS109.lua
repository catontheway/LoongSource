--���RWS109���BUFF���ڷ��ڣ�2018901��

--�������ʱ
function q13109_OnComplete(mapID, instanceID, questID, ownerID, enderID)
	unit.CancelBuff(mapID, instanceID, ownerID, 2018901)
end

--ע��
aux.RegisterQuestEvent(13109, 1, "q13109_OnComplete")

