--[[�̶��Ⱥ����¹������

--ScriptData�и����ֶ�����˵��
--0:��Ľ�����1- 20Ϊǰ10��С�ӱ�ţ�˳�������ν��д洢��

--����������(����RoleID, RebornTime, IsDeadLeave, IsInMap)
Act21_RoleData = {} --������

--����������(��������С�ӱ��, Act18_RoleData1, Act18_RoleData2, Act18_RoleData3��Act18_RoleData4��Act18_RoleData5��Act18_RoleData6)
Act21_TeamRoleData = {}

--��е����С������(����С�ӱ��, Score, JoinTime)
Act21_TeamData = {}

--�̶���ѿ�ʼ��ʱ�䣨��λ�Ƿ��ӣ�
Act21_LastTime = 0

--���С�ӱ�����ֵ
Act21_TeamIndex = 1

--���ǰ����
Act21_Expire = 0

--�Ƿ��Ѿ������˸���
IsCreateAct21Instance = 0

--�����
Act21_RewardItem = {1220011, 1230011, 1210003, 1420007, 1420011, 1420015, 1420019, 1420031, 1420035}

--���ˢ���ľ�������
Act21_CrystalNum = 0

--��¼ÿ�����ɱ���ĵ��˵Ĵ���
PKAct21_killrole = {}
-------------------------------------------------------------------------------------------------------
--����ȽϺ���
function Act21_Cmp(a,b)
    if Act21_TeamData[b].Score < Act21_TeamData[a].Score then
		return true
	elseif Act21_TeamData[b].Score == Act21_TeamData[a].Score and Act21_TeamData[b].JoinTime < Act21_TeamData[a].JoinTime then
		return true
	else
		return false
    end
end

--------------------------------------------------------------------------------------------------------
--�洢��ǰ����
function SaveAct21TeamData()
    act.SetActScriptData(21, 1, 0, Act21_Expire)
    local Index = 1
    for k in pairsByKeys(Act21_TeamData, Act21_Cmp) do
        act.SetActScriptData(21, 1, Index, k)
        Index = Index + 1
        if Index > 10 then
            break
        end
	end
end

--------------------------------------------------------------------------------------------------------
--�̶����ʼ
function Act21_OnStart(ActID)
	--��ʼ����ѽ���ʱ��
	Act21_LastTime = 0

	--���ˢ���ľ�������
    Act21_CrystalNum = 0
	Act21_Expire = act.GetActScriptData(21, 1, 0)

	--ͬ�����г������������ʱ��
	for n in pairs(Act21_TeamRoleData) do
		for k in pairs(Act21_TeamRoleData[n]) do
		    if Act21_TeamRoleData[n][k].IsInMap then
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

function Act21_OnTimerMin(ActID)
	--�ۼӻ����ʱ��
    Act21_LastTime = Act21_LastTime + 1

		for k in pairs(Act21_TeamRoleData) do
			for n in pairs(Act21_TeamRoleData[k]) do
				--����ڻ��ͼ��
				if Act21_TeamRoleData[k][n].IsInMap then
					--���û������
					Act21_TeamData[k].Score = Act21_TeamData[k].Score + 6
					--����Ƿ��ڻ�Ծ����
					if map.IsInArea(2983743695, 4, n, 636)	then
						if unit.IsDead(2983743695, 4, n) then
							Act21_TeamData[k].Score = Act21_TeamData[k].Score + 6
						end
					end
				end
			end
			--ͬ���ͻ��˻���
			SendScore2Teamate(154, Act21_TeamData[k].Score, Act21_TeamRoleData[k])
		end
    if Act21_LastTime % 5 == 0 then
		local total = 0
		local avaliable = {}
		for ii=1,20 do
			if m22_SpawnPos[ii].Crystal100 == false then
				total = total+1
				avaliable[total] = ii
			end
		end
		while Act21_CrystalNum < 5 and total > 0 do
		    --���һ������
			local Index = math.random(1,total)
			Index = avaliable[Index]
		    avaliable[Index] = avaliable[total]
			Act21_CrystalNum = Act21_CrystalNum + 1
			m22_SpawnPos[Index].Crystal100 = true
		    total = total -1
			--����õ�һ��ˮ����ID
			local CrystalBallID = math.random(1520001, 1520007)
			--ˢ��ˮ����
			local CreatureID = map.MapCreateCreature(2983743695, 4, CrystalBallID, m22_SpawnPos[Index].x, m22_SpawnPos[Index].y, m22_SpawnPos[Index].z)
			cre.SetCreatureScriptData(2983743695, 4, CreatureID, 1, 0, Index)
		end
	end

	--�ۼ���Ҳμӻ��ʱ��
	for k in pairs(Act21_TeamData) do
	    Act21_TeamData[k].JoinTime = Act21_TeamData[k].JoinTime + 1
	    if (Act21_TeamData[k].JoinTime % 10) == 0 then
		    for n in pairs(Act21_TeamRoleData[k]) do
		        if Act21_TeamRoleData[k][n].IsInMap and false == unit.IsDead(2983743695, 1, n)then
			        --����2000��
			        role.AddRoleSilver(2983743695, 1, n, 2000, 1001)
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 101, 2853)   --���������ۼ�ʱ��ﵽ10�ı�������ö���Ľ�Ǯ����
					msg.DispatchRoleMsgEvent(n, MsgID)
			    end
			end
		end
	end
	--ÿ��15���ӣ�������ڲ߻�ָ���ص�ˢ�³�1�����ǹ���
	if Act21_LastTime % 10 == 0 then
        local Index = math.random(1, 2)
        map.MapCreateCreature(2983743695, 4, 1520021, XiongXing_BornPos[Index].x, XiongXing_BornPos[Index].y, XiongXing_BornPos[Index].z)
	end
end

--�����
function Act21_OnEnd(actID)
	--ɾ�������
	IsCreateAct21Instance = 0
	SaveAct21TeamData()
    PKAct21_killrole = {}
    Act21_RoleData = {}
	local rank ={}
	for i=1,10 do
	    rank[act.GetActScriptData(21, 1, i)] = i
	end

	--�����ȡһ����������д������
	for k in pairs(Act21_TeamData) do
	    --�����ȡһ����������д������
		for n in pairs(Act21_TeamRoleData[k]) do
			if Act21_TeamRoleData[k][n].IsInMap then
				local AddExp = role.GetRoleLevelUpExp(2983743695, 4, n)
				local rolelevel = role.GetRoleLevel(2983743695, 4, n)
				local Scoreact = Act21_TeamData[k].Score
				if Scoreact >3000 then
					Scoreact = 3000
				end
				local exp = AddExp * Scoreact / 3000 * 130 / (rolelevel - 25) / (rolelevel - 25)
				role.AddRoleExp(2983743695, 4, n, exp)
				local wushiyin = 0
				if Act21_TeamData[k].Score >= 2000 then
					wushiyin = 30
				elseif Act21_TeamData[k].Score >= 1000 then
					wushiyin = 20
				elseif Act21_TeamData[k].Score >= 500 then
					wushiyin = 15
				elseif Act21_TeamData[k].Score >= 250 then
					wushiyin = 10
				elseif Act21_TeamData[k].Score >= 150 then
					wushiyin = 6
				elseif Act21_TeamData[k].Score >= 100 then
					wushiyin = 4
				elseif Act21_TeamData[k].Score >= 50 then
					wushiyin = 2
				else
					wushiyin = 1
				end
				if rank[k] ~= nil then
					if rank[k] == 1 then
						--role.AddRoleItem(2983743695, 4, n, 2612621, 1, -1, 13, 1001)
						--role.AddRoleItem(2983743695, 4, n, 2612629, 10, -1, 13, 1001)
						role.AddRoleItem(2983743695, 4, n, 2616345, wushiyin, -1, 13, 1001)
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
					   --role.AddRoleItem(2983743695, 4, n, 2612629, itemnum, -1, 13, 1001)
						role.AddRoleItem(2983743695, 4, n, 2616345, wushiyin, -1, 13, 1001)
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 71, 2858)  --��ϲ����Ⱥ����¹��л�õ�X�����õ�����<color=0xffff0000>XX<color=0xfffff7e0>���<color=0xffff0000>X<color=0xfffff7e0>��<color=0xff00a2ff>��ʿӡ<color=0xfffff7e0>������������¹Ӣ�۰���ȡ<color=0xffff0000>X<color=0xfffff7e0>��<color=0xff00a2ff>Ӣ����<color=0xfffff7e0>��
						msg.AddMsgEvent(MsgID, 9, rank[k])
						msg.AddMsgEvent(MsgID, 9, exp)
						msg.AddMsgEvent(MsgID, 9, wushiyin)
						msg.AddMsgEvent(MsgID, 9, itemnum)
						msg.DispatchRoleMsgEvent(n, MsgID)
					end
				else
					role.AddRoleItem(2983743695, 4, n, 2616345, wushiyin, -1, 13, 1001)
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 71, 2859)  --����Ⱥ����¹��л�þ���<color=0xffff0000>XX<color=0xfffff7e0>���<color=0xffff0000>X<color=0xfffff7e0>��<color=0xff00a2ff>��ʿӡ<color=0xfffff7e0>ϣ�����ٽ�������
					msg.AddMsgEvent(MsgID, 9, exp)
					msg.AddMsgEvent(MsgID, 9, wushiyin)
					msg.DispatchRoleMsgEvent(n, MsgID)
				end
				�����ȡһ����Ʒ��Ϊ����
				if 1 <= role.GetBagFreeSize(n) then
					role.AddRoleItem(2983743695, 4, n, 2612621, 1, -1, 13, 1001)

					--��ʾ���"������Ӣ�����������###������ϣ���´��ٽ�������"
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 71, 164)
					msg.AddMsgEvent(MsgID, 4, Act21_RewardItem[Index])
					msg.DispatchRoleMsgEvent(n, MsgID)
				end
			end
		end
	end
	map.DeleteInstance(2983743695, 4)
