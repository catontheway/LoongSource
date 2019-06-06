--н��ʹ�߽ű�

function XinChouShiZhe(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)

	local rolelevel = role.GetRoleLevel(MapID, InstanceID, RoleID)
	local Item1Num = role.GetRoleItemNum(RoleID, 3303501) --����
	local Item2Num = role.GetRoleItemNum(RoleID, 3303502) --����
	local Salary_Times = role.GetRoleScriptData(RoleID, 1, RoleDataType["Salary_Times"])
	local Last_Salary_M = role.GetRoleScriptData(RoleID, 1, RoleDataType["Last_Salary_M"])

	local current_M = os.date("%m")
	current_M = tonumber(current_M)

	if current_M ~= Last_Salary_M then
		Salary_Times = 0
		role.SetRoleScriptData(RoleID, 1, RoleDataType["Salary_Times"], 0)
		role.SetRoleScriptData(RoleID, 1, RoleDataType["Last_Salary_M"], current_M)
	end

	if TalkIndex == -1 then

		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 1611)		--��ͨ�Ի�
		msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
		msg.AddMsgEvent(MsgID, 21,	4)			--ȷ����ť
		msg.AddMsgEvent(MsgID, 1,	1612)		--���鿴��ȡ������
		msg.AddMsgEvent(MsgID, 21,	5)			--ȷ����ť
		msg.AddMsgEvent(MsgID, 1,	1613)		--����ȡ������н�ꡱ
		msg.AddMsgEvent(MsgID, 21,	6)			--ȷ����ť
		msg.AddMsgEvent(MsgID, 1,	1614)		--����ȡ������н�ꡱ
		msg.DispatchRoleMsgEvent(RoleID, MsgID)

	elseif TalkIndex == 4 then	--�鿴��ȡ˵��


		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 1615)		--��ȡ��������ȡ����
		msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
		msg.AddMsgEvent(MsgID, 21,	4)			--ȷ����ť
		msg.AddMsgEvent(MsgID, 1,	1612)		--���鿴��ȡ������
		msg.AddMsgEvent(MsgID, 21,	5)			--ȷ����ť
		msg.AddMsgEvent(MsgID, 1,	1613)		--����ȡ������н�ꡱ
		msg.AddMsgEvent(MsgID, 21,	6)			--ȷ����ť
		msg.AddMsgEvent(MsgID, 1,	1614)		--����ȡ������н�ꡱ
		msg.DispatchRoleMsgEvent(RoleID, MsgID)

	elseif TalkIndex == 5 then --�ô�����֤����ȡ

		if rolelevel < 50 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 1616)		--��ֻ��50��������Ҳſ���ȡн�꣡��
			msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
			if Salary_Times >= 15 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 1617)		--������������ȡ��15��н�꣬�޷��ٴ���ȡ����
				msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			else
				if Item1Num<10 then
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 20, 1618)		--�����Ĵ����ߵ�֤��������������10������ʱ�޷���ȡ����
					msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
				else
					local k = math.floor(rolelevel/10)*10
					role.RemoveFromRole(MapID, InstanceID, RoleID, 3303501, 10, 420)
					role.AddRoleGiftPoints(MapID, InstanceID, RoleID, Salary_RoleLevel_Tabel[k])
					role.SetRoleScriptData(RoleID, 1, RoleDataType["Salary_Times"], Salary_Times+1)

					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 20, 1620)		--�����ѳɹ���ȡ����Ϊ**������㣬����̳ǽ�����в鿴����
					msg.AddMsgEvent(MsgID, 9, Salary_RoleLevel_Tabel[k])
					msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
				end
			end
		end

	elseif TalkIndex == 6 then	--�ÿ�����֤����ȡ

		if rolelevel < 50 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 1616)		--��ֻ��50��������Ҳſ���ȡн�ꡱ
			msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
			if Salary_Times >= 15 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 1617)		--������������ȡ��15��н�꣬�޷��ٴ���ȡ����
				msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			else
				if Item2Num<5 then
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 20, 1619)		--�����Ŀ����ߵ�֤��������������5������ʱ�޷���ȡ����
					msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
				else
					local k = math.floor(rolelevel/10)*10
					role.RemoveFromRole(MapID, InstanceID, RoleID, 3303502, 5, 420)
					role.AddRoleGiftPoints(MapID, InstanceID, RoleID, Salary_RoleLevel_Tabel[k])
					role.SetRoleScriptData(RoleID, 1, RoleDataType["Salary_Times"], Salary_Times+1)

					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 20, 1620)		--�����ѳɹ���ȡ����Ϊ**������㣬����̳ǽ�����в鿴����
					msg.AddMsgEvent(MsgID, 9, Salary_RoleLevel_Tabel[k])
					msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
				end
			end
		end

	end
end

-- ע��
aux.RegisterCreatureEvent(3610202, 7, "XinChouShiZhe")
