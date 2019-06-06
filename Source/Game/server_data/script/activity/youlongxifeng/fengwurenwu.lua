Quest_FengWu = {}
Quest_FengWu[1]={TargetType=QuestTargetType["Collect"], TargetID1=2612617, TargetID2=0, TargetID3=0, TargetID4=0, Num1=1, Num2=0, Num3=0, Num4=0, MsgID=14005}
Quest_FengWu[2]={TargetType=QuestTargetType["Collect"], TargetID1=2612618, TargetID2=0, TargetID3=0, TargetID4=0, Num1=1, Num2=0, Num3=0, Num4=0, MsgID=14006}
Quest_FengWu[3]={TargetType=QuestTargetType["Collect"], TargetID1=2612619, TargetID2=0, TargetID3=0, TargetID4=0, Num1=1, Num2=0, Num3=0, Num4=0, MsgID=14007}
Quest_FengWu[4]={TargetType=QuestTargetType["Collect"], TargetID1=2612620, TargetID2=0, TargetID3=0, TargetID4=0, Num1=1, Num2=0, Num3=0, Num4=0, MsgID=14008}


function x20039_OnQuestInit(MapID, InstanceID, QuestID, OwnerID, NPCID)

	local Index = math.random(4)

	if Index == 1 then
		role.AddRoleItem(MapID, InstanceID, OwnerID, 2612612, 1, -1, 8, 102)
	elseif Index == 2 then
		role.AddRoleItem(MapID, InstanceID, OwnerID, 2612613, 1, -1, 8, 102)
	elseif Index == 3 then
		role.AddRoleItem(MapID, InstanceID, OwnerID, 2612614, 1, -1, 8, 102)
	elseif Index == 4 then
		role.AddRoleItem(MapID, InstanceID, OwnerID, 2612615, 1, -1, 8, 102)
	end

	role.SetRoleScriptData(OwnerID, 1, RoleDataType["LongFeng_Index"], Index)

	--��ʼ������Ķ�̬����
	role.QuestInit(OwnerID, QuestID, GetRandQuestTable(Quest_FengWu, Index))

	--ͬ���ͻ�����������������˵��
	local LoopNum = role.GetRoleScriptData(OwnerID, 1, RoleDataType["LongFeng"])

	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 11, QuestID)
	msg.AddMsgEvent(MsgID, 10, LoopNum)
	msg.AddMsgEvent(MsgID, 1, Quest_FengWu[Index].MsgID)
	msg.DispatchRoleMsgEvent(OwnerID, MsgID)
end

function x20039_OnComplete(MapID, InstanceID, QuestID, OwnerID, NPCID)

	local exp_award = 1500

	--����������ɴ���>200��ʱ��>90ʱ����������
	Longfeng_Times = Longfeng_Times+1

	if Longfeng_Times>=200 and act26_TimerMin>=40 and Longfeng_Xiuqiu==1 then
		--map.MapCreateCreature(3017299663, -1, 1521009, 2012, 18346, 2686)
		--map.MapCreateCreature(3017299663, -1, 1521010, 2193, 18346, 2687)
		Longfeng_Xiuqiu =2
		--��������¹ȫ����֪ͨ[��Ļ��������������ʾ]����***��
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 100, 124)
		msg.DispatchMapMsgEvent(MsgID, MapID, -1)
	end

	--�õ���ǰ�����ܻ���
	local LoopNum = role.GetRoleScriptData(OwnerID, 1, RoleDataType["LongFeng"])
	local TotalNum = role.GetRoleScriptData(OwnerID, 1, RoleDataType["LongFeng_Total"])
	--���㻷��
	local LoopNum_Ring = LoopNum % 10
	if LoopNum_Ring == 0 then
		LoopNum_Ring = 10
	end
	--��������
	local LoopNum_Round = (LoopNum - LoopNum_Ring) /10 + 1
	--���㾭�齱��
	local Exp = exp_award*(1+(LoopNum_Ring-1)*0.3)*Longfeng_Xiuqiu

	--������Ҿ���
	role.AddRoleExp(MapID, InstanceID, OwnerID, Exp)
	--�������칱��ֵ
	local Cont_val = role.GetRoleScriptData(OwnerID, 1, RoleDataType["LongFeng_Cont"])
	local yirong = role.GetRoleAttValue(MapID, InstanceID, OwnerID, 71)

	Cont_val = math.floor(Cont_val+10+yirong/20)


	--��ʮ��������ԯ����
	if LoopNum_Ring == 10 then
		role.AddRoleItem(MapID, InstanceID, OwnerID, 2612606, 1, -1, 8, 102)
		Cont_val = Cont_val+50
	end

	role.SetRoleScriptData(OwnerID, 1, RoleDataType["LongFeng_Cont"], Cont_val)

	--����������һ
	role.SetRoleScriptData(OwnerID, 1, RoleDataType["LongFeng"], LoopNum+1)
	role.SetRoleScriptData(OwnerID, 1, RoleDataType["LongFeng_Total"], TotalNum+1)
	--���õ�ǰ��������IndexΪ0(GT_INVALID)
	role.SetRoleScriptData(OwnerID, 1, RoleDataType["LongFeng_Index"], 0)
