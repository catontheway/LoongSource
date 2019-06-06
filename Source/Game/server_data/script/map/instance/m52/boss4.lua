--[[
MapScriptData 1�� 10���ʱ��ʹ�ã����ڼ��������ﵽ��׼���ͺ��������е���һ���
MapScriptData 2����¼˫��������ID
MapScriptData 3����¼1���Ӵ�����ID
MapScriptData 4����¼�Ƿ�
MapScriptData 5����¼���ӷ�
MapScriptData 6����¼�ϻ���
MapScriptData 7����¼����
MapScriptData 8����¼��դ������
MapScriptData 9����¼˫�������Ƿ������ս����������=1����ս/����=0
MapScriptData 10:��¼����԰��ID
MapScriptData 11:��¼��³����ID
MapScriptData 12:��¼�ϴ��ID
MapScriptData 13:��¼��2��ID
MapScriptData 14:��¼��3��ID
MapScriptData 15:��¼�ƹǶ�Ы��ID
MapScriptData 16:��¼�ƹǶ�Ы��ID
MapScriptData 17:��¼�ƹǶ�Ы��ID
MapScriptData 18:��¼�ƹǶ�Ы��ID
MapScriptData 19:��¼�ƹǶ�Ы��ID
MapScriptData 20:��¼�ƹǶ�Ы��ID
MapScriptData 21:��¼�ƹǶ�Ы��ID
MapScriptData 22:��¼�ƹǶ�Ы��ID
MapScriptData 23:��¼�ƹǶ�Ы��ID
MapScriptData 24:��¼�ƹǶ�Ы��ID
MapScriptData 25:��¼������Ч1��ID
MapScriptData 26:��¼������Ч2��ID
MapScriptData 27:��¼������Ч3��ID
MapScriptData 28:��¼�����Ƿ����
MapScriptData 29:��¼���Ѿ�����Ĭ��ֵ
MapScriptData 30:��¼���������Ƿ����
MapScriptData 31:��¼ˢ�ŵ�ʱ��
]]--

--����NPC��
--npc id��δָ��
function m52_OnPlayerEnter(MapID, InstanceID, RoleID)

	--���븱��ʱɾ����ҵ�����buff
	local bhave = role.IsRoleHaveBuff(MapID, InstanceID, RoleID, 99996)
	if bhave == true then
		unit.CancelBuff(MapID, InstanceID, RoleID, 9999601)
	end

    if map.GetMapScriptData(MapID, InstanceID, 1, 29) ~= 1 then
		map.OpenCloseDoor(MapID, InstanceID, 1072, 1)  --����
		map.OpenCloseDoor(MapID, InstanceID, 1087, 1)  --����
		map.OpenCloseDoor(MapID, InstanceID, 1073, 1)   --��³��
		map.OpenCloseDoor(MapID, InstanceID, 1074, nil)

		local c_table = {}
		c_table[1] = {Cretype=1535463,x=357,y=1000,z=346}
		c_table[2] = {Cretype=1535463,x=351,y=1000,z=340}
		c_table[3] = {Cretype=1535464,x=328,y=1000,z=303}
		c_table[4] = {Cretype=1535464,x=322,y=1000,z=297}
		c_table[5] = {Cretype=1535465,x=320,y=1000,z=372}
		c_table[6] = {Cretype=1535465,x=314,y=1000,z=366}
		c_table[7] = {Cretype=1535466,x=289,y=1000,z=330}
		c_table[8] = {Cretype=1535466,x=283,y=1000,z=324}
		c_table[9] = {Cretype=1535466,x=289,y=1000,z=324}
		c_table[10] = {Cretype=1535462,x=331,y=1000,z=325}  --����԰
		c_table[11] = {Cretype=1535478,x=448,y=2110,z=193}  --��³��
		c_table[12] = {Cretype=1535482,x=383,y=1050,z=118}  --�ϴ�
		c_table[13] = {Cretype=1535483,x=368,y=1050,z=106}   --��2
		c_table[14] = {Cretype=1535484,x=370,y=1050,z=118}   --��3
		c_table[15] = {Cretype=1535479,x=439,y=2110,z=165}   --С��1
		c_table[16] = {Cretype=1535480,x=439,y=2110,z=180}   --С��2
		c_table[17] = {Cretype=1535479,x=439,y=2110,z=195}    --С��3
		c_table[18] = {Cretype=1535480,x=452,y=2110,z=165}    --С��4
		c_table[19] = {Cretype=1535479,x=452,y=2110,z=180}    --С��5
		c_table[20] = {Cretype=1535480,x=452,y=2110,z=195}    --С��6
		c_table[21] = {Cretype=1535479,x=465,y=2110,z=165}    --С��7
		c_table[22] = {Cretype=1535480,x=465,y=2110,z=180}    --С��8
		c_table[23] = {Cretype=1535479,x=465,y=2110,z=195}    --С��9
		c_table[24] = {Cretype=1535480,x=460,y=2110,z=187}    --С��10
		c_table[25] = {Cretype=4902025,x=226,y=1250,z=138}    --����1  ����԰
		c_table[26] = {Cretype=4902026,x=246,y=1250,z=109}    --����2  ��³��
		c_table[27] = {Cretype=4902027,x=268,y=1250,z=138}    --����3  ����


		for i = 1,27 do
			local ID = map.MapCreateColCreature(MapID, InstanceID, c_table[i].Cretype, c_table[i].x, c_table[i].y, c_table[i].z, "")
			if i >= 1 and i <= 9 then
			    cre.SetCreatureScriptData(MapID, InstanceID, ID, 1, 0, 1)
			end
			if i >= 10 and i <= 27 then
				map.SetMapScriptData(MapID, InstanceID, 1, i, ID)
				if i >= 15 and i <= 24 then
					cre.SetCreatureScriptData(MapID, InstanceID, ID, 1, 0, i-14)
				end
			end
		end

		map.SetMapScriptData(MapID, InstanceID, 1, 28, 1)
		map.SetMapScriptData(MapID, InstanceID, 1, 29, 1)
	end

