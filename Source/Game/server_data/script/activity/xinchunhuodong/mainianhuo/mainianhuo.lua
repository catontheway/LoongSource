Quest_NianHuo = {}
Quest_NianHuo[1]={TargetType=QuestTargetType["Collect"], TargetID1=3303035, TargetID2=0, TargetID3=0, TargetID4=0, Num1=3, Num2=0, Num3=0, Num4=0, MsgID=20001}
Quest_NianHuo[2]={TargetType=QuestTargetType["Collect"], TargetID1=3303036, TargetID2=0, TargetID3=0, TargetID4=0, Num1=3, Num2=0, Num3=0, Num4=0, MsgID=20002}
Quest_NianHuo[3]={TargetType=QuestTargetType["Collect"], TargetID1=3303037, TargetID2=0, TargetID3=0, TargetID4=0, Num1=3, Num2=0, Num3=0, Num4=0, MsgID=20003}
Quest_NianHuo[4]={TargetType=QuestTargetType["Collect"], TargetID1=3303038, TargetID2=0, TargetID3=0, TargetID4=0, Num1=3, Num2=0, Num3=0, Num4=0, MsgID=20004}
Quest_NianHuo[5]={TargetType=QuestTargetType["Collect"], TargetID1=3303039, TargetID2=0, TargetID3=0, TargetID4=0, Num1=3, Num2=0, Num3=0, Num4=0, MsgID=20005}
Quest_NianHuo[6]={TargetType=QuestTargetType["Collect"], TargetID1=3303040, TargetID2=0, TargetID3=0, TargetID4=0, Num1=1, Num2=0, Num3=0, Num4=0, MsgID=20006}
Quest_NianHuo[7]={TargetType=QuestTargetType["Collect"], TargetID1=3303041, TargetID2=0, TargetID3=0, TargetID4=0, Num1=1, Num2=0, Num3=0, Num4=0, MsgID=20007}
Quest_NianHuo[8]={TargetType=QuestTargetType["Collect"], TargetID1=3303042, TargetID2=0, TargetID3=0, TargetID4=0, Num1=1, Num2=0, Num3=0, Num4=0, MsgID=20008}



--��ȡ����
function H20171_OnQuestInit(MapID, InstanceID, QuestID, RoleID, NPCID)

	local Index = math.random(1,8)
	--��ʼ������Ķ�̬����
	role.QuestInit(RoleID, QuestID, GetRandQuestTable(Quest_NianHuo, Index))
    --ͬ���ͻ�������˵��
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 11, QuestID)
	msg.AddMsgEvent(MsgID, 1, Quest_NianHuo[Index].MsgID)
	msg.DispatchRoleMsgEvent(RoleID, MsgID)
end

