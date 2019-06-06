--[[�̶��Ⱥ����¹������

--ScriptData�и����ֶ�����˵��
--0:��Ľ�����1- 20Ϊǰ10��С�ӱ�ţ�˳�������ν��д洢��

--����������(����RoleID, KillPlayer, IsDeadLeave, IsInMap)
Act18_RoleData = {} --������

--����������(��������С�ӱ��, Act18_RoleData1, Act18_RoleData2, Act18_RoleData3��Act18_RoleData4��Act18_RoleData5��Act18_RoleData6)
Act18_TeamRoleData = {}

--��е����С������(����С�ӱ��, Score, JoinTime, LeaderID)
Act18_TeamData = {}

--�̶���ѿ�ʼ��ʱ�䣨��λ�Ƿ��ӣ�
Act18_LastTime = 0

--���С�ӱ�����ֵ
Act18_TeamIndex = 1

--���ǰ����
Act18_Expire = 0

--�Ƿ��Ѿ������˸���
IsCreateAct18Instance = 0

--�����
Act18_RewardItem = {1220005, 1230005, 1210003, 1420007, 1420011, 1420015, 1420019, 1420031, 1420035}

--���ˢ���ľ�������
Act18_CrystalNum = 0

--��¼ÿ�����ɱ���ĵ��˵Ĵ���
PKAct18_killrole = {}

--------------------------------------------------------------------------------------------------------
local function RoleRevive(MapID, InstanceID, RoleID, ReviveHP, ReviveMP, fx, fy, fz, RebornMapID, Act_TeamRoleData)
	--�����ҵ�С�ӱ��
	local TeamKey = role.GetRoleScriptData(RoleID, 1, RoleDataType["Act18_TeamKey"])

	if Act_TeamRoleData[TeamKey][RoleID].KillPlayer == 0 then
		fx = 2156
		fy = 18363
		fz = 2420
		RebornMapID = 3017299663

		--�������Ϊ�������뿪
		Act_TeamRoleData[TeamKey][RoleID].IsDeadLeave = true
	else
		--����õ�һ�����������
		local Index = math.random(1, 8)
		fx = m22_RebornPos[Index].x
		fy = m22_RebornPos[Index].y
		fz = m22_RebornPos[Index].z
		RebornMapID = 2983743695

		���������1
		Act_TeamRoleData[TeamKey][RoleID].KillPlayer = Act_TeamRoleData[TeamKey][RoleID].KillPlayer - 1

		--ͬ���ͻ��˸������
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 60,	152+InstanceID)		                --���
		msg.AddMsgEvent(MsgID, 17,	Act_TeamRoleData[TeamKey][RoleID].KillPlayer)	--�������
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	end
	unit.AddBuff(MapID, InstanceID, RoleID, 9999001, RoleID)
	return ReviveHP, ReviveMP, fx, fy, fz, RebornMapID
end

--------------------------------------------------------------------------------------------------------
local function PlayerEnter(MapID, InstanceID, RoleID, Act_Expire, Act_TeamRoleData, Act_TeamData, TeamKey)
    --�������ϴβμӻ�Ľ���
	local RoleExpire = role.GetRoleScriptData(RoleID, 1, RoleDataType["Act18_Expire"])

	if RoleExpire == Act_Expire then
	    --���������������뿪�
		--if Act_TeamRoleData[TeamKey][RoleID] ~= nil then
		    if InstanceID == 1 then
		        --�۳����2000��
			    role.DecRoleSilver(MapID, InstanceID, RoleID, 2000, 1001)
			end

			if InstanceID == 2 then
			    --�۳����5000��
			    role.DecRoleSilver(MapID, InstanceID, RoleID, 5000, 1001)
			end

			if InstanceID == 3 then
			    --�۳����10000��
			    role.DecRoleSilver(MapID, InstanceID, RoleID, 10000, 1001)
			end

			if InstanceID == 4 then
			    --�۳����20000��
			    role.DecRoleSilver(MapID, InstanceID, RoleID, 20000, 1001)
			end

			Act_TeamRoleData[TeamKey][RoleID].IsDeadLeave = false
		--end

		--������û�ж���, ͳ�ƻ��С�ӳ�Ա������
		local TeamID = role.IsRoleHaveTeam(MapID, InstanceID, RoleID)
		local TeamKey = role.GetRoleScriptData(RoleID, 1, RoleDataType["Act18_TeamKey"])
		local TeamNum = 0
		local TeamateInMap = 4294967295
		if TeamID == 4294967295 then
		    for k in pairs(Act_TeamRoleData[TeamKey]) do
		        if Act_TeamRoleData[TeamKey][k].IsInMap then
		            TeamNum = TeamNum + 1
		            TeamateInMap = k
		        end
		    end
		end

		--���ֻ��һ��С�Ӷ���
		if TeamNum == 1 then
		    local TeamateTeamID = role.IsRoleHaveTeam(MapID, InstanceID, TeamateInMap)
		    --����û��С��, �򴴽�һ��С��
		    if TeamateTeamID == 4294967295 then
		        team.CreateTeam(TeamateInMap, RoleID)
		    else   --��С�ӣ�������С��
		        team.AddTeamMember(TeamateTeamID, RoleID)
		    end
		end

		--��Ҽ���ԭ����С��
		if TeamNum > 1 then
		   TeamID = role.IsRoleHaveTeam(MapID, InstanceID, TeamateInMap)
		   team.AddTeamMember(TeamID, RoleID)
		end

		Act_TeamRoleData[TeamKey][RoleID].IsInMap = true
	else
	    --��ҵ�һ�ν���
--		table.insert(Act_TeamRoleData[TeamKey], RoleID, {KillPlayer=3, IsDeadLeave=false, IsInMap=true})

	    --�������ScriptDataΪ��ǰ��Ľ���
	    role.SetRoleScriptData(RoleID, 1, RoleDataType["Act18_Expire"], Act_Expire)

	    if not Act_TeamData[TeamKey] then
--	        table.insert(Act_TeamData, TeamKey, {Score = 0, JoinTime = 0, LeaderID = RoleID})
	        Act_TeamData[TeamKey] = {Score = 0, JoinTime = 0, LeaderID = RoleID}
	        Act_TeamRoleData[TeamKey] = {}
	    end

	    Act_TeamRoleData[TeamKey][RoleID] = {KillPlayer=0, IsDeadLeave=false, IsInMap=true}
		if InstanceID == 1 then
		    if Act18_RoleData[RoleID] == nil then
			    Act18_RoleData[RoleID] = {KillPlayer=0}
			end
		elseif InstanceID == 2 then
			if Act19_RoleData[RoleID] == nil then
			    Act19_RoleData[RoleID] = {KillPlayer=0}
			end
		elseif InstanceID == 3 then
		    if Act20_RoleData[RoleID] == nil then
			    Act20_RoleData[RoleID] = {KillPlayer=0}
			end
		elseif InstanceID == 4 then
			if Act21_RoleData[RoleID] == nil then
			    Act21_RoleData[RoleID] = {KillPlayer=0}
			end
		end
	end
end

--------------------------------------------------------------------------------------------------------
--ͬ���ͻ���С�ӻ���
function SendScore2Teamate(ActGroup, Score, TeamRoleData)
    for k in pairs(TeamRoleData) do
        if TeamRoleData[k].IsInMap then
	        --ͬ���ͻ��˻���
	        local MsgID = msg.BeginMsgEvent()
	        msg.AddMsgEvent(MsgID, 60,	ActGroup)							--���
	        msg.AddMsgEvent(MsgID, 14,	Score)                      		--����
	        msg.DispatchRoleMsgEvent(k, MsgID)
        end
	end
end

--------------------------------------------------------------------------------------------------------
--�洢��ǰ����
function SaveAct18TeamData()
    act.SetActScriptData(18, 1, 0, Act18_Expire)

    local Index = 1
    for k in pairsByKeys(Act18_TeamData, Act18_Cmp) do
        act.SetActScriptData(18, 1, Index, k)
        Index = Index + 1
        if Index > 10 then
            break
        end
	end
end
--------------------------------------------------------------------------------------------------------
--�̶����ʼ
function act18_OnStart(ActID)
	--��ʼ����ѽ���ʱ��
	Act18_LastTime = 0

	--��ʼ��ˮ��������
	Act18_CrystalNum = 0

	Act18_Expire = act.GetActScriptData(18, 1, 0)

	--ͬ�����г������������ʱ��
	for n in pairs(Act18_TeamRoleData) do
		for k in pairs(Act18_TeamRoleData[n]) do
		    if Act18_TeamRoleData[n][k].IsInMap then
		        --���ͻʣ��ʱ��
	            local MsgID = msg.BeginMsgEvent()
	            msg.AddMsgEvent(MsgID, 60,	153)
	            msg.AddMsgEvent(MsgID, 16,	1800)							                 --ʣ��ʱ��
	            msg.DispatchRoleMsgEvent(k, MsgID)


                --�������Ϊ����̨״̬(���ڿͻ��˵����жϣ�
	            role.SetRoleState(RoleID, 4, false)
	        end
	    end
	end

	for k in pairs(m22_SpawnPos) do
		m22_SpawnPos[k].Crystal40 = false
		m22_SpawnPos[k].Crystal60 = false
		m22_SpawnPos[k].Crystal80 = false
		m22_SpawnPos[k].Crystal100 = false
	end
end

function act18_OnTimerMin(ActID)
	--�ۼӻ����ʱ��
    Act18_LastTime = Act18_LastTime + 1

		for k in pairs(Act18_TeamRoleData) do
			for n in pairs(Act18_TeamRoleData[k]) do
				--����ڻ��ͼ��
				if Act18_TeamRoleData[k][n].IsInMap then
					--���û������
					Act18_TeamData[k].Score = Act18_TeamData[k].Score + 6
					--����Ƿ��ڻ�Ծ����
					if map.IsInArea(2983743695, 1, n, 636)	then
						if unit.IsDead(2983743695, 1, n) then
							Act18_TeamData[k].Score = Act18_TeamData[k].Score + 6
						end
					end
				end
			end
			--ͬ���ͻ��˻���
			SendScore2Teamate(153, Act18_TeamData[k].Score, Act18_TeamRoleData[k])
		end
    if Act18_LastTime % 5 == 0 then
	    local total = 0
		local avaliable = {}
		for ii=1,20 do
			if m22_SpawnPos[ii].Crystal40 == false then
				total = total+1
				avaliable[total] = ii
			end
		end
		while Act18_CrystalNum < 5 and total > 0 do
		    --���һ������
			local Index = math.random(1,total)
			Index = avaliable[Index]
		    avaliable[Index] = avaliable[total]
			Act18_CrystalNum = Act18_CrystalNum + 1
			m22_SpawnPos[Index].Crystal40 = true
		    total = total -1
			--����õ�һ��ˮ����ID
			local CrystalBallID = math.random(1520001, 1520007)
			--ˢ��ˮ����
			local CreatureID = map.MapCreateCreature(2983743695, 1, CrystalBallID, m22_SpawnPos[Index].x, m22_SpawnPos[Index].y, m22_SpawnPos[Index].z)
			cre.SetCreatureScriptData(2983743695, 1, CreatureID, 1, 0, Index)
		end
	end

	--�ۼ���Ҳμӻ��ʱ��
	for k in pairs(Act18_TeamData) do
	    Act18_TeamData[k].JoinTime = Act18_TeamData[k].JoinTime + 1
	    if (Act18_TeamData[k].JoinTime % 10) == 0 then
		    for n in pairs(Act18_TeamRoleData[k]) do
		        if Act18_TeamRoleData[k][n].IsInMap and false == unit.IsDead(2983743695, 1, n)then
			        --����2000��
			        role.AddRoleSilver(2983743695, 1, n, 2000, 1001)
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 101, 2853)   --���������ۼ�ʱ��ﵽ10�ı�������ö���Ľ�Ǯ����
					msg.DispatchRoleMsgEvent(n, MsgID)
			    end
			end
		end
	end
end

--�����
function act18_OnEnd(actID)
	--ɾ�������
	IsCreateAct18Instance = 0
	SaveAct18TeamData()
    PKAct18_killrole = {}
    Act18_RoleData = {}
	local rank ={}
	for i=1,10 do
	    rank[act.GetActScriptData(18, 1, i)] = i
	end

	--�����ȡһ����������д������
	for k in pairs(Act18_TeamData) do
	    --�����ȡһ����������д������
		for n in pairs(Act18_TeamRoleData[k]) do
			if Act18_TeamRoleData[k][n].IsInMap then
				local AddExp = role.GetRoleLevelUpExp(2983743695, 1, n)
				local rolelevel = role.GetRoleLevel(2983743695, 1, n)
				local Scoreact = Act18_TeamData[k].Score
				if Scoreact >3000 then
					Scoreact = 3000
				end
				local exp = AddExp * Scoreact / 3000 * 130 / (rolelevel - 25) / (rolelevel - 25)
				role.AddRoleExp(2983743695, 1, n, exp)
				local wushiyin = 0
				if Act18_TeamData[k].Score >= 2000 then
					wushiyin = 30
				elseif Act18_TeamData[k].Score >= 1000 then
					wushiyin = 20
				elseif Act18_TeamData[k].Score >= 500 then
					wushiyin = 15
				elseif Act18_TeamData[k].Score >= 250 then
					wushiyin = 10
				elseif Act18_TeamData[k].Score >= 150 then
					wushiyin = 6
				elseif Act18_TeamData[k].Score >= 100 then
					wushiyin = 4
				elseif Act18_TeamData[k].Score >= 50 then
					wushiyin = 2
				else
					wushiyin = 1
				end
				if rank[k] ~= nil then
					if rank[k] == 1 then
						--role.AddRoleItem(2983743695, 1, n, 2612621, 1, -1, 13, 1001)
						--role.AddRoleItem(2983743695, 1, n, 2612629, 10, -1, 13, 1001)
						role.AddRoleItem(2983743695, 1, n, 2616345, wushiyin, -1, 13, 1001)
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 71, 2857)  --����Ⱥ����¹��л�õ�һ�����õ�����<color=0xffff0000>XX<color=0xfffff7e0>���<color=0xffff0000>X<color=0xfffff7e0>��<color=0xff00a2ff>��ʿӡ<color=0xfffff7e0>������������¹Ӣ�۰���ȡר���ƺź�<color=0xffff0000>10<color=0xfffff7e0>��<color=0xff00a2ff>Ӣ����<color=0xfffff7e0>����<color=0xffff0000>XX<color=0xfffff7e0>�㣡
						msg.AddMsgEvent(MsgID, 9, exp)
						msg.AddMsgEvent(MsgID, 9, wushiyin)
						msg.DispatchRoleMsgEvent(n, MsgID)
					elseif rank[k] <= 10 then
						local itemnum = 1
						if rank[k] == 2 or rank[k] == 3 then
							itemnum = 8
						elseif rank[k] == 4 or rank[k] == 5 or rank[k] == 6 then
							itemnum = 4
						elseif rank[k] == 7 or rank[k] == 8 or rank[k] == 9 or rank[k] == 10 then
							itemnum = 2
						end
					   --role.AddRoleItem(2983743695, 1, n, 2612629, itemnum, -1, 13, 1001)
						role.AddRoleItem(2983743695, 1, n, 2616345, wushiyin, -1, 13, 1001)
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 71, 2858)  --��ϲ����Ⱥ����¹��л�õ�X�����õ�����<color=0xffff0000>XX<color=0xfffff7e0>���<color=0xffff0000>X<color=0xfffff7e0>��<color=0xff00a2ff>��ʿӡ<color=0xfffff7e0>������������¹Ӣ�۰���ȡ<color=0xffff0000>X<color=0xfffff7e0>��<color=0xff00a2ff>Ӣ����<color=0xfffff7e0>��
						msg.AddMsgEvent(MsgID, 9, rank[k])
						msg.AddMsgEvent(MsgID, 9, exp)
						msg.AddMsgEvent(MsgID, 9, wushiyin)
						msg.AddMsgEvent(MsgID, 9, itemnum)
						msg.DispatchRoleMsgEvent(n, MsgID)
					end
				else
					role.AddRoleItem(2983743695, 1, n, 2616345, wushiyin, -1, 13, 1001)
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 71, 2859)  --����Ⱥ����¹��л�þ���<color=0xffff0000>XX<color=0xfffff7e0>���<color=0xffff0000>X<color=0xfffff7e0>��<color=0xff00a2ff>��ʿӡ<color=0xfffff7e0>ϣ�����ٽ�������
					msg.AddMsgEvent(MsgID, 9, exp)
					msg.AddMsgEvent(MsgID, 9, wushiyin)
					msg.DispatchRoleMsgEvent(n, MsgID)
				end
				�����ȡһ����Ʒ��Ϊ����
				if 1 <= role.GetBagFreeSize(n) then
					role.AddRoleItem(2983743695, 1, n, 2612621, 1, -1, 13, 1001)

					--��ʾ���"������Ӣ�����������###������ϣ���´��ٽ�������"
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 71, 164)
					msg.AddMsgEvent(MsgID, 4, Act18_RewardItem[Index])
					msg.DispatchRoleMsgEvent(n, MsgID)
				end
			end
		end
	end
	map.DeleteInstance(2983743695, 1)