end

--ע��
aux.RegisterMapEvent("m52", 2, "m52_OnPlayerEnter")

--BOOS����ǰ���Դ���
function m52_OnEnterTrigger(MapID, InstanceID, RoleID, ObjID)
	local Trigger_ObjID = {}
	Trigger_ObjID[1248] = {data=10,x=434,y=20,z=360} --����԰
	Trigger_ObjID[1249] = {data=11,x=216,y=44,z=38} --��³��
	Trigger_ObjID[1250] = {data=28,x=439,y=20,z=328} --����
	if map.GetMapScriptData(MapID, InstanceID, 1, Trigger_ObjID[ObjID].data) ~= 0 then
		role.RoleGotoNewMap(MapID, InstanceID, RoleID, 3067629775, Trigger_ObjID[ObjID].x, Trigger_ObjID[ObjID].y, Trigger_ObjID[ObjID].z)
	end
end
aux.RegisterMapEvent("m52",6,"m52_OnEnterTrigger")

function x4902029_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
    --��ȡ������ҵȼ�
	local rolelevel = role.GetRoleLevel(MapID, InstanceID, RoleID)
	local TeamID = role.IsRoleHaveTeam(MapID, InstanceID, RoleID)
    if TalkIndex == -1 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20,	12701)		--��������������ʱ�ջ�������ܵ��ﴫ˵�е��ϹŶ�Ѩ����ֻ��100�����ϵ���ҿ��Խ��룩��
		msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
		msg.AddMsgEvent(MsgID, 21,	6)			--ȷ����ť
		msg.AddMsgEvent(MsgID, 1,	12702)			--�������ϹŶ�Ѩ��
		msg.AddMsgEvent(MsgID, 21,	5)			--ȡ����ť
		msg.AddMsgEvent(MsgID, 1,	22)			--��ȡ����
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	elseif TalkIndex == 6 then
		if TeamID == 4294967295 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 	2812)	--��û����ӣ����ܽ���ø���
			msg.AddMsgEvent(MsgID, 21, 	5)		--ȡ����ť
			msg.AddMsgEvent(MsgID, 1, 	22)		--ȡ��
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
			local Role = {}
			Role[1], Role[2], Role[3], Role[4], Role[5], Role[6] = role.GetRoleTeamMemID(TeamID)
			local Num = 0
			for i=1, 6 do
				if Role[i] and Role[i]~= 4294967295 then
					Num = Num + 1
				end
			end
			if rolelevel < 100 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 	2813)	--���ȼ����㣬���ܽ���ø���
				msg.AddMsgEvent(MsgID, 21, 	5)		--ȡ����ť
				msg.AddMsgEvent(MsgID, 1, 	22)		--ȡ��
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			elseif Num < 4 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 12703)	--����Ҫ�չ�4�����Ѳ��ܽ����ϹŶ�Ѩ
				msg.AddMsgEvent(MsgID, 21, 	5)		--ȡ����ť
				msg.AddMsgEvent(MsgID, 1, 	22)		--ȡ��
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			else
				role.RoleGotoNewMap(MapID, InstanceID, RoleID, 3067629775, 61, 46, 450)
			end
		end
	end
end
--
aux.RegisterCreatureEvent(4902029, 7, "x4902029_OnTalk")


----------------------------------------����԰-----------------------------------------------------------

--����çԳ����ս������
function c1535462_OnEnterCombat(MapID, InstanceID, CreatureID)
	map.OpenCloseDoor(MapID, InstanceID, 1072, nil)
end
aux.RegisterCreatureEvent(1535462, 2, "c1535462_OnEnterCombat")

--boss��ս�ſ���
function c1535462_OnLeaveCombat(MapID, InstanceID, CreatureID)
	map.OpenCloseDoor(MapID, InstanceID, 1072, 1)
end
aux.RegisterCreatureEvent(1535462, 3, "c1535462_OnLeaveCombat")

--BOSS��������
function c1535462_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

    map.OpenCloseDoor(MapID, InstanceID, 1072, 1)
	map.SetMapScriptData(MapID, InstanceID, 1, 10, 0)
	map.MapDeleteCreature(MapID, InstanceID, map.GetMapScriptData(MapID, InstanceID, 1, 25))

	if map.GetMapScriptData(MapID, InstanceID, 1, 11) == 0 and map.GetMapScriptData(MapID, InstanceID, 1, 28) == 0 then
        map.OpenCloseDoor(MapID, InstanceID, 1074, 1)
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 101, 12704)  --ͨ��˫��������Ѩ�Ĵ����Ѿ�������
		msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)
    end

end
aux.RegisterCreatureEvent(1535462, 4, "c1535462_OnDie")