--�������
function H20171_OnComplete(MapID, InstanceID, QuestID, RoleID, NPCID)

	local exp_award = {}
	exp_award[30] = 3350
	exp_award[35] = 3975
	exp_award[40] = 4600
	exp_award[45] = 5650
	exp_award[50] = 6700
	exp_award[55] = 7750
	exp_award[60] = 9060
	exp_award[65] = 10760
	exp_award[70] = 12120
	exp_award[75] = 14160
	exp_award[80] = 16200
	exp_award[85] = 18400
	exp_award[90] = 21000
	exp_award[95] = 25200
	exp_award[100] = 30276
	exp_award[105] = 38253
	exp_award[110] = 50582


	--[[local silver_award = {}
	silver_award[30] = 1
	silver_award[35] = 1
	silver_award[40] = 1
	silver_award[45] = 1
	silver_award[50] = 1
	silver_award[55] = 1
	silver_award[60] = 1
	silver_award[65] = 1
	silver_award[70] = 1
	silver_award[75] = 1
	silver_award[80] = 1
	silver_award[85] = 1
	silver_award[90] = 1
	silver_award[95] = 1
	silver_award[100] = 1]]


	local RoleLvl = role.GetRoleLevel(MapID, InstanceID, RoleID)
	local k = math.floor(RoleLvl/5)
	k = k * 5

	--������Ҿ������Ǯ
	role.AddRoleExp(MapID, InstanceID, RoleID, exp_award[k])
	--role.AddRoleSilver(MapID, InstanceID, RoleID, silver_award[k], 102)
    local y = math.random(100)
	local x = math.random(100)
	local k = math.random(100)
	--local curmon = tonumber(os.date("%m"))
	--local curday = tonumber(os.date("%d"))
		if y <= 70 then
		    role.AddRoleItem(MapID, InstanceID, RoleID, 3303006, 1, -1, 8, 102) --��ζ����
		elseif y <= 99 and y > 70 then
		    role.AddRoleItem(MapID, InstanceID, RoleID, 3303008, 1, -1, 8, 102) --õ������
		elseif y > 99 then
		    role.AddRoleItem(MapID, InstanceID, RoleID, 3303007, 1, -1, 8, 102) --��������
			local MsgID = msg.BeginMsgEvent()
	        msg.AddMsgEvent(MsgID, 13, 1)
	        msg.AddMsgEvent(MsgID, 1, 2115) --&lt;p1&gt;�ڰ�����ũة�ð�����Ĺ����У�������ϡ�����ֽ������������֡�һ�š�
	        msg.AddMsgEvent(MsgID, 2, RoleID)
			msg.DispatchWorldMsgEvent(MsgID)
		end

		if x <= 45 then
		    role.AddRoleItem(MapID, InstanceID, RoleID, 3303009, 1, -1, 8, 102) --����֮ˮ
		elseif x > 45 and x < 91 then
		    role.AddRoleItem(MapID, InstanceID, RoleID, 3303011, 1, -1, 8, 102) --����ũ��
        elseif x >= 91 and x < 96 then
		    role.AddRoleItem(MapID, InstanceID, RoleID, 3303010, 1, -1, 8, 102) --������ˮ
			--[[local MsgID = msg.BeginMsgEvent()
	        msg.AddMsgEvent(MsgID, 13, 1)
	        msg.AddMsgEvent(MsgID, 1, 2116) --&lt;p1&gt;�ڰ�����ũة�ð�����Ĺ����У������á�&lt;p2&gt;��һ�ݣ�
	        msg.AddMsgEvent(MsgID, 2, RoleID)
	        msg.AddMsgEvent(MsgID, 4, 3303010)
			msg.DispatchWorldMsgEvent(MsgID)]]
        elseif x > 95 then
		    role.AddRoleItem(MapID, InstanceID, RoleID, 3303012, 1, -1, 8, 102) --�������
            --[[local MsgID = msg.BeginMsgEvent()
	        msg.AddMsgEvent(MsgID, 13, 1)
	        msg.AddMsgEvent(MsgID, 1, 2116) --&lt;p1&gt;�ڰ�����ũة�ð�����Ĺ����У������á�&lt;p2&gt;��һ�ݣ�
	        msg.AddMsgEvent(MsgID, 2, RoleID)
	        msg.AddMsgEvent(MsgID, 4, 3303012)
			msg.DispatchWorldMsgEvent(MsgID)]]
		end

		local FreeSize = role.GetBagFreeSize(RoleID)
		if(FreeSize > 0) then
		    if k <= 5 then
			    local kuangshi = {2000003,2000008,2000013,2000018,2000023}
				role.AddRoleItem(MapID, InstanceID, RoleID, kuangshi[k], 1, -1, 8, 102)
				--[[local MsgID = msg.BeginMsgEvent()
	            msg.AddMsgEvent(MsgID, 13, 1)
	            msg.AddMsgEvent(MsgID, 1, 2116) --&lt;p1&gt;�ڰ�����ũة�ð�����Ĺ����У������á�&lt;p2&gt;��һ�ݣ�
	            msg.AddMsgEvent(MsgID, 2, RoleID)
	            msg.AddMsgEvent(MsgID, 4, kuangshi[k])
			    msg.DispatchWorldMsgEvent(MsgID)]]
			elseif k == 6 then
			    role.AddRoleItem(MapID, InstanceID, RoleID, 1211004, 1, -1, 8, 102)
				--[[local MsgID = msg.BeginMsgEvent()
	            msg.AddMsgEvent(MsgID, 13, 1)
	            msg.AddMsgEvent(MsgID, 1, 2116) --&lt;p1&gt;�ڰ�����ũة�ð�����Ĺ����У������á�&lt;p2&gt;��һ�ݣ�
	            msg.AddMsgEvent(MsgID, 2, RoleID)
	            msg.AddMsgEvent(MsgID, 4, 1211004)
			    msg.DispatchWorldMsgEvent(MsgID)]]
			elseif k == 7 then
			    role.AddRoleItem(MapID, InstanceID, RoleID, 1212004, 1, -1, 8, 102)
				--[[local MsgID = msg.BeginMsgEvent()
	            msg.AddMsgEvent(MsgID, 13, 1)
	            msg.AddMsgEvent(MsgID, 1, 2116) --&lt;p1&gt;�ڰ�����ũة�ð�����Ĺ����У������á�&lt;p2&gt;��һ�ݣ�
	            msg.AddMsgEvent(MsgID, 2, RoleID)
	            msg.AddMsgEvent(MsgID, 4, 1212004)
			    msg.DispatchWorldMsgEvent(MsgID)]]
			elseif k==8 then
			    local type = math.random(1,6)
				local laoyin = {1410002,1410014,1410026,1410038,1410050,1410062}
				role.AddRoleItem(MapID, InstanceID, RoleID, laoyin[type], 1, -1, 8, 102)
			   --[[ local MsgID = msg.BeginMsgEvent()
	            msg.AddMsgEvent(MsgID, 13, 1)
	            msg.AddMsgEvent(MsgID, 1, 2116) --&lt;p1&gt;�ڰ�����ũة�ð�����Ĺ����У������á�&lt;p2&gt;��һ�ݣ�
	            msg.AddMsgEvent(MsgID, 2, RoleID)
	            msg.AddMsgEvent(MsgID, 4, laoyin[type])
			    msg.DispatchWorldMsgEvent(MsgID)]]
			end
		end

	local TotalNum = role.GetRoleScriptData(RoleID, 1, RoleDataType["NianHuo_Total"])
    role.SetRoleScriptData(RoleID, 1, RoleDataType["NianHuo_Total"], TotalNum + 1)
