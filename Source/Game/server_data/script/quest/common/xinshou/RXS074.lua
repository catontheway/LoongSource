

--�������ʱ
function q074_OnComplete(mapID, instanceID, questID, ownerID, enderID)
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 26, 2207)
	msg.DispatchRoleMsgEvent(ownerID, MsgID)

	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 13, 0)
	msg.AddMsgEvent(MsgID, 1, 2207)
	msg.DispatchRoleMsgEvent(ownerID, MsgID)
end

--ע��
aux.RegisterQuestEvent(74, 1, "q074_OnComplete")

