--����ɱ ̰����� 1008078 ˢ�� ̰�� 1008074 ���꣺x 2819 y 26387 z 684 ������������ ������� 1008073 ��ʧ
function c1008078_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	map.MapCreateCreature(MapID, InstanceID, 1008074, 2819, 26387, 684)
	--map.MapDeleteCreature(MapID, InstanceID, 1008073)
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 26, 1766)	--̰���ѳ���������[2819, 684]��
	msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID)
end
aux.RegisterCreatureEvent(1008078, 4, "c1008078_OnDie")
