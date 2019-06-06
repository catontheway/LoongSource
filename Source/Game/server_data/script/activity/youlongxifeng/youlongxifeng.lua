YouLongXiFeng_Boss = 0
act26_TimerMin = 0
Longfeng_Times = 0
Longfeng_Xiuqiu = 0

function act26_OnStart(actID)
	--��������¹ȫ����֪ͨ[��Ļ��������������ʾ]����***��
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 100, 120)
	msg.DispatchMapMsgEvent(MsgID, 3017299663, -1)

	--���þ۱���ˢ�±�־λ
	YouLongXiFeng_Boss = 0

	--���ñ���������ɴ���
	Longfeng_Times = 0
	act26_TimerMin = 0
	Longfeng_Xiuqiu = 1

	--ˢ�����ɺͷ���
	map.MapCreateCreature(3017299663, -1, 3020601, 2012, 18346, 2694)
	map.MapCreateCreature(3017299663, -1, 3020616, 2008, 18346, 2690)
	map.MapCreateCreature(3017299663, -1, 3020616, 2008, 18346, 2698)
	map.MapCreateCreature(3017299663, -1, 3020616, 2016, 18346, 2690)
	map.MapCreateCreature(3017299663, -1, 3020616, 2016, 18346, 2698)

	map.MapCreateCreature(3017299663, -1, 3020602, 2193, 18346, 2695)
	map.MapCreateCreature(3017299663, -1, 3020615, 2189, 18346, 2691)
	map.MapCreateCreature(3017299663, -1, 3020615, 2189, 18346, 2699)
	map.MapCreateCreature(3017299663, -1, 3020615, 2197, 18346, 2691)
	map.MapCreateCreature(3017299663, -1, 3020615, 2197, 18346, 2699)

end

function act26_OnTimerMin(actID)

	act26_TimerMin = act26_TimerMin +1

end


--ע��û������¼�
aux.RegisterActEvent(28, 2, "act26_OnStart")
aux.RegisterActEvent(28, 4, "act26_OnTimerMin")
aux.RegisterActEvent(29, 2, "act26_OnStart")
aux.RegisterActEvent(29, 4, "act26_OnTimerMin")
aux.RegisterActEvent(30, 2, "act26_OnStart")
aux.RegisterActEvent(30, 4, "act26_OnTimerMin")
aux.RegisterActEvent(31, 2, "act26_OnStart")
aux.RegisterActEvent(31, 4, "act26_OnTimerMin")


--����乱�׼ν��˶Ի���������
function x3020612_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)

	local Cont_val = role.GetRoleScriptData(RoleID, 1, RoleDataType["LongFeng_Cont"])
	local Mark_val = role.GetRoleScriptData(RoleID, 1, RoleDataType["LongFeng_Mark"])
	local exp_val = 5000 * math.pow(Cont_val, 0.5)

	if Mark_val == 1 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 	123)		--���������Ѿ��һ������顱
		msg.AddMsgEvent(MsgID, 21, 	5)			--ȡ����ť
		msg.AddMsgEvent(MsgID, 1, 	22)			--��ȡ����
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	else

		if TalkIndex == -1 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20,	122)		--�����Ĺ���ֵΪ###���ܻ�ȡ###�㾭�顣������ֵ����150ʱ��ͬʱ�������ָ�ȫ������
			msg.AddMsgEvent(MsgID, 9,	Cont_val)	--����ֵ
			msg.AddMsgEvent(MsgID, 9,	exp_val)	--����ֵ
			msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
			msg.AddMsgEvent(MsgID, 21,	4)			--ȷ����ť
			msg.AddMsgEvent(MsgID, 1,	21)			--��ȷ����
			msg.AddMsgEvent(MsgID, 21,	5)			--ȡ����ť
			msg.AddMsgEvent(MsgID, 1,	22)			--��ȡ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)

		elseif TalkIndex == 4 then
			role.AddRoleExp(MapID, InstanceID, RoleID, exp_val)
			if Cont_val>= 150 then
				unit.AddBuff(MapID, InstanceID, RoleID, 2016701, RoleID)
			end
			role.SetRoleScriptData(RoleID, 1, RoleDataType["LongFeng_Mark"], 1)
		end
	end
end
--ע����乱�׼ν��˶Ի��¼�
aux.RegisterCreatureEvent(3020612, 7, "x3020612_OnTalk")



--�������Ŀ����������¼�
function x1521001_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	local k = math.random(100)

	if k<= 15 then
		local x, y, z = unit.GetPosition(MapID, InstanceID, TargetID)
		map.MapCreateCreature(3017299663, -1, 1521005, x+1, y, z+1)
		map.MapCreateCreature(3017299663, -1, 1521005, x+2, y, z)
		map.MapCreateCreature(3017299663, -1, 1521005, x, y, z+2)
		map.MapCreateCreature(3017299663, -1, 1521005, x, y, z-2)
		map.MapCreateCreature(3017299663, -1, 1521005, x-1, y, z-1)
		map.MapCreateCreature(3017299663, -1, 1521005, x-2, y, z)
	elseif k<= 20 then
		local CreatureID = map.MapCreateCreature(3017299663, -1, 3020617, 2112, 17850, 3150)
		unit.AddBuff(MapID, InstanceID, CreatureID, 5001401+math.random(3)*200, CreatureID)
	elseif k<= 25 then
		local CreatureID = map.MapCreateCreature(3017299663, -1, 3020618, 2104, 17850, 3150)
		unit.AddBuff(MapID, InstanceID, CreatureID, 5002001+math.random(3)*200, CreatureID)
	elseif k>98 then
		if YouLongXiFeng_Boss == 0 then
			map.MapCreateCreature(3017299663, -1, 3020607, 2108, 17850, 3147)
			YouLongXiFeng_Boss = 1
			--��������¹ȫ����֪ͨ[��Ļ��������������ʾ]����***��
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 100, 121)
			msg.DispatchMapMsgEvent(MsgID, 3017299663, -1)
		end
	end
