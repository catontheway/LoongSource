

--��õ�������Ի���õ����
function RoseTu_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	local seedowner = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 1)--��ֲ��ID
	local growtime = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 2)--�ѽ�ˮ����
	local HaveDelete_growthitem = 0
--1.5��������////////////////////////////////////////////////up,���в��Ϩ�����������
	local Role = {}
	local seedownerRole = {}
	local TeamID = role.IsRoleHaveTeam(MapID, InstanceID, RoleID)
	local seedownerTeamID = role.IsRoleHaveTeam(MapID, InstanceID, seedowner)
	local IsInOwnerTeam = 0
	if TeamID ~= 4294967295 then
		Role[1], Role[2], Role[3], Role[4], Role[5], Role[6] = role.GetRoleTeamMemID(TeamID)
		for k = 1,6 do
			if seedowner == Role[k] then
				IsInOwnerTeam = 1
			end
		end
	end
	if seedownerTeamID ~= 4294967295 then
		seedownerRole[1], seedownerRole[2], seedownerRole[3], seedownerRole[4], seedownerRole[5], seedownerRole[6] = role.GetRoleTeamMemID(seedownerTeamID)
	end
--1.5��������/////////////////////////////////////////////////down,���в��ϡ���������������
	local farmer_level = role.GetRoleLevel(MapID, InstanceID, RoleID)
	if farmer_level > 120 then
	    local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 1344)	-- "����120������ҽ��޷�������ֲ��"
		msg.AddMsgEvent(MsgID, 24, TargetID)
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	else
		if TalkIndex == -1 then
			if seedowner ~= 0 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 1333)	-- "###(�����)���ֵ�õ������,���ָոտ�ʼ��ѿ����ཽˮ�ɴٽ���������"
				msg.AddMsgEvent(MsgID, 2, seedowner)	--���������
				msg.AddMsgEvent(MsgID, 21, 4)	-- ѡ��һ
				msg.AddMsgEvent(MsgID, 1, 1302)	-- "�ｽˮ"
				msg.AddMsgEvent(MsgID, 21, 5)	-- ѡ���
				msg.AddMsgEvent(MsgID, 1, 1303)	-- "�뿪"
				msg.AddMsgEvent(MsgID, 24, TargetID)
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
				TalkIndex = -1
			else
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 1334)	-- "õ�������ոտ�ʼ��ѿ����ཽˮ�ɴٽ���������\n����û�����ˣ�ֻҪ������һ��ˮ��Ϊ����������"
				msg.AddMsgEvent(MsgID, 21, 4)	-- ѡ��һ
				msg.AddMsgEvent(MsgID, 1, 1302)	-- "�ｽˮ"
				msg.AddMsgEvent(MsgID, 21, 5)	-- ѡ���
				msg.AddMsgEvent(MsgID, 1, 1303)	-- "�뿪"
				msg.AddMsgEvent(MsgID, 24, TargetID)
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
				TalkIndex = -1
			end

		elseif TalkIndex == 4 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 1304)	-- "��ˮ��Ҫ����1�ݡ�����֮ˮ����1�ݡ�������ˮ�������ν�ˮ֮������1���ӣ�\n��������ˮ��Ϊ�ɼ�ʥˮ���书Ч����ͨ������֮ˮ����5����"
				msg.AddMsgEvent(MsgID, 21, 6)	-- ѡ��һ
				msg.AddMsgEvent(MsgID, 1, 1305)	-- "�ｽ������֮ˮ��"
				msg.AddMsgEvent(MsgID, 21, 7)	-- ѡ���
				msg.AddMsgEvent(MsgID, 1, 1306)	-- "�ｽ��������ˮ��"
				msg.AddMsgEvent(MsgID, 24, TargetID)
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
				TalkIndex = -1
				--1.5��������//////////////////////////////////////////////////////////////////////////////up�����в��Ϩ�����������
				local x, y, z = unit.GetPosition(MapID, InstanceID, TargetID)
				if seedowner ~= 0 and seedowner ~= RoleID and IsInOwnerTeam == 0 then
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 101, 1342) --"λ��XX����ͼ����[X,Z]��ֲ������[RoleID]���죬��ǰ��Ӧ�ԡ�"
					msg.AddMsgEvent(MsgID, 6, MapID)
					msg.AddMsgEvent(MsgID, 9, x)
					msg.AddMsgEvent(MsgID, 9, z)
					msg.AddMsgEvent(MsgID, 2, RoleID)
					msg.DispatchRoleMsgEvent(seedowner, MsgID)
					if seedownerTeamID ~= 4294967295 then
						for k in pairs(seedownerRole) do
							if seedownerRole[k] ~= 4294967295 then
								local MsgID = msg.BeginMsgEvent()
								msg.AddMsgEvent(MsgID, 101, 1342) --"λ��XX����ͼ����[X,Z]��ֲ������[RoleID]���죬��ǰ��Ӧ�ԡ�"
								msg.AddMsgEvent(MsgID, 6, MapID)
								msg.AddMsgEvent(MsgID, 9, x)
								msg.AddMsgEvent(MsgID, 9, z)
								msg.AddMsgEvent(MsgID, 2, RoleID)
								msg.DispatchRoleMsgEvent(seedownerRole[k], MsgID)
							end
						end
					end
				end
				--1.5��������///////////////////////////////////////////////////////////////////////////////down�����в��ϡ�������������
		elseif TalkIndex == 6 then
				local WaterNum = role.GetRoleItemNum(RoleID, 3303009)--����֮ˮ����
				if WaterNum > 0 then
					local cTime = os.time() --��ǰʱ��
					local lTime = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 3)
					local s = os.difftime(cTime,lTime) --�ϴν�ˮ������ʱ��

					if 	s > 60 then --�������1����
	--1.5��������////////////////////////////////////////////////////////////////////////////uuuuuuuupppppppppppppp���ͼ����Ϩ�����������

						if seedowner == 0 or seedowner == RoleID or IsInOwnerTeam == 1 then --�����������߻���Ϊ�������������ڶ�����
							role.RemoveFromRole(MapID, InstanceID, RoleID, 3303009, 1, 101)---------ɾ������֮ˮ
							growtime = growtime + 1	-------------------------��¼��������
							cre.SetCreatureScriptData(MapID, InstanceID, TargetID, 1, 3, cTime)-----------�߼����ϲ���¼
							HaveDelete_growthitem = 1
							if TeamID == 4294967295 then
								local rolelevel = role.GetRoleLevel(MapID, InstanceID, RoleID)
								local roleexp = 100*growth_exp[rolelevel].gaojipt-------------����ֲ���þ���
								role.AddRoleExp(MapID, InstanceID, RoleID, roleexp)
								unit.AddBuff(MapID, InstanceID, TargetID, 5003801, TargetID)
							else
								local aroundmember = 0
								for k = 1,6 do
									if 4294967295 ~= Role[k] then
										local Role_MapID, Role_InstanceID = role.GetRoleMapID(Role[k])
										if Role_MapID == MapID or Role_InstanceID == InstanceID then
											local x1, y1, z1 = unit.GetPosition(MapID, InstanceID, TargetID)
											local x2, y2, z2 = unit.GetPosition(Role_MapID, Role_InstanceID, Role[k])
											if math.abs(x1-x2)<200 and math.abs(z1-z2)<200 then
												aroundmember = aroundmember + 1
											end
										end
									end
								end
								for k = 1,6 do
									if 4294967295 ~= Role[k] then
										local Role_MapID, Role_InstanceID = role.GetRoleMapID(Role[k])
										if Role_MapID == MapID or Role_InstanceID == InstanceID then
											local x1, y1, z1 = unit.GetPosition(MapID, InstanceID, TargetID)
											local x2, y2, z2 = unit.GetPosition(Role_MapID, Role_InstanceID, Role[k])
											if math.abs(x1-x2)<200 and math.abs(z1-z2)<200 then
												local rolelevel = role.GetRoleLevel(Role_MapID, Role_InstanceID, Role[k])
												local roleexp = 100*growth_exp[rolelevel].gaojipt-------------����ֲ���þ���
												if aroundmember == 1 then
													roleexp = roleexp
												else
													roleexp = roleexp/aroundmember *1.2
												end
												role.AddRoleExp(MapID, InstanceID, Role[k], roleexp)
												unit.AddBuff(MapID, InstanceID, TargetID, 5003801, TargetID)
											end
										end
									end
								end
							end
						else --���������������߶���
							local MsgID = msg.BeginMsgEvent()
							msg.AddMsgEvent(MsgID, 20, 1341) --"��ֲ�ﲻ����������С�ӣ�����뿪������ģʽ�ſ��Խ��֣���׼�����ܸ��������ͷ���"
							msg.AddMsgEvent(MsgID, 21, 8)	-- ѡ��һ
							msg.AddMsgEvent(MsgID, 1, 21)	-- "ȷ��"
							msg.AddMsgEvent(MsgID, 21, 5)	-- ѡ��һ
							msg.AddMsgEvent(MsgID, 1, 1303)	-- "�뿪"
							msg.AddMsgEvent(MsgID, 24, TargetID)
							msg.DispatchRoleMsgEvent(RoleID, MsgID)
						end
	--1.5��������////////////////////////////////////////////////////////////////////dddddooooooowwwwwwnnnnnn���ͼ����ϡ���������
					else
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 20, 1307)-- "�Ը�ֲ������ν�ˮ�������1���ӣ�Ϊ��ֹƵ����ˮ��ֲ����ɵ��˺��������Ե�Ƭ��������ˮ��"
						msg.AddMsgEvent(MsgID, 21, 5)	-- ѡ��һ
						msg.AddMsgEvent(MsgID, 1, 1303)	-- "�뿪"
						msg.AddMsgEvent(MsgID, 24, TargetID)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
					end
				else

					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 20, 1308) --"��û������֮ˮ��\n����֮ˮ������ֲ���˴����й��򣬻�ͨ����ɴ�ũة���ġ������ɹ��������á�"
					msg.AddMsgEvent(MsgID, 21, 5)	-- ѡ��һ
					msg.AddMsgEvent(MsgID, 1, 1303)	-- "�뿪"
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
				end
				TalkIndex = -1
		elseif TalkIndex == 7 then
				local WaterNum = role.GetRoleItemNum(RoleID, 3303010)--������ˮ����
				if WaterNum > 0 then
					local cTime = os.time() --��ǰʱ��
					local lTime = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 3)
					local s = os.difftime(cTime,lTime) --�ϴν�ˮ������ʱ��

					if 	s > 60 then --�������1����
	--1.5��������////////////////////////////////////////////////////////////////////////////uuuuuuuupppppppppppppp���߼����Ϩ�����������
						if seedowner == 0 or seedowner == RoleID or IsInOwnerTeam == 1 then --�����������߻���Ϊ�������������ڶ�����
							role.RemoveFromRole(MapID, InstanceID, RoleID, 3303010, 1, 101)---------ɾ��������ˮ
							growtime = 5	-------------------------��¼��������
							HaveDelete_growthitem = 1
							if TeamID == 4294967295 then
								local rolelevel = role.GetRoleLevel(MapID, InstanceID, RoleID)
								local roleexp = 100*growth_exp[rolelevel].gaojiim-------------����ֲ���þ���
								role.AddRoleExp(MapID, InstanceID, RoleID, roleexp)
								unit.AddBuff(MapID, InstanceID, TargetID, 5003801, TargetID)
							else
								local aroundmember = 0
								for k = 1,6 do
									if 4294967295 ~= Role[k] then
										local Role_MapID, Role_InstanceID = role.GetRoleMapID(Role[k])
										if Role_MapID == MapID or Role_InstanceID == InstanceID then
											local x1, y1, z1 = unit.GetPosition(MapID, InstanceID, TargetID)
											local x2, y2, z2 = unit.GetPosition(Role_MapID, Role_InstanceID, Role[k])
											if math.abs(x1-x2)<200 and math.abs(z1-z2)<200 then
												aroundmember = aroundmember + 1
											end
										end
									end
								end
								for k = 1,6 do
									if 4294967295 ~= Role[k] then
										local Role_MapID, Role_InstanceID = role.GetRoleMapID(Role[k])
										if Role_MapID == MapID or Role_InstanceID == InstanceID then
											local x1, y1, z1 = unit.GetPosition(MapID, InstanceID, TargetID)
											local x2, y2, z2 = unit.GetPosition(Role_MapID, Role_InstanceID, Role[k])
											if math.abs(x1-x2)<200 and math.abs(z1-z2)<200 then
												local rolelevel = role.GetRoleLevel(Role_MapID, Role_InstanceID, Role[k])
												local roleexp = 100*growth_exp[rolelevel].gaojiim-------------����ֲ���þ���
												if aroundmember == 1 then
													roleexp = roleexp
												else
													roleexp = roleexp/aroundmember *1.2
												end
												role.AddRoleExp(MapID, InstanceID, Role[k], roleexp)
												unit.AddBuff(MapID, InstanceID, TargetID, 5003801, TargetID)
											end
										end
									end
								end
							end
						else --���������������߶���
							local MsgID = msg.BeginMsgEvent()
							msg.AddMsgEvent(MsgID, 20, 1341) --"��ֲ�ﲻ����������С�ӣ�����뿪������ģʽ�ſ��Խ��֣���׼�����ܸ��������ͷ���"
							msg.AddMsgEvent(MsgID, 21, 9)	-- ѡ��һ-------------------------------���ݲ�����˴��߼�9
							msg.AddMsgEvent(MsgID, 1, 21)	-- "ȷ��"
							msg.AddMsgEvent(MsgID, 21, 5)	-- ѡ��һ
							msg.AddMsgEvent(MsgID, 1, 1303)	-- "�뿪"
							msg.AddMsgEvent(MsgID, 24, TargetID)
							msg.DispatchRoleMsgEvent(RoleID, MsgID)
						end
	--1.5��������////////////////////////////////////////////////////////////////////dddddooooooowwwwwwnnnnnn���߼����ϡ���������
					else
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 20, 1307)	-- "�Ը�ֲ������ν�ˮ�������1���ӣ�Ϊ��ֹƵ����ˮ��ֲ����ɵ��˺��������Ե�Ƭ��������ˮ��"
						msg.AddMsgEvent(MsgID, 21, 5)	-- ѡ��һ
						msg.AddMsgEvent(MsgID, 1, 1303)	-- "�뿪"
						msg.AddMsgEvent(MsgID, 24, TargetID)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
					end
				else
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 20, 1309) --"��û��������ˮ��\n������ˮ�����ɼ�ʥˮ����������������˵�ڴ�ũة����ɡ������ɹ��������н�С���ʻ��������ˮ�����⿪����������ϻ�����´�������Ⱦ���һ�����ʻ��������ˮ��\n���������Գ���ʹ����ͨ����֮ˮ�Ը�ֲ����н�ˮ"
					msg.AddMsgEvent(MsgID, 21, 5)	-- ѡ��һ
					msg.AddMsgEvent(MsgID, 1, 1303)	-- "�뿪"
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
				end
				TalkIndex = -1
	--///////.///////////////////////////////////////////////////////////////////////////////////////1.5up,�ͼ����Ϩ�����������
		elseif TalkIndex == 8 then
			local Temp = 1
			if role.IsRoleInStatus(RoleID, 1048576) == false then --���뿪�����ɲſ���������
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 1341) --"��ֲ�ﲻ����������С�ӣ�����뿪������ģʽ�ſ��Խ��֣���׼�����ܸ��������ͷ���"
				msg.AddMsgEvent(MsgID, 21, 5)	-- ѡ��һ
				msg.AddMsgEvent(MsgID, 1, 1303)	-- "�뿪"
				msg.AddMsgEvent(MsgID, 24, TargetID)
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
				Temp = 0
			end
			if Temp == 1 then
				role.RemoveFromRole(MapID, InstanceID, RoleID, 3303009, 1, 101)--------------ɾ������֮ˮ
				growtime = growtime + 1-------------------------��¼��������
				local cTime = os.time() --��ǰʱ��
				local rolelevel = role.GetRoleLevel(MapID, InstanceID, RoleID)
				cre.SetCreatureScriptData(MapID, InstanceID, TargetID, 1, 3, cTime)------------�߼����ϲ���¼
				HaveDelete_growthitem = 1
				local roleexp = 100*growth_exp[rolelevel].gaojipt-----------------����ֲ�������
				role.AddRoleExp(MapID, InstanceID, RoleID, roleexp)
				unit.AddBuff(MapID, InstanceID, TargetID, 5003801, TargetID)
				unit.AddBuff(MapID, InstanceID, RoleID, 2025601, RoleID)
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 26, 1343) --"��ֲ�ﲻ���������Ķ��ѣ�������ܵ�������Ч����Ӱ�졣"
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			end
	--/////////////////////////////////////////////////////////////////////////////////////////////////1.5down���ͼ����ϡ���������
	--///////.///////////////////////////////////////////////////////////////////////////////////////1.5up,�߼����Ϩ�����������
		elseif TalkIndex == 9 then
			local Temp = 1
			if role.IsRoleInStatus(RoleID, 1048576) == false then --���뿪�����ɲſ���������
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 1341) --"��ֲ�ﲻ����������С�ӣ�����뿪������ģʽ�ſ��Խ��֣���׼�����ܸ��������ͷ���"
				msg.AddMsgEvent(MsgID, 21, 5)	-- ѡ��һ
				msg.AddMsgEvent(MsgID, 1, 1303)	-- "�뿪"
				msg.AddMsgEvent(MsgID, 24, TargetID)
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
				Temp = 0
			end
			if Temp == 1 then
				role.RemoveFromRole(MapID, InstanceID, RoleID, 3303010, 1, 101)--------------ɾ��������ˮ
				growtime = 5-------------------------��¼��������
				HaveDelete_growthitem = 1
				local rolelevel = role.GetRoleLevel(MapID, InstanceID, RoleID)
				local roleexp = 100*growth_exp[rolelevel].gaojiim-----------------����ֲ�������
				role.AddRoleExp(MapID, InstanceID, RoleID, roleexp)
				unit.AddBuff(MapID, InstanceID, TargetID, 5003801, TargetID)
				unit.AddBuff(MapID, InstanceID, RoleID, 2025601, RoleID)
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 26, 1343) --"��ֲ�ﲻ���������Ķ��ѣ�������ܵ�������Ч����Ӱ�졣"
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			end
	--/////////////////////////////////////////////////////////////////////////////////////////////////1.5down���߼����ϡ���������
		end
	end


	if 	HaveDelete_growthitem == 1 then
			--���ӽ���ʵ��ֵ
			Increase_shili(MapID, InstanceID, RoleID, 0, 1,"jinjieshili_A")

		if growtime == 5 then
			local x, y, z = unit.GetPosition(MapID, InstanceID, TargetID)
			local creatureID = map.MapCreateCreature(MapID, InstanceID, 4900415, x, y, z)
			if seedowner == 0 then
			seedowner = RoleID
			end
			cre.SetCreatureScriptData(MapID, InstanceID, creatureID, 1, 1, seedowner)--��¼��ֲ����õ����������ҵ�ID

			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 13, 0)
			msg.AddMsgEvent(MsgID, 1, 1321)--������ֲ�ﾭ��������ˮ��ʼ��׳�ɳ������������ջ��Ѿ���Զ����ϲ��ϲ��
			msg.DispatchRoleMsgEvent(seedowner, MsgID)

			map.MapDeleteCreature(MapID, InstanceID, TargetID)
		else
			if seedowner == 0 then
			cre.SetCreatureScriptData(MapID, InstanceID, TargetID, 1, 1, RoleID)
			end
			cre.SetCreatureScriptData(MapID, InstanceID, TargetID, 1, 2, growtime)--��¼�����ѵĽ�ˮ����
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 26, 1310)	-- "ʹ��ǰֲ�����������á�����֮ˮ������XXX�Σ�����ֱ��ʹ�á�������ˮ������һ�Ρ�"
			msg.AddMsgEvent(MsgID, 9, 5-growtime)	--���뻹�ɽ���Ĵ���
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end

	end
