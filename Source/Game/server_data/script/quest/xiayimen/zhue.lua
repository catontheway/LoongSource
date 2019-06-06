Quest_ZhuE = {}
Quest_ZhuE[1]={TargetType=QuestTargetType["Collect"], TargetID1=3200003, TargetID2=0, TargetID3=0, TargetID4=0, Num1=1, Num2=0, Num3=0, Num4=0, MsgID=16002}

--�����ȡ����

function q20078_OnQuestInit(MapID, InstanceID, QuestID, RoleID, NPCID)

	ZhuE_GetPoint(MapID, InstanceID, RoleID)--��ȡ��������
	role.SetRoleScriptData(RoleID, 1, RoleDataType["ZhuE_UseTimes"],0) --������Ʒʹ�ô���

	--��ʼ������Ķ�̬����
	role.QuestInit(RoleID, QuestID, GetRandQuestTable(Quest_ZhuE ,1))



	--ͬ���ͻ�����������������˵��
	local LoopNum = role.GetRoleScriptData(RoleID, 1, RoleDataType["ZhuE_Loop"])

	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 11, QuestID)
	msg.AddMsgEvent(MsgID, 10, LoopNum)
	msg.AddMsgEvent(MsgID, 1, Quest_ZhuE[1].MsgID)
	msg.DispatchRoleMsgEvent(RoleID, MsgID)

end

--�������
function q20078_OnComplete(MapID, InstanceID, QuestID, RoleID, NPCID)

	local exp_award = {}
	exp_award[2] = 1240
	exp_award[3] = 1760
	exp_award[4] = 2220
	exp_award[5] = 3080
	exp_award[6] = 4060
	exp_award[7] = 5540
	exp_award[8] = 7080
	exp_award[9] = 8960
	exp_award[10]= 10420

	local money = {}
	money[25] = 50000
	money[30] = 50000
	money[35] = 70000
	money[40] = 100000
	money[45] = 150000
	money[50] = 200000
	money[55] = 250000
	money[60] = 300000
	money[65] = 325000
	money[70] = 350000
	money[75] = 375000
	money[80] = 400000
	money[85] = 425000
	money[90] = 450000
	money[95] = 475000
	money[100] = 500000

	local exp_extra=0

	local rolelevel=role.GetRoleLevel(MapID, InstanceID, RoleID)
	local k = math.floor(rolelevel/10)
	if k>10 then
		k = 10
	end

	--�õ���ǰ�����ܻ���
	local LoopNum = role.GetRoleScriptData(RoleID, 1, RoleDataType["ZhuE_Loop"])
	local TotalNum = role.GetRoleScriptData(RoleID, 1, RoleDataType["ZhuE_Total"])
	--���㻷��
	local LoopNum_Ring = LoopNum % 10
	if LoopNum_Ring == 0 then
		LoopNum_Ring = 10
	end
	--������ҽ�Ǯ
	local s = math.floor(rolelevel/5)*5
	local silver = money[s]*(LoopNum_Ring+10)/155
	role.AddRoleSilver(MapID, InstanceID, RoleID, silver, 102)

	local Exp=0
	--���㾭�齱��
	if LoopNum_Ring == 10 then
		Exp = exp_award[k]*(1+(LoopNum_Ring-1)*0.5)+ exp_extra
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 102, 100112)--"���������š�<p1>�������һ������������ܹ����<p2>��ҽ�����"
		msg.AddMsgEvent(MsgID, 2, RoleID)
		msg.AddMsgEvent(MsgID, 9, math.floor(money[s]/10000))
		msg.DispatchBroadcast(MsgID,-1,-1,-1)
	else
		Exp = exp_award[k]*(1+(LoopNum_Ring-1)*0.5)
	end

	--������Ҿ���
	role.AddRoleExp(MapID, InstanceID, RoleID, Exp)

	--��ǰ������һ,����������һ
	role.SetRoleScriptData(RoleID, 1, RoleDataType["ZhuE_Loop"], LoopNum+1)
	role.SetRoleScriptData(RoleID, 1, RoleDataType["ZhuE_Total"], TotalNum+1)

	--���ӽ���ʵ��ֵ
	Increase_shili(MapID, InstanceID, RoleID, 0, 2, "jinjieshili_Q")
    --[[��ѧ�
    local curhour = os.date("%H")
	local act_start = act.GetActIsStart(77)
	if act_start == true and (curhour=="19" or curhour=="20" or curhour=="21" or curhour=="22" or curhour=="23") then
	    role.AddRoleItem(MapID, InstanceID, RoleID, 3303206, 1, -1, 8, 102)
		HuoDeLeiFengMao(RoleID)
		local i = math.random(3)
		if i == 1 then
			role.AddRoleItem(MapID, InstanceID, RoleID, 3303208, 1, -1, 8, 102)
		end
	end]]

end

