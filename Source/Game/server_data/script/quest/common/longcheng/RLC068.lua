--�������ͣ�10068
--�����������ʱ��������������[m02 3281,4653,1656]

--�����ύʱ����
function q10068_OnComplete(mapID, instanceID, questID, ownerID, enderID)
	role.RoleGotoNewMap(mapID, instanceID, ownerID, 3017298127, 3281, 4653, 1656)
end

--ע��
aux.RegisterQuestEvent(10068, 1, "q10068_OnComplete")

