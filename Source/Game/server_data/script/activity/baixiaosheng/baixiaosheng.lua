BaiXiaoSheng_CreatureID = {}			-- ��Ŵ����������󷵻ص�id�����ڻ������ɾ��npc
BaiXiaoSheng_Attend = {}				-- �Ƿ��ڴ˻ʱ���ڲμӹ�
BaiXiaoSheng_Questions = {}				-- ������
BaiXiaoSheng_RightAnswer = {}			-- ��ȷ�����ĸ�ѡ���е�����
BaiXiaoSheng_Normal_Question_Point = 10	-- ������ͨ��Ŀ�ķ���
BaiXiaoSheng_Hard_Question_Point = 20 	-- ����������Ŀ�ķ���
BaiXiaoSheng_Time = {}					-- ��Ŵ��⿪ʼʱ��ϵͳʱ�䣬��Ӧ��ɫid
BaiXiaoSheng_Role_Point = {}			-- ��һ�õķ�������Ӧ��ɫid
BaiXiaoSheng_Stepindex = {}				-- ÿ����ҵĻ����
BaiXiaoSheng_Question_Type = {}			-- ��ǰ��Ŀ���ͣ��������ѻ������ͨ
BaiXiaoSheng_Role_Level = {}			-- ��ҵ�ǰ�ȼ�
BaiXiaoSheng_Time_Counter = 45			-- ����ĵ���ʱ
BaiXiaoSheng_Current_Question = {}      -- ��ҵ�ǰ����

BaiXiaoSheng_Hard_Question_Num = 212		-- �������ѵ���Ŀ��
BaiXiaoSheng_Normal_Question_Num = 259	-- ������ͨ����Ŀ��

BX_CallDeep = 0							-- �ݹ�������

BaiXiaoSheng_SpeciallyQuestionStatus = {}	--�����һ�λ���Ƿ��������������һ����Ե��
BaiXiaoSheng_SpeciallyQuestion_Num = 11	--��Ե������
BaiXiaoSheng_SpeciallyQuestion_Point = 50	--��Ե��Ŀ����
BaixiaoSheng_RoleBliss = {}	--��Ҹ�Եֵ��¼

BaiXiao_Hard_Question_Table = {}
BaiXiao_Normal_Question_Table = {}

g_BX_RoleNum = 0						-- �μӰ�������������Ҹ���
tbl_BX_RoleID = {}						-- ��g_BX_RoleNumΪ������Ӧ��ҵ�RoleID
tbl_BX_Index = {}						-- ��RoleIDΪ������Ӧ��ҵĴ���Index
tbl_BX_Questions = {}					-- ����Ѿ��ش������Ŀ

