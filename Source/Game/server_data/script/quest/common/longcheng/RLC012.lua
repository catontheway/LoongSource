--����������̽�㣺10012
--����1����������ʱ��������������[m02: 2046, 10500, 2119]
--����2���������ʱ��������������[m02��2176, 5800, 2083]

--�����ȡʱ����
function x10012_OnAccept(mapID, instanceID, questID, ownerID, accepterID)
	role.RoleGotoNewMap(mapID, instanceID, ownerID, 3017298127, 2046, 10500, 2119)
end

--�����ύʱ����
function x10012_OnComplete(mapID, instanceID, questID, ownerID, enderID)
	role.RoleGotoNewMap(mapID, instanceID, ownerID, 3017298127, 2176, 5800, 2083)
end

--ע��
aux.RegisterQuestEvent(10012, 0, "x10012_OnAccept")
aux.RegisterQuestEvent(10012, 1, "x10012_OnComplete")

