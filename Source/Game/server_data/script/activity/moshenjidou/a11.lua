
function a11_PlayerEnter(MapID, InstanceID, RoleID)
	local IsHave = role.IsRoleHaveBuff(MapID, InstanceID,RoleID,99996)
	local IsHaveSkill = role.IsLearnedSkill(MapID,InstanceID,RoleID,90019)
	role.AddSkill(RoleID, 9001901)--���������һ��������ĵ���ļ���
	if IsHave == true then
		unit.CancelBuff(MapID, InstanceID,RoleID,9999601)
	end
	unit.AddBuff(MapID, InstanceID, RoleID, 1121301, RoleID)
	role.SetRoleScriptData(RoleID, 1, RoleDataType["Jidou_Times"], role.GetRoleScriptData(RoleID, 1, RoleDataType["Jidou_Times"])+1)
	SendNormalInformation(MapID, InstanceID, RoleID) --ͬ����Ϣ

	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 71, 3708)			--ħ�񼤶��ѿ�ʼ����ѡ��Ӣ��
	msg.DispatchRoleMsgEvent(RoleID, MsgID)

	if Jidou.RoomInfo[InstanceID]~= nil and Jidou.RoomInfo[InstanceID].Role[RoleID]~= nil then
	    if Jidou.RoomInfo[InstanceID].Role[RoleID].Side == "Red" then
		    role.SetForceInDota(MapID, InstanceID, RoleID, 0) --��ɫ
		else
			role.SetForceInDota(MapID, InstanceID, RoleID, 1) --��ɫ
		end
	end

end

--ע�������ͼ�¼�
aux.RegisterMapEvent("a11", 2, "a11_PlayerEnter")


--���봥�������BUFF
function a11_OnEnterTrigger(MapID, InstanceID, RoleID, ObjID)
    if Jidou.RoomInfo[InstanceID].BuffTrigger[ObjID] ~= nil and  Jidou.RoomInfo[InstanceID].BuffTrigger[ObjID].CreID ~= nil and  Jidou.RoomInfo[InstanceID].BuffTrigger[ObjID].CreID ~= 0 then
	    unit.AddBuff(MapID, InstanceID, RoleID, Jidou.CreBuff[Jidou.RoomInfo[InstanceID].BuffTrigger[ObjID].CreTypeID], RoleID)
        map.MapDeleteCreature(MapID, InstanceID, Jidou.RoomInfo[InstanceID].BuffTrigger[ObjID].CreID)
		Jidou.RoomInfo[InstanceID].BuffTrigger[ObjID].CreTypeID = 0
		Jidou.RoomInfo[InstanceID].BuffTrigger[ObjID].CreID = 0
		Jidou.RoomInfo[InstanceID].Role[RoleID].Level = Jidou.RoomInfo[InstanceID].Role[RoleID].Level + 1
		if Jidou.RoomInfo[InstanceID].Role[RoleID].Level <= 40 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 13, 1)
			msg.AddMsgEvent(MsgID, 1, 12901)           --����ֵ����ΪX/40�㣡ÿ����10������ֵ�������������ļ��ܵȼ���
			msg.AddMsgEvent(MsgID, 9, Jidou.RoomInfo[InstanceID].Role[RoleID].Level)
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
        if Jidou.RoomInfo[InstanceID].Role[RoleID].Level ~= 0 and Jidou.RoomInfo[InstanceID].Role[RoleID].Level % 10 == 0 then
		    local k = Jidou.RoomInfo[InstanceID].Role[RoleID].Level/10 + 1
			if k <= 0 then
			    k = 2
			elseif k > 5 then
			    k = 5
			end
            for i=11231, 11238 do
				if role.IsRoleHaveBuff(MapID, InstanceID, RoleID, i) then
				    unit.AddBuff(MapID, InstanceID, RoleID, i*100+k, RoleID)
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 101, 12902)           --��������������XX����
					msg.AddMsgEvent(MsgID, 9, k)
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
				end
			end
		end
	end
end
aux.RegisterMapEvent("a11",6,"a11_OnEnterTrigger")