-- npc�Ի��������������
function x3020750_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	if TalkIndex == -1 then
		-- ��ʼ���ձ�
		BaiXiaoSheng_InitNullTable(RoleID)
	end

	-- �õ�������
	BaiXiaoSheng_Role_Level[tbl_BX_Index[RoleID]] = role.GetRoleLevel(MapID, InstanceID, RoleID)
	if role.GetRoleAttValue(MapID,InstanceID,RoleID,70)~= nil then
		BaixiaoSheng_RoleBliss[RoleID] = role.GetRoleAttValue(MapID,InstanceID,RoleID,70)
	end

	if BaiXiaoSheng_Stepindex[tbl_BX_Index[RoleID]] == 1 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 202)	-- "���Ƿ�����μӻ��İ��������"
		msg.AddMsgEvent(MsgID, 21, 4)	-- ȷ����ť
		msg.AddMsgEvent(MsgID, 1, 206)	-- "ȷ��"

		msg.AddMsgEvent(MsgID, 21, 5)	-- ȡ����ť
		msg.AddMsgEvent(MsgID, 1, 207)	-- "ȡ��"

		msg.AddMsgEvent(MsgID, 24, TargetID)
		BaiXiaoSheng_Stepindex[tbl_BX_Index[RoleID]] = 2
		msg.DispatchRoleMsgEvent(RoleID, MsgID)

	elseif BaiXiaoSheng_Stepindex[tbl_BX_Index[RoleID]] == 2 then
		if TalkIndex == 4 then			-- ѡ����ȷ���μӰ�ť��
			if BaiXiaoSheng_Role_Level[tbl_BX_Index[RoleID]] <= 15 then	-- �ȼ�����15���Ļ�������ʾ
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 203)		-- "�������������15�����ϲſɲ���"
				msg.AddMsgEvent(MsgID, 21, 4)		-- ȷ����ť
				msg.AddMsgEvent(MsgID, 1, 206)		-- "ȷ��"
				BaiXiaoSheng_Stepindex[tbl_BX_Index[RoleID]] = 1
				msg.DispatchRoleMsgEvent(RoleID, MsgID)

			elseif BaiXiaoSheng_Attend[tbl_BX_Index[RoleID]] == 1 then	-- �ڴ˻ʱ�����Ѳμӹ�
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 204)		-- "���Ѳμӹ����ΰ���������ɼ����μ��¸�ʱ��Ļ��"
				msg.AddMsgEvent(MsgID, 21, 4)		-- ȷ����ť
				msg.AddMsgEvent(MsgID, 1, 206)		-- "ȷ��"
				BaiXiaoSheng_Stepindex[tbl_BX_Index[RoleID]] = 1
				msg.DispatchRoleMsgEvent(RoleID, MsgID)

			else
				-- ���������ڿ�ʼ��
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 205)
				msg.AddMsgEvent(MsgID, 21, 4)
				msg.AddMsgEvent(MsgID, 1, 206)
				msg.AddMsgEvent(MsgID, 24, TargetID)
				BaiXiaoSheng_Stepindex[tbl_BX_Index[RoleID]] = 3
				msg.DispatchRoleMsgEvent(RoleID, MsgID)

				BaiXiaoSheng_Attend[tbl_BX_Index[RoleID]] = 1				-- �Ѳμӹ����λ
				BaiXiaoSheng_Current_Question[tbl_BX_Index[RoleID]] = {question, index}
			end
		elseif TalkIndex == 5 then
			BaiXiaoSheng_Stepindex[tbl_BX_Index[RoleID]] = 1
		else
			BaiXiaoSheng_Stepindex[tbl_BX_Index[RoleID]] = 1
		end


	elseif BaiXiaoSheng_Stepindex[tbl_BX_Index[RoleID]] == 3 then				-- ���⿪ʼ
		BaiXiaoSheng_GenerateQuestion(RoleID, TargetID)
		BaiXiaoSheng_Stepindex[tbl_BX_Index[RoleID]] = 4


	elseif BaiXiaoSheng_Stepindex[tbl_BX_Index[RoleID]] == 4 then	-- �����������ʱ��
		if TalkIndex == 5 then						-- �����������ҵ�ǰ�÷�Ӧ�õĽ���
			BaiXiaoSheng_RewardRole(MapID, InstanceID, RoleID)
		else
			-- ������⻨�ѵ�ʱ��
			local time1 = BaiXiaoSheng_Time[tbl_BX_Index[RoleID]]
			local dtime = os.difftime(os.time(), time1)

			-- �жϴ��Ƿ���ȷ
			if BaiXiaoSheng_RightAnswer[tbl_BX_Index[RoleID]] == TalkIndex then
				-- �ش�������ʱ�䲻ͬ���÷ֲ�ͬ����������2.0�汾���Ѿ�ȥ����
				if dtime <= 45 then				-- 45���ڻش����ȷ�𰸣������Ŀ4����
					if BaiXiaoSheng_Question_Type[tbl_BX_Index[RoleID]] == 1 then  	-- �������ѵ÷�
						BaiXiaoSheng_Role_Point[tbl_BX_Index[RoleID]] = BaiXiaoSheng_Role_Point[tbl_BX_Index[RoleID]] + BaiXiaoSheng_Hard_Question_Point
					elseif BaiXiaoSheng_Question_Type[tbl_BX_Index[RoleID]] == 6 then
						BaiXiaoSheng_Role_Point[tbl_BX_Index[RoleID]] = BaiXiaoSheng_Role_Point[tbl_BX_Index[RoleID]] + BaiXiaoSheng_SpeciallyQuestion_Point
						BaiXiaoSheng_SpeciallyQuestionStatus[RoleID] = 0
						--��Ҵ�Ը�Ե��󲥷�ȫ���㲥������ҡ�<p1>����Ե��˫���ڰ����������ȷ�ش��˰������ر�׼���ĸ�Ե��Ŀ��һ�»����50����֣���
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 102, 100117)
						msg.AddMsgEvent(MsgID, 2, RoleID)
						RunBroad(MsgID, -1, -1, -1, -1, DPR_ALL, CFT_BROAD, 100117, 5)--�ù㲥�Ĳ���Ƶ��Ϊ����5����
					else							-- ������ͨ�÷�
						BaiXiaoSheng_Role_Point[tbl_BX_Index[RoleID]] = BaiXiaoSheng_Role_Point[tbl_BX_Index[RoleID]] + BaiXiaoSheng_Normal_Question_Point

					end
				end

			end

			if BaiXiaoSheng_Question_Type[tbl_BX_Index[RoleID]] == 6 and BaiXiaoSheng_SpeciallyQuestionStatus[RoleID] == 3 then
				--����ڰ�������д��3�θ�Ե�⣺����ҡ�<p1>���ڱ��ΰ�����������������θ�Ե��Ŀ����������Ȼȫ�����һ��δ�ã�ʵ���������������ţ���
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 102, 100118)
				msg.AddMsgEvent(MsgID, 2, RoleID)
				RunBroad(MsgID, -1, -1, -1, -1, DPR_ALL, CFT_BROAD, 100117, 2)--�ù㲥�Ĳ���Ƶ��Ϊ����2����
			end

			-- ������Ŀ
			if BaiXiaoSheng_Questions[tbl_BX_Index[RoleID]] <= 30 then    -- ������С��30��ʱ����������Ŀ
				BaiXiaoSheng_GenerateQuestion(RoleID, TargetID)
			else
				-- �������
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 40, -1)
				msg.DispatchRoleMsgEvent(RoleID, MsgID)

				BaiXiaoSheng_RewardRole(MapID, InstanceID, RoleID)
				BaiXiaoSheng_Stepindex[tbl_BX_Index[RoleID]] = 1
			end

			-- �������
			if (role.IsRoleOnLine(MapID, InstanceID, RoleID) == 0) then
				-- ����Ѿ��μӹ�������õ�ǰ������Ӧ�Ľ��������һ�Զ���ֹ
				if (BaiXiaoSheng_Attend[tbl_BX_Index[RoleID]] == 1) then
					BaiXiaoSheng_RewardRole(MapID, InstanceID, RoleID)
					BaiXiaoSheng_Stepindex[tbl_BX_Index[RoleID]] = 1
				end
			end
		end
	end