end

aux.RegisterCreatureEvent(1521001, 4, "x1521001_OnDie")

function x1521002_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	local k = math.random(100)

	if k<= 15 then
		local x, y, z = unit.GetPosition(MapID, InstanceID, TargetID)
		map.MapCreateCreature(3017299663, -1, 1521006, x+1, y, z+1)
		map.MapCreateCreature(3017299663, -1, 1521006, x+2, y, z)
		map.MapCreateCreature(3017299663, -1, 1521006, x, y, z+2)
		map.MapCreateCreature(3017299663, -1, 1521006, x, y, z-2)
		map.MapCreateCreature(3017299663, -1, 1521006, x-1, y, z-1)
		map.MapCreateCreature(3017299663, -1, 1521006, x-2, y, z)
	elseif k<= 20 then
		local CreatureID = map.MapCreateCreature(3017299663, -1, 3020617, 2112, 17850, 3150)
		unit.AddBuff(MapID, InstanceID, CreatureID, 5001401+math.random(3)*200, CreatureID)
	elseif k<= 25 then
		local CreatureID = map.MapCreateCreature(3017299663, -1, 3020618, 2104, 17850, 3150)
		unit.AddBuff(MapID, InstanceID, CreatureID, 5002001+math.random(3)*200, CreatureID)
	elseif k>98 then
		if YouLongXiFeng_Boss == 0 then
			map.MapCreateCreature(3017299663, -1, 3020608, 2108, 17850, 3147)
			YouLongXiFeng_Boss = 1
			--��������¹ȫ����֪ͨ[��Ļ��������������ʾ]����***��
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 100, 121)
			msg.DispatchMapMsgEvent(MsgID, 3017299663, -1)
		end
	end

end

aux.RegisterCreatureEvent(1521002, 4, "x1521002_OnDie")

function x1521003_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	local k = math.random(100)

	if k<= 15 then
		local x, y, z = unit.GetPosition(MapID, InstanceID, TargetID)
		map.MapCreateCreature(3017299663, -1, 1521007, x+1, y, z+1)
		map.MapCreateCreature(3017299663, -1, 1521007, x+2, y, z)
		map.MapCreateCreature(3017299663, -1, 1521007, x, y, z+2)
		map.MapCreateCreature(3017299663, -1, 1521007, x, y, z-2)
		map.MapCreateCreature(3017299663, -1, 1521007, x-1, y, z-1)
		map.MapCreateCreature(3017299663, -1, 1521007, x-2, y, z)
	elseif k<= 20 then
		local CreatureID = map.MapCreateCreature(3017299663, -1, 3020617, 2112, 17850, 3150)
		unit.AddBuff(MapID, InstanceID, CreatureID, 5001401+math.random(3)*200, CreatureID)
	elseif k<= 25 then
		local CreatureID = map.MapCreateCreature(3017299663, -1, 3020618, 2104, 17850, 3150)
		unit.AddBuff(MapID, InstanceID, CreatureID, 5002001+math.random(3)*200, CreatureID)
	elseif k>98 then
		if YouLongXiFeng_Boss == 0 then
			map.MapCreateCreature(3017299663, -1, 3020609, 2108, 17850, 3147)
			YouLongXiFeng_Boss = 1
			--��������¹ȫ����֪ͨ[��Ļ��������������ʾ]����***��
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 100, 121)
			msg.DispatchMapMsgEvent(MsgID, 3017299663, -1)

		end
	end

end

aux.RegisterCreatureEvent(1521003, 4, "x1521003_OnDie")

function x1521004_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	local k = math.random(100)

	if k<= 15 then
		local x, y, z = unit.GetPosition(MapID, InstanceID, TargetID)
		map.MapCreateCreature(3017299663, -1, 1521008, x+1, y, z+1)
		map.MapCreateCreature(3017299663, -1, 1521008, x+2, y, z)
		map.MapCreateCreature(3017299663, -1, 1521008, x, y, z+2)
		map.MapCreateCreature(3017299663, -1, 1521008, x, y, z-2)
		map.MapCreateCreature(3017299663, -1, 1521008, x-1, y, z-1)
		map.MapCreateCreature(3017299663, -1, 1521008, x-2, y, z)
	elseif k<= 20 then
		local CreatureID = map.MapCreateCreature(3017299663, -1, 3020617, 2112, 17850, 3150)
		unit.AddBuff(MapID, InstanceID, CreatureID, 5001401+math.random(3)*200, CreatureID)
	elseif k<= 25 then
		local CreatureID = map.MapCreateCreature(3017299663, -1, 3020618, 2104, 17850, 3150)
		unit.AddBuff(MapID, InstanceID, CreatureID, 5002001+math.random(3)*200, CreatureID)
	elseif k>98 then
		if YouLongXiFeng_Boss == 0 then
			map.MapCreateCreature(3017299663, -1, 3020610, 2108, 17850, 3147)
			YouLongXiFeng_Boss = 1
			--��������¹ȫ����֪ͨ[��Ļ��������������ʾ]����***��
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 100, 121)
			msg.DispatchMapMsgEvent(MsgID, 3017299663, -1)
		end
	end

end

aux.RegisterCreatureEvent(1521004, 4, "x1521004_OnDie")