end

--��й㲥(State = 0-������ʼ 1-��ʼ 2-�������� 3-���� )
function act18_ActivityBroad(State)
    if PVPactivity_trigger ~= 1 then
	    return
	end
    --�������ʼ
    if State == 0 then
        --֪ͨȫ���������ʼ
	    local MsgID = msg.BeginMsgEvent()
	    msg.AddMsgEvent(MsgID, 100, 165)
	    msg.DispatchWorldMsgEvent(MsgID)
    end
end

--ע��û������¼�
aux.RegisterActEvent(18, 2, "act18_OnStart") 	--������
aux.RegisterActEvent(18, 4, "act18_OnTimerMin") --������
aux.RegisterActEvent(18, 3, "act18_OnEnd") 		--������
aux.RegisterActEvent(18, 6, "act18_ActivityBroad")
aux.RegisterActEvent(103, 2, "act18_OnStart") 	--������
aux.RegisterActEvent(103, 4, "act18_OnTimerMin") --������
aux.RegisterActEvent(103, 3, "act18_OnEnd") 		--������
aux.RegisterActEvent(103, 6, "act18_ActivityBroad")
--------------------------------------------------------------------------------------------------------
--todo��ҽ�������
function m22_PlayerEnter(MapID, InstanceID, RoleID)
    local Min  		= tonumber(os.date("%M"))
	local Second	= tonumber(os.date("%S"))
	local leftSec 	= (29 - Min) * 60 + 59 - Second		--�ʣ��ʱ��
	--�������ΪPK״̬(���ڿͻ��˵����жϣ�
    role.SetRoleState(RoleID, 8, false)
    --�õ����С�ӱ��
	local TeamKey = role.GetRoleScriptData(RoleID, 1, RoleDataType["Act18_TeamKey"])

	--������
	if InstanceID == 1 then
        PlayerEnter(MapID, InstanceID, RoleID, Act18_Expire, Act18_TeamRoleData, Act18_TeamData, TeamKey)
	    --���ͻ������ͻ���
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 60,	153)								                --������
		msg.AddMsgEvent(MsgID, 25,	18)									                --�ID
		msg.AddMsgEvent(MsgID, 14,	Act18_TeamData[TeamKey].Score)		                --����
		msg.AddMsgEvent(MsgID, 17,	Act18_TeamRoleData[TeamKey][RoleID].KillPlayer)     --�������
		msg.AddMsgEvent(MsgID, 18,	TeamKey)                                            --������
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	end

    --������
    if InstanceID == 2 then
        PlayerEnter(MapID, InstanceID, RoleID, Act19_Expire, Act19_TeamRoleData, Act19_TeamData, TeamKey)
	    --���ͻ������ͻ���
        local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 60,	154)								                --������
		msg.AddMsgEvent(MsgID, 25,	19)									                --�ID
		msg.AddMsgEvent(MsgID, 14,	Act19_TeamData[TeamKey].Score)		                --����
		msg.AddMsgEvent(MsgID, 17,	Act19_TeamRoleData[TeamKey][RoleID].KillPlayer)     --�������
		msg.AddMsgEvent(MsgID, 18,	TeamKey)                                            --������
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
    end

    --Ӣ����
    if InstanceID == 3 then
        PlayerEnter(MapID, InstanceID, RoleID, Act20_Expire, Act20_TeamRoleData, Act20_TeamData, TeamKey)
	    --���ͻ������ͻ���
        local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 60,	155)								                --Ӣ����
		msg.AddMsgEvent(MsgID, 25,	20)									                --�ID
		msg.AddMsgEvent(MsgID, 14,	Act20_TeamData[TeamKey].Score)		                --����
		msg.AddMsgEvent(MsgID, 17,	Act20_TeamRoleData[TeamKey][RoleID].KillPlayer)     --�������
		msg.AddMsgEvent(MsgID, 18,	TeamKey)                                            --������
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
    end

    --������
    if InstanceID == 4 then
        PlayerEnter(MapID, InstanceID, RoleID, Act21_Expire, Act21_TeamRoleData, Act21_TeamData, TeamKey)
	    --���ͻ������ͻ���
	    local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 60,	156)								                --������
		msg.AddMsgEvent(MsgID, 25,	21)									                --�ID
		msg.AddMsgEvent(MsgID, 14,	Act21_TeamData[TeamKey].Score)		                --����
		msg.AddMsgEvent(MsgID, 17,	Act21_TeamRoleData[TeamKey][RoleID].KillPlayer)     --�������
		msg.AddMsgEvent(MsgID, 18,	TeamKey)                                            --������
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
    end

    --�жϻ�Ƿ��Ѿ���ʼ
	local bStart = act.GetActIsStart(18)
	if bStart then
        --���ͻʣ��ʱ��
	    local MsgID = msg.BeginMsgEvent()
	    msg.AddMsgEvent(MsgID, 60,	153)								                 --������
	    msg.AddMsgEvent(MsgID, 16,	leftSec)							                 --ʣ��ʱ��
	    msg.DispatchRoleMsgEvent(RoleID, MsgID)


        --�������Ϊ����̨״̬(���ڿͻ��˵����жϣ�
	    role.SetRoleState(RoleID, 8, true)
	end
end

--ע�������ͼ�¼�
aux.RegisterMapEvent("m22", 2, "m22_PlayerEnter")

-------------------------------------------------------------------------------------------------------
--����뿪�����
function m22_PlayerLeave(MapID, InstanceID, RoleID)
	--�����ҵ�С�ӱ��
	local TeamKey = role.GetRoleScriptData(RoleID, 1, RoleDataType["Act18_TeamKey"])

	--��������뿪����̨״̬(���ڿͻ��˵����жϣ�
	role.UnSetRoleState(RoleID, 8, true)
	--ͬ���ͻ��˹رջ����
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 60, -1)								--�رջ����
	msg.DispatchRoleMsgEvent(RoleID, MsgID)

	--������
	if InstanceID == 1 then
		Act18_TeamRoleData[TeamKey][RoleID].IsInMap = false
	end

	--������
	if InstanceID == 2 then
		Act19_TeamRoleData[TeamKey][RoleID].IsInMap = false
	end

	--Ӣ����
	if InstanceID == 3 then
		Act20_TeamRoleData[TeamKey][RoleID].IsInMap = false
	end

	--������
	if InstanceID == 4 then
		Act21_TeamRoleData[TeamKey][RoleID].IsInMap = false
	end
end

--ע���뿪���ͼ�¼�
aux.RegisterMapEvent("m22", 3, "m22_PlayerLeave")

-------------------------------------------------------------------------------------------------------
--����ڻ�и���
function m22_RoleRevive(MapID, InstanceID, RoleID, ReviveType, ReviveHP, ReviveMP, fx, fy, fz, RebornMapID)
	--���ѡ��سǸ���
	if ReviveType == 0 then
		ReviveHP = role.GetRoleAttValue(MapID, InstanceID, RoleID, 8)
		ReviveMP = role.GetRoleAttValue(MapID, InstanceID, RoleID, 10)

		--���Ĭ����3�λ������ᡣ���ѡ��سǸ������1�λ������ᣬ������������ڵ�1����������и��
		if InstanceID == 1 then
            return RoleRevive(MapID, InstanceID, RoleID, ReviveHP, ReviveMP, fx, fy, fz, RebornMapID, Act18_TeamRoleData)
		end

		--������
		if InstanceID == 2 then
		    return RoleRevive(MapID, InstanceID, RoleID, ReviveHP, ReviveMP, fx, fy, fz, RebornMapID, Act19_TeamRoleData)
		end

		--Ӣ����
		if InstanceID == 3 then
			return RoleRevive(MapID, InstanceID, RoleID, ReviveHP, ReviveMP, fx, fy, fz, RebornMapID, Act20_TeamRoleData)
		end

		--������
		if InstanceID == 4 then
			return RoleRevive(MapID, InstanceID, RoleID, ReviveHP, ReviveMP, fx, fy, fz, RebornMapID, Act21_TeamRoleData)
		end
	end

	return ReviveHP, ReviveMP, fx, fy, fz, RebornMapID
end

aux.RegisterMapEvent("m22",  11, "m22_RoleRevive")

-------------------------------------------------------------------------------------------------------
--����������todo
function m22_RoleDie(MapID, InstanceID, RoleID, KillerID)
	if unit.IsPlayer(KillerID) then
	    --�����ҵ�С�ӱ��
		local KillerTeamKey = role.GetRoleScriptData(KillerID, 1, RoleDataType["Act18_TeamKey"])
		local RoleTeamKey = role.GetRoleScriptData(RoleID, 1, RoleDataType["Act18_TeamKey"])

		--��ɱһ����ң���û������ӣ�������=10��[1-������ȼ�-Ŀ��ȼ���/10]����Сȡ1
		local RoleLevel = role.GetRoleLevel(MapID, InstanceID, RoleID)
		if RoleLevel < 40 then             --��ֹ�ȼ�������ʱ����
		    RoleLevel = 40
		elseif RoleLevel > 100 then
		    RoleLevel = 100
		end
		local KillerLevel = role.GetRoleLevel(MapID, InstanceID, KillerID)
		local Score = 30*(1-(KillerLevel - RoleLevel)/10)
		--local PunishScore = 1
		if Score < 1 then
			Score = 1
		end

		--������
		if InstanceID == 1 then
			���15����
		    if Act18_LastTime > 15 then
		    	--��ɱ��������ǰ10������ң���û�����������ͨ״̬��2��
		    	local Index = 0
		    	for k in pairsByKeys(Act18_TeamData, Act18_Cmp) do
		    		if k == RoleTeamKey then
		    			Score = Score*2
		    			--PunishScore = 5
		    			break
		    		end

		    		if Index > 10 then
		    			break
		    		end

	    			Index = Index + 1
	    		end
	    	end
			--���ӻ�ɱ��
			Act18_RoleData[KillerID].KillPlayer = Act18_RoleData[KillerID].KillPlayer + 1
			if Act18_RoleData[KillerID].KillPlayer % 10 == 0 and Act18_RoleData[KillerID].KillPlayer ~= 0 then
				--֪ͨȫ���������ʼ
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 102, 100108)    --��ս����XXX�²��ɵ������X��ն��
				msg.AddMsgEvent(MsgID, 2, KillerID)
				msg.AddMsgEvent(MsgID, 9, Act18_RoleData[KillerID].KillPlayer)
				msg.DispatchBroadcast(MsgID, MapID, InstanceID,-1)
			end
			Act18_TeamRoleData[KillerTeamKey][KillerID].KillPlayer = Act18_TeamRoleData[KillerTeamKey][KillerID].KillPlayer + 1
			--�����ɱ���齱��
			local killexp = PKAct_killexp[RoleLevel]
            if PKAct18_killrole[KillerID] == nil then
			    PKAct18_killrole[KillerID] = {}
				if PKAct18_killrole[KillerID][RoleID] == nil then
				    PKAct18_killrole[KillerID][RoleID] = 0
				else
				    PKAct18_killrole[KillerID][RoleID] = PKAct18_killrole[KillerID][RoleID] + 1
				end
			else
			    if PKAct18_killrole[KillerID][RoleID] == nil then
				    PKAct18_killrole[KillerID][RoleID] = 0
				else
				    PKAct18_killrole[KillerID][RoleID] = PKAct18_killrole[KillerID][RoleID] + 1
				end
			end
			if PKAct18_killrole[KillerID][RoleID] < 10 then
			    role.AddRoleExp(2983743695, 1, KillerID, math.floor(killexp*(10-PKAct18_killrole[KillerID][RoleID])/10))
			    Score = math.floor(Score*(10-PKAct18_killrole[KillerID][RoleID])/10)
			else
			    Score = 0
			end
			--���ӻ�ɱ�߻���todo
			Act18_TeamData[KillerTeamKey].Score = Act18_TeamData[KillerTeamKey].Score + Score
			SendScore2Teamate(153, Act18_TeamData[KillerTeamKey].Score, Act18_TeamRoleData[KillerTeamKey])

			if PKAct18_killrole[KillerID][RoleID] < 10 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 101, 2860)           --��ɱXXX����þ���XXX�㣬����XXX��
				msg.AddMsgEvent(MsgID, 2, RoleID)
				msg.AddMsgEvent(MsgID, 9, math.floor(killexp*(10-PKAct18_killrole[KillerID][RoleID])/10))
				msg.AddMsgEvent(MsgID, 9, Score)
				msg.DispatchRoleMsgEvent(KillerID, MsgID)
			end

			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 60,	153)								                --������
			msg.AddMsgEvent(MsgID, 17,	Act18_RoleData[KillerID].KillPlayer)     --ɱ����
			msg.DispatchRoleMsgEvent(KillerID, MsgID)
			--���������߻���
			--Act18_TeamData[RoleTeamKey].Score = Act18_TeamData[RoleTeamKey].Score - PunishScore
			--SendScore2Teamate(153, Act18_TeamData[RoleTeamKey].Score, Act18_TeamRoleData[RoleTeamKey])
		end

		--������
		if InstanceID == 2 then
		    ���15����
		    if Act19_LastTime > 15 then
		    	--��ɱ��������ǰ10������ң���û�����������ͨ״̬��2��
		    	local Index = 0
		    	for k in pairsByKeys(Act19_TeamData, Act19_Cmp) do
		    		if k == RoleTeamKey then
		    			Score = Score*2
		    			--PunishScore = 5
		    			break
		    		end

		    		if Index > 10 then
		    			break
		    		end

	    			Index = Index + 1
	    		end
	    	end
			--���ӻ�ɱ��
			Act19_RoleData[KillerID].KillPlayer = Act19_RoleData[KillerID].KillPlayer + 1
			if Act19_RoleData[KillerID].KillPlayer % 10 == 0 and Act19_RoleData[KillerID].KillPlayer ~= 0 then
				--֪ͨȫ���������ʼ
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 102, 100108)    --��ս����XXX�²��ɵ������X��ն��
				msg.AddMsgEvent(MsgID, 2, KillerID)
				msg.AddMsgEvent(MsgID, 9, Act19_RoleData[KillerID].KillPlayer)
				msg.DispatchBroadcast(MsgID, MapID, InstanceID,-1)
			end
			Act19_TeamRoleData[KillerTeamKey][KillerID].KillPlayer = Act19_TeamRoleData[KillerTeamKey][KillerID].KillPlayer + 1
			--�����ɱ���齱��
			local killexp = PKAct_killexp[RoleLevel]
            if PKAct19_killrole[KillerID] == nil then
			    PKAct19_killrole[KillerID] = {}
				if PKAct19_killrole[KillerID][RoleID] == nil then
				    PKAct19_killrole[KillerID][RoleID] = 0
				else
				    PKAct19_killrole[KillerID][RoleID] = PKAct19_killrole[KillerID][RoleID] + 1
				end
			else
			    if PKAct19_killrole[KillerID][RoleID] == nil then
				    PKAct19_killrole[KillerID][RoleID] = 0
				else
				    PKAct19_killrole[KillerID][RoleID] = PKAct19_killrole[KillerID][RoleID] + 1
				end
			end
			if PKAct19_killrole[KillerID][RoleID] < 10 then
			    role.AddRoleExp(2983743695, 2, KillerID, math.floor(killexp*(10-PKAct19_killrole[KillerID][RoleID])/10))
			    Score = math.floor(Score*(10-PKAct19_killrole[KillerID][RoleID])/10)
			else
			    Score = 0
			end
			--���ӻ�ɱ�߻���todo
			Act19_TeamData[KillerTeamKey].Score = Act19_TeamData[KillerTeamKey].Score + Score
			SendScore2Teamate(154, Act19_TeamData[KillerTeamKey].Score, Act19_TeamRoleData[KillerTeamKey])

			if PKAct19_killrole[KillerID][RoleID] < 10 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 101, 2860)           --��ɱXXX����þ���XXX�㣬����XXX��
				msg.AddMsgEvent(MsgID, 2, RoleID)
				msg.AddMsgEvent(MsgID, 9, math.floor(killexp*(10-PKAct19_killrole[KillerID][RoleID])/10))
				msg.AddMsgEvent(MsgID, 9, Score)
				msg.DispatchRoleMsgEvent(KillerID, MsgID)
			end

			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 60,	154)								                --������
			msg.AddMsgEvent(MsgID, 17,	Act19_RoleData[KillerID].KillPlayer)     --ɱ����
			msg.DispatchRoleMsgEvent(KillerID, MsgID)
		end

		--Ӣ����
		if InstanceID == 3 then
		    ���15����
		    if Act19_LastTime > 15 then
		    	--��ɱ��������ǰ10������ң���û�����������ͨ״̬��2��
		    	local Index = 0
		    	for k in pairsByKeys(Act19_TeamData, Act19_Cmp) do
		    		if k == RoleTeamKey then
		    			Score = Score*2
		    			--PunishScore = 5
		    			break
		    		end

		    		if Index > 10 then
		    			break
		    		end

	    			Index = Index + 1
	    		end
	    	end
			--���ӻ�ɱ��
			Act20_RoleData[KillerID].KillPlayer = Act20_RoleData[KillerID].KillPlayer + 1
			if Act20_RoleData[KillerID].KillPlayer % 10 == 0 and Act20_RoleData[KillerID].KillPlayer ~= 0 then
				--֪ͨȫ���������ʼ
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 102, 100108)    --��ս����XXX�²��ɵ������X��ն��
				msg.AddMsgEvent(MsgID, 2, KillerID)
				msg.AddMsgEvent(MsgID, 9, Act20_RoleData[KillerID].KillPlayer)
				msg.DispatchBroadcast(MsgID, MapID, InstanceID,-1)
			end
			Act20_TeamRoleData[KillerTeamKey][KillerID].KillPlayer = Act20_TeamRoleData[KillerTeamKey][KillerID].KillPlayer + 1
			--�����ɱ���齱��
			local killexp = PKAct_killexp[RoleLevel]
            if PKAct20_killrole[KillerID] == nil then
			    PKAct20_killrole[KillerID] = {}
				if PKAct20_killrole[KillerID][RoleID] == nil then
				    PKAct20_killrole[KillerID][RoleID] = 0
				else
				    PKAct20_killrole[KillerID][RoleID] = PKAct20_killrole[KillerID][RoleID] + 1
				end
			else
			    if PKAct20_killrole[KillerID][RoleID] == nil then
				    PKAct20_killrole[KillerID][RoleID] = 0
				else
				    PKAct20_killrole[KillerID][RoleID] = PKAct20_killrole[KillerID][RoleID] + 1
				end
			end
			if PKAct20_killrole[KillerID][RoleID] < 10 then
			    role.AddRoleExp(2983743695, 3, KillerID, math.floor(killexp*(10-PKAct20_killrole[KillerID][RoleID])/10))
			    Score = math.floor(Score*(10-PKAct20_killrole[KillerID][RoleID])/10)
			else
			    Score = 0
			end
			--���ӻ�ɱ�߻���todo
			Act20_TeamData[KillerTeamKey].Score = Act20_TeamData[KillerTeamKey].Score + Score
			SendScore2Teamate(155, Act20_TeamData[KillerTeamKey].Score, Act20_TeamRoleData[KillerTeamKey])

			if PKAct20_killrole[KillerID][RoleID] < 10 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 101, 2860)           --��ɱXXX����þ���XXX�㣬����XXX��
				msg.AddMsgEvent(MsgID, 2, RoleID)
				msg.AddMsgEvent(MsgID, 9, math.floor(killexp*(10-PKAct20_killrole[KillerID][RoleID])/10))
				msg.AddMsgEvent(MsgID, 9, Score)
				msg.DispatchRoleMsgEvent(KillerID, MsgID)
			end

			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 60,	155)								                --������
			msg.AddMsgEvent(MsgID, 17,	Act20_RoleData[KillerID].KillPlayer)     --ɱ����
			msg.DispatchRoleMsgEvent(KillerID, MsgID)
		end

		--������
		if InstanceID == 4 then
		    ���15����
		    if Act21_LastTime > 15 then
		    	--��ɱ��������ǰ10������ң���û�����������ͨ״̬��2��
		    	local Index = 0
		    	for k in pairsByKeys(Act21_TeamData, Act21_Cmp) do
		    		if k == RoleTeamKey then
		    			Score = Score*2
		    			--PunishScore = 5
		    			break
		    		end

		    		if Index > 10 then
		    			break
		    		end

	    			Index = Index + 1
	    		end
	    	end
			--���ӻ�ɱ��
			Act21_RoleData[KillerID].KillPlayer = Act21_RoleData[KillerID].KillPlayer + 1
			if Act21_RoleData[KillerID].KillPlayer % 10 == 0 and Act21_RoleData[KillerID].KillPlayer ~= 0 then
				--֪ͨȫ���������ʼ
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 102, 100108)    --��ս����XXX�²��ɵ������X��ն��
				msg.AddMsgEvent(MsgID, 2, KillerID)
				msg.AddMsgEvent(MsgID, 9, Act21_RoleData[KillerID].KillPlayer)
				msg.DispatchBroadcast(MsgID, MapID, InstanceID,-1)
			end
			Act21_TeamRoleData[KillerTeamKey][KillerID].KillPlayer = Act21_TeamRoleData[KillerTeamKey][KillerID].KillPlayer + 1
			--�����ɱ���齱��
			local killexp = PKAct_killexp[RoleLevel]
            if PKAct21_killrole[KillerID] == nil then
			    PKAct21_killrole[KillerID] = {}
				if PKAct21_killrole[KillerID][RoleID] == nil then
				    PKAct21_killrole[KillerID][RoleID] = 0
				else
				    PKAct21_killrole[KillerID][RoleID] = PKAct21_killrole[KillerID][RoleID] + 1
				end
			else
			    if PKAct21_killrole[KillerID][RoleID] == nil then
				    PKAct21_killrole[KillerID][RoleID] = 0
				else
				    PKAct21_killrole[KillerID][RoleID] = PKAct21_killrole[KillerID][RoleID] + 1
				end
			end
			if PKAct21_killrole[KillerID][RoleID] < 10 then
			    role.AddRoleExp(2983743695, 4, KillerID, math.floor(killexp*(10-PKAct21_killrole[KillerID][RoleID])/10))
			    Score = math.floor(Score*(10-PKAct21_killrole[KillerID][RoleID])/10)
			else
			    Score = 0
			end
			--���ӻ�ɱ�߻���todo
			Act21_TeamData[KillerTeamKey].Score = Act21_TeamData[KillerTeamKey].Score + Score
			SendScore2Teamate(156, Act21_TeamData[KillerTeamKey].Score, Act21_TeamRoleData[KillerTeamKey])

			if PKAct21_killrole[KillerID][RoleID] < 10 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 101, 2860)           --��ɱXXX����þ���XXX�㣬����XXX��
				msg.AddMsgEvent(MsgID, 2, RoleID)
				msg.AddMsgEvent(MsgID, 9, math.floor(killexp*(10-PKAct21_killrole[KillerID][RoleID])/10))
				msg.AddMsgEvent(MsgID, 9, Score)
				msg.DispatchRoleMsgEvent(KillerID, MsgID)
			end

			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 60,	156)								                --������
			msg.AddMsgEvent(MsgID, 17,	Act21_RoleData[KillerID].KillPlayer)     --ɱ����
			msg.DispatchRoleMsgEvent(KillerID, MsgID)
		end
	end