--ʹ�ü��ܽ����̷���,�������һ����Ұ�˳�����һ��Χ��
function s2432701_Cast(MapID, InstanceID, SkillID, OwnerID)
	local RoleTbl = map.GetMapAllRoleID(MapID, InstanceID)
	local Role = {}
	local k = 0
	for u,v in pairs(RoleTbl) do
		if v ~= nil then
			k = k + 1
			Role[k] = v
		end
	end
	if k > 0 then
		local r = math.random(1,k)
		if unit.IsDead(MapID, InstanceID, Role[r]) then
		    for i = 1,k do
			    if not unit.IsDead(MapID, InstanceID, Role[i]) then
				    r = i
				end
			end
		end
		if not unit.IsDead(MapID, InstanceID, Role[r]) then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 101, 12705)  --:���ҵĳ������������棡
			msg.AddMsgEvent(MsgID, 2, Role[r])
			msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)
			local Place = {}
			Place[1] = {x = 354,y = 20,z = 343} --�Ƿ�
			Place[2] = {x = 325,y = 20,z = 300} --���ӷ�
			Place[3] = {x = 317,y = 20,z = 369} --�ϻ���
			Place[4] = {x = 286,y = 20,z = 327} --����

			if map.GetMapScriptData(MapID, InstanceID, 1, 8) == 0 then
				map.SetMapScriptData(MapID, InstanceID, 1, 8, 1)
			end

			local p = map.GetMapScriptData(MapID, InstanceID, 1, 8) % 4 + 1
			role.RoleGotoNewMap(MapID, InstanceID, Role[r], 3067629775, Place[p].x, Place[p].y, Place[p].z)
			--map.SetMapScriptData(MapID, InstanceID, 1, 1, Role1[r])
		end
	end
end
aux.RegisterSkillEvent(2432701, 1, "s2432701_Cast")


--Χ���й�������,�ﵽ����,������һص���������
function c1535463_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
    if cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 0) == 1 then
		map.SetMapScriptData(MapID, InstanceID, 1, 4, map.GetMapScriptData(MapID, InstanceID, 1, 4) + 1)
		if(map.GetMapScriptData(MapID, InstanceID, 1, 4) == 2) then
			local RoleTbl = map.GetMapAllRoleID(MapID, InstanceID)
			for u,v in pairs(RoleTbl) do
				if v ~= nil then
					if map.IsInArea(MapID, InstanceID, v, 1188) then  --1188�Ƿ�������
						role.RoleGotoNewMap(MapID, InstanceID, v, 3067629775, 331, 20, 325)
					end
				end
			end
			local ID = map.MapCreateColCreature(MapID, InstanceID, 1535463, 357, 1000, 346, "")
			cre.SetCreatureScriptData(MapID, InstanceID, ID, 1, 0, 1)
			local ID = map.MapCreateColCreature(MapID, InstanceID, 1535463, 351, 1000, 340, "")
			cre.SetCreatureScriptData(MapID, InstanceID, ID, 1, 0, 1)
			map.SetMapScriptData(MapID, InstanceID, 1, 4, 0)
		end
	end
end

function c1535464_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
    if cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 0) == 1 then
		map.SetMapScriptData(MapID, InstanceID, 1, 5, map.GetMapScriptData(MapID, InstanceID, 1, 5) + 1)
		if(map.GetMapScriptData(MapID, InstanceID, 1, 5) == 2) then
			local RoleTbl = map.GetMapAllRoleID(MapID, InstanceID)
			for u,v in pairs(RoleTbl) do
				if v ~= nil then
					if map.IsInArea(MapID, InstanceID, v, 1189) then  --1189���ӷ�������
						role.RoleGotoNewMap(MapID, InstanceID, v, 3067629775, 331, 20, 325)
					end
				end
			end
			local ID = map.MapCreateColCreature(MapID, InstanceID, 1535464, 328, 1000, 303, "")
			cre.SetCreatureScriptData(MapID, InstanceID, ID, 1, 0, 1)
			local ID = map.MapCreateColCreature(MapID, InstanceID, 1535464, 322, 1000, 297, "")
			cre.SetCreatureScriptData(MapID, InstanceID, ID, 1, 0, 1)
			map.SetMapScriptData(MapID, InstanceID, 1, 5, 0)
		end
	end
end

function c1535465_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
    if cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 0) == 1 then
		map.SetMapScriptData(MapID, InstanceID, 1, 6, map.GetMapScriptData(MapID, InstanceID, 1, 6) + 1)
		if(map.GetMapScriptData(MapID, InstanceID, 1, 6) == 2) then
			local RoleTbl = map.GetMapAllRoleID(MapID, InstanceID)
			for u,v in pairs(RoleTbl) do
				if v ~= nil then
					if map.IsInArea(MapID, InstanceID, v, 1190) then  --1190�ϻ���������
						role.RoleGotoNewMap(MapID, InstanceID, v, 3067629775, 331, 20, 325)
					end
				end
			end
			local ID = map.MapCreateColCreature(MapID, InstanceID, 1535465, 320, 1000, 372, "")
			cre.SetCreatureScriptData(MapID, InstanceID, ID, 1, 0, 1)
			local ID = map.MapCreateColCreature(MapID, InstanceID, 1535465, 314, 1000, 366, "")
			cre.SetCreatureScriptData(MapID, InstanceID, ID, 1, 0, 1)
			map.SetMapScriptData(MapID, InstanceID, 1, 6, 0)
		end
	end
end