end

aux.RegisterCreatureEvent(4900414, 7, "RoseTu_OnTalk")






--��õ�������Ի���õ����
function RoseShu_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	local seedowner = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 1)--��ֲ��ID
	local growtime = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 2)--�ѽ�ˮ����
	local HaveDelete_growthitem = 0
--1.5��������////////////////////////////////////////////////up,���в��Ϩ�����������
	local Role = {}
	local seedownerRole = {}
	local TeamID = role.IsRoleHaveTeam(MapID, InstanceID, RoleID)
	local seedownerTeamID = role.IsRoleHaveTeam(MapID, InstanceID, seedowner)
	local IsInOwnerTeam = 0
	if TeamID ~= 4294967295 then
		Role[1], Role[2], Role[3], Role[4], Role[5], Role[6] = role.GetRoleTeamMemID(TeamID)
		for k = 1,6 do
			if seedowner == Role[k] then
				IsInOwnerTeam = 1
			end
		end
	end
	if seedownerTeamID ~= 4294967295 then
		seedownerRole[1], seedownerRole[2], seedownerRole[3], seedownerRole[4], seedownerRole[5], seedownerRole[6] = role.GetRoleTeamMemID(seedownerTeamID)
	end

--1.5��������/////////////////////////////////////////////////down,���в��ϡ���������������
	local farmer_level = role.GetRoleLevel(MapID, InstanceID, RoleID)
	if farmer_level > 120 then
	    local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 1344)	-- "����120������ҽ��޷�������ֲ��"
		msg.AddMsgEvent(MsgID, 24, TargetID)
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	else
		if TalkIndex == -1 then
			if seedowner ~= 0 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 1335)	-- "###(�����)���ֵ�###(ֲ����),���翪ʼ��׳�ɳ�������ʩ�ʱ�֤���֣�"
				msg.AddMsgEvent(MsgID, 2, seedowner)	--���������
				msg.AddMsgEvent(MsgID, 21, 4)	-- ѡ��һ
				msg.AddMsgEvent(MsgID, 1, 1312)	-- "��ʩ��"
				msg.AddMsgEvent(MsgID, 21, 5)	-- ѡ���
				msg.AddMsgEvent(MsgID, 1, 1303)	-- "�뿪"
				msg.AddMsgEvent(MsgID, 24, TargetID)
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
				TalkIndex = -1
			else
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 1336)	-- "õ�������ոտ�ʼ��׳�ɳ�������ʩ�ʱ�֤���֣�\n����û�����ˣ�ֻҪ������һ��ˮ��Ϊ����������"
				msg.AddMsgEvent(MsgID, 21, 4)	-- ѡ��һ
				msg.AddMsgEvent(MsgID, 1, 1312)	-- "��ʩ��"
				msg.AddMsgEvent(MsgID, 21, 5)	-- ѡ���
				msg.AddMsgEvent(MsgID, 1, 1303)	-- "�뿪"
				msg.AddMsgEvent(MsgID, 24, TargetID)
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
				TalkIndex = -1
			end

		elseif TalkIndex == 4 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 1313)-- "ʩ����Ҫ����1�ݡ�����ũ�ʡ���1�ݡ�������ʡ�������ʩ��֮������1���ӣ�\n��������ʡ���Ч����ͨ������ũ�ʡ���5����"
				msg.AddMsgEvent(MsgID, 21, 6)	-- ѡ��һ
				msg.AddMsgEvent(MsgID, 1, 1314)	-- "��ʩ������ũ�ʡ�"
				msg.AddMsgEvent(MsgID, 21, 7)	-- ѡ���
				msg.AddMsgEvent(MsgID, 1, 1315)	-- "��ʩ��������ʡ�"
				msg.AddMsgEvent(MsgID, 24, TargetID)
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
				TalkIndex = -1
				--1.5��������//////////////////////////////////////////////////////////////////////////////up�����в��Ϩ�����������
				local x, y, z = unit.GetPosition(MapID, InstanceID, TargetID)
				if seedowner ~= 0 and seedowner ~= RoleID and IsInOwnerTeam == 0 then
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 101, 1342) --"λ��XX����ͼ����[X,Z]��ֲ������[RoleID]���죬��ǰ��Ӧ�ԡ�"
					msg.AddMsgEvent(MsgID, 6, MapID)
					msg.AddMsgEvent(MsgID, 9, x)
					msg.AddMsgEvent(MsgID, 9, z)
					msg.AddMsgEvent(MsgID, 2, RoleID)
					msg.DispatchRoleMsgEvent(seedowner, MsgID)
					if seedownerTeamID ~= 4294967295 then
						for k in pairs(seedownerRole) do
							if seedownerRole[k] ~= 4294967295 then
								local MsgID = msg.BeginMsgEvent()
								msg.AddMsgEvent(MsgID, 101, 1342) --"λ��XX����ͼ����[X,Z]��ֲ������[RoleID]���죬��ǰ��Ӧ�ԡ�"
								msg.AddMsgEvent(MsgID, 6, MapID)
								msg.AddMsgEvent(MsgID, 9, x)
								msg.AddMsgEvent(MsgID, 9, z)
								msg.AddMsgEvent(MsgID, 2, RoleID)
								msg.DispatchRoleMsgEvent(seedownerRole[k], MsgID)
							end
						end
					end
				end
				--1.5��������///////////////////////////////////////////////////////////////////////////////down�����в��ϡ�������������
		elseif TalkIndex == 6 then
				local ManureNum = role.GetRoleItemNum(RoleID, 3303011)--����ũ������
				if ManureNum > 0 then
					local cTime = os.time() --��ǰʱ��
					local lTime = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 3)
					local s = os.difftime(cTime,lTime) --�ϴ�ʩ�ʾ�����ʱ��

					if 	s > 60 then --�������1����
	--1.5��������////////////////////////////////////////////////////////////////////////////uuuuuuuupppppppppppppp���ͼ����Ϩ�����������
						if seedowner == 0 or seedowner == RoleID or IsInOwnerTeam == 1 then --�����������߻���Ϊ�������������ڶ�����
							role.RemoveFromRole(MapID, InstanceID, RoleID, 3303011, 1, 101)---------ɾ����ͨ��
							growtime = growtime + 1	-------------------------��¼��������
							cre.SetCreatureScriptData(MapID, InstanceID, TargetID, 1, 3, cTime)-----------�߼����ϲ���¼ʱ��
							HaveDelete_growthitem = 1
							if TeamID == 4294967295 then
								local rolelevel = role.GetRoleLevel(MapID, InstanceID, RoleID)
								local roleexp = 100*growth_exp[rolelevel].gaojipt-------------����ֲ���þ���
								role.AddRoleExp(MapID, InstanceID, RoleID, roleexp)
								unit.AddBuff(MapID, InstanceID, TargetID, 5003901, TargetID)-----��ˮ����ʩ��
							else
								local aroundmember = 0
								for k = 1,6 do
									if 4294967295 ~= Role[k] then
										local Role_MapID, Role_InstanceID = role.GetRoleMapID(Role[k])
										if Role_MapID == MapID or Role_InstanceID == InstanceID then
											local x1, y1, z1 = unit.GetPosition(MapID, InstanceID, TargetID)
											local x2, y2, z2 = unit.GetPosition(Role_MapID, Role_InstanceID, Role[k])
											if math.abs(x1-x2)<200 and math.abs(z1-z2)<200 then
												aroundmember = aroundmember + 1
											end
										end
									end
								end
								for k = 1,6 do
									if 4294967295 ~= Role[k] then
										local Role_MapID, Role_InstanceID = role.GetRoleMapID(Role[k])
										if Role_MapID == MapID or Role_InstanceID == InstanceID then
											local x1, y1, z1 = unit.GetPosition(MapID, InstanceID, TargetID)
											local x2, y2, z2 = unit.GetPosition(Role_MapID, Role_InstanceID, Role[k])
											if math.abs(x1-x2)<200 and math.abs(z1-z2)<200 then
												local rolelevel = role.GetRoleLevel(Role_MapID, Role_InstanceID, Role[k])
												local roleexp = 100*growth_exp[rolelevel].gaojipt-------------����ֲ���þ���
												if aroundmember == 1 then
													roleexp = roleexp
												else
													roleexp = roleexp/aroundmember *1.2
												end
												role.AddRoleExp(MapID, InstanceID, Role[k], roleexp)
												unit.AddBuff(MapID, InstanceID, TargetID, 5003901, TargetID)-----��ˮ����ʩ��
											end
										end
									end
								end
							end
						else --���������������߶���
							local MsgID = msg.BeginMsgEvent()
							msg.AddMsgEvent(MsgID, 20, 1341) --"��ֲ�ﲻ����������С�ӣ�����뿪������ģʽ�ſ��Խ��֣���׼�����ܸ��������ͷ���"
							msg.AddMsgEvent(MsgID, 21, 8)	-- ѡ��һ
							msg.AddMsgEvent(MsgID, 1, 21)	-- "ȷ��"
							msg.AddMsgEvent(MsgID, 21, 5)	-- ѡ��һ
							msg.AddMsgEvent(MsgID, 1, 1303)	-- "�뿪"
							msg.AddMsgEvent(MsgID, 24, TargetID)
							msg.DispatchRoleMsgEvent(RoleID, MsgID)
						end
	--1.5��������////////////////////////////////////////////////////////////////////dddddooooooowwwwwwnnnnnn���ͼ����ϡ���������

					else
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 20, 1316)-- "�Ը�ֲ�������ʩ�ʼ������1���ӣ�Ϊ��ֹƵ��ʩ�ʶ�ֲ����ɵ��˺��������Ե�Ƭ������ʩ�ʡ�"
						msg.AddMsgEvent(MsgID, 21, 5)	-- ѡ��һ
						msg.AddMsgEvent(MsgID, 1, 1303)	-- "�뿪"
						msg.AddMsgEvent(MsgID, 24, TargetID)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
					end
				else

					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 20, 1317) --"��û������ũ�ʡ�\n����ũ�ʿ�����ֲ���˴����й��򣬻�ͨ����ɴ�ũة���ġ������ɹ��������á�"
					msg.AddMsgEvent(MsgID, 21, 5)	-- ѡ��һ
					msg.AddMsgEvent(MsgID, 1, 1303)	-- "�뿪"
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
				end
				TalkIndex = -1
		elseif TalkIndex == 7 then
				local ManureNum = role.GetRoleItemNum(RoleID, 3303012)--�����������
				if ManureNum > 0 then
					local cTime = os.time() --��ǰʱ��
					local lTime = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 3)
					local s = os.difftime(cTime,lTime) --�ϴ�ʩ�ʾ�����ʱ��

					if 	s > 60 then --�������1����
	--1.5��������////////////////////////////////////////////////////////////////////////////uuuuuuuupppppppppppppp���߼����Ϩ�����������
						if seedowner == 0 or seedowner == RoleID or IsInOwnerTeam == 1 then --�����������߻���Ϊ�������������ڶ�����
							role.RemoveFromRole(MapID, InstanceID, RoleID, 3303012, 1, 101)---------ɾ���߼���
							growtime = 5	-------------------------��¼��������
							HaveDelete_growthitem = 1
							if TeamID == 4294967295 then
								local rolelevel = role.GetRoleLevel(MapID, InstanceID, RoleID)
								local roleexp = 100*growth_exp[rolelevel].gaojiim-------------����ֲ���þ���
								role.AddRoleExp(MapID, InstanceID, RoleID, roleexp)
								unit.AddBuff(MapID, InstanceID, TargetID, 5003901, TargetID)-----��ˮ����ʩ��
							else
								local aroundmember = 0
								for k = 1,6 do
									if 4294967295 ~= Role[k] then
										local Role_MapID, Role_InstanceID = role.GetRoleMapID(Role[k])
										if Role_MapID == MapID or Role_InstanceID == InstanceID then
											local x1, y1, z1 = unit.GetPosition(MapID, InstanceID, TargetID)
											local x2, y2, z2 = unit.GetPosition(Role_MapID, Role_InstanceID, Role[k])
											if math.abs(x1-x2)<200 and math.abs(z1-z2)<200 then
												aroundmember = aroundmember + 1
											end
										end
									end
								end
								for k = 1,6 do
									if 4294967295 ~= Role[k] then
										local Role_MapID, Role_InstanceID = role.GetRoleMapID(Role[k])
										if Role_MapID == MapID or Role_InstanceID == InstanceID then
											local x1, y1, z1 = unit.GetPosition(MapID, InstanceID, TargetID)
											local x2, y2, z2 = unit.GetPosition(Role_MapID, Role_InstanceID, Role[k])
											if math.abs(x1-x2)<200 and math.abs(z1-z2)<200 then
												local rolelevel = role.GetRoleLevel(Role_MapID, Role_InstanceID, Role[k])
												local roleexp = 100*growth_exp[rolelevel].gaojiim-------------����ֲ���þ���
												if aroundmember == 1 then
													roleexp = roleexp
												else
													roleexp = roleexp/aroundmember *1.2
												end
												role.AddRoleExp(MapID, InstanceID, Role[k], roleexp)
												unit.AddBuff(MapID, InstanceID, TargetID, 5003901, TargetID)-----��ˮ����ʩ��
											end
										end
									end
								end
							end
						else --���������������߶���
							local MsgID = msg.BeginMsgEvent()
							msg.AddMsgEvent(MsgID, 20, 1341) --"��ֲ�ﲻ����������С�ӣ�����뿪������ģʽ�ſ��Խ��֣���׼�����ܸ��������ͷ���"
							msg.AddMsgEvent(MsgID, 21, 9)	-- ѡ��һ-------------------------------���ݲ�����˴��߼�9
							msg.AddMsgEvent(MsgID, 1, 21)	-- "ȷ��"
							msg.AddMsgEvent(MsgID, 21, 5)	-- ѡ��һ
							msg.AddMsgEvent(MsgID, 1, 1303)	-- "�뿪"
							msg.AddMsgEvent(MsgID, 24, TargetID)
							msg.DispatchRoleMsgEvent(RoleID, MsgID)
						end
	--1.5��������////////////////////////////////////////////////////////////////////dddddooooooowwwwwwnnnnnn���߼����ϡ���������
					else
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 20, 1316)-- "�Ը�ֲ�������ʩ�ʼ������1���ӣ�Ϊ��ֹƵ��ʩ�ʶ�ֲ����ɵ��˺��������Ե�Ƭ������ʩ�ʡ�"
						msg.AddMsgEvent(MsgID, 21, 5)	-- ѡ��һ
						msg.AddMsgEvent(MsgID, 1, 1303)	-- "�뿪"
						msg.AddMsgEvent(MsgID, 24, TargetID)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
					end
				else
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 20, 1318) --"��û��������ʡ�\n������������ɼ�ʥ���������������˵�ڴ�ũة����ɡ������ɹ��������н�С���ʻ��������ʣ����⿪����������ϻ�����´�������Ⱦ���һ�����ʻ�á�\n���������Գ���ʹ����ͨ����ũ�ʶԸ�ֲ�����ʩ�ʡ�"
					msg.AddMsgEvent(MsgID, 21, 5)	-- ѡ��һ
					msg.AddMsgEvent(MsgID, 1, 1303)	-- "�뿪"
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
				end
				TalkIndex = -1
	--///////.///////////////////////////////////////////////////////////////////////////////////////1.5up,�ͼ����Ϩ�����������
		elseif TalkIndex == 8 then
			local Temp = 1
			if role.IsRoleInStatus(RoleID, 1048576) == false then --���뿪�����ɲſ���������
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 1341) --"��ֲ�ﲻ����������С�ӣ�����뿪������ģʽ�ſ��Խ��֣���׼�����ܸ��������ͷ���"
				msg.AddMsgEvent(MsgID, 21, 5)	-- ѡ��һ
				msg.AddMsgEvent(MsgID, 1, 1303)	-- "�뿪"
				msg.AddMsgEvent(MsgID, 24, TargetID)
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
				Temp = 0
			end
			if Temp == 1 then
				role.RemoveFromRole(MapID, InstanceID, RoleID, 3303011, 1, 101)--------------ɾ���ͼ���
				growtime = growtime + 1-------------------------��¼��������
				local cTime = os.time() --��ǰʱ��
				local rolelevel = role.GetRoleLevel(MapID, InstanceID, RoleID)
				cre.SetCreatureScriptData(MapID, InstanceID, TargetID, 1, 3, cTime)------------�߼����ϲ���¼
				HaveDelete_growthitem = 1
				local roleexp = 100*growth_exp[rolelevel].gaojipt-----------------����ֲ�������
				role.AddRoleExp(MapID, InstanceID, RoleID, roleexp)
				unit.AddBuff(MapID, InstanceID, TargetID, 5003901, TargetID)-----��ˮ����ʩ��
				unit.AddBuff(MapID, InstanceID, RoleID, 2025601, RoleID)
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 26, 1343) --"��ֲ�ﲻ���������Ķ��ѣ�������ܵ�������Ч����Ӱ�졣"
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			end
	--/////////////////////////////////////////////////////////////////////////////////////////////////1.5down���ͼ����ϡ���������
	--///////.///////////////////////////////////////////////////////////////////////////////////////1.5up,�߼����Ϩ�����������
		elseif TalkIndex == 9 then
			local Temp = 1
			if role.IsRoleInStatus(RoleID, 1048576) == false then --���뿪�����ɲſ���������
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 1341) --"��ֲ�ﲻ����������С�ӣ�����뿪������ģʽ�ſ��Խ��֣���׼�����ܸ��������ͷ���"
				msg.AddMsgEvent(MsgID, 21, 5)	-- ѡ��һ
				msg.AddMsgEvent(MsgID, 1, 1303)	-- "�뿪"
				msg.AddMsgEvent(MsgID, 24, TargetID)
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
				Temp = 0
			end
			if Temp == 1 then
				role.RemoveFromRole(MapID, InstanceID, RoleID, 3303012, 1, 101)--------------ɾ���߼���
				growtime = 5-------------------------��¼��������
				HaveDelete_growthitem = 1
				local rolelevel = role.GetRoleLevel(MapID, InstanceID, RoleID)
				local roleexp = 100*growth_exp[rolelevel].gaojiim-----------------����ֲ�������
				role.AddRoleExp(MapID, InstanceID, RoleID, roleexp)
				unit.AddBuff(MapID, InstanceID, TargetID, 5003901, TargetID)-----��ˮ����ʩ��
				unit.AddBuff(MapID, InstanceID, RoleID, 2025601, RoleID)
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 26, 1343) --"��ֲ�ﲻ���������Ķ��ѣ�������ܵ�������Ч����Ӱ�졣"
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			end
	--/////////////////////////////////////////////////////////////////////////////////////////////////1.5down���߼����ϡ���������
		end
    end

	if 	HaveDelete_growthitem == 1 then
			--���ӽ���ʵ��ֵ
			Increase_shili(MapID, InstanceID, RoleID, 0, 1,"jinjieshili_A")

		if growtime == 5 then
			local i = math.random(1,200)--0.5%����
			if i ~= 99 then
				local x, y, z = unit.GetPosition(MapID, InstanceID, TargetID)
				local creatureID = map.MapCreateCreature(MapID, InstanceID, 4900416, x, y, z)
				if seedowner == 0 then
				seedowner = RoleID
				end
				cre.SetCreatureScriptData(MapID, InstanceID, creatureID, 1, 1, seedowner)--��¼��ҵ�ID

				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 13, 0)
				msg.AddMsgEvent(MsgID, 1, 1320)--������ֲ��ֲ���ڼ���ʩ�ʺ����ڿ����������������ǰ���ջ��Է����ή��
				msg.DispatchRoleMsgEvent(seedowner, MsgID)
			else
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 13, 0)
				msg.AddMsgEvent(MsgID, 1, 1339)--"���ڸ��ֹ��������⾪�Ѿ������ţ�һ�����񣬻�����ȥ�������ű���"
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
				local MsgID1 = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID1, 26, 1339)	-- "���ڸ��ֹ��������⾪�Ѿ������ţ�һ�����񣬻�����ȥ�������ű���"
				msg.DispatchRoleMsgEvent(RoleID, MsgID1)

				map.MapCreateCreature(3017298383, InstanceID, 1534111,854,10000,1836)

				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 100, 1340)--"<��ɫ��>�ڸ��ֹ���������ų��������ţ�����������֮�����������������ϣ����ᱦ����������м��ʵ��仢����ޣ����˽�ʱװ�������ߡ�"
				msg.AddMsgEvent(MsgID, 2, RoleID)	--���������
				msg.DispatchWorldMsgEvent(MsgID)
			end


			map.MapDeleteCreature(MapID, InstanceID, TargetID)

		else
			if seedowner == 0 then
			cre.SetCreatureScriptData(MapID, InstanceID, TargetID, 1, 1, RoleID)
			end
			cre.SetCreatureScriptData(MapID, InstanceID, TargetID, 1, 2, growtime)--��¼�����ѵĽ�ˮ����
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 26, 1319)-- "ʹ��ǰֲ���������á�����ũ�ʡ�ʩ��XXX�Σ�����ֱ��ʹ�á�������ʡ�ʩ��һ�Ρ�"
			msg.AddMsgEvent(MsgID, 9, 5-growtime)--���뻹�ɽ���Ĵ���
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end

	end
end

aux.RegisterCreatureEvent(4900415, 7, "RoseShu_OnTalk")

