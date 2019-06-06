--�̶������ǿʳ������


--ScriptData�и����ֶ�����˵��
--1 - 20Ϊǰ20����ҵ�ID��˳�������ν��д洢

--����������(����RoleID, KillPlayer, Score��IsDeadLeave, IsInMap, JoinTime)
Act14_RoleData = {} --������

--�̶���ѿ�ʼ��ʱ�䣨��λ�Ƿ��ӣ�
Act14_LastTime = 0

--�����
Act14_RewardItem = {1220005, 1230005, 1210003, 1420007, 1420011, 1420015, 1420019, 1420031, 1420035}

--���ˢ���ľ�������
Act14_CrystalNum = 0

--�㲥�������
Act14_BroadCast = 1

--ÿ�λ��ҿ�����5�λ�����10������
Act14_EXP10 = 5

PKAct14_killrole = {}  --��¼ÿ�����ɱ���ĵ��˵Ĵ���

PKAct_killexp = {}
PKAct_killexp[40] = 576
PKAct_killexp[41] = 664
PKAct_killexp[42] = 766
PKAct_killexp[43] = 870
PKAct_killexp[44] = 976
PKAct_killexp[45] = 1084
PKAct_killexp[46] = 1194
PKAct_killexp[47] = 1304
PKAct_killexp[48] = 1416
PKAct_killexp[49] = 1528
PKAct_killexp[50] = 1686
PKAct_killexp[51] = 1844
PKAct_killexp[52] = 2004
PKAct_killexp[53] = 2162
PKAct_killexp[54] = 2320
PKAct_killexp[55] = 2478
PKAct_killexp[56] = 2634
PKAct_killexp[57] = 2788
PKAct_killexp[58] = 2940
PKAct_killexp[59] = 3090
PKAct_killexp[60] = 3280
PKAct_killexp[61] = 3368
PKAct_killexp[62] = 3454
PKAct_killexp[63] = 3532
PKAct_killexp[64] = 3608
PKAct_killexp[65] = 3678
PKAct_killexp[66] = 3742
PKAct_killexp[67] = 3802
PKAct_killexp[68] = 3854
PKAct_killexp[69] = 3902
PKAct_killexp[70] = 3944
PKAct_killexp[71] = 3978
PKAct_killexp[72] = 4006
PKAct_killexp[73] = 4028
PKAct_killexp[74] = 4042
PKAct_killexp[75] = 4050
PKAct_killexp[76] = 4116
PKAct_killexp[77] = 4180
PKAct_killexp[78] = 4242
PKAct_killexp[79] = 4300
PKAct_killexp[80] = 4324
PKAct_killexp[81] = 4508
PKAct_killexp[82] = 4850
PKAct_killexp[83] = 5318
PKAct_killexp[84] = 5632
PKAct_killexp[85] = 5854
PKAct_killexp[86] = 6358
PKAct_killexp[87] = 6720
PKAct_killexp[88] = 6890
PKAct_killexp[89] = 6998
PKAct_killexp[90] = 7230
PKAct_killexp[91] = 7922
PKAct_killexp[92] = 8656
PKAct_killexp[93] = 9094
PKAct_killexp[94] = 9282
PKAct_killexp[95] = 9434
PKAct_killexp[96] = 9630
PKAct_killexp[97] = 9848
PKAct_killexp[98] = 10666
PKAct_killexp[99] = 11498
PKAct_killexp[100] = 12336
PKAct_killexp[101] = 13426
PKAct_killexp[102] = 14732
PKAct_killexp[103] = 15908
PKAct_killexp[104] = 17223
PKAct_killexp[105] = 18463
PKAct_killexp[106] = 19763
PKAct_killexp[107] = 21347
PKAct_killexp[108] = 23104
PKAct_killexp[109] = 25007
PKAct_killexp[110] = 27000



-------------------------------------------------------------------------------------------------------
--����ȽϺ���
local function Act14_Cmp(a, b)
	if Act14_RoleData[b].Score < Act14_RoleData[a].Score then
		return true
	elseif Act14_RoleData[b].Score == Act14_RoleData[a].Score and Act14_RoleData[b].KillPlayer < Act14_RoleData[a].KillPlayer then
		return true
	elseif Act14_RoleData[b].Score == Act14_RoleData[a].Score and Act14_RoleData[b].KillPlayer == Act14_RoleData[a].KillPlayer and Act14_RoleData[b].JoinTime < Act14_RoleData[a].JoinTime then
		return true
	else
		return false
    end
end

--------------------------------------------------------------------------------------------------------
local function PlayerEnter(MapID, InstanceID, RoleID, Act_RoleData)
    if Act_RoleData[RoleID] then
		Act_RoleData[RoleID].IsInMap = true

		--���������������뿪�
		--if Act_RoleData[RoleID].IsDeadLeave then
	    if Act14_RoleData[RoleID] then
		    local rolelevel = role.GetRoleLevel(2983744463, 1, RoleID)
			if rolelevel <= 59 then
				--�۳����2000��
				role.DecRoleSilver(MapID, InstanceID, RoleID, 2000, 1001)
			elseif rolelevel <= 79 then
			    role.DecRoleSilver(MapID, InstanceID, RoleID, 5000, 1001)
			elseif rolelevel <= 99 then
	            role.DecRoleSilver(MapID, InstanceID, RoleID, 10000, 1001)
			else
			    role.DecRoleSilver(MapID, InstanceID, RoleID, 20000, 1001)
		    end
		end
		--[[if Act15_RoleData[RoleID] then
			--�۳����5000��
			role.DecRoleSilver(MapID, InstanceID, RoleID, 5000, 1001)
		end
		if Act16_RoleData[RoleID] then
			--�۳����10000��
			role.DecRoleSilver(MapID, InstanceID, RoleID, 10000, 1001)
		end
		if Act17_RoleData[RoleID] then
			--�۳����20000��
			role.DecRoleSilver(MapID, InstanceID, RoleID, 20000, 1001)
		end
			--Act_RoleData[RoleID].IsDeadLeave = false
		--end]]
	else
		--��һ�ν��룬����û��RoleData��
--		local RoleDataTemp = {KillPlayer=3, Score=0, IsDeadLeave=false, IsInMap=true, JoinTime=0}
--		table.insert(Act_RoleData, RoleID, RoleDataTemp)
        Act_RoleData[RoleID] = {KillPlayer=0, Score=0, Score2=0, IsDeadLeave=false, IsInMap=true, JoinTime=0, KillAnimal=0}
		--if InstanceID == 3 then
		--    Act16_RoleNum = Act16_RoleNum + 1
		--end
		--if InstanceID == 4 then
		--    Act17_RoleNum = Act17_RoleNum + 1
		--end
	end

	--�������Ϊ����̨״̬(���ڿͻ��˵����жϣ�
	role.SetRoleState(RoleID, 8, true)
end

-------------------------------------------------------------------------------------------------------

--�洢��ǰ����
function SaveAct14RoleData()
    local Index = 1
    for k in pairsByKeys(Act14_RoleData, Act14_Cmp) do
        act.SetActScriptData(14, 1, Index, k)
        Index = Index + 1
        if Index > 20 then
            break
        end
    end
end

--------------------------------------------------------------------------------------------------------
--�̶����ʼ
function act14_OnStart(ActID)
	--���������
	map.CreateInstance(2983744463, 1)

	--��ʼ����ѽ���ʱ��
	Act14_LastTime = 0
	Act14_BroadCast = 1
	Act14_EXP10 = 5
	--��ʼ��ˮ��������
	--Act14_CrystalNum = 0

	--��ʼ������������
	local Empty = {}
	Act14_RoleData = Empty

	--[[for k in pairs(m21_SpawnPos) do
		m21_SpawnPos[k].Crystal40 = false
		m21_SpawnPos[k].Crystal60 = false
		m21_SpawnPos[k].Crystal80 = false
		m21_SpawnPos[k].Crystal100 = false
	end]]
end

function act14_OnTimerMin(ActID)
	--�ۼӻ����ʱ��
	Act14_LastTime = Act14_LastTime + 1

	--���ʼ��ĵ�5������ʱ�䣨��5��10����50��55���ӣ�, ������һ���+5
	    if Act14_LastTime % 5 == 0 and Act14_LastTime <= 15 then
			--֪ͨȫ���������ʼ
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 102, 100107)    --����ǿʳ����ڰ˵�����ʼ����ʱ���·Ӣ�����������ǻ�Ƶ۳��ҵ�����ǿʳʹ�߽���������
			msg.DispatchBroadcast(MsgID,-1,-1,-1)
	    end
		for k in pairs(Act14_RoleData) do
			--����ڻ��ͼ��
			if Act14_RoleData[k].IsInMap then
			    Act14_RoleData[k].Score = Act14_RoleData[k].Score + 33
				--ͬ���ͻ��˻���
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 13,	1)
				msg.AddMsgEvent(MsgID, 1, 2851)			--�ʱ����ֽ�������������33��
				msg.DispatchRoleMsgEvent(k, MsgID)
			    if map.IsInArea(2983744463, 1, k, 636)	then--����Ƿ��ڻ�Ծ����
					--���û������
					if false == unit.IsDead(2983744463, 1, k) then
						Act14_RoleData[k].Score = Act14_RoleData[k].Score + 33
						Act14_RoleData[k].Score2 = Act14_RoleData[k].Score2 + 33
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13,	1)
						msg.AddMsgEvent(MsgID, 1, 2852)			--���ڻ�Ծ���򣬻��ֽ���������
						msg.DispatchRoleMsgEvent(k, MsgID)
					end
				end
				--ͬ���ͻ��˻���
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 60,	153)								--������
				msg.AddMsgEvent(MsgID, 14,	Act14_RoleData[k].Score)			--����
				msg.DispatchRoleMsgEvent(k, MsgID)
			end
		end
    if Act14_LastTime % 2 == 0 then
	    --if ruorouqiangshi_jingqiu == 0 then
			--ruorouqiangshi_jingqiu = 1
			local Index = math.random(1,20)
			map.MapCreateCreature(2983744463, 1, 1534154, m21_SpawnPos[Index].x, m21_SpawnPos[Index].y, m21_SpawnPos[Index].z)
            local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 101, 2867)    --XXX�����������򣬻�������������ǿ��״̬��
			msg.AddMsgEvent(MsgID, 9, m21_SpawnPos[Index].x)
			msg.AddMsgEvent(MsgID, 9, m21_SpawnPos[Index].z)
			msg.DispatchMapMsgEvent(MsgID, 2983744463, 1)
		--end
	end
	if Act14_LastTime % 5 == 0 then
        Act14_BroadCast = 1
	end
end

--�����
function act14_OnEnd(actID)
	SaveAct14RoleData()
	PKAct14_killrole = {}
    local rank ={}
	for i=1,10 do
	    if rank[act.GetActScriptData(14, 1, i)] == nil then
	        rank[act.GetActScriptData(14, 1, i)] = i
		end
	end

	--�����ȡһ����������д������
	for n in pairs(Act14_RoleData) do
		if Act14_RoleData[n].IsInMap then
		    local AddExp = role.GetRoleLevelUpExp(2983744463, 1, n)
			local rolelevel = role.GetRoleLevel(2983744463, 1, n)
			local Scoreact = Act14_RoleData[n].Score
			if Scoreact >3000 then
			    Scoreact = 3000
			end
			local exp = activity_newexp[rolelevel] * Scoreact / 3000
			role.AddRoleExp(2983744463, 1, n, exp)
			local wushiyin = 0
			if Act14_RoleData[n].Score2 >= 1500 then
                wushiyin = 30
            elseif Act14_RoleData[n].Score2 >= 1000 then
			    wushiyin = 20
			elseif Act14_RoleData[n].Score2 >= 500 then
			    wushiyin = 10
			elseif Act14_RoleData[n].Score2 >= 250 then
			    wushiyin = 5
			elseif Act14_RoleData[n].Score2 >= 150 then
			    wushiyin = 4
			elseif Act14_RoleData[n].Score2 >= 100 then
			    wushiyin = 3
			elseif Act14_RoleData[n].Score2 >= 50 then
			    wushiyin = 2
			else
			    wushiyin = 1
			end
			if rank[n] ~= nil then
			    if rank[n] == 1 then
				    --role.AddRoleItem(2983744463, 1, n, 2612621, 1, -1, 13, 1001)
					--role.AddRoleItem(2983744463, 1, n, 2612629, 10, -1, 13, 1001)
					role.AddRoleItem(2983744463, 1, n, 2616345, wushiyin, -1, 13, 1001)
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
				    role.AddRoleItem(2983744463, 1, n, 2616345, wushiyin, -1, 13, 1001)
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 71, 2855)  --��ϲ����Ⱥ�������л�õ�X�����õ�����<color=0xffff0000>XX<color=0xfffff7e0>���<color=0xffff0000>X<color=0xfffff7e0>��<color=0xff00a2ff>��ʿӡ<color=0xfffff7e0>������������¹Ӣ�۰���ȡ<color=0xffff0000>X<color=0xfffff7e0>��<color=0xff00a2ff>Ӣ����<color=0xfffff7e0>��
					msg.AddMsgEvent(MsgID, 9, rank[n])
					msg.AddMsgEvent(MsgID, 9, exp)
					msg.AddMsgEvent(MsgID, 9, wushiyin)
					msg.AddMsgEvent(MsgID, 9, itemnum)
					msg.DispatchRoleMsgEvent(n, MsgID)
				end
			else
			    role.AddRoleItem(2983744463, 1, n, 2616345, wushiyin, -1, 13, 1001)
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 71, 2856)  --����Ⱥ�������л�þ���<color=0xffff0000>XX<color=0xfffff7e0>���<color=0xffff0000>X<color=0xfffff7e0>��<color=0xff00a2ff>��ʿӡ<color=0xfffff7e0>ϣ�����ٽ�������
				msg.AddMsgEvent(MsgID, 9, exp)
				msg.AddMsgEvent(MsgID, 9, wushiyin)
				msg.DispatchRoleMsgEvent(n, MsgID)
			end
			--[[�����ȡһ����Ʒ��Ϊ����
			if 1 <= role.GetBagFreeSize(n) then
			    role.AddRoleItem(2983744463, 1, n, 2612621, 1, -1, 13, 1001)

			    --��ʾ���"����������ǿʳ������###������ϣ���´��ٽ�������"
	            local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 71, 164)
	            msg.AddMsgEvent(MsgID, 4, Act14_RewardItem[Index])
	            msg.DispatchRoleMsgEvent(n, MsgID)
	        end]]
		end
	end

	--ɾ�������
	map.DeleteInstance(2983744463, 1)
