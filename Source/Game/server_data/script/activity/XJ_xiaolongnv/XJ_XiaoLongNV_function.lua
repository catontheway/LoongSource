
-- ��ʼ���ձ��Ĭ��ֵ
function XJ_XiaoLongNV_InitNullTable(RoleID)
	-- ����һЩ�ձ��Ĭ��ֵ
	if XJ_XiaoLongNV_Attend[RoleID] == nil then
		XJ_XiaoLongNV_Attend[RoleID] = 0
	end
	if XJ_XiaoLongNV_Questions[RoleID] == nil then
		XJ_XiaoLongNV_Questions[RoleID] = {NUM = 0, ID = 0, Quest = {} }  --NUMΪ��Ҫ���������һ��20����idΪ��Ҫ�����ţ�QuestΪ����¼���д�������
	end
    if XJ_XiaoLongNV_Role_Point[RoleID] == nil then
        XJ_XiaoLongNV_Role_Point[RoleID] = 0
    end
	if XJ_XiaoLongNV_RewardsRecevied[RoleID] == nil then
	    XJ_XiaoLongNV_RewardsRecevied[RoleID] = 0
	end
end


-- ��ʼ�����
function XJ_XiaoLongNV_InitQuestionTable()
	-- ��id_msg�е��մ������ݷ������
	for i=1, XJ_XiaoLongNV_Question_Num do
		local id = 150001 + (i-1)*5
		XJ_DengMi_Question_Table[i] = {Content = id, RightAnswer = id+1, WrongAnswer1 = id+2, WrongAnswer2 = id+3, WrongAnswer3 = id+4}
	end

end

--[[�����������ҽ���
function XJ_XiaoLongNV_RewardRole(MapID, InstanceID, RoleID, TargetID, TalkIndex)
    if TalkIndex == 10 then
	    local level = role.GetRoleLevel(MapID, InstanceID, RoleID)
		local x = XJ_XiaoLongNV_Role_Point[RoleID]/10
		local reward
		if x <= 0 or x == nil or x > 20 then
		    reward = 0
		else
		    reward = (300*x*level*level+x*XJ_DengMi_levelexp[level]+30000*level)/100/level
		end
		if x < 10 and x >=0 then
			role.AddRoleExp(MapID, InstanceID, RoleID, reward)
			XJ_XiaoLongNV_Role_Point[RoleID] = 0
			XJ_XiaoLongNV_RewardsRecevied[RoleID] = 0
		elseif x > 10 then
			local FreeSize = role.GetBagFreeSize(RoleID)
			if FreeSize < 1 then
                local MsgID = msg.BeginMsgEvent()
			    msg.AddMsgEvent(MsgID, 20, 2108)	-- "�´����տ��٣�\n    �����ҵĿռ䲻�㣬�������Ժ�������
			    msg.AddMsgEvent(MsgID, 21, 9)	-- ȷ����ť
			    msg.AddMsgEvent(MsgID, 1, 206)	-- "ȷ��"
			    msg.DispatchRoleMsgEvent(RoleID, MsgID)
			else
			    role.AddRoleExp(MapID, InstanceID, RoleID, reward)
			    role.AddRoleItem(MapID, InstanceID, TargetID, 4900401, 3, -1, 8, 420)
	            XJ_XiaoLongNV_Role_Point[RoleID] = 0
			    XJ_XiaoLongNV_RewardsRecevied[RoleID] = 0
	        end
		end
	elseif TalkIndex ~=10 then
		local level = role.GetRoleLevel(MapID, InstanceID, RoleID)
		local x = XJ_XiaoLongNV_Role_Point[RoleID]/10
		local reward
		if x <= 0 or x == nil then
		    reward = 0
		else
		    reward = (300*x*level*level+x*XJ_DengMi_levelexp[level]+30000*level)/100/level
		end
		if x < 10 and x >=0 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 2109)	-- "�´����տ��٣�\n    ���ε���ϲ�ֻ��������##�⣬���###�㾭�飬���ٽ�������
			msg.AddMsgEvent(MsgID, 9, x)
			msg.AddMsgEvent(MsgID, 9, reward)
			msg.AddMsgEvent(MsgID, 21, 10)	-- ȷ����ť
			msg.AddMsgEvent(MsgID, 1, 206)	-- "ȷ��"
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		elseif x > 10 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 2110)	-- "�´����տ��٣�\n    ���ε���ϲ�ֻ��������##�⡣��ϲ������10�⣬�����###�㾭���⣬�����������´�С���1ö����ȷ���������������㹻��λ����ȡ����������������������ձ��������콱��\n   [С��ʾ������δ��ȡ������²��ܲμ���һ�ֵĵ���ϲ�ֻᡣ]
			msg.AddMsgEvent(MsgID, 9, x)
			msg.AddMsgEvent(MsgID, 9, reward)
			msg.AddMsgEvent(MsgID, 21, 10)	-- ȷ����ť
			msg.AddMsgEvent(MsgID, 1, 2111)	-- "��ȡ��Ʒ"
			msg.AddMsgEvent(MsgID, 21, 9)	-- ȷ����ť
			msg.AddMsgEvent(MsgID, 1, 2112)	-- "�Ժ���ȡ"
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
        end
	end
