

--�������ʱ
function q005_OnComplete(mapID, instanceID, questID, ownerID, enderID)
	role.OpenFramePage(ownerID, 1)
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 26, 2206)
	msg.DispatchRoleMsgEvent(ownerID, MsgID)

	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 13, 0)
	msg.AddMsgEvent(MsgID, 1, 2206)
	msg.DispatchRoleMsgEvent(ownerID, MsgID)
end

--ע��
aux.RegisterQuestEvent(5, 1, "q005_OnComplete")

