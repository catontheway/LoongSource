--15431��3070738�Ի���ʧȥ�����飨2615216��10�����ٻ����޵Ĵ����1009215(1070,12740,258)

function n3070738_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
   if TalkIndex == -1 then
		local MsgID = msg.BeginMsgEvent()
	    msg.AddMsgEvent(MsgID, 20, 522)	-- "�����ú������������޵Ĵ������"
		msg.AddMsgEvent(MsgID, 24, TargetID) --npcid
	    msg.AddMsgEvent(MsgID, 21, 4)	-- ȷ����ť
	    msg.AddMsgEvent(MsgID, 1, 21)	-- "ȷ��"
	    msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
    elseif TalkIndex == 4 then
        local i = role.GetRoleItemNum(RoleID, 2615216)
        if i < 10 then
            local MsgID = msg.BeginMsgEvent()
            msg.AddMsgEvent(MsgID, 20, 523)	-- "��Ҫ10������������������޵Ĵ���ӡ�"
            msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
        else
            role.RemoveFromRole(MapID, InstanceID, RoleID, 2615216, 10, 420)
	    map.MapCreateCreature(MapID, InstanceID, 1009215, 1070, 12740, 258)
        end
    end
end

aux.RegisterCreatureEvent(3070738, 7, "n3070738_OnTalk")