end


-- ��ʼ���ձ��Ĭ��ֵ
function BaiXiaoSheng_InitNullTable(RoleID)
	-- ����һЩ�ձ��Ĭ��ֵ
	if tbl_BX_Index[RoleID] == nil then
		g_BX_RoleNum = g_BX_RoleNum + 1
		tbl_BX_RoleID[g_BX_RoleNum] = RoleID
		tbl_BX_Index[RoleID] = g_BX_RoleNum
		tbl_BX_Questions[g_BX_RoleNum] = {}
	end

	if BaiXiaoSheng_Stepindex[g_BX_RoleNum] == nil then
		BaiXiaoSheng_Stepindex[g_BX_RoleNum] = 1
	end
	if BaiXiaoSheng_Attend[g_BX_RoleNum] == nil then
		BaiXiaoSheng_Attend[g_BX_RoleNum] = 0
	end
	if BaiXiaoSheng_Questions[g_BX_RoleNum] == nil then
		BaiXiaoSheng_Questions[g_BX_RoleNum] = 1
	end
    if BaiXiaoSheng_Role_Point[g_BX_RoleNum] == nil then
        BaiXiaoSheng_Role_Point[g_BX_RoleNum] = 0
    end
	if BaiXiaoSheng_Question_Type[g_BX_RoleNum] == nil then
	    BaiXiaoSheng_Question_Type[g_BX_RoleNum] = 0
	end
	
	if BaiXiaoSheng_SpeciallyQuestionStatus[RoleID] == nil then
		BaiXiaoSheng_SpeciallyQuestionStatus[RoleID] = 1
	end		
end