function c1535466_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
    if cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 0) == 1 then
		map.SetMapScriptData(MapID, InstanceID, 1, 7, map.GetMapScriptData(MapID, InstanceID, 1, 7) + 1)
		if(map.GetMapScriptData(MapID, InstanceID, 1, 7) == 3) then
			local RoleTbl = map.GetMapAllRoleID(MapID, InstanceID)
			for u,v in pairs(RoleTbl) do
				if v ~= nil then
					if map.IsInArea(MapID, InstanceID, v, 1191) then  --1191���ѵ�����
						role.RoleGotoNewMap(MapID, InstanceID, v, 3067629775, 331, 20, 325)
					end
				end
			end
			local ID = map.MapCreateColCreature(MapID, InstanceID, 1535466, 289, 1000, 330, "")
			cre.SetCreatureScriptData(MapID, InstanceID, ID, 1, 0, 1)
			local ID = map.MapCreateColCreature(MapID, InstanceID, 1535466, 283, 1000, 324, "")
			cre.SetCreatureScriptData(MapID, InstanceID, ID, 1, 0, 1)
			local ID = map.MapCreateColCreature(MapID, InstanceID, 1535466, 289, 1000, 324, "")
			cre.SetCreatureScriptData(MapID, InstanceID, ID, 1, 0, 1)
			map.SetMapScriptData(MapID, InstanceID, 1, 7, 0)
		end
	end
end
aux.RegisterCreatureEvent(1535463, 4, "c1535463_OnDie")
aux.RegisterCreatureEvent(1535464, 4, "c1535464_OnDie")
aux.RegisterCreatureEvent(1535465, 4, "c1535465_OnDie")
aux.RegisterCreatureEvent(1535466, 4, "c1535466_OnDie")


--ˢ���Ĺֶ�ʱ��ʧ
--դ���������������
function m52_OnRoleDie(MapID, InstanceID, RoleID, KillID)
	if map.IsInArea(MapID, InstanceID, RoleID, 1188) or map.IsInArea(MapID, InstanceID, RoleID, 1189) or map.IsInArea(MapID, InstanceID, RoleID, 1190) or map.IsInArea(MapID, InstanceID, RoleID, 1191) then
		role.RoleGotoNewMap(MapID, InstanceID, RoleID, 3067629775, 331, 1000, 325)
	end
end
aux.RegisterMapEvent("m52", 5, "m52_OnRoleDie")

--�ٻ�����,����ʹ�ú�,�����м�4�������ˢһ�������Ĺ���,ͬʱҲ����ǰBOSSĿ������ˢ����NPC

function s2432801_Cast(MapID, InstanceID, SkillID, OwnerID)

	local RanC = {1535467, 1535468, 1535469, 1535470}
	local PlaceCenter = {}
	PlaceCenter[1] = {x = 330, y = 1000, z = 349}
	PlaceCenter[2] = {x = 330, y = 1000, z = 324}
	PlaceCenter[3] = {x = 311, y = 1000, z = 319}
	PlaceCenter[4] = {x = 302, y = 1000, z = 344}

	for i = 1,4 do
		local rc = math.random(1,4)
		map.MapCreateColCreature(MapID, InstanceID, RanC[rc], PlaceCenter[i].x, PlaceCenter[i].y, PlaceCenter[i].z, "")
	end
	local x1, y1, z1 = unit.GetPosition(MapID,InstanceID,OwnerID)
	local CreatureID = map.MapCreateCreature(MapID,InstanceID, 4902024,x1,y1,z1)
	--unit.AddBuff(MapID, InstanceID, CreatureID, 9441501, CreatureID)

	return 0
end

aux.RegisterSkillEvent(2432801, 1, "s2432801_Cast")

-------------------------------------------��³��------------------------------------------------------------------------
--��³������ս������
function c1535478_OnEnterCombat(MapID, InstanceID, CreatureID)
	map.OpenCloseDoor(MapID, InstanceID, 1073, nil)
	unit.AddBuff(MapID, InstanceID, CreatureID, 9443401, CreatureID)
end
aux.RegisterCreatureEvent(1535478, 2, "c1535478_OnEnterCombat")

--boss��ս�ſ���
function c1535478_OnLeaveCombat(MapID, InstanceID, CreatureID)
	map.OpenCloseDoor(MapID, InstanceID, 1073, 1)
	unit.CancelBuff(MapID, InstanceID, CreatureID, 9443401)
end
aux.RegisterCreatureEvent(1535478, 3, "c1535478_OnLeaveCombat")

--BOSS��������
function c1535478_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

    for i = 1,10 do
	    for k = 1,10 do
		    local a = math.random(2)
			local p = math.random(100)
			if p > 80 then
			    map.MapCreateCreature(MapID,InstanceID, 1535478+a,218+i*5,1250,96+k*7)
			end
		end
	end

	map.OpenCloseDoor(MapID, InstanceID, 1073, 1)
	map.SetMapScriptData(MapID, InstanceID, 1, 11, 0)
	map.MapDeleteCreature(MapID, InstanceID, map.GetMapScriptData(MapID, InstanceID, 1, 26))

	if map.GetMapScriptData(MapID, InstanceID, 1, 10) == 0 and map.GetMapScriptData(MapID, InstanceID, 1, 28) == 0 then
        map.OpenCloseDoor(MapID, InstanceID, 1074, 1)
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 101, 12704)  --ͨ��˫��������Ѩ�Ĵ����Ѿ�������
		msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)
    end
end
aux.RegisterCreatureEvent(1535478, 4, "c1535478_OnDie")

--������Ѽ�ֻ��Ы
function s2433001_Cast(MapID, InstanceID, SkillID, OwnerID)
    for i=15,24 do
	    if math.random(10) > 6 and map.GetMapScriptData(MapID, InstanceID, 1, i) ~= 0 then
		    unit.AddBuff(MapID, InstanceID, map.GetMapScriptData(MapID, InstanceID, 1, i), 9443401, map.GetMapScriptData(MapID, InstanceID, 1, i))
			local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, OwnerID)
			cre.AddEnmity(MapID, InstanceID, map.GetMapScriptData(MapID, InstanceID, 1, i), TargetID, 1)

		end
	end

	return 0
