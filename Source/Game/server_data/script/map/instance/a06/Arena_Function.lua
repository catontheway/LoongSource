--[[Arena = {}                    --������¼��������ͼ������Ӧ�ľ�����������Լ�����˫��ʣ�������
PlayerStillInArena = {}              --������¼���뾺������û���뿪�����
PlayerTransToArena = {}          --������¼�����ͽ������������
Alive_Arena = {}              --������¼�������д������
Totle_Arena = {}               --������¼�����������е����
ArenaCreature_Timer = {}           --������¼�������еļ�ʱ���������ڵĸ���ID������ID��
	ArenaCreature_Timer[creatureID]= {TimerTypeID}
	   TimerTypeIDö��ֵ
	   1 1���ӵĿ��ż�ʱ��
	   2 10���ӵĵ���ʱ��ʱ��
	   3 30��Ĺ��ɼ�ʱ������15��ģ�
	   4 15������Լ�ʱ��������15��Ϳ��ŵļ�ʱ����
	   5 10���ӵĽ����ʱ�����ڿ��ź�ˢ�����ӿ��أ����û�����򲻻ᴥ��Ч��
	   6 15��Ĵ��ͼ�ʱ��

BeInArena = {}                --��¼������Ƿ�ϵͳ���ͽ���������������ǣ����䴫�������ܾ�����.�뿪������ʱ����ֵ������
MapIDBeforeEnterArena = {}      --��¼��ҽ��뾺������ǰ�ĵ�ͼID����Ҫ������뿪ʱɾ����
BattleEnd_Arena = {}          --��¼�ø���������Ƿ��Ѿ����㣨�����˲��뿪����Ҳ��ᱻ�۷�)
BattleBegin_Arena = {}          --��¼�ø����Ƿ��Ѿ��򿪴��ţ����˴��Ų��ܿ۷֣������˳����۷�)
KillPeople_Arena = {}         --��¼����ھ�������ɱ��������
Arena_Minutes = {}            --��¼��Ϸ���е����ڵķ�����]]

--���徺������صĲ���
function DefineValue_Arena (InstanceID, RoomID, RoleID1, RoleID2, RoleID3, RoleID4, RoleID5, RoleID6)

    Arena[InstanceID] = {Room = RoomID, red = 0, blue = 0, Type = 1}                --��¼�õ�ͼ��Ӧ�ķ����

	BattleEnd_Arena[InstanceID] = 0
	BattleBegin_Arena[InstanceID] = 0

	KillPeople_Arena[InstanceID] = {}
	KillPeople_Arena[InstanceID][1] = 0
	KillPeople_Arena[InstanceID][2] = 0
	KillPeople_Arena[InstanceID][3] = 0
	KillPeople_Arena[InstanceID][4] = 0
	KillPeople_Arena[InstanceID][5] = 0
	KillPeople_Arena[InstanceID][6] = 0

    Alive_Arena[InstanceID] = {}
	Alive_Arena[InstanceID][1] = -1
	Alive_Arena[InstanceID][2] = -1
	Alive_Arena[InstanceID][3] = -1
	Alive_Arena[InstanceID][4] = -1
	Alive_Arena[InstanceID][5] = -1
	Alive_Arena[InstanceID][6] = -1

	PlayerStillInArena[InstanceID] = {}
	PlayerStillInArena[InstanceID][1] = -1
	PlayerStillInArena[InstanceID][2] = -1
	PlayerStillInArena[InstanceID][3] = -1
	PlayerStillInArena[InstanceID][4] = -1
	PlayerStillInArena[InstanceID][5] = -1
	PlayerStillInArena[InstanceID][6] = -1

	PlayerTransToArena[InstanceID] = {}
	PlayerTransToArena[InstanceID][1] = -1
	PlayerTransToArena[InstanceID][2] = -1
	PlayerTransToArena[InstanceID][3] = -1
	PlayerTransToArena[InstanceID][4] = -1
	PlayerTransToArena[InstanceID][5] = -1
	PlayerTransToArena[InstanceID][6] = -1

	Totle_Arena[InstanceID] = {}
	Totle_Arena[InstanceID][1] = -1
	Totle_Arena[InstanceID][2] = -1
	Totle_Arena[InstanceID][3] = -1
	Totle_Arena[InstanceID][4] = -1
	Totle_Arena[InstanceID][5] = -1
	Totle_Arena[InstanceID][6] = -1

	Arena_Minutes[InstanceID] = 0

	local RoleID = {}
	RoleID[1] = RoleID1
	RoleID[2] = RoleID2
	RoleID[3] = RoleID3
	RoleID[4] = RoleID4
	RoleID[5] = RoleID5
	RoleID[6] = RoleID6

	RankPoint_Arena[InstanceID] = {}
	for i = 1, 6 do
	    local MapID, Instance_ID = role.GetRoleMapID(RoleID[i])
		if i <= 3 then
		    RankPoint_Arena[InstanceID][i] = {RankPoint = role.GetRoleAttValue(MapID, Instance_ID, RoleID[i], 79), ArenaPoint = role.GetRoleAttValue(MapID, Instance_ID, RoleID[i], 80), EnemyPoint = RankPoint_count(RoleID4, RoleID5, RoleID6), GetRankPoint = 0, GetArenaPoint = 0, GetRankPoint_today = 0, GetRankPoint_max = 100}
		else
		    RankPoint_Arena[InstanceID][i] = {RankPoint = role.GetRoleAttValue(MapID, Instance_ID, RoleID[i], 79), ArenaPoint = role.GetRoleAttValue(MapID, Instance_ID, RoleID[i], 80), EnemyPoint = RankPoint_count(RoleID1, RoleID2, RoleID3), GetRankPoint = 0, GetArenaPoint = 0, GetRankPoint_today = 0, GetRankPoint_max = 100}
        end
	end

	for i = 1, 6 do
	    local buff = {1,2,3,4,5,6,7,8,9,10}
		local id1 = math.random(1,10)
		local buff1 = buff[id1]
        table.remove(buff, id1)
		local id2 = math.random(1,9)
		local buff2 = buff[id2]
		table.remove(buff, id2)
		local id3 = math.random(1,8)
		local buff3 = buff[id3]
	    map.SetMapScriptData(2092185140,InstanceID,1,i,buff3*10000+buff2*100+buff1)
	end
