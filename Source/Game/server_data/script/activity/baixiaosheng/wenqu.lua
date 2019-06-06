WenQu_Attend = {}					-- �Ƿ��ڴ˻ʱ���ڲμӹ�
WenQu_Questions = {}				-- ������
WenQu_RightAnswer = {}				-- ��ȷ�����ĸ�ѡ���е�����
WenQu_Normal_Question_Point = 10	-- ������ͨ��Ŀ�ķ���
WenQu_Hard_Question_Point = 20 		-- ����������Ŀ�ķ���
WenQu_Time = {}						-- ��Ŵ��⿪ʼʱ��ϵͳʱ�䣬��Ӧ��ɫid
WenQu_Role_Point = {}				-- ��һ�õķ�������Ӧ��ɫid
WenQu_Stepindex = {}				-- ÿ����ҵĻ����
WenQu_Question_Type = {}			-- ��ǰ��Ŀ���ͣ��������ѻ�������ͨ
WenQu_Role_Level = {}				-- ��ҵ�ǰ�ȼ�
WenQu_Time_Counter = 30				-- ����ĵ���ʱ
BaiXiaoYin_TypeID = 2612616			-- ����ӡ��typeid
WenQu_Current_Question = {}      	-- ��ҵ�ǰ����

BaiXiaoSheng_Hard_Question_Num = 212		-- �������ѵ���Ŀ��
BaiXiaoSheng_Normal_Question_Num = 259	-- ������ͨ����Ŀ��
WenQu_Hard_Question_Num = 126			-- �������ѵ���Ŀ��
WenQu_Normal_Question_Num = 195			-- ������ͨ����Ŀ��

WQ_CallDeep = 0							-- �ݹ�������

WenQu_Hard_Question_Table = {}
WenQu_Normal_Question_Table = {}

WenQu_Npc_Accept	= 	0			-- �Ƿ���Բμӻ

g_WQ_RoleNum = 0						-- �μ�������������Ҹ���
tbl_WQ_RoleID = {}						-- ��g_WQ_RoleNumΪ������Ӧ��ҵ�RoleID
tbl_WQ_Index = {}						-- ��RoleIDΪ������Ӧ��ҵĴ���Index
tbl_WQ_Questions = {}					-- ����Ѿ��ش������Ŀ