end

aux.RegisterSkillEvent(2433001, 1, "s2433001_Cast")

function c1535479_OnEnterCombat(MapID, InstanceID, CreatureID)
	unit.AddBuff(MapID, InstanceID, CreatureID, 9442801, CreatureID)
end
aux.RegisterCreatureEvent(1535479, 2, "c1535479_OnEnterCombat")
aux.RegisterCreatureEvent(1535480, 2, "c1535479_OnEnterCombat")

--
function c1535479_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

    local k = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 0)

    if map.GetMapScriptData(MapID, InstanceID, 1, 11) ~= 0 and k ~= 0 then
	    local pos = {}
	    pos[1] = {Cretype=1535479,x=439,y=2110,z=165}   --С��1
		pos[2] = {Cretype=1535480,x=439,y=2110,z=180}   --С��2
		pos[3] = {Cretype=1535479,x=439,y=2110,z=195}    --С��3
		pos[4] = {Cretype=1535480,x=452,y=2110,z=165}    --С��4
		pos[5] = {Cretype=1535479,x=452,y=2110,z=180}    --С��5
		pos[6] = {Cretype=1535480,x=452,y=2110,z=195}    --С��6
		pos[7] = {Cretype=1535479,x=465,y=2110,z=165}    --С��7
		pos[8] = {Cretype=1535480,x=465,y=2110,z=180}    --С��8
		pos[9] = {Cretype=1535479,x=465,y=2110,z=195}    --С��9
		pos[10] = {Cretype=1535480,x=460,y=2110,z=187}    --С��10
		local monid = map.MapCreateColCreature(MapID, InstanceID, pos[k].Cretype, pos[k].x, pos[k].y, pos[k].z, "")
		cre.SetCreatureScriptData(MapID, InstanceID, monid, 1, 0, k)
		map.SetMapScriptData(MapID, InstanceID, 1, k+14, monid)
	end

end
aux.RegisterCreatureEvent(1535479, 4, "c1535479_OnDie")
aux.RegisterCreatureEvent(1535480, 4, "c1535479_OnDie")
---------------------------------------------------------����-------------------------------------------------------------------------------------------

--����
function c1535482_OnEnterCombat(MapID, InstanceID, CreatureID)

	map.OpenCloseDoor(MapID, InstanceID, 1087, nil)

	local Creature = {}
	Creature[1535482] = {13,14}
	Creature[1535483] = {12,14}
	Creature[1535484] = {12,13}

	local pos = {}
	pos[12] = {Cretype=1535482,x=383,y=1050,z=118}  --�ϴ�
	pos[13] = {Cretype=1535483,x=368,y=1050,z=106}   --��2
	pos[14] = {Cretype=1535484,x=370,y=1050,z=118}   --��3

	local TypeID = cre.GetCreatureTypeID(MapID, InstanceID, CreatureID)
	local Boss1 = map.GetMapScriptData(MapID, InstanceID, 1, Creature[TypeID][1])
	local Boss2 = map.GetMapScriptData(MapID, InstanceID, 1, Creature[TypeID][2])
	if Boss1 == 0 then
		local mon = map.MapCreateColCreature(MapID, InstanceID, pos[Creature[TypeID][1]].Cretype, pos[Creature[TypeID][1]].x, pos[Creature[TypeID][1]].y, pos[Creature[TypeID][1]].z, "")
		map.SetMapScriptData(MapID, InstanceID, 1, Creature[TypeID][1], mon)
	end
	if Boss2 == 0 then
		local mon = map.MapCreateColCreature(MapID, InstanceID, pos[Creature[TypeID][2]].Cretype, pos[Creature[TypeID][2]].x, pos[Creature[TypeID][2]].y, pos[Creature[TypeID][2]].z, "")
		map.SetMapScriptData(MapID, InstanceID, 1, Creature[TypeID][2], mon)
	end

	local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
    --ͬ����޸���ΧΪ3�����ӣ��߶�Ϊ20���ѷ�����
    g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID, 1, 20, 20)

end
aux.RegisterCreatureEvent(1535482, 2, "c1535482_OnEnterCombat")
aux.RegisterCreatureEvent(1535483, 2, "c1535482_OnEnterCombat")
aux.RegisterCreatureEvent(1535484, 2, "c1535482_OnEnterCombat")

