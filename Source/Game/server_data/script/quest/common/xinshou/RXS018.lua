--�����ݺầ�˻ģ�18
--����1����ȡ�������BUFFԪʼ����[2010201]

--�����ȡʱ
function x018_OnAccept(mapID, instanceID, questID, ownerID, accepterID)
	unit.AddBuff(mapID, instanceID, ownerID, 2010201, ownerID)
end



--ע��
aux.RegisterQuestEvent(18, 0, "x018_OnAccept")

--�������ʱ
function q018_OnComplete(mapID, instanceID, questID, ownerID, enderID)
	role.OpenFramePage(ownerID, 2)
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 26, 2210)
	msg.DispatchRoleMsgEvent(ownerID, MsgID)

	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 13, 0)
	msg.AddMsgEvent(MsgID, 1, 2210)
	msg.DispatchRoleMsgEvent(ownerID, MsgID)
end

--ע��
aux.RegisterQuestEvent(18, 1, "q018_OnComplete")