end

aux.RegisterMapEvent("m22", 5, "m22_RoleDie")

-------------------------------------------------------------------------------------------------------
--����ȽϺ���
function Act18_Cmp(a,b)
    if Act18_TeamData[b].Score < Act18_TeamData[a].Score then
		return true
	elseif Act18_TeamData[b].Score == Act18_TeamData[a].Score and Act18_TeamData[b].JoinTime < Act18_TeamData[a].JoinTime then
		return true
	else
		return false
    end
end

-------------------------------------------------------------------------------------------------------
--����ڻ�е��ߺ�����
function m22_CanTakeOverWhenOnline(MapID, RoleID)
	--������ҵ����ߵ�ͼ������todo
	return 3017299663, 2156, 18363, 2420
end

aux.RegisterMapEvent("m22",  12, "m22_CanTakeOverWhenOnline")

-------------------------------------------------------------------------------------------------------
--�ҵ���ͼ�ĳ��ں�����
function m22_GetExporMapAndCoord(MapID, InstanceID, RoleID)
	return 1, 3017299663, 2156, 18363, 2420
end

aux.RegisterMapEvent("m22",  13, "m22_GetExporMapAndCoord")

-------------------------------------------------------------------------------------------------------
--�ҵ�����ʵĸ���ʵ��
function m22_GetOnePerfectMap(MapID, InstanceID, RoleID)
	local RoleLevel = role.GetRoleLevel(MapID, InstanceID, RoleID)
    if Act18_RoleData[RoleID] ~= nil then
	    return 1
	end

	if Act19_RoleData[RoleID] ~= nil then
	    return 2
	end

	if Act20_RoleData[RoleID] ~= nil then
	    return 3
	end

	if Act21_RoleData[RoleID] ~= nil then
	    return 4
	end
	--���͵����ͳ���
	if RoleLevel >= 40 and RoleLevel < 60 then
		return 1
	end

	--���͵����ܳ���
	if RoleLevel >= 60 and RoleLevel < 80 then
		return 2
	end

	--���͵�Ӣ�۳���
	if RoleLevel >= 80 and RoleLevel < 100 then
		return 3
	end

	--���͵����³���
	if RoleLevel >= 100 then
		return 4
	end

	return 0