-- ��ʼ�����
function BaiXiaoSheng_InitQuestionTable()
	-- ��id_msg�а������Ѵ������ݷ������
	for i=1, BaiXiaoSheng_Hard_Question_Num do
		local id = 100001 + (i-1)*5
		BaiXiao_Hard_Question_Table[i] = {Content = id, RightAnswer = id+1, WrongAnswer1 = id+2, WrongAnswer2 = id+3, WrongAnswer3 = id+4}
	end

	-- ��id_msg�а�����ͨ�������ݷ������
	for i=1, BaiXiaoSheng_Normal_Question_Num do
		local id = 120001 + (i-1)*5
		BaiXiao_Normal_Question_Table[i] = {Content = id, RightAnswer = id+1, WrongAnswer1 = id+2, WrongAnswer2 = id+3, WrongAnswer3 = id+4}
	end

end

-- �����������ҵ��ߣ�������ҽ���
function BaiXiaoSheng_RewardRole(MapID, InstanceID, RoleID)
	-- �������飬��Ǯ

	-- ��Ǯ
	local RoleIndex = tbl_BX_Index[RoleID]
	local addmoney = 5000 * BaiXiaoSheng_Role_Point[RoleIndex] / 360
	role.AddRoleSilver(MapID, InstanceID, RoleID, addmoney, 0)

	-- ����
	local LevelUpNeedExp = role.GetRoleLevelUpExp(MapID, InstanceID, RoleID)
	local addexp = LevelUpNeedExp * 150 / (BaiXiaoSheng_Role_Level[RoleIndex]+10)^2 * BaiXiaoSheng_Role_Point[RoleIndex] / 360 * 2
	role.AddRoleExp(MapID, InstanceID, RoleID, addexp)

	-- ������Ʒ(δ���)
	-- �÷�>=300��ʱ ����һ������ӡ
	if BaiXiaoSheng_Role_Point[RoleIndex] >= 300 then
		role.QuestAddRoleItem(MapID, InstanceID, RoleID, -1, 2612616, 1, -1, 104)
	end


	--���ӽ���ʵ��ֵ
	local n = BaiXiaoSheng_Role_Point[RoleIndex]

	Increase_shili(MapID, InstanceID, RoleID, 0, n/10, "jinjieshili_A")
	BaiXiaoSheng_Stepindex[RoleIndex] = 1
end

-- ���ز��ظ�����Ŀ  ����0Ϊ������Ŀ 1Ϊ��ͨ��Ŀ
function BX_GetQuestion(RoleIndex, n)
	BX_CallDeep = BX_CallDeep + 1		-- �ݹ����+1
	local question
	local index
	
	
	
	if n == 0 then
		if table.maxn(BaiXiao_Hard_Question_Table) == 0 then	--�ж�֮ǰ�Ļ���й����У��Ƿ������ؽű��������������ԭ����������ÿ�
			BaiXiaoSheng_InitQuestionTable()
		end
		index = math.random(1, table.maxn(BaiXiao_Hard_Question_Table))
		question = BaiXiao_Hard_Question_Table[index]
	else
		if table.maxn(BaiXiao_Normal_Question_Table) == 0 then	--�ж�֮ǰ�Ļ���й����У��Ƿ������ؽű��������������ԭ����������ÿ�
			BaiXiaoSheng_InitQuestionTable()
		end
		index = math.random(1, table.maxn(BaiXiao_Normal_Question_Table))
		question = BaiXiao_Normal_Question_Table[index]
	end

	if tbl_BX_Questions[RoleIndex] == nil then
		local question_table = {}
		tbl_BX_Questions[RoleIndex] = {question_table}
		table.insert(tbl_BX_Questions[RoleIndex], question.Content)
		return question
	end

	local flag = 0
	for k, v in pairs(tbl_BX_Questions[RoleIndex]) do
		if v == question.Content then
			flag = 1
			break
		end
	end

	if flag == 1 then
		if BX_CallDeep >= 30 then			-- �ݹ���ȳ���30�Ļ���ֱ�ӷ����ظ�����
			return question
		else
			return BX_GetQuestion(RoleIndex, n)
		end
	else
		table.insert(tbl_BX_Questions[RoleIndex], question.Content)
		return question
	end
end

