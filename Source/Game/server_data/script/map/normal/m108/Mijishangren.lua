
--[[�ؼ�����

1442201	�ؼ�����
1442202	�ؼ�����
1442203	�ؼ�������
1442204	�ؼ�����
1442205	�ؼ�������
1442206	�ؼ�������
1442207	�ؼ���ҽ��
1442208	�ؼ�������

]]--

--�������Ƿ��п��Զһ����ؼ�



function mijijiance(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

	local Silver = 	role.GetRoleSilver(MapID, InstanceID, RoleID)
	local Item = role.GetRoleItemNum(RoleID,2310170)
	local MiJi_Request = false
	local chiyoumijishu = 0
	local	MiJi_ID = {}
			MiJi_ID[1] = 1442201
			MiJi_ID[2] = 1442202
			MiJi_ID[3] = 1442203
			MiJi_ID[4] = 1442204
			MiJi_ID[5] = 1442205
			MiJi_ID[6] = 1442206
			MiJi_ID[7] = 1442207
			MiJi_ID[8] = 1442208
	for k = 1,8 do
		local n = role.GetRoleItemNum(RoleID,MiJi_ID[k])
		chiyoumijishu = chiyoumijishu + n
	end
	if chiyoumijishu > 0 then
		if	Silver >= 10000000 then
			if	Item >= 100	then
				MiJi_Request = true
			else
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 26, 4202)   --������������
				msg.AddMsgEvent(MsgID, 4, 2310170) --��õ�������
				msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
			end
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 26, 4201)   --��Ǯ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
		end
	else
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 4203)   --ȱ���ؼ�
		msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
	end
	return MiJi_Request
end

