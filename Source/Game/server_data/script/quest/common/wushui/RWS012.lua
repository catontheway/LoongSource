--���Ŀ��飺13012
--����1�������������ұ���������Ͽ�þ�������[q05: 492, 6781, 725]
--����2�������������һ��BUFF���Ĺ�[2612501]

--�����ȡʱ����
function x13012_OnAccept(mapID, instanceID, questID, ownerID, accepterID)
	unit.AddBuff(mapID, instanceID, unitID, 2012501, ownerID)
	role.RoleGotoNewMap(mapID, instanceID, ownerID, 1826991655, 492, 6781, 725)
end

--ע��
aux.RegisterQuestEvent(13012, 0, "x13012_OnAccept")