-- npc�Ի��������������
function x3020751_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)

	if WenQu_Npc_Accept == 1 then

		if TalkIndex == -1 then
			-- ��ʼ���ձ�
			WenQu_InitNullTable(RoleID)
		end

		-- �õ�������
		WenQu_Role_Level[tbl_WQ_Index[RoleID]] = role.GetRoleLevel(MapID, InstanceID, RoleID)

		if WenQu_Stepindex[tbl_WQ_Index[RoleID]] == 1 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 213)	-- "���Ƿ�����μӻ����������ǻ��"
			msg.AddMsgEvent(MsgID, 21, 4)	-- ȷ����ť
			msg.AddMsgEvent(MsgID, 1, 206)	-- "ȷ��"

			msg.AddMsgEvent(MsgID, 21, 5)	-- ȡ����ť
			msg.AddMsgEvent(MsgID, 1, 207)	-- "ȡ��"

			msg.AddMsgEvent(MsgID, 24, TargetID)
			WenQu_Stepindex[tbl_WQ_Index[RoleID]] = 2
			msg.DispatchRoleMsgEvent(RoleID, MsgID)

		elseif WenQu_Stepindex[tbl_WQ_Index[RoleID]] == 2 then
			if TalkIndex == 4 then			-- ѡ����ȷ���μӰ�ť��
				if WenQu_Role_Level[tbl_WQ_Index[RoleID]] <= 15 then	-- �ȼ�����15���Ļ�������ʾ
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 20, 214)		-- "�����Ǿ��������15�����ϲſɲ���"
					msg.AddMsgEvent(MsgID, 21, 4)		-- ȷ����ť
					msg.AddMsgEvent(MsgID, 1, 206)		-- "ȷ��"
					WenQu_Stepindex[tbl_WQ_Index[RoleID]] = 1
					msg.DispatchRoleMsgEvent(RoleID, MsgID)

				elseif role.GetRoleItemNum(RoleID, BaiXiaoYin_TypeID) < 5 then
					-- ����ӡ��С��5���޷��μ�
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 20, 214)		-- "��Ҫ5������ӡ"
					msg.AddMsgEvent(MsgID, 21, 4)		-- ȷ����ť
					msg.AddMsgEvent(MsgID, 1, 206)		-- "ȷ��"
					WenQu_Stepindex[tbl_WQ_Index[RoleID]] = 1
					msg.DispatchRoleMsgEvent(RoleID, MsgID)

				elseif WenQu_Attend[tbl_WQ_Index[RoleID]] == 1 then	-- �ڴ˻ʱ�����Ѳμӹ�
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 20, 215)		-- "���Ѳμӹ����������Ǿ�����ɼ����μ��¸�ʱ��Ļ��"
					msg.AddMsgEvent(MsgID, 21, 4)		-- ȷ����ť
					msg.AddMsgEvent(MsgID, 1, 206)		-- "ȷ��"
					WenQu_Stepindex[tbl_WQ_Index[RoleID]] = 1
					msg.DispatchRoleMsgEvent(RoleID, MsgID)

				else
					-- ���������ڿ�ʼ��
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 20, 205)
					msg.AddMsgEvent(MsgID, 21, 4)
					msg.AddMsgEvent(MsgID, 1, 206)
					msg.AddMsgEvent(MsgID, 24, TargetID)
					WenQu_Stepindex[tbl_WQ_Index[RoleID]] = 3
					msg.DispatchRoleMsgEvent(RoleID, MsgID)

					-- �ӱ����м�ȥ5������ӡ
					role.RemoveFromRole(MapID, InstanceID, RoleID, BaiXiaoYin_TypeID, 5, 300)
					-- ��ʼ����ҵ÷�
					role.SetRoleScriptData(RoleID, 1, RoleDataType["RolePoint"], 0)
					WenQu_Attend[tbl_WQ_Index[RoleID]] = 1				-- �Ѳμӹ����λ
					WenQu_Current_Question[tbl_WQ_Index[RoleID]] = {question, index}
					-- ��MapID��InstanceID����scriptdata
					role.SetRoleScriptData(RoleID, 1, RoleDataType["RoleMapID"], MapID)
					role.SetRoleScriptData(RoleID, 1, RoleDataType["RoleInstanceID"], MapID)

				end
			elseif TalkIndex == 5 then
				WenQu_Stepindex[tbl_WQ_Index[RoleID]] = 1
			else
				WenQu_Stepindex[tbl_WQ_Index[RoleID]] = 1
			end


		elseif WenQu_Stepindex[tbl_WQ_Index[RoleID]] == 3 then				-- ���⿪ʼ

			-- ������Ŀ
			WenQu_GenerateQuestion(RoleID, TargetID)

			WenQu_Stepindex[tbl_WQ_Index[RoleID]] = 4

		elseif WenQu_Stepindex[tbl_WQ_Index[RoleID]] == 4 then	-- �����������ʱ��
			if TalkIndex == 5 then						-- �����������ҵ�ǰ�÷�Ӧ�õĽ���
				WenQu_RewardRole(MapID, InstanceID, RoleID)
			else
				-- ������⻨�ѵ�ʱ��
				local time1 = WenQu_Time[tbl_WQ_Index[RoleID]]
				local dtime = os.difftime(os.time(), time1)

				-- �жϴ��Ƿ���ȷ
				if WenQu_RightAnswer[tbl_WQ_Index[RoleID]] == TalkIndex then
					local pt = role.GetRoleScriptData(RoleID, 1, RoleDataType["RolePoint"])
					if pt == nil then
						pt = 0
					end

					-- �ش�������ʱ�䲻ͬ���÷ֲ�ͬ
					if dtime <= 10 then				-- 10���ڻش����ȷ�𰸣������Ŀȫ������
						if WenQu_Question_Type[tbl_WQ_Index[RoleID]] == 1 then  	-- �������ѵ÷�
							pt = pt + WenQu_Hard_Question_Point

						else							-- ������ͨ�÷�
							pt = pt + WenQu_Normal_Question_Point

						end

					elseif dtime <= 20 then			-- 20���ڻش����ȷ�𰸣������Ŀһ�����
						if WenQu_Question_Type[tbl_WQ_Index[RoleID]] == 1 then  	-- �������ѵ÷�
							pt = pt + WenQu_Hard_Question_Point*0.5

						else							-- ������ͨ�÷�
							pt = pt + WenQu_Normal_Question_Point*0.5

						end
					else								-- 30���ڻش����ȷ�𰸣����20%����Ŀ����
						if WenQu_Question_Type[tbl_WQ_Index[RoleID]] == 1 then  	-- �������ѵ÷�
							pt = pt + WenQu_Hard_Question_Point*0.2

						else							-- ������ͨ�÷�
							pt = pt + WenQu_Normal_Question_Point*0.2
						end
					end

					role.SetRoleScriptData(RoleID, 1, RoleDataType["RolePoint"], pt)
				end

				-- ������Ŀ
				if WenQu_Questions[tbl_WQ_Index[RoleID]] <= 50 then    -- ������С��50��ʱ����������Ŀ
					WenQu_GenerateQuestion(RoleID, TargetID)
				else
					-- �������
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 40, -1)
					msg.DispatchRoleMsgEvent(RoleID, MsgID)

					WenQu_RewardRole(MapID, InstanceID, RoleID)
					WenQu_Stepindex[tbl_WQ_Index[RoleID]] = 1
				end
				-- �������
				if (role.IsRoleOnLine(MapID, InstanceID, RoleID) == 0) then
					-- ����Ѿ��μӹ�������õ�ǰ������Ӧ�Ľ��������һ�Զ���ֹ
					if (WenQu_Attend[tbl_WQ_Index[RoleID]] == 1) then
						WenQu_RewardRole(MapID, InstanceID, RoleID)
						WenQu_Stepindex[tbl_WQ_Index[RoleID]] = 1
					end
				end
			end
		end
	else
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 218)
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	end
end


