--���ͷ��ϣ�11021
--����1����ȡ����ʱ���BUFF����[2011101]
--����2��ɾ�����������ʱɾ��BUFF����[2011101]

--�����ȡʱ
function x11021_OnAccept(mapID, instanceID, questID, ownerID, accepterID)
	unit.AddBuff(mapID, instanceID, ownerID, 2011101, ownerID)
end

--�������ʱ
function x11021_OnComplete(mapID, instanceID, questID, ownerID, enderID)
	unit.CancelBuff(mapID, instanceID, ownerID, 2011101)
end

--�������ʱ
function x11021_OnCancel(mapID, instanceID, questID, ownerID, enderID)
	unit.CancelBuff(mapID, instanceID, ownerID, 2011101)
end

--ע��
aux.RegisterQuestEvent(11021, 0, "x11021_OnAccept")
aux.RegisterQuestEvent(11021, 1, "x11021_OnComplete")
aux.RegisterQuestEvent(11021, 2, "x11021_OnCancel")