function mijishangren_On_Talk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)

	local MijiID_1 = {1442202,1442203,1442204,1442205,1442206,1442207,1442208}
	local MijiID_2 = {1442201,1442203,1442204,1442205,1442206,1442207,1442208}
	local MijiID_3 = {1442201,1442202,1442204,1442205,1442206,1442207,1442208}
	local MijiID_4 = {1442201,1442202,1442203,1442205,1442206,1442207,1442208}
	local MijiID_5 = {1442201,1442202,1442203,1442204,1442206,1442207,1442208}
	local MijiID_6 = {1442201,1442202,1442203,1442204,1442205,1442207,1442208}
	local MijiID_7 = {1442201,1442202,1442203,1442204,1442205,1442206,1442208}
	local MijiID_8 = {1442201,1442202,1442203,1442204,1442205,1442206,1442207}
	local Silver = 	role.GetRoleSilver(MapID, InstanceID, RoleID)
	local Item = role.GetRoleItemNum(RoleID,2310170)

	if TalkIndex == -1 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 4204)	-- "����ӵ�����Լ�תְ���ʲ�ƥ����ؼ��������ڴ�����һ���ĵ��ߺͽ�Ǯ��ȡ����Ҫ���ؼ���\n �һ��������£���һ�����ؼ� + �����֮��*100 + 1000��"
		msg.AddMsgEvent(MsgID, 24, TargetID)
		msg.AddMsgEvent(MsgID, 21, 4)	-- ѡ��һ
		msg.AddMsgEvent(MsgID, 1, 4205)	-- "�һ��⹦�����ؼ�"
		msg.AddMsgEvent(MsgID, 21, 5)	-- ѡ���
		msg.AddMsgEvent(MsgID, 1, 4206)	-- "�һ��ڹ������ؼ�"
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	elseif TalkIndex == 4 then 	-- ѡ���⹦�����ؼ�
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 4207)	-- "�⹦�����ؼ�"
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 21, 6)	-- ѡ��1
			msg.AddMsgEvent(MsgID, 1, 4209)	-- "�һ��ؼ�����"
			msg.AddMsgEvent(MsgID, 21, 7)	-- ѡ��2
			msg.AddMsgEvent(MsgID, 1, 4210)	-- "�һ��ؼ�����"
			msg.AddMsgEvent(MsgID, 21, 8)	-- ѡ��3
			msg.AddMsgEvent(MsgID, 1, 4211)	-- "�һ��ؼ�������"
			msg.AddMsgEvent(MsgID, 21, 9)	-- ѡ��4
			msg.AddMsgEvent(MsgID, 1, 4212)	-- "�һ��ؼ�����"
			msg.AddMsgEvent(MsgID, 21, -1)	-- ѡ��5
			msg.AddMsgEvent(MsgID, 1, 4125)	-- "����"
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
	elseif TalkIndex == 6 then
			if mijijiance(MapID, InstanceID, TargetID, TargetTypeID, RoleID) then
				for i = 1, 7 do
					local m = MijiID_1[i]
					local n = role.GetRoleItemNum(RoleID,m)
					if n ~= 0 then
						role.RemoveFromRole(MapID, InstanceID, RoleID, m, 1, 420)
						role.RemoveFromRole(MapID, InstanceID, RoleID, 2310170, 100, 420)
						role.DecRoleSilver(MapID, InstanceID, RoleID, 10000000, 101)
						role.AddRoleItem(MapID, InstanceID, RoleID, 1442201, 1, -1, 8, 420)
					break
					end
				end
			end
	elseif TalkIndex == 7 then
			if mijijiance(MapID, InstanceID, TargetID, TargetTypeID, RoleID)  then
				for i = 1, 7 do
					local m = MijiID_2[i]
					local n = role.GetRoleItemNum(RoleID,m)
					if n ~= 0 then
						role.RemoveFromRole(MapID, InstanceID, RoleID, m, 1, 420)
						role.RemoveFromRole(MapID, InstanceID, RoleID, 2310170, 100, 420)
						role.DecRoleSilver(MapID, InstanceID, RoleID, 10000000, 101)
						role.AddRoleItem(MapID, InstanceID, RoleID, 1442202, 1, -1, 8, 420)
					break
					end
				end
			end
	elseif TalkIndex == 8 then
			if mijijiance(MapID, InstanceID, TargetID, TargetTypeID, RoleID)  then
				for i = 1, 7 do
					local m = MijiID_3[i]
					local n = role.GetRoleItemNum(RoleID, m)
					if n ~= 0 then
						role.RemoveFromRole(MapID, InstanceID, RoleID, m, 1, 420)
						role.RemoveFromRole(MapID, InstanceID, RoleID, 2310170, 100, 420)
						role.DecRoleSilver(MapID, InstanceID, RoleID, 10000000, 101)
						role.AddRoleItem(MapID, InstanceID, RoleID, 1442203, 1, -1, 8, 420)
					break
					end
				end
			end
	elseif TalkIndex == 9 then
			if mijijiance(MapID, InstanceID, TargetID, TargetTypeID, RoleID)  then
				for i = 1, 7 do
					local m = MijiID_4[i]
					local n = role.GetRoleItemNum(RoleID,m)
					if n ~= 0 then
						role.RemoveFromRole(MapID, InstanceID, RoleID, m, 1, 420)
						role.RemoveFromRole(MapID, InstanceID, RoleID, 2310170, 100, 420)
						role.DecRoleSilver(MapID, InstanceID, RoleID, 10000000, 101)
						role.AddRoleItem(MapID, InstanceID, RoleID, 1442204, 1, -1, 8, 420)
					break
					end
				end
			end
	elseif TalkIndex == 5 then	--ѡ���ڹ������ؼ�
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 4208)	-- "�ڹ������ؼ�"
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.AddMsgEvent(MsgID, 21, 10)	-- ѡ��1
			msg.AddMsgEvent(MsgID, 1, 4213)	-- "�һ��ؼ�������"
			msg.AddMsgEvent(MsgID, 21, 11)	-- ѡ��2
			msg.AddMsgEvent(MsgID, 1, 4214)	-- "�һ��ؼ�������"
			msg.AddMsgEvent(MsgID, 21, 12)	-- ѡ��3
			msg.AddMsgEvent(MsgID, 1, 4215)	-- "�һ��ؼ���ҽ��"
			msg.AddMsgEvent(MsgID, 21, 13)	-- ѡ��4
			msg.AddMsgEvent(MsgID, 1, 4216)	-- "�һ��ؼ�������"
			msg.AddMsgEvent(MsgID, 21, -1)	-- ѡ��5
			msg.AddMsgEvent(MsgID, 1, 4125)	-- "����"
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
	elseif TalkIndex == 10 then
			if mijijiance(MapID, InstanceID, TargetID, TargetTypeID, RoleID)  then
				for i = 1, 7 do
					local m = MijiID_5[i]
					local n = role.GetRoleItemNum(RoleID,m)
					if n ~= 0 then
						role.RemoveFromRole(MapID, InstanceID, RoleID, m, 1, 420)
						role.RemoveFromRole(MapID, InstanceID, RoleID, 2310170, 100, 420)
						role.DecRoleSilver(MapID, InstanceID, RoleID, 10000000, 101)
						role.AddRoleItem(MapID, InstanceID, RoleID, 1442205, 1, -1, 8, 420)
					break
					end
				end
			end
	elseif TalkIndex == 11 then
			if mijijiance(MapID, InstanceID, TargetID, TargetTypeID, RoleID)  then
				for i = 1, 7 do
					local m = MijiID_6[i]
					local n = role.GetRoleItemNum(RoleID,m)
					if n ~= 0 then
						role.RemoveFromRole(MapID, InstanceID, RoleID, m, 1, 420)
						role.RemoveFromRole(MapID, InstanceID, RoleID, 2310170, 100, 420)
						role.DecRoleSilver(MapID, InstanceID, RoleID, 10000000, 101)
						role.AddRoleItem(MapID, InstanceID, RoleID, 1442206, 1, -1, 8, 420)
					break
					end
				end
			end
	elseif TalkIndex == 12 then
			if mijijiance(MapID, InstanceID, TargetID, TargetTypeID, RoleID) then
				for i = 1, 7 do
					local m = MijiID_7[i]
					local n = role.GetRoleItemNum(RoleID,m)
					if n ~= 0 then
						role.RemoveFromRole(MapID, InstanceID, RoleID, m, 1, 420)
						role.RemoveFromRole(MapID, InstanceID, RoleID, 2310170, 100, 420)
						role.DecRoleSilver(MapID, InstanceID, RoleID, 10000000, 101)
						role.AddRoleItem(MapID, InstanceID, RoleID, 1442207, 1, -1, 8, 420)
					break
					end
				end
			end
	elseif TalkIndex == 13 then
			if mijijiance(MapID, InstanceID, TargetID, TargetTypeID, RoleID) then
				for i = 1, 7 do
					local m = MijiID_8[i]
					local n = role.GetRoleItemNum(RoleID,m)
					if n ~= 0 then
						role.RemoveFromRole(MapID, InstanceID, RoleID, m, 1, 420)
						role.RemoveFromRole(MapID, InstanceID, RoleID, 2310170, 100, 420)
						role.DecRoleSilver(MapID, InstanceID, RoleID, 10000000, 101)
						role.AddRoleItem(MapID, InstanceID, RoleID, 1442208, 1, -1, 8, 420)
					break
					end
				end
			end
	end
end


aux. RegisterCreatureEvent(4902006, 7, "mijishangren_On_Talk")