end

function H20171_OnCheckAccept(MapID, InstanceID, QuestID, RoleID, NPCID)
--��������ϴθ������ڣ����뵱ǰ���ڲ�һ�£��������ܻ�������������
	local TotalNum = role.GetRoleScriptData(RoleID, 1, RoleDataType["NianHuo_Total"])
	local UpdateTime = role.GetRoleScriptData(RoleID, 1, RoleDataType["NianHuo_Update"])
	local CurTime = tonumber(os.date("%j"))
	if CurTime~=UpdateTime then
		role.SetRoleScriptData(RoleID, 1, RoleDataType["NianHuo_Total"], 0)
		role.SetRoleScriptData(RoleID, 1, RoleDataType["NianHuo_Update"], CurTime)
	end
	--�������ѽ�ȡ10�Σ����޷��ٽ�ȡ
	if TotalNum > 10 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 147)
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
		return 0
	end
	return 1
end

function H20171_OnCheckComplete(MapID, InstanceID, QuestID, RoleID, NPCID)
	--�жϱ������пռ��Ƿ��㹻
		local FreeSize = role.GetBagFreeSize(RoleID)
		if(FreeSize < 3) then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 26, 142)
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
			return 0
		end
	return 1
end


aux.RegisterQuestEvent(20171, 1, "H20171_OnComplete")
aux.RegisterQuestEvent(20171, 7, "H20171_OnQuestInit")
aux.RegisterQuestEvent(20171, 4, "H20171_OnCheckAccept")
aux.RegisterQuestEvent(20171, 5, "H20171_OnCheckComplete")