function a11_OnRoleDie(MapID, InstanceID, RoleID, KillerID)

	local ReviveTime={}
	ReviveTime[0]=5
	ReviveTime[1]=5
	ReviveTime[2]=8
	ReviveTime[3]=10
	ReviveTime[4]=12
	ReviveTime[5]=15
	ReviveTime[6]=15
	ReviveTime[7]=20
	ReviveTime[8]=20
	ReviveTime[9]=20
	ReviveTime[10]=20

	local x, y, z = unit.GetPosition(MapID, InstanceID, RoleID)
	role.DotaTryRevive(MapID, InstanceID, RoleID, ReviveTime[math.floor(Jidou.RoomInfo[InstanceID].Role[RoleID].Level/10)+1])

	Jidou.RoomInfo[InstanceID].Role[RoleID].Die = Jidou.RoomInfo[InstanceID].Role[RoleID].Die + 1
	SendNormalInformation(MapID, InstanceID, RoleID)

	if role.IsRoleHaveBuff(MapID, InstanceID, RoleID, 11401) then --����BUFF
	    unit.CancelBuff(MapID, InstanceID,RoleID, 1140101)
		Jidou.RoomInfo[InstanceID].BlueFlag.FlagID = map.MapCreateCreature(MapID, InstanceID, 4902112, x, y, z)
		Jidou.RoomInfo[InstanceID].BlueFlag.State = "outside"
		Jidou.RoomInfo[InstanceID].BlueFlag.Role = 0
		Jidou.RoomInfo[InstanceID].BlueFlag.Time = 0
	elseif role.IsRoleHaveBuff(MapID, InstanceID, RoleID, 11402) then  --����BUFF
	    unit.CancelBuff(MapID, InstanceID,RoleID, 1140201)
		Jidou.RoomInfo[InstanceID].RedFlag.FlagID = map.MapCreateCreature(MapID, InstanceID, 4902113, x, y, z)
		Jidou.RoomInfo[InstanceID].RedFlag.State = "outside"
		Jidou.RoomInfo[InstanceID].RedFlag.Role = 0
		Jidou.RoomInfo[InstanceID].RedFlag.Time = 0
	end

	if unit.IsPlayer(KillerID) then
		local Target_MapID, Target_InstanceID = role.GetRoleMapID(KillerID)
		if Target_MapID ~= nil and Target_MapID == 2108961076 then
			Jidou.RoomInfo[InstanceID].Role[KillerID].Kill = Jidou.RoomInfo[InstanceID].Role[KillerID].Kill + 1
			SendNormalInformation(MapID, InstanceID, KillerID)
		end
	end
end

--�����¼�
aux.RegisterMapEvent("a11",  5, "a11_OnRoleDie")

function a11_LeaveMap(MapID, InstanceID, RoleID)

    if InstanceID ~= nil and InstanceID ~= 4294967295 then
		unit.UnsetState(MapID, InstanceID, RoleID, 13)
		unit.UnsetState(MapID, InstanceID, RoleID, 14)
    end

    if InstanceID ~= nil then
		local TeamID = role.IsRoleHaveTeam(MapID, InstanceID, RoleID)
		if TeamID ~= nil and TeamID ~= 4294967295 then
			role.LetRoleLeaveTeam(TeamID, RoleID)
		end
	end

	if InstanceID ~= nil and role.IsRoleHaveBuff(MapID, InstanceID, RoleID, 11401) then
	    unit.CancelBuff(MapID, InstanceID,RoleID, 1140101)
		local x, y, z = unit.GetPosition(MapID, InstanceID, RoleID)
		Jidou.RoomInfo[InstanceID].BlueFlag.FlagID = map.MapCreateCreature(MapID, InstanceID, 4902112, x, y, z)
		Jidou.RoomInfo[InstanceID].BlueFlag.State = "outside"
		Jidou.RoomInfo[InstanceID].BlueFlag.Role = 0
		Jidou.RoomInfo[InstanceID].BlueFlag.Time = 0
	elseif InstanceID ~= nil and role.IsRoleHaveBuff(MapID, InstanceID, RoleID, 11402) then
	    unit.CancelBuff(MapID, InstanceID,RoleID, 1140201)
		local x, y, z = unit.GetPosition(MapID, InstanceID, RoleID)
		Jidou.RoomInfo[InstanceID].RedFlag.FlagID = map.MapCreateCreature(MapID, InstanceID, 4902113, x, y, z)
		Jidou.RoomInfo[InstanceID].RedFlag.State = "outside"
		Jidou.RoomInfo[InstanceID].RedFlag.Role = 0
		Jidou.RoomInfo[InstanceID].RedFlag.Time = 0
	end

	if  Jidou.RoomInfo[InstanceID] ~= nil and Jidou.RoomInfo[InstanceID].IsOver ~= 1 then
		Jidou.RoomInfo[InstanceID].Role[RoleID].GetFlag = 0
		Jidou.RoomInfo[InstanceID].Role[RoleID].MissFlag = 0
		Jidou.RoomInfo[InstanceID].Role[RoleID].Kill = 0
		Jidou.RoomInfo[InstanceID].Role[RoleID].Die = 0
		Jidou.RoomInfo[InstanceID].Role[RoleID].Leave = 1
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 101, 12903)			--XXX�Ѿ��˳��˱���
		msg.AddMsgEvent(MsgID, 2, RoleID)
		msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID)
		MSJD_CheckIsOver(MapID, InstanceID)
	elseif Jidou.RoomInfo[InstanceID] ~= nil then
	    Jidou.RoomInfo[InstanceID].Role[RoleID].Leave = 1
	end