-- ��ʼ���ձ��Ĭ��ֵ
function WenQu_InitNullTable(RoleID)
	-- ����һЩ�ձ��Ĭ��ֵ
	if tbl_WQ_Index[RoleID] == nil then
		g_WQ_RoleNum = g_WQ_RoleNum + 1
		tbl_WQ_RoleID[g_WQ_RoleNum] = RoleID
		tbl_WQ_Index[RoleID] = g_WQ_RoleNum
		tbl_WQ_Questions[g_WQ_RoleNum] = {}
	end
	if WenQu_Stepindex[g_WQ_RoleNum] == nil then
		WenQu_Stepindex[g_WQ_RoleNum] = 1
	end
	if WenQu_Attend[g_WQ_RoleNum] == nil then
	    WenQu_Attend[g_WQ_RoleNum] = 0
	end
	if WenQu_Questions[g_WQ_RoleNum] == nil then
		WenQu_Questions[g_WQ_RoleNum] = 1
	end
	if WenQu_Question_Type[g_WQ_RoleNum] == nil then
	    WenQu_Question_Type[g_WQ_RoleNum] = 0
	end

end

-- ��ʼ�����
function WenQu_InitQuestionTable()
	-- ��id_msg���������ѺͰ������Ѵ������ݷ������
	local x = 1
	for j=0, BaiXiaoSheng_Hard_Question_Num-1 do
		local id = 100001 + j*5
		WenQu_Hard_Question_Table[x] = {Content = id, RightAnswer = id+1, WrongAnswer1 = id+2, WrongAnswer2 = id+3, WrongAnswer3 = id+4}
		x = x + 1
	end

	for j=0, WenQu_Hard_Question_Num-1 do
		local id = 110001 + j*5
		WenQu_Hard_Question_Table[x] = {Content = id, RightAnswer = id+1, WrongAnswer1 = id+2, WrongAnswer2 = id+3, WrongAnswer3 = id+4}
		x = x + 1
	end

	-- ��id_msg��������ͨ�Ͱ�����ͨ�������ݷ������
	local y = 1
	for j=0, BaiXiaoSheng_Normal_Question_Num-1 do
		local id = 120001 + j*5
		WenQu_Normal_Question_Table[y] = {Content = id, RightAnswer = id+1, WrongAnswer1 = id+2, WrongAnswer2 = id+3, WrongAnswer3 = id+4}
		y = y + 1
	end

	for j=0, WenQu_Normal_Question_Num-1 do
		local id = 130001 + j*5
		WenQu_Normal_Question_Table[y] = {Content = id, RightAnswer = id+1, WrongAnswer1 = id+2, WrongAnswer2 = id+3, WrongAnswer3 = id+4}
		y = y + 1
	end

