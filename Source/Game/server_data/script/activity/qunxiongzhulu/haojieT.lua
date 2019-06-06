--[[�̶��Ⱥ����¹������

--ScriptData�и����ֶ�����˵��
--0:��Ľ�����1- 20Ϊǰ10��С�ӱ�ţ�˳�������ν��д洢��

--����������(����RoleID, RebornTime, IsDeadLeave, IsInMap)
Act19_RoleData = {} --������

--����������(��������С�ӱ��, Act19_RoleData1, Act19_RoleData2, Act19_RoleData3��Act19_RoleData4��Act19_RoleData5��Act19_RoleData6)
Act19_TeamRoleData = {}

--��е����С������(����С�ӱ��, Score, JoinTime)
Act19_TeamData = {}

--�̶���ѿ�ʼ��ʱ�䣨��λ�Ƿ��ӣ�
Act19_LastTime = 0

--���С�ӱ�����ֵ
Act19_TeamIndex = 1

--���ǰ����
Act19_Expire = 0

--�Ƿ��Ѿ������˸���
IsCreateAct19Instance = 0

--�����
Act19_RewardItem = {1220007, 1230007, 1210003, 1420007, 1420011, 1420015, 1420019, 1420031, 1420035}

--���ˢ���ľ�������
Act19_CrystalNum = 0

--��¼ÿ�����ɱ���ĵ��˵Ĵ���
PKAct19_killrole = {}

-------------------------------------------------------------------------------------------------------
--����ȽϺ���
function Act19_Cmp(a,b)
    if Act19_TeamData[b].Score < Act19_TeamData[a].Score then
		return true
	elseif Act19_TeamData[b].Score == Act19_TeamData[a].Score and Act19_TeamData[b].JoinTime < Act19_TeamData[a].JoinTime then
		return true
	else
		return false
    end
end

--------------------------------------------------------------------------------------------------------
--�洢��ǰ����
function SaveAct19TeamData()
    act.SetActScriptData(19, 1, 0, Act19_Expire)
    local Index = 1
    for k in pairsByKeys(Act19_TeamData, Act19_Cmp) do
        act.SetActScriptData(19, 1, Index, k)
        Index = Index + 1
        if Index > 10 then
            break
        end
	end
end

--------------------------------------------------------------------------------------------------------
--�̶����ʼ
function act19_OnStart(ActID)
	--��ʼ����ѽ���ʱ��
	Act19_LastTime = 0

	--���ˢ���ľ�������
    Act19_CrystalNum = 0
	Act19_Expire = act.GetActScriptData(19, 1, 0)

	--ͬ�����г������������ʱ��
	for n in pairs(Act19_TeamRoleData) do
		for k in pairs(Act19_TeamRoleData[n]) do
		    if Act19_TeamRoleData[n][k].IsInMap then
		        --���ͻʣ��ʱ��
	            local MsgID = msg.BeginMsgEvent()
	            msg.AddMsgEvent(MsgID, 60,	153)
	            msg.AddMsgEvent(MsgID, 16,	1800)							                 --ʣ��ʱ��
	            msg.DispatchRoleMsgEvent(k, MsgID)

	            --�������Ϊ����̨״̬(���ڿͻ��˵����жϣ�
	            role.SetRoleState(k, 8, true)
	        end
	    end
	end
end

function act19_OnTimerMin(ActID)
	--�ۼӻ����ʱ��
    Act19_LastTime = Act19_LastTime + 1

		for k in pairs(Act19_TeamRoleData) do
			for n in pairs(Act19_TeamRoleData[k]) do
				--����ڻ��ͼ��
				if Act19_TeamRoleData[k][n].IsInMap then
					--���û������
					Act19_TeamData[k].Score = Act19_TeamData[k].Score + 6
					--����Ƿ��ڻ�Ծ����
					if map.IsInArea(2983743695, 2, n, 636)	then
						if unit.IsDead(2983743695, 2, n) then
							Act19_TeamData[k].Score = Act19_TeamData[k].Score + 6
						end
					end
				end
			end
			--ͬ���ͻ��˻���
			SendScore2Teamate(154, Act19_TeamData[k].Score, Act19_TeamRoleData[k])
		end
    if Act19_LastTime % 5 == 0 then
		local total = 0
		local avaliable = {}
		for ii=1,20 do
			if m22_SpawnPos[ii].Crystal60 == false then
				total = total+1
				avaliable[total] = ii
			end
		end
		while Act19_CrystalNum < 5 and total > 0 do
		    --���һ������
			local Index = math.random(1,total)
			Index = avaliable[Index]
		    avaliable[Index] = avaliable[total]
			Act19_CrystalNum = Act19_CrystalNum + 1
			m22_SpawnPos[Index].Crystal60 = true
		    total = total -1
			--����õ�һ��ˮ����ID
			local CrystalBallID = math.random(1520001, 1520007)
			--ˢ��ˮ����
			local CreatureID = map.MapCreateCreature(2983743695, 2, CrystalBallID, m22_SpawnPos[Index].x, m22_SpawnPos[Index].y, m22_SpawnPos[Index].z)
			cre.SetCreatureScriptData(2983743695, 2, CreatureID, 1, 0, Index)
		end
	end

	--�ۼ���Ҳμӻ��ʱ��
	for k in pairs(Act19_TeamData) do
	    Act19_TeamData[k].JoinTime = Act19_TeamData[k].JoinTime + 1
	    if (Act19_TeamData[k].JoinTime % 10) == 0 then
		    for n in pairs(Act19_TeamRoleData[k]) do
		        if Act19_TeamRoleData[k][n].IsInMap and false == unit.IsDead(2983743695, 2, n)then
			        --����2000��
			        role.AddRoleSilver(2983743695, 2, n, 2000, 1001)
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 101, 2853)   --���������ۼ�ʱ��ﵽ10�ı�������ö���Ľ�Ǯ����
					msg.DispatchRoleMsgEvent(n, MsgID)
			    end
			end
		end
	end
end

--�����
function act19_OnEnd(actID)
	--ɾ�������
	IsCreateAct19Instance = 0
	SaveAct19TeamData()
    PKAct19_killrole = {}
    Act19_RoleData = {}
	local rank ={}
	for i=1,10 do
	    rank[act.GetActScriptData(19, 1, i)] = i
	end

	--�����ȡһ����������д������
	for k in pairs(Act19_TeamData) do
	    --�����ȡһ����������д������
		for n in pairs(Act19_TeamRoleData[k]) do
			if Act19_TeamRoleData[k][n].IsInMap then
				local AddExp = role.GetRoleLevelUpExp(2983743695, 2, n)
				local rolelevel = role.GetRoleLevel(2983743695, 2, n)
				local Scoreact = Act19_TeamData[k].Score
				if Scoreact >3000 then
					Scoreact = 3000
				end
				local exp = AddExp * Scoreact / 3000 * 130 / (rolelevel - 25) / (rolelevel - 25)
				role.AddRoleExp(2983743695, 2, n, exp)
				local wushiyin = 0
				if Act19_TeamData[k].Score >= 2000 then
					wushiyin = 30
				elseif Act19_TeamData[k].Score >= 1000 then
					wushiyin = 20
				elseif Act19_TeamData[k].Score >= 500 then
					wushiyin = 15
				elseif Act19_TeamData[k].Score >= 250 then
					wushiyin = 10
				elseif Act19_TeamData[k].Score >= 150 then
					wushiyin = 6
				elseif Act19_TeamData[k].Score >= 100 then
					wushiyin = 4
				elseif Act19_TeamData[k].Score >= 50 then
					wushiyin = 2
				else
					wushiyin = 1
				end
				if rank[k] ~= nil then
					if rank[k] == 1 then
						--role.AddRoleItem(2983743695, 2, n, 2612621, 1, -1, 13, 1001)
						--role.AddRoleItem(2983743695, 2, n, 2612629, 10, -1, 13, 1001)
						role.AddRoleItem(2983743695, 2, n, 2616345, wushiyin, -1, 13, 1001)
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
					   --role.AddRoleItem(2983743695, 2, n, 2612629, itemnum, -1, 13, 1001)
						role.AddRoleItem(2983743695, 2, n, 2616345, wushiyin, -1, 13, 1001)
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 71, 2858)  --��ϲ����Ⱥ����¹��л�õ�X�����õ�����<color=0xffff0000>XX<color=0xfffff7e0>���<color=0xffff0000>X<color=0xfffff7e0>��<color=0xff00a2ff>��ʿӡ<color=0xfffff7e0>������������¹Ӣ�۰���ȡ<color=0xffff0000>X<color=0xfffff7e0>��<color=0xff00a2ff>Ӣ����<color=0xfffff7e0>��
						msg.AddMsgEvent(MsgID, 9, rank[k])
						msg.AddMsgEvent(MsgID, 9, exp)
						msg.AddMsgEvent(MsgID, 9, wushiyin)
						msg.AddMsgEvent(MsgID, 9, itemnum)
						msg.DispatchRoleMsgEvent(n, MsgID)
					end
				else
					role.AddRoleItem(2983743695, 2, n, 2616345, wushiyin, -1, 13, 1001)
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 71, 2859)  --����Ⱥ����¹��л�þ���<color=0xffff0000>XX<color=0xfffff7e0>���<color=0xffff0000>X<color=0xfffff7e0>��<color=0xff00a2ff>��ʿӡ<color=0xfffff7e0>ϣ�����ٽ�������
					msg.AddMsgEvent(MsgID, 9, exp)
					msg.AddMsgEvent(MsgID, 9, wushiyin)
					msg.DispatchRoleMsgEvent(n, MsgID)
				end
				--�����ȡһ����Ʒ��Ϊ����
				if 1 <= role.GetBagFreeSize(n) then
					role.AddRoleItem(2983743695, 2, n, 2612621, 1, -1, 13, 1001)

					--��ʾ���"������Ӣ�����������###������ϣ���´��ٽ�������"
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 71, 164)
					msg.AddMsgEvent(MsgID, 4, Act19_RewardItem[Index])
					msg.DispatchRoleMsgEvent(n, MsgID)
				end
			end
		end
	end
	map.DeleteInstance(2983743695, 2)