end


--ɾ�������ֵ���ͷ��ڴ�
function RemoveValue_Arena (InstanceID)

    for i=1,6 do
		if PlayerStillInArena[InstanceID][i] ~= -1 and PlayerStillInArena[InstanceID][i] ~= nil then
			MapIDBeforeEnterArena[PlayerStillInArena[InstanceID][i]] = nil
			if BeInArena[PlayerStillInArena[InstanceID][i]] ~= nil then
				BeInArena[PlayerStillInArena[InstanceID][i]] = nil
			end
		end
	end
	Arena[InstanceID] = nil
	Alive_Arena[InstanceID] = nil
	Totle_Arena[InstanceID] = nil
	BattleEnd_Arena[InstanceID] = nil
	PlayerStillInArena[InstanceID] = nil
    BattleBegin_Arena[InstanceID] = nil
    KillPeople_Arena[InstanceID] = nil
	RankPoint_Arena[InstanceID] = nil
	Arena_Minutes[InstanceID] = nil

end


--�����е������ʾһ���µĴ���б�
function ShowNewAliveFrame(InstanceID)
    for i=1, 3 do
	    if PlayerStillInArena[InstanceID][i] ~= -1 and PlayerStillInArena[InstanceID][i] ~= nil then
		    role.ShowAlivePeopleInArena(PlayerStillInArena[InstanceID][i], Arena[InstanceID].red, Arena[InstanceID].blue)
		end
	end
	for i=4, 6 do
	    if PlayerStillInArena[InstanceID][i] ~= -1 and PlayerStillInArena[InstanceID][i] ~= nil then
		    role.ShowAlivePeopleInArena(PlayerStillInArena[InstanceID][i], Arena[InstanceID].blue, Arena[InstanceID].red)
		end
	end
end

--����һ����ҵ���ѫ�ȼ�
function RankPoint_Self(INT)
    if INT >= 15000 then
        return 10
	elseif INT >= 10000 then
	    return 9
	elseif INT >= 6000 then
	    return 8
	elseif INT >= 3000 then
	    return 7
	elseif INT >= 2000 then
	    return 6
	elseif INT >= 1000 then
	    return 5
	elseif INT >= 500 then
	    return 4
	elseif INT >= 200 then
	    return 3
	elseif INT >= 40 then
	    return 2
	else
	    return 1
	end
end

--����һ����ҵ��似ֵ����
function ArenaPoint_Max(INT)
    if INT >= 15000 then
        return 4000
	elseif INT >= 10000 then
	    return 2600
	elseif INT >= 6000 then
	    return 2300
	elseif INT >= 3000 then
	    return 2000
	elseif INT >= 2000 then
	    return 1700
	elseif INT >= 1000 then
	    return 1400
	elseif INT >= 500 then
	    return 1100
	elseif INT >= 200 then
	    return 800
	elseif INT >= 40 then
	    return 500
	else
	    return 200
	end
end

--����һ����ҵĻ�õ��似ֵ
function ArenaPoint_count(minutes, win_fail)
    if minutes >= 10 then
	    if win_fail == 1 then
            return 12
		else
		    return 3
		end
	elseif minutes >= 7 then
	    if win_fail == 1 then
            return 8
		else
		    return 2
		end
	elseif minutes >= 4 then
	    if win_fail == 1 then
            return 5
		else
		    return 1
		end
	elseif minutes >= 2 then
	    if win_fail == 1 then
            return 2
		else
		    return 1
		end
	else
	    return 0
	end
end

--����һ�������ƽ������ȼ�
function RankPoint_count(RoleID1, RoleID2, RoleID3)
    local MapID1, InstanceID1 = role.GetRoleMapID(RoleID1)
	local MapID2, InstanceID2 = role.GetRoleMapID(RoleID2)
	local MapID3, InstanceID3 = role.GetRoleMapID(RoleID3)
    local RankPoint={role.GetRoleAttValue(MapID1, InstanceID1, RoleID1, 79), role.GetRoleAttValue(MapID2, InstanceID2, RoleID2, 79), role.GetRoleAttValue(MapID3, InstanceID3, RoleID1, 79)}

	for i=1,3 do
        if RankPoint[i] == nil then
	        RankPoint[i] = 0
		end
	end

	local temp = 0
	if RankPoint[1] < RankPoint[2] then
	    temp = RankPoint[1]
		RankPoint[1] = RankPoint[2]
		RankPoint[2] = temp
	end
	if RankPoint[2] < RankPoint[3] then
	    temp = RankPoint[2]
		RankPoint[2] = RankPoint[3]
		RankPoint[3] = temp
	end
	if RankPoint[1] < RankPoint[2] then
	    temp = RankPoint[1]
		RankPoint[1] = RankPoint[2]
		RankPoint[2] = temp
	end

    local Rank = {}
	for i=1,3 do
        Rank[i] = RankPoint_Self(RankPoint[i])
	end
	local RankAverage = 0
	if (Rank[1] - Rank[3])/2 >= 2 then
	    RankAverage = math.ceil((Rank[1] + Rank[2]) / 2)
	else
	    RankAverage = math.ceil((Rank[1] + Rank[2] + Rank[3]) / 3)
    end
	return RankAverage
end

