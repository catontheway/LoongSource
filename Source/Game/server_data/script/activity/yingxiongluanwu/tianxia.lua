--[[�̶��Ӣ������������

--ScriptData�и����ֶ�����˵��
--1 - 20Ϊǰ20����ҵ�ID��˳�������ν��д洢

--����������((����RoleID, RebornTime, Score��IsDeadLeave, IsInMap, JoinTime)
Act17_RoleData = {} --������

--�̶���Ѿ���ʼ��ʱ��(��λ�Ƿ���)
Act17_LastTime = 0

--�����
Act17_RewardItem = {1220007, 1230007, 1210003, 1420007, 1420011, 1420015, 1420019, 1420031, 1420035}

--���ˢ���ľ�������
Act17_CrystalNum = 0

--��¼ÿ�����ɱ���ĵ��˵Ĵ���
PKAct17_killrole = {}

--����ȽϺ���
local function Act17_Cmp(a, b)
	if Act17_RoleData[b].Score < Act17_RoleData[a].Score then
		return true
	elseif Act17_RoleData[b].Score == Act17_RoleData[a].Score and Act17_RoleData[b].KillPlayer < Act17_RoleData[a].KillPlayer then
		return true
	elseif Act17_RoleData[b].Score == Act17_RoleData[a].Score and Act17_RoleData[b].KillPlayer == Act17_RoleData[a].KillPlayer and Act17_RoleData[b].JoinTime < Act17_RoleData[a].JoinTime then
		return true
	else
		return false
    end
end

--�洢��ǰ����
function SaveAct17RoleData()
    local Index = 1
    for k in pairsByKeys(Act17_RoleData, Act17_Cmp) do
        act.SetActScriptData(17, 1, Index, k)
        Index = Index + 1
        if Index > 20 then
            break
        end
    end
end

--------------------------------------------------------------------------------------------------------
--�̶����ʼ
function Act17_OnStart(ActID)
	--���������
	map.CreateInstance(2983744463, 4)

	--��ʼ�������ʱ��
	Act17_LastTime = 0
	--��ʼ��ˮ��������
	Act17_CrystalNum = 0

	--��ʼ������������
    local Empty = {}
	Act17_RoleData = Empty
end

function Act17_OnTimerMin(ActID)
	--�ۼӻ����ʱ��
	Act17_LastTime = Act17_LastTime + 1

	--���ʼ��ĵ�5������ʱ�䣨��5��10����50��55���ӣ�, ������һ���+5
	--if Act17_LastTime % 5 == 0 then
		for k in pairs(Act17_RoleData) do
			--����ڻ��ͼ��
			if Act17_RoleData[k].IsInMap then
			    Act17_RoleData[k].Score = Act17_RoleData[k].Score + 33
				--ͬ���ͻ��˻���
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 13,	1)
				msg.AddMsgEvent(MsgID, 1, 2851)			--�ʱ����ֽ�������������33��
				msg.DispatchRoleMsgEvent(k, MsgID)
			    if map.IsInArea(2983744463, 4, k, 636)	then--����Ƿ��ڻ�Ծ����
					--���û������
					if false == unit.IsDead(2983744463, 4, k) then
						Act17_RoleData[k].Score = Act17_RoleData[k].Score + 33
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13,	1)
						msg.AddMsgEvent(MsgID, 1, 2852)			--���ڻ�Ծ���򣬻��ֽ���������
						msg.DispatchRoleMsgEvent(k, MsgID)
					end
				end
				--ͬ���ͻ��˻���
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 60,	153)								--������
				msg.AddMsgEvent(MsgID, 14,	Act17_RoleData[k].Score)			--����
				msg.DispatchRoleMsgEvent(k, MsgID)
			end
		end
    if Act17_LastTime % 5 == 0 then
		local total = 0
		local avaliable = {}
		for ii=1,20 do
			if m21_SpawnPos[ii].Crystal100 == false then
				total = total+1
				avaliable[total] = ii
			end
		end
		while Act17_CrystalNum < 5 and total>0 do
		    --���һ������
			local Index = math.random(1,total)
			Index = avaliable[Index]
		    avaliable[Index] = avaliable[total]
			Act17_CrystalNum = Act17_CrystalNum + 1
			m21_SpawnPos[Index].Crystal100 = true
		    total = total -1
			--����õ�һ��ˮ����ID
			local CrystalBallID = math.random(1520001, 1520007)
			--ˢ��ˮ����
			local CreatureID = map.MapCreateCreature(2983744463, 4, CrystalBallID, m21_SpawnPos[Index].x, m21_SpawnPos[Index].y, m21_SpawnPos[Index].z)
			cre.SetCreatureScriptData(2983744463, 4, CreatureID, 1, 0, Index)
		end
	end

	--�ۼ���Ҳμӻ��ʱ��
	for n in pairs(Act17_RoleData) do
		if Act17_RoleData[n].IsInMap then
			Act17_RoleData[n].JoinTime = Act17_RoleData[n].JoinTime + 1
			if (Act17_RoleData[n].JoinTime % 10) == 0 and false == unit.IsDead(2983744463, 4, n) then
				--����2000��
				role.AddRoleSilver(2983744463, 4, n, 2000, 1001)
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 101, 2853)   --���������ۼ�ʱ��ﵽ10�ı�������ö���Ľ�Ǯ����
				msg.DispatchRoleMsgEvent(n, MsgID)
			end
		end
	end

	--ÿ��10���ӣ�������ڲ߻�ָ���ص�ˢ�³�1�����ǹ���
	if Act17_LastTime % 10 == 0 then
        local Index = math.random(1, 2)
        map.MapCreateCreature(2983744463, 4, 1520020, XiongXing_BornPos[Index].x, XiongXing_BornPos[Index].y, XiongXing_BornPos[Index].z)
	end

