function shituzige_OnComplete(MapID, InstanceID, QuestID, RoleID, NPCID)

	role.RoleBecomeMaster(MapID, InstanceID, RoleID)

	--���ŷ������㲥
	local MsgID = msg.BeginMsgEvent();
	msg.AddMsgEvent(MsgID,102,100027);
	msg.AddMsgEvent(MsgID,2,RoleID);
	msg.DispatchBroadcast(MsgID,-1,-1,-1);
end

--ע��
aux.RegisterQuestEvent(8001, 1, "shituzige_OnComplete")