--����һ����ҵķ���
function CountPoint_Arena(RoomID, InstanceID, RoleID1, RoleID2, RoleID3, RoleID4, RoleID5, RoleID6, RolePos, Win_fail, In_out)  --6���˶��ǰ���123һ��456һ��,Win_fail��0���䣬1��Ӯ,2��ƽ��In_out��0�������棬1��������
    local Role = {RoleID1, RoleID2, RoleID3, RoleID4, RoleID5, RoleID6}
	--local q = {0, 0, 0.5, 0.5, 1, 1, 2, 2, 3, 3}
	--local room_type = role.GetArenaType(RoomID,Role[RolePos])
	local RankAverage = RankPoint_Arena[InstanceID][RolePos].EnemyPoint
	--[[if RolePos <= 3 then
        RankAverage = RankPoint_count(RoleID4, RoleID5, RoleID6)
	else
	    RankAverage = RankPoint_count(RoleID1, RoleID2, RoleID3)
	end]]
	local RoleRank = RankPoint_Self(RankPoint_Arena[InstanceID][RolePos].RankPoint)
	local GetRankPoint,GetArenaPoint = 0, 0
	if BattleBegin_Arena[InstanceID] == 1 or PlayerStillInArena[InstanceID][RolePos] == -1 then   --������Ѿ��򿪻�������뿪������
		if Win_fail == 1 then
			GetRankPoint = 5
			GetArenaPoint = ArenaPoint_count(Arena_Minutes[InstanceID], 1)
			if RoleRank - RankAverage > 3 then
				GetArenaPoint = 0
			end
			if RoleRank - RankAverage > 2 then
				GetRankPoint = 0
			end
			--if In_out == 1 then
				local Date = role.GetRoleScriptData(Role[RolePos], 1, RoleDataType["Date_Arena"])
				local CurTime = tonumber(os.date("%j"))
				if Date ~= CurTime then
					role.SetRoleScriptData(Role[RolePos], 1, RoleDataType["Date_Arena"], CurTime)
					role.SetRoleScriptData(Role[RolePos], 1, RoleDataType["TimesToday_Arena"], 0)
					role.SetRoleScriptData(Role[RolePos], 1, RoleDataType["RankToday_Arena"], 0)
				end
				if GetRankPoint > 0 then
				    local TotleTimes_Arena = role.GetRoleScriptData(Role[RolePos], 1, RoleDataType["TotleTimes_Arena"])
					role.SetRoleScriptData(Role[RolePos], 1, RoleDataType["TotleTimes_Arena"], TotleTimes_Arena + 1)

					local WinTimes_Arena = role.GetRoleScriptData(Role[RolePos], 1, RoleDataType["WinTimes_Arena"])
					role.SetRoleScriptData(Role[RolePos], 1, RoleDataType["WinTimes_Arena"], WinTimes_Arena + 1)

					local TimesToday = role.GetRoleScriptData(Role[RolePos], 1, RoleDataType["TimesToday_Arena"])
					TimesToday = TimesToday + 1
					role.SetRoleScriptData(Role[RolePos], 1, RoleDataType["TimesToday_Arena"], TimesToday)
					if TimesToday == 10 then
						GetRankPoint = GetRankPoint + 20
						--��ʾ���
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 2511)   --�������Ѿ��μ�##����ƽ�ľ���������������˶����25����ѫ
						msg.AddMsgEvent(MsgID, 9, TimesToday)
						msg.DispatchRoleMsgEvent(Role[RolePos], MsgID)     --������Ϣ
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13, 0)
						msg.AddMsgEvent(MsgID, 1, 2511)   --�������Ѿ��μ�##����ƽ�ľ���������������˶����25����ѫ
						msg.AddMsgEvent(MsgID, 9, TimesToday)
						msg.DispatchRoleMsgEvent(Role[RolePos], MsgID)     --������Ϣ
					elseif TimesToday < 10 then
						--��ʾ���
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 2512)   --�������Ѿ��μ�##����ƽ�ľ���������
						msg.AddMsgEvent(MsgID, 9, TimesToday)
						msg.DispatchRoleMsgEvent(Role[RolePos], MsgID)     --������Ϣ
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13, 0)
						msg.AddMsgEvent(MsgID, 1, 2512)   --�������Ѿ��μ�##����ƽ�ľ���������
						msg.AddMsgEvent(MsgID, 9, TimesToday)
						msg.DispatchRoleMsgEvent(Role[RolePos], MsgID)     --������Ϣ
					end
				end

				GetRankPoint = RankPointToday_count(InstanceID, GetRankPoint, Role[RolePos], RolePos)
				--[[local RankPointToday = role.GetRoleScriptData(Role[RolePos], 1, RoleDataType["RankToday_Arena"])
				RankPointToday = GetRankPoint + RankPointToday
				local dayinweek = os.date("%A")
				if day == "Saturday" or day == "Sunday" then

				    RankPoint_Arena[InstanceID][RolePos].GetRankPoint_max = 200

					if RankPointToday > 200 then
						GetRankPoint = GetRankPoint + 200 - RankPointToday
						RankPointToday = 200
					end

				else

				    if RankPointToday > 100 then
						GetRankPoint = GetRankPoint + 100 - RankPointToday
						RankPointToday = 100
					end

				end

				RankPoint_Arena[InstanceID][RolePos].GetRankPoint_today = RankPointToday
				role.SetRoleScriptData(Role[RolePos], 1, RoleDataType["RankToday_Arena"], RankPointToday)]]

				if GetArenaPoint > 0 then
					local ArenaPoint = RankPoint_Arena[InstanceID][RolePos].ArenaPoint
					ArenaPoint = ArenaPoint + GetArenaPoint
					local ArenaPoint_Max = ArenaPoint_Max(RankPoint_Arena[InstanceID][RolePos].RankPoint)
					if ArenaPoint > ArenaPoint_Max then
						GetArenaPoint = GetArenaPoint -  ArenaPoint + ArenaPoint_Max
						--��ʾ���
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 2513)   --����Ҫ�Ƚ��似ֵ�һ��ɽ�������ܼ�������似ֵ
						msg.DispatchRoleMsgEvent(Role[RolePos], MsgID)     --������Ϣ
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13, 0)
						msg.AddMsgEvent(MsgID, 1, 2513)   --����Ҫ�Ƚ��似ֵ�һ��ɽ�������ܼ�������似ֵ
						msg.DispatchRoleMsgEvent(Role[RolePos], MsgID)     --������Ϣ
					end
				end
			--end
		elseif Win_fail == 0 then
			GetRankPoint = 0
			--[[if RoleRank - RankAverage + 5 < 3 then
				GetRankPoint = 0
			end]]
			if In_out == 1 then
				local Date = role.GetRoleScriptData(Role[RolePos], 1, RoleDataType["Date_Arena"])
				local CurTime = tonumber(os.date("%j"))
				if Date ~= CurTime then
					role.SetRoleScriptData(Role[RolePos], 1, RoleDataType["Date_Arena"], CurTime)
					role.SetRoleScriptData(Role[RolePos], 1, RoleDataType["TimesToday_Arena"], 0)
					role.SetRoleScriptData(Role[RolePos], 1, RoleDataType["RankToday_Arena"], 0)
				end
				--if GetRankPoint < 0 then

				    local TotleTimes_Arena = role.GetRoleScriptData(Role[RolePos], 1, RoleDataType["TotleTimes_Arena"])
					role.SetRoleScriptData(Role[RolePos], 1, RoleDataType["TotleTimes_Arena"], TotleTimes_Arena + 1)

					local TimesToday = role.GetRoleScriptData(Role[RolePos], 1, RoleDataType["TimesToday_Arena"])
					TimesToday = TimesToday + 1
					role.SetRoleScriptData(Role[RolePos], 1, RoleDataType["TimesToday_Arena"], TimesToday)
					if TimesToday == 10 then
						GetRankPoint = GetRankPoint + 20
						--��ʾ���
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 2511)   --�������Ѿ��μ�##����ƽ�ľ���������������˶����25����ѫ
						msg.AddMsgEvent(MsgID, 9, TimesToday)
						msg.DispatchRoleMsgEvent(Role[RolePos], MsgID)     --������Ϣ
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13, 0)
						msg.AddMsgEvent(MsgID, 1, 2511)   --�������Ѿ��μ�##����ƽ�ľ���������������˶����25����ѫ
						msg.AddMsgEvent(MsgID, 9, TimesToday)
						msg.DispatchRoleMsgEvent(Role[RolePos], MsgID)     --������Ϣ
					elseif TimesToday < 10 then
						--��ʾ���
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 2512)   --�������Ѿ��μ�##����ƽ�ľ���������
						msg.AddMsgEvent(MsgID, 9, TimesToday)
						msg.DispatchRoleMsgEvent(Role[RolePos], MsgID)     --������Ϣ
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13, 0)
						msg.AddMsgEvent(MsgID, 1, 2512)   --�������Ѿ��μ�##����ƽ�ľ���������
						msg.AddMsgEvent(MsgID, 9, TimesToday)
						msg.DispatchRoleMsgEvent(Role[RolePos], MsgID)     --������Ϣ
					end
				--end
                GetRankPoint = RankPointToday_count(InstanceID, GetRankPoint, Role[RolePos], RolePos)
				GetArenaPoint = ArenaPoint_count(Arena_Minutes[InstanceID], 0)

				if GetArenaPoint > 0 then
					local ArenaPoint = RankPoint_Arena[InstanceID][RolePos].ArenaPoint
					ArenaPoint = ArenaPoint + GetArenaPoint
					local ArenaPoint_Max = ArenaPoint_Max(RankPoint_Arena[InstanceID][RolePos].RankPoint)
					if ArenaPoint > ArenaPoint_Max then
						GetArenaPoint = GetArenaPoint -  ArenaPoint + ArenaPoint_Max
						--��ʾ���
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 2513)   --����Ҫ�Ƚ��似ֵ�һ��ɽ�������ܼ�������似ֵ
						msg.DispatchRoleMsgEvent(Role[RolePos], MsgID)     --������Ϣ
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13, 0)
						msg.AddMsgEvent(MsgID, 1, 2513)   --����Ҫ�Ƚ��似ֵ�һ��ɽ�������ܼ�������似ֵ
						msg.DispatchRoleMsgEvent(Role[RolePos], MsgID)     --������Ϣ
					end
				end
			end
		elseif Win_fail == 2 then
			if In_out == 1 then
			    local Date = role.GetRoleScriptData(Role[RolePos], 1, RoleDataType["Date_Arena"])
				local CurTime = tonumber(os.date("%j"))
				if Date ~= CurTime then
					role.SetRoleScriptData(Role[RolePos], 1, RoleDataType["Date_Arena"], CurTime)
					role.SetRoleScriptData(Role[RolePos], 1, RoleDataType["TimesToday_Arena"], 0)
					role.SetRoleScriptData(Role[RolePos], 1, RoleDataType["RankToday_Arena"], 0)
				end

				local TotleTimes_Arena = role.GetRoleScriptData(Role[RolePos], 1, RoleDataType["TotleTimes_Arena"])
				role.SetRoleScriptData(Role[RolePos], 1, RoleDataType["TotleTimes_Arena"], TotleTimes_Arena + 1)

				local TimesToday = role.GetRoleScriptData(Role[RolePos], 1, RoleDataType["TimesToday_Arena"])
				TimesToday = TimesToday + 1
				role.SetRoleScriptData(Role[RolePos], 1, RoleDataType["TimesToday_Arena"], TimesToday)
				if TimesToday == 10 then
					GetRankPoint = GetRankPoint + 20
					--��ʾ���
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 26, 2511)   --�������Ѿ��μ�##����ƽ�ľ���������������˶����25����ѫ
					msg.AddMsgEvent(MsgID, 9, TimesToday)
					msg.DispatchRoleMsgEvent(Role[RolePos], MsgID)     --������Ϣ
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 13, 0)
					msg.AddMsgEvent(MsgID, 1, 2511)   --�������Ѿ��μ�##����ƽ�ľ���������������˶����25����ѫ
					msg.AddMsgEvent(MsgID, 9, TimesToday)
					msg.DispatchRoleMsgEvent(Role[RolePos], MsgID)     --������Ϣ
				elseif TimesToday < 10 then
					--��ʾ���
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 26, 2512)   --�������Ѿ��μ�##����ƽ�ľ���������
					msg.AddMsgEvent(MsgID, 9, TimesToday)
					msg.DispatchRoleMsgEvent(Role[RolePos], MsgID)     --������Ϣ
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 13, 0)
					msg.AddMsgEvent(MsgID, 1, 2512)   --�������Ѿ��μ�##����ƽ�ľ���������
					msg.AddMsgEvent(MsgID, 9, TimesToday)
					msg.DispatchRoleMsgEvent(Role[RolePos], MsgID)     --������Ϣ
				end

				GetRankPoint = RankPointToday_count(InstanceID, GetRankPoint, Role[RolePos], RolePos)
				GetArenaPoint = ArenaPoint_count(Arena_Minutes[InstanceID], 0)


				local ArenaPoint = RankPoint_Arena[InstanceID][RolePos].ArenaPoint
				ArenaPoint = ArenaPoint + GetArenaPoint
				local RankArenaPoint_Max = ArenaPoint_Max(RankPoint_Arena[InstanceID][RolePos].RankPoint)
				if ArenaPoint > RankArenaPoint_Max then
					GetArenaPoint = GetArenaPoint -  ArenaPoint + RankArenaPoint_Max
					--��ʾ���
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 26, 2513)   --����Ҫ�Ƚ��似ֵ�һ��ɽ�������ܼ�������似ֵ
					msg.DispatchRoleMsgEvent(Role[RolePos], MsgID)     --������Ϣ
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 13, 0)
					msg.AddMsgEvent(MsgID, 1, 2513)   --����Ҫ�Ƚ��似ֵ�һ��ɽ�������ܼ�������似ֵ
					msg.DispatchRoleMsgEvent(Role[RolePos], MsgID)     --������Ϣ
				end
			end
		end
	end
	RankPoint_Arena[InstanceID][RolePos].GetRankPoint = GetRankPoint
	RankPoint_Arena[InstanceID][RolePos].GetArenaPoint = GetArenaPoint
	return GetRankPoint,GetArenaPoint