end

--��й㲥(State = 0-������ʼ 1-��ʼ 2-�������� 3-���� )
function act14_ActivityBroad(State)
    if PVPactivity_trigger ~= 1 then
	    return
	end
    --�������ʼ
    if State == 0 then
        --֪ͨȫ���������ʼ
	    local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 102, 100107)    --����ǿʳ����ڰ˵�����ʼ����ʱ���·Ӣ�����������ǻ�Ƶ۳��ҵ�����ǿʳʹ�߽���������
		msg.DispatchBroadcast(MsgID,-1,-1,-1)
	elseif State == 1 then
	    --֪ͨȫ���������ʼ
	    local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 102, 100133)    --����ǿʳ����ڰ˵�����ʼ����ʱ���·Ӣ�����������ǻ�Ƶ۳��ҵ�����ǿʳʹ�߽���������
		msg.DispatchBroadcast(MsgID,-1,-1,-1)
    end
end

--ע��û������¼�
aux.RegisterActEvent(14, 2, "act14_OnStart")
aux.RegisterActEvent(14, 4, "act14_OnTimerMin")
aux.RegisterActEvent(14, 3, "act14_OnEnd")
aux.RegisterActEvent(97, 2, "act14_OnStart")
aux.RegisterActEvent(97, 4, "act14_OnTimerMin")
aux.RegisterActEvent(97, 3, "act14_OnEnd")
aux.RegisterActEvent(14, 6, "act14_ActivityBroad")
aux.RegisterActEvent(97, 6, "act14_ActivityBroad")
aux.RegisterActEvent(18, 2, "act14_OnStart")
aux.RegisterActEvent(18, 4, "act14_OnTimerMin")
aux.RegisterActEvent(18, 3, "act14_OnEnd")
aux.RegisterActEvent(18, 6, "act14_ActivityBroad")
aux.RegisterActEvent(103, 2, "act14_OnStart")
aux.RegisterActEvent(103, 4, "act14_OnTimerMin")
aux.RegisterActEvent(103, 3, "act14_OnEnd")
aux.RegisterActEvent(103, 6, "act14_ActivityBroad")
--------------------------------------------------------------------------------------------------------
--todo��ҽ�������
function m21_PlayerEnter(MapID, InstanceID, RoleID)
	local Min  		= tonumber(os.date("%M"))
	local Second	= tonumber(os.date("%S"))
	local leftSec 	= (29 - Min) * 60 + 59 - Second		--�ʣ��ʱ��
    --�������ΪPK״̬(���ڿͻ��˵����жϣ�
	role.SetRoleState(RoleID, 8, false)
	--������
	if InstanceID == 1 then
        PlayerEnter(MapID, InstanceID, RoleID, Act14_RoleData)
		--ͬ���ͻ��˻����
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 60,	153)								--������
		msg.AddMsgEvent(MsgID, 25,	14)									--�ID
		msg.AddMsgEvent(MsgID, 14,	Act14_RoleData[RoleID].Score)		--����
		msg.AddMsgEvent(MsgID, 17,	Act14_RoleData[RoleID].KillPlayer - Act14_RoleData[RoleID].KillAnimal)	--ɱ����
		msg.AddMsgEvent(MsgID, 16,	leftSec)							--ʣ��ʱ��
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	end

	--[[������
	if InstanceID == 2 then
        PlayerEnter(MapID, InstanceID, RoleID, Act15_RoleData)
		--ͬ���ͻ��˻����
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 60,	154)								--������
		msg.AddMsgEvent(MsgID, 25,	15)									--�ID
		msg.AddMsgEvent(MsgID, 14,	Act15_RoleData[RoleID].Score)		--����
		msg.AddMsgEvent(MsgID, 17,	Act15_RoleData[RoleID].KillPlayer)	--ɱ����
		msg.AddMsgEvent(MsgID, 16,	leftSec)							--ʣ��ʱ��
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	end

	--Ӣ����
	if InstanceID == 3 then
        PlayerEnter(MapID, InstanceID, RoleID, Act16_RoleData)
		--ͬ���ͻ��˻����
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 60,	155)								--������
		msg.AddMsgEvent(MsgID, 25,	16)									--�ID
		msg.AddMsgEvent(MsgID, 14,	Act16_RoleData[RoleID].Score)		--����
		msg.AddMsgEvent(MsgID, 17,	Act16_RoleData[RoleID].KillPlayer)	--ɱ����
		msg.AddMsgEvent(MsgID, 16,	leftSec)							--ʣ��ʱ��
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end

	--������
	if InstanceID == 4 then
        PlayerEnter(MapID, InstanceID, RoleID, Act17_RoleData)
		--ͬ���ͻ��˻����
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 60,	156)								--������
		msg.AddMsgEvent(MsgID, 25,	17)									--�ID
		msg.AddMsgEvent(MsgID, 14,	Act17_RoleData[RoleID].Score)		--����
		msg.AddMsgEvent(MsgID, 17,	Act17_RoleData[RoleID].KillPlayer)	--ɱ����
		msg.AddMsgEvent(MsgID, 16,	leftSec)							--ʣ��ʱ��
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	end]]
end

