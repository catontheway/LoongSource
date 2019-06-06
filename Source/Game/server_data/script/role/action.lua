--��ҵ�ĳЩ�¼��ű�

-------------------------------------------------------------------------------------------------------
--����л���ͼ
function x_RoleEnterMap(mapid, instanceid, roleid)
	unit.AddBuff(mapid, instanceid, roleid, 9999801)    --��һ��15�����޵�buff
end

--ע�ắ��
aux.RegisterRoleEvent(4, "x_RoleEnterMap")

function Increase_shili(MapID, InstanceID, RoleID, ToTeam, Num, POS) --ToTeam:0-���˼ӣ� 1-ȫ�Ӽ�   Num:���ӷ�ֵ
--POS: "jinjieshili"������		"jinjieshili_A",�		"jinjieshili_Q",ÿ�պ��������		"jinjieshili_K",��ɱ�ǰ������

	local shili = role.GetRoleScriptData(RoleID, 1, RoleDataType[POS])
	local e = role.GetConsumptiveStrength(RoleID)

	local d = 0
	if POS == "jinjieshili" then
		d = 12001
	elseif POS == "jinjieshili_A" then
		d = 12002
	elseif POS == "jinjieshili_Q" then
		d = 12003
	elseif POS == "jinjieshili_K" then
		d = 12004
	end

	if ToTeam == 0 then
		--���ӽ���ʵ��ֵ�Ϳ�����ʵ��ֵ
		role.SetRoleScriptData(RoleID, 1, RoleDataType[POS],shili+Num)
		role.SetConsumptiveStrength(RoleID, e+Num)
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 26, d)	--����XX����ʵ��ֵ������xx��
			msg.AddMsgEvent(MsgID, 9, Num)	--��������ֵ
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
	elseif ToTeam == 1 then
		local TeamID = role.IsRoleHaveTeam(MapID, InstanceID, RoleID)
			--���û��С��
		if 4294967295 == TeamID  then
			--���ӽ���ʵ��ֵ
			role.SetRoleScriptData(RoleID, 1, RoleDataType[POS],shili+Num)
			role.SetConsumptiveStrength(RoleID, e+Num)
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 26, d)	--����XX����ʵ��ֵ������xx��
			msg.AddMsgEvent(MsgID, 9, Num)	--��������ֵ
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
			--���С�����ID
			local Role = {}
			local Role_MapID = {}
			local Role_InstanceID = {}
			Role[1], Role[2], Role[3], Role[4], Role[5], Role[6] = role.GetRoleTeamMemID(TeamID)
			for k in pairs(Role) do
				if 4294967295 ~= Role[k] then
					Role_MapID[k], Role_InstanceID[k] = role.GetRoleMapID(Role[k])
					if Role_MapID[k] == MapID or Role_InstanceID[k] == InstanceID then
						local x1, y1, z1 = unit.GetPosition(MapID, InstanceID, RoleID)
						local x2, y2, z2 = unit.GetPosition(Role_MapID[k], Role_InstanceID[k], Role[k])
						if math.abs(x1-x2)<100 and math.abs(z1-z2)<100 then
							--���ӽ���ʵ��ֵ�Ϳ�����ʵ��ֵ
							local shili_team = role.GetRoleScriptData(Role[k], 1, RoleDataType[POS])
							local e_team = role.GetConsumptiveStrength(Role[k])
							role.SetRoleScriptData(Role[k], 1, RoleDataType[POS],shili_team + Num)
							role.SetConsumptiveStrength(Role[k], e_team + Num)
							local MsgID = msg.BeginMsgEvent()
							msg.AddMsgEvent(MsgID, 26, d)	--����XX����ʵ��ֵ������xx��
							msg.AddMsgEvent(MsgID, 9, Num)	--��������ֵ
							msg.DispatchRoleMsgEvent(Role[k], MsgID)
						end
					end
				end
			end
		end
	end
end

-------------------------------------------------------------------------------------------------------
--����������Ƿ���������ͷ�
--[[function r_RoleIsDeadPenalty(MapID, InstanceID, RoleID)
	--pvp�̶���в����������ͷ�
	if MapID == 2983744463 then
		return 0
	end

	if MapID == 2983743695 then
		return 0
	end


	local morality = role.GetRoleAttValue(MapID, InstanceID, RoleID, 74)
	-- ��ҵ���ֵС�ڵ���-30���ұ������������ʱ����������ͷ�
	if morality <= -30 then
		local num = role.GetRoleItemNum(RoleID, 1320011)
		if num > 0 then
			role.RemoveFromRole(MapID, InstanceID, RoleID, 1320011, 1, 300)
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 26, 11001)
			msg.DispatchRoleMsgEvent(RoleID, MsgID)

			return 0
		end
	end

	return 1
end]]


----------------------------------------------------------------------------------
--[[�������
function r_RoleLevelChange(MapID, InstanceID, RoleID)
	local level = role.GetRoleLevel(MapID, InstanceID, RoleID)

	-- �������20������ʾ��������
	if level == 20 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 283)
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	end


end]]


--aux.RegisterRoleEvent(5, "r_RoleIsDeadPenalty")
--aux.RegisterRoleEvent(6, "r_RoleLevelChange")
