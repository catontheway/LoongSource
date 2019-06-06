----------------------------------------------------------------------------------
--����Ƿ���װ������ͷ�	0 -- �޳ͷ�  1 -- �гͷ�
function r_RoleIsEquipLootPenalty(MapID, InstanceID, RoleID)
	if role.IsRoleInStatus(ERS_HoldChest) then
		return false
	end
	if MapID == 3618849499 then--test02
		return true
	elseif MapID == 3017298127 then--m02
		return true
	elseif MapID == 3017298383 then--m03
		return true
	elseif MapID == 3017299663 then--m04
		return true
	elseif MapID == 3017299919 then--m05
		return true
	elseif MapID == 3017299151 then--m06
		return true
	elseif MapID == 1146339967 then--m06_1
		return true
	elseif MapID == 3017299407 then--m07
		return true
	elseif MapID == 2291722097 then--m101
		return true
	elseif MapID == 2291721329 then--m102
		return true
	elseif MapID == 2291721585 then--m103
		return true
	elseif MapID == 2291720817 then--m104
		return true
	elseif MapID == 2291721073 then--m105
		return true
	elseif MapID == 2291720305 then--m106
		return true
	elseif MapID == 2291720561 then--m107
		return true
	end

	return false
end


----------------------------------------------------------------------------------
--����Ƿ��о���ͷ�	0 -- �޳ͷ�  1 -- �гͷ�
function r_RoleIsExpPenalty(MapID, InstanceID, RoleID)
	if role.IsRoleInStatus(ERS_HoldChest) then
		return false
	end
	if MapID == 3618849499 then--test02
		return true
	elseif MapID == 3017298127 then--m02
		return true
	elseif MapID == 3017298383 then--m03
		return true
	elseif MapID == 3017299663 then--m04
		return true
	elseif MapID == 3017299919 then--m05
		return true
	elseif MapID == 3017299151 then--m06
		return true
	elseif MapID == 1146339967 then--m06_1
		return true
	elseif MapID == 3017299407 then--m07
		return true
	elseif MapID == 2291722097 then--m101
		return true
	elseif MapID == 2291721329 then--m102
		return true
	elseif MapID == 2291721585 then--m103
		return true
	elseif MapID == 2291720817 then--m104
		return true
	elseif MapID == 2291721073 then--m105
		return true
	elseif MapID == 2291720305 then--m106
		return true
	elseif MapID == 2291720561 then--m107
		return true
	end

	return false
end


----------------------------------------------------------------------------------
--����Ƿ������˳ͷ�	0 -- �޳ͷ�  1 -- �гͷ�
function r_RoleIsInjuryPenalty(MapID, InstanceID, RoleID)
	if MapID == 3618849499 or MapID == 2092185140 or MapID == 2108961076 then--test02,a06
		return false
	elseif MapID == 2983744463 or MapID == 2983743695 or MapID == 2108960820 then--m21,m22,a09
		return false
	elseif MapID == 3017298383 or MapID == 3017299663 or MapID == 3017299919 or  MapID == 3017299151 or  MapID == 3017299407 then
		local godCurState = guild.GetGodMiracleCurState(GetGodMiracleIDMinOne(MapID))
		if godCurState == 2 then
			if  map.IsInArea(MapID, InstanceID, RoleID, GetAreaID(MapID)) ~= 0 then
				local GuildID = guild.GetRoleGuildID(RoleID)
				if GuildID ~= 4294967295 then
					if GetDefendGuildID(MapID) == GuildID or GetCaptureGuildID(MapID) == GuildID then
						return false
					end
				end
			end
		else
			return true
		end
	end

	return true
end