end

--�����
function Act17_OnEnd(actID)
	SaveAct17RoleData()
	PKAct17_killrole = {}
    local rank ={}
	for i=1,10 do
	    rank[act.GetActScriptData(17, 1, i)] = i
	end

	--�����ȡһ����������д������
	for n in pairs(Act17_RoleData) do
		if Act17_RoleData[n].IsInMap then
		    local AddExp = role.GetRoleLevelUpExp(2983744463, 4, n)
			local rolelevel = role.GetRoleLevel(2983744463, 4, n)
			local Scoreact = Act17_RoleData[n].Score
			if Scoreact >3000 then
			    Scoreact = 3000
			end
			local exp = AddExp * Scoreact / 3000 * 130 / (rolelevel - 25) / (rolelevel - 25)
			role.AddRoleExp(2983744463, 4, n, exp)
			local wushiyin = 0
			if Act17_RoleData[n].Score >= 2000 then
                wushiyin = 30
            elseif Act17_RoleData[n].Score >= 1000 then
			    wushiyin = 20
			elseif Act17_RoleData[n].Score >= 500 then
			    wushiyin = 15
			elseif Act17_RoleData[n].Score >= 250 then
			    wushiyin = 10
			elseif Act17_RoleData[n].Score >= 150 then
			    wushiyin = 6
			elseif Act17_RoleData[n].Score >= 100 then
			    wushiyin = 4
			elseif Act17_RoleData[n].Score >= 50 then
			    wushiyin = 2
			else
			    wushiyin = 1
			end
			if rank[n] ~= nil then
			    if rank[n] == 1 then
				    --role.AddRoleItem(2983744463, 1, n, 2612621, 1, -1, 13, 1001)
					--role.AddRoleItem(2983744463, 1, n, 2612629, 10, -1, 13, 1001)
					role.AddRoleItem(2983744463, 4, n, 2616345, wushiyin, -1, 13, 1001)
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 71, 2854)  --����Ⱥ�������л�õ�һ�����õ�����<color=0xffff0000>XX<color=0xfffff7e0>���<color=0xffff0000>X<color=0xfffff7e0>��<color=0xff00a2ff>��ʿӡ<color=0xfffff7e0>������������¹Ӣ�۰���ȡר���ƺź�<color=0xffff0000>10<color=0xfffff7e0>��<color=0xff00a2ff>Ӣ����<color=0xfffff7e0>����<color=0xffff0000>XX<color=0xfffff7e0>�㣡
					msg.AddMsgEvent(MsgID, 9, exp)
					msg.AddMsgEvent(MsgID, 9, wushiyin)
					msg.DispatchRoleMsgEvent(n, MsgID)
				elseif rank[n] <= 10 then
				    local itemnum = 1
					if rank[n] == 2 or rank[n] == 3 then
					    itemnum = 8
					elseif rank[n] == 4 or rank[n] == 5 or rank[n] == 6 then
					    itemnum = 4
					elseif rank[n] == 7 or rank[n] == 8 or rank[n] == 9 or rank[n] == 10 then
					    itemnum = 2
					end
				   --role.AddRoleItem(2983744463, 1, n, 2612629, itemnum, -1, 13, 1001)
				    role.AddRoleItem(2983744463, 4, n, 2616345, wushiyin, -1, 13, 1001)
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 71, 2855)  --��ϲ����Ⱥ�������л�õ�X�����õ�����<color=0xffff0000>XX<color=0xfffff7e0>���<color=0xffff0000>X<color=0xfffff7e0>��<color=0xff00a2ff>��ʿӡ<color=0xfffff7e0>������������¹Ӣ�۰���ȡ<color=0xffff0000>X<color=0xfffff7e0>��<color=0xff00a2ff>Ӣ����<color=0xfffff7e0>��
					msg.AddMsgEvent(MsgID, 9, rank[n])
					msg.AddMsgEvent(MsgID, 9, exp)
					msg.AddMsgEvent(MsgID, 9, wushiyin)
					msg.AddMsgEvent(MsgID, 9, itemnum)
					msg.DispatchRoleMsgEvent(n, MsgID)
				end
			else
			    role.AddRoleItem(2983744463, 4, n, 2616345, wushiyin, -1, 13, 1001)
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 71, 2856)  --����Ⱥ�������л�þ���<color=0xffff0000>XX<color=0xfffff7e0>���<color=0xffff0000>X<color=0xfffff7e0>��<color=0xff00a2ff>��ʿӡ<color=0xfffff7e0>ϣ�����ٽ�������
				msg.AddMsgEvent(MsgID, 9, exp)
				msg.AddMsgEvent(MsgID, 9, wushiyin)
				msg.DispatchRoleMsgEvent(n, MsgID)
			end
		end
	end

	--ɾ�������
	map.DeleteInstance(2983744463, 4)
end

--ע��û������¼�
aux.RegisterActEvent(17, 2, "Act17_OnStart") 	--������
aux.RegisterActEvent(17, 4, "Act17_OnTimerMin") --������
aux.RegisterActEvent(17, 3, "Act17_OnEnd") 		--������
aux.RegisterActEvent(102, 2, "Act17_OnStart") 	--������
aux.RegisterActEvent(102, 4, "Act17_OnTimerMin") --������
aux.RegisterActEvent(102, 3, "Act17_OnEnd") 	--������

--�ͻ�����ͬ������������
function Act17_DefaultRequest(ActID, RoleID, EventType)
	local Index = 1
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 50, 0)

	for k in pairsByKeys(Act17_RoleData, Act17_Cmp) do
		msg.AddMsgEvent(MsgID, 15, Index)
		msg.AddMsgEvent(MsgID, 2, k)
		msg.AddMsgEvent(MsgID, 14, Act17_RoleData[k].Score)

		Index = Index + 1
		if Index > 10 then
			break
		end
	end
	msg.DispatchRoleMsgEvent(RoleID, MsgID)
end


aux.RegisterActEvent(17, 5, "Act17_DefaultRequest")
aux.RegisterActEvent(102, 5, "Act17_DefaultRequest")]]