--ע�������ͼ�¼�
aux.RegisterMapEvent("m21", 2, "m21_PlayerEnter")

-------------------------------------------------------------------------------------------------------
--����뿪�����
function m21_PlayerLeave(MapID, InstanceID, RoleID)
	--��������뿪����̨״̬(���ڿͻ��˵����жϣ�
	role.UnSetRoleState(RoleID, 8, true)
	--ͬ���ͻ��˹رջ����
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 60, -1)								--�رջ����
	msg.DispatchRoleMsgEvent(RoleID, MsgID)

	--������
	if InstanceID == 1 then
		Act14_RoleData[RoleID].IsInMap = false
	end

	--[[������
	if InstanceID == 2 then
		Act15_RoleData[RoleID].IsInMap = false
	end

	--Ӣ����
	if InstanceID == 3 then
		Act16_RoleData[RoleID].IsInMap = false
	end

	--������
	if InstanceID == 4 then
		Act17_RoleData[RoleID].IsInMap = false
	end]]
end

--ע���뿪���ͼ�¼�
aux.RegisterMapEvent("m21", 3, "m21_PlayerLeave")

-------------------------------------------------------------------------------------------------------
--����ڻ�и���
function m21_RoleRevive(MapID, InstanceID, RoleID, ReviveType, ReviveHP, ReviveMP, fx, fy, fz, RebornMapID)
	--���ѡ��سǸ���
	if ReviveType == 0 then
		ReviveHP = role.GetRoleAttValue(MapID, InstanceID, RoleID, 8)
		ReviveMP = role.GetRoleAttValue(MapID, InstanceID, RoleID, 10)


		if InstanceID == 1 then
			--[[if Act14_RoleData[RoleID].KillPlayer == 0 then
				fx = 2156
				fy = 18363
				fz = 2420
				RebornMapID = 3017299663

				--�������Ϊ�������뿪
				Act14_RoleData[RoleID].IsDeadLeave = true
			else]]
				--����õ�һ�����������
				local Index = math.random(1, 8)
				fx = m21_BornPos[Index].x
				fy = m21_BornPos[Index].y
				fz = m21_BornPos[Index].z
				RebornMapID = 2983744463

				--[[���������1
				Act14_RoleData[RoleID].KillPlayer = Act14_RoleData[RoleID].KillPlayer - 1

				--ͬ���ͻ��˸������
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 60,	153)								--������
				msg.AddMsgEvent(MsgID, 17,	Act14_RoleData[RoleID].KillPlayer)	--�������
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			end
		end

		-������
		if InstanceID == 2 then
			--if Act15_RoleData[RoleID].KillPlayer == 0 then
				fx = 2156
				fy = 18363
				fz = 2420
				RebornMapID = 3017299663

				--�������Ϊ�������뿪
				Act15_RoleData[RoleID].IsDeadLeave = true
			else
				--����õ�һ�����������
				local Index = math.random(1, 8)
				fx = m21_BornPos[Index].x
				fy = m21_BornPos[Index].y
				fz = m21_BornPos[Index].z
				RebornMapID = 2983744463

				--���������1
				Act15_RoleData[RoleID].KillPlayer = Act15_RoleData[RoleID].KillPlayer - 1

				--ͬ���ͻ��˸������
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 60,	154)								--������
				msg.AddMsgEvent(MsgID, 17,	Act15_RoleData[RoleID].KillPlayer)	--�������
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			end
		end

		--Ӣ����
		if InstanceID == 3 then
			--if Act16_RoleData[RoleID].KillPlayer == 0 then
				fx = 2156
				fy = 18363
				fz = 2420
				RebornMapID = 3017299663

				--�������Ϊ�������뿪
				Act16_RoleData[RoleID].IsDeadLeave = true
			else
				--����õ�һ�����������
				local Index = math.random(1, 8)
				fx = m21_BornPos[Index].x
				fy = m21_BornPos[Index].y
				fz = m21_BornPos[Index].z
				RebornMapID = 2983744463

				--���������1
				Act16_RoleData[RoleID].KillPlayer = Act16_RoleData[RoleID].KillPlayer - 1

				--ͬ���ͻ��˸������
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 60,	155)								--������
				msg.AddMsgEvent(MsgID, 17,	Act16_RoleData[RoleID].KillPlayer)	--�������
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			end
		end

		--������
		if InstanceID == 4 then
			--if Act17_RoleData[RoleID].KillPlayer == 0 then
				fx = 2156
				fy = 18363
				fz = 2420
				RebornMapID = 3017299663

				--�������Ϊ�������뿪
				Act17_RoleData[RoleID].IsDeadLeave = true
			else
				--����õ�һ�����������
				local Index = math.random(1, 8)
				fx = m21_BornPos[Index].x
				fy = m21_BornPos[Index].y
				fz = m21_BornPos[Index].z
				RebornMapID = 2983744463

				--���������1
				Act17_RoleData[RoleID].KillPlayer = Act17_RoleData[RoleID].KillPlayer - 1

				--ͬ���ͻ��˸������
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 60,	156)								--������
				msg.AddMsgEvent(MsgID, 17,	Act17_RoleData[RoleID].KillPlayer)	--�������
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			end]]
		end
	end
    unit.AddBuff(MapID, InstanceID, RoleID, 9999001, RoleID)
	return ReviveHP, ReviveMP, fx, fy, fz, RebornMapID
end

aux.RegisterMapEvent("m21",  11, "m21_RoleRevive")

