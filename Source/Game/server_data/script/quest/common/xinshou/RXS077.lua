

--�������ʱ
function q077_OnComplete(mapID, instanceID, questID, ownerID, enderID)
	role.OpenFramePage(ownerID, 2)
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 26, 2208)
	msg.DispatchRoleMsgEvent(ownerID, MsgID)

	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 13, 0)
	msg.AddMsgEvent(MsgID, 1, 2208)
	msg.DispatchRoleMsgEvent(ownerID, MsgID)
end

--ע��
aux.RegisterQuestEvent(77, 1, "q077_OnComplete")

