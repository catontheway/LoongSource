--Ѱ�һ��11211
--����1����������ʱ���BUFF����[2011201]

--�����ȡʱ
function x11211_OnAccept(mapID, instanceID, questID, ownerID, accepterID)
	unit.AddBuff(mapID, instanceID, ownerID, 2011201, ownerID)
end

--ע��
aux.RegisterQuestEvent(11211, 0, "x11211_OnAccept")