-- ������Ŀ��ѡ��
function BaiXiaoSheng_GenerateQuestion(RoleID, TargetID)
	local RoleIndex = tbl_BX_Index[RoleID]

	-- ��������5�ı���ʱ��ȡ�������ѵ���Ŀ
	if math.fmod(BaiXiaoSheng_Questions[RoleIndex], 5	) == 0 then  
		BX_CallDeep = 0
		BaiXiaoSheng_Current_Question[RoleIndex].question = BX_GetQuestion(RoleIndex, 0)

		local MsgID = msg.BeginMsgEvent()

		msg.AddMsgEvent(MsgID, 40, BaiXiaoSheng_Current_Question[RoleIndex].question.Content)	-- ��Ŀ����
		msg.AddMsgEvent(MsgID, 1, 217)
		msg.AddMsgEvent(MsgID, 12, BaiXiaoSheng_Role_Point[RoleIndex])		-- �÷�


		-- ��ȷ�����ĸ�ѡ���е����λ��
		BaiXiaoSheng_RightAnswer[RoleIndex] = math.random(0, 3)
		local index = 0
		for i=0, 3 do									-- �ĸ�ѡ��
			if BaiXiaoSheng_RightAnswer[RoleIndex] == i then
				msg.AddMsgEvent(MsgID, 21, i)			-- ѡ��
				msg.AddMsgEvent(MsgID, 1, BaiXiaoSheng_Current_Question[RoleIndex].question.RightAnswer)	-- ѡ������
			else
				msg.AddMsgEvent(MsgID, 21, i)
				msg.AddMsgEvent(MsgID, 1, BaiXiaoSheng_Current_Question[RoleIndex].question.WrongAnswer1+index)
				index = index + 1
			end
		end

		BaiXiaoSheng_Question_Type[RoleIndex] = 1		-- ��ǰ��Ŀ����
		-- ����ǰʱ�����table��
		BaiXiaoSheng_Time[RoleIndex] = os.time()
		-- ����ʱ
		msg.AddMsgEvent(MsgID, 9, BaiXiaoSheng_Time_Counter)
		-- �ѻش���Ŀ��
		msg.AddMsgEvent(MsgID, 12, BaiXiaoSheng_Questions[RoleIndex]-1)
		msg.AddMsgEvent(MsgID, 24, TargetID)
		BaiXiaoSheng_Questions[RoleIndex] = BaiXiaoSheng_Questions[RoleIndex] + 1 	-- ��ǰ������+1
		msg.DispatchRoleMsgEvent(RoleID, MsgID)

	-- һ�����ʲ���һ����Ե�⣬���ʴ�С����Ҹ�ԵֵӰ�죻
	-- ÿ�������ÿ�ΰ�������ڼ�ֻ�д��һ�ΰ�������Ե��Ļ��ᣬ���ô������ĸ�Ե��û�д�ԣ�����һ�������ٴ�������Ե�⣬ֱ��������̽���������ȷ�ش�Ե��Ϊֹ������ٴ�������Ե��ĸ����ܸ���Ҹ�Եֵ��Ӱ�죬ÿ���һ�Σ����ٴ������ĸ��ʻήΪ��һ�ε�1/3
	-- ��Ե��ֻ���ڴ������ﵽ5��ʱ�Ż����
	elseif  BaiXiaoSheng_Questions[RoleIndex]>5 and BaiXiaoSheng_SpeciallyQuestionStatus[RoleID] ~= 0 and math.random(1,200)<BaixiaoSheng_RoleBliss[RoleID] then
		
		BaixiaoSheng_RoleBliss[RoleID]=BaixiaoSheng_RoleBliss[RoleID]/3

		local QuestionIndex = 2200001 + math.random(0,BaiXiaoSheng_SpeciallyQuestion_Num-1)*5
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 40, QuestionIndex)	-- ��Ŀ����
		msg.AddMsgEvent(MsgID, 1, 217)
		msg.AddMsgEvent(MsgID, 12, BaiXiaoSheng_Role_Point[RoleIndex])
		BaiXiaoSheng_RightAnswer[RoleIndex] = math.random(0, 3)
		local index = 2
		for i=0, 3 do									-- �ĸ�ѡ��
			if BaiXiaoSheng_RightAnswer[RoleIndex] == i then
				msg.AddMsgEvent(MsgID, 21, i)			-- ѡ��
				msg.AddMsgEvent(MsgID, 1, QuestionIndex+1)	-- ѡ������
			else
				msg.AddMsgEvent(MsgID, 21, i)			-- ѡ��
				msg.AddMsgEvent(MsgID, 1, QuestionIndex+index)	-- ѡ������
				index = index + 1
			end
		end
		BaiXiaoSheng_Question_Type[RoleIndex] = 6
		BaiXiaoSheng_Time[RoleIndex] = os.time()
		msg.AddMsgEvent(MsgID, 9, BaiXiaoSheng_Time_Counter)
		-- �ѻش���Ŀ��
		msg.AddMsgEvent(MsgID, 12, BaiXiaoSheng_Questions[RoleIndex]-1)
		msg.AddMsgEvent(MsgID, 24, TargetID)
		BaiXiaoSheng_Questions[RoleIndex] = BaiXiaoSheng_Questions[RoleIndex] + 1 	-- ��ǰ������+1
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
		BaiXiaoSheng_SpeciallyQuestionStatus[RoleID] = BaiXiaoSheng_SpeciallyQuestionStatus[RoleID]+1

	-- ����������ͨ��Ŀ
	else
		BX_CallDeep = 0
		BaiXiaoSheng_Current_Question[RoleIndex].question = BX_GetQuestion(RoleIndex, 1)

		local MsgID = msg.BeginMsgEvent()

		msg.AddMsgEvent(MsgID, 40, BaiXiaoSheng_Current_Question[RoleIndex].question.Content)	-- ��Ŀ����
		msg.AddMsgEvent(MsgID, 1, 217)
		msg.AddMsgEvent(MsgID, 12, BaiXiaoSheng_Role_Point[RoleIndex])		-- �÷�

		-- ��ȷ�����ĸ�ѡ���е����λ��
		BaiXiaoSheng_RightAnswer[RoleIndex] = math.random(0, 3)
		local index = 0
		for i=0, 3 do
			if BaiXiaoSheng_RightAnswer[RoleIndex] == i then
				msg.AddMsgEvent(MsgID, 21, i)
				msg.AddMsgEvent(MsgID, 1, BaiXiaoSheng_Current_Question[RoleIndex].question.RightAnswer)
			else
				msg.AddMsgEvent(MsgID, 21, i)
				msg.AddMsgEvent(MsgID, 1, BaiXiaoSheng_Current_Question[RoleIndex].question.WrongAnswer1+index)
				index = index + 1
			end
		end

		BaiXiaoSheng_Question_Type[RoleIndex] = 0		-- ��ǰ��Ŀ����
		-- ����ǰʱ�����table��
		BaiXiaoSheng_Time[RoleIndex] = os.time()

		-- ����ʱ45��
		msg.AddMsgEvent(MsgID, 9, BaiXiaoSheng_Time_Counter)

		-- �ѻش���Ŀ��
		msg.AddMsgEvent(MsgID, 12, BaiXiaoSheng_Questions[RoleIndex]-1)
		msg.AddMsgEvent(MsgID, 24, TargetID)
		BaiXiaoSheng_Questions[RoleIndex] = BaiXiaoSheng_Questions[RoleIndex] + 1 	-- �Ѵ�����+1
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	end
end

