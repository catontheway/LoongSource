--���ﺰ���ű�

-- �������ս������
function c1010529_OnEnterCombat(MapID, InstanceID, CreatureID)

	cre.MonsterSay(MapID, InstanceID, CreatureID, 50073)

end

aux.RegisterCreatureEvent(1010529, 2, "c1010529_OnEnterCombat")

function c1010529_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	--ˢ�¼�ħ̨
	local x, y, z = unit.GetPosition(MapID, InstanceID, TargetID)
	map.MapCreateCreature(MapID, InstanceID, 1531107, x, y, z)
	Increase_shili(MapID, InstanceID, RoleID, 1, 4, "jinjieshili")

	--��һ�ɱ���Թ��[1010529]���ŷ������㲥
	local MsgID = msg.BeginMsgEvent();					--ע��㲥��Ϣ���
	local r = math.random(1,100);	--ȡ�����

	msg.AddMsgEvent(MsgID,102,100026);
	msg.AddMsgEvent(MsgID,2,RoleID);
	if ( r<11 ) then
		msg.DispatchBroadcast(MsgID,-1,-1,-1);
	elseif ( r<61 ) then
		msg.DispatchBroadcast(MsgID,3017299407,-1,-1);
		msg.DispatchBroadcast(MsgID,MapID,InstanceID,-1);
	else
		msg.DispatchBroadcast(MsgID,MapID,InstanceID,-1);
	end

end
aux.RegisterCreatureEvent(1010529, 4, "c1010529_OnDie")