end
--�뿪��ͼ
aux.RegisterMapEvent("a11", 3, "a11_LeaveMap")


function a11_On_Revive(MapID, InstanceID, RoleID, ReviveType, ReviveHP, ReviveMP, fx, fy, fz, RebornMapID)

	--if ReviveType == 7 then
	if Jidou.RoomInfo[InstanceID].Role[RoleID].Side == "Red" then
		unit.AddBuff(MapID, InstanceID, RoleID, 1140701, RoleID) --�޵У��ɶ�����Ŀ�����
		unit.AddBuff(MapID, InstanceID, RoleID, 1140801, RoleID)  --�ɶ�����Ŀ����ӵ�Խ��ԽӲ�ɵ��ӵ�BUFF��IDδ�С�����ÿ3���1%Ѫ��ͬʱ����һ���������ס�
		return ReviveHP, ReviveMP, 442,5502,105, 2108961076

	else
		unit.AddBuff(MapID, InstanceID, RoleID, 1140701, RoleID) --�޵У��ɶ�����Ŀ�����
		unit.AddBuff(MapID, InstanceID, RoleID, 1140801, RoleID)  --�ɶ�����Ŀ����ӵ�Խ��ԽӲ�ɵ��ӵ�BUFF��IDδ�С�����ÿ3���1%Ѫ��ͬʱ����һ���������ס�
		return ReviveHP, ReviveMP, 81,5555,426, 2108961076
	end


	--end
	--return ReviveHP, ReviveMP, fx, fy, fz, RebornMapID

end

--�����¼�
aux.RegisterMapEvent("a11", 11, "a11_On_Revive")

-- �������ĵ��ҹ�ϵ�ж� -- ��ͬɫ����ȫ�ǵ���
function a11_FriendEnemy(MapID, InstanceID, SrcID, TargetID)

	-- �������
	if unit.IsPlayer(SrcID) and unit.IsPlayer(TargetID) then
        if Jidou.RoomInfo[InstanceID] ~= nil and Jidou.RoomInfo[InstanceID].Role[SrcID] ~= nil and Jidou.RoomInfo[InstanceID].Role[TargetID] ~= nil then
			if Jidou.RoomInfo[InstanceID].Role[SrcID].Side == Jidou.RoomInfo[InstanceID].Role[TargetID].Side then
		        return true, false, false, true
			else
			    return false, true, false, true
			end
		end
    elseif unit.IsCreature(SrcID) and unit.IsPlayer(TargetID)then
	    local TypeID = cre.GetCreatureTypeID(MapID, InstanceID, SrcID)
	    if cre.GetCreatureScriptData(MapID, InstanceID, SrcID, 1, 0) == 1 or TypeID == 4902117 then
		    if Jidou.RoomInfo[InstanceID].Role[TargetID].Side == "Red" then        --��0��λ����1�Ĺ���ͺ췽������ѷ����������ǵз�
			    return true, false, false, true
			else
			    return false, true, false, true
			end
		end
		if cre.GetCreatureScriptData(MapID, InstanceID, SrcID, 1, 0) == 2 or TypeID == 4902116 then
		    if Jidou.RoomInfo[InstanceID].Role[TargetID].Side == "Red" then        --��0��λ����2�Ĺ��������������ѷ����ͺ췽�ǵз�
			    return false, true, false, true
			else
			    return true, false, false, true
			end
		end
	elseif unit.IsPlayer(SrcID) and unit.IsCreature(TargetID) then
	    local TypeID = cre.GetCreatureTypeID(MapID, InstanceID, TargetID)
	    if cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 0) == 2 or TypeID == 4902116 then
		    if Jidou.RoomInfo[InstanceID].Role[SrcID].Side == "Red" then        --��0��λ����2�Ĺ��������������ѷ����ͺ췽�ǵз�
			    return false, true, false, true
			else
			    return true, false, false, true
			end
		end
		if cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 0) == 1 or TypeID == 4902117 then
		    if Jidou.RoomInfo[InstanceID].Role[SrcID].Side == "Red" then        --��0��λ����1�Ĺ���ͺ췽������ѷ����������ǵз�
			    return true, false, false, true
			else
			    return false, true, false, true
			end
		end
	else
	    return false, false, false, false
	end