function q20078_OnCheckAccept(MapID, InstanceID, QuestID, RoleID, NPCID)

	local FreeSize = role.GetBagFreeSize(RoleID)
	if(FreeSize < 1) then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 141)
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
		return 0
	end

	--�жϱ��������ȡ�����Ƿ�����
	--�õ���ǰ������������
	local LoopNum = role.GetRoleScriptData(RoleID, 1, RoleDataType["ZhuE_Loop"])
	local TotalNum = role.GetRoleScriptData(RoleID, 1, RoleDataType["ZhuE_Total"])
	local UpdateTime = role.GetRoleScriptData(RoleID, 1, RoleDataType["ZhuE_Update"])

	--�ܻ���Ϊ0ʱ�������ܻ���Ϊ��һ��
	if 0 == LoopNum then
		LoopNum = 1
		role.SetRoleScriptData(RoleID, 1, RoleDataType["ZhuE_Loop"], LoopNum)
	end
	if 0 == TotalNum then
		TotalNum = 1
		role.SetRoleScriptData(RoleID, 1, RoleDataType["ZhuE_Total"], TotalNum)
	end

	--��������ϴθ������ڣ����뵱ǰ���ڲ�һ�£��������ܻ�������������
	local CurTime = tonumber(os.date("%j"))
	if CurTime~=UpdateTime then
		role.SetRoleScriptData(RoleID, 1, RoleDataType["ZhuE_Loop"], 1)
		role.SetRoleScriptData(RoleID, 1, RoleDataType["ZhuE_Total"], 1)
		role.SetRoleScriptData(RoleID, 1, RoleDataType["ZhuE_Update"], CurTime)
	end
	--�������ѽ�ȡ50�Σ����޷��ٽ�ȡ
	if TotalNum > 50 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 147)
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
		return 0
	end

	return 1
end



--��������
function q20078_OnCancel(MapID, InstanceID, QuestID, RoleID)

	local LoopNum = role.GetRoleScriptData(RoleID, 1, RoleDataType["ZhuE_Loop"])
	local TotalNum = role.GetRoleScriptData(RoleID, 1, RoleDataType["ZhuE_Total"])
	local k = LoopNum % 10
	if k == 0 then
		k=10
	end
	--����������һ����ǰ������Ϊ1
	role.SetRoleScriptData(RoleID, 1, RoleDataType["ZhuE_Loop"], LoopNum-k+1)
	role.SetRoleScriptData(RoleID, 1, RoleDataType["ZhuE_Total"], TotalNum+1)
end



--�Ƿ����ʹ���������
function I3200001_CanUseItem(MapID, InstanceID, TypeID, RoleID)
	--�������ѵ�����ָ�������
	local CanUse = ZhuE_EnterNearArea(MapID, InstanceID, RoleID)
	--�����Ҳ���ָ�����긽������ָ�����귢���ͻ���
	if CanUse == false then
		local Index = role.GetRoleScriptData(RoleID, 1, RoleDataType["ZhuE_Pos"])
	    local MsgID = msg.BeginMsgEvent()
	    msg.AddMsgEvent(MsgID, 13, 0)
	    msg.AddMsgEvent(MsgID, 1, 253)
	    msg.AddMsgEvent(MsgID, 6, Quest_ZhuE_Pos[Index].MapCrc)
	    msg.AddMsgEvent(MsgID, 9, Quest_ZhuE_Pos[Index].x)
	    msg.AddMsgEvent(MsgID, 9, Quest_ZhuE_Pos[Index].z)
	    msg.DispatchRoleMsgEvent(RoleID, MsgID)

	    local MsgID1 = msg.BeginMsgEvent()
	    msg.AddMsgEvent(MsgID1, 26, 253)
	    msg.AddMsgEvent(MsgID1, 6, Quest_ZhuE_Pos[Index].MapCrc)
	    msg.AddMsgEvent(MsgID1, 9, Quest_ZhuE_Pos[Index].x)
	    msg.AddMsgEvent(MsgID1, 9, Quest_ZhuE_Pos[Index].z)
	    msg.DispatchRoleMsgEvent(RoleID, MsgID1)
	    return 36, false
    else
        return 0, false
	end
end


--ʹ���������
function I3200001_UseItem(MapID, InstanceID, TypeID, RoleID)
	--����ָ�������,ʹ����Ʒ
	ZhuE_Use(MapID, InstanceID, RoleID)
end



aux.RegisterQuestEvent(20078, 1, "q20078_OnComplete")
aux.RegisterQuestEvent(20078, 7, "q20078_OnQuestInit")
aux.RegisterQuestEvent(20078, 2, "q20078_OnCancel")
aux.RegisterQuestEvent(20078, 4, "q20078_OnCheckAccept")

aux.RegisterItemEvent(3200001, 0, "I3200001_CanUseItem")
aux.RegisterItemEvent(3200001, 1, "I3200001_UseItem")

