function I2616400_OnUse(MapID, InstanceID, TypeID, TargetID)

	local x,y,z = unit.GetPosition(MapID, InstanceID,TargetID)
	local MsgID = msg.BeginMsgEvent()		-- XXX�����˷���������Ϊ����������Xλ��������ң�
	msg.AddMsgEvent(MsgID, 13, 0)
	msg.AddMsgEvent(MsgID, 1, 2730)
	msg.AddMsgEvent(MsgID, 9, x)
	msg.AddMsgEvent(MsgID, 9, y)
	msg.AddMsgEvent(MsgID, 9, z)
	msg.DispatchWorldMsgEvent(MsgID)

end

aux.RegisterItemEvent(2616400, 1, "I2616400_OnUse")

