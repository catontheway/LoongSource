--���̻��ʣ�12270
--����1�����鶾ʯ[3030141]��ɾ������Ȫˮ[2612144]���ÿ�Ȫˮƿ[2612143]

--�����ѱ����麯��
function q12270_OnInvest(MapID, InstanceID, QuestID, RoleID, NPCID)

	role.RemoveFromRole(MapID, InstanceID, RoleID, 2612144, 1, 102)
	role.QuestAddRoleItem(MapID, InstanceID, RoleID, 12270, 2612143, 1, -1, 101)
end
aux.RegisterQuestEvent(12270, 9, "q12270_OnInvest")