-------------------------------------------------------------------------------------------------------
--����������
function m21_RoleDie(MapID, InstanceID, RoleID, KillerID)
	if unit.IsPlayer(KillerID) then
		--��ɱһ����ң���û������ӣ�������=10��[1-������ȼ�-Ŀ��ȼ���/10]����Сȡ1
		local RoleLevel = role.GetRoleLevel(MapID, InstanceID, RoleID)
		if RoleLevel < 40 then             --��ֹ�ȼ�������ʱ����
		    RoleLevel = 40
		elseif RoleLevel > 100 then
		    RoleLevel = 100
		end
		local KillerLevel = role.GetRoleLevel(MapID, InstanceID, KillerID)
		local Score = 0         --30*(1-(KillerLevel - RoleLevel)/10)
		--local PunishScore = 1
		if KillerLevel > RoleLevel then
			Score = 10
		else
		    Score = 10 + RoleLevel - KillerLevel
		end

		--������
		if InstanceID == 1 then
		    --[[���15����
		    if Act14_LastTime > 15 then
		    	--��ɱ��������ǰ10������ң���û�����������ͨ״̬��2��
		    	local Index = 0
		    	for k in pairsByKeys(Act14_RoleData, Act14_Cmp) do
		    		if k == RoleID then
		    			Score = Score*2
		    			--PunishScore = 5
		    			break
		    		end

		    		if Index > 10 then
		    			break
		    		end

	    			Index = Index + 1
	    		end
	    	end]]
			--���ӻ�ɱ��
			Act14_RoleData[KillerID].KillPlayer = Act14_RoleData[KillerID].KillPlayer + 1
			--[[if (Act14_RoleData[KillerID].KillPlayer - Act14_RoleData[KillerID].KillAnimal) % 10 == 0 and Act14_RoleData[KillerID].KillPlayer - Act14_RoleData[KillerID].KillAnimal ~= 0 then
				--֪ͨȫ���������ʼ
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 102, 100108)    --��ս����XXX�²��ɵ������X��ն��
				msg.AddMsgEvent(MsgID, 2, KillerID)
				msg.AddMsgEvent(MsgID, 9, Act14_RoleData[KillerID].KillPlayer)
				msg.DispatchBroadcast(MsgID, MapID, InstanceID,-1)
			end]]
			--�����ɱ���齱��
			local killexp = PKAct_killexp[RoleLevel]
			if Act14_RoleData[KillerID].KillPlayer > 60 then
			    local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 13, 0)
				msg.AddMsgEvent(MsgID, 1, 2868)             --ÿ������ǿʳ���ֻ�ܻ��100�λ�ɱ����
				msg.DispatchRoleMsgEvent(KillerID, MsgID)
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 26, 2868)             --ÿ������ǿʳ���ֻ�ܻ��100�λ�ɱ����
				msg.DispatchRoleMsgEvent(KillerID, MsgID)
			    killexp = 0
			end
			local bool3 = true
            if PKAct14_killrole[KillerID] == nil then
			    PKAct14_killrole[KillerID] = {}
				PKAct14_killrole[KillerID][1] = RoleID
				PKAct14_killrole[KillerID][4] = 1
			else
				for i=1,3 do
					if PKAct14_killrole[KillerID][i] ~= nil and PKAct14_killrole[KillerID][i] == RoleID then
					    bool3 = false
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13, 0)
						msg.AddMsgEvent(MsgID, 1, 2869)             --��ɱ�ո�ɱ����3����Ҳ���������û�ɱ����ͻ���
						msg.DispatchRoleMsgEvent(KillerID, MsgID)
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 2869)             --��ɱ�ո�ɱ����3����Ҳ���������û�ɱ����ͻ���
						msg.DispatchRoleMsgEvent(KillerID, MsgID)
						break
					end
				end
                if 	bool3 == true then
                    if 	PKAct14_killrole[KillerID][2] == nil then
						PKAct14_killrole[KillerID][2] = RoleID
						PKAct14_killrole[KillerID][4] = 2
					elseif PKAct14_killrole[KillerID][3] == nil then
						PKAct14_killrole[KillerID][3] = RoleID
						PKAct14_killrole[KillerID][4] = 3
					else
					    PKAct14_killrole[KillerID][4] = PKAct14_killrole[KillerID][4] + 1
						PKAct14_killrole[KillerID][3 - PKAct14_killrole[KillerID][4] % 3] = RoleID
					end
				end
			end
			if bool3 == true then
			    if KillerLevel < RoleLevel then
					killexp = math.floor(((RoleLevel-KillerLevel)/10/7*5 + 1)*killexp)
			    end
				role.AddRoleExp(2983744463, 1, KillerID, killexp)
			    --Score = math.floor(Score*(10-PKAct14_killrole[KillerID][RoleID])/10)
			else
			    Score = 0
			end
			--���ӻ�ɱ�߻���
			Act14_RoleData[KillerID].Score = Act14_RoleData[KillerID].Score + Score
			Act14_RoleData[KillerID].Score2 = Act14_RoleData[KillerID].Score2 + Score
			if bool3 == true then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 101, 2860)           --��ɱXXX����þ���XXX�㣬����XXX��
				msg.AddMsgEvent(MsgID, 2, RoleID)
				msg.AddMsgEvent(MsgID, 9, killexp)
				msg.AddMsgEvent(MsgID, 9, Score)
				msg.DispatchRoleMsgEvent(KillerID, MsgID)
			end
			--ͬ���ͻ��˻���
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 60,	153)								--������
			msg.AddMsgEvent(MsgID, 14,	Act14_RoleData[KillerID].Score)		--����
			msg.AddMsgEvent(MsgID, 17,	Act14_RoleData[KillerID].KillPlayer - Act14_RoleData[KillerID].KillAnimal)
			msg.DispatchRoleMsgEvent(KillerID, MsgID)

			if RoleLevel - KillerLevel > 30 and Act14_BroadCast == 1  and killexp > 0 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 102, 100135)
				msg.AddMsgEvent(MsgID, 2, KillerID)
				msg.AddMsgEvent(MsgID, 9, RoleLevel - KillerLevel)
				msg.AddMsgEvent(MsgID, 9, killexp)
				msg.DispatchBroadcast(MsgID,-1,-1,-1)
				Act14_BroadCast = 0
		    end

			if Act14_RoleData[KillerID].KillPlayer - Act14_RoleData[KillerID].KillAnimal == 50 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 102, 100136)
				msg.AddMsgEvent(MsgID, 2, KillerID)
				msg.DispatchBroadcast(MsgID, MapID, InstanceID,-1)
		    end

			--���������߻���
			--Act14_RoleData[RoleID].Score = Act14_RoleData[RoleID].Score - PunishScore
			--ͬ���ͻ��˻���
			--local MsgID = msg.BeginMsgEvent()
			--msg.AddMsgEvent(MsgID, 60,	153)								--������
			--msg.AddMsgEvent(MsgID, 14,	Act14_RoleData[RoleID].Score)		--����
			--msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end

		--[[������
		if InstanceID == 2 then
		    --���15����
		    if Act15_LastTime > 15 then
		    	--��ɱ��������ǰ10������ң���û�����������ͨ״̬��2��
		    	local Index = 0
		    	for k in pairsByKeys(Act15_RoleData, Act15_Cmp) do
		    		if k == RoleID then
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
			Act15_RoleData[KillerID].KillPlayer = Act15_RoleData[KillerID].KillPlayer + 1
			if Act15_RoleData[KillerID].KillPlayer % 10 == 0 and Act15_RoleData[KillerID].KillPlayer ~= 0 then
				--֪ͨȫ���������ʼ
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 102, 100108)    --��ս����XXX�²��ɵ������X��ն��
				msg.AddMsgEvent(MsgID, 2, KillerID)
				msg.AddMsgEvent(MsgID, 9, Act15_RoleData[KillerID].KillPlayer)
				msg.DispatchBroadcast(MsgID, MapID, InstanceID,-1)
			end
			--�����ɱ���齱��
			local killexp = PKAct_killexp[RoleLevel]
            if PKAct15_killrole[KillerID] == nil then
			    PKAct15_killrole[KillerID] = {}
				if PKAct15_killrole[KillerID][RoleID] == nil then
				    PKAct15_killrole[KillerID][RoleID] = 0
				else
				    PKAct15_killrole[KillerID][RoleID] = PKAct15_killrole[KillerID][RoleID] + 1
				end
			else
			    if PKAct15_killrole[KillerID][RoleID] == nil then
				    PKAct15_killrole[KillerID][RoleID] = 0
				else
				    PKAct15_killrole[KillerID][RoleID] = PKAct15_killrole[KillerID][RoleID] + 1
				end
			end
			if PKAct15_killrole[KillerID][RoleID] < 10 then
			    role.AddRoleExp(2983744463, 2, KillerID, math.floor(killexp*(10-PKAct15_killrole[KillerID][RoleID])/10))
			    Score = math.floor(Score*(10-PKAct15_killrole[KillerID][RoleID])/10)
			else
			    Score = 0
			end
			--���ӻ�ɱ�߻���
			Act15_RoleData[KillerID].Score = Act14_RoleData[KillerID].Score + Score
			if PKAct15_killrole[KillerID][RoleID] < 10 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 101, 2860)           --��ɱXXX����þ���XXX�㣬����XXX��
				msg.AddMsgEvent(MsgID, 2, RoleID)
				msg.AddMsgEvent(MsgID, 9, math.floor(killexp*(10-PKAct15_killrole[KillerID][RoleID])/10))
				msg.AddMsgEvent(MsgID, 9, Score)
				msg.DispatchRoleMsgEvent(KillerID, MsgID)
			end
			--ͬ���ͻ��˻���
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 60,	154)								--������
			msg.AddMsgEvent(MsgID, 14,	Act15_RoleData[KillerID].Score)		--����
			msg.AddMsgEvent(MsgID, 17,	Act15_RoleData[KillerID].KillPlayer)
			msg.DispatchRoleMsgEvent(KillerID, MsgID)

			--���������߻���
			Act15_RoleData[RoleID].Score = Act15_RoleData[RoleID].Score - PunishScore
			--ͬ���ͻ��˻���
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 60,	155)								--Ӣ����
			msg.AddMsgEvent(MsgID, 14,	Act16_RoleData[RoleID].Score)		--����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end

		--Ӣ����
		if InstanceID == 3 then
		    --���15����
		    if Act16_LastTime > 15 then
		    	--��ɱ��������ǰ10������ң���û�����������ͨ״̬��2��
		    	local Index = 0
		    	for k in pairsByKeys(Act16_RoleData, Act16_Cmp) do
		    		if k == RoleID then
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
			Act16_RoleData[KillerID].KillPlayer = Act16_RoleData[KillerID].KillPlayer + 1
			if Act16_RoleData[KillerID].KillPlayer % 10 == 0 and Act16_RoleData[KillerID].KillPlayer ~= 0 then
				--֪ͨȫ���������ʼ
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 102, 100108)    --��ս����XXX�²��ɵ������X��ն��
				msg.AddMsgEvent(MsgID, 2, KillerID)
				msg.AddMsgEvent(MsgID, 9, Act16_RoleData[KillerID].KillPlayer)
				msg.DispatchBroadcast(MsgID, MapID, InstanceID,-1)
			end
			--�����ɱ���齱��
			local killexp = PKAct_killexp[RoleLevel]
            if PKAct16_killrole[KillerID] == nil then
			    PKAct16_killrole[KillerID] = {}
				if PKAct16_killrole[KillerID][RoleID] == nil then
				    PKAct16_killrole[KillerID][RoleID] = 0
				else
				    PKAct16_killrole[KillerID][RoleID] = PKAct16_killrole[KillerID][RoleID] + 1
				end
			else
			    if PKAct16_killrole[KillerID][RoleID] == nil then
				    PKAct16_killrole[KillerID][RoleID] = 0
				else
				    PKAct16_killrole[KillerID][RoleID] = PKAct16_killrole[KillerID][RoleID] + 1
				end
			end
			if PKAct16_killrole[KillerID][RoleID] < 10 then
			    role.AddRoleExp(2983744463, 3, KillerID, math.floor(killexp*(10-PKAct16_killrole[KillerID][RoleID])/10))
			    Score = math.floor(Score*(10-PKAct16_killrole[KillerID][RoleID])/10)
			else
			    Score = 0
			end
			--���ӻ�ɱ�߻���
			Act16_RoleData[KillerID].Score = Act16_RoleData[KillerID].Score + Score
			if PKAct16_killrole[KillerID][RoleID] < 10 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 101, 2860)           --��ɱXXX����þ���XXX�㣬����XXX��
				msg.AddMsgEvent(MsgID, 2, RoleID)
				msg.AddMsgEvent(MsgID, 9, math.floor(killexp*(10-PKAct16_killrole[KillerID][RoleID])/10))
				msg.AddMsgEvent(MsgID, 9, Score)
				msg.DispatchRoleMsgEvent(KillerID, MsgID)
			end
			--ͬ���ͻ��˻���
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 60,	155)								--������
			msg.AddMsgEvent(MsgID, 14,	Act16_RoleData[KillerID].Score)		--����
			msg.AddMsgEvent(MsgID, 17,	Act16_RoleData[KillerID].KillPlayer)
			msg.DispatchRoleMsgEvent(KillerID, MsgID)

			--���������߻���
			Act16_RoleData[RoleID].Score = Act16_RoleData[RoleID].Score - PunishScore
			--ͬ���ͻ��˻���
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 60,	155)								--Ӣ����
			msg.AddMsgEvent(MsgID, 14,	Act16_RoleData[RoleID].Score)		--����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end

		--������
		if InstanceID == 4 then
		    --���15����
		    if Act17_LastTime > 15 then
		    	--��ɱ��������ǰ10������ң���û�����������ͨ״̬��2��
		    	local Index = 0
		    	for k in pairsByKeys(Act17_RoleData, Act17_Cmp) do
		    		if k == RoleID then
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
			Act17_RoleData[KillerID].KillPlayer = Act17_RoleData[KillerID].KillPlayer + 1
			if Act17_RoleData[KillerID].KillPlayer % 10 == 0 and Act17_RoleData[KillerID].KillPlayer ~= 0 then
				--֪ͨȫ���������ʼ
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 102, 100108)    --��ս����XXX�²��ɵ������X��ն��
				msg.AddMsgEvent(MsgID, 2, KillerID)
				msg.AddMsgEvent(MsgID, 9, Act17_RoleData[KillerID].KillPlayer)
				msg.DispatchBroadcast(MsgID, MapID, InstanceID,-1)
			end
			--�����ɱ���齱��
			local killexp = PKAct_killexp[RoleLeve]
            if PKAct17_killrole[KillerID] == nil then
			    PKAct17_killrole[KillerID] = {}
				if PKAct17_killrole[KillerID][RoleID] == nil then
				    PKAct17_killrole[KillerID][RoleID] = 0
				else
				    PKAct17_killrole[KillerID][RoleID] = PKAct17_killrole[KillerID][RoleID] + 1
				end
			else
			    if PKAct17_killrole[KillerID][RoleID] == nil then
				    PKAct17_killrole[KillerID][RoleID] = 0
				else
				    PKAct17_killrole[KillerID][RoleID] = PKAct17_killrole[KillerID][RoleID] + 1
				end
			end
			if PKAct17_killrole[KillerID][RoleID] < 10 then
			    role.AddRoleExp(2983744463, 4, KillerID, math.floor(killexp*(10-PKAct17_killrole[KillerID][RoleID])/10))
			    Score = math.floor(Score*(10-PKAct17_killrole[KillerID][RoleID])/10)
			else
			    Score = 0
			end
			--���ӻ�ɱ�߻���
			Act17_RoleData[KillerID].Score = Act17_RoleData[KillerID].Score + Score
			if PKAct17_killrole[KillerID][RoleID] < 10 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 101, 2860)           --��ɱXXX����þ���XXX�㣬����XXX��
				msg.AddMsgEvent(MsgID, 2, RoleID)
				msg.AddMsgEvent(MsgID, 9, math.floor(killexp*(10-PKAct17_killrole[KillerID][RoleID])/10))
				msg.AddMsgEvent(MsgID, 9, Score)
				msg.DispatchRoleMsgEvent(KillerID, MsgID)
			end
			--ͬ���ͻ��˻���
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 60,	156)								--������
			msg.AddMsgEvent(MsgID, 14,	Act17_RoleData[KillerID].Score)		--����
			msg.AddMsgEvent(MsgID, 17,	Act17_RoleData[KillerID].KillPlayer)
			msg.DispatchRoleMsgEvent(KillerID, MsgID)
			--���������߻���
			Act17_RoleData[RoleID].Score = Act17_RoleData[RoleID].Score - PunishScore
			--ͬ���ͻ��˻���
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 60,	156)								--������
			msg.AddMsgEvent(MsgID, 14,	Act17_RoleData[RoleID].Score)		--����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end]]
	end
