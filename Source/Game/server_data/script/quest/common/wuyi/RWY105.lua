--��������3060043��ʧȥ����2614055���������������2614056

function c3060043_OnCheck(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
 	role.QuestAddRoleItem(MapID, InstanceID, RoleID, 14105, 2614056, 1, 0, 102)
	role.RemoveFromRole(MapID, InstanceID, RoleID, 2614055, 1, 420)	
end

--ע��
aux.RegisterCreatureEvent(3060043, 6, "c3060043_OnCheck")
