--����װ���������3040624��������ף�2613618��������2613619�����ף�2613620��������2613621���̶���2613622���ܲ���2613623��
function c3040624_OnCheck(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
 	local a = role.GetRoleItemNum(RoleID, 2613618)
	local b = role.GetRoleItemNum(RoleID, 2613619)
	local c = role.GetRoleItemNum(RoleID, 2613620)
	local d = role.GetRoleItemNum(RoleID, 2613621)
	local e = role.GetRoleItemNum(RoleID, 2613622)
	local f = role.GetRoleItemNum(RoleID, 2613623)
	if a==1 and b==1 and c==1 and d==1 and e==1 and d==1 then
	else
		if f == 0 then
				role.QuestAddRoleItem(MapID, InstanceID, RoleID, 13106, 2613623, 1, 0, 102)
		else
			if b == 0 then
					role.QuestAddRoleItem(MapID, InstanceID, RoleID, 13106, 2613619, 1, 0, 102)
			else
				if c == 0 then
						role.QuestAddRoleItem(MapID, InstanceID, RoleID, 13106, 2613620, 1, 0, 102)
				else
					if d == 0 then
							role.QuestAddRoleItem(MapID, InstanceID, RoleID, 13106, 2613621, 1, 0, 102)
					else
						if e == 0 then
							role.QuestAddRoleItem(MapID, InstanceID, RoleID, 13106, 2613622, 1, 0, 102)
						else
							if a == 0 then
							        role.QuestAddRoleItem(MapID, InstanceID, RoleID, 13106, 2613618, 1, 0, 102)
								
							end
						end
					end
				end
			end
		end
	end
end

--ע��
aux.RegisterCreatureEvent(3040624, 6, "c3040624_OnCheck")

