--�Ի� ��������3010248��������¸���0���򱻴��ͳ�����
function n3010248_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	if TalkIndex == -1 then
	    local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20,	11002)		--���Ƿ��ͳ�ȥ��
		msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
		msg.AddMsgEvent(MsgID, 21,	4)			--ȷ����ť
		msg.AddMsgEvent(MsgID, 1,	21)			--��ȷ����
		msg.AddMsgEvent(MsgID, 21,	5)			--ȡ����ť
		msg.AddMsgEvent(MsgID, 1,	22)			--��ȡ����
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	elseif TalkIndex == 4 then
	    local i=role.GetRoleAttValue(MapID, InstanceID, RoleID, 74)
	    if i >= 0 then
			role.RoleGotoNewMap(MapID, InstanceID, RoleID, 3017298127, 2687, 6296, 2655)
		else
		    local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20,	11003)		--������ֵ���㣬�������ȥ��
		    msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
		    msg.AddMsgEvent(MsgID, 21,	5)			--ȷ����ť
		    msg.AddMsgEvent(MsgID, 1,	21)			--��ȷ����
		    msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	end

end
aux.RegisterCreatureEvent(3010248, 7, "n3010248_OnTalk")
