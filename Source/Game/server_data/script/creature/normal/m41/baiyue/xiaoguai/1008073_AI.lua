--����ɱ ������� 1008073 ��10%���ʴ�����������һ���ű���
--ˢ�� �Űħ˧ 1008086 ���꣺x 2412 y 26387 z 707 ������������ ������� 1008073 ��ʧ
--ˢ�� ����ħ˧ 1008087 ���꣺x 2817 y 26387 z 708 ������������ ������� 1008073 ��ʧ
--ˢ�� ���ħ˧ 1008088 ���꣺x 2795 y 26387 z 310 ������������ ������� 1008073 ��ʧ
--ˢ�� ̰����� 1008078 ���꣺x 2436 y 26387 z 355
--ˢ�� ̰����� 1008079 ���꣺x 2377 y 26387 z 741
--ˢ�� ̰����� 1008080 ���꣺x 2840 y 26387 z 298
--��1%���ʴ�������ű���
--ˢ�� ������ɷ��ħ�� 1008081 ���꣺x 2600 y 25797 z 447 ������������ ������� 1008073 ��ʧ

function c1008073_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	local k = math.random(1,100)
	local i = math.random(1,6)
	if k > 90 then
		if i == 1 then
			map.MapCreateCreature(MapID, InstanceID, 1008086, 2412, 26387, 707)
			--map.MapDeleteCreature(MapID, InstanceID, 1008073)
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 26, 1759)	--�Űħ˧�ѳ���������[2412, 707]��
			msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID)
		elseif i == 2 then
			map.MapCreateCreature(MapID, InstanceID, 1008087, 2817, 26387, 708)
			--map.MapDeleteCreature(MapID, InstanceID, 1008073)
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 26, 1760)	--����ħ˧�ѳ���������[2817, 708]��
			msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID)
		elseif i == 3 then
			map.MapCreateCreature(MapID, InstanceID, 1008088, 2795, 26387, 310)
			--map.MapDeleteCreature(MapID, InstanceID, 1008073)
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 26, 1761)	--���ħ˧�ѳ���������[2795, 310]��
			msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID)
		elseif i == 4 then
			map.MapCreateCreature(MapID, InstanceID, 1008078, 2436, 26387, 355)
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 26, 1762)	--̰������ѳ���������[2436, 355]��
			msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID)
		elseif i == 5 then
			map.MapCreateCreature(MapID, InstanceID, 1008079, 2377, 26387, 741)
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 26, 1763)	--̰������ѳ���������[2377, 741]��
			msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID)
		elseif i == 6 then
			map.MapCreateCreature(MapID, InstanceID, 1008080, 2840, 26387, 298)
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 26, 1764)	--̰������ѳ���������[2840, 298]��
			msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID)
		end
	elseif k == 1 then
		map.MapCreateCreature(MapID, InstanceID, 1008081, 2600, 25797, 447)
		--map.MapDeleteCreature(MapID, InstanceID, 1008073)
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 1765)	--������ɷ��ħ���ѳ���������[2600, 447]��
		msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID)
	end
end
aux.RegisterCreatureEvent(1008073, 4, "c1008073_OnDie")

