--�Ի� ��è 3010006 �Ի����������[10005],û�г���[2610018]���������[2610018]
function n3010006_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	if TalkIndex == -1 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 269)	-- "��ֻè�Ķ��������ƺ���һ�ѳ��ӣ�Ҫ����"
		msg.AddMsgEvent(MsgID, 21, 4)	-- ȷ����ť
		msg.AddMsgEvent(MsgID, 1, 206)	-- "ȷ��"
		msg.AddMsgEvent(MsgID, 21, 5)	-- ȡ����ť
		msg.AddMsgEvent(MsgID, 1, 207)	-- "ȡ��"
		msg.AddMsgEvent(MsgID, 24, TargetID)
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	elseif TalkIndex == 4 then
		if role.IsRoleHaveQuest(MapID, InstanceID, RoleID, 10005) == true then
			if role.GetRoleItemNum(RoleID, 2610018) == 0 then
				role.AddRoleItem(MapID, InstanceID, RoleID, 2610018, 1, -1, 8, 420)
				TalkIndex = -1
			else
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 270)	-- "���Ѿ���һ�ѳ�����"

				msg.AddMsgEvent(MsgID, 24, TargetID)
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
				TalkIndex = -1

			end
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 271)	-- "�㱻��ֻè�ݺݵ�ץ��һ�£����۰���"

			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
			TalkIndex = -1

		end
	end

end
aux.RegisterCreatureEvent(3010006, 7, "n3010006_OnTalk")
