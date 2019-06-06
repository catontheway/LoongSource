
function feisheng_clickbutton(MapID, InstanceID, RoleID)
    if FeiSheng_trigger == 1 then
		local level = role.GetRoleLevel(MapID, InstanceID, RoleID)
		if level >= 80 then
			if (role.GetSoarValue(RoleID) == 0 or role.GetSoarValue(RoleID) == 2) and not (role.IsRoleHaveQuest(MapID, InstanceID, RoleID, 40001) or role.IsRoleCompleteQuest(MapID, InstanceID, RoleID, 40001)) then
				role.AddQuest(RoleID, 40001)
			end
			if (role.IsRoleHaveQuest(MapID, InstanceID, RoleID, 40001) or role.IsRoleCompleteQuest(MapID, InstanceID, RoleID, 40001)) and role.GetSoarValue(RoleID) ~= 1 then
				role.SetSoarValue(RoleID, 2)
			end
		end
	else
	    local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 71, 3114)--����ϵͳ��δ���ţ������ڴ���
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	end
end

aux.RegisterRoleEvent(39, "feisheng_clickbutton")

--�������
function feishengquest_Oncheckaccept(MapID, InstanceID, QuestID, RoleID, NPCID)

    if role.GetSoarValue(RoleID) == 1 then
	    local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 3181)	--���Ѿ��������޷���ȡ������
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	    return 0
	else
	    return 1
	end


end

aux.RegisterQuestEvent(40004, 4, "feishengquest_Oncheckaccept")
aux.RegisterQuestEvent(40005, 4, "feishengquest_Oncheckaccept")
aux.RegisterQuestEvent(40006, 4, "feishengquest_Oncheckaccept")
aux.RegisterQuestEvent(40007, 4, "feishengquest_Oncheckaccept")