--boss��ս�ſ���
function c1535482_OnLeaveCombat(MapID, InstanceID, CreatureID)

    local Creature = {}
	Creature[1535482] = {13,14}
	Creature[1535483] = {12,14}
	Creature[1535484] = {12,13}
	local TypeID = cre.GetCreatureTypeID(MapID, InstanceID, CreatureID)
	local Boss1 = map.GetMapScriptData(MapID, InstanceID, 1, Creature[TypeID][1])
	local Boss2 = map.GetMapScriptData(MapID, InstanceID, 1, Creature[TypeID][2])
	local bool1 = false
	local bool2 = false
    if Boss1 == 0 then
	    bool1 = true
	elseif not unit.IsAttack(MapID, InstanceID, Boss1) then
	    bool1 = true
	end
	if Boss2 == 0 then
	    bool2 = true
	elseif not unit.IsAttack(MapID, InstanceID, Boss2) then
	    bool2 = true
	end
	if bool1 and bool2 then
		map.OpenCloseDoor(MapID, InstanceID, 1087, 1)
		local pos = {}
		pos[12] = {Cretype=1535482,x=383,y=1050,z=118}  --�ϴ�
		pos[13] = {Cretype=1535483,x=368,y=1050,z=106}   --��2
		pos[14] = {Cretype=1535484,x=370,y=1050,z=118}   --��3
		if Boss1 == 0 then
		    local mon = map.MapCreateColCreature(MapID, InstanceID, pos[Creature[TypeID][1]].Cretype, pos[Creature[TypeID][1]].x, pos[Creature[TypeID][1]].y, pos[Creature[TypeID][1]].z, "")
			map.SetMapScriptData(MapID, InstanceID, 1, Creature[TypeID][1], mon)
		end
		if Boss2 == 0 then
		    local mon = map.MapCreateColCreature(MapID, InstanceID, pos[Creature[TypeID][2]].Cretype, pos[Creature[TypeID][2]].x, pos[Creature[TypeID][2]].y, pos[Creature[TypeID][2]].z, "")
			map.SetMapScriptData(MapID, InstanceID, 1, Creature[TypeID][2], mon)
		end
		for i = 1,3 do
			unit.CancelBuff(MapID, InstanceID, map.GetMapScriptData(MapID, InstanceID, 1, 11+i), 9442301)
			unit.CancelBuff(MapID, InstanceID, map.GetMapScriptData(MapID, InstanceID, 1, 11+i), 9442401)
			unit.CancelBuff(MapID, InstanceID, map.GetMapScriptData(MapID, InstanceID, 1, 11+i), 9442501)
		end
	end

end
aux.RegisterCreatureEvent(1535482, 3, "c1535482_OnLeaveCombat")
aux.RegisterCreatureEvent(1535483, 3, "c1535482_OnLeaveCombat")
aux.RegisterCreatureEvent(1535484, 3, "c1535482_OnLeaveCombat")


--BOSS��������
function c1535484_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

	map.SetMapScriptData(MapID, InstanceID, 1, TargetTypeID-1535470, 0)

	local Creature = {}
	Creature[1535482] = {13,14,9442301}
	Creature[1535483] = {12,14,9442401}
	Creature[1535484] = {12,13,9442501}
	local Boss1 = map.GetMapScriptData(MapID, InstanceID, 1, Creature[TargetTypeID][1])
	local Boss2 = map.GetMapScriptData(MapID, InstanceID, 1, Creature[TargetTypeID][2])
	if Boss1 ~= 0 then
	    unit.AddBuff(MapID, InstanceID, Boss1,  Creature[TargetTypeID][3], Boss1)
	    unit.AddBuff(MapID, InstanceID, Boss1,  9443001, Boss1)
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 101, 12706)  --�����ֵܼ̳�����ȥ�����������������
		msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)
	end
	if Boss2 ~= 0 then
		unit.AddBuff(MapID, InstanceID, Boss2,  Creature[TargetTypeID][3], Boss2)
	    unit.AddBuff(MapID, InstanceID, Boss2,  9443001, Boss2)
	    local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 101, 12706)  --�����ֵܼ̳�����ȥ�����������������
		msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)
	end

	if Boss1 == 0 and Boss2 == 0 then

		map.SetMapScriptData(MapID, InstanceID, 1, 28, 0)

		map.OpenCloseDoor(MapID, InstanceID, 1073, 1)
		map.MapDeleteCreature(MapID, InstanceID, map.GetMapScriptData(MapID, InstanceID, 1, 27))

		map.MapCreateColCreature(MapID, InstanceID, 4902028, 383, 1050, 118, "")

		if map.GetMapScriptData(MapID, InstanceID, 1, 10) == 0 and map.GetMapScriptData(MapID, InstanceID, 1, 11) == 0 then
			map.OpenCloseDoor(MapID, InstanceID, 1074, 1)
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 101, 12704)  --ͨ��˫��������Ѩ�Ĵ����Ѿ�������
			msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)
		end

	end

end
aux.RegisterCreatureEvent(1535482, 4, "c1535484_OnDie")
aux.RegisterCreatureEvent(1535483, 4, "c1535484_OnDie")
aux.RegisterCreatureEvent(1535484, 4, "c1535484_OnDie")

---------------------------------------------------------˫������----------------------------------------------------------


--˫����������ս������B
function c1535455_OnEnterCombat(MapID, InstanceID, CreatureID)

	map.OpenCloseDoor(MapID, InstanceID, 1075, nil)
    map.MapCreateColCreature(MapID, InstanceID, 4902021, 1, 1, 1, "")

	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 101, 12707)  --˫��������������Ⱥ��֪����Ķ���
	msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)

	--��¼BOSS����ս����
	map.SetMapScriptData(MapID,InstanceID,1,9, 1)
end

aux.RegisterCreatureEvent(1535455, 2, "c1535455_OnEnterCombat")