end

--����������Ҹ����л��ھ������е������֣�����ַ�����Ȼ���͸�������
function ShowFinalFrame(MapID, InstanceID)
--�����ʱһ������Ϊ0�������ս����BattleBegin_Arena=0��BattleEnd_Arena=1��û������
	local acount = {}
	acount[1] = {-1,-1,-1,-1,-1,-1,-1,-1,-1}
	acount[2] = {-1,-1,-1,-1,-1,-1,-1,-1,-1}
	acount[3] = {-1,-1,-1,-1,-1,-1,-1,-1,-1}
	acount[4] = {-1,-1,-1,-1,-1,-1,-1,-1,-1}
	acount[5] = {-1,-1,-1,-1,-1,-1,-1,-1,-1}
	acount[6] = {-1,-1,-1,-1,-1,-1,-1,-1,-1}
	for i=1,6 do
		acount[i][1] = Totle_Arena[InstanceID][i]
		if i<=3 then
			acount[i][2] = 1
		else
			acount[i][2] = 0
		end
		acount[i][3] = RankPoint_Self(RankPoint_Arena[InstanceID][i].RankPoint)
		acount[i][4] = KillPeople_Arena[InstanceID][i]
		local Winnercolour = 0           --����ʤ
		for j=1,3 do
			if Alive_Arena[InstanceID][j] ~= -1 then
				Winnercolour = 1          --�췽ʤ
				break
			end
		end
		if Arena[InstanceID].red == Arena[InstanceID].blue then
			Winnercolour = 2               --ƽ��
		elseif Arena[InstanceID].red < Arena[InstanceID].blue then
		    Winnercolour = 0
		end

		if PlayerStillInArena[InstanceID][i] ~= -1 then
		    if Arenaact_isopen == 1 then                  --������������ؽű�
			    role.AddRoleItem(MapID, InstanceID, Totle_Arena[InstanceID][i], 3300102, 1, -1, 8, 420)
				if Arenaact_times[Totle_Arena[InstanceID][i]] == nil then
				    Arenaact_times[Totle_Arena[InstanceID][i]] = 0
				end
			    Arenaact_times[Totle_Arena[InstanceID][i]] = Arenaact_times[Totle_Arena[InstanceID][i]] + 1
				if Arenaact_times[Totle_Arena[InstanceID][i]] <= 9 then
				    local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 71, 2750)	--<font=С���ź�,0,21,20><color=0xffff0000>������������ɶȣ�<color=0xfffff7e0><color=0xff05ff00>X/10<color=0xfffff7e0><color=0xffff0000>��ÿ������21��00��22��00���μӾ������������ܻ��<color=0xfffff7e0><color=0xff05ff00>�ɽ�ɽ���<color=0xfffff7e0><color=0xffff0000>��ͬʱֻҪ���10���������������ܻ�ô������飡<color=0xfffff7e0><font=С���ź�,0,19,20>
					msg.AddMsgEvent(MsgID, 9, Arenaact_times[Totle_Arena[InstanceID][i]])
					msg.DispatchRoleMsgEvent(Totle_Arena[InstanceID][i], MsgID)
				elseif Arenaact_times[Totle_Arena[InstanceID][i]] == 10 then
				    local level = role.GetRoleLevel(MapID, InstanceID, Totle_Arena[InstanceID][i])
					local roleexp = math.floor(activity_newexp[level]*0.8)
					role.AddRoleExp(MapID, InstanceID, Totle_Arena[InstanceID][i], roleexp)
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 71, 2751)	--<font=С���ź�,0,21,20><color=0xffff0000>������������ɣ���þ���<color=0xfffff7e0><color=0xff05ff00>roleexp<color=0xfffff7e0><color=0xffff0000>�㣡<color=0xfffff7e0><font=С���ź�,0,19,20>
					msg.AddMsgEvent(MsgID, 9, roleexp)
					msg.DispatchRoleMsgEvent(Totle_Arena[InstanceID][i], MsgID)
					if guangbo_Arena == 0 then
						guangbo_Arena = 1
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 102, 100105)  --���������š�XXX����˾����������������xxx�㾭�飡
						msg.AddMsgEvent(MsgID, 2, Totle_Arena[InstanceID][i])
						msg.AddMsgEvent(MsgID, 9, roleexp)
						msg.DispatchMapMsgEvent(MsgID, -1, -1, -1)
					else
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 102, 100105)  --���������š�XXX����˾����������������XXX�㾭�飡
						msg.AddMsgEvent(MsgID, 2, RoleID)
						msg.AddMsgEvent(MsgID, 9, roleexp)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
					end
				end
			end

			if Winnercolour == 1 then
				if i <= 3 then
					acount[i][5], acount[i][6] = CountPoint_Arena(Arena[InstanceID].Room, InstanceID, Totle_Arena[InstanceID][1], Totle_Arena[InstanceID][2], Totle_Arena[InstanceID][3], Totle_Arena[InstanceID][4], Totle_Arena[InstanceID][5], Totle_Arena[InstanceID][6], i, 1, 1)
					role.ModRoleAttValue(MapID, InstanceID, Totle_Arena[InstanceID][i], 79, acount[i][5])
				    role.ModRoleAttValue(MapID, InstanceID, Totle_Arena[InstanceID][i], 80, acount[i][6])
					if Arena[InstanceID].red == 3 then
						acount[i][7] = 1
					else
						acount[i][7] = 2
					end
					if Arena[InstanceID].Type == 0 and acount[i][5] > 1 then
					    local level = role.GetRoleLevel(MapID, InstanceID, Totle_Arena[InstanceID][i])
						local itemnum, itemid = GetRandomArenaItem(level)
						role.AddRoleItem(MapID, InstanceID, Totle_Arena[InstanceID][i], itemid, itemnum, -1, 8, 420)
						if itemid == 1351208 then
						    local MsgID = msg.BeginMsgEvent()
							msg.AddMsgEvent(MsgID, 101, 2550)	--XXX�������������л����һ����ţ֮�ۣ�
							msg.AddMsgEvent(MsgID, 2, Totle_Arena[InstanceID][i])
							msg.DispatchWorldMsgEvent(MsgID)
						end
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 71, 2538)	--<font=С���ź�,0,21,20><color=0xffff0000>���������ģʽʤ�������<color=0xfffff7e0><color=0xff05ff00>n<color=0xfffff7e0><color=0xffff0000>��<color=0xfffff7e0><color=0xff05ff00>XXX<color=0xfffff7e0><font=С���ź�,0,19,20>
						msg.AddMsgEvent(MsgID, 9, itemnum)
						msg.AddMsgEvent(MsgID, 4, itemid)
						msg.DispatchRoleMsgEvent(Totle_Arena[InstanceID][i], MsgID)
                    end
				else
					acount[i][5], acount[i][6] = CountPoint_Arena(Arena[InstanceID].Room, InstanceID, Totle_Arena[InstanceID][1], Totle_Arena[InstanceID][2], Totle_Arena[InstanceID][3], Totle_Arena[InstanceID][4], Totle_Arena[InstanceID][5], Totle_Arena[InstanceID][6], i, 0, 1)
					role.ModRoleAttValue(MapID, InstanceID, Totle_Arena[InstanceID][i], 79, acount[i][5])
				    role.ModRoleAttValue(MapID, InstanceID, Totle_Arena[InstanceID][i], 80, acount[i][6])
					acount[i][7] = 3
				end
			elseif Winnercolour == 0 then
				if i <= 3 then
					acount[i][5], acount[i][6] = CountPoint_Arena(Arena[InstanceID].Room, InstanceID, Totle_Arena[InstanceID][1], Totle_Arena[InstanceID][2], Totle_Arena[InstanceID][3], Totle_Arena[InstanceID][4], Totle_Arena[InstanceID][5], Totle_Arena[InstanceID][6], i, 0, 1)
					role.ModRoleAttValue(MapID, InstanceID, Totle_Arena[InstanceID][i], 79, acount[i][5])
				    role.ModRoleAttValue(MapID, InstanceID, Totle_Arena[InstanceID][i], 80, acount[i][6])
					acount[i][7] = 3
				else
					acount[i][5], acount[i][6] = CountPoint_Arena(Arena[InstanceID].Room, InstanceID, Totle_Arena[InstanceID][1], Totle_Arena[InstanceID][2], Totle_Arena[InstanceID][3], Totle_Arena[InstanceID][4], Totle_Arena[InstanceID][5], Totle_Arena[InstanceID][6], i, 1, 1)
					role.ModRoleAttValue(MapID, InstanceID, Totle_Arena[InstanceID][i], 79, acount[i][5])
				    role.ModRoleAttValue(MapID, InstanceID, Totle_Arena[InstanceID][i], 80, acount[i][6])
					if Arena[InstanceID].blue == 3 then
						acount[i][7] = 1
					else
						acount[i][7] = 2
					end
					if Arena[InstanceID].Type == 0 and acount[i][5] > 1 then
						local level = role.GetRoleLevel(MapID, InstanceID, Totle_Arena[InstanceID][i])
						local itemnum, itemid = GetRandomArenaItem(level)
						role.AddRoleItem(MapID, InstanceID, Totle_Arena[InstanceID][i], itemid, itemnum, -1, 8, 420)
						if itemid == 1351208 then
						    local MsgID = msg.BeginMsgEvent()
							msg.AddMsgEvent(MsgID, 101, 2550)	--XXX�������������л����һ����ţ֮�ۣ�
							msg.AddMsgEvent(MsgID, 2, Totle_Arena[InstanceID][i])
							msg.DispatchWorldMsgEvent(MsgID)
						end
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 71, 2538)	--<font=С���ź�,0,21,20><color=0xffff0000>���������ģʽʤ�������<color=0xfffff7e0><color=0xff05ff00>n<color=0xfffff7e0><color=0xffff0000>��<color=0xfffff7e0><color=0xff05ff00>XXX<color=0xfffff7e0><font=С���ź�,0,19,20>
						msg.AddMsgEvent(MsgID, 9, itemnum)
						msg.AddMsgEvent(MsgID, 4, itemid)
						msg.DispatchRoleMsgEvent(Totle_Arena[InstanceID][i], MsgID)
                    end
				end
			elseif Winnercolour == 2 then
				acount[i][5], acount[i][6] = CountPoint_Arena(Arena[InstanceID].Room, InstanceID, Totle_Arena[InstanceID][1], Totle_Arena[InstanceID][2], Totle_Arena[InstanceID][3], Totle_Arena[InstanceID][4], Totle_Arena[InstanceID][5], Totle_Arena[InstanceID][6], i, 2, 1)
				role.ModRoleAttValue(MapID, InstanceID, Totle_Arena[InstanceID][i], 79, acount[i][5])
				role.ModRoleAttValue(MapID, InstanceID, Totle_Arena[InstanceID][i], 80, acount[i][6])
				acount[i][7] = 4
			end
		else
			acount[i][5] = RankPoint_Arena[InstanceID][i].GetRankPoint
			acount[i][6] = RankPoint_Arena[InstanceID][i].GetArenaPoint
			acount[i][7] = 5
		end
		acount[i][8] = RankPoint_Arena[InstanceID][i].GetRankPoint_today
		acount[i][9] = RankPoint_Arena[InstanceID][i].GetRankPoint_max
	end
	local char = nil
    for i=1,6 do
	    for j =1,9 do
		    if i == 1 and j == 1 then
		        char = tostring(acount[1][1])
			else
			    char = char..","..tostring(acount[i][j])
			end
		end
	end
	for i= 1,6 do
        role.ShowArenaEndFrame(Totle_Arena[InstanceID][i], char)
		if Alive_Arena[InstanceID][i] ~= -1 then
		    unit.AddBuff(MapID, InstanceID, Alive_Arena[InstanceID][i], 2025301, Alive_Arena[InstanceID][i])
		end
	end
