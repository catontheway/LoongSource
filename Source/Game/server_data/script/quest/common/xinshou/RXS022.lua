--22����Բͨ��ʮ�� 201 ǰ����������
--����1����������������������[m02: 2299, 5733, 2204]

--�������ʱ
function q022_OnComplete(mapID, instanceID, questID, ownerID, enderID)
	role.RoleGotoNewMap(mapID, instanceID, ownerID, 3017298127, 2299, 5733, 2204)
end

--ע��
aux.RegisterQuestEvent(22, 1, "q022_OnComplete")
aux.RegisterQuestEvent(201, 1, "q022_OnComplete")

