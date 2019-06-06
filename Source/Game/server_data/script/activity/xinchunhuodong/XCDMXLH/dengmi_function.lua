
-- ��ʼ���ձ��Ĭ��ֵ
function XinChunDengMi_InitNullTable(RoleID)
	-- ����һЩ�ձ��Ĭ��ֵ
	if XinChunDengMi_Attend[RoleID] == nil then
		XinChunDengMi_Attend[RoleID] = 0
	end
	if XinChunDengMi_Questions[RoleID] == nil then
		XinChunDengMi_Questions[RoleID] = {NUM = 0, ID = 0, Quest = {} }  --NUMΪ��Ҫ���������һ��20����idΪ��Ҫ�����ţ�QuestΪ����¼���д�������
	end
    if XinChunDengMi_Role_Point[RoleID] == nil then
        XinChunDengMi_Role_Point[RoleID] = 0
    end
	if XinChunDengMi_RewardsRecevied[RoleID] == nil then
	    XinChunDengMi_RewardsRecevied[RoleID] = 0
	end
end


-- ��ʼ�����
function XinChunDengMi_InitQuestionTable()
	-- ��id_msg�е��մ������ݷ������
	for i=1, XinChunDengMi_Question_Num do
		local id = 150001 + (i-1)*5
		DengMi_Question_Table[i] = {Content = id, RightAnswer = id+1, WrongAnswer1 = id+2, WrongAnswer2 = id+3, WrongAnswer3 = id+4}
	end

end

--[[�����������ҽ���
function XinChunDengMi_RewardRole(MapID, InstanceID, RoleID, TargetID, TalkIndex)
    if TalkIndex == 10 then
	    local level = role.GetRoleLevel(MapID, InstanceID, RoleID)
		local x = XinChunDengMi_Role_Point[RoleID]/10
		local reward
		if x <= 0 or x == nil or x > 20 then
		    reward = 0
		else
		    reward = (300*x*level*level+x*dengmi_levelexp[level]+30000*level)/100/level
		end
		if x < 10 and x >=0 then
			role.AddRoleExp(MapID, InstanceID, RoleID, reward)
			XinChunDengMi_Role_Point[RoleID] = 0
			XinChunDengMi_RewardsRecevied[RoleID] = 0
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
	            XinChunDengMi_Role_Point[RoleID] = 0
			    XinChunDengMi_RewardsRecevied[RoleID] = 0
	        end
		end
	elseif TalkIndex ~=10 then
		local level = role.GetRoleLevel(MapID, InstanceID, RoleID)
		local x = XinChunDengMi_Role_Point[RoleID]/10
		local reward
		if x <= 0 or x == nil then
		    reward = 0
		else
		    reward = (300*x*level*level+x*dengmi_levelexp[level]+30000*level)/100/level
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
function DM_GetQuestion(RoleID, DM_CallDeep)
	DM_CallDeep = DM_CallDeep + 1		-- �ݹ����+1
	local question
	local index
	index = math.random(1, XinChunDengMi_Question_Num)
	question = DengMi_Question_Table[index]

	local flag = 0
	for k, v  in pairs(XinChunDengMi_Questions[RoleID].Quest) do
		if v == question.Content then
			flag = 1
			break
		end
	end

	if flag == 1 then
		if DM_CallDeep >= 5 then			-- �ݹ���ȳ���5�Ļ���ֱ�ӷ����ظ�����
			return question
		else
			return DM_GetQuestion(RoleID, DM_CallDeep)
		end
	else
		return question
	end
end

-- ������Ŀ��ѡ��
function XinChunDengMi_GenerateQuestion(RoleID, TargetID)
	local DM_CallDeep = 0
	local question = DM_GetQuestion(RoleID, DM_CallDeep)
	XinChunDengMi_Questions[RoleID].NUM = XinChunDengMi_Questions[RoleID].NUM + 1
	XinChunDengMi_Questions[RoleID].ID = question.Content
	local k = XinChunDengMi_Questions[RoleID].NUM
    XinChunDengMi_Questions[RoleID].Quest[k] = question.Content
	if k == 20 then                                           --��һ��ܹؼ����������ڵڶ�ʮ����Ŀû��ѡ������ܻ��޷���ý�����
	    XinChunDengMi_RewardsRecevied[RoleID] = 1
		XinChunDengMi_Attend[RoleID] = 1		               -- �Ѳμӹ����λ
	end


	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 40, question.Content)	-- ��Ŀ����
	msg.AddMsgEvent(MsgID, 1, 217)
	msg.AddMsgEvent(MsgID, 12, XinChunDengMi_Role_Point[RoleID])		-- �÷�

	-- ��ȷ�����ĸ�ѡ���е����λ��
	XinChunDengMi_RightAnswer[RoleID] = math.random(0, 3)
	local index = 0
	for i=0, 3 do
		if XinChunDengMi_RightAnswer[RoleID] == i then
			msg.AddMsgEvent(MsgID, 21, i)
			msg.AddMsgEvent(MsgID, 1, question.RightAnswer)
		else
			msg.AddMsgEvent(MsgID, 21, i)
			msg.AddMsgEvent(MsgID, 1, question.WrongAnswer1+index)
			index = index + 1
		end
	end
	-- ����ǰʱ�����table��
	XinChunDengMi_Time[RoleID] = os.time()

	-- ����ʱ30��
	msg.AddMsgEvent(MsgID, 9, XinChunDengMi_Time_Counter)

	-- �ѻش���Ŀ��
	msg.AddMsgEvent(MsgID, 12, XinChunDengMi_Questions[RoleID].NUM - 1)
	msg.AddMsgEvent(MsgID, 24, TargetID)
	msg.DispatchRoleMsgEvent(RoleID, MsgID)

end

function DengMi_Time()
    local curhour = tonumber(os.date("%H"))
	local curmin = tonumber(os.date("%M"))
	if (curhour == 11 or curhour == 19) and curmin >= 30 then
	    return true
	else
	    return false
	end
end