end

--�жϸ�ID�ڱ����ĸ�λ�õĺ���
function SearchARoleID(btable, RoleID)
    for i=1, 6 do
	    if btable ~= nil and btable[i] == RoleID then
			return i
		end
	end
	return -1
end

--���㵱����Ի����ѫֵ�ĺ���
function RankPointToday_count(InstanceID, GetRankPoint, RoleID, Pos)
	local RankPointToday = role.GetRoleScriptData(RoleID, 1, RoleDataType["RankToday_Arena"])
	RankPointToday = GetRankPoint + RankPointToday
	local GetRankPoint1 =  GetRankPoint
	if Arena[InstanceID].Type == 1 then
		local dayinweek = os.date("%A")
		if dayinweek == "Saturday" or dayinweek == "Sunday" then

			RankPoint_Arena[InstanceID][Pos].GetRankPoint_max = 200

			if RankPointToday > 200 then
				GetRankPoint1 = GetRankPoint + 200 - RankPointToday
				RankPointToday = 200
			end

		else

			if RankPointToday > 100 then
				GetRankPoint1 = GetRankPoint + 100 - RankPointToday
				RankPointToday = 100
			end

		end

		RankPoint_Arena[InstanceID][Pos].GetRankPoint_today = RankPointToday
		role.SetRoleScriptData(RoleID, 1, RoleDataType["RankToday_Arena"], RankPointToday)
    end
	return GetRankPoint1