end

-- ���ز��ظ�����Ŀ  ����0Ϊ������Ŀ 1Ϊ��ͨ��Ŀ
function WQ_GetQuestion(RoleIndex, n)
	WQ_CallDeep = WQ_CallDeep + 1		-- �ݹ����+1
	local question
	local index
	if n == 0 then
		if table.maxn(WenQu_Hard_Question_Table) == 0 then	--�ж�֮ǰ�Ļ���й����У��Ƿ������ؽű��������������ԭ����������ÿ�
			WenQu_InitQuestionTable()
		end
		index = math.random(1, table.maxn(WenQu_Hard_Question_Table))
		question = WenQu_Hard_Question_Table[index]
	else
		if table.maxn(WenQu_Normal_Question_Table) == 0 then	--�ж�֮ǰ�Ļ���й����У��Ƿ������ؽű��������������ԭ����������ÿ�
			WenQu_InitQuestionTable()
		end
		index = math.random(1, table.maxn(WenQu_Normal_Question_Table))
		question = WenQu_Normal_Question_Table[index]
	end

	if tbl_WQ_Questions[RoleIndex] == nil then
		local question_table = {}
		tbl_WQ_Questions[RoleIndex] = {question_table}
		table.insert(tbl_WQ_Questions[RoleIndex], question.Content)
		return question
	end

	local flag = 0
	for k, v in pairs(tbl_WQ_Questions[RoleIndex]) do
		if v == question.Content then
			flag = 1
			break
		end
	end

	if flag == 1 then
		if WQ_CallDeep >= 30 then			-- �ݹ���ȳ���10�Ļ���ֱ�ӷ����ظ�����
			return question
		else
			return WQ_GetQuestion(RoleIndex, n)
		end
	else
		table.insert(tbl_WQ_Questions[RoleIndex], question.Content)
		return question
	end
end

