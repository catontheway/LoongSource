ShiDianYanLuo_Questions = {}				-- �Ѵ�����
ShiDianYanLuo_RightAnswer = {}			    -- ��ȷ�����ĸ�ѡ���е�����
ShiDianYanLuo_Stepindex1 = {}		        -- ÿ����ҵĻ��ȡ����
ShiDianYanLuo_RightPoint = {}	            -- �����Ŀ�ĸ���
ShiDianYanLuo_Question_Step = {}            -- ÿ����Ҵ���Ĳ���
ShiDianYanLuo_Question_Table = {}
ShiDianYanLuo_Role_Level = {}               -- ÿ����ҵĵȼ�
ShiDianYanLuo_Role_Quest = {}               -- ÿ����ҵ�����
ShiDianYanLuo_Role_Attend = {}              -- ÿ������Ƿ�μ��˻
ShiDianYanLuo_Role_Done = {}                -- ����Ƿ�����˴�������
ShiDianYanLuo_Question_Num = 105		    --�������Ŀ��
ShiDianYanLuo_Current_Question = {}			-- ��ҵ�ǰ����Ŀ

ShiDianYanLuo_Accept_Time = {3600}  	    -- npc����ʱ��Ϊ30����
ShiDianYanLuo_CanAccept = 0                 -- �Ƿ��ܴ�npc����ȡ����

g_SDYL_RoleNum = 0							-- �μ�ʮ�����޴�������Ҹ���
tbl_SDYL_RoleID = {}						-- ��g_SDYL_RoleNumΪ������Ӧ��ҵ�RoleID
tbl_SDYL_Index = {}							-- ��RoleIDΪ������Ӧ��ҵĴ���Index
tbl_SDYL_Questions = {}						-- ����Ѿ��ش������Ŀ

SDYL_CallDeep = 0							-- �ݹ�������

--��ͼm02�����������
map_m05 = {}
--5��NPC����С��
map_m05[1]={x=1055,y=24286,z=1828}
map_m05[2]={x=1070,y=25207,z=1717}
map_m05[3]={x=1185,y=25026,z=1981}
map_m05[4]={x=1343,y=22712,z=2128}
map_m05[5]={x=1150,y=26532,z=1893}

--ʮ��NPC

NPC = {}
NPC[1]={TypeID=3020801,x=936,y=19115,z=1184}
NPC[2]={TypeID=3020802,x=927,y=18887,z=1499}
NPC[3]={TypeID=3020803,x=630,y=24876,z=1648}
NPC[4]={TypeID=3020804,x=1175,y=26666,z=1893}
NPC[5]={TypeID=3020805,x=1639,y=22837,z=1913}
NPC[6]={TypeID=3020806,x=2328,y=16290,z=2658}
NPC[7]={TypeID=3020807,x=2700,y=28540,z=1825}
NPC[8]={TypeID=3020808,x=2312,y=20087,z=1333}
NPC[9]={TypeID=3020809,x=1910,y=22690,z=389}
NPC[10]={TypeID=3020810,x=1708,y=19826,z=495}

--ˢ��1��NPC����С��ˢ��10��NPC
function ShiDianYanLuo_CreateCreature()
		local randPoint = math.random(1, 5)
		local x = map_m05[randPoint].x
		local y = map_m05[randPoint].y
		local z = map_m05[randPoint].z
		map.MapCreateCreature(3017299919, -1, 3020811, x, y, z)

		for i = 1,10 do
			local x1 = NPC[i].x
			local y1 = NPC[i].y
			local z1 = NPC[i].z
			local TypeID = NPC[i].TypeID
			map.MapCreateCreature(3017299919, -1, TypeID, x1, y1, z1)
		end
end

