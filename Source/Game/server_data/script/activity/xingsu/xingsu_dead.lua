--�̶�����������·�


--��й��������¼�
function xingsu_Dead(MapID, InstanceID, CreatureID, TypeID, KillerID)
	--���Killer�Ƿ���С��
	local TeamID = role.IsRoleHaveTeam(MapID, InstanceID, KillerID)
			--���ӽ���ʵ��ֵ
			Increase_shili(MapID, InstanceID, KillerID, 1, 20, "jinjieshili_A")
	--���ù���Ϊ����״̬
	act1_Creature[TypeID].IsDead = true

	--���û��С��
	if 4294967295 == TeamID  then
		--����������
		local BuffID = act1_Creature[TypeID].BuffID
		if -1 ~= BuffID	 then
			-- �����һ������buff
			unit.AddBuff(MapID, InstanceID, KillerID, BuffID, KillerID)
		end

	else	-- ��������С��
		--���С�����ID
		local Role = {}
		Role[1], Role[2], Role[3], Role[4], Role[5], Role[6] = role.GetRoleTeamMemID(TeamID)

		for k in pairs(Role) do
			if 4294967295 ~= Role[k] then
				--����������
				local BuffID = act1_Creature[TypeID].BuffID
				if -1 ~= BuffID	 then
					-- �����һ������buff
					unit.AddBuff(MapID, InstanceID, Role[k], BuffID, Role[k])
				end
			end
		end
	end

	--�������㲥��###�ɹ���ս����###��
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 13, 1)
	msg.AddMsgEvent(MsgID, 1, 1)
	msg.AddMsgEvent(MsgID, 2, KillerID)
	msg.AddMsgEvent(MsgID, 5, TypeID)
	msg.DispatchWorldMsgEvent(MsgID)

	--���������·�����
	Rate = math.random(1, 100)
	if Rate < 30 then
		--���һ��ˢ�ֵ�
		local randPoint = math.random(500, 520)
		local x = map_list[MapID].map[randPoint].x
		local y = map_list[MapID].map[randPoint].y
		local z = map_list[MapID].map[randPoint].z

		--ˢ������
		map.MapCreateCreature(MapID, -1, 1500000, x, y, z)
		--ȫ���㲥�������·�������������ѣ�������###�� ��ҸϽ�ȥץ����
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 101, 3)
		msg.AddMsgEvent(MsgID, 6, MapID)
		msg.AddMsgEvent(MsgID, 9, x)
		msg.AddMsgEvent(MsgID, 9, z)
		msg.DispatchWorldMsgEvent(MsgID)
	end
end

--ע����������¼�
aux.RegisterCreatureEvent(1500001, 4, "xingsu_Dead")
aux.RegisterCreatureEvent(1500002, 4, "xingsu_Dead")
aux.RegisterCreatureEvent(1500003, 4, "xingsu_Dead")
aux.RegisterCreatureEvent(1500004, 4, "xingsu_Dead")
aux.RegisterCreatureEvent(1500005, 4, "xingsu_Dead")
aux.RegisterCreatureEvent(1500006, 4, "xingsu_Dead")
aux.RegisterCreatureEvent(1500007, 4, "xingsu_Dead")
aux.RegisterCreatureEvent(1500008, 4, "xingsu_Dead")
aux.RegisterCreatureEvent(1500009, 4, "xingsu_Dead")
aux.RegisterCreatureEvent(1500010, 4, "xingsu_Dead")
aux.RegisterCreatureEvent(1500011, 4, "xingsu_Dead")
aux.RegisterCreatureEvent(1500012, 4, "xingsu_Dead")
aux.RegisterCreatureEvent(1500013, 4, "xingsu_Dead")
aux.RegisterCreatureEvent(1500014, 4, "xingsu_Dead")
aux.RegisterCreatureEvent(1500015, 4, "xingsu_Dead")
aux.RegisterCreatureEvent(1500016, 4, "xingsu_Dead")
aux.RegisterCreatureEvent(1500017, 4, "xingsu_Dead")
aux.RegisterCreatureEvent(1500018, 4, "xingsu_Dead")
aux.RegisterCreatureEvent(1500019, 4, "xingsu_Dead")
aux.RegisterCreatureEvent(1500020, 4, "xingsu_Dead")
aux.RegisterCreatureEvent(1500021, 4, "xingsu_Dead")
aux.RegisterCreatureEvent(1500022, 4, "xingsu_Dead")
aux.RegisterCreatureEvent(1500023, 4, "xingsu_Dead")
aux.RegisterCreatureEvent(1500024, 4, "xingsu_Dead")
aux.RegisterCreatureEvent(1500025, 4, "xingsu_Dead")
aux.RegisterCreatureEvent(1500026, 4, "xingsu_Dead")
aux.RegisterCreatureEvent(1500027, 4, "xingsu_Dead")
aux.RegisterCreatureEvent(1500028, 4, "xingsu_Dead")

