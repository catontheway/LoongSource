--���̻��ʣ�13275
--����1��������[3040050]��ɾ�����۵���[2613030]���õ���֭����[2613031]

--�����ѱ����麯��
function q13275_OnInvest(MapID, InstanceID, QuestID, RoleID, NPCID)

	role.RemoveFromRole(MapID, InstanceID, RoleID, 2613030, 1, 101)
	role.QuestAddRoleItem(MapID, InstanceID, RoleID, QuestID, 2613031, 1, -1, 101)
end

aux.RegisterQuestEvent(13275, 9, "q13275_OnInvest")
