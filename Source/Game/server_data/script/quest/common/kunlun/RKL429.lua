--15429��3070736�Ի���ʧȥѩ���飨2615214��10�����ٻ����޵�������1009218(1234,10730,414)

function n3070736_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
   if TalkIndex == -1 then
		local MsgID = msg.BeginMsgEvent()
	    msg.AddMsgEvent(MsgID, 20, 518)	-- "������ѩ�����������޵���������"
		msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
	    msg.AddMsgEvent(MsgID, 21, 4)	-- ȷ����ť
	    msg.AddMsgEvent(MsgID, 1, 21)	-- "ȷ��"
	    msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
    elseif TalkIndex == 4 then
        local i = role.GetRoleItemNum(RoleID, 2615214)
        if i < 10 then
            local MsgID = msg.BeginMsgEvent()
            msg.AddMsgEvent(MsgID, 20, 519)	-- "��Ҫ10��ѩ��������������޵������ӡ�"
            msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
        else
            role.RemoveFromRole(MapID, InstanceID, RoleID, 2615214, 10, 420)
	    map.MapCreateCreature(MapID, InstanceID, 1009218, 1234, 10730, 414)
        end
    end
end

aux.RegisterCreatureEvent(3070736, 7, "n3070736_OnTalk")
