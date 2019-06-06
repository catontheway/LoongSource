--����������10850
--����1����NPC����[3010186]�Ի�����Բ����ʯ��[2610176]

--NPC�Ի�����
function x10850_OnNPCTalk(MapID, InstanceID, QuestID, RoleID, NpcID, NpctypeID)
	if NpctypeID == 3010186 then
		role.AddRoleItem(MapID, InstanceID, RoleID, 2610176, 1, -1, 8, 420)
	end
end

--ע��
aux.RegisterQuestEvent(10850, 6, "x10850_OnNPCTalk")


--10850:��������
--����н�ʵ�����[2610174],����Ƥ��[2610175],Բ��ʯ��[2610176]������������Ʒ��ʹ��Բ��ʯ��[2610176]��ɾ������������Ʒ����� ��������[2610177]��
function i2610176_QuestUsable(MapID, InstanceID, TypeID, RoleID)
	role.RemoveFromRole(MapID, InstanceID, RoleID, 2610174, 1, 101)
	role.RemoveFromRole(MapID, InstanceID, RoleID, 2610175, 1, 101)
	role.RemoveFromRole(MapID, InstanceID, RoleID, 2610176, 1, 101)
	role.AddRoleItem(MapID, InstanceID, RoleID, 2610177, 1, -1, 8, 420)
end


function i2610176_CanUse(MapID, InstanceID, TypeID, RoleID)
	local num1 = role.GetRoleItemNum(RoleID, 2610174)
	local num2 = role.GetRoleItemNum(RoleID, 2610175)


	if num1 < 1 or num2 < 1 then
	return 32,false
	else
	return 0, false
	end
end

--ע��

aux.RegisterItemEvent(2610176, 1, "i2610176_QuestUsable")
aux.RegisterItemEvent(2610176, 0, "i2610176_CanUse")
