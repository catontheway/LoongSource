--RWY274������Կ��
--����1������������ʱ�޵���"����Կ��"2614118��ʹ�ú���븱�����Ϲ�ħ��

--����������ʱ�޵���"����Կ��"2614118
function q14274_OnComplete(MapID, InstanceID, QuestID, RoleID, NPCID)
	role.QuestAddRoleItem(MapID, InstanceID, RoleID, 14274, 2614118, 1, -1, 101)

	--���ӽ���ʵ��ֵ
		Increase_shili(MapID, InstanceID, RoleID, 0, 2, "jinjieshili_Q")


end


aux.RegisterQuestEvent(14274, 1, "q14274_OnComplete")