end

function x20039_OnCancel(MapID, InstanceID, QuestID, OwnerID)
	local LoopNum = role.GetRoleScriptData(OwnerID, 1, RoleDataType["LongFeng"])
	local TotalNum = role.GetRoleScriptData(OwnerID, 1, RoleDataType["LongFeng_Total"])
	local k = LoopNum % 10
	if k == 0 then
		k=10
	end
	--����������һ����ǰ������Ϊ1
	role.SetRoleScriptData(OwnerID, 1, RoleDataType["LongFeng"], LoopNum-k+1)
	role.SetRoleScriptData(OwnerID, 1, RoleDataType["LongFeng_Total"], TotalNum+1)
	--���õ�ǰ�ڱ�����IndexΪ0(GT_INVALID)
	role.SetRoleScriptData(OwnerID, 1, RoleDataType["LongFeng_Index"], 0)
end


function x20039_OnCheckAccept(MapID, InstanceID, QuestID, RoleID, NPCID)

	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(RoleID)
	if(FreeSize < 1) then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 141)
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
		return 0
	end

	--�жϱ��������ȡ�����Ƿ�����
	--�õ���ǰ������������
	local LoopNum = role.GetRoleScriptData(RoleID, 1, RoleDataType["LongFeng"])
	local TotalNum = role.GetRoleScriptData(RoleID, 1, RoleDataType["LongFeng_Total"])
	local UpdateTime = role.GetRoleScriptData(RoleID, 1, RoleDataType["LongFeng_Update"])

	--�ܻ���Ϊ0ʱ�������ܻ���Ϊ��һ��
	if 0 == LoopNum then
		LoopNum = 1
		role.SetRoleScriptData(RoleID, 1, RoleDataType["LongFeng"], LoopNum)
	end
	if 0 == TotalNum then
		TotalNum = 1
		role.SetRoleScriptData(RoleID, 1, RoleDataType["LongFeng_Total"], TotalNum)
	end

	--��������ϴθ������ڣ����뵱ǰ���ڲ�һ�£��������ܻ�������������
	local CurTime = tonumber(os.date("%j"))
	if CurTime~=UpdateTime then
		TotalNum = 1
		role.SetRoleScriptData(RoleID, 1, RoleDataType["LongFeng_Total"], TotalNum)
		LoopNum = 1
		role.SetRoleScriptData(RoleID, 1, RoleDataType["LongFeng"], LoopNum)
		role.SetRoleScriptData(RoleID, 1, RoleDataType["LongFeng_Update"], CurTime)
		role.SetRoleScriptData(RoleID, 1, RoleDataType["LongFeng_Cont"], 0)
		role.SetRoleScriptData(RoleID, 1, RoleDataType["LongFeng_Mark"], 0)
	end

	return 1
end

function x20039_OnCheckComplete(MapID, InstanceID, QuestID, RoleID, NPCID)
	--�жϱ������пռ��Ƿ��㹻
	local LoopNum = role.GetRoleScriptData(RoleID, 1, RoleDataType["LongFeng"])
	local k = LoopNum % 10

	if k ==0 then

		local FreeSize = role.GetBagFreeSize(RoleID)
		if(FreeSize < 1) then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 26, 2)
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
			return 0
		end
	end
	return 1
end


aux.RegisterQuestEvent(20039, 1, "x20039_OnComplete")
aux.RegisterQuestEvent(20039, 7, "x20039_OnQuestInit")
aux.RegisterQuestEvent(20039, 2, "x20039_OnCancel")
aux.RegisterQuestEvent(20039, 4, "x20039_OnCheckAccept")
aux.RegisterQuestEvent(20039, 5, "x20039_OnCheckComplete")
