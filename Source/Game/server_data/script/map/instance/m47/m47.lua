--map script data
--0. С�ֻ�ɱ����
--1. BOSS��ɱ����
--2-7. ʹ�ù���ʯ��Ƭ���˵�id
--8. ��������index
--9. �ٻ���start_tick
--10. �Ѳμ��ٻ�������
--11-16. ����ȡ�������˵�id
--17. BOSS ����
--18-20. �Ѳμ��ٻ����˵�id



--�ɷ�ʹ�ú���
function i3304016_CanUse(MapID, InstanceID, TypeID, TargetID)
    local bRet, bIgnore = 0, false

	--���������ħ�磬����ʹ��
	if MapID ~= 3084408271 then
	    bRet = 43
	else
	    --�õ���ҵ�ǰ����
	    local x, y ,z = unit.GetPosition(MapID, InstanceID, TargetID)
	    if x <= 364 or x >= 424 or z <= 260 or z >= 320 then  --�������������귶Χ������ʹ��
	        bRet = 43
	    end

		for i=2,7 do

			if TargetID==map.GetMapScriptData(MapID, InstanceID, 1, i) then
				bRet = 32
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 26, 1824)			--�Ѿ�ʹ�ù�
				msg.DispatchRoleMsgEvent(TargetID, MsgID)
				break
			end

		end

		if bRet~=40 and map.GetMapScriptData(MapID, InstanceID, 1, 7)~=0 then
			bRet = 32
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 26, 1825)			--�Ѿ�ʹ�ù�
			msg.DispatchRoleMsgEvent(TargetID, MsgID)
		end

	end

	--����
	return bRet, bIgnore
end

--Կ��ʹ��Ч��
function i3304016_QuestUsable(MapID, InstanceID, TypeID, TargetID)

	for i=2,7 do
		if map.GetMapScriptData(MapID, InstanceID, 1, i)==0 then
			map.SetMapScriptData(MapID, InstanceID, 1, i, TargetID)
			break
		end
	end

	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 26, 1826)			--���ٻ�����ħ����ħ
	msg.DispatchRoleMsgEvent(RoleID, MsgID)

	local x, y, z = unit.GetPosition(MapID, InstanceID, TargetID)

	map.MapCreateCreature(MapID, InstanceID, 1011011, 411, y, 297)
	map.MapCreateCreature(MapID, InstanceID, 1011011, 395, y, 300)
	map.MapCreateCreature(MapID, InstanceID, 1011011, 401, y, 276)
	map.MapCreateCreature(MapID, InstanceID, 1011011, 374, y, 262)
	map.MapCreateCreature(MapID, InstanceID, 1011011, 397, y, 260)
	map.MapCreateCreature(MapID, InstanceID, 1011012, 412, y, 312)
	map.MapCreateCreature(MapID, InstanceID, 1011012, 378, y, 319)
	map.MapCreateCreature(MapID, InstanceID, 1011012, 395, y, 332)
	map.MapCreateCreature(MapID, InstanceID, 1011012, 378, y, 300)
	map.MapCreateCreature(MapID, InstanceID, 1011012, 411, y, 310)

end

--ע��
aux.RegisterItemEvent(3304016, 1, "i3304016_QuestUsable")
aux.RegisterItemEvent(3304016, 0, "i3304016_CanUse")


--�����ά���˽ű�

function n_XinShiTai(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)

	local summon_start = map.GetMapScriptData(MapID, InstanceID, 1, 8)

	if summon_start==100 then		--ս���ѿ�ʼ

		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 1827)		--ս���ѿ�ʼ�����ܽ����ٻ�
		msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
		msg.DispatchRoleMsgEvent(RoleID, MsgID)

	elseif summon_start==200 then	--BOSS������������ȡ����

		local can_receive = 1

		for i = 11, 16 do

			if map.GetMapScriptData(MapID, InstanceID, 1, i)==RoleID then

				can_receive = 0
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 1828)		--������ȡ������
				msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
				msg.DispatchRoleMsgEvent(RoleID, MsgID)

			end

		end

		if map.GetMapScriptData(MapID, InstanceID, 1, 16)~=0 then

			can_receive = 0
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 1829)		--����6����ȡ������
			msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
			msg.DispatchRoleMsgEvent(RoleID, MsgID)

		end

		local FreeSize = role.GetBagFreeSize(RoleID)

		if FreeSize<=1 then
			can_receive = 0
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 1831)		--��������
			msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end

		if can_receive == 1 then
			for i = 11, 16 do
				if map.GetMapScriptData(MapID, InstanceID, 1, i)==0 then

					local BossType = map.GetMapScriptData(MapID, InstanceID, 1, 17)

					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 20, 1830)		--��ȡ�����ɹ�
					msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
					msg.DispatchRoleMsgEvent(RoleID, MsgID)

					role.AddRoleItem(MapID, InstanceID, RoleID, 3304000+BossType, 3, -1, 8, 420)

					map.SetMapScriptData(MapID, InstanceID, 1, i, RoleID)

					break

				end
			end
		end

	else

		if TalkIndex == -1 then

			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 1832)
			msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
			msg.AddMsgEvent(MsgID, 21,	4)			--ȷ����ť
			msg.AddMsgEvent(MsgID, 1,	1833)		--�ٻ���ħ1
			msg.AddMsgEvent(MsgID, 21,	5)			--ȷ����ť
			msg.AddMsgEvent(MsgID, 1,	1834)		--�ٻ���ħ2
			msg.AddMsgEvent(MsgID, 21,	6)			--ȷ����ť
			msg.AddMsgEvent(MsgID, 1,	1835)		--�ٻ���ħ3
			msg.DispatchRoleMsgEvent(RoleID, MsgID)

		elseif TalkIndex == 4 or TalkIndex == 5 or TalkIndex == 6 then

			if summon_start==0 then
			--��ʼ�ٻ�

				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 101, 1832+TalkIndex)	--�ѿ�ʼ�ٻ�
				msg.AddMsgEvent(MsgID, 2, RoleID)
				msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID)

				local start_tick = aux.GetWorldTick()

				summon_start = TalkIndex

				map.SetMapScriptData(MapID, InstanceID, 1, 8, summon_start)
				map.SetMapScriptData(MapID, InstanceID, 1, 9, start_tick)
				map.SetMapScriptData(MapID, InstanceID, 1, 18, RoleID)
				map.SetMapScriptData(MapID, InstanceID, 1, 10, 1)

			elseif summon_start==TalkIndex then

				local can_summon = 1
				local start_tick = map.GetMapScriptData(MapID, InstanceID, 1, 9)
				local cur_tick = aux.GetWorldTick()

				for i = 18, 20 do
					if map.GetMapScriptData(MapID, InstanceID, 1, i)==RoleID then
						can_summon = 0
					end
				end

				if cur_tick-start_tick>150 then
					map.SetMapScriptData(MapID, InstanceID, 1, 8, 0)
					map.SetMapScriptData(MapID, InstanceID, 1, 10, 0)

					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 101, 1839)	--�����¿�ʼ�ٻ�
					msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID)

				elseif can_summon==0 then

					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 20, 1840)	--���Ѿ����й��ٻ�
					msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
					msg.DispatchRoleMsgEvent(RoleID, MsgID)

				else

					local summon_num = map.GetMapScriptData(MapID, InstanceID, 1, 10)+1
					map.SetMapScriptData(MapID, InstanceID, 1, 17+summon_num, RoleID)
					map.SetMapScriptData(MapID, InstanceID, 1, 10, summon_num)

					if summon_num == 3 then

						local x, y, z = unit.GetPosition(MapID, InstanceID, TargetID)

						map.MapCreateCreature(MapID, InstanceID, 1011019+TalkIndex-4, x, y, z)		--ˢ��BOSS

						map.SetMapScriptData(MapID, InstanceID, 1, 8, 100)
						map.SetMapScriptData(MapID, InstanceID, 1, 17, TalkIndex)
						--[[�����˻����׶

						local TeamID = role.IsRoleHaveTeam(MapID, InstanceID, RoleID)
						--���û��С��

						if TeamID == nil or TeamID == 4294967295 then
							role.AddRoleItem(MapID, InstanceID, RoleID, 3304017, 99, -1, 8, 420)
						else
							local Role = {}
							Role[1], Role[2], Role[3], Role[4], Role[5], Role[6] = role.GetRoleTeamMemID(TeamID)
							for k in pairs(Role) do
								role.AddRoleItem(MapID, InstanceID, Role[1], 3304017, 99, -1, 8, 420)
							end
						end]]--

						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 101, 1841)	--BOSS�ѳ���
						msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID)

					end
				end
			elseif summon_start~=TalkIndex then

				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 1842)		--ѡ���������˲�����������ѡ��
				msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
				msg.AddMsgEvent(MsgID, 21,	4)			--ȷ����ť
				msg.AddMsgEvent(MsgID, 1,	1833)		--�ٻ���ħ1
				msg.AddMsgEvent(MsgID, 21,	5)			--ȷ����ť
				msg.AddMsgEvent(MsgID, 1,	1834)		--�ٻ���ħ2
				msg.AddMsgEvent(MsgID, 21,	6)			--ȷ����ť
				msg.AddMsgEvent(MsgID, 1,	1835)		--�ٻ���ħ3
				msg.DispatchRoleMsgEvent(RoleID, MsgID)

			end


		end

	end
end

-- ע��
aux.RegisterCreatureEvent(3075222, 7, "n_XinShiTai")
