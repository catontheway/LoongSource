function c1006507_OnDie(MapID, InstanceID, CreatureID)

	local Hanbing_Num = map.GetMapScriptData(MapID, InstanceID, 1, 3)
	map.SetMapScriptData(MapID,InstanceID,1,3,Hanbing_Num+1)

	if Hanbing_Num+1<10 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 1744)	--���ѻ�ɱxx������Ӫ�׶�����ɱ10���󺮱���������֣�
		msg.AddMsgEvent(MsgID, 9, Hanbing_Num+1)
		msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID)
	elseif Hanbing_Num+1==10 then
		map.MapCreateCreature(MapID, InstanceID, 1006556, 1541, 4346, 401)
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 101, 1745)	--�������Ѿ�����������[1541, 401]��
		msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID)
	end

end

aux.RegisterCreatureEvent(1006507, 4, "c1006507_OnDie")

