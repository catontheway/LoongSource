

--[[�������ʱ
function q10001_OnComplete(mapID, instanceID, questID, ownerID, enderID)
	role.OpenFramePage(ownerID, 2)
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 26, 2212)
	msg.DispatchRoleMsgEvent(ownerID, MsgID)

	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 13, 0)
	msg.AddMsgEvent(MsgID, 1, 2212)
	msg.DispatchRoleMsgEvent(ownerID, MsgID)
end

--ע��
aux.RegisterQuestEvent(10001, 1, "q10001_OnComplete")]]

