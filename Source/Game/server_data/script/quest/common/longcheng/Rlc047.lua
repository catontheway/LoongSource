--����֮����10047
--����1�������������BUFF�ɷ�������[]
--����2�������������������q02[q02: 154, 5, 35]

--�����ȡʱ����
function x10047_OnAccept(mapID, instanceID, questID, ownerID, accepterID)
	role.RoleGotoNewMap(mapID, instanceID, ownerID, 1826990375, 154, 5, 35)
end

--ע��
aux.RegisterQuestEvent(10047, 0, "x10047_OnAccept")