----------------------------------------------------------------------------------
--����Ƿ��е��³ͷ�	0 -- �޳ͷ�  1 -- �гͷ�
function r_RoleIsMoralityPenalty(MapID, InstanceID, RoleID)
	if role.IsRoleInStatus(ERS_HoldChest) then
		return false
	end
	if MapID == 3618849499 then--test02
		return true
	elseif MapID == 3017298127 then--m02
		return true
	elseif MapID == 1146339967 then--m06_1
		return true
	elseif MapID == 3017298383 or MapID == 3017299663 or MapID == 3017299919 or MapID == 3017299151 or MapID == 3017299407 then
		local godCurState = guild.GetGodMiracleCurState(GetGodMiracleIDMinOne(MapID))
		if godCurState == 2 then
			if  map.IsInArea(MapID, InstanceID, RoleID, GetAreaID(MapID)) ~= 0 then
				local GuildID = guild.GetRoleGuildID(RoleID)
				if GuildID ~= 4294967295 then
					if GetDefendGuildID(MapID) == GuildID or GetCaptureGuildID(MapID) == GuildID then
						return false
					end
				end
			end
		else
			return true
		end
	end

	return false
end

-- breif: ��������,����������қ�����ֵ����
function IsMinusHostilityPenalty(MapID, RoleID)
	if  map.IsInArea(MapID, InstanceID, RoleID, GetAreaID(MapID)) ~= 0 then
		local GuildID = guild.GetRoleGuildID(RoleID)
		if GuildID ~= 4294967295 then
			if IsInGodCaptureGuild(GuildID) == 1 then
				return false
			end
		end
	end
	return true
end

----------------------------------------------------------------------------------
--����Ƿ�������ֵ�ͷ�	0 -- �޳ͷ�  1 -- �гͷ�
function r_RoleIsHostilityPenalty(MapID, InstanceID, RoleID)
	if role.IsRoleInStatus(ERS_HoldChest) then
		return false
	end

	if MapID == 3618849499 then--test02
		return true
	elseif MapID == 3017298127 then--m02
		return true
	elseif MapID == 1146339967 then--m06_1
		return true
	elseif MapID == 3017298383 or MapID == 3017299663 or MapID == 3017299919 or MapID == 3017299151 or MapID == 3017299407 then
		local godCurState = guild.GetGodMiracleCurState(GetGodMiracleIDMinOne(MapID))
		if godCurState == 2 then
			if  map.IsInArea(MapID, InstanceID, RoleID, GetAreaID(MapID)) ~= 0 then
				local GuildID = guild.GetRoleGuildID(RoleID)
				if GuildID ~= 4294967295 then
					if GetDefendGuildID(MapID) == GuildID or GetCaptureGuildID(MapID) == GuildID then
						return false
					end
				end
			end
		else
			return true
		end
	end

	return false
end


----------------------------------------------------------------------------------
--����Ƿ������������ͷ�	0 -- �޳ͷ�  1 -- �гͷ�
function r_RoleIsCommercePenalty(MapID, InstanceID, RoleID)
	if role.IsRoleInStatus(ERS_HoldChest) then
		return false
	end

	if MapID == 3618849499 or MapID == 2108961076 then
		return false
	end

	return true
end


----------------------------------------------------------------------------------
--����Ƿ�����������ͷ�	����ֵ 1 - װ������  2 - ����  3 - ����  4 -  ����  5 - ��������
function r_RoleCanCancelPenalty(MapID, InstanceID, RoleID, KillerID)

	if role.IsRoleInStatus(ERS_HoldChest) then
		return true, true, true, true, true
	end

	local morality = role.GetRoleAttValue(MapID, InstanceID, RoleID, 74)
	-- ��ҵ���ֵС�ڵ���-30���ұ������������ʱ���������װ������ͷ�
	if morality <= -30 then
		local num = role.GetRoleItemNum(RoleID, 1320011)
		if num > 0 then
			role.RemoveFromRole(MapID, InstanceID, RoleID, 1320011, 1, 300)
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 26, 11001)
			msg.DispatchRoleMsgEvent(RoleID, MsgID)

			return true, false, false, false, false
		end
	end

	return false, false, false, false, false

end

aux.RegisterRoleEvent(15, "r_RoleIsEquipLootPenalty")
aux.RegisterRoleEvent(16, "r_RoleIsExpPenalty")
aux.RegisterRoleEvent(17, "r_RoleIsInjuryPenalty")
aux.RegisterRoleEvent(18, "r_RoleIsMoralityPenalty")
aux.RegisterRoleEvent(19, "r_RoleIsHostilityPenalty")
aux.RegisterRoleEvent(20, "r_RoleIsCommercePenalty")
aux.RegisterRoleEvent(22, "r_RoleCanCancelPenalty")
