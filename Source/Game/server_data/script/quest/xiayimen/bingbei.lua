Quest_BingBei={}
Quest_BingBei[1]={TargetType=QuestTargetType["Collect"], TargetID1=2000001, TargetID2=0, TargetID3=0, TargetID4=0, Num1=4, Num2=0, Num3=0, Num4=0, MsgID=16003}
Quest_BingBei[2]={TargetType=QuestTargetType["Collect"], TargetID1=2000006, TargetID2=0, TargetID3=0, TargetID4=0, Num1=4, Num2=0, Num3=0, Num4=0, MsgID=16003}
Quest_BingBei[3]={TargetType=QuestTargetType["Collect"], TargetID1=2000011, TargetID2=0, TargetID3=0, TargetID4=0, Num1=4, Num2=0, Num3=0, Num4=0, MsgID=16003}
Quest_BingBei[4]={TargetType=QuestTargetType["Collect"], TargetID1=2000016, TargetID2=0, TargetID3=0, TargetID4=0, Num1=4, Num2=0, Num3=0, Num4=0, MsgID=16003}
Quest_BingBei[5]={TargetType=QuestTargetType["Collect"], TargetID1=2000021, TargetID2=0, TargetID3=0, TargetID4=0, Num1=4, Num2=0, Num3=0, Num4=0, MsgID=16003}
Quest_BingBei[6]={TargetType=QuestTargetType["Collect"], TargetID1=2000101, TargetID2=0, TargetID3=0, TargetID4=0, Num1=4, Num2=0, Num3=0, Num4=0, MsgID=16003}
Quest_BingBei[7]={TargetType=QuestTargetType["Collect"], TargetID1=2000106, TargetID2=0, TargetID3=0, TargetID4=0, Num1=4, Num2=0, Num3=0, Num4=0, MsgID=16003}
Quest_BingBei[8]={TargetType=QuestTargetType["Collect"], TargetID1=2000111, TargetID2=0, TargetID3=0, TargetID4=0, Num1=4, Num2=0, Num3=0, Num4=0, MsgID=16003}
Quest_BingBei[9]={TargetType=QuestTargetType["Collect"], TargetID1=2000116, TargetID2=0, TargetID3=0, TargetID4=0, Num1=4, Num2=0, Num3=0, Num4=0, MsgID=16003}
Quest_BingBei[10]={TargetType=QuestTargetType["Collect"], TargetID1=2000201, TargetID2=0, TargetID3=0, TargetID4=0, Num1=4, Num2=0, Num3=0, Num4=0, MsgID=16003}
Quest_BingBei[11]={TargetType=QuestTargetType["Collect"], TargetID1=2000206, TargetID2=0, TargetID3=0, TargetID4=0, Num1=4, Num2=0, Num3=0, Num4=0, MsgID=16003}
Quest_BingBei[12]={TargetType=QuestTargetType["Collect"], TargetID1=2000211, TargetID2=0, TargetID3=0, TargetID4=0, Num1=4, Num2=0, Num3=0, Num4=0, MsgID=16003}
Quest_BingBei[13]={TargetType=QuestTargetType["Collect"], TargetID1=2000216, TargetID2=0, TargetID3=0, TargetID4=0, Num1=4, Num2=0, Num3=0, Num4=0, MsgID=16003}
Quest_BingBei[14]={TargetType=QuestTargetType["Collect"], TargetID1=2000221, TargetID2=0, TargetID3=0, TargetID4=0, Num1=4, Num2=0, Num3=0, Num4=0, MsgID=16003}
Quest_BingBei[15]={TargetType=QuestTargetType["Collect"], TargetID1=2000301, TargetID2=0, TargetID3=0, TargetID4=0, Num1=4, Num2=0, Num3=0, Num4=0, MsgID=16003}
Quest_BingBei[16]={TargetType=QuestTargetType["Collect"], TargetID1=2000306, TargetID2=0, TargetID3=0, TargetID4=0, Num1=4, Num2=0, Num3=0, Num4=0, MsgID=16003}
Quest_BingBei[17]={TargetType=QuestTargetType["Collect"], TargetID1=2000311, TargetID2=0, TargetID3=0, TargetID4=0, Num1=4, Num2=0, Num3=0, Num4=0, MsgID=16003}
Quest_BingBei[18]={TargetType=QuestTargetType["Collect"], TargetID1=2000316, TargetID2=0, TargetID3=0, TargetID4=0, Num1=4, Num2=0, Num3=0, Num4=0, MsgID=16003}
Quest_BingBei[19]={TargetType=QuestTargetType["Collect"], TargetID1=2000321, TargetID2=0, TargetID3=0, TargetID4=0, Num1=4, Num2=0, Num3=0, Num4=0, MsgID=16003}

