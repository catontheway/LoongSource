--��񱨸���11218
--����1����ȡ����ʱ����BUFF�������[2011301]

--�����ȡʱ
function x11218_OnAccept(mapID, instanceID, questID, ownerID, accepterID)
	unit.AddBuff(mapID, instanceID, ownerID, 2011301, ownerID)
end

--ע��
aux.RegisterQuestEvent(11218, 0, "x11218_OnAccept")