end

aux.RegisterMapEvent("a11", 16, "a11_FriendEnemy")

--�ҵ�����ʵĸ���ʵ��
function a11_GetOnePerfectMap(MapID, InstanceID, RoleID)

	for k,v in pairs(Jidou.RoomInfo) do
	    if v.Role[RoleID] ~= nil and v.Role[RoleID].Leave ~= 1 then
		    return k
		end
	end
	return 0

end

aux.RegisterMapEvent("a11",  14, "a11_GetOnePerfectMap")

--����ڻ�е��ߺ�����
function a11_CanTakeOverWhenOnline(MapID, RoleID)
	--������ҵ����ߵ�ͼ������todo
	return 3017298383, 880, 10000, 1987
end

aux.RegisterMapEvent("a11",  12, "a11_CanTakeOverWhenOnline")

-------------------------------------------------------------------------------------------------------
--�ҵ���ͼ�ĳ��ں�����
function a11_GetExporMapAndCoord(MapID, InstanceID, RoleID)
	return 1, 3017298383, 880, 10000, 1987
end

aux.RegisterMapEvent("a11",  13, "a11_GetExporMapAndCoord")

--�������Ƿ��ܹ���������
function a11_CanEnter(MapID, InstanceID, RoleID)

	return 1

end

aux.RegisterMapEvent("a11",  15, "a11_CanEnter")

--�����ؼ��

function a11_CanInviteJoinTeam(MapID, InstanceID)

	return false

end

function a11_CanLeaveTeam(MapID, InstanceID)

	return false

end

function a11_CanChangeLeader(MapID, InstanceID)

	return false

end

function a11_CanKickMember(MapID, InstanceID)

	return false

end

aux.RegisterMapEvent("a11",  8, "a11_CanInviteJoinTeam")
aux.RegisterMapEvent("a11",  9, "a11_CanLeaveTeam")
aux.RegisterMapEvent("a11",  10, "a11_CanChangeLeader")
aux.RegisterMapEvent("a11",  17, "a11_CanKickMember")

