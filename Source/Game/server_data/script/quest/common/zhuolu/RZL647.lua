--�����Ʒ���12647
--����1�������������ұ����͵���һ���Ʒ�����[m04: 1803, 14602, 1816]
--����2�������������һ��BUFF��������[2012001]

--�����ȡ����
function x12647_OnAccept(mapID, instanceID, questID, ownerID, accepterID)
	unit.AddBuff(mapID, instanceID, ownerID, 2012001, ownerID)
	role.RoleGotoNewMap(mapID, instanceID, ownerID, 3017299663, 1803, 14602, 1816)
end

aux.RegisterQuestEvent(12647, 0, "x12647_OnAccept")

