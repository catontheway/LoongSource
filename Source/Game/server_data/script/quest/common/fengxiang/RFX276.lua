--������⣺11276
--����1����ȡ����ʱ����������q03[q03: 190, 3460, 165]

--�����ȡʱ����
function x11276_OnAccept(mapID, instanceID, questID, ownerID, accepterID)
	role.RoleGotoNewMap(mapID, instanceID, ownerID, 1826990119, 190, 3460, 165)
end

--ע��
aux.RegisterQuestEvent(11276, 0, "x11276_OnAccept")
