--������������
function m15_canfeng_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

	local CanFeng_Num = map.GetMapScriptData(MapID,InstanceID,1,1)
	map.SetMapScriptData(MapID,InstanceID,1,1,CanFeng_Num+1)

	if CanFeng_Num+1== 40 then
		map.MapCreateCreature(MapID, InstanceID, 1004424, 527, 5789, 746)
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 101, 1703)	--�з������Ѿ�����������[248, 563]��
		msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID)
	elseif CanFeng_Num+1< 40 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 1704)	--���ѻ�ɱxx���з粿�ڣ���ɱ40����з����׽�����֣�
		msg.AddMsgEvent(MsgID, 9, CanFeng_Num+1)
		msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID)
	end
end

aux.RegisterCreatureEvent(1004404, 4, "m15_canfeng_OnDie")
aux.RegisterCreatureEvent(1004405, 4, "m15_canfeng_OnDie")
aux.RegisterCreatureEvent(1004406, 4, "m15_canfeng_OnDie")

