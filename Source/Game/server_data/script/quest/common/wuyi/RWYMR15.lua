

--�������ʱ [2614046]�����Ļ��Ǽ��㡣����8���������Ǯ�����ڵ���5�������顣
--[[function q14318_OnComplete(MapID, InstanceID, QuestID, RoleID, EnderID)

end

--ע��
aux.RegisterQuestEvent(14318, 1, "q14318_OnComplete")]]


function q14318_OnCheckComplete(MapID, InstanceID, QuestID, RoleID, NPCID)
	local num = role.GetRoleItemNum(RoleID, 2614046)
	if num == 8 then
		role.AddRoleExp(MapID, InstanceID, RoleID, 10000)
		role.AddRoleSilver(MapID, InstanceID, RoleID, 3000, 101)
	elseif num >= 5 then
		role.AddRoleExp(MapID, InstanceID, RoleID, 10000)
	end

	return 1
end

aux.RegisterQuestEvent(14318, 5, "q14318_OnCheckComplete")
