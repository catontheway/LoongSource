--NPC�����������˽ű�

function JiaMianRen(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)

	if TalkIndex == -1 then

		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20,	3029)		--����������η�������𣿹�Ȼ��Ӣ���ˬ�����к��ܡ��ҽ����������֮�ƺż�ս�ۣ�����������û�������֮������
		msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
		msg.AddMsgEvent(MsgID, 21,	4)			--ȷ����ť
		msg.AddMsgEvent(MsgID, 1,	3030)			--����ȡ�����ƺż�ս�ۡ�
		msg.AddMsgEvent(MsgID, 21,	5)			--ȡ����ť
		msg.AddMsgEvent(MsgID, 1,	22)			--��������
		msg.DispatchRoleMsgEvent(RoleID, MsgID)

	elseif TalkIndex == 4 then
		local Role_GuildID = guild.GetRoleGuildID(RoleID)
		local Master_GuilID = city.GetCityAppointedAtt(3, 2)

		local Role_ZhiWei = guild.GetRoleGuildPos(Role_GuildID, RoleID)

		if Role_GuildID==4294967295 or Role_GuildID~=Master_GuilID or Role_ZhiWei~= 2 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 	3031)		--ֻ���������츮ռ����ɵİ������Ƿ������������ƭ���ҵ��۾���
			msg.AddMsgEvent(MsgID, 21, 	5)		--ȡ����ť
			msg.AddMsgEvent(MsgID, 1, 	22)		--ȡ��
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else

			local FreeSize = role.GetBagFreeSize(RoleID)

			if FreeSize<6 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 	3032)		--��ȡ����ս����Ҫ6���������ҿո������������ٴ���ȡ��
				msg.AddMsgEvent(MsgID, 21, 	5)		--ȡ����ť
				msg.AddMsgEvent(MsgID, 1, 	22)		--ȡ��
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			else
				role.SigTitleEvent(RoleID, 262)

				local role_sex = role.GetRoleSex(RoleID)
				if role_sex == 0 then
					role.AddRoleItem(MapID, InstanceID, RoleID, 9140307, 1, 4, 0, 380)
					role.AddRoleItem(MapID, InstanceID, RoleID, 9140308, 1, 4, 0, 380)
					role.AddRoleItem(MapID, InstanceID, RoleID, 9140309, 1, 4, 0, 380)
					role.AddRoleItem(MapID, InstanceID, RoleID, 9140310, 1, 4, 0, 380)
					role.AddRoleItem(MapID, InstanceID, RoleID, 9140311, 1, 4, 0, 380)
					role.AddRoleItem(MapID, InstanceID, RoleID, 9140312, 1, 4, 0, 380)
				elseif role_sex == 1 then
					role.AddRoleItem(MapID, InstanceID, RoleID, 9140301, 1, 4, 0, 380)
					role.AddRoleItem(MapID, InstanceID, RoleID, 9140302, 1, 4, 0, 380)
					role.AddRoleItem(MapID, InstanceID, RoleID, 9140303, 1, 4, 0, 380)
					role.AddRoleItem(MapID, InstanceID, RoleID, 9140304, 1, 4, 0, 380)
					role.AddRoleItem(MapID, InstanceID, RoleID, 9140305, 1, 4, 0, 380)
					role.AddRoleItem(MapID, InstanceID, RoleID, 9140306, 1, 4, 0, 380)
				end

				map.MapDeleteCreature(MapID, InstanceID, TargetID)

				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 101,	3033)		--XX���ɵ�XX�Ѿ���ʽ��ΪΪ���η����������������֮����
				msg.AddMsgEvent(MsgID, 19, Role_GuildID)
				msg.AddMsgEvent(MsgID, 2, RoleID)
				msg.DispatchWorldMsgEvent(MsgID)
			end
		end

	end
end

aux.RegisterCreatureEvent(4500705, 7, "JiaMianRen")