--��ȡ����
function x20081_OnQuestInit(MapID, InstanceID, QuestID, RoleID, NPCID)

	--�õ����е�һ������

	local Index = 0
	local k = math.random(100)
	local rolelevel = role.GetRoleLevel(MapID, InstanceID, RoleID)

	if k>75 then
		if rolelevel>=70 then
			Index = 0+math.random(5)
		elseif rolelevel>=40 then
			Index = 0+math.random(4)
		elseif rolelevel>=25 then
			Index = 0+math.random(2)
		end
	elseif k>50 then
		if rolelevel>=80 then
			Index = 5+math.random(4)
		elseif rolelevel>=70 then
			Index = 5+math.random(3)
		elseif rolelevel>=40 then
			Index = 5+math.random(2)
		elseif rolelevel>=25 then
			Index = 5+math.random(1)
		end
	elseif k>25 then
		if rolelevel>=70 then
			Index = 9+math.random(5)
		elseif rolelevel>=40 then
			Index = 9+math.random(4)
		elseif rolelevel>=25 then
			Index = 9+math.random(2)
		end
	elseif k>0 then
		if rolelevel>=70 then
			Index = 14+math.random(5)
		elseif rolelevel>=40 then
			Index = 14+math.random(4)
		elseif rolelevel>=25 then
			Index = 14+math.random(2)
		end
	end

	--��ʼ������Ķ�̬����
	role.QuestInit(RoleID, QuestID, GetRandQuestTable(Quest_BingBei, Index))

	--ͬ���ͻ�����������������˵��
	local LoopNum = role.GetRoleScriptData(RoleID, 1, RoleDataType["BingBei_Loop"])

	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 11, QuestID)
	msg.AddMsgEvent(MsgID, 10, LoopNum)
	msg.AddMsgEvent(MsgID, 1, Quest_BingBei[Index].MsgID)
	msg.DispatchRoleMsgEvent(RoleID, MsgID)
end

--�������
function x20081_OnComplete(MapID, InstanceID, QuestID, RoleID, NPCID)

	local exp_award = {}
	exp_award[20] = 500
	exp_award[25] = 800
	exp_award[30] = 1200
	exp_award[35] = 1700
	exp_award[40] = 2300
	exp_award[45] = 3000
	exp_award[50] = 3600
	exp_award[55] = 4100
	exp_award[60] = 4500
	exp_award[65] = 4800
	exp_award[70] = 5100
	exp_award[75] = 5400
	exp_award[80] = 5700
	exp_award[85] = 6000
	exp_award[90] = 6300
	exp_award[95] = 6600
	exp_award[100]= 6900

	local rolelevel=role.GetRoleLevel(MapID, InstanceID, RoleID)
	local k = math.floor(rolelevel/5)*5
	if k>100 then
		k = 100
	end

	--�õ���ǰ�����ܻ���
	local LoopNum = role.GetRoleScriptData(RoleID, 1, RoleDataType["BingBei_Loop"])
	local TotalNum = role.GetRoleScriptData(RoleID, 1, RoleDataType["BingBei_Total"])
	--���㻷��
	local LoopNum_Ring = LoopNum % 10
	if LoopNum_Ring == 0 then
		LoopNum_Ring = 10
	end

	local IsDouble = 0
	if TotalNum <= 10 then
		IsDouble = 2
	else
		IsDouble = 1
	end

	--���㾭�齱��
	local Exp = exp_award[k]*(1+(LoopNum_Ring-1)*0.3)*IsDouble

	--[[��Ǯ����ϵ��
	for i=1,30 do
		if i*5>rolelevel then
			silver=activity_money[i*5]*activity_money_rate["BingBei"]
			break
		end
	end
	silver = silver*(1+(LoopNum_Ring-1)*0.3)/2.35
	role.AddRoleSilver(MapID, InstanceID, RoleID, silver, 102)
	]]
	--������Ҿ���
	role.AddRoleExp(MapID, InstanceID, RoleID, Exp)

	--ÿ�������ۻ��ÿ��1��
		role.AddRoleItem(MapID, InstanceID, RoleID, 3200201, IsDouble, -1, 8, 102)
	--û10��������ζ����
	if LoopNum_Ring == 10 then
		local y = math.random(100)
		local x = math.random(100)
		if y <= 70 then
		    role.AddRoleItem(MapID, InstanceID, RoleID, 3303006, 1, -1, 8, 102) --��ζ����
		elseif y <= 90 and y >= 71 then
		    role.AddRoleItem(MapID, InstanceID, RoleID, 3303008, 1, -1, 8, 102) --õ������
		elseif y >= 91 then
		    role.AddRoleItem(MapID, InstanceID, RoleID, 3303007, 1, -1, 8, 102) --��������
            local MsgID = msg.BeginMsgEvent()
	        msg.AddMsgEvent(MsgID, 13, 1)
	        msg.AddMsgEvent(MsgID, 1, 2126) --&lt;p1&gt;�������һ��������-�������������á�&lt;p2&gt��һ��
	        msg.AddMsgEvent(MsgID, 2, RoleID)
	        msg.AddMsgEvent(MsgID, 4, 3303007)
			msg.DispatchWorldMsgEvent(MsgID)
		end

		if x <= 40 then
		    role.AddRoleItem(MapID, InstanceID, RoleID, 3303009, 1, -1, 8, 102) --����֮ˮ
		elseif x > 40 and x < 81 then
		    role.AddRoleItem(MapID, InstanceID, RoleID, 3303011, 1, -1, 8, 102) --����ũ��
        elseif x >= 81 and x < 91 then
		    role.AddRoleItem(MapID, InstanceID, RoleID, 3303010, 1, -1, 8, 102) --������ˮ
			local MsgID = msg.BeginMsgEvent()
	        msg.AddMsgEvent(MsgID, 13, 1)
	        msg.AddMsgEvent(MsgID, 1, 2126) --&lt;p1&gt;�������һ��������-�������������á�&lt;p2&gt��һ��
	        msg.AddMsgEvent(MsgID, 2, RoleID)
	        msg.AddMsgEvent(MsgID, 4, 3303010)
			msg.DispatchWorldMsgEvent(MsgID)
        elseif x > 90 then
		    role.AddRoleItem(MapID, InstanceID, RoleID, 3303012, 1, -1, 8, 102) --�������
            local MsgID = msg.BeginMsgEvent()
	        msg.AddMsgEvent(MsgID, 13, 1)
	        msg.AddMsgEvent(MsgID, 1, 2126) --&lt;p1&gt;�������һ��������-�������������á�&lt;p2&gt��һ��
	        msg.AddMsgEvent(MsgID, 2, RoleID)
	        msg.AddMsgEvent(MsgID, 4, 3303012)
			msg.DispatchWorldMsgEvent(MsgID)
		end
	end
	--����������һ
	role.SetRoleScriptData(RoleID, 1, RoleDataType["BingBei_Loop"], LoopNum+1)
	role.SetRoleScriptData(RoleID, 1, RoleDataType["BingBei_Total"], TotalNum+1)

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