--���ʼ�¼�
function ShiDianYanLuo_OnStart(actID)
	tbl_SDYL_RoleID = {}
	tbl_SDYL_Index = {}
	tbl_SDYL_Questions = {}
	g_SDYL_RoleNum = 0
	-- �����������
	math.randomseed(os.time())
	-- ��ʼ�����
	ShiDianYanLuo_InitQuestionTable()
	--ˢ��5��NPC����С��
	ShiDianYanLuo_CreateCreature()
	--ȫ�����㲥���ʼ
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 100, 220)
	msg.DispatchMapMsgEvent(MsgID, 3017299919, -1)

    -- ��������ʹ�������ȡ
    ShiDianYanLuo_CanAccept = 1

end

-- ��ʼ��Npc��ȡ����ʱ��
function ShiDianYanLuo_OnInit(actID)
	act.InitEventTime("ShiDianYanLuo_Accept_Time", 1, actID)
end

-- ����1��Сʱ�����ʹ�߹ر������ȡ
function ShiDianYanLuo_OnTimer(actID)
    ShiDianYanLuo_CanAccept = 0
end

-- ��ʼ���ձ��Ĭ��ֵ
function ShiDianYanLuo_InitNullTable(RoleID)
	-- ����һЩ�ձ��Ĭ��ֵ
	if tbl_SDYL_Index[RoleID] == nil then
		g_SDYL_RoleNum = g_SDYL_RoleNum + 1
		tbl_SDYL_RoleID[g_SDYL_RoleNum] = RoleID
		tbl_SDYL_Index[RoleID] = g_SDYL_RoleNum
		tbl_SDYL_Questions[g_SDYL_RoleNum] = {}
	end

	if ShiDianYanLuo_Stepindex1[g_SDYL_RoleNum] == nil then
		ShiDianYanLuo_Stepindex1[g_SDYL_RoleNum] = 1
	end
	if ShiDianYanLuo_Questions[g_SDYL_RoleNum] == nil then
		ShiDianYanLuo_Questions[g_SDYL_RoleNum] = 0
	end
	if ShiDianYanLuo_RightPoint[g_SDYL_RoleNum] == nil then
		ShiDianYanLuo_RightPoint[g_SDYL_RoleNum] = 0
	end
	if ShiDianYanLuo_Question_Step[g_SDYL_RoleNum] == nil then
		ShiDianYanLuo_Question_Step[g_SDYL_RoleNum] = 0
	end
    if ShiDianYanLuo_Role_Quest[g_SDYL_RoleNum] == nil then
        ShiDianYanLuo_Role_Quest[g_SDYL_RoleNum] = 0
    end
    if ShiDianYanLuo_Role_Attend[g_SDYL_RoleNum] == nil then
        ShiDianYanLuo_Role_Attend[g_SDYL_RoleNum] = 0
    end
    if ShiDianYanLuo_Role_Done[g_SDYL_RoleNum] == nil then
        ShiDianYanLuo_Role_Done[g_SDYL_RoleNum] = 0
    end

end

-- ����ʹ��npc�Ի�
function n3600014_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
    -- �����ȡ�ѱ��ر�
    if ShiDianYanLuo_CanAccept ~= 1 then
		-- ���˽�ȡ����ʱ�䣬��������������Ȼ������ȡ������
		if role.GetRoleItemNum(RoleID, 2612661) > 0 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 242)     -- "ʮ����������ͨ��"
			msg.AddMsgEvent(MsgID, 21, 4)       -- ȷ����ť
			msg.AddMsgEvent(MsgID, 1, 206)      -- "ȷ��"
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
			-- ɾ����ת������
			role.RemoveFromRole(MapID, InstanceID, RoleID, 2612661, 1, 101)
			-- ������ҽ�����δ��ɣ�


			return 0
		end

        -- npc����ͨ�Ի�
        local MsgID = msg.BeginMsgEvent()
        msg.AddMsgEvent(MsgID, 20, 243)     -- "����ʹ�ߵ���ͨ�Ի�"
        msg.AddMsgEvent(MsgID, 21, 4)       -- ȷ����ť
        msg.AddMsgEvent(MsgID, 1, 206)      -- "ȷ��"
        msg.DispatchRoleMsgEvent(RoleID, MsgID)
        return 0
    end

    -- ��ұ���������ת�����ƵĻ������뽱���������¿�ʼ
	if role.GetRoleScriptData(RoleID, 1, RoleDataType["SDYL_Done"]) == 1 then
		if role.GetRoleItemNum(RoleID, 2612661) > 0 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 242)     -- "ʮ����������ͨ��"
			msg.AddMsgEvent(MsgID, 21, 4)       -- ȷ����ť
			msg.AddMsgEvent(MsgID, 1, 206)      -- "ȷ��"
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
			-- ɾ����ת������
			role.RemoveFromRole(MapID, InstanceID, RoleID, 2612661, 1, 101)
			-- ������ҽ�����δ��ɣ�
			role.AddRoleExp(MapID, InstanceID, RoleID, 16800)

			--����ʵ��ֵ
			Increase_shili(MapID, InstanceID, RoleID, 0, 25,"jinjieshili_A")

			return 0
		end
	end

	if TalkIndex == -1 then
		--��ʼ���ձ�
		ShiDianYanLuo_InitNullTable(RoleID)
		-- �õ�������
		ShiDianYanLuo_Role_Level[tbl_SDYL_Index[RoleID]] = role.GetRoleLevel(MapID, InstanceID, RoleID)
	end

	if ShiDianYanLuo_Stepindex1[tbl_SDYL_Index[RoleID]] == 1 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 221)	-- "���Ƿ�����μ�ʮ�����޻��"
		msg.AddMsgEvent(MsgID, 21, 4)	-- ȷ����ť
		msg.AddMsgEvent(MsgID, 1, 206)	-- "ȷ��"

		msg.AddMsgEvent(MsgID, 21, 5)	-- ȡ����ť
		msg.AddMsgEvent(MsgID, 1, 207)	-- "ȡ��"

		msg.AddMsgEvent(MsgID, 24, TargetID)
		ShiDianYanLuo_Stepindex1[tbl_SDYL_Index[RoleID]] = 2
		msg.DispatchRoleMsgEvent(RoleID, MsgID)

	elseif ShiDianYanLuo_Stepindex1[tbl_SDYL_Index[RoleID]] == 2 then
		if TalkIndex == 4 then			-- ѡ����ȷ���μӰ�ť��
			if ShiDianYanLuo_Role_Level[tbl_SDYL_Index[RoleID]] < 55 then	-- �ȼ�����55���Ļ�������ʾ
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 222)		-- "ʮ�����޻������55�����ϲſɲ���"
				msg.AddMsgEvent(MsgID, 21, 4)		-- ȷ����ť
				msg.AddMsgEvent(MsgID, 1, 206)		-- "ȷ��"
				ShiDianYanLuo_Stepindex1[tbl_SDYL_Index[RoleID]] = 1
				msg.DispatchRoleMsgEvent(RoleID, MsgID)

			elseif ShiDianYanLuo_Role_Attend[tbl_SDYL_Index[RoleID]] == 1 then	-- �ڴ˻ʱ�����Ѳμӹ�
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 223)		-- "���Ѳμ���ʮ�����޻��"
				msg.AddMsgEvent(MsgID, 21, 4)		-- ȷ����ť
				msg.AddMsgEvent(MsgID, 1, 206)		-- "ȷ��"
				ShiDianYanLuo_Stepindex1[tbl_SDYL_Index[RoleID]] = 1
				msg.DispatchRoleMsgEvent(RoleID, MsgID)

			else
				-- ������ڿ�ʼ��
				ShiDianYanLuo_Stepindex1[tbl_SDYL_Index[RoleID]] = 1
				ShiDianYanLuo_Question_Step[tbl_SDYL_Index[RoleID]] = 1
				ShiDianYanLuo_Role_Attend[tbl_SDYL_Index[RoleID]] = 1       -- �Ѳμӱ��λ
				-- ��ʼ��ScriptData
				role.SetRoleScriptData(RoleID, 1, RoleDataType["SDYL_Done"], 0)
				--������ҵ���[��������]
				role.QuestAddRoleItem(MapID, InstanceID, RoleID, 20061, 2612651, 1, -1, 102)
				ShiDianYanLuo_Current_Question[tbl_SDYL_Index[RoleID]] = {question, index}
			end
		else
			ShiDianYanLuo_Stepindex1[tbl_SDYL_Index[RoleID]] = 1
		end
	end
end

-- ��ʼ�����
function ShiDianYanLuo_InitQuestionTable()
	-- ��id_msg��ʮ�����޴������ݷ������
	for i=1, ShiDianYanLuo_Question_Num do
		local id = 140001 + (i-1)*5
		ShiDianYanLuo_Question_Table[i] = {Content = id, RightAnswer = id+1, WrongAnswer1 = id+2, WrongAnswer2 = id+3, WrongAnswer3 = id+4}
	end
end

-- ���ز��ظ�����Ŀ
function SDYL_GetQuestion(RoleIndex)
	SDYL_CallDeep = SDYL_CallDeep + 1		-- �ݹ����+1
	local question
	local index
	index = math.random(1, table.maxn(ShiDianYanLuo_Question_Table))
	question = ShiDianYanLuo_Question_Table[index]

	if tbl_SDYL_Questions[RoleIndex] == nil then
		local question_table = {}
		tbl_SDYL_Questions[RoleIndex] = {question_table}
		table.insert(tbl_SDYL_Questions[RoleIndex], question.Content)
		return question
	end

	local flag = 0
	for k, v in pairs(tbl_SDYL_Questions[RoleIndex]) do
		if v == question.Content then
			flag = 1
			break
		end
	end

	if flag == 1 then
		if SDYL_CallDeep >= 10 then			-- �ݹ���ȳ���10�Ļ���ֱ�ӷ����ظ�����
			return question
		else
			return SDYL_GetQuestion(RoleIndex)
		end
	else
		table.insert(tbl_SDYL_Questions[RoleIndex], question.Content)
		return question
	end
end

-- ������Ŀ��ѡ��
function ShiDianYanLuo_GenerateQuestion(RoleID, TargetID)
		local RoleIndex = tbl_SDYL_Index[RoleID]
		SDYL_CallDeep = 0
		ShiDianYanLuo_Current_Question[RoleIndex].question = SDYL_GetQuestion(RoleIndex)

		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 40, ShiDianYanLuo_Current_Question[RoleIndex].question.Content)	-- ��Ŀ����
		msg.AddMsgEvent(MsgID, 1, 217)
		msg.AddMsgEvent(MsgID, 12, ShiDianYanLuo_RightPoint[RoleIndex])		-- �÷�
		-- ��ȷ�����ĸ�ѡ���е����λ��
		ShiDianYanLuo_RightAnswer[RoleIndex] = math.random(0, 3)
		local index = 0
		for i=0, 3 do
			if ShiDianYanLuo_RightAnswer[RoleIndex] == i then
				msg.AddMsgEvent(MsgID, 21, i)
				msg.AddMsgEvent(MsgID, 1, ShiDianYanLuo_Current_Question[RoleIndex].question.RightAnswer)
			else
				msg.AddMsgEvent(MsgID, 21, i)
				msg.AddMsgEvent(MsgID, 1, ShiDianYanLuo_Current_Question[RoleIndex].question.WrongAnswer1+index)
				index = index + 1
			end
		end
        -- ����ʱ
		msg.AddMsgEvent(MsgID, 9, 30)
		-- �ѻش���Ŀ��
		msg.AddMsgEvent(MsgID, 12, ShiDianYanLuo_Questions[RoleIndex])
		msg.AddMsgEvent(MsgID, 24, TargetID)
        msg.DispatchRoleMsgEvent(RoleID, MsgID)

		ShiDianYanLuo_Questions[RoleIndex] = ShiDianYanLuo_Questions[RoleIndex] + 1 	-- �Ѵ�����+1
end

-- ���ܶԻ�����
function ShiDianYanLuo_OnAccept(MapID, InstanceID, QuestID, RoleID, NPCID)
	local RoleIndex = tbl_SDYL_Index[RoleID]
    --����Я����Ʒ������QuestID
    local num1 = role.GetRoleItemNum(RoleID, 2612651)
    local num2 = role.GetRoleItemNum(RoleID, 2612655)
    local num3 = role.GetRoleItemNum(RoleID, 2612658)
    if num1 >0 then
        ShiDianYanLuo_Role_Quest[RoleIndex] = 20062
    elseif num2 >0 then
        ShiDianYanLuo_Role_Quest[RoleIndex] = 20069
    elseif num3 >0 then
        ShiDianYanLuo_Role_Quest[RoleIndex] = 20072
    end

	nQuestion_OnTalk(MapID, InstanceID, NPCID, -1, RoleID, 1)
end



--�ع�����������������������Ի�
function nQuestion_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)

	--�ж��Ƿ�������
	if(role.IsRoleHaveQuest(MapID, InstanceID, RoleID, ShiDianYanLuo_Role_Quest[tbl_SDYL_Index[RoleID]]) == true) then

		if ShiDianYanLuo_Question_Step[tbl_SDYL_Index[RoleID]] == 1 then
			ShiDianYanLuo_GenerateQuestion(RoleID, TargetID)
			ShiDianYanLuo_Question_Step[tbl_SDYL_Index[RoleID]] = 2
		else
			if TalkIndex == 5 then      -- ��������
			    -- ɾ��������  �����½�ȡ
			    role.DeleteQuest(RoleID, ShiDianYanLuo_Role_Quest[tbl_SDYL_Index[RoleID]])
				-- ����Ѵ����ÿ�
				tbl_SDYL_Questions[tbl_SDYL_Index[RoleID]] = {}
				--����÷����
				ShiDianYanLuo_RightPoint[tbl_SDYL_Index[RoleID]] = 0
				ShiDianYanLuo_Questions[tbl_SDYL_Index[RoleID]] = 0
			else
				-- �������ȷ����ȷ������1
				if ShiDianYanLuo_RightAnswer[tbl_SDYL_Index[RoleID]] == TalkIndex then
					ShiDianYanLuo_RightPoint[tbl_SDYL_Index[RoleID]] = ShiDianYanLuo_RightPoint[tbl_SDYL_Index[RoleID]] + 1
				end

				--������Ŀ �������������������Ҵ�������δ������������������֮һ����ȷ�ش������ʱ
				--Ĭ�����������
				if ShiDianYanLuo_Questions[tbl_SDYL_Index[RoleID]] < ShiDianYanLuo_Question_Num/5 and ShiDianYanLuo_RightPoint[tbl_SDYL_Index[RoleID]] < 5 then --������С��10��ʱ����������Ŀ
					ShiDianYanLuo_GenerateQuestion(RoleID, TargetID)

				else
					-- �������
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 40, -1)
					msg.DispatchRoleMsgEvent(RoleID, MsgID)

					-- ������5�������ϻ��ߴ��������⣬ShiDianYanLuo_RightPoint��Ϊ1
					ShiDianYanLuo_Role_Done[tbl_SDYL_Index[RoleID]] = 1
					ShiDianYanLuo_RightPoint[tbl_SDYL_Index[RoleID]] = 0 --���ô����
					ShiDianYanLuo_Questions[tbl_SDYL_Index[RoleID]] = 0 --���ô�����
				end
			end
        end
    end
end

--��ȡ����
function n3020802_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)

	--����Ŀ�������
	local rand = math.random(1,4)

	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 20, 226)	-- "���ڶ����Ű���뵽���Ź�����Ҫ���Ǽ�ǿ���䡣"
	msg.AddMsgEvent(MsgID, 21, 4)	-- ȷ����ť
	msg.AddMsgEvent(MsgID, 1, 206)	-- "ȷ��"

	msg.AddMsgEvent(MsgID, 21, 5)	-- ȡ����ť
	msg.AddMsgEvent(MsgID, 1, 207)	-- "ȡ��"

	msg.AddMsgEvent(MsgID, 24, TargetID)
	msg.DispatchRoleMsgEvent(RoleID, MsgID)
	if TalkIndex == 4 then
		if rand == 1 then
			role.AddQuest(RoleID,20063)
		elseif rand == 2 then
			role.AddQuest(RoleID,20064)
		elseif rand == 3 then
			role.AddQuest(RoleID,20065)
		elseif rand == 4 then
			role.AddQuest(RoleID,20066)
		end
	end
end


function question_OnCheckComplete(MapID, InstanceID, QuestID, RoleID, NPCID)
	--�ж��Ƿ��Ѵ��5����
	if ShiDianYanLuo_Role_Done[tbl_SDYL_Index[RoleID]] ~= 1 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 224)
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
		return 0
	end

	--�жϱ������пռ��Ƿ��㹻
	if(role.GetBagFreeSize(RoleID) < 1) then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 142)
		msg.DispatchRoleMsgEvent(MsgID)
		return 0
	end

	-- ����
	ShiDianYanLuo_Role_Done[tbl_SDYL_Index[RoleID]] = 0

	return 1
end

-- ��������
function Quest_OnCancel(MapID, InstanceID, QuestID, RoleID)
	-- ��������Ա���в���
	if g_SDYL_RoleNum == 0 then
		-- ����Ѵ����ÿ�
		tbl_SDYL_Questions[tbl_SDYL_Index[RoleID]] = {}

		--��;����������Ϊδ�μӻ
		ShiDianYanLuo_Role_Attend[tbl_SDYL_Index[RoleID]] = 0

		--����÷����
		ShiDianYanLuo_RightPoint[tbl_SDYL_Index[RoleID]] = 0
		ShiDianYanLuo_Questions[tbl_SDYL_Index[RoleID]] = 0

	end
	-- ��ȥ��������
	for i=2612651, 2612661 do
		ShiDianYanLuo_RemoveBar(MapID, InstanceID, RoleID, i)
	end
end

-- ��ȥ�������ӵ�е�����
function ShiDianYanLuo_RemoveBar(MapID, InstanceID, RoleID, ItemID)
    if role.GetRoleItemNum(RoleID, ItemID) > 0 then
        role.RemoveFromRole(MapID, InstanceID, RoleID, ItemID, 1, 103)
    end
end

--�������ʮ�����޺󣬱��Ϊδ�μӻ��������Ҵ���������ʹ��
function Quest_OnComplete(MapID, InstanceID, QuestID, RoleID, NPCID)
	-- ����Ѵ����ÿ�
	tbl_SDYL_Questions[tbl_SDYL_Index[RoleID]] = {}
	-- ��SDYL_DoneΪ1
	role.SetRoleScriptData(RoleID, 1, RoleDataType["SDYL_Done"], 1)
	ShiDianYanLuo_Role_Attend[tbl_SDYL_Index[RoleID]] = 0
	role.RoleGotoNewMap(MapID, InstanceID, RoleID, 3017299919, 957, 20140, 848)
end

--20071 �ߵ����� ��ȡ�������BUFF�������[2016201]
function n20071_OnAccept(MapID, InstanceID, QuestID, OwnerID, AccepterID)
	unit.AddBuff(MapID, InstanceID, OwnerID, 2016201, OwnerID)
end

--20073 �ŵ����� ����������BUFFƽ������[2016301]
function n20073_OnComplete(MapID, InstanceID, QuestID, RoleID, NPCID)
	unit.AddBuff(MapID, InstanceID, RoleID, 2016301, RoleID)
end

-- �����
function ShiDianYanLuo_OnEnd(actID)
    ShiDianYanLuo_Stepindex1 = {}
    ShiDianYanLuo_Questions = {}
    ShiDianYanLuo_RightPoint = {}
    ShiDianYanLuo_Question_Step = {}
    ShiDianYanLuo_Role_Quest = {}
    ShiDianYanLuo_Role_Attend = {}
    ShiDianYanLuo_Role_Done = {}
	ShiDianYanLuo_Current_Question = {}
	tbl_SDYL_RoleID = {}
	tbl_SDYL_Index = {}
	tbl_SDYL_Questions = {}
	g_SDYL_RoleNum = 0
end
--aux.RegisterQuestEvent(20062, 0,"ShiDianYanLuo_OnAccept")
--aux.RegisterQuestEvent(20069, 0,"ShiDianYanLuo_OnAccept")
--aux.RegisterQuestEvent(20072, 0,"ShiDianYanLuo_OnAccept")
--[[ ע����Ӧ�¼�
aux.RegisterCreatureEvent(3600014, 7, "n3600014_OnTalk")
aux.RegisterCreatureEvent(3020801, 7, "nQuestion_OnTalk")
aux.RegisterCreatureEvent(3020805, 7, "nQuestion_OnTalk")
aux.RegisterCreatureEvent(3020808, 7, "nQuestion_OnTalk")
aux.RegisterQuestEvent(20062, 5, "question_OnCheckComplete")
aux.RegisterQuestEvent(20069, 5, "question_OnCheckComplete")
aux.RegisterQuestEvent(20072, 5, "question_OnCheckComplete")

aux.RegisterActEvent(48, 0, "ShiDianYanLuo_OnInit")
aux.RegisterActEvent(48, 1, "ShiDianYanLuo_OnTimer")
aux.RegisterActEvent(48, 2, "ShiDianYanLuo_OnStart")
aux.RegisterActEvent(48, 3, "ShiDianYanLuo_OnEnd")
aux.RegisterActEvent(49, 0, "ShiDianYanLuo_OnInit")
aux.RegisterActEvent(49, 1, "ShiDianYanLuo_OnTimer")
aux.RegisterActEvent(49, 2, "ShiDianYanLuo_OnStart")
aux.RegisterActEvent(49, 3, "ShiDianYanLuo_OnEnd")
aux.RegisterActEvent(50, 0, "ShiDianYanLuo_OnInit")
aux.RegisterActEvent(50, 1, "ShiDianYanLuo_OnTimer")
aux.RegisterActEvent(50, 2, "ShiDianYanLuo_OnStart")
aux.RegisterActEvent(50, 3, "ShiDianYanLuo_OnEnd")
aux.RegisterActEvent(51, 0, "ShiDianYanLuo_OnInit")
aux.RegisterActEvent(51, 1, "ShiDianYanLuo_OnTimer")
aux.RegisterActEvent(51, 2, "ShiDianYanLuo_OnStart")
aux.RegisterActEvent(51, 3, "ShiDianYanLuo_OnEnd")



aux.RegisterQuestEvent(20061, 2, "Quest_OnCancel")
aux.RegisterQuestEvent(20062, 2, "Quest_OnCancel")
aux.RegisterQuestEvent(20063, 2, "Quest_OnCancel")
aux.RegisterQuestEvent(20064, 2, "Quest_OnCancel")
aux.RegisterQuestEvent(20065, 2, "Quest_OnCancel")
aux.RegisterQuestEvent(20066, 2, "Quest_OnCancel")
aux.RegisterQuestEvent(20067, 2, "Quest_OnCancel")
aux.RegisterQuestEvent(20068, 2, "Quest_OnCancel")
aux.RegisterQuestEvent(20069, 2, "Quest_OnCancel")
aux.RegisterQuestEvent(20070, 2, "Quest_OnCancel")
aux.RegisterQuestEvent(20071, 2, "Quest_OnCancel")
aux.RegisterQuestEvent(20072, 2, "Quest_OnCancel")
aux.RegisterQuestEvent(20073, 2, "Quest_OnCancel")
aux.RegisterQuestEvent(20074, 2, "Quest_OnCancel")

aux.RegisterQuestEvent(20074,1,"Quest_OnComplete")
aux.RegisterQuestEvent(20073,1,"n20073_OnComplete")

aux.RegisterCreatureEvent(3020802, 7, "n3020802_OnTalk")
aux.RegisterQuestEvent(20071, 0, "n20071_OnAccept")]]