end

--�ж�����ܲ��ܽ��뾺����
function JoinArenaLimit(MapID, InstanceID, RoleID)
    if role.IsRoleInStatus(RoleID, 4) and MapID ~= 2092184628 then
        local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 2539)   --������������״̬���޷����뾺��������
		msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
        role.DeletRoleFromArenaTable(RoleID)
		return false
	end

	if role.IsRoleInStatus(RoleID, 65536) then
	    local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 2540)   --������������״̬���޷����뾺��������
		msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
        role.DeletRoleFromArenaTable(RoleID)
		return false
	end

	local x, y, z= unit.GetPosition(MapID, InstanceID, RoleID)
	if MapID == 3017298127 and x < 2400 and x > 2000 and z <2500 and z > 2060 then
		return true
	elseif MapID == 3017298383 and x < 1150 and x > 629 and z <2400 and z > 1855 then
		return true
	elseif MapID == 3017299663 and x < 2250 and x > 1940 and z <2990 and z > 2280 then
		return true
	elseif MapID == 3017299919 and x < 1600 and x > 690 and z <1240 and z > 600 then
		return true
	elseif MapID == 3017299151 and x < 2300 and x > 1980 and z <1900 and z > 1640 then
		return true
	elseif MapID == 3017299407 and x < 2040 and x > 1650 and z <3000 and z > 2170 then
		return true
	elseif MapID == 2092184628 then
	    return true
	else
	    role.DeletRoleFromArenaTable(RoleID)
		--��ʾ���
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 2501)   --�����ڵ�λ�ò��ܱ����ͽ�������
		msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
		return false
	end
