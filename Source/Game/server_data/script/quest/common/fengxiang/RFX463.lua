--�Ϲ�֮ʯ��11463
--����1����ȡ����ʱ���BUFF��������[2011501]

--�����ȡʱ����
function x11463_OnAccept(mapID, instanceID, questID, ownerID, accepterID)
	unit.AddBuff(mapID, instanceID, ownerID, 2011501, ownerID)
end

--ע��
aux.RegisterQuestEvent(11463, 0, "x11463_OnAccept")
