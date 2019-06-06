XJ_XiaoLongNV_Attend = {}				-- �Ƿ��ڴ˻ʱ���ڲμӹ�
XJ_XiaoLongNV_Questions = {}				-- ����ĸ�����Ϣ
XJ_XiaoLongNV_RewardsRecevied = {}	-- �Ƿ��Ѿ���ȡ��Ʒ(1��û����)
XJ_XiaoLongNV_Role_Point = {}			-- ��һ�õķ�������Ӧ��ɫid
XJ_XiaoLongNV_Question_Num = 439	-- ���յ���Ŀ��
XJ_XiaoLongNV_Time_Counter = 30			-- ����ĵ���ʱ
XJ_XiaoLongNV_Time = {}					-- ��Ŵ��⿪ʼʱ��ϵͳʱ�䣬��Ӧ��ɫid
XJ_XiaoLongNV_RightAnswer = {}			-- ��ȷ�����ĸ�ѡ���е�����
--XJ_XiaoLongNV_CreatureID = {}			-- ��Ŵ���С��Ů���ٺ󷵻ص�id�����ڻ������ɾ��npc
XJ_DengMi_Question_Table = {}


-- npc�Ի��������������
function x4900449_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	if TalkIndex == -1 then                                                --��ʼ�Ի�
	    -- �����������
		math.randomseed(os.time())
		XJ_XiaoLongNV_InitNullTable(RoleID)
		if XJ_XiaoLongNV_RewardsRecevied[RoleID] == 1 then                    --���������꣬������û�и�
			local level = role.GetRoleLevel(MapID, InstanceID, RoleID)
			local x = XJ_XiaoLongNV_Role_Point[RoleID]/10
			local reward
			if x <= 0 or x == nil then                                        --������������
				reward = 0
			else
				reward = (300*x*level*level+x*dengmi_levelexp[level]+30000*level)/100/level
			end
			if x < 10 and x >=0 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 2701)	-- "С��Ů���٣�\n    ���δ����������##�⣬���###�㾭�飬�������������������ٽ�������
				msg.AddMsgEvent(MsgID, 9, x)
				msg.AddMsgEvent(MsgID, 9, reward)
				msg.AddMsgEvent(MsgID, 21, 10)	-- ȷ����ť
				msg.AddMsgEvent(MsgID, 1, 206)	-- "ȷ��"
				msg.AddMsgEvent(MsgID, 24, TargetID)
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			elseif x >= 10 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 2702)	-- "С��Ů���٣�\n    ���δ����������##�⡣��ϲ������10�⣬�����###�㾭���⣬���������20��������������ȷ����������ӵ���㹻�Ŀռ䣡\n   [С��ʾ������δ��ȡ������²��ܲμ���һ�ֵİ���С��Ů���]
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
		    if XJ_DengMi_Time(RoleID) then                        --���û�н����ж�λ��Ϊ1�����ж��Ƿ��ڻ��
			    if XJ_XiaoLongNV_Questions[RoleID].NUM < 20 and XJ_XiaoLongNV_Questions[RoleID].NUM > 0 then
					XJ_XiaoLongNV_GenerateQuestion(RoleID, TargetID)
				else
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 20, 2703)	-- "С��Ů���٣�\n    ���г���<color=0xffff0000>30<color=0xfffff7e0>������Ҷ�����������μ�<color=0xff05ff00>������С��Ů��<color=0xfffff7e0>������в������Ҷ����Ի�÷��ľ��齱����ÿ�������10���⻹����10������������Ϊ��������������ѡ��ͶƱ�˴�����Ͷ�������ǵ�С��Ůѡ�֡�������ͶƱ����һ��л�����<color=0xffff6000>����ʿ<color=0xfffff7e0>�ƺŽ�����"
					msg.AddMsgEvent(MsgID, 21, 4)	-- ȷ����ť
					msg.AddMsgEvent(MsgID, 1, 2704)	-- "�μӰ���С��Ů�"
					msg.AddMsgEvent(MsgID, 21, 11)	-- ȡ����ť
					msg.AddMsgEvent(MsgID, 1, 2705)	-- "�ý𱴶һ���������"
					msg.AddMsgEvent(MsgID, 21, 12)	-- ȡ����ť
					msg.AddMsgEvent(MsgID, 1, 2706)	-- "����С��Ůѡ��"
					msg.AddMsgEvent(MsgID, 21, 9)	-- ȡ����ť
					msg.AddMsgEvent(MsgID, 1, 2103)	-- "���μ�"
					msg.AddMsgEvent(MsgID, 24, TargetID)
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
				end
			else                                      --������ڻʱ��
			   --[[ if XJ_XiaoLongNV_Role_Point[RoleID] > 0 then           --�������ϴ�û���꣬�������
				    local level = role.GetRoleLevel(MapID, InstanceID, RoleID)
					local x = XJ_XiaoLongNV_Role_Point[RoleID]/10
					local reward
					if x <= 0 or x == nil then
						reward = 0
					else
						reward = (300*x*level*level+x*dengmi_levelexp[level]+30000*level)/100/level
					end
					if x < 10 and x >=0 then
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 20, 2701)	-- "С��Ů���٣�\n    ���δ����������##�⣬���###�㾭�飬�������������������ٽ�������
						msg.AddMsgEvent(MsgID, 9, x)
						msg.AddMsgEvent(MsgID, 9, reward)
						msg.AddMsgEvent(MsgID, 21, 10)	-- ȷ����ť
						msg.AddMsgEvent(MsgID, 1, 206)	-- "ȷ��"
						msg.AddMsgEvent(MsgID, 24, TargetID)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
					elseif x >= 10 then
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 20, 2702)	-- "С��Ů���٣�\n    ���δ����������##�⡣��ϲ������10�⣬�����###�㾭���⣬���������20��������������ȷ����������ӵ���㹻�Ŀռ䣡\n   [С��ʾ������δ��ȡ������²��ܲμ���һ�ֵİ���С��Ů���]
						msg.AddMsgEvent(MsgID, 9, x)
						msg.AddMsgEvent(MsgID, 9, reward)
						msg.AddMsgEvent(MsgID, 21, 10)	-- ȷ����ť
						msg.AddMsgEvent(MsgID, 1, 2111)	-- "��ȡ��Ʒ"
						msg.AddMsgEvent(MsgID, 21, 9)	-- ȷ����ť
						msg.AddMsgEvent(MsgID, 1, 2112)	-- "�Ժ���ȡ"
						msg.AddMsgEvent(MsgID, 24, TargetID)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
					end
				elseif XJ_XiaoLongNV_Role_Point[RoleID] == 0 then             --������û�����õĽ���������ʾ�´�����]]
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 20, 2707)	-- "С��Ů���٣�\n    �������Ѿ��μӹ������ˡ�\n    ���г���<color=0xffff0000>30<color=0xfffff7e0>������Ҷ�����������μ�<color=0xff05ff00>������С��Ů��<color=0xfffff7e0>������в������Ҷ����Ի�÷��ľ��齱����ÿ�������10���⻹����10������������Ϊ��������������ѡ��ͶƱ�˴�����Ͷ�������ǵ�С��Ůѡ�֡�������ͶƱ����һ��л�����<color=0xffff6000>����ʿ<color=0xfffff7e0>�ƺŽ�����"
					msg.AddMsgEvent(MsgID, 21, 11)	-- ȡ����ť
					msg.AddMsgEvent(MsgID, 1, 2705)	-- "�ý𱴶һ���������"
					msg.AddMsgEvent(MsgID, 21, 12)	-- ȡ����ť
					msg.AddMsgEvent(MsgID, 1, 2706)	-- "����С��Ůѡ��"
					msg.AddMsgEvent(MsgID, 21, 9)	-- ȷ����ť
					msg.AddMsgEvent(MsgID, 1, 207)	-- "ȡ��"
					msg.AddMsgEvent(MsgID, 24, TargetID)
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
				--end
			end
	    end
	elseif TalkIndex == 4 then                             -- �������ڻʱ��û�н������ã�������򡭡�
	    local Player_level = role.GetRoleLevel(MapID, InstanceID, RoleID)
		local CurTime = tonumber(os.date("%j"))
	    local UpdateTime = role.GetRoleScriptData(RoleID, 1, RoleDataType["XJ_Dati_Date"])
	    if Player_level < 30 then	-- �ȼ�����30���Ļ�������ʾ
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 2708)		-- "С��Ů���٣�\n    ���ĵȼ�����10�������Ƚ��ȼ�������10�������μӱ����"
			msg.AddMsgEvent(MsgID, 21, 9)		-- ȷ����ť
			msg.AddMsgEvent(MsgID, 1, 206)		-- "ȷ��"
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
	    elseif CurTime == UpdateTime then
	        local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 2709)		-- "�������Ѳμӹ�����С��Ů���������������"
			msg.AddMsgEvent(MsgID, 21, 9)		-- ȷ����ť
			msg.AddMsgEvent(MsgID, 1, 206)		-- "ȷ��"
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
	    else
			-- ���������ڿ�ʼ��
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 2710)      -- ������С��Ů�������ڿ�ʼ��
			msg.AddMsgEvent(MsgID, 21, 6)
			msg.AddMsgEvent(MsgID, 1, 206)
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
	    end
	elseif TalkIndex == 6 then                       --���տ�ʼ���Զ�����
		XJ_XiaoLongNV_GenerateQuestion(RoleID, TargetID)
	elseif TalkIndex == 0 or TalkIndex == 1 or TalkIndex == 2 or TalkIndex == 3 then    --���ѡ��ѡ�����в�ͬ�Ľ��
	    if XJ_XiaoLongNV_Time[RoleID] == nil then
		    -- �������
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 40, -1)
			msg.DispatchRoleMsgEvent(RoleID, MsgID)                            --������������轱��
			local level = role.GetRoleLevel(MapID, InstanceID, RoleID)
			local x = XJ_XiaoLongNV_Role_Point[RoleID]/10
			local reward
			if x <= 0 or x == nil then
				reward = 0
			else
				reward = (300*x*level*level+x*dengmi_levelexp[level]+30000*level)/100/level
			end
			if x < 10 and x >=0 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 2701)	-- "С��Ů���٣�\n    ���δ����������##�⣬���###�㾭�飬�������������������ٽ�������
				msg.AddMsgEvent(MsgID, 9, x)
				msg.AddMsgEvent(MsgID, 9, reward)
				msg.AddMsgEvent(MsgID, 21, 10)	-- ȷ����ť
				msg.AddMsgEvent(MsgID, 1, 206)	-- "ȷ��"
				msg.AddMsgEvent(MsgID, 24, TargetID)
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			elseif x >= 10 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 2702)	-- "С��Ů���٣�\n    ���δ����������##�⡣��ϲ������10�⣬�����###�㾭���⣬���������20��������������ȷ����������ӵ���㹻�Ŀռ䣡\n   [С��ʾ������δ��ȡ������²��ܲμ���һ�ֵİ���С��Ů���]
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
			local time1 = XJ_XiaoLongNV_Time[RoleID]
			local dtime = os.difftime(os.time(), time1)
			-- �жϴ��Ƿ���ȷ
			if XJ_XiaoLongNV_RightAnswer[RoleID] == TalkIndex then
				-- �ش�������ʱ�䲻ͬ���÷ֲ�ͬ
				if dtime <= 30 then				-- 30���ڻش����ȷ�𰸣������Ŀȫ������
					-- ���յ÷�
					XJ_XiaoLongNV_Role_Point[RoleID] = XJ_XiaoLongNV_Role_Point[RoleID] + 10
				end
			end
			-- ������Ŀ
			if XJ_XiaoLongNV_Questions[RoleID].NUM < 20 then    -- ������С��20��ʱ����������Ŀ
				XJ_XiaoLongNV_GenerateQuestion(RoleID, TargetID)
			else
				-- �������
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 40, -1)
				msg.DispatchRoleMsgEvent(RoleID, MsgID)                            --������������轱��
				local level = role.GetRoleLevel(MapID, InstanceID, RoleID)
				local x = XJ_XiaoLongNV_Role_Point[RoleID]/10
				local reward
				if x <= 0 or x == nil then
					reward = 0
				else
					reward = (300*x*level*level+x*dengmi_levelexp[level]+30000*level)/100/level
				end
				if x < 10 and x >=0 then
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 20, 2701)	-- "С��Ů���٣�\n    ���δ����������##�⣬���###�㾭�飬�������������������ٽ�������
					msg.AddMsgEvent(MsgID, 9, x)
					msg.AddMsgEvent(MsgID, 9, reward)
					msg.AddMsgEvent(MsgID, 21, 10)	-- ȷ����ť
					msg.AddMsgEvent(MsgID, 1, 206)	-- "ȷ��"
					msg.AddMsgEvent(MsgID, 24, TargetID)
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
				elseif x >= 10 then
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 20, 2702)	-- "С��Ů���٣�\n    ���δ����������##�⡣��ϲ������10�⣬�����###�㾭���⣬���������10��������������ȷ����������ӵ���㹻�Ŀռ䣡\n   [С��ʾ������δ��ȡ������²��ܲμ���һ�ֵİ���С��Ů���]
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
		local x = XJ_XiaoLongNV_Role_Point[RoleID]/10
		local reward
		if x <= 0 or x == nil or x > 20 then
		    reward = 0
		else
		    reward = (300*x*level*level+x*dengmi_levelexp[level]+30000*level)/100/level
		end
		if x < 10 and x >=0 then
			role.AddRoleExp(MapID, InstanceID, RoleID, reward)
			XJ_XiaoLongNV_Role_Point[RoleID] = 0                             --��շ����ͽ�����־λ
			XJ_XiaoLongNV_RewardsRecevied[RoleID] = 0
		elseif x >= 10 then
			local FreeSize = role.GetBagFreeSize(RoleID)
			if FreeSize < 1 then
                local MsgID = msg.BeginMsgEvent()
			    msg.AddMsgEvent(MsgID, 20, 2711)	-- "С��Ů���٣�\n    �����ҵĿռ䲻�㣬�������Ժ�������
			    msg.AddMsgEvent(MsgID, 21, 9)	-- ȷ����ť
			    msg.AddMsgEvent(MsgID, 1, 206)	-- "ȷ��"
			    msg.DispatchRoleMsgEvent(RoleID, MsgID)
			else
				role.AddRoleExp(MapID, InstanceID, RoleID, reward)
				role.AddRoleItem(MapID, InstanceID, RoleID, 3301056, 10, -1, 8, 420)
				XJ_XiaoLongNV_Role_Point[RoleID] = nil
				XJ_XiaoLongNV_RewardsRecevied[RoleID] = nil
				XJ_XiaoLongNV_Questions[RoleID] = nil
		        XJ_XiaoLongNV_Time[RoleID] = nil
		        XJ_XiaoLongNV_RightAnswer[RoleID] = nil
	        end
		end
	elseif TalkIndex == 11 then
	    local MsgID = msg.BeginMsgEvent()
        msg.AddMsgEvent(MsgID, 20, 2712)	--С��Ů���٣�\n    ÿʮ���𱴿��Զһ�һ����������������Ҫ�һ���
		msg.AddMsgEvent(MsgID, 24, TargetID)
		msg.AddMsgEvent(MsgID, 21, 13)	-- ȷ����ť
		msg.AddMsgEvent(MsgID, 1, 2705)	-- "�һ���������"
		msg.AddMsgEvent(MsgID, 21, -1)	-- ȷ����ť
		msg.AddMsgEvent(MsgID, 1, 207)	-- "����"
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	elseif TalkIndex == 12 then
	    local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 2713)	--С��Ů���٣�\n    <color=0xffff6000>��Ԫ��<color=0xfffff7e0>��<color=0xffff6000>������������<color=0xfffff7e0>��������˭�ң��������Ĳ��룡С��Ůѡ�����ھ��У������Ů�Ǿ�������վ<color=0xff05ff00>xln.love0991.com<color=0xfffff7e0>�����μӣ��˽����Ļ��Ϣ��\n    ���⣬���г���<color=0xffff0000>30<color=0xfffff7e0>������Ҷ���������������<color=0xffffae00>10�������һ�1����������<color=0xfffff7e0>����������ѡ��ͶƱ�˴�����Ͷ�������ǵ�С��Ůѡ�֡�
		msg.AddMsgEvent(MsgID, 21, -1)	-- ȷ����ť
		msg.AddMsgEvent(MsgID, 1, 206)	-- "ȷ��"
		msg.AddMsgEvent(MsgID, 24, TargetID)
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
    elseif TalkIndex == 13 then
		local Player_level = role.GetRoleLevel(MapID, InstanceID, RoleID)
        local JinBei = role.GetRoleItemNum(RoleID, 3301055)
        local FreeSize = role.GetBagFreeSize(RoleID)
        if Player_level < 30 then
		    local MsgID = msg.BeginMsgEvent()
		    msg.AddMsgEvent(MsgID, 26, 2714)	--ֻ�и���30������Ҳſ��Զһ���������
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		elseif JinBei < 10 then
		    local MsgID = msg.BeginMsgEvent()
		    msg.AddMsgEvent(MsgID, 26, 2715)	--����Ҫ����10���𱴲ſ��Զһ���������
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		elseif FreeSize < 1 then
		    local MsgID = msg.BeginMsgEvent()
		    msg.AddMsgEvent(MsgID, 26, 2716)	--��������ñ��������һ���������
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
		    if 0 == role.RemoveFromRole(MapID, InstanceID, RoleID, 3301055, 10, 420) then
	            role.AddRoleItem(MapID, InstanceID, RoleID, 3301056, 1, -1, 8, 420)
            end
		end
	end
end


-- ע����Ӧ�¼�
aux.RegisterCreatureEvent(4900449, 7, "x4900449_OnTalk")


