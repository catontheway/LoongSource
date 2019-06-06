--[[�̶��Ӣ������Ӣ����

--ScriptData�и����ֶ�����˵��
--1 - 20Ϊǰ20����ҵ�ID��˳�������ν��д洢

--����������((����RoleID, RebornTime, Score��IsDeadLeave, IsInMap, JoinTime)
Act16_RoleData = {} --Ӣ����

--�̶���Ѿ���ʼ��ʱ��(��λ�Ƿ���)
Act16_LastTime = 0

--�����
Act16_RewardItem = {1220007, 1230007, 1210003, 1420007, 1420011, 1420015, 1420019, 1420031, 1420035}

--���ˢ���ľ�������
Act16_CrystalNum = 0

--��¼ÿ�����ɱ���ĵ��˵Ĵ���
PKAct16_killrole = {}

--����ȽϺ���
local function Act16_Cmp(a, b)
	if Act16_RoleData[b].Score < Act16_RoleData[a].Score then
		return true
	elseif Act16_RoleData[b].Score == Act16_RoleData[a].Score and Act16_RoleData[b].KillPlayer < Act16_RoleData[a].KillPlayer then
		return true
	elseif Act16_RoleData[b].Score == Act16_RoleData[a].Score and Act16_RoleData[b].KillPlayer == Act16_RoleData[a].KillPlayer and Act16_RoleData[b].JoinTime < Act16_RoleData[a].JoinTime then
		return true
	else
		return false
    end
end

--�洢��ǰ����
function SaveAct16RoleData()
    local Index = 1
    for k in pairsByKeys(Act16_RoleData, Act16_Cmp) do
        act.SetActScriptData(16, 1, Index, k)
        Index = Index + 1
        if Index > 20 then
            break
        end
    end
end

--------------------------------------------------------------------------------------------------------
--�̶����ʼ
function Act16_OnStart(ActID)
	--���������
	map.CreateInstance(2983744463, 3)

	--��ʼ�������ʱ��
	Act16_LastTime = 0
	--��ʼ��ˮ��������
	Act16_CrystalNum = 0

	--��ʼ������������
    local Empty = {}
	Act16_RoleData = Empty
end

function Act16_OnTimerMin(ActID)
	--�ۼӻ����ʱ��
	Act16_LastTime = Act16_LastTime + 1

	--���ʼ��ĵ�5������ʱ�䣨��5��10����50��55���ӣ�, ������һ���+5
	--if Act16_LastTime % 5 == 0 then
		for k in pairs(Act16_RoleData) do
			--����ڻ��ͼ��
			if Act16_RoleData[k].IsInMap then
			    Act16_RoleData[k].Score = Act16_RoleData[k].Score + 33
				--ͬ���ͻ��˻���
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 13,	1)
				msg.AddMsgEvent(MsgID, 1, 2851)			--�ʱ����ֽ�������������33��
				msg.DispatchRoleMsgEvent(k, MsgID)
			    if map.IsInArea(2983744463, 3, k, 636)	then--����Ƿ��ڻ�Ծ����
					--���û������
					if false == unit.IsDead(2983744463, 3, k) then
						Act16_RoleData[k].Score = Act16_RoleData[k].Score + 33
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13,	1)
						msg.AddMsgEvent(MsgID, 1, 2852)			--���ڻ�Ծ���򣬻��ֽ���������
						msg.DispatchRoleMsgEvent(k, MsgID)
					end
				end
				--ͬ���ͻ��˻���
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 60,	153)								--������
				msg.AddMsgEvent(MsgID, 14,	Act16_RoleData[k].Score)			--����
				msg.DispatchRoleMsgEvent(k, MsgID)
			end
		end
    if Act16_LastTime % 5 == 0 then
		local total = 0
		local avaliable = {}
		for ii=1,20 do
			if m21_SpawnPos[ii].Crystal80 == false then
				total = total+1
				avaliable[total] = ii
			end
		end
		while Act16_CrystalNum < 5 and total > 0 do
		    --���һ������
			local Index = math.random(1,total)
			Index = avaliable[Index]
		    avaliable[Index] = avaliable[total]
			Act16_CrystalNum = Act16_CrystalNum + 1
			m21_SpawnPos[Index].Crystal80 = true
		    total = total -1
			--����õ�һ��ˮ����ID
			local CrystalBallID = math.random(1520001, 1520007)
			--ˢ��ˮ����
			local CreatureID = map.MapCreateCreature(2983744463, 3, CrystalBallID, m21_SpawnPos[Index].x, m21_SpawnPos[Index].y, m21_SpawnPos[Index].z)
			cre.SetCreatureScriptData(2983744463, 3, CreatureID, 1, 0, Index)
		end
	end

	--�ۼ���Ҳμӻ��ʱ��
	for n in pairs(Act16_RoleData) do
		if Act16_RoleData[n].IsInMap then
			Act16_RoleData[n].JoinTime = Act16_RoleData[n].JoinTime + 1
			if (Act16_RoleData[n].JoinTime % 10) == 0 and false == unit.IsDead(2983744463, 3, n) then
				--����2000��
				role.AddRoleSilver(2983744463, 3, n, 2000, 1001)
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 101, 2853)   --���������ۼ�ʱ��ﵽ10�ı�������ö���Ľ�Ǯ����
				msg.DispatchRoleMsgEvent(n, MsgID)
			end
		end
	end

	--ÿ��10���ӣ�������ڲ߻�ָ���ص�ˢ�³�1�����ǹ���
	if Act16_LastTime % 10 == 0 then
        local Index = math.random(1, 2)
        map.MapCreateCreature(2983744463, 3, 1520020, XiongXing_BornPos[Index].x, XiongXing_BornPos[Index].y, XiongXing_BornPos[Index].z)
	end

