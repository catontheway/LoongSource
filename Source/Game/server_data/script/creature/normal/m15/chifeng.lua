--������������
function m15_chifeng_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

	local ChiFeng_Num = map.GetMapScriptData(MapID,InstanceID,1,0)
	map.SetMapScriptData(MapID,InstanceID,1,0,ChiFeng_Num+1)

	if ChiFeng_Num+1== 20 then
		map.MapCreateCreature(MapID, InstanceID, 1004423, 248, 7392, 563)
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 101, 1701)	--��������Ѿ�����������[248, 563]��
		msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID)
	elseif ChiFeng_Num+1< 20 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 1702)	--���ѻ�ɱxx����粿�ڣ���ɱ20���������׽�����֣�
		msg.AddMsgEvent(MsgID, 9, ChiFeng_Num+1)
		msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID)
	end
end

aux.RegisterCreatureEvent(1004401, 4, "m15_chifeng_OnDie")