--��������
function x20081_OnCancel(MapID, InstanceID, QuestID, RoleID)
	local TotalNum = role.GetRoleScriptData(RoleID, 1, RoleDataType["BingBei_Total"])
	local LoopNum = role.GetRoleScriptData(RoleID, 1, RoleDataType["BingBei_Loop"])
	local LoopNum_Ring = LoopNum % 10
	if LoopNum_Ring == 0 then
		LoopNum_Ring = 10
	end
	--����������һ����ǰ������Ϊ1
	role.SetRoleScriptData(RoleID, 1, RoleDataType["BingBei_Loop"], LoopNum-LoopNum_Ring+1)
	role.SetRoleScriptData(RoleID, 1, RoleDataType["BingBei_Total"], TotalNum+1)

end

function x20081_OnCheckAccept(MapID, InstanceID, QuestID, RoleID, NPCID)

	--�жϱ��������ȡ�����Ƿ�����
	--�õ���ǰ������������
	local LoopNum = role.GetRoleScriptData(RoleID, 1, RoleDataType["BingBei_Loop"])
	local TotalNum = role.GetRoleScriptData(RoleID, 1, RoleDataType["BingBei_Total"])
	local UpdateTime = role.GetRoleScriptData(RoleID, 1, RoleDataType["BingBei_Update"])

	--�ܻ���Ϊ0ʱ�������ܻ���Ϊ��һ��
	if 0 == LoopNum then
		LoopNum = 1
		role.SetRoleScriptData(RoleID, 1, RoleDataType["BingBei_Loop"], LoopNum)
	end
	if 0 == TotalNum then
		TotalNum = 1
		role.SetRoleScriptData(RoleID, 1, RoleDataType["BingBei_Total"], TotalNum)
	end

	--��������ϴθ������ڣ����뵱ǰ���ڲ�һ�£��������ܻ�������������
	local CurTime = tonumber(os.date("%j"))
	if CurTime~=UpdateTime then
		role.SetRoleScriptData(RoleID, 1, RoleDataType["BingBei_Loop"], 1)
		role.SetRoleScriptData(RoleID, 1, RoleDataType["BingBei_Total"], 1)
		role.SetRoleScriptData(RoleID, 1, RoleDataType["BingBei_Update"], CurTime)
	end
	--�������ѽ�ȡ200�Σ����޷��ٽ�ȡ
	if TotalNum > 200 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 147)
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
		return 0
	end

	return 1
end

function x20081_OnCheckComplete(MapID, InstanceID, QuestID, RoleID, NPCID)

	local LoopNum = role.GetRoleScriptData(RoleID, 1, RoleDataType["BingBei_Loop"])
	local LoopNum_Ring = LoopNum % 10
	if LoopNum_Ring == 0 then
		--�жϱ������пռ��Ƿ��㹻
		local FreeSize = role.GetBagFreeSize(RoleID)
		if(FreeSize < 1) then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 26, 142)
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
			return 0
		end
	end

	return 1
end


aux.RegisterQuestEvent(20081, 1, "x20081_OnComplete")
aux.RegisterQuestEvent(20081, 7, "x20081_OnQuestInit")
aux.RegisterQuestEvent(20081, 2, "x20081_OnCancel")
aux.RegisterQuestEvent(20081, 4, "x20081_OnCheckAccept")
aux.RegisterQuestEvent(20081, 5, "x20081_OnCheckComplete")

