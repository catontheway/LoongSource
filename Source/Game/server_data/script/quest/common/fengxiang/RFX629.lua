--�������֣�11629
--����1������������BUFF��������[2011801]

--�������ʱ����
function x11629_OnComplete(mapID, instanceID, questID, ownerID, enderID)
	unit.AddBuff(mapID, instanceID, ownerID, 2011801, ownerID)
end

--ע��
aux.RegisterQuestEvent(11629, 1, "x11629_OnComplete")