-- ���ʼ
function BaiXiaoSheng_OnStart(actID)
	g_BX_RoleNum = 0
	tbl_BX_RoleID = {}
	tbl_BX_Index = {}
	tbl_BX_Questions = {}

	-- �����������
	math.randomseed(os.time())
	-- ��ʼ�����
	BaiXiaoSheng_InitQuestionTable()

	-- �㲥���������ʼ
	--local MsgID = msg.BeginMsgEvent()
	--msg.AddMsgEvent(MsgID, 100, 201)
	--msg.DispatchWorldMsgEvent(MsgID)
	--�����¹㲥���Ʋ��Ź㲥2.1�汾�޸�
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 102, 500001)
	RunBroad(MsgID, -1, -1, -1, -1, DPR_ALL, CFT_ACT, actID, 0)


	-- �ڸ�������ˢ��npc������

	BaiXiaoSheng_CreatureID[1] = map.MapCreateCreature(3017298127, -1, 3020750, 2184, 6646, 2269)
	BaiXiaoSheng_CreatureID[2] = map.MapCreateCreature(3017298383, -1, 3020750, 888, 10000, 2288)
	BaiXiaoSheng_CreatureID[3] = map.MapCreateCreature(3017299663, -1, 3020750, 2106, 18345, 2700)
	BaiXiaoSheng_CreatureID[4] = map.MapCreateCreature(3017299919, -1, 3020750, 1508, 21823, 910)
	BaiXiaoSheng_CreatureID[5] = map.MapCreateCreature(3017299151, -1, 3020750, 2184, 6789, 1741)
	BaiXiaoSheng_CreatureID[6] = map.MapCreateCreature(3017299407, -1, 3020750, 1807, 14024, 2467)