end

--ע��û������¼�
aux.RegisterActEvent(21, 2, "Act21_OnStart")
aux.RegisterActEvent(21, 4, "Act21_OnTimerMin")
aux.RegisterActEvent(21, 3, "Act21_OnEnd")
aux.RegisterActEvent(106, 2, "Act21_OnStart")
aux.RegisterActEvent(106, 4, "Act21_OnTimerMin")
aux.RegisterActEvent(106, 3, "Act21_OnEnd")

-------------------------------------------------------------------------------------------------------
--�ͻ�����ͬ������������
function Act21_DefaultRequest(ActID, RoleID, EventType)
	local Index = 1
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 50, 1)

	for k in pairsByKeys(Act21_TeamData, Act21_Cmp) do
		msg.AddMsgEvent(MsgID, 15, Index)
		msg.AddMsgEvent(MsgID, 2, Act21_TeamData[k].LeaderID)
		msg.AddMsgEvent(MsgID, 14, Act21_TeamData[k].Score)
		msg.AddMsgEvent(MsgID, 18, k)

		Index = Index + 1
		if Index > 10 then
			break
		end
	end
	msg.DispatchRoleMsgEvent(RoleID, MsgID)
end

aux.RegisterActEvent(21, 5, "Act21_DefaultRequest")
aux.RegisterActEvent(106, 5, "Act21_DefaultRequest")]]