end

aux.RegisterMapEvent("m22",  14, "m22_GetOnePerfectMap")

-------------------------------------------------------------------------------------------------------
--�������ǹ��ܹ���������
function m22_CanEnter(MapID, InstanceID, RoleID)
	local RoleLevel = role.GetRoleLevel(MapID, InstanceID, RoleID)
	local TeamKey = role.GetRoleScriptData(RoleID, 1, RoleDataType["Act18_TeamKey"])
	--��ҵȼ�С��40��
	if RoleLevel < 40 then
		return 0
	end

	--���������������뿪���ж���ҵ�ǰ��Ǯ�Ƿ��㹻��
	local MoneyNum = role.GetRoleSilver(MapID, InstanceID, RoleID)
	--if RoleLevel < 60 then
		if Act18_TeamRoleData[TeamKey] and Act18_TeamRoleData[TeamKey][RoleID] and MoneyNum < 2000 then
			--��ʾ��ҽ�Ǯ����
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20,	158)
			msg.AddMsgEvent(MsgID, 21,	5)			    --ȡ��
		    msg.AddMsgEvent(MsgID, 1,	22)
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
			return 0
		end
	--elseif RoleLevel < 80 then
		if Act19_TeamRoleData[TeamKey] and Act19_TeamRoleData[TeamKey][RoleID] and MoneyNum < 5000 then
			--��ʾ��ҽ�Ǯ����
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20,	158)
			msg.AddMsgEvent(MsgID, 21,	5)			    --ȡ��
		    msg.AddMsgEvent(MsgID, 1,	22)
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
			return 0
		end
	--elseif RoleLevel < 100 then
		if Act19_TeamRoleData[TeamKey] and Act19_TeamRoleData[TeamKey][RoleID] and MoneyNum < 10000 then
			--��ʾ��ҽ�Ǯ����
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20,	158)
			msg.AddMsgEvent(MsgID, 21,	5)			    --ȡ��
		    msg.AddMsgEvent(MsgID, 1,	22)
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
			return 0
		end
	--else
		if Act19_TeamRoleData[TeamKey] and Act19_TeamRoleData[TeamKey][RoleID] and MoneyNum < 20000 then
			--��ʾ��ҽ�Ǯ����
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20,	158)
			msg.AddMsgEvent(MsgID, 21,	5)			    --ȡ��
		    msg.AddMsgEvent(MsgID, 1,	22)
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
			return 0
		end
	--end

	return 1