end

aux.RegisterMapEvent("m21", 5, "m21_RoleDie")

-------------------------------------------------------------------------------------------------------
--����ڻ�е��ߺ�����
function m21_CanTakeOverWhenOnline(MapID, RoleID)
	--������ҵ����ߵ�ͼ������todo
	return 3017299663, 2156, 18363, 2420
end

aux.RegisterMapEvent("m21",  12, "m21_CanTakeOverWhenOnline")

-------------------------------------------------------------------------------------------------------
--�ҵ���ͼ�ĳ��ں�����
function m21_GetExporMapAndCoord(MapID, InstanceID, RoleID)
	return 1, 3017299663, 2156, 18363, 2420
end

aux.RegisterMapEvent("m21",  13, "m21_GetExporMapAndCoord")

-------------------------------------------------------------------------------------------------------
--�ҵ�����ʵĸ���ʵ��
function m21_GetOnePerfectMap(MapID, InstanceID, RoleID)
	--[[local RoleLevel = role.GetRoleLevel(MapID, InstanceID, RoleID)

    if Act14_RoleData[RoleID] ~= nil then]]
	    return 1
	--[[elseif Act15_RoleData[RoleID] ~= nil then
	    return 2
	elseif Act16_RoleData[RoleID] ~= nil then
	    return 3
	elseif Act17_RoleData[RoleID] ~= nil then
	    return 4
	else
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
	end

	return 0]]
