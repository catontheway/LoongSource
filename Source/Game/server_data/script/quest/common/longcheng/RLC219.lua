--������Ԯ��10219
--����1�������������������q01[q01��154, 5, 35]

--�����ȡʱ����
function x10219_OnAccept(mapID, instanceID, questID, ownerID, accepterID)
	role.RoleGotoNewMap(mapID, instanceID, ownerID, 1826990631, 154, 5, 35)
end

--ע��
aux.RegisterQuestEvent(10219, 0, "x10219_OnAccept")