end

--�ж�����ܲ��ܽ��뾺��������
function JoinArenaRoomLimit(MapID, InstanceID, RoleID)

    if role.IsRoleInStatus(RoleID, 4) and MapID ~= 2092184628 then
        local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 2539)   --������������״̬���޷����뾺��������
		msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
		return false
	end

	if role.IsRoleInStatus(RoleID, 65536) then
	    local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 2540)   --������������״̬���޷����뾺��������
		msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
       -- role.DeletRoleFromArenaTable(RoleID)
		return false
	end

	local x, y, z= unit.GetPosition(MapID, InstanceID, RoleID)
	if MapID == 3017298127 and x < 2400 and x > 2000 and z <2500 and z > 2060 then
		return true
	elseif MapID == 3017298383 and x < 1150 and x > 629 and z <2400 and z > 1855 then
		return true
	elseif MapID == 3017299663 and x < 2250 and x > 1940 and z <2990 and z > 2280 then
		return true
	elseif MapID == 3017299919 and x < 1600 and x > 690 and z <1240 and z > 600 then
		return true
	elseif MapID == 3017299151 and x < 2300 and x > 1980 and z <1900 and z > 1640 then
		return true
	elseif MapID == 3017299407 and x < 2040 and x > 1650 and z <3000 and z > 2170 then
		return true
	elseif MapID == 2092184628 then
	    return true
	else
	    --��ʾ���
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 2549)   --�����봦�ڸ�������֮�в���ִ�д������
		msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
		return false
	end