end


-- npc����ʱ�䵽����������ڸ�������ɾ����npc
function BaiXiaoSheng_OnEnd(actID)
	map.MapDeleteCreature(3017298127, -1, BaiXiaoSheng_CreatureID[1])
	map.MapDeleteCreature(3017298383, -1, BaiXiaoSheng_CreatureID[2])
	map.MapDeleteCreature(3017299663, -1, BaiXiaoSheng_CreatureID[3])
	map.MapDeleteCreature(3017299919, -1, BaiXiaoSheng_CreatureID[4])
	map.MapDeleteCreature(3017299151, -1, BaiXiaoSheng_CreatureID[5])
	map.MapDeleteCreature(3017299407, -1, BaiXiaoSheng_CreatureID[6])

	-- ��ձ�������
	BaiXiaoSheng_Attend = {}
	BaiXiaoSheng_Questions = {}
	BaiXiaoSheng_RightAnswer = {}
	BaiXiaoSheng_Time = {}
	BaiXiaoSheng_Role_Point = {}
	BaiXiaoSheng_Stepindex = {}
	BaiXiaoSheng_Role_Questions = {}
	BaiXiaoSheng_Question_Type = {}
	BaiXiao_Hard_Question_Table = {}
	BaiXiao_Normal_Question_Table = {}
	BaiXiaoSheng_Role_Level = {}
	BaiXiaoSheng_Current_Question = {}
	tbl_BX_RoleID = {}
	tbl_BX_Index = {}
	tbl_BX_Questions = {}
	g_BX_RoleNum = 0
	BaiXiaoSheng_SpeciallyQuestionStatus = {}
	BaixiaoSheng_RoleBliss = {}
end

-- ע����Ӧ�¼�
aux.RegisterCreatureEvent(3020750, 7, "x3020750_OnTalk")
aux.RegisterActEvent(32, 3, "BaiXiaoSheng_OnEnd")
aux.RegisterActEvent(32, 2, "BaiXiaoSheng_OnStart")
aux.RegisterActEvent(33, 3, "BaiXiaoSheng_OnEnd")
aux.RegisterActEvent(33, 2, "BaiXiaoSheng_OnStart")
aux.RegisterActEvent(34, 3, "BaiXiaoSheng_OnEnd")
aux.RegisterActEvent(34, 2, "BaiXiaoSheng_OnStart")
aux.RegisterActEvent(35, 3, "BaiXiaoSheng_OnEnd")
aux.RegisterActEvent(35, 2, "BaiXiaoSheng_OnStart")
aux.RegisterActEvent(36, 3, "BaiXiaoSheng_OnEnd")
aux.RegisterActEvent(36, 2, "BaiXiaoSheng_OnStart")
aux.RegisterActEvent(37, 3, "BaiXiaoSheng_OnEnd")
aux.RegisterActEvent(37, 2, "BaiXiaoSheng_OnStart")
aux.RegisterActEvent(38, 3, "BaiXiaoSheng_OnEnd")
aux.RegisterActEvent(38, 2, "BaiXiaoSheng_OnStart")
aux.RegisterActEvent(39, 3, "BaiXiaoSheng_OnEnd")
aux.RegisterActEvent(39, 2, "BaiXiaoSheng_OnStart")
aux.RegisterActEvent(40, 3, "BaiXiaoSheng_OnEnd")
aux.RegisterActEvent(40, 2, "BaiXiaoSheng_OnStart")
aux.RegisterActEvent(41, 3, "BaiXiaoSheng_OnEnd")
aux.RegisterActEvent(41, 2, "BaiXiaoSheng_OnStart")