-- ������Ŀ��ѡ��
function WenQu_GenerateQuestion(RoleID, TargetID)
	local RoleIndex = tbl_WQ_Index[RoleID]
	if WenQu_Questions[RoleIndex] > 30 then  -- ����������30��ʱ��ȡ�������Ѻ��������������Ŀ

		-- �����ѱ��������ȡ��һ����
		WQ_CallDeep = 0
		WenQu_Current_Question[RoleIndex].question = WQ_GetQuestion(RoleIndex, 0)

		local MsgID = msg.BeginMsgEvent()

		msg.AddMsgEvent(MsgID, 40, WenQu_Current_Question[RoleIndex].question.Content)	-- ��Ŀ����
		msg.AddMsgEvent(MsgID, 1, 217)
		local pt = role.GetRoleScriptData(RoleID, 1, RoleDataType["RolePoint"])
		msg.AddMsgEvent(MsgID, 12, pt)		-- �÷�

		-- ��ȷ�����ĸ�ѡ���е����λ��
		WenQu_RightAnswer[RoleIndex] = math.random(0, 3)
		local index = 0
		for i=0, 3 do									-- �ĸ�ѡ��
			if WenQu_RightAnswer[RoleIndex] == i then
				msg.AddMsgEvent(MsgID, 21, i)			-- ѡ��
				msg.AddMsgEvent(MsgID, 1, WenQu_Current_Question[RoleIndex].question.RightAnswer)	-- ѡ������
			else
				msg.AddMsgEvent(MsgID, 21, i)
				msg.AddMsgEvent(MsgID, 1, WenQu_Current_Question[RoleIndex].question.WrongAnswer1+index)
				index = index + 1
			end
		end

		WenQu_Question_Type[RoleIndex] = 1		-- ��ǰ��Ŀ����
		-- ����ǰʱ�����table��
		WenQu_Time[RoleIndex] = os.time()

		-- ����ʱ
		msg.AddMsgEvent(MsgID, 9, WenQu_Time_Counter)
		-- �ѻش���Ŀ��
		msg.AddMsgEvent(MsgID, 12, WenQu_Questions[RoleIndex]-1)


		msg.AddMsgEvent(MsgID, 24, TargetID)
		WenQu_Questions[RoleIndex] = WenQu_Questions[RoleIndex] + 1 	-- �Ѵ�����+1
		msg.DispatchRoleMsgEvent(RoleID, MsgID)

	-- ������ͨ��Ŀ
	else
		WQ_CallDeep = 0
		WenQu_Current_Question[RoleIndex].question = WQ_GetQuestion(RoleIndex, 1)

		local MsgID = msg.BeginMsgEvent()

		msg.AddMsgEvent(MsgID, 40, WenQu_Current_Question[RoleIndex].question.Content)	-- ��Ŀ����
		msg.AddMsgEvent(MsgID, 1, 217)
		local pt = role.GetRoleScriptData(RoleID, 1, RoleDataType["RolePoint"])
		msg.AddMsgEvent(MsgID, 12, pt)		-- �÷�

		-- ��ȷ�����ĸ�ѡ���е����λ��
		WenQu_RightAnswer[RoleIndex] = math.random(0, 3)
		local index = 0
		for i=0, 3 do
			if WenQu_RightAnswer[RoleIndex] == i then
				msg.AddMsgEvent(MsgID, 21, i)
				msg.AddMsgEvent(MsgID, 1, WenQu_Current_Question[RoleIndex].question.RightAnswer)
			else
				msg.AddMsgEvent(MsgID, 21, i)
				msg.AddMsgEvent(MsgID, 1, WenQu_Current_Question[RoleIndex].question.WrongAnswer1+index)
				index = index + 1
			end
		end

		WenQu_Question_Type[RoleIndex] = 0		-- ��ǰ��Ŀ����
		-- ����ǰʱ�����table��
		WenQu_Time[RoleIndex] = os.time()

		-- ����ʱ30��
		msg.AddMsgEvent(MsgID, 9, WenQu_Time_Counter)
		-- �ѻش���Ŀ��
		msg.AddMsgEvent(MsgID, 12, WenQu_Questions[RoleIndex]-1)

		msg.AddMsgEvent(MsgID, 24, TargetID)
		WenQu_Questions[RoleIndex] = WenQu_Questions[RoleIndex] + 1 	-- �Ѵ�����+1
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	end

end

-- �����������ҵ��ߣ�������ҽ���
function WenQu_RewardRole(MapID, InstanceID, RoleID)

	-- ��Ǯ
	local pt = role.GetRoleScriptData(RoleID, 1, RoleDataType["RolePoint"])
	local addmoney = 20000 * pt / 360
	role.AddRoleSilver(MapID, InstanceID, RoleID, addmoney, 0)

	-- ����
	local LevelUpNeedExp = role.GetRoleLevelUpExp(MapID, InstanceID, RoleID)
	local addexp = LevelUpNeedExp * 150 / (WenQu_Role_Level[tbl_WQ_Index[RoleID]]+10)^2 * pt / 360 * 2
	role.AddRoleExp(MapID, InstanceID, RoleID, addexp)

	-- �����id�͵÷ִ���WenQu_Role_Point
	table.insert(WenQu_Role_Point, {id = RoleID, point = pt})

	-- ������Ʒ(δ���)


	--���ӽ���ʵ��ֵ

	Increase_shili(MapID, InstanceID, RoleID, 0, pt/10, "jinjieshili_A")

	WenQu_Stepindex[tbl_WQ_Index[RoleID]] = 1
end


-- ���ʼ
function WenQu_OnStart(actID)
	g_WQ_RoleNum = 0
	tbl_WQ_RoleID = {}
	tbl_WQ_Index = {}
	tbl_WQ_Questions = {}

	-- �����������
	math.randomseed(os.time())
	-- ��ʼ�����
	WenQu_InitQuestionTable()
	-- ���Դ�npc��ȡ����
	WenQu_Npc_Accept = 1

	-- �㲥�����Ǿ����ʼ
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 100, 212)
	msg.DispatchWorldMsgEvent(MsgID)

end