function MSJD_OnDisappear(MapID, InstanceID, TargetID, TargetTypeID)
    if Jidou.RoomInfo[InstanceID] ~= nil then
		if cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 0)  ~= 0 and cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 0) ~= 40 then        --��λ�ļ�ʱ��
			if Jidou.RoomInfo[InstanceID].IsOver ~= 1 then
				if cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 0) == Jidou.RoomInfo[InstanceID].RedFlag.FlagID then
					map.MapDeleteCreature(MapID, InstanceID, Jidou.RoomInfo[InstanceID].RedFlag.FlagID)
					Jidou.RoomInfo[InstanceID].RedFlag.FlagID = map.MapCreateCreature(MapID, InstanceID, 4902112,425,5483,109)--�췽����
					Jidou.RoomInfo[InstanceID].RedFlag.State = "home"
					Jidou.RoomInfo[InstanceID].RedFlag.Role = 0
					Jidou.RoomInfo[InstanceID].RedFlag.Time = 0
					map.MapCreateCreature(2108961076, Index, 4902114,64,5555,440)--��������
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 101, 12904)			--�췽�������Ѿ��ص�������Ӫ�ص���
					msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID)

				elseif cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 0) == Jidou.RoomInfo[InstanceID].BlueFlag.FlagID then
					map.MapDeleteCreature(MapID, InstanceID, Jidou.RoomInfo[InstanceID].BlueFlag.FlagID)
					Jidou.RoomInfo[InstanceID].BlueFlag.FlagID = map.MapCreateCreature(MapID, InstanceID, 4902113,97,5556,406)--��������
					Jidou.RoomInfo[InstanceID].BlueFlag.State = "home"
					Jidou.RoomInfo[InstanceID].BlueFlag.Role = 0
					Jidou.RoomInfo[InstanceID].BlueFlag.Time = 0
					map.MapCreateCreature(MapID, InstanceID,4902133 ,464,5517,96)  --�췽����
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 101, 12905)			--�����������Ѿ��ص�������Ӫ�ص���
					msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID)
				end
			end
		elseif Jidou.RoomInfo[InstanceID].Timer30  == 40 and cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 0) == 40 then        --���͵ļ�ʱ��
			map.DeleteInstance(MapID, InstanceID)
			Jidou.RoomInfo[InstanceID] = nil
		elseif Jidou.RoomInfo[InstanceID].IsOver ~= 1 then
			map.MapCreateCreature(2108961076, InstanceID, 4902134, 1, 1, 1)           --ˢ��һ��30���͸����ʱ�����ù���δ������
			Jidou.RoomInfo[InstanceID].Timer30 = Jidou.RoomInfo[InstanceID].Timer30 + 1
			MSJD_CheckIsOver(MapID, InstanceID)

			if Jidou.RoomInfo[InstanceID].Timer30 <= 39 and Jidou.RoomInfo[InstanceID].IsOver ~= 1 then

				--Ϊÿ��û�й���Ĵ�����ˢһ������
				for k,v in pairs(Jidou.RoomInfo[InstanceID].BuffTrigger) do
					if v.CreID == 0 then
						v.CreTypeID = math.random(4) + 4902118
						v.CreID = map.MapCreateColCreature(MapID, InstanceID, v.CreTypeID, v.x, v.y, v.z, 1)
					end
				end

				if Jidou.RoomInfo[InstanceID].BlueFlag.State == "outside" and Jidou.RoomInfo[InstanceID].BlueFlag.Role == 0 then
					Jidou.RoomInfo[InstanceID].BlueFlag.Time = Jidou.RoomInfo[InstanceID].BlueFlag.Time + 1
					if Jidou.RoomInfo[InstanceID].BlueFlag.Time >= 3 then
						map.MapDeleteCreature(MapID, InstanceID, Jidou.RoomInfo[InstanceID].BlueFlag.FlagID)
						Jidou.RoomInfo[InstanceID].BlueFlag.FlagID = map.MapCreateCreature(MapID, InstanceID, 4902113,97,5556,406)--��������
						Jidou.RoomInfo[InstanceID].BlueFlag.State = "home"
						Jidou.RoomInfo[InstanceID].BlueFlag.Role = 0
						Jidou.RoomInfo[InstanceID].BlueFlag.Time = 0
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 101, 12906)			--�����������Ѿ��ص�������Ӫ�ص���
						msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID)
					end
				elseif Jidou.RoomInfo[InstanceID].RedFlag.State == "outside" and Jidou.RoomInfo[InstanceID].RedFlag.Role == 0 then
					Jidou.RoomInfo[InstanceID].RedFlag.Time = Jidou.RoomInfo[InstanceID].RedFlag.Time + 1
					if Jidou.RoomInfo[InstanceID].RedFlag.Time >= 3 then
						map.MapDeleteCreature(MapID, InstanceID, Jidou.RoomInfo[InstanceID].RedFlag.FlagID)
						Jidou.RoomInfo[InstanceID].RedFlag.FlagID = map.MapCreateCreature(MapID, InstanceID, 4902113,425,5483,109)--�췽����
						Jidou.RoomInfo[InstanceID].RedFlag.State = "home"
						Jidou.RoomInfo[InstanceID].RedFlag.Role = 0
						Jidou.RoomInfo[InstanceID].RedFlag.Time = 0
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 101, 12907)			--�췽�������Ѿ��ص�������Ӫ�ص���
						msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID)
					end
				end
			end
		end
	end
end

aux.RegisterCreatureEvent(4902134, 13, "MSJD_OnDisappear")--��ʱ����ʧ�¼�ע�ᣬ�ټ�ʱ��
