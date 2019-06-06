XinChunDengMi_Attend = {}				-- �Ƿ��ڴ˻ʱ���ڲμӹ�
XinChunDengMi_Questions = {}				-- ����ĸ�����Ϣ
XinChunDengMi_RewardsRecevied = {}	-- �Ƿ��Ѿ���ȡ��Ʒ(1��û����)
XinChunDengMi_Role_Point = {}			-- ��һ�õķ�������Ӧ��ɫid
XinChunDengMi_Question_Num = 439	-- ���յ���Ŀ��
XinChunDengMi_Time_Counter = 30			-- ����ĵ���ʱ
XinChunDengMi_Time = {}					-- ��Ŵ��⿪ʼʱ��ϵͳʱ�䣬��Ӧ��ɫid
XinChunDengMi_RightAnswer = {}			-- ��ȷ�����ĸ�ѡ���е�����
XinChunDengMi_CreatureID = {}			-- ��Ŵ����´����տ��ٺ󷵻ص�id�����ڻ������ɾ��npc
DengMi_Question_Table = {}

dengmi_levelexp = {}
dengmi_levelexp[10] = 4100
dengmi_levelexp[11] = 4800
dengmi_levelexp[12] = 5400
dengmi_levelexp[13] = 6100
dengmi_levelexp[14] = 6900
dengmi_levelexp[15] = 7600
dengmi_levelexp[16] = 8500
dengmi_levelexp[17] = 9300
dengmi_levelexp[18] = 10200
dengmi_levelexp[19] = 11200
dengmi_levelexp[20] = 13000
dengmi_levelexp[21] = 15000
dengmi_levelexp[22] = 17300
dengmi_levelexp[23] = 19800
dengmi_levelexp[24] = 22400
dengmi_levelexp[25] = 25200
dengmi_levelexp[26] = 28100
dengmi_levelexp[27] = 31200
dengmi_levelexp[28] = 34400
dengmi_levelexp[29] = 37800
dengmi_levelexp[30] = 41300
dengmi_levelexp[31] = 48600
dengmi_levelexp[32] = 56200
dengmi_levelexp[33] = 64300
dengmi_levelexp[34] = 72700
dengmi_levelexp[35] = 81600
dengmi_levelexp[36] = 90800
dengmi_levelexp[37] = 100400
dengmi_levelexp[38] = 110400
dengmi_levelexp[39] = 120800
dengmi_levelexp[40] = 131600
dengmi_levelexp[41] = 154800
dengmi_levelexp[42] = 181800
dengmi_levelexp[43] = 210600
dengmi_levelexp[44] = 240900
dengmi_levelexp[45] = 273000
dengmi_levelexp[46] = 306600
dengmi_levelexp[47] = 342000
dengmi_levelexp[48] = 378900
dengmi_levelexp[49] = 417600
dengmi_levelexp[50] = 470200
dengmi_levelexp[51] = 525300
dengmi_levelexp[52] = 582700
dengmi_levelexp[53] = 642600
dengmi_levelexp[54] = 704800
dengmi_levelexp[55] = 769500
dengmi_levelexp[56] = 836500
dengmi_levelexp[57] = 906000
dengmi_levelexp[58] = 977800
dengmi_levelexp[59] = 1052100
dengmi_levelexp[60] = 1144500
dengmi_levelexp[61] = 1204500
dengmi_levelexp[62] = 1265900
dengmi_levelexp[63] = 1328500
dengmi_levelexp[64] = 1392500
dengmi_levelexp[65] = 1457700
dengmi_levelexp[66] = 1524100
dengmi_levelexp[67] = 1591900
dengmi_levelexp[68] = 1660900
dengmi_levelexp[69] = 1731300
dengmi_levelexp[70] = 1802900
dengmi_levelexp[71] = 1875700
dengmi_levelexp[72] = 1949900
dengmi_levelexp[73] = 2025300
dengmi_levelexp[74] = 2102100
dengmi_levelexp[75] = 2180100
dengmi_levelexp[76] = 2259300
dengmi_levelexp[77] = 2339900
dengmi_levelexp[78] = 2421700
dengmi_levelexp[79] = 2504900
dengmi_levelexp[80] = 2616700
dengmi_levelexp[81] = 3500000
dengmi_levelexp[82] = 5200000
dengmi_levelexp[83] = 7600000
dengmi_levelexp[84] = 9300000
dengmi_levelexp[85] = 10600000
dengmi_levelexp[86] = 13440000
dengmi_levelexp[87] = 15618000
dengmi_levelexp[88] = 16820000
dengmi_levelexp[89] = 17700000
dengmi_levelexp[90] = 19320000
dengmi_levelexp[91] = 23719606
dengmi_levelexp[92] = 28520000
dengmi_levelexp[93] = 31679550
dengmi_levelexp[94] = 33379200
dengmi_levelexp[95] = 34908250
dengmi_levelexp[96] = 36748800
dengmi_levelexp[97] = 38803050
dengmi_levelexp[98] = 44934400
dengmi_levelexp[99] = 51359805
dengmi_levelexp[100] = 58044000
dengmi_levelexp[101] = 64000000
dengmi_levelexp[102] = 66563455
dengmi_levelexp[103] = 69941705
dengmi_levelexp[104] = 73441712
dengmi_levelexp[105] = 77554046
dengmi_levelexp[106] = 81320031
dengmi_levelexp[107] = 85216560
dengmi_levelexp[108] = 89246539
dengmi_levelexp[109] = 93959179
dengmi_levelexp[110] = 98280000

-- npc�Ի��������������
function x4900401_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	if TalkIndex == -1 then                                                --��ʼ�Ի�
	    XinChunDengMi_InitNullTable(RoleID)
		if XinChunDengMi_RewardsRecevied[RoleID] == 1 then                    --���������꣬������û�и�
			local level = role.GetRoleLevel(MapID, InstanceID, RoleID)
			local x = XinChunDengMi_Role_Point[RoleID]/10
			local reward
			if x <= 0 or x == nil then                                        --������������
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
			elseif x >= 10 then
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
		else
		    if DengMi_Time() then                        --���û�н����ж�λ��Ϊ1�����ж��Ƿ��ڻ��
			    if XinChunDengMi_Questions[RoleID].NUM < 20 and XinChunDengMi_Questions[RoleID].NUM > 0 then
					XinChunDengMi_GenerateQuestion(RoleID, TargetID)
				else
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 20, 2101)	-- "�´����տ��٣�\n    ���Ρ��´�����ϲ�ֻᡱ�Ѿ���ʼ������10��������Ҿ��ɲ�����Ⲣ׬ȡ���飬���10�����ϻ��ɶ������´�С������������Ƿ�Ҫ������"
					msg.AddMsgEvent(MsgID, 21, 4)	-- ȷ����ť
					msg.AddMsgEvent(MsgID, 1, 2102)	-- "�μ�[����ϲ�ֻ�]"
					msg.AddMsgEvent(MsgID, 21, 9)	-- ȡ����ť
					msg.AddMsgEvent(MsgID, 1, 2103)	-- "���μ�"
					msg.AddMsgEvent(MsgID, 24, TargetID)
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
				end
			else                                      --������ڻʱ��
			    if XinChunDengMi_Role_Point[RoleID] > 0 then           --�������ϴ�û���꣬�������
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
					elseif x >= 10 then
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
				elseif XinChunDengMi_Role_Point[RoleID] == 0 then             --������û�����õĽ���������ʾ�´�����
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 20, 2107)	-- "�´����տ��٣�\n    ����׬���飬�������´���\n    ���´�����ϲ�ֻᡱ��ÿ���[11:00 �C 12:00]��[19:00 �C 20:00]������һ�Σ�����10��������Ҿ��ɲ�������þ��齱������һ�λ�д��10�������������һ��ɶ����á��´�С�����һ����"
					msg.AddMsgEvent(MsgID, 21, 4)	-- ȷ����ť
					msg.AddMsgEvent(MsgID, 1, 206)	-- "ȷ��"
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
				end
			end
	    end
	elseif TalkIndex == 4 then                             -- �������ڻʱ��û�н������ã�������򡭡�
	    local Player_level = role.GetRoleLevel(MapID, InstanceID, RoleID)
	    if Player_level < 10 then	-- �ȼ�����10���Ļ�������ʾ
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 2104)		-- "�´����տ��٣�\n    ���ĵȼ�����10�������Ƚ��ȼ�������10�������μӱ����"
			msg.AddMsgEvent(MsgID, 21, 4)		-- ȷ����ť
			msg.AddMsgEvent(MsgID, 1, 206)		-- "ȷ��"
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
	    elseif XinChunDengMi_Attend[RoleID] == 1 then
	        local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 2105)		-- "���Ѳμӹ������´����ջ�����´�����ϲ�ֻᡱ��ÿ���[11:00 �C 12:00]��[19:00 �C 20:00]������һ�Σ����¸�ʱ���������"
			msg.AddMsgEvent(MsgID, 21, 4)		-- ȷ����ť
			msg.AddMsgEvent(MsgID, 1, 206)		-- "ȷ��"
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
	    else
			-- ���������ڿ�ʼ��
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 2106)      -- �����ջ���ڿ�ʼ��
			msg.AddMsgEvent(MsgID, 21, 6)
			msg.AddMsgEvent(MsgID, 1, 206)
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
	    end
	elseif TalkIndex == 6 then                       --���տ�ʼ���Զ�����
		XinChunDengMi_GenerateQuestion(RoleID, TargetID)
	elseif TalkIndex == 0 or TalkIndex == 1 or TalkIndex == 2 or TalkIndex == 3 then    --���ѡ��ѡ�����в�ͬ�Ľ��
	    if XinChunDengMi_Time[RoleID] == nil then
		    -- �������
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 40, -1)
			msg.DispatchRoleMsgEvent(RoleID, MsgID)                            --������������轱��
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
			elseif x >= 10 then
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
		else
			-- ������⻨�ѵ�ʱ��
			local time1 = XinChunDengMi_Time[RoleID]
			local dtime = os.difftime(os.time(), time1)
			-- �жϴ��Ƿ���ȷ
			if XinChunDengMi_RightAnswer[RoleID] == TalkIndex then
				-- �ش�������ʱ�䲻ͬ���÷ֲ�ͬ
				if dtime <= 30 then				-- 30���ڻش����ȷ�𰸣������Ŀȫ������
					-- ���յ÷�
					XinChunDengMi_Role_Point[RoleID] = XinChunDengMi_Role_Point[RoleID] + 10
				end
			end
			-- ������Ŀ
			if XinChunDengMi_Questions[RoleID].NUM < 20 then    -- ������С��20��ʱ����������Ŀ
				XinChunDengMi_GenerateQuestion(RoleID, TargetID)
			else
				-- �������
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 40, -1)
				msg.DispatchRoleMsgEvent(RoleID, MsgID)                            --������������轱��
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
				elseif x >= 10 then
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
		end
	elseif TalkIndex == 10 then                         --����ڵ��ȷ��Ҫ��ȡ����֮�󣬾���ķ�������
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
			XinChunDengMi_Role_Point[RoleID] = 0                             --��շ����ͽ�����־λ
			XinChunDengMi_RewardsRecevied[RoleID] = 0
		elseif x >= 10 then
			local FreeSize = role.GetBagFreeSize(RoleID)
			if FreeSize < 2 then
                local MsgID = msg.BeginMsgEvent()
			    msg.AddMsgEvent(MsgID, 20, 2108)	-- "�´����տ��٣�\n    �����ҵĿռ䲻�㣬�������Ժ�������
			    msg.AddMsgEvent(MsgID, 21, 9)	-- ȷ����ť
			    msg.AddMsgEvent(MsgID, 1, 206)	-- "ȷ��"
			    msg.DispatchRoleMsgEvent(RoleID, MsgID)
			else
			    if x == 20 then
				    role.SigTitleEvent(RoleID, 190)--��óƺ� �Ÿ߰˶�
					-- �㲥�´����տ��ٻ��ʼ
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 100, 2124)    --��ϲXXX������´����յ�����20����Ŀ����á��Ÿ߰˶����ƺţ�
					msg.AddMsgEvent(MsgID, 2, RoleID)
					msg.DispatchWorldMsgEvent(MsgID)
				end
				role.AddRoleExp(MapID, InstanceID, RoleID, reward)
				role.AddRoleItem(MapID, InstanceID, RoleID, 3303004, 1, -1, 8, 420)
				role.AddRoleItem(MapID, InstanceID, RoleID, 3303001, 1, -1, 8, 420)
				XinChunDengMi_Role_Point[RoleID] = 0
				XinChunDengMi_RewardsRecevied[RoleID] = 0
	        end
		end
	end
end




-- ���ʼ
function XinChun_OnTimerMin(actID)
    local curmon = tonumber(os.date("%m"))
	local curday = tonumber(os.date("%d"))
	if (curmon == 2 and curday >= 20) or (curmon == 3 and curday <= 7)then
	    local TypeID = cre.GetCreatureTypeID(3017299663, -1, XinChunDengMi_CreatureID[15])
	    if TypeID ~= 4900406 then
		    map.MapDeleteCreature(3017299663, -1, XinChunDengMi_CreatureID[15])
	        map.MapDeleteCreature(3017298127, -1, XinChunDengMi_CreatureID[16])
		    XinChunDengMi_CreatureID[15] = map.MapCreateCreature(3017299663, -1, 4900406, 2139, 18345, 2424)--�Ƶ۳�ʳ��
	        XinChunDengMi_CreatureID[16] = map.MapCreateCreature(3017298127, -1, 4900406, 2171, 6646, 2224)--����ʳ��
		end
	elseif curmon >= 2 and curday >= 5 then
	    local TypeID = cre.GetCreatureTypeID(3017299663, -1, XinChunDengMi_CreatureID[15])
	    if TypeID ~= 4900405 then
		    map.MapDeleteCreature(3017299663, -1, XinChunDengMi_CreatureID[15])
	        map.MapDeleteCreature(3017298127, -1, XinChunDengMi_CreatureID[16])
		    XinChunDengMi_CreatureID[15] = map.MapCreateCreature(3017299663, -1, 4900405, 2139, 18345, 2424)--�Ƶ۳�ʳ��
	        XinChunDengMi_CreatureID[16] = map.MapCreateCreature(3017298127, -1, 4900405, 2171, 6646, 2224)--����ʳ��
		end
	end
    local curhour = os.date("%H")
	local curmin = os.date("%M")
	if curhour ~= "00" and curmin ~= "00" then
	    curhour = tonumber(os.date("%H"))
	    curmin = tonumber(os.date("%M"))
	else
	    if curhour == "00" then
		    curhour = 0
		end
		if curmin == "00" then
		    curmin = 0
		end
	end
	if (curhour == 11 or curhour == 19) and curmin == 28 then
        XinChunDengMi_RewardsRecevied = {}	-- �Ƿ��Ѿ���ȡ��Ʒ(1��û����)
        XinChunDengMi_Role_Point = {}			-- ��һ�õķ�������Ӧ��ɫid
	elseif (curhour == 11 or curhour == 19) and curmin == 29 then
		-- �����������
		math.randomseed(os.time())
	    -- ��ʼ�����
		XinChunDengMi_InitQuestionTable()
		XinChunDengMi_RewardsRecevied = {}	-- �Ƿ��Ѿ���ȡ��Ʒ(1��û����)
        XinChunDengMi_Role_Point = {}			-- ��һ�õķ�������Ӧ��ɫid
    elseif (curhour == 11 or curhour == 19) and curmin == 30 then
		-- �����������
		math.randomseed(os.time())
		-- ��ʼ�����
		XinChunDengMi_InitQuestionTable()

		-- �㲥�´����տ��ٻ��ʼ
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 100, 2113)    --����׬���飬�������´����´�����ϲ�ֻ��Ѿ���ʼ������10��������Ҿ���ǰ���������ǵ��´����տ��ٴ����뱾�λ�����������߻��л������´�С��������λ����ж���
		msg.DispatchWorldMsgEvent(MsgID)

    elseif (curhour ~= 11 and curhour ~= 19) or curmin < 30 then
		-- ��ձ�������
		XinChunDengMi_Attend = {}				-- �Ƿ��ڴ˻ʱ���ڲμӹ�
		XinChunDengMi_Questions = {}				-- ������
		XinChunDengMi_Time = {}					-- ��Ŵ��⿪ʼʱ��ϵͳʱ�䣬��Ӧ��ɫid
		XinChunDengMi_RightAnswer = {}			-- ��ȷ�����ĸ�ѡ���е�����
		DengMi_Question_Table = {}
	end
end

-- ���ʼ
function XinChun_OnStart(actID)
	-- �ڸ�������ˢ��npc�´����տ���,��ũة
    -- ��ʼ�����
	XinChunDengMi_InitQuestionTable()
	XinChunDengMi_CreatureID[1] = map.MapCreateCreature(3017298127, -1, 4900401, 2171, 6646, 2219)   --����NPC
	XinChunDengMi_CreatureID[2] = map.MapCreateCreature(3017298383, -1, 4900401, 928, 10000, 2007)
	XinChunDengMi_CreatureID[3] = map.MapCreateCreature(3017299663, -1, 4900401, 2139, 18345, 2419)
	XinChunDengMi_CreatureID[4] = map.MapCreateCreature(3017299919, -1, 4900401, 1021, 20140, 975)
	XinChunDengMi_CreatureID[5] = map.MapCreateCreature(3017299151, -1, 4900401, 2038, 6365, 1799)
	XinChunDengMi_CreatureID[6] = map.MapCreateCreature(3017299407, -1, 4900401, 1822, 14024, 2280)
	XinChunDengMi_CreatureID[7] = map.MapCreateCreature(3017298127, -1, 4900417, 2166, 6646, 2219)--�����´�ʹ��
	XinChunDengMi_CreatureID[8] = map.MapCreateCreature(3017298383, -1, 4900417, 923, 10000, 2007)
	XinChunDengMi_CreatureID[9] = map.MapCreateCreature(3017299663, -1, 4900417, 2134, 18345, 2419)
	XinChunDengMi_CreatureID[10] = map.MapCreateCreature(3017299919, -1, 4900417, 1016, 20140, 975)
	XinChunDengMi_CreatureID[11] = map.MapCreateCreature(3017299151, -1, 4900417, 2033, 6365, 1799)
	XinChunDengMi_CreatureID[12] = map.MapCreateCreature(3017299407, -1, 4900417, 1817, 14024, 2280)
	XinChunDengMi_CreatureID[13] = map.MapCreateCreature(3017298127, -1, 4900402, 2166, 6646, 2224)--��ũ��
	XinChunDengMi_CreatureID[14] = map.MapCreateCreature(3017298383, -1, 4900402, 923, 10000, 2012)
    XinChunDengMi_CreatureID[15] = map.MapCreateCreature(3017299663, -1, 4900404, 2139, 18345, 2424)--�Ƶ۳�ʳ��
	XinChunDengMi_CreatureID[16] = map.MapCreateCreature(3017298127, -1, 4900404, 2171, 6646, 2224)--����ʳ��
end

-- npc����ʱ�䵽����������ڸ�������ɾ����npc
function XinChun_OnEnd(actID)
	map.MapDeleteCreature(3017298127, -1, XinChunDengMi_CreatureID[1])
	map.MapDeleteCreature(3017298383, -1, XinChunDengMi_CreatureID[2])
	map.MapDeleteCreature(3017299663, -1, XinChunDengMi_CreatureID[3])
	map.MapDeleteCreature(3017299919, -1, XinChunDengMi_CreatureID[4])
	map.MapDeleteCreature(3017299151, -1, XinChunDengMi_CreatureID[5])
	map.MapDeleteCreature(3017299407, -1, XinChunDengMi_CreatureID[6])
	map.MapDeleteCreature(3017298127, -1, XinChunDengMi_CreatureID[7])
	map.MapDeleteCreature(3017298383, -1, XinChunDengMi_CreatureID[8])
	map.MapDeleteCreature(3017299663, -1, XinChunDengMi_CreatureID[9])
	map.MapDeleteCreature(3017299919, -1, XinChunDengMi_CreatureID[10])
	map.MapDeleteCreature(3017299151, -1, XinChunDengMi_CreatureID[11])
	map.MapDeleteCreature(3017299407, -1, XinChunDengMi_CreatureID[12])
	map.MapDeleteCreature(3017298127, -1, XinChunDengMi_CreatureID[13])
	map.MapDeleteCreature(3017298383, -1, XinChunDengMi_CreatureID[14])
	map.MapDeleteCreature(3017299663, -1, XinChunDengMi_CreatureID[15])
	map.MapDeleteCreature(3017298127, -1, XinChunDengMi_CreatureID[16])

end



-- ���ʼ
function LaBa_OnStart(actID)
	-- �㲥�´����տ��ٻ��ʼ
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 100, 2114)    --��Хһ��ɽ�������������缪�������������´�ϵ�л�Ѿ���ʽ�����Ļ���������ڽ��е������˻��С���������ˡ�����ϲ�ֻᡱ���������ɹ���������Ĵ���֡������´���ʳ����⿡��ȷḻ���ݼ������ź��������������������´�ʹ�ߴ���ѯ����ӭ��ӻԾ���룬ף���´���죡
	msg.DispatchWorldMsgEvent(MsgID)
end


function LaBa_OnTimerMin(actID)
    local curhour = os.date("%H")
	local curmin = os.date("%M")
	if curhour ~= "00" and curmin ~= "00" then
	    curhour = tonumber(os.date("%H"))
	    curmin = tonumber(os.date("%M"))
	else
	    if curhour == "00" then
		    curhour = 0
		end
		if curmin == "00" then
		    curmin = 0
		end
	end
	if curhour % 3 == 0 and curmin == 0 then
	    -- �㲥�´����տ��ٻ��ʼ
	    local MsgID = msg.BeginMsgEvent()
	    msg.AddMsgEvent(MsgID, 100, 2114)    --��Хһ��ɽ�������������缪�������������´�ϵ�л�Ѿ���ʽ�����Ļ���������ڽ��е������˻��С���������ˡ�����ϲ�ֻᡱ���������ɹ���������Ĵ���֡������´���ʳ����⿡��ȷḻ���ݼ������ź��������������������´�ʹ�ߴ���ѯ����ӭ��ӻԾ���룬ף���´���죡
	    msg.DispatchWorldMsgEvent(MsgID)
	end
end

-- ע����Ӧ�¼�
aux.RegisterCreatureEvent(4900401, 7, "x4900401_OnTalk")
aux.RegisterActEvent(71, 4, "XinChun_OnTimerMin")
aux.RegisterActEvent(71, 2, "XinChun_OnStart")
aux.RegisterActEvent(71, 3, "XinChun_OnEnd")
aux.RegisterActEvent(72, 4, "LaBa_OnTimerMin")
aux.RegisterActEvent(72, 2, "LaBa_OnStart")

