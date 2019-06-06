--NPC�����������˽ű�

function JCXL_chuanru(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	local curhour = tonumber(os.date("%H"))

	if TalkIndex == -1 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20,	87)		--���Ƿ���븱����
		msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
		msg.AddMsgEvent(MsgID, 21,	4)			--ȷ����ť
		msg.AddMsgEvent(MsgID, 1,	80)			--��ȷ����
		msg.AddMsgEvent(MsgID, 21,	5)			--ȡ����ť
		msg.AddMsgEvent(MsgID, 1,	81)			--��ȡ����
		msg.DispatchRoleMsgEvent(RoleID, MsgID)

	elseif TalkIndex == 4 then
		local Role_GuildID = guild.GetRoleGuildID(RoleID)

		local Master_GuilID = city.GetCityAppointedAtt(3, 2)

		if Role_GuildID==4294967295 or Role_GuildID~=Master_GuilID then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 	88)	--�����Ǳ���ռ����ɳ�Ա���޷����������������
			msg.AddMsgEvent(MsgID, 21, 	5)		--ȡ����ť
			msg.AddMsgEvent(MsgID, 1, 	22)		--ȡ��
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
			if curhour<15 or curhour>=18 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 	89)	--��������������ʱ��δ������15:00-18:00֮��������
				msg.AddMsgEvent(MsgID, 21, 	5)		--ȡ����ť
				msg.AddMsgEvent(MsgID, 1, 	22)		--ȡ��
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			else

				local Cur_D = tonumber(os.date("%j"))
				local Last_JCXL_Day = role.GetRoleScriptData(RoleID, 1, RoleDataType["Last_JCXL_Day"])

				if Cur_D~=Last_JCXL_Day then
					role.RoleGotoNewMap(MapID, InstanceID, RoleID, 1356752602, 175, 3, 87)
					role.SetRoleScriptData(RoleID, 1, RoleDataType["Last_JCXL_Day"], Cur_D)
				else
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 20, 	90)	--�������Ѿ��������������������ˣ��޷��ٴν��룡
					msg.AddMsgEvent(MsgID, 21, 	5)		--ȡ����ť
					msg.AddMsgEvent(MsgID, 1, 	22)		--ȡ��
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
				end
			end
		end

	end
end

function JCXL_chuanchu(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)

	if TalkIndex == -1 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20,	79)		--���Ƿ���븱����
		msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
		msg.AddMsgEvent(MsgID, 21,	4)			--ȷ����ť
		msg.AddMsgEvent(MsgID, 1,	84)			--��ȷ����
		msg.AddMsgEvent(MsgID, 21,	5)			--ȡ����ť
		msg.AddMsgEvent(MsgID, 1,	85)			--��ȡ����
		msg.DispatchRoleMsgEvent(RoleID, MsgID)

	elseif TalkIndex == 4 then
		role.RoleGotoNewMap(MapID, InstanceID, RoleID, 3695619387, 800, 10350, 590)

	end
end

-- ע��
aux.RegisterCreatureEvent(4500701, 7, "JCXL_chuanchu")
aux.RegisterCreatureEvent(4500702, 7, "JCXL_chuanru")
