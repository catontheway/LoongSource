--15430��3070737�Ի���ʧȥ�����飨2615215��10�����ٻ����޵Ķ�����1009217(961,12571,351)

function n3070737_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
   if TalkIndex == -1 then
		local MsgID = msg.BeginMsgEvent()
	    msg.AddMsgEvent(MsgID, 20, 520)	-- "�����ñ������������޵Ķ�������"
		msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
	    msg.AddMsgEvent(MsgID, 21, 4)	-- ȷ����ť
	    msg.AddMsgEvent(MsgID, 1, 21)	-- "ȷ��"
	    msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
    elseif TalkIndex == 4 then
        local i = role.GetRoleItemNum(RoleID, 2615215)
        if i < 10 then
            local MsgID = msg.BeginMsgEvent()
            msg.AddMsgEvent(MsgID, 20, 521)	-- "��Ҫ10������������������޵Ķ����ӡ�"
            msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
        else
            role.RemoveFromRole(MapID, InstanceID, RoleID, 2615215, 10, 420)
	    map.MapCreateCreature(MapID, InstanceID, 1009217, 961, 12571, 351)
        end
    end
end

aux.RegisterCreatureEvent(3070737, 7, "n3070737_OnTalk")
