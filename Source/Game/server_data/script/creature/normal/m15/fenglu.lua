--������������
function m15_fenglu_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

	local Fenglu_Num = map.GetMapScriptData(MapID,InstanceID,1,2)
	map.SetMapScriptData(MapID,InstanceID,1,2,Fenglu_Num+1)

	if Fenglu_Num+1== 40 then
		map.MapCreateCreature(MapID, InstanceID, 1004425, 735, 7248, 630)
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 101, 1705)	--��¹�Գ��Ѿ�����������[735, 630]��
		msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID)
	elseif Fenglu_Num+1< 40 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 1706)	--���ѻ�ɱxx����¹���ڣ���ɱ40�����¹�Գ�������֣�
		msg.AddMsgEvent(MsgID, 9, Fenglu_Num+1)
		msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID)
	end
end

aux.RegisterCreatureEvent(1004416, 4, "m15_fenglu_OnDie")
aux.RegisterCreatureEvent(1004417, 4, "m15_fenglu_OnDie")