-- �������
function WenQu_OnEnd(actID)

	WenQu_Npc_Accept = 0	-- �����

	-- ��WenQu_Role_Point�������ݰ��÷�ֵ��������
	table.sort(WenQu_Role_Point, function(a,b) return a.point > b.point end)

	-- �����ǰ10��
	local rolenum = table.getn(WenQu_Role_Point)

	if rolenum > 0 then
		local n = 0
		if rolenum >= 10 then
			n = 10
		else
			n = rolenum
		end

		-- ����״Ԫ��ǰ10��RoleID
		local roleid = {}
		if n ~= 0 then
			local i = 0
			for k, v in pairs(WenQu_Role_Point) do
				-- ��RoleID����roleid�У�����ǰ10��ʱ�õ�
				roleid[i] = v.id
				i = i + 1
				if i >= n then
					break
				end
			end
		end

		-- �㲥״Ԫ�������
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 100, 216)
		msg.AddMsgEvent(MsgID, 2, roleid[0])
		msg.DispatchWorldMsgEvent(MsgID)

		-- ����״Ԫ��ǰ10��
		-- ״Ԫ
		local mapid = role.GetRoleScriptData(roleid[0], 1, RoleDataType["RoleMapID"])
		local instanceid = role.GetRoleScriptData(roleid[0], 1, RoleDataType["RoleInstanceID"])
		if mapid ~= nil then
			role.AddRoleSilver(mapid, instanceid, roleid[0], 100000, 104)
			-- �������ߣ�Buff ����ԣ�������
			role.AddRoleItem(mapid, instanceid, roleid[0], 2612630, 1, -1, 3, 104)
		end

		-- ����
		mapid = role.GetRoleScriptData(roleid[1], 1, RoleDataType["RoleMapID"])
		instanceid = role.GetRoleScriptData(roleid[1], 1, RoleDataType["RoleInstanceID"])
		if mapid ~= nil then
			role.AddRoleSilver(mapid, instanceid, roleid[1], 80000, 104)
			-- �������ߣ�Buff ����ԣ�������
			role.AddRoleItem(mapid, instanceid, roleid[1], 2612631, 1, -1, 3, 104)
		end

		-- ̽��
		mapid = role.GetRoleScriptData(roleid[2], 1, RoleDataType["RoleMapID"])
		instanceid = role.GetRoleScriptData(roleid[2], 1, RoleDataType["RoleInstanceID"])
		if mapid ~= nil then
			role.AddRoleSilver(mapid, instanceid, roleid[2], 50000, 104)
			-- �������ߣ�Buff ����ԣ�������
			role.AddRoleItem(mapid, instanceid, roleid[2], 2612632, 1, -1, 3, 104)
		end

		-- ����ʣ�µ�
		for i=3, n do
			mapid = role.GetRoleScriptData(roleid[i], 1, RoleDataType["RoleMapID"])
			instanceid = role.GetRoleScriptData(roleid[i], 1, RoleDataType["RoleInstanceID"])
			if mapid ~= nil then
				role.AddRoleSilver(mapid, instanceid, roleid[i], 20000, 104)
				-- �������ߣ�Buff ����ԣ�������
			end
			i = i + 1
		end
	end

	-- ��ձ�������
	WenQu_Attend = {}
	WenQu_Questions = {}
	WenQu_RightAnswer = {}
	WenQu_Time = {}
	WenQu_Role_Point = {}
	WenQu_Stepindex = {}
	WenQu_Question_Type = {}
	WenQu_Hard_Question_Table = {}
	WenQu_Normal_Question_Table = {}
	WenQu_Role_Level = {}
	WenQu_Current_Question = {}
	tbl_WQ_RoleID = {}
	tbl_WQ_Index = {}
	tbl_WQ_Questions = {}
	g_WQ_RoleNum = 0
end

-- ע����Ӧ�¼�
aux.RegisterCreatureEvent(3020751, 7, "x3020751_OnTalk")
aux.RegisterActEvent(42, 3, "WenQu_OnEnd")
aux.RegisterActEvent(42, 2, "WenQu_OnStart")
aux.RegisterActEvent(43, 3, "WenQu_OnEnd")
aux.RegisterActEvent(43, 2, "WenQu_OnStart")