end

aux.RegisterMapEvent("m21",  14, "m21_GetOnePerfectMap")

-------------------------------------------------------------------------------------------------------
--��еĵ����ж�
function m21_FriendEnemy(MapID, InstanceID, RoleID, TargetID)
    local RoleLevel1 = role.GetRoleLevel(MapID, InstanceID, RoleID)
	local RoleLevel2 = role.GetRoleLevel(MapID, InstanceID, TargetID)
	if unit.IsPlayer(RoleID) and unit.IsPlayer(TargetID) then
		return false, true, false, true
	elseif unit.IsPlayer(RoleID) and unit.IsCreature(TargetID) then
	    local TypeID = cre.GetCreatureTypeID(MapID, InstanceID, TargetID)
	    if TypeID == 1534150 or TypeID == 1534151 or TypeID == 1534152 or TypeID == 1534153 then
			if RoleLevel1 <= 79 then
				return false, true, false, true
			else
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 26, 2865)           --ֻ��79�����µ���Ҳ��ܻ�ɱ�ó����е�С����
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
				return true, false, false, true
			end
		end
	else
		return false, false, false, false
	end
end

aux.RegisterMapEvent("m21", 16, "m21_FriendEnemy")

-------------------------------------------------------------------------------------------------------
--�������Ƿ��ܹ���������
function m21_CanEnter(MapID, InstanceID, RoleID)
	local RoleLevel = role.GetRoleLevel(MapID, InstanceID, RoleID)
	--��ҵȼ�С��40��
	if RoleLevel < 40 then
		return 0
	end

	if Act14_RoleData[RoleID] ~= nil then --or Act15_RoleData[RoleID] ~= nil or Act16_RoleData[RoleID] ~= nil or Act17_RoleData[RoleID] ~= nil then
		--���������������뿪���ж���ҵ�ǰ��Ǯ�Ƿ��㹻��
		local MoneyNum = role.GetRoleSilver(MapID, InstanceID, RoleID)
		if Act14_RoleData[RoleID] then
			if RoleLevel < 60 then
				if MoneyNum < 2000 then
					--��ʾ��ҽ�Ǯ����
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 20,	158)
					msg.AddMsgEvent(MsgID, 21,	5)			    --ȡ��
					msg.AddMsgEvent(MsgID, 1,	22)
					msg.DispatchRoleMsgEvent(RoleID, MsgID)

					return 0
				end
			elseif RoleLevel < 80 then
				if MoneyNum < 5000 then
					--��ʾ��ҽ�Ǯ����
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 20,	158)
					msg.AddMsgEvent(MsgID, 21,	5)			    --ȡ��
					msg.AddMsgEvent(MsgID, 1,	22)
					msg.DispatchRoleMsgEvent(RoleID, MsgID)

					return 0
				end
			elseif RoleLevel < 100 then
				if MoneyNum < 10000 then
					--��ʾ��ҽ�Ǯ����
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 20,	158)
					msg.AddMsgEvent(MsgID, 21,	5)			    --ȡ��
					msg.AddMsgEvent(MsgID, 1,	22)
					msg.DispatchRoleMsgEvent(RoleID, MsgID)

					return 0
				end
			else
				if MoneyNum < 20000 then
					--��ʾ��ҽ�Ǯ����
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 20,	158)
					msg.AddMsgEvent(MsgID, 21,	5)			    --ȡ��
					msg.AddMsgEvent(MsgID, 1,	22)
					msg.DispatchRoleMsgEvent(RoleID, MsgID)

					return 0
				end
			end
		end
	end

	return 1