end

--ע��û������¼�
aux.RegisterActEvent(19, 2, "act19_OnStart") 	--������
aux.RegisterActEvent(19, 4, "act19_OnTimerMin") --������
aux.RegisterActEvent(19, 3, "act19_OnEnd") 		--������
aux.RegisterActEvent(104, 2, "act19_OnStart") 	--������
aux.RegisterActEvent(104, 4, "act19_OnTimerMin") --������
aux.RegisterActEvent(104, 3, "act19_OnEnd") 		--������
-------------------------------------------------------------------------------------------------------
--�ͻ�����ͬ������������
function Act19_DefaultRequest(ActID, RoleID, EventType)
	local Index = 1
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 50, 1)

	for k in pairsByKeys(Act19_TeamData, Act19_Cmp) do
		msg.AddMsgEvent(MsgID, 15, Index)
		msg.AddMsgEvent(MsgID, 2, Act19_TeamData[k].LeaderID)
		msg.AddMsgEvent(MsgID, 14, Act19_TeamData[k].Score)
		msg.AddMsgEvent(MsgID, 18, k)

		Index = Index + 1
		if Index > 20 then
			break
		end
	end
	msg.DispatchRoleMsgEvent(RoleID, MsgID)
end

aux.RegisterActEvent(19, 5, "Act19_DefaultRequest")
aux.RegisterActEvent(104, 5, "Act19_DefaultRequest")]]