--˫������ʹ�ü��ܣ�ĩ��������ˢ��������
function s2432201_Cast(MapID, InstanceID, SkillID, OwnerID)

	if map.GetMapScriptData(MapID,InstanceID,1,30) ~= 1 then

	    local cTime = os.time() --��ǰʱ��
		local lTime = map.GetMapScriptData(MapID,InstanceID,1,31)
		if lTime == 0 or os.difftime(cTime,lTime) > 60 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 101, 12708)  --[����]һ�����ҵĵض�ɽҡ�󣬳����г�����һ���ռ䴫���ţ�
			msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)

			--���ˢ������
			local door_30s = {}
			door_30s[1] = {x=127, y=1000, z=362 }
			door_30s[2] = {x=125, y=1000, z=399 }
			door_30s[3] = {x=104, y=1000, z=385 }
			door_30s[4] = {x=107, y=1000, z=359 }
			local k = math.random(4)  -- <=1,<=4

			local door = map.MapCreateColCreature(MapID, InstanceID, 4902020, door_30s[k].x, door_30s[k].y, door_30s[k].z, "")
			--��¼������ID
			map.SetMapScriptData(MapID,InstanceID,1,3, door)
			map.SetMapScriptData(MapID,InstanceID,1,31,cTime)
		end
    end
	return 0
end

aux.RegisterSkillEvent(2432201, 1, "s2432201_Cast")




--�����ŶԻ��������
function x4902020_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)

	if TalkIndex == -1 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 12709)		--�������ţ�\n    ˫�������������������κ�������û�е�����֮������Ҫ�����𣿡�
		msg.AddMsgEvent(MsgID, 24, TargetID)	--npcid
		msg.AddMsgEvent(MsgID, 21, 4)			--ȷ�ϰ�ť
		msg.AddMsgEvent(MsgID, 1, 12710)		--���ǵ�,��Ҫ���������������硣��
		msg.AddMsgEvent(MsgID, 21, 5)			--ȡ����ť
		msg.AddMsgEvent(MsgID, 1, 22)		--��ȡ����
		msg.DispatchRoleMsgEvent(RoleID, MsgID)

	elseif TalkIndex == 4 then			--ȷ�Ϻ��ͣ������͵�����Ͽ�buff

		--�ж�BOSS�Ƿ���ս��
		local Boss4_InBattle = map.GetMapScriptData(MapID,InstanceID,1,9)
		if Boss4_InBattle == 1 then
			--local door = map.GetMapScriptData(MapID,InstanceID,1,3)
			map.MapDeleteCreature(MapID, InstanceID, TargetID)  --�������ɾ��������
			map.MapCreateColCreature(MapID, InstanceID, 4902030, 1, 1, 1, "")  --ˢ��10s��ʱ��
			local k=role.GetRoleAttValue(MapID, InstanceID, RoleID, 65)
			role.ModRoleAttValue(MapID, InstanceID, RoleID, 65, -k)
			role.RoleGotoNewMap(MapID, InstanceID, RoleID, 3067629775, 65, 21, 42)
			map.SetMapScriptData(MapID,InstanceID,1,30, 1)

		end

	end
end

aux.RegisterCreatureEvent(4902020, 7, "x4902020_OnTalk")


--10���ʱ������ʱ���š�XX��󽫻��뿪˫���������������硱
function c4902030_OnDisappear(MapID, InstanceID, TargetID, TargetTypeID)
	local k1 = map.GetMapScriptData(MapID,InstanceID,1,1)
	map.SetMapScriptData(MapID,InstanceID,1,1, k1+1)
	local k2 = map.GetMapScriptData(MapID,InstanceID,1,1)
	if k2 == 1 or k2 == 2 then
		map.MapCreateColCreature(MapID, InstanceID, 4902030, 1, 1, 1, "")  --ˢ��10s��ʱ��

	elseif k2 == 3 then
	    local RoleTbl = map.GetMapAllRoleID(MapID, InstanceID)
	    for u,v in pairs(RoleTbl) do
			if v ~= nil then
				if map.IsInArea(MapID, InstanceID, v, 1092) then  --1092�������������
					local MsgID = msg.BeginMsgEvent() --30��󽫻��뿪˫����������������
					msg.AddMsgEvent(MsgID, 101, 12711)
					msg.AddMsgEvent(MsgID, 9, 30)
					msg.DispatchRoleMsgEvent(v, MsgID)
				end
			end
		end

		map.MapCreateColCreature(MapID, InstanceID, 4902030, 1, 1, 1, "")  --ˢ��10s��ʱ��

	elseif k2 == 4 then

		local RoleTbl = map.GetMapAllRoleID(MapID, InstanceID)
	    for u,v in pairs(RoleTbl) do
			if v ~= nil then
				if map.IsInArea(MapID, InstanceID, v, 1092) then  --1092�������������
					local MsgID = msg.BeginMsgEvent() --20��󽫻��뿪˫����������������
					msg.AddMsgEvent(MsgID, 101, 12711)
					msg.AddMsgEvent(MsgID, 9, 20)
					msg.DispatchRoleMsgEvent(v, MsgID)
				end
			end
		end

		map.MapCreateColCreature(MapID, InstanceID, 4902030, 1, 1, 1, "")  --ˢ��10s��ʱ��

	elseif k2 == 5 then

		local RoleTbl = map.GetMapAllRoleID(MapID, InstanceID)
	    for u,v in pairs(RoleTbl) do
			if v ~= nil then
				if map.IsInArea(MapID, InstanceID, v, 1092) then  --1092�������������
					local MsgID = msg.BeginMsgEvent() --10��󽫻��뿪˫����������������
					msg.AddMsgEvent(MsgID, 101, 12711)
					msg.AddMsgEvent(MsgID, 9, 10)
					msg.DispatchRoleMsgEvent(v, MsgID)
				end
			end
		end

		map.MapCreateColCreature(MapID, InstanceID, 4902030, 1, 1, 1, "")  --ˢ��10s��ʱ��

	elseif k2 >= 6 then --6��=60�룬������1���ӣ���ʱ����������������˾ͱ�������
		local RoleTbl = map.GetMapAllRoleID(MapID, InstanceID)
		map.SetMapScriptData(MapID,InstanceID,1,1, 0)
		map.SetMapScriptData(MapID,InstanceID,1,30, 0)
		for u,v in pairs(RoleTbl) do
			if v ~= nil then
				if map.IsInArea(MapID, InstanceID, v, 1092) then  --1092�������������
					role.RoleGotoNewMap(MapID, InstanceID, v, 3067629775, 113, 20, 369)
				end
			end
		end
	end