end

aux.RegisterMapEvent("m21",  15, "m21_CanEnter")

--�ͻ�����ͬ������������
function Act14_DefaultRequest(ActID, RoleID, EventType)
	local Index = 1
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 50, 0)

	for k in pairsByKeys(Act14_RoleData, Act14_Cmp) do
		msg.AddMsgEvent(MsgID, 15, Index)
		msg.AddMsgEvent(MsgID, 2, k)
		msg.AddMsgEvent(MsgID, 14, Act14_RoleData[k].Score)

		Index = Index + 1
		if Index > 20 then
			break
		end
	end
	msg.DispatchRoleMsgEvent(RoleID, MsgID)
end


aux.RegisterActEvent(14, 5, "Act14_DefaultRequest")
aux.RegisterActEvent(97, 5, "Act14_DefaultRequest")
aux.RegisterActEvent(18, 5, "Act14_DefaultRequest")
aux.RegisterActEvent(103, 5, "Act14_DefaultRequest")
--��������
function cxiongxing_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
    if TargetTypeID == 1534154 then
	   -- ruorouqiangshi_jingqiu = 0
		unit.AddBuff(MapID, InstanceID, RoleID, 5004401, -1)
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 101, 2866)    --XXX�����������򣬻�������������ǿ��״̬��
		msg.AddMsgEvent(MsgID, 2, RoleID)
		msg.DispatchMapMsgEvent(MsgID, 2983744463, 1)
	else
	    local RoleLevel = role.GetRoleLevel(MapID, InstanceID, RoleID)
		if RoleLevel <= 79 then
		    local killexp = PKAct_killexp[RoleLevel]
			Act14_RoleData[RoleID].KillPlayer = Act14_RoleData[RoleID].KillPlayer + 1
			if Act14_RoleData[RoleID].KillPlayer > 60 then
			    local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 13, 0)
				msg.AddMsgEvent(MsgID, 1, 2868)             --ÿ������ǿʳ���ֻ�ܻ��100�λ�ɱ����
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 26, 2868)             --ÿ������ǿʳ���ֻ�ܻ��100�λ�ɱ����
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			    killexp = 0
			end
			if killexp > 0 and Act14_EXP10 > 0 and math.random(100) <= 5 then
			    killexp = killexp * 10
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 102, 100137)
				msg.AddMsgEvent(MsgID, 2, RoleID)
				msg.AddMsgEvent(MsgID, 9, killexp)
				msg.DispatchBroadcast(MsgID,-1,-1,-1)
			    Act14_EXP10 = Act14_EXP10 - 1
			end
			role.AddRoleExp(MapID, InstanceID, RoleID, killexp)
			Act14_RoleData[RoleID].Score = Act14_RoleData[RoleID].Score + 2
			Act14_RoleData[RoleID].Score2 = Act14_RoleData[RoleID].Score2 + 2
			Act14_RoleData[RoleID].KillAnimal = Act14_RoleData[RoleID].KillAnimal + 1
			--ͬ���ͻ��˻���
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 60,	153)								--������
			msg.AddMsgEvent(MsgID, 14,	Act14_RoleData[RoleID].Score)		--����
			msg.AddMsgEvent(MsgID, 17,	Act14_RoleData[RoleID].KillPlayer - Act14_RoleData[RoleID].KillAnimal)
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 101, 2860)           --��ɱXXX����þ���XXX�㣬����XXX��
			msg.AddMsgEvent(MsgID, 5, TargetTypeID)
			msg.AddMsgEvent(MsgID, 9, killexp)
			msg.AddMsgEvent(MsgID, 9, 2)
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	end
end
aux.RegisterCreatureEvent(1534154, 4, "cxiongxing_OnDie")
aux.RegisterCreatureEvent(1534150, 4, "cxiongxing_OnDie")
aux.RegisterCreatureEvent(1534151, 4, "cxiongxing_OnDie")
aux.RegisterCreatureEvent(1534152, 4, "cxiongxing_OnDie")
aux.RegisterCreatureEvent(1534153, 4, "cxiongxing_OnDie")

function RuoRouQiangShi_Use(MapID, InstanceID, TypeID, TargetID)
    local k = TypeID - 3302201
    unit.AddBuff(MapID, InstanceID, TargetID, 5004001+k*100, TargetID)

	if TypeID == 3302202 and Act14_BroadCast == 1 then
	    local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 102, 100134)
        msg.AddMsgEvent(MsgID, 2, TargetID)
		msg.DispatchBroadcast(MsgID,-1,-1,-1)
		Act14_BroadCast = 0
	end
end

function RuoRouQiangShi_CanUse(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	if MapID ~= 2983744463 then
		--��ʾ��ұ����ռ䲻��
		bRet = 32
	end
	return bRet, bIgnore
end
aux.RegisterItemEvent(3302201, 1, "RuoRouQiangShi_Use")
aux.RegisterItemEvent(3302201, 0, "RuoRouQiangShi_CanUse")
aux.RegisterItemEvent(3302202, 1, "RuoRouQiangShi_Use")
aux.RegisterItemEvent(3302202, 0, "RuoRouQiangShi_CanUse")
aux.RegisterItemEvent(3302203, 1, "RuoRouQiangShi_Use")
aux.RegisterItemEvent(3302203, 0, "RuoRouQiangShi_CanUse")
aux.RegisterItemEvent(3302204, 1, "RuoRouQiangShi_Use")
aux.RegisterItemEvent(3302204, 0, "RuoRouQiangShi_CanUse")

function pairsByKeys (t, f)
    local a = {}
    for n in pairs(t) do a[#a + 1] = n end
    table.sort(a, f)
    local i = 0                 -- iterator variable
    local iter = function ()    -- iterator function
        i = i + 1
        if a[i] == nil then return nil
        else return a[i], t[a[i]]
        end
    end
    return iter
end

