end

--�������������ҿ�����������Ӧ�ļ�ʱ����
function GetRandomArenaItem(level)
    local k = math.random(100)
	if k <= 42 then
	    return 1,1351213
	elseif k <= 43 then
	    return 1,1351212
	elseif k <= 47 then
	    return 1,1351211
	elseif k <= 48 then
	    local CurTime = tonumber(os.date("%j"))
		if act.GetActScriptData(99, 1, 15) == 0 then
		    act.SetActScriptData(99, 1, 15, CurTime)
		end
		if CurTime - act.GetActScriptData(99, 1, 15) > 6 then
		    act.SetActScriptData(99, 1, 15, CurTime)
			act.SetActScriptData(99, 1, 14, 0)
			act.SetActScriptData(99, 1, 13, 0)
		end
		if CurTime < act.GetActScriptData(99, 1, 15) then
		    act.SetActScriptData(99, 1, 15, CurTime)
			act.SetActScriptData(99, 1, 14, 0)
			act.SetActScriptData(99, 1, 13, 0)
		end
	    if math.floor(act.GetActScriptData(99, 1, 13) /  (CurTime - act.GetActScriptData(99, 1, 15) + 1))  < 10 then
		    act.SetActScriptData(99, 1, 13, act.GetActScriptData(99, 1, 13)+1)
			act.SaveActScriptData(99)
		    return 1,1351210
		else
		    act.SaveActScriptData(99)
		    return 1,1351211
		end
	elseif k <= 49 then
	    local CurTime = tonumber(os.date("%j"))
		if act.GetActScriptData(99, 1, 15) == 0 then
		    act.SetActScriptData(99, 1, 15, CurTime)
		end
		if CurTime - act.GetActScriptData(99, 1, 15) > 6 then
		    act.SetActScriptData(99, 1, 15, CurTime)
			act.SetActScriptData(99, 1, 14, 0)
			act.SetActScriptData(99, 1, 13, 0)
		end
		if CurTime < act.GetActScriptData(99, 1, 15) then
		    act.SetActScriptData(99, 1, 15, CurTime)
			act.SetActScriptData(99, 1, 14, 0)
			act.SetActScriptData(99, 1, 13, 0)
		end
	    if math.floor(act.GetActScriptData(99, 1, 13) /  (CurTime - act.GetActScriptData(99, 1, 15) + 1))  < 10 then
		    act.SetActScriptData(99, 1, 13, act.GetActScriptData(99, 1, 13)+1)
			act.SaveActScriptData(99)
		    return 1,1351209
		else
		    act.SaveActScriptData(99)
		    return 1,1351211
		end
	elseif k <= 50 then
	    if math.floor(100) == 50 then
		    local CurTime = tonumber(os.date("%j"))
			if act.GetActScriptData(99, 1, 15) == 0 then
				act.SetActScriptData(99, 1, 15, CurTime)
			end
			if CurTime - act.GetActScriptData(99, 1, 15) > 6 then
				act.SetActScriptData(99, 1, 15, CurTime)
				act.SetActScriptData(99, 1, 14, 0)
				act.SetActScriptData(99, 1, 13, 0)
			end
			if CurTime < act.GetActScriptData(99, 1, 15) then
				act.SetActScriptData(99, 1, 15, CurTime)
				act.SetActScriptData(99, 1, 14, 0)
				act.SetActScriptData(99, 1, 13, 0)
			end
			if act.GetActScriptData(99, 1, 14) ~= 1 then
				act.SetActScriptData(99, 1, 14, 1)
				act.SaveActScriptData(99)
				return 1,1351208
			else
			    act.SaveActScriptData(99)
				return 1,1351209
			end
		else
		    return 1,1351211
		end
	else
	    local q = math.random(1,50)
		local p = 2420004
		if q <= 35 then
		    q = 1
		elseif q <= 45 then
		    q = 2
		else
		    q = 3
		end
	    if level <= 40 then
		    p = 2420004
		elseif level <= 50 then
		    p = 2420005
        elseif level <= 60 then
		    p = 2420006
        elseif level <= 70 then
		    p = 2420007
        elseif level <= 80 then
		    p = 2420008
        elseif level <= 90 then
		    p = 2420009
        elseif level > 90 then
		    p = 2420010
        end
		return q, p
	end
end