end]]




-- ���ز��ظ�����Ŀ
function XJ_DM_GetQuestion(RoleID, XJ_DM_CallDeep)
	XJ_DM_CallDeep = XJ_DM_CallDeep + 1		-- �ݹ����+1
	local question
	local index
	index = math.random(6, XJ_XiaoLongNV_Question_Num)
	question = XJ_DengMi_Question_Table[index]

	local flag = 0
	for k, v  in pairs(XJ_XiaoLongNV_Questions[RoleID].Quest) do
		if v == question.Content then
			flag = 1
			break
		end
	end

	if flag == 1 then
		if XJ_DM_CallDeep >= 5 then			-- �ݹ���ȳ���5�Ļ���ֱ�ӷ����ظ�����
			return question
		else
			return XJ_DM_GetQuestion(RoleID, XJ_DM_CallDeep)
		end
	else
		return question
	end
end

-- ������Ŀ��ѡ��
function XJ_XiaoLongNV_GenerateQuestion(RoleID, TargetID)
	local XJ_DM_CallDeep = 0
	local question = XJ_DM_GetQuestion(RoleID, XJ_DM_CallDeep)
	XJ_XiaoLongNV_Questions[RoleID].NUM = XJ_XiaoLongNV_Questions[RoleID].NUM + 1
	XJ_XiaoLongNV_Questions[RoleID].ID = question.Content
	local k = XJ_XiaoLongNV_Questions[RoleID].NUM
    XJ_XiaoLongNV_Questions[RoleID].Quest[k] = question.Content
	if k == 20 then                                           --��һ��ܹؼ����������ڵڶ�ʮ����Ŀû��ѡ������ܻ��޷���ý�����
	    XJ_XiaoLongNV_RewardsRecevied[RoleID] = 1
		XJ_XiaoLongNV_Attend[RoleID] = 1		               -- �Ѳμӹ����λ
	    local CurTime = tonumber(os.date("%j"))
		role.SetRoleScriptData(RoleID, 1, RoleDataType["XJ_Dati_Date"], CurTime)
	end


	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 40, question.Content)	-- ��Ŀ����
	msg.AddMsgEvent(MsgID, 1, 217)
	msg.AddMsgEvent(MsgID, 12, XJ_XiaoLongNV_Role_Point[RoleID])		-- �÷�

	-- ��ȷ�����ĸ�ѡ���е����λ��
	XJ_XiaoLongNV_RightAnswer[RoleID] = math.random(0, 3)
	local index = 0
	for i=0, 3 do
		if XJ_XiaoLongNV_RightAnswer[RoleID] == i then
			msg.AddMsgEvent(MsgID, 21, i)
			msg.AddMsgEvent(MsgID, 1, question.RightAnswer)
		else
			msg.AddMsgEvent(MsgID, 21, i)
			msg.AddMsgEvent(MsgID, 1, question.WrongAnswer1+index)
			index = index + 1
		end
	end
	-- ����ǰʱ�����table��
	XJ_XiaoLongNV_Time[RoleID] = os.time()

	-- ����ʱ30��
	msg.AddMsgEvent(MsgID, 9, XJ_XiaoLongNV_Time_Counter)

	-- �ѻش���Ŀ��
	msg.AddMsgEvent(MsgID, 12, XJ_XiaoLongNV_Questions[RoleID].NUM - 1)
	msg.AddMsgEvent(MsgID, 24, TargetID)
	msg.DispatchRoleMsgEvent(RoleID, MsgID)

end

function XJ_DengMi_Time(RoleID)
    local CurTime = tonumber(os.date("%j"))
	local UpdateTime = role.GetRoleScriptData(RoleID, 1, RoleDataType["XJ_Dati_Date"])
	if CurTime ~= UpdateTime then
	    return true
	else
	    return false
	end
end

