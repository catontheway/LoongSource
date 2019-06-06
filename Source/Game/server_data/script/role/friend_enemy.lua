-- test02�ĵ��ҹ�ϵ�ж� -- Ĭ��Ϊ����  ����ֵ�ֱ�Ϊ �Ѻã����������������������жϼ��
function test02_FriendEnemy(MapID, InstanceID, SrcID, TargetID)
	-- �������
	if unit.IsPlayer(SrcID) and unit.IsPlayer(TargetID) then

		-- ����
		local teamid = role.IsRoleHaveTeam(MapID, InstanceID, SrcID)
		if teamid ~= 4294967295 and teamid == role.IsRoleHaveTeam(MapID, InstanceID, TargetID) then
			return true, false, false, false
		end

		-- ����
		local guildid = guild.GetRoleGuildID(SrcID)
		if guildid ~= 4294967295 and guildid == guild.GetRoleGuildID(TargetID) then
			return true, false, false, false
		end

		-- ����
		if role.IsCouple(SrcID, TargetID) then
			return true, false, false, false
		end

	end

	return false, false, false, false
end

aux.RegisterMapEvent("test02", 16, "test02_FriendEnemy")

--------------------------------------------------------------------------------------
-- a04�ĵ��ҹ�ϵ�ж� -- ������������ȫΪ�з�
function a04_FriendEnemy(MapID, InstanceID, SrcID, TargetID)
	-- �������
	if unit.IsPlayer(SrcID) and unit.IsPlayer(TargetID) then

		-- ����
		local teamid = role.IsRoleHaveTeam(MapID, InstanceID, SrcID)
		if teamid ~= 4294967295 and teamid == role.IsRoleHaveTeam(MapID, InstanceID, TargetID) then
			return true, false, false, true
		else
			if role.IsRoleInStatus(SrcID, 1) or role.IsRoleInStatus(TargetID, 1) then			-- ��һ��pk����
				return false, false, false, true

			elseif role.IsRoleInStatus(SrcID, 4) then		-- ��ҿ�����ܹ���
				return false, true, false, true

			elseif role.IsRoleInStatus(SrcID, 1048576) and ( role.IsRoleInStatus(TargetID, 4) or role.IsRoleInStatus(TargetID, 1048576) )then
				return false, true, false, true

			else
				return false, false, false, true
			end

		end

	end

	return false, false, false, false
end

aux.RegisterMapEvent("a04", 16, "a04_FriendEnemy")

---------------------------------------------------------------------------------------

-- a05�ĵ��ҹ�ϵ�ж� -- ������������ȫΪ�з�
function a05_FriendEnemy(MapID, InstanceID, SrcID, TargetID)
	-- �������
	if unit.IsPlayer(SrcID) and unit.IsPlayer(TargetID) then

		-- ����
		local teamid = role.IsRoleHaveTeam(MapID, InstanceID, SrcID)
		if teamid ~= 4294967295 and teamid == role.IsRoleHaveTeam(MapID, InstanceID, TargetID) then
			return true, false, false, true
		else
			if role.IsRoleInStatus(SrcID, 1) or role.IsRoleInStatus(TargetID, 1) then			-- ��һ��pk����
				return false, false, false, true

			elseif role.IsRoleInStatus(SrcID, 4) then		-- ��ҿ�����ܹ���
				return false, true, false, true

			elseif role.IsRoleInStatus(SrcID, 1048576) and ( role.IsRoleInStatus(TargetID, 4) or role.IsRoleInStatus(TargetID, 1048576) )then
				return false, true, false, true

			else
				return false, false, false, true
			end

		end

	end

	return false, false, false, false
end

aux.RegisterMapEvent("a05", 16, "a05_FriendEnemy")

------------------------------------------------------------------------------------

-- a06�ĵ��ҹ�ϵ�ж� -- ��ͬɫ����ȫ�ǵ���
function a06_FriendEnemy(MapID, InstanceID, SrcID, TargetID)
    local a,b = -1,-1
	-- �������
	if unit.IsPlayer(SrcID) and unit.IsPlayer(TargetID) then
        for i=1, 6 do
		    if Totle_Arena[InstanceID][i] == SrcID then
			    if i <= 3 then
			        a = 1
				    break
				else
				    a = 0
					break
				end
			end
		end

		for i=1, 6 do
		    if Totle_Arena[InstanceID][i] == TargetID then
			    if i <= 3 then
			        b = 1
				    break
				else
				    b = 0
					break
				end
			end
		end

		if a ~= b and a~= -1 and b ~= -1 then
			return false, true, false, true
		elseif a == b and a~= -1 then
		    return true, false, false, true
		end
	end

	return false, false, false, false
end

aux.RegisterMapEvent("a06", 16, "a06_FriendEnemy")

---------------------------------------------------------------------------------------