end

--�����
function Act16_OnEnd(actID)
	SaveAct16RoleData()
	PKAct16_killrole = {}
    local rank ={}
	for i=1,10 do
	    rank[act.GetActScriptData(16, 1, i)] = i
	end

	--�����ȡһ����������д������
	for n in pairs(Act16_RoleData) do
		if Act16_RoleData[n].IsInMap then
		    local AddExp = role.GetRoleLevelUpExp(2983744463, 3, n)
			local rolelevel = role.GetRoleLevel(2983744463, 3, n)
			local Scoreact = Act16_RoleData[n].Score
			if Scoreact >3000 then
			    Scoreact = 3000
			end
			local exp = AddExp * Scoreact / 3000 * 130 / (rolelevel - 25) / (rolelevel - 25)
			role.AddRoleExp(2983744463, 3, n, exp)
			local wushiyin = 0
			if Act16_RoleData[n].Score >= 2000 then
                wushiyin = 30
            elseif Act16_RoleData[n].Score >= 1000 then
			    wushiyin = 20
			elseif Act16_RoleData[n].Score >= 500 then
			    wushiyin = 15
			elseif Act16_RoleData[n].Score >= 250 then
			    wushiyin = 10
			elseif Act16_RoleData[n].Score >= 150 then
			    wushiyin = 6
			elseif Act16_RoleData[n].Score >= 100 then
			    wushiyin = 4
			elseif Act16_RoleData[n].Score >= 50 then
			    wushiyin = 2
			else
			    wushiyin = 1
			end
			if rank[n] ~= nil then
			    if rank[n] == 1 then
				    --role.AddRoleItem(2983744463, 1, n, 2612621, 1, -1, 13, 1001)
					--role.AddRoleItem(2983744463, 1, n, 2612629, 10, -1, 13, 1001)
					role.AddRoleItem(2983744463, 3, n, 2616345, wushiyin, -1, 13, 1001)
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
				    role.AddRoleItem(2983744463, 3, n, 2616345, wushiyin, -1, 13, 1001)
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 71, 2855)  --��ϲ����Ⱥ�������л�õ�X�����õ�����<color=0xffff0000>XX<color=0xfffff7e0>���<color=0xffff0000>X<color=0xfffff7e0>��<color=0xff00a2ff>��ʿӡ<color=0xfffff7e0>������������¹Ӣ�۰���ȡ<color=0xffff0000>X<color=0xfffff7e0>��<color=0xff00a2ff>Ӣ����<color=0xfffff7e0>��
					msg.AddMsgEvent(MsgID, 9, rank[n])
					msg.AddMsgEvent(MsgID, 9, exp)
					msg.AddMsgEvent(MsgID, 9, wushiyin)
					msg.AddMsgEvent(MsgID, 9, itemnum)
					msg.DispatchRoleMsgEvent(n, MsgID)
				end
			else
			    role.AddRoleItem(2983744463, 3, n, 2616345, wushiyin, -1, 13, 1001)
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 71, 2856)  --����Ⱥ�������л�þ���<color=0xffff0000>XX<color=0xfffff7e0>���<color=0xffff0000>X<color=0xfffff7e0>��<color=0xff00a2ff>��ʿӡ<color=0xfffff7e0>ϣ�����ٽ�������
				msg.AddMsgEvent(MsgID, 9, exp)
				msg.AddMsgEvent(MsgID, 9, wushiyin)
				msg.DispatchRoleMsgEvent(n, MsgID)
		    end
		end
    end
	--ɾ�������
	map.DeleteInstance(2983744463, 3)
end

--ע��û������¼�
aux.RegisterActEvent(16, 2, "Act16_OnStart") 	--Ӣ����
aux.RegisterActEvent(16, 4, "Act16_OnTimerMin") --Ӣ����
aux.RegisterActEvent(16, 3, "Act16_OnEnd") 		--Ӣ����
aux.RegisterActEvent(101, 2, "Act16_OnStart") 	--Ӣ����
aux.RegisterActEvent(101, 4, "Act16_OnTimerMin") --Ӣ����
aux.RegisterActEvent(101, 3, "Act16_OnEnd") 		--Ӣ����

--�ͻ�����ͬ������������
function Act16_DefaultRequest(ActID, RoleID, EventType)
	local Index = 1
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 50, 0)

	for k in pairsByKeys(Act16_RoleData, Act16_Cmp) do
		msg.AddMsgEvent(MsgID, 15, Index)
		msg.AddMsgEvent(MsgID, 2, k)
		msg.AddMsgEvent(MsgID, 14, Act16_RoleData[k].Score)

		Index = Index + 1
		if Index > 10 then
			break
		end
	end
	msg.DispatchRoleMsgEvent(RoleID, MsgID)
end


aux.RegisterActEvent(16, 5, "Act16_DefaultRequest")
aux.RegisterActEvent(101, 5, "Act16_DefaultRequest")]]