end

aux.RegisterCreatureEvent(4902030, 13, "c4902030_OnDisappear")




--20s��ʱ����ʧʱˢ��ѡ��������
function c4902021_OnDisappear(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	if map.GetMapScriptData(MapID,InstanceID,1,9) == 1 then
		local k = math.random(4, 9)
		for i = 1, k do
			local x = math.random(76, 162)
			local z = math.random(334, 424)
			local m52_HuoDui = map.MapCreateColCreature(MapID, InstanceID, 4902023, x, 1010, z, "")
			--unit.AddBuff(MapID, InstanceID, m52_HuoDui, 9440601, m52_HuoDui)     --ˢ������npc�����Լ����ϵ�buff
		end
		map.MapCreateColCreature(MapID, InstanceID, 4902021, 1, 1, 1, "")
	end
end

aux.RegisterCreatureEvent(4902021, 13, "c4902021_OnDisappear")


--��Ҳ���ս����ս��ʱ������ҽ������Ѫ
function aYanJiangQu_OnEnterArea(MapID, InstanceID, RoleID, ObjID)
	if ObjID==1084 then
		unit.AddBuff(MapID, InstanceID, RoleID, 9441101, RoleID)
	end
end

aux.RegisterMapEvent("m52", 7, "aYanJiangQu_OnEnterArea")


--������������B��
function c1535455_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

	map.OpenCloseDoor(MapID, InstanceID, 1075, 1)
	--�ͷ�ȫ�ֱ���
	map.SetMapScriptData(MapID,InstanceID,1,2, 0)

	--��¼BOSS����/��ս��
	map.SetMapScriptData(MapID,InstanceID,1,9, 0)

	--ɾ��������
	local del_door = map.GetMapScriptData(MapID,InstanceID,1,3)
	map.MapDeleteCreature(MapID, InstanceID, del_door)  --�������ɾ��������

end

aux.RegisterCreatureEvent(1535455, 4, "c1535455_OnDie")


--˫�������뿪ս��������D���Ҵ�ȫԱǿ�ƴ��͵���B�⣬��B��
function c1535455_OnLeaveCombat(MapID, InstanceID, CreatureID)
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 101, 12712)  --���������������������棬��Ȼ�����Ͳ�Ҫ��̤���ҵ���������
	msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)

	local TeamID = role.IsRoleHaveTeam(MapID, InstanceID, RoleID)
	if TeamID ~= nil then
		local Role = {}
			Role[1], Role[2], Role[3], Role[4], Role[5], Role[6] = role.GetRoleTeamMemID(TeamID)
			for i=1, 6 do
				role.RoleGotoNewMap(MapID, InstanceID, Role[i], 3373741864, 116, 1043, 320)
			end
	end
	map.OpenCloseDoor(MapID, InstanceID, 1075, 1)

	--��¼BOSS����/��ս��
	map.SetMapScriptData(MapID,InstanceID,1,9, 0)

	--ɾ��������
	local del_door = map.GetMapScriptData(MapID,InstanceID,1,3)
	map.MapDeleteCreature(MapID, InstanceID, del_door)  --�������ɾ��������

end
aux.RegisterCreatureEvent(1535455, 3, "c1535455_OnLeaveCombat")




--�ͻ�ʦ������ˢ��˫����������B����
function c1535461_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

	map.OpenCloseDoor(MapID, InstanceID, 1075, 1)

	--ȫ�ֱ�����С��������������buff��
	local ObjID = map.MapCreateColCreature(MapID, InstanceID, 1535455, 110, 1000, 385, "")
	map.SetMapScriptData(MapID,InstanceID,1,2, ObjID)

	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 101, 12713)  --��˫����������Ⱥû�õĶ��������ɣ���Ҫ�����������ǡ�����
	msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)
end

aux.RegisterCreatureEvent(1535461, 4, "c1535461_OnDie")



--�ػ�����������˫�������õ����Խ���buffһ��
function c1535456_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	--�ж�BOSS�Ƿ���ս��
	local Boss4_InBattle = map.GetMapScriptData(MapID,InstanceID,1,9)
	if Boss4_InBattle == 1 then
		local Boss4_ID = map.GetMapScriptData(MapID,InstanceID,1,2)
		unit.AddBuff(MapID, InstanceID, Boss4_ID, 9440901, Boss4_ID)
	end

end

aux.RegisterCreatureEvent(1535456, 4, "c1535456_OnDie")


--�ػ�����������˫�������õ����Խ���buffһ��
function c1535457_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	--�ж�BOSS�Ƿ���ս��
	local Boss4_InBattle = map.GetMapScriptData(MapID,InstanceID,1,9)
	if Boss4_InBattle == 1 then
		local Boss4_ID = map.GetMapScriptData(MapID,InstanceID,1,2)
		unit.AddBuff(MapID, InstanceID, Boss4_ID, 9440901, Boss4_ID)
	end

end

aux.RegisterCreatureEvent(1535457, 4, "c1535457_OnDie")