-- brief: ��������
-- remark: �񼣸���ͼ �����ж�
function GodMiracle_FriendEnemy(MapID, InstanceID, SrcID, TargetID)

	--����boss�жϣ����뿪�죩
	if unit.IsPlayer(SrcID) and unit.IsCreature(TargetID) then
		local TargetTypeID = unit.GetUnitTypeID(MapID, InstanceID, TargetID)
		local temp=0
		local boss_id_list={}
		boss_id_list[1] = 1535326
		boss_id_list[2] = 1535327
		boss_id_list[3] = 1550201
		boss_id_list[4] = 1550202
		boss_id_list[5] = 1550203
		boss_id_list[6] = 1550204

		for i=1,6 do
			if TargetTypeID==boss_id_list[i] then
				temp=1
				break
			end
		end

		if temp==1 then
			if role.IsRoleInStatus(SrcID, 1048576) or role.IsRoleInStatus(SrcID, 4) then
				return false, true, false, true
			else
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 26,	3624)		--�����Ѳμӹ�5�μ���
				msg.DispatchRoleMsgEvent(SrcID, MsgID)
				return false, false, true, true
			end
		else
			return false, false, false, false
		end
	end


-- ��ͼ�����ж�
	if IsInGodAreaMap(MapID) == 0 then
		return false, false, false, false
	end

	--  û��������
	if  map.IsInArea(MapID, InstanceID, SrcID, GetAreaID(MapID)) == false and   map.IsInArea(MapID, InstanceID, TargetID, GetAreaID(MapID)) == false then
		return false, false, false, false
	else

		if unit.IsCreature(SrcID) == true or unit.IsCreature(TargetID) == true  then
			return false, false, false, false
		end

		-- ʦͽ��ϵ���ɴ�
		local Src = role.GetMasterID(MapID, InstanceID, SrcID)
		if (src ~= 4294967295) and src ==  TargetID then
			return false, false, false, false
		end

		local Target = role.GetMasterID(MapID, InstanceID, TargetID)
		if (Target ~= 4294967295) and   Target == SrcID then
			return false, false, false, false
		end

		-- ״̬0��1�����������һ��
		local godCurState = guild.GetGodMiracleCurState(GetGodMiracleIDMinOne(MapID))
		if godCurState == 2 then

			local SrcGuild 		 = guild.GetRoleGuildID(SrcID)
			local TargetGuildID = guild.GetRoleGuildID(TargetID)

			if SrcGuild == 4294967295 and TargetGuildID == 4294967295 then
					return false, false, false, false
			elseif SrcGuild == 4294967295 and TargetGuildID ~= 4294967295 then
				if GetDefendGuildID(MapID) == TargetGuildID or GetCaptureGuildID(MapID) == TargetGuildID then
					return true, false, false, true
				end
			elseif SrcGuild ~= 4294967295 and TargetGuildID == 4294967295 then
				if GetDefendGuildID(MapID) == SrcGuild or GetCaptureGuildID(MapID) == SrcGuild then
					return true, false, false, true
				end
			elseif	SrcGuild ~= 4294967295 and TargetGuildID ~= 4294967295 then
				if SrcGuild == TargetGuildID then
					return true, false, false, true
				end
				if (GetDefendGuildID(MapID) == TargetGuildID or GetCaptureGuildID(MapID) == TargetGuildID) and (GetDefendGuildID(MapID) == SrcGuild or GetCaptureGuildID(MapID) == SrcGuild) then
					return false, true, false, true
				else
					return true, false, false, true
				end
			end
		else
			-- ��������ʱ���ڣ���һ���ж�
			return false, false, false, false
		end
	end

end

---------------------------------------------------------------------------------------
-- m03 �������ж�
function m03_FriendEnemy(MapID, InstanceID, SrcID, TargetID)
	return GodMiracle_FriendEnemy(MapID, InstanceID, SrcID, TargetID)
end

-- m04 �������ж�
function m04_FriendEnemy(MapID, InstanceID, SrcID, TargetID)
	return GodMiracle_FriendEnemy(MapID, InstanceID, SrcID, TargetID)
end
-- m05 �������ж�
function m05_FriendEnemy(MapID, InstanceID, SrcID, TargetID)
	return GodMiracle_FriendEnemy(MapID, InstanceID, SrcID, TargetID)
end
-- m06 �������ж�
function m06_FriendEnemy(MapID, InstanceID, SrcID, TargetID)
	return GodMiracle_FriendEnemy(MapID, InstanceID, SrcID, TargetID)
end
-- m07 �������ж�
function m07_FriendEnemy(MapID, InstanceID, SrcID, TargetID)
    --���鱦�����M07�޷�PK
    if KuiLingBaoShu_Start == 1 and unit.IsPlayer(SrcID) and unit.IsPlayer(TargetID) then
	    return true, false, false, true
	else
	    return GodMiracle_FriendEnemy(MapID, InstanceID, SrcID, TargetID)
	end
end
aux.RegisterMapEvent("m03", 16, "m03_FriendEnemy")
aux.RegisterMapEvent("m04", 16, "m04_FriendEnemy")
aux.RegisterMapEvent("m05", 16, "m05_FriendEnemy")
aux.RegisterMapEvent("m06", 16, "m06_FriendEnemy")
aux.RegisterMapEvent("m07", 16, "m07_FriendEnemy")