end

aux.RegisterMapEvent("m22",  15, "m22_CanEnter")

-------------------------------------------------------------------------------------------------------
--�ͻ�����ͬ������������
function Act18_DefaultRequest(ActID, RoleID, EventType)
	local Index = 1
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 50, 1)

	for k in pairsByKeys(Act18_TeamData, Act18_Cmp) do
		msg.AddMsgEvent(MsgID, 15, Index)
		msg.AddMsgEvent(MsgID, 2, Act18_TeamData[k].LeaderID)
		msg.AddMsgEvent(MsgID, 14, Act18_TeamData[k].Score)
		msg.AddMsgEvent(MsgID, 18, k)

		Index = Index + 1
		if Index > 10 then
			break
		end
	end
	msg.DispatchRoleMsgEvent(RoleID, MsgID)
end

aux.RegisterActEvent(18, 5, "Act18_DefaultRequest")
aux.RegisterActEvent(103, 5, "Act18_DefaultRequest")

-------------------------------------------------------------------------------------------------------
--��еĵ����ж�
function m22_FriendEnemy(MapID, InstanceID, RoleID, TargetID)
	-- ��ȷ����Ƿ�ʼ
	local bActIsStart = act.GetActIsStart(18)
    local bActIsStart1 = act.GetActIsStart(103)
	-- �û�п�ʼ�޷�pk
	if bActIsStart ~= true and bActIsStart1 ~= true then
		return false, false, false, true
	end

	if unit.IsPlayer(RoleID) and unit.IsPlayer(TargetID) then
	    local RoleTeamID = role.IsRoleHaveTeam(MapID, InstanceID, RoleID)
	    local TargetTeamID = role.IsRoleHaveTeam(MapID, InstanceID, TargetID)
		if RoleTeamID ~= 4294967295 and RoleTeamID ~= TargetTeamID then
		    return false, true, false, true
		elseif RoleTeamID == 4294967295 then
		    return false, true, false, true
		else
		    return false, false, false, false
		end
	else
		return false, false, false, false
	end
end

aux.RegisterMapEvent("m22", 16, "m22_FriendEnemy")

-------------------------------------------------------------------------------------------------------
--����޷���������������
function m22_CanInviteJoinTeam(MapID, InstanceID)
    return 9
end

aux.RegisterMapEvent("m22", 8, "m22_CanInviteJoinTeam")

-------------------------------------------------------------------------------------------------------
--�������޷��뿪С��
function m22_CanLeaveTeam(MapID, InstanceID)
    return 10
end

aux.RegisterMapEvent("m22", 9, "m22_CanLeaveTeam")

-------------------------------------------------------------------------------------------------------
--����޷��ƽ��ӳ�
function m22_CanChangeLeader(MapID, InstanceID)
    return 11
end

aux.RegisterMapEvent("m22", 10, "m22_CanChangeLeader")

-------------------------------------------------------------------------------------------------------
--���С���޷�����
function m22_CanKickMember(MapID, InstanceID)
    return 12
end
aux.RegisterMapEvent("m22", 17, "m22_CanKickMember")]]