-- npc�Ի����һ�����
function x4900702_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)

	if TalkIndex == -1 then    	--��ʼ�Ի�
	    if KuiLingBaoShu_Start == 1 then
		    local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 3136)	--��ü�ɹ�:\n        ���鱦�������������������һ������������һ���������֮���ܾ������������ħ���֡�����������ֲ�������칬֮���������������й����֪�����ɷ������� \n        С��ʾ���û����һ����Сʱ���������л�����ϡ��ǿ�����ߺʹ������顣������Ժ������Լ����ò��϶һ������\n        ��ұ�����Ӳ�����ȡ���ֵ��ߡ�����������6����Աʱ����ˮ��ʩ�ʻ�õľ��齱�����ڵ���ʱ��6����
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 21, 16)	-- ȷ����ť
			msg.AddMsgEvent(MsgID, 1, 3133)	-- "��ȡ�ɽ���ˮ"
			msg.AddMsgEvent(MsgID, 21, 17)	-- ȷ����ť
			msg.AddMsgEvent(MsgID, 1, 3134)	-- "��ȡ�ɽ����"
			msg.AddMsgEvent(MsgID, 21, 18)	-- ȷ����ť
			msg.AddMsgEvent(MsgID, 1, 3135)	-- "��ȡ���羭����"
			msg.AddMsgEvent(MsgID, 21, 19)	-- ȷ����ť
			msg.AddMsgEvent(MsgID, 1, 207)	-- "����"
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
			if role.GetSoarValue(RoleID) == 2 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 3115)	--��ü�ɹ�:\n        ��������˾���ɹ٣���֮ͥ�ϵ�������䣬���Ѳ�ε�����һӦ�ɱ��ɳ�졣�����ʿ��������Ϊ�����֮�����������ÿ��Ը�����һЩ�����ܼ��İ�����ֻҪ�㽫һЩ����˼乲�õĲ��Ͻ����ڱ��ɣ����ɾͽ���Щ�������������㡣�������õ�����������Ϳ���ĳ����ˡ����⣬�������������˵����ֽ����Ҽ�������֮��������ۣ���ʿ����������Ҫץ�����ǡ�
				msg.AddMsgEvent(MsgID, 24, TargetID)
				msg.AddMsgEvent(MsgID, 21, 5)	-- ȷ����ť
				msg.AddMsgEvent(MsgID, 1, 3116)	-- "��ȡ������"
				msg.AddMsgEvent(MsgID, 21, 19)	-- ȷ����ť
				msg.AddMsgEvent(MsgID, 1, 207)	-- "����"
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			else
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 3117)	--��ü�ɹ�:\n        �õ�����֮��Ӧ�����²���Ϊ�أ���Щ���������Щ����̰����������������������𣬶��ݺ�ɱ�ߣ�������Ǵ��
				msg.AddMsgEvent(MsgID, 24, TargetID)
				msg.AddMsgEvent(MsgID, 21, 19)	-- ȷ����ť
				msg.AddMsgEvent(MsgID, 1, 207)	-- "����"
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			end
		end
	elseif TalkIndex == 5 then
	    --[[local k = role.GetRoleScriptData(RoleID, 1, RoleDataType["FeiShengDuihuan_Update"])
		local CurTime = tonumber(os.date("%j"))
		if k ~= CurTime then
		    role.SetRoleScriptData(RoleID, 1, RoleDataType["FeiShengDuihuan_Update"], CurTime)
	        role.SetRoleScriptData(RoleID, 1, RoleDataType["FeiShengDuihuan_times"], 0)
	        local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 3118)	  --��ü�ɹ٣�\n        ֻ�п�ʯ�Ͳ��������ණ����������������ó��������ѡ������һ������Ҷһ������
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 21, 6)	-- ȷ����ť
			msg.AddMsgEvent(MsgID, 1, 3119)	-- "���ɿ�ʯ"
			msg.AddMsgEvent(MsgID, 21, 7)	-- ȷ����ť
			msg.AddMsgEvent(MsgID, 1, 3120)	-- "���ɲ���"
			msg.AddMsgEvent(MsgID, 21, -1)	-- ȷ����ť
			msg.AddMsgEvent(MsgID, 1, 207)	-- "����"
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
	   else
		    local b = role.GetRoleScriptData(RoleID, 1, RoleDataType["FeiShengDuihuan_times"])
			if b < 10 then]]
			    local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 3118)	  --��ü�ɹ٣�\n        ֻ�п�ʯ�Ͳ��������ණ����������������ó��������ѡ������һ������Ҷһ������
				msg.AddMsgEvent(MsgID, 24, TargetID)
				msg.AddMsgEvent(MsgID, 21, 6)	-- ȷ����ť
				msg.AddMsgEvent(MsgID, 1, 3119)	-- "���ɿ�ʯ"
				msg.AddMsgEvent(MsgID, 21, 7)	-- ȷ����ť
				msg.AddMsgEvent(MsgID, 1, 3120)	-- "���ɲ���"
				msg.AddMsgEvent(MsgID, 21, -1)	-- ȷ����ť
				msg.AddMsgEvent(MsgID, 1, 207)	-- "����"
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			--[[else
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 3121)	  --��ü�ɹ٣�\n        ��ÿ��ֻ���ò��϶һ�10���������׼���ò������������һ���
				msg.AddMsgEvent(MsgID, 24, TargetID)
				msg.AddMsgEvent(MsgID, 21, -1)	-- ȷ����ť
				msg.AddMsgEvent(MsgID, 1, 207)	-- "����"
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
    		end
		end]]
	elseif TalkIndex == 6 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 3122)	  --��ü�ɹ٣�\n        ���е�ʯ�ϣ���ʯ���������󣬽����������ȡ�����ÿ��ֻ�ܶһ�10�Σ���ѡ������Ҫ�һ������ͣ�
	    msg.AddMsgEvent(MsgID, 24, TargetID)
		msg.AddMsgEvent(MsgID, 21, 8)	-- ȷ����ť
		msg.AddMsgEvent(MsgID, 1, 3123)	-- "10�������϶һ�200������"
		msg.AddMsgEvent(MsgID, 21, 9)	-- ȷ����ť
		msg.AddMsgEvent(MsgID, 1, 3124)	-- "10��Ʒ���϶һ�35������"
		msg.AddMsgEvent(MsgID, 21, 10)	-- ȷ����ť
		msg.AddMsgEvent(MsgID, 1, 3125)	-- "10ϡ�в��϶һ�6������"
		msg.AddMsgEvent(MsgID, 21, 11)	-- ȷ����ť
		msg.AddMsgEvent(MsgID, 1, 3126)	-- "10���ʲ��϶һ�1������"
		msg.AddMsgEvent(MsgID, 21, 5)	-- ȷ����ť
		msg.AddMsgEvent(MsgID, 1, 207)	-- "����"
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	elseif TalkIndex == 7 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 3127)	  --��ü�ɹ٣�\n        ���е��鲼���޲���Ƥ�˿����ɴ����������ȡ�����ÿ��ֻ�ܶһ�10�Σ���ѡ������Ҫ�һ������ͣ�
	    msg.AddMsgEvent(MsgID, 24, TargetID)
		msg.AddMsgEvent(MsgID, 21, 12)	-- ȷ����ť
		msg.AddMsgEvent(MsgID, 1, 3123)	-- "10�������϶һ�200������"
		msg.AddMsgEvent(MsgID, 21, 13)	-- ȷ����ť
		msg.AddMsgEvent(MsgID, 1, 3124)	-- "10��Ʒ���϶һ�35������"
		msg.AddMsgEvent(MsgID, 21, 14)	-- ȷ����ť
		msg.AddMsgEvent(MsgID, 1, 3125)	-- "10ϡ�в��϶һ�6������"
		msg.AddMsgEvent(MsgID, 21, 15)	-- ȷ����ť
		msg.AddMsgEvent(MsgID, 1, 3126)	-- "10���ʲ��϶һ�1������"
		msg.AddMsgEvent(MsgID, 21, 5)	-- ȷ����ť
		msg.AddMsgEvent(MsgID, 1, 207)	-- "����"
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	elseif TalkIndex <= 11 and TalkIndex >= 8 then
	    local FreeSize = role.GetBagFreeSize(RoleID)
		if FreeSize < 1 then
		    local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 3128)	  --��ü�ɹ٣�\n        �������еĿ�λ���㡣�뱣֤�����϶���һ����λ�����һ���
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 21, 6)	-- ȷ����ť
			msg.AddMsgEvent(MsgID, 1, 207)	-- "����"
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
		--[[local k = role.GetRoleScriptData(RoleID, 1, RoleDataType["FeiShengDuihuan_Update"])
		local CurTime = tonumber(os.date("%j"))
		local bool_times = true
		if k ~= CurTime then
		    role.SetRoleScriptData(RoleID, 1, RoleDataType["FeiShengDuihuan_Update"], CurTime)
	        role.SetRoleScriptData(RoleID, 1, RoleDataType["FeiShengDuihuan_times"], 0)
	    else
		    local b = role.GetRoleScriptData(RoleID, 1, RoleDataType["FeiShengDuihuan_times"])
			if b >= 10 then
                bool_times = false
    		end
		end
		if 	bool_times then]]
		    local num_type = {}
			num_type[8] = {10, 200, 2000901,2000902,2000903,2000904,2000905}
			num_type[9] = {10, 35, 1160001,2000005,2000042,2000010,2000045,1160002,2000015,2000048,2000020,2000051,2000025,2000054}
			num_type[10] = {10, 6, 1160101,2000004,2000041,1160102,2000009,2000044,1160103,2000014,2000047,2000019,2000050,2000024,2000053}
			num_type[11] = {10, 1, 1160201,2000003,2000040,2000008,2000043,1160202,2000013,2000046,2000018,2000049,2000023,2000052}
		    local bool_duihuan = false
			local allnum = 0
			for k,v in pairs(num_type[TalkIndex]) do
			    if k > 2 then
					local i = role.GetRoleItemNum(RoleID, v)
					allnum = i + allnum
				end
				if num_type[TalkIndex][1] <= allnum then
				    bool_duihuan = true
					break
				end
			end
			local shengyu = 10
			if bool_duihuan then
			    for k,v in pairs(num_type[TalkIndex]) do
					if k > 2 then
						local i = role.GetRoleItemNum(RoleID, v)
						if i < shengyu then
							role.RemoveFromRole(MapID, InstanceID, RoleID, v, i, 420)
							shengyu = shengyu - i
						else
							role.RemoveFromRole(MapID, InstanceID, RoleID, v, shengyu, 420)
							break
						end
					end
				end
				role.AddRoleItem(MapID, InstanceID, RoleID, 1351203, num_type[TalkIndex][2], -1, 8, 420)
			else
			    local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 3129)	  --��ü�ɹ٣�\n        ��û���㹻�Ĳ��������жһ�����ȷ��������������ָ�����͵Ĳ��ϡ�
				msg.AddMsgEvent(MsgID, 24, TargetID)
				msg.AddMsgEvent(MsgID, 21, 6)	-- ȷ����ť
				msg.AddMsgEvent(MsgID, 1, 207)	-- "����"
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			end
		end
		--[[else
		    local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 3121)	  --��ü�ɹ٣�\n        ��ÿ��ֻ���ò��϶һ�10���������׼���ò������������һ���
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 21, -1)	-- ȷ����ť
			msg.AddMsgEvent(MsgID, 1, 207)	-- "����"
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end]]
	elseif TalkIndex <= 15 and TalkIndex >= 12 then
	    local FreeSize = role.GetBagFreeSize(RoleID)
		if FreeSize < 1 then
		    local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 3128)	  --��ü�ɹ٣�\n        �������еĿ�λ���㡣�뱣֤�����϶���һ����λ�����һ���
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 21, 7)	-- ȷ����ť
			msg.AddMsgEvent(MsgID, 1, 207)	-- "����"
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
		--[[local k = role.GetRoleScriptData(RoleID, 1, RoleDataType["FeiShengDuihuan_Update"])
		local CurTime = tonumber(os.date("%j"))
		local bool_times = true
		if k ~= CurTime then
		    role.SetRoleScriptData(RoleID, 1, RoleDataType["FeiShengDuihuan_Update"], CurTime)
	        role.SetRoleScriptData(RoleID, 1, RoleDataType["FeiShengDuihuan_times"], 0)
	    else
		    local b = role.GetRoleScriptData(RoleID, 1, RoleDataType["FeiShengDuihuan_times"])
			if b >= 10 then
                bool_times = false
    		end
		end
		if 	bool_times then]]
		    local num_type = {}
			num_type[12] = {10, 200, 2000906,2000908,2000907,2000910,2000909}
			num_type[13] = {10, 35,2000205,2000242,2000215,2000248,2000210,2000245,2000225,2000254,2000220,2000251}
			num_type[14] = {10, 6,1160104,2000204,2000241,1160106,2000214,2000247,1160105,2000209,2000244,2000224,2000253,2000219,2000250}
			num_type[15] = {10, 1,2000203,2000240,2000213,2000246,2000208,2000243,2000223,2000252,2000218,2000249}
		    local bool_duihuan = false
			local allnum = 0
			for k,v in pairs(num_type[TalkIndex]) do
			    if k > 2 then
					local i = role.GetRoleItemNum(RoleID, v)
					allnum = i + allnum
				end
				if num_type[TalkIndex][1] <= allnum then
				    bool_duihuan = true
					break
				end
			end
			local shengyu = 10
			if bool_duihuan then
			    for k,v in pairs(num_type[TalkIndex]) do
					if k > 2 then
						local i = role.GetRoleItemNum(RoleID, v)
						if i < shengyu then
							role.RemoveFromRole(MapID, InstanceID, RoleID, v, i, 420)
							shengyu = shengyu - i
						else
							role.RemoveFromRole(MapID, InstanceID, RoleID, v, shengyu, 420)
							break
						end
					end
				end
				role.AddRoleItem(MapID, InstanceID, RoleID, 1351203, num_type[TalkIndex][2], -1, 8, 420)
			else
			    local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 3129)	  --��ü�ɹ٣�\n        ��û���㹻�Ĳ��������жһ�����ȷ��������������ָ�����͵Ĳ��ϡ�
				msg.AddMsgEvent(MsgID, 24, TargetID)
				msg.AddMsgEvent(MsgID, 21, 7)	-- ȷ����ť
				msg.AddMsgEvent(MsgID, 1, 207)	-- "����"
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			end
		--[[else
		    local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 3121)	  --��ü�ɹ٣�\n        ��ÿ��ֻ���ò��϶һ�10���������׼���ò������������һ���
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 21, -1)	-- ȷ����ť
			msg.AddMsgEvent(MsgID, 1, 207)	-- "����"
			msg.DispatchRoleMsgEvent(RoleID, MsgID)]]
		end
	elseif TalkIndex == 16 then
	    if KuiLingBaoShu_Complete == 1 then
		    local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 3137)	  --��ü�ɹ٣�\n        ���鱦���Ѿ����ɣ�������Ҫ��Щ�����ˡ�
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 21, -1)	-- ȷ����ť
			msg.AddMsgEvent(MsgID, 1, 207)	-- "����"
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
		    local TeamID = role.IsRoleHaveTeam(MapID, InstanceID, RoleID)
			if TeamID ~= nil and TeamID ~= 4294967295 then
				local FreeSize = role.GetBagFreeSize(RoleID)
				if FreeSize < 1 then
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 20, 3140)	  --��ü�ɹ٣�\n        �������еĿ�λ���㡣�뱣֤�����϶���һ����λ������ȡ��
					msg.AddMsgEvent(MsgID, 24, TargetID)
					msg.AddMsgEvent(MsgID, 21, -1)	-- ȷ����ť
					msg.AddMsgEvent(MsgID, 1, 207)	-- "����"
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
				else
					local i = role.GetRoleItemNum(RoleID, 1351202)
					if i < 5 then
						role.AddRoleItem(MapID, InstanceID, RoleID, 1351202, 5-i, -1, 8, 420)
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 20, 3138)	  --��ü�ɹ٣�\n        ����X��XXX�����úá�����֮����������ұ�����ȡ��\n        С��ʾ�����ÿ���˶����Ի��120�ν�ˮ��ʩ�ʾ��飬ֻҪ��������һ�˽�ˮ����ʩ�ʣ�ȫ�ӵ����д������ص���Ҷ�����þ��齱����
						msg.AddMsgEvent(MsgID, 24, TargetID)
						msg.AddMsgEvent(MsgID, 9, 5-i)
						msg.AddMsgEvent(MsgID, 4, 1351202)
						msg.AddMsgEvent(MsgID, 21, -1)	-- ȷ����ť
						msg.AddMsgEvent(MsgID, 1, 207)	-- "����"
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
					else
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 20, 3139)	  --��ü�ɹ٣�\n        ÿ����ֻ��Я��5�������ˮ��5�������ʣ�1�����羭���䡣��������ȡ����ĸ���Ʒ�ˡ�
						msg.AddMsgEvent(MsgID, 24, TargetID)
						msg.AddMsgEvent(MsgID, 21, -1)	-- ȷ����ť
						msg.AddMsgEvent(MsgID, 1, 207)	-- "����"
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
					end
				end
			else
			    local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 3179)	  --��ü�ɹ٣�\n        ���������һ�����������ȡ����Ʒ�����������Խ�࣬���ܻ�õĽ�ˮ��ʩ�ʾ��齱����Խ�ߡ�������ĳ�Ա�ﵽ6��ʱ����ˮ��ʩ��һ�����ܻ�þ��齫��һ����ʱ��6����
				msg.AddMsgEvent(MsgID, 24, TargetID)
				msg.AddMsgEvent(MsgID, 21, -1)	-- ȷ����ť
				msg.AddMsgEvent(MsgID, 1, 207)	-- "����"
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			end
		end
	elseif TalkIndex == 17 then
        if KuiLingBaoShu_Complete == 1 then
		    local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 3137)	  --��ü�ɹ٣�\n        ���鱦���Ѿ����ɣ�������Ҫ��Щ�����ˡ�
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 21, -1)	-- ȷ����ť
			msg.AddMsgEvent(MsgID, 1, 207)	-- "����"
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
		    local TeamID = role.IsRoleHaveTeam(MapID, InstanceID, RoleID)
			if TeamID ~= nil and TeamID ~= 4294967295 then
				local FreeSize = role.GetBagFreeSize(RoleID)
				if FreeSize < 1 then
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 20, 3140)	  --��ü�ɹ٣�\n        �������еĿ�λ���㡣�뱣֤�����϶���һ����λ������ȡ��
					msg.AddMsgEvent(MsgID, 24, TargetID)
					msg.AddMsgEvent(MsgID, 21, -1)	-- ȷ����ť
					msg.AddMsgEvent(MsgID, 1, 207)	-- "����"
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
				else
					local i = role.GetRoleItemNum(RoleID, 1351201)
					if i < 5 then
						role.AddRoleItem(MapID, InstanceID, RoleID, 1351201, 5-i, -1, 8, 420)
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 20, 3138)	  --��ü�ɹ٣�\n        ����X��XXX�����úá�����֮����������ұ�����ȡ��\n        С��ʾ�����ÿ���˶����Ի��120�ν�ˮ��ʩ�ʾ��飬ֻҪ��������һ�˽�ˮ����ʩ�ʣ�ȫ�ӵ����д������ص���Ҷ�����þ��齱����
						msg.AddMsgEvent(MsgID, 24, TargetID)
						msg.AddMsgEvent(MsgID, 9, 5-i)
						msg.AddMsgEvent(MsgID, 4, 1351201)
						msg.AddMsgEvent(MsgID, 21, -1)	-- ȷ����ť
						msg.AddMsgEvent(MsgID, 1, 207)	-- "����"
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
					else
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 20, 3139)	  --��ü�ɹ٣�\n        ÿ����ֻ��Я��5�������ˮ��5�������ʣ�1�����羭���䡣��������ȡ����ĸ���Ʒ�ˡ�
						msg.AddMsgEvent(MsgID, 24, TargetID)
						msg.AddMsgEvent(MsgID, 21, -1)	-- ȷ����ť
						msg.AddMsgEvent(MsgID, 1, 207)	-- "����"
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
					end
				end
			else
			    local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 3179)	  --��ü�ɹ٣�\n        ���������һ�����������ȡ����Ʒ�����������Խ�࣬���ܻ�õĽ�ˮ��ʩ�ʾ��齱����Խ�ߡ�������ĳ�Ա�ﵽ6��ʱ����ˮ��ʩ��һ�����ܻ�þ��齫��һ����ʱ��6����
				msg.AddMsgEvent(MsgID, 24, TargetID)
				msg.AddMsgEvent(MsgID, 21, -1)	-- ȷ����ť
				msg.AddMsgEvent(MsgID, 1, 207)	-- "����"
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			end
		end
	elseif TalkIndex == 18 then
	    local FreeSize = role.GetBagFreeSize(RoleID)
		if FreeSize < 1 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 3140)	  --��ü�ɹ٣�\n        �������еĿ�λ���㡣�뱣֤�����϶���һ����λ������ȡ��
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 21, -1)	-- ȷ����ť
			msg.AddMsgEvent(MsgID, 1, 207)	-- "����"
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
		    local TeamID = role.IsRoleHaveTeam(MapID, InstanceID, RoleID)
			if TeamID ~= nil and TeamID ~= 4294967295 then
				if KuiLingBaoShu_Start == 1 and KuiLingBaoShu_jilu[RoleID] == nil then
					KuiLingBaoShu_jilu[RoleID] = {jingyanxiang = 1, guaiwu = 0, jiaoshui = 0}
					role.AddRoleItem(MapID, InstanceID, RoleID, 1351214, 1, -1, 8, 420)
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 20, 3141)	  --��ü�ɹ٣�\n        ���羭������������鱦�����Խ���ħ֮���ӡ��ת��������������������ɱǰ����ֹ���鱦����������Щ��ħ�����ۻ��������д������ħ֮�꣨�������κ�һ����һ�ɱ��ħ֮�꽫Ϊ���еĶ����Ա�Ļ��羭��������һ����ħ֮�꣩�����������е���ħ֮��ﵽһ������֮��ʹ�������Ի�ô������顣
					msg.AddMsgEvent(MsgID, 24, TargetID)
					msg.AddMsgEvent(MsgID, 21, -1)	-- ȷ����ť
					msg.AddMsgEvent(MsgID, 1, 207)	-- "����"
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
				elseif KuiLingBaoShu_Start == 1 and KuiLingBaoShu_jilu[RoleID].jingyanxiang == 0 then
					KuiLingBaoShu_jilu[RoleID].jingyanxiang = 1
					role.AddRoleItem(MapID, InstanceID, RoleID, 1351214, 1, -1, 8, 420)
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 20, 3141)	  --��ü�ɹ٣�\n        ���羭������������鱦�����Խ���ħ֮���ӡ��ת��������������������ɱǰ����ֹ���鱦����������Щ��ħ�����ۻ��������д������ħ֮�꣨�������κ�һ����һ�ɱ��ħ֮�꽫Ϊ���еĶ����Ա�Ļ��羭��������һ����ħ֮�꣩�����������е���ħ֮��ﵽһ������֮��ʹ�������Ի�ô������顣
					msg.AddMsgEvent(MsgID, 24, TargetID)
					msg.AddMsgEvent(MsgID, 21, -1)	-- ȷ����ť
					msg.AddMsgEvent(MsgID, 1, 207)	-- "����"
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
				elseif role.GetRoleItemNum(RoleID, 1351214) > 1 or KuiLingBaoShu_jilu[RoleID].jingyanxiang == 1 then
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 20, 3142)	  --��ü�ɹ٣�\n        ÿ�λÿ����ֻ����ȡһ�����羭���䡣
					msg.AddMsgEvent(MsgID, 24, TargetID)
					msg.AddMsgEvent(MsgID, 21, -1)	-- ȷ����ť
					msg.AddMsgEvent(MsgID, 1, 207)	-- "����"
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
				end
			else
			    local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 3179)	  --��ü�ɹ٣�\n        ���������һ�����������ȡ����Ʒ�����������Խ�࣬���ܻ�õĽ�ˮ��ʩ�ʾ��齱����Խ�ߡ�������ĳ�Ա�ﵽ6��ʱ����ˮ��ʩ��һ�����ܻ�þ��齫��һ����ʱ��6����
				msg.AddMsgEvent(MsgID, 24, TargetID)
				msg.AddMsgEvent(MsgID, 21, -1)	-- ȷ����ť
				msg.AddMsgEvent(MsgID, 1, 207)	-- "����"
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			end
		end
	end
end


-- ע����Ӧ�¼�
aux.RegisterCreatureEvent(4900702, 7, "x4900702_OnTalk")
