
--ScriptData ��0λ �Ƿ�ˢ��BOSS
--ScriptData ��1λ BOSS1id
--ScriptData ��2λ BOSS2id
--ScriptData ��3λ BOSS3id
--ScriptData ��4λ BOSS4id
--ScriptData ��5λ BOSS5id
--ScriptData ��6λ BOSS6id
--ScriptData ��7λ BOSS7id
--ScriptData ��8λ ��¼����������
--ScriptData ��9λ ��¼���ɱ����BOSS����
--ScriptData ��10λ ��¼BOSS3�ĵ�1��С���Ƿ���
--ScriptData ��11λ ��¼BOSS3�ĵ�2��С���Ƿ���
--ScriptData ��12λ ��¼BOSS3�ĵ�3��С���Ƿ���
--ScriptData ��13λ ��¼BOSS3�ĵ�4��С���Ƿ���
--ScriptData ��14λ ��¼BOSS3�ĵ�5��С���Ƿ���
--ScriptData ��15λ ��¼BOSS3�ĵ�6��С���Ƿ���
--ScriptData ��16λ ��¼BOSS3�ĵ�7��С���Ƿ���
--ScriptData ��17λ ��¼BOSS3�ĵ�8��С���Ƿ���
--ScriptData ��18λ ��¼BOSS3�ĵ�9��С���Ƿ���
--ScriptData ��19λ ��¼BOSS3�ĵ�10��С���Ƿ���
--ScriptData ��20λ ��¼BOSS3�ĵ�11��С���Ƿ���
--ScriptData ��21λ ��¼BOSS3�ĵ�12��С���Ƿ���
--ScriptData ��22λ ��¼��ɱ����1�ĵڼ���С��
--ScriptData ��23λ ��¼��7�ļ�ʱ�����ò���ˢ
--ScriptData ��24λ ��¼��4��5�Ƿ�����
--ScriptData ��25λ ��¼��4��5����ʱ���X
--ScriptData ��26λ ��¼��4��5����ʱ���y
--ScriptData ��27λ ��¼��4��5����ʱ���z
--ScriptData ��28λ ��¼��7�Ƿ���ս��״̬

function m49_OnPlayerEnter(MapID, InstanceID, RoleID)

    local bhave = role.IsRoleHaveBuff(MapID, InstanceID, RoleID, 99996)
	if bhave == true then
		unit.CancelBuff(MapID, InstanceID, RoleID, 9999601)
	end

    if map.GetMapScriptData(MapID,InstanceID,1,0) == 0 then
	    map.OpenCloseDoor(MapID, InstanceID, 2427, nil)
		map.OpenCloseDoor(MapID, InstanceID, 2773, nil)
		map.OpenCloseDoor(MapID, InstanceID, 2872, nil)
		map.OpenCloseDoor(MapID, InstanceID, 2985, nil)
		map.OpenCloseDoor(MapID, InstanceID, 3026, nil)
		map.OpenCloseDoor(MapID, InstanceID, 2402, nil)
		map.SetMapScriptData(MapID,InstanceID,1,0,1)
		local BossID1 = map.MapCreateColCreature(MapID, InstanceID, 1012017, 940, 40, 517, "")
		local BossID2 = map.MapCreateColCreature(MapID, InstanceID, 1012018, 521, 40, 693, "")
		local BossID3 = map.MapCreateColCreature(MapID, InstanceID, 1012061, 758, 40, 1225, "")
		local BossID4 = map.MapCreateColCreature(MapID, InstanceID, 1012036, 453, 40, 885, "")
		local BossID5 = map.MapCreateColCreature(MapID, InstanceID, 1012037, 453, 40, 869, "")
		unit.AddBuff(MapID, InstanceID, BossID4, 9439801, BossID4)
		unit.AddBuff(MapID, InstanceID, BossID5, 9439801, BossID5)
		local BossID6 = map.MapCreateColCreature(MapID, InstanceID, 1012055, 529, 40, 481, "")
		local BossID7 = map.MapCreateColCreature(MapID, InstanceID, 1012054, 194, 40, 286, "")
		local MonID1 = map.MapCreateColCreature(MapID, InstanceID, 1012038, 734, 40, 1217, "")
		local MonID2 = map.MapCreateColCreature(MapID, InstanceID, 1012038, 782, 40, 1217, "")
		local MonID3 = map.MapCreateColCreature(MapID, InstanceID, 1012038, 723, 40, 1247, "")
		local MonID4 = map.MapCreateColCreature(MapID, InstanceID, 1012038, 793, 40, 1247, "")
		local MonID5 = map.MapCreateColCreature(MapID, InstanceID, 1012038, 717, 40, 1219, "")
		local MonID6 = map.MapCreateColCreature(MapID, InstanceID, 1012038, 798, 40, 1219, "")
		local MonID7 = map.MapCreateColCreature(MapID, InstanceID, 1012040, 734, 40, 1204, "")
		local MonID8 = map.MapCreateColCreature(MapID, InstanceID, 1012040, 781, 40, 1204, "")
		local MonID9 = map.MapCreateColCreature(MapID, InstanceID, 1012040, 701, 40, 1231, "")
		local MonID10 = map.MapCreateColCreature(MapID, InstanceID, 1012040, 814, 40, 1231, "")
		local MonID11 = map.MapCreateColCreature(MapID, InstanceID, 1012040, 730, 40, 1264, "")
		local MonID12 = map.MapCreateColCreature(MapID, InstanceID, 1012040, 786, 40, 1264, "")
		map.MapCreateColCreature(MapID, InstanceID, 4900496, 526, 40, 290, "")
		map.MapCreateColCreature(MapID, InstanceID, 1012057, 909, 40, 330, "")
		map.MapCreateColCreature(MapID, InstanceID, 1012057, 699, 40, 329, "")
		map.MapCreateColCreature(MapID, InstanceID, 1012057, 836, 40, 689, "")
		map.MapCreateColCreature(MapID, InstanceID, 1012057, 608, 40, 927, "")
		map.MapCreateColCreature(MapID, InstanceID, 1012057, 840, 40, 925, "")
		map.MapCreateColCreature(MapID, InstanceID, 1012057, 443, 40, 1120, "")
		map.MapCreateColCreature(MapID, InstanceID, 1012057, 141, 40, 1077, "")
		map.MapCreateColCreature(MapID, InstanceID, 1012057, 512, 40, 486, "")
		map.SetMapScriptData(MapID,InstanceID,1,1,BossID1)
		map.SetMapScriptData(MapID,InstanceID,1,2,BossID2)
		map.SetMapScriptData(MapID,InstanceID,1,3,BossID3)
		map.SetMapScriptData(MapID,InstanceID,1,4,BossID4)
		map.SetMapScriptData(MapID,InstanceID,1,5,BossID5)
		map.SetMapScriptData(MapID,InstanceID,1,6,BossID6)
		map.SetMapScriptData(MapID,InstanceID,1,7,BossID7)
		map.SetMapScriptData(MapID,InstanceID,1,10,MonID1)
		map.SetMapScriptData(MapID,InstanceID,1,11,MonID2)
		map.SetMapScriptData(MapID,InstanceID,1,12,MonID3)
		map.SetMapScriptData(MapID,InstanceID,1,13,MonID4)
		map.SetMapScriptData(MapID,InstanceID,1,14,MonID5)
		map.SetMapScriptData(MapID,InstanceID,1,15,MonID6)
		map.SetMapScriptData(MapID,InstanceID,1,16,MonID7)
		map.SetMapScriptData(MapID,InstanceID,1,17,MonID8)
		map.SetMapScriptData(MapID,InstanceID,1,18,MonID9)
		map.SetMapScriptData(MapID,InstanceID,1,19,MonID10)
		map.SetMapScriptData(MapID,InstanceID,1,20,MonID11)
		map.SetMapScriptData(MapID,InstanceID,1,21,MonID12)

    end
end

--ע��
aux.RegisterMapEvent("m49", 2, "m49_OnPlayerEnter")


function m49_OnRoleEnterArea(MapID, InstanceID, RoleID, ObjAreaID)

    if map.GetMapScriptData(MapID,InstanceID,1,8) == 0 then
	    local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 12201)	--ʹ����Ĺ����Կ�׿������ſ�����ս���Ѷȸ���
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 12202)	--���ſ���������BOSS����Ϊ��һ��BOSS
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	end

end

aux.RegisterMapEvent("m49", 7, "m49_OnRoleEnterArea")


function M49_OnDie(MapID, InstanceID, CreatureID, CreatureTypeID, RoleID)
    if CreatureTypeID == 1012038 or CreatureTypeID == 1012040 then
	    for i = 10,21 do
		    if map.GetMapScriptData(MapID,InstanceID,1,i) == CreatureID then
			    map.SetMapScriptData(MapID,InstanceID,1,i,0)
				break
			end
		end
	elseif CreatureTypeID == 1012057 then
	    local TeamID = role.IsRoleHaveTeam(MapID, InstanceID, RoleID)
		if TeamID ~= nil and TeamID ~= 4294967295 then
			local Role = {}
			Role[1], Role[2], Role[3], Role[4], Role[5], Role[6] = role.GetRoleTeamMemID(TeamID)
			for i=1, 6 do
				if Role[i] and Role[i]~= 4294967295 then
					local Target_MapID, Target_InstanceID = role.GetRoleMapID(Role[i])
					if Target_MapID == MapID then
					    local rolelevel = role.GetRoleLevel(Target_MapID, Target_InstanceID, Role[i])
						role.AddRoleExp(MapID, InstanceID, Role[i], math.floor(activity_newexp[rolelevel]/20))
						if role.GetBagFreeSize(Role[i]) > 0 then
						    role.AddRoleItem(MapID, InstanceID, Role[i], 6000218, 1, -1, 8, 420)
						end
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 101, 12203)	--�������ʯ����þ���xx��
						msg.AddMsgEvent(MsgID, 9,math.floor(activity_newexp[rolelevel]/20))
						msg.DispatchRoleMsgEvent(Role[i], MsgID)
					end
				end
			end
		else
		    local rolelevel = role.GetRoleLevel(MapID, InstanceID, RoleID)
			role.AddRoleExp(MapID, InstanceID, RoleID, math.floor(activity_newexp[rolelevel]/20))
			if role.GetBagFreeSize(RoleID) > 0 then
				role.AddRoleItem(MapID, InstanceID, RoleID, 6000218, 1, -1, 8, 420)
			end
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 101, 12203)	--�������ʯ����þ���xx��
			msg.AddMsgEvent(MsgID, 9, math.floor(activity_newexp[rolelevel]/20))
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	elseif CreatureTypeID == 1012058 then
		if map.GetMapScriptData(MapID,InstanceID,1,7) ~= 0 and map.GetMapScriptData(MapID,InstanceID,1,23) == 1 then
			unit.AddBuff(MapID, InstanceID, map.GetMapScriptData(MapID,InstanceID,1,7), 9440205, map.GetMapScriptData(MapID,InstanceID,1,7))
			unit.CancelBuff(MapID, InstanceID, map.GetMapScriptData(MapID,InstanceID,1,7), 9440205)
			unit.AddBuff(MapID, InstanceID, map.GetMapScriptData(MapID,InstanceID,1,7), 9440201, map.GetMapScriptData(MapID,InstanceID,1,7))
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 101, 12204)	--����ħ������������ʱ������
			msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID)
		end
	elseif CreatureTypeID == 1012056 then
	    map.SetMapScriptData(MapID,InstanceID,1,22,map.GetMapScriptData(MapID,InstanceID,1,22)+1)
	    if map.GetMapScriptData(MapID,InstanceID,1,22) == 6 then
			unit.CancelBuff(MapID, InstanceID, map.GetMapScriptData(MapID,InstanceID,1,1), 9439101)
			unit.CancelBuff(MapID, InstanceID, map.GetMapScriptData(MapID,InstanceID,1,1), 9439201)
			unit.CancelBuff(MapID, InstanceID, map.GetMapScriptData(MapID,InstanceID,1,1), 9439301)
		end
	elseif CreatureTypeID == 1012017 or CreatureTypeID == 1012018 or CreatureTypeID == 1012061 or CreatureTypeID == 1012054 or CreatureTypeID == 1012055 then

		if map.GetMapScriptData(MapID,InstanceID,1,8) == 1 then
            local x, y, z = unit.GetPosition(MapID, InstanceID, CreatureID)
			map.MapCreateColCreature(MapID, InstanceID, 4900712, x, y, z, "")
			if CreatureTypeID == 1012054 then
			    map.OpenCloseDoor(MapID, InstanceID, 3026, 1)
			elseif CreatureTypeID == 1012055 then
				map.OpenCloseDoor(MapID, InstanceID, 2985, 1)
			elseif CreatureTypeID == 1012061 then
				map.OpenCloseDoor(MapID, InstanceID, 2773, 1)
				map.OpenCloseDoor(MapID, InstanceID, 2872, 1)
			elseif CreatureTypeID == 1012018 then
				map.OpenCloseDoor(MapID, InstanceID, 2427, 1)
				unit.CancelBuff(MapID, InstanceID, map.GetMapScriptData(MapID,InstanceID,1,1), 9440401)
			    local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 101, 12205)	--�����๤���޵�״̬�Ѿ�ȡ��
				msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID)
			end
		else
		   -- local x, y, z = unit.GetPosition(MapID, InstanceID, CreatureID)
		   -- map.MapCreateColCreature(MapID, InstanceID, 4900711, x, y, z, "")
			if CreatureTypeID == 1012017 then
			    map.OpenCloseDoor(MapID, InstanceID, 2427, 1)
			elseif CreatureTypeID == 1012018 then
				map.OpenCloseDoor(MapID, InstanceID, 2773, 1)
			elseif CreatureTypeID == 1012061 then
				map.OpenCloseDoor(MapID, InstanceID, 2773, 1)
				map.OpenCloseDoor(MapID, InstanceID, 2872, 1)
			elseif CreatureTypeID == 1012055 then
				map.OpenCloseDoor(MapID, InstanceID, 3026, 1)
			elseif CreatureTypeID == 1012054 then
				map.OpenCloseDoor(MapID, InstanceID, 2402, 1)
			end
		end
		if CreatureTypeID == 1012054 then
		    map.SetMapScriptData(MapID,InstanceID,1,23, 0)
			map.GetMapScriptData(MapID,InstanceID,1, 7, 0)
		end
	elseif CreatureTypeID == 1012036 or CreatureTypeID == 1012037 then
		map.SetMapScriptData(MapID,InstanceID,1,24, map.GetMapScriptData(MapID,InstanceID,1,24)+1)
		if CreatureTypeID == 1012036 then
		    map.SetMapScriptData(MapID,InstanceID,1,4, 0)
		else
		    map.SetMapScriptData(MapID,InstanceID,1,5, 0)
		end
		if map.GetMapScriptData(MapID,InstanceID,1,24) ~= 2 then
			local x, y, z = unit.GetPosition(MapID, InstanceID, CreatureID)
			map.SetMapScriptData(MapID,InstanceID,1,25, x)
			map.SetMapScriptData(MapID,InstanceID,1,26, y)
			map.SetMapScriptData(MapID,InstanceID,1,27, z)
			local id = 1012036
			if id == CreatureTypeID then
				id = 1012037
			end
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 101, 12206)	--����15���ڻ�ɱXXX������XXX���Ḵ��
			msg.AddMsgEvent(MsgID, 5, id)
			msg.AddMsgEvent(MsgID, 5, CreatureTypeID)
			msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID)
			map.MapCreateColCreature(MapID, InstanceID, 1012059, 1, 40, 1, "")
		else
            local x, y, z = unit.GetPosition(MapID, InstanceID, CreatureID)
			if map.GetMapScriptData(MapID,InstanceID,1,8) == 1 then
			    map.MapCreateColCreature(MapID, InstanceID, 4900712, x, y, z, "")
				map.OpenCloseDoor(MapID, InstanceID, 2872, 1)
			else
                map.MapCreateColCreature(MapID, InstanceID, 4900711, x, y, z, "")
				map.OpenCloseDoor(MapID, InstanceID, 2985, 1)
			end
        end
	end
end
aux.RegisterCreatureEvent(1012017, 4, "M49_OnDie")
aux.RegisterCreatureEvent(1012018, 4, "M49_OnDie")
aux.RegisterCreatureEvent(1012061, 4, "M49_OnDie")
aux.RegisterCreatureEvent(1012036, 4, "M49_OnDie")
aux.RegisterCreatureEvent(1012037, 4, "M49_OnDie")
aux.RegisterCreatureEvent(1012038, 4, "M49_OnDie")
aux.RegisterCreatureEvent(1012040, 4, "M49_OnDie")
aux.RegisterCreatureEvent(1012054, 4, "M49_OnDie")
aux.RegisterCreatureEvent(1012055, 4, "M49_OnDie")
aux.RegisterCreatureEvent(1012056, 4, "M49_OnDie")
aux.RegisterCreatureEvent(1012057, 4, "M49_OnDie")
aux.RegisterCreatureEvent(1012058, 4, "M49_OnDie")



function M49_OnEnterCombat(MapID, InstanceID, CreatureID)
    local TypeID = cre.GetCreatureTypeID(MapID, InstanceID, CreatureID)
	if TypeID == 1012061 then
		map.OpenCloseDoor(MapID, InstanceID, 2773, nil)
		map.OpenCloseDoor(MapID, InstanceID, 2872, nil)
	    local tbl = {}
		tbl[10] = {734, 40, 1217}
		tbl[11] = {782, 40, 1217}
		tbl[12] = {723, 40, 1247}
		tbl[13] = {793, 40, 1247}
		tbl[14] = {717, 40, 1219}
		tbl[15] = {798, 40, 1219}
		tbl[16] = {734, 40, 1204}
		tbl[17] = {781, 40, 1204}
		tbl[18] = {701, 40, 1231}
		tbl[19] = {814, 40, 1231}
		tbl[20] = {730, 40, 1264}
		tbl[21] = {786, 40, 1264}
	    for i = 10,15 do
		    if map.GetMapScriptData(MapID,InstanceID,1,i) == 0 then
			    map.SetMapScriptData(MapID,InstanceID,1,i,map.MapCreateColCreature(MapID, InstanceID, 1012038, tbl[i][1], tbl[i][2], tbl[i][3], ""))
			end
		end
		for i = 16,21 do
		    if map.GetMapScriptData(MapID,InstanceID,1,i) == 0 then
			    map.SetMapScriptData(MapID,InstanceID,1,i,map.MapCreateColCreature(MapID, InstanceID, 1012040, tbl[i][1], tbl[i][2], tbl[i][3], ""))
			end
		end
		local Creature = {}
		for i = 10, 21 do
			Creature[i] = map.GetMapScriptData(MapID,InstanceID,1,i)
			unit.CancelBuff(MapID, InstanceID, Creature[i], 9431501)
			unit.AddBuff(MapID, InstanceID, Creature[i], 9431501, Creature[i])  --��������С�ֱ�ǿ
		end
		    --�õ�����ĵ�ǰĿ��
		local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
		--ͬ����޸���ΧΪ8�����ӣ��߶�Ϊ20���ѷ�����
		g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID, 1, 30, 20)
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 101, 12207)	--��������Ĵ��Ŷ��ѹر�
		msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID)
	elseif TypeID == 1012036 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 101, 12208)	--�����ڳ�׽�������15���ڻ�ɱ��׽�
		msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID)
	elseif TypeID == 1012054 then
		unit.AddBuff(MapID, InstanceID, CreatureID, 9440201, CreatureID)
	    local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 101, 12209)	--�ҵ����ݻٷ����еľ����������ʱ��������ħ��������
		msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID)
        map.MapCreateColCreature(MapID, InstanceID, 1012060, 1, 1, 1, "")
		local Point = {}
		Point[1] = {x = 153,y = 40,z = 216}
		Point[2] = {x = 145,y = 40,z = 265}
		Point[3] = {x = 146,y = 40,z = 319}
		Point[4] = {x = 193,y = 40,z = 345}
		Point[5] = {x = 249,y = 40,z = 355}
		Point[6] = {x = 267,y = 40,z = 316}
		Point[7] = {x = 269,y = 40,z = 266}
		Point[8] = {x = 272,y = 40,z = 237}
		Point[9] = {x = 199,y = 40,z = 218}
		Point[10] = {x = 232,y = 40,z = 215}
		Point[11] = {x = 180,y = 40,z = 249}
		Point[12] = {x = 232,y = 40,z = 319}
		Point[13] = {x = 242,y = 40,z = 253}
		Point[14] = {x = 182,y = 40,z = 309}
		Point[15] = {x = 211,y = 40,z = 269}
		map.SetMapScriptData(MapID,InstanceID,1,23, 1)
		local temp = math.random(15)
		map.MapCreateColCreature(MapID, InstanceID, 1012058, Point[temp].x, Point[temp].y, Point[temp].z, "")
	end
	--�õ�����ĵ�ǰĿ��
	local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
	--ͬ����޸���ΧΪ8�����ӣ��߶�Ϊ20���ѷ�����
	g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID, 1, 30, 20)
end
aux.RegisterCreatureEvent(1012054, 2, "M49_OnEnterCombat")
aux.RegisterCreatureEvent(1012055, 2, "M49_OnEnterCombat")
aux.RegisterCreatureEvent(1012061, 2, "M49_OnEnterCombat")
aux.RegisterCreatureEvent(1012036, 2, "M49_OnEnterCombat")
aux.RegisterCreatureEvent(1012037, 2, "M49_OnEnterCombat")
aux.RegisterCreatureEvent(1012017, 2, "M49_OnEnterCombat")
aux.RegisterCreatureEvent(1012018, 2, "M49_OnEnterCombat")
aux.RegisterCreatureEvent(1012038, 2, "M49_OnEnterCombat")
aux.RegisterCreatureEvent(1012040, 2, "M49_OnEnterCombat")

function c1012054_LeaveCombat(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	map.SetMapScriptData(MapID,InstanceID,1,23, 0)
	unit.AddBuff(MapID, InstanceID, TargetID, 9440205, TargetID)
	unit.CancelBuff(MapID, InstanceID, TargetID, 9440205)
end
aux.RegisterCreatureEvent(1012054, 3, "c1012054_LeaveCombat")

function c1012061_LeaveCombat(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	if map.GetMapScriptData(MapID,InstanceID,1,8) == 1 then
	    map.OpenCloseDoor(MapID, InstanceID, 2872, 1)
	else
		map.OpenCloseDoor(MapID, InstanceID, 2773, 1)
	end
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 101, 12210)	--�����е�һ�ȴ��Ŵ���
	msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID)
end
aux.RegisterCreatureEvent(1012061, 3, "c1012061_LeaveCombat")



--��15��������ʱ�䣬����ʱ��ʹ�ã���4��5��
function c1012059_OnDisappear(MapID, InstanceID, TargetID, TargetTypeID)
    if map.GetMapScriptData(MapID,InstanceID,1,24) < 2 then
		map.SetMapScriptData(MapID,InstanceID,1,24,0)
		local x = map.GetMapScriptData(MapID,InstanceID,1,25)
		local y = map.GetMapScriptData(MapID,InstanceID,1,26)
		local z = map.GetMapScriptData(MapID,InstanceID,1,27)
		local id = 1012036
		local id2 = 0
		if map.GetMapScriptData(MapID,InstanceID,1,4) ~= 0 then
		    id = 1012037
			id2 = map.SetMapScriptData(MapID,InstanceID,1,5,map.MapCreateColCreature(MapID, InstanceID, 1012037, x, y, z, ""))
		else
		    id2 = map.SetMapScriptData(MapID,InstanceID,1,4,map.MapCreateColCreature(MapID, InstanceID, 1012036, x, y, z, ""))
		end
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 101, 12211)	--                 XXX�Ѿ�����
		msg.AddMsgEvent(MsgID, 5, id)
		msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID)
		unit.AddBuff(MapID, InstanceID, id2, 9439801, id2)
		if map.GetMapScriptData(MapID,InstanceID,1,8) == 1 then
            unit.AddBuff(MapID, InstanceID, id2, 9440301, id2)
        end
	end
end
aux.RegisterCreatureEvent(1012059, 13, "c1012059_OnDisappear")

--��15��������ʱ�䣬����ʱ��ʹ�ã���7��
function c1012060_OnDisappear(MapID, InstanceID, TargetID, TargetTypeID)
    if map.GetMapScriptData(MapID,InstanceID,1,23) == 1 then
        map.MapCreateColCreature(MapID, InstanceID, 1012060, 1, 1, 1, "")
		local Point = {}
		Point[1] = {x = 153,y = 40,z = 216}
		Point[2] = {x = 145,y = 40,z = 265}
		Point[3] = {x = 146,y = 40,z = 319}
		Point[4] = {x = 193,y = 40,z = 345}
		Point[5] = {x = 249,y = 40,z = 355}
		Point[6] = {x = 267,y = 40,z = 316}
		Point[7] = {x = 269,y = 40,z = 266}
		Point[8] = {x = 272,y = 40,z = 237}
		Point[9] = {x = 199,y = 40,z = 218}
		Point[10] = {x = 232,y = 40,z = 215}
		Point[11] = {x = 180,y = 40,z = 249}
		Point[12] = {x = 232,y = 40,z = 319}
		Point[13] = {x = 242,y = 40,z = 253}
		Point[14] = {x = 182,y = 40,z = 309}
		Point[15] = {x = 211,y = 40,z = 269}
		local temp = math.random(15)
		map.MapCreateColCreature(MapID, InstanceID, 1012058, Point[temp].x, Point[temp].y, Point[temp].z, "")
	end
end
aux.RegisterCreatureEvent(1012060, 13, "c1012060_OnDisappear")


--�ɷ�ʹ�ú���
function i6000217_CanUse(MapID, InstanceID, TypeID, TargetID)
    local bRet, bIgnore = 0, false
	if MapID ~= 3084409807 then
	    bRet = 43
	else
	    if map.IsInArea(3084409807, InstanceID, TargetID, 3175) ~= true then
		    bRet = 32
		end
		if map.GetMapScriptData(MapID,InstanceID,1,8) ~= 0 or map.GetMapScriptData(MapID,InstanceID,1,9) ~= 0 then
		    bRet = 32
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 26, 12212)	--��ɱ��һ��BOSS֮���޷���������
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	end

	--����
	return bRet, bIgnore
end

--Կ��ʹ��Ч��
function i6000217_QuestUsable(MapID, InstanceID, TypeID, TargetID)
	map.SetMapScriptData(MapID,InstanceID,1,8,1)
	map.OpenCloseDoor(MapID, InstanceID, 2402, 1)
	local BOSS = {}
	for i = 1, 7 do
		BOSS[i] = map.GetMapScriptData(MapID,InstanceID,1,i)
	    unit.AddBuff(MapID, InstanceID, BOSS[i], 9440301, BOSS[i])  --����BOSS��ǿ
	    unit.AddBuff(MapID, InstanceID, BOSS[i], 1101803, BOSS[i])
	end
	local Creature = {}
	for i = 10, 21 do
		Creature[i] = map.GetMapScriptData(MapID,InstanceID,1,i)
	    unit.AddBuff(MapID, InstanceID, Creature[i], 9431501, Creature[i])  --��������С�ֱ�ǿ
	end
	unit.AddBuff(MapID, InstanceID, BOSS[1], 9440401, BOSS[1])  --��һ��BOSS�޵�
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 101, 12213)			-- ����������Ѿ�����������BOSS�Ѷ�����
	msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID)
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 101, 12214)			-- �����๤���޵�״̬���ڻ�ɱ��������BOSS����ʧ
	msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID)
end

--ע��
aux.RegisterItemEvent(6000217, 1, "i6000217_QuestUsable")
aux.RegisterItemEvent(6000217, 0, "i6000217_CanUse")


--�ɷ�ʹ�ú���
function i6000218_CanUse(MapID, InstanceID, TypeID, TargetID)
    local bRet, bIgnore = 0, false
	local bhave = role.IsRoleHaveBuff(MapID, InstanceID, TargetID, 94401)
	if not bhave then
	    bRet = 32
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 12215)	--���봦�����ȼ�յ�״̬�в���ʹ�ø���Ʒ
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	end

	--����
	return bRet, bIgnore
end

--Կ��ʹ��Ч��
function i6000218_QuestUsable(MapID, InstanceID, TypeID, TargetID)
	unit.CancelBuff(MapID, InstanceID, TargetID, 9440101)
end

--ע��
aux.RegisterItemEvent(6000218, 1, "i6000218_QuestUsable")
aux.RegisterItemEvent(6000218, 0, "i6000218_CanUse")

function N4900496_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	if TalkIndex == -1 then
	    if map.GetMapScriptData(MapID,InstanceID,1,8) == 1 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20,	12216)		--���������飺\n    ���뿪�ҵĵ��̣����ǲ�����Ҫ�ҵı���Կ�����Ҹ�����ֻ�����ѣ�����һ��һ�����˸Ͻ�����\n    ��ʾ��ÿ����ֻ����ȡһ��Կ�ס���
			msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
			msg.AddMsgEvent(MsgID, 21,	4)			--ȡ����ť
			msg.AddMsgEvent(MsgID, 1,	21)			--����ȡԿ�ס�
			msg.AddMsgEvent(MsgID, 21,	5)			--ȡ����ť
			msg.AddMsgEvent(MsgID, 1,	22)			--��ȡ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
            local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20,	12217)		--���������飺\n    ������ʲô�ˣ���Ϊʲô�������������ЩԿ���Ǹ�ʲô�õģ����ǿ�Щ�뿪�ɣ���ͷ�۵�������\n    ��ʾ����������ģʽ�����Ժ���ҿ��ԴӾ������鴦��ȡ������Ĺ�����Կ�ס���Ĺ����������ģʽ�»�ɱBOSS�Ķ��⽱������
			msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
			msg.AddMsgEvent(MsgID, 21,	5)			--ȡ����ť
			msg.AddMsgEvent(MsgID, 1,	21)			--��ȡ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	elseif TalkIndex == 4 then
	    local bool = false
		local k = 999
	    for i = 0,5 do
		    if cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, i) == 0 then
			    k = i
			end
		    if cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, i) == RoleID then
			    bool = true
				break
			end
		end
        if bool then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20,	12218)		--���������飺\n    ���Ѿ��õ�Կ���ˣ��Ҳ����ٸ����κζ��������ߣ���
			msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
			msg.AddMsgEvent(MsgID, 21,	5)			--ȡ����ť
			msg.AddMsgEvent(MsgID, 1,	21)			--��ȡ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		elseif k == 999 then
		    local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20,	12219)		--���������飺\n    �ҵ�����Կ���Ѿ����������ˣ���Ҫ���������ˡ�����
			msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
			msg.AddMsgEvent(MsgID, 21,	5)			--ȡ����ť
			msg.AddMsgEvent(MsgID, 1,	21)			--��ȡ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		elseif role.GetBagFreeSize(RoleID) < 1 then
		    local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20,	12220)		--���������飺\n    �ѵ�����������һ��Կ�׵ĵط���û���𣿡�
			msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
			msg.AddMsgEvent(MsgID, 21,	5)			--ȡ����ť
			msg.AddMsgEvent(MsgID, 1,	21)			--��ȡ����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
		    cre.SetCreatureScriptData(MapID, InstanceID, TargetID, 1, k, RoleID)
			role.AddRoleItem(MapID, InstanceID, RoleID, 6000219, 1, -1, 8, 420)
		end
	end
end

aux.RegisterCreatureEvent(4900496, 7, "N4900496_OnTalk")

function s2430301_Cast(MapID, InstanceID, SkillID, OwnerID)
     local TypeID = cre.GetCreatureTypeID(MapID, InstanceID, OwnerID)
     if TypeID == 1012017 then
		map.SetMapScriptData(MapID,InstanceID,1,22,0)
		local tbl = {}
		local RoleTbl = map.GetMapAllRoleID(MapID, InstanceID)
		local k = 0
		for u,v in pairs(RoleTbl) do
		    if u ~= nil then
			    k = k + 1
			    tbl[k] = v
			end
		end
		if k > 0 then
		    for i = 1,6 do
			    local x, y, z = unit.GetPosition(MapID, InstanceID, tbl[i%k+1])
			    map.MapCreateColCreature(MapID, InstanceID, 1012056, x, y, z, "")
			end
		end
     end
     return 0
end

--ע��

aux.RegisterSkillEvent(2430301, 1, "s2430301_Cast")

function N4902002_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	if TalkIndex == -1 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20,	12222)		--��¬������\n    �����ﵽ100�������Ѿ��������ұ��а취�������������Ĺ̽Ѱ�������µı��������Ȱ������ܶ����������һ���ȥ����Ȼ���������汣ס������\n    ��ʾ�������ӵ����Ĺ����Կ�׵�ʱ�򣬿���ͨ����������������Ĵ�������������������ģʽ����ģʽ�¸�����BOSS���ѶȻ��нϴ�����������䱻��ɱ�Ժ��ˢ������ı��䡣������ҪԿ�ײ��ܿ�����ÿ���˶����ԴӸ����еľ������鴦��ȡһ������Կ�ס�\n    ������������ģʽ��δ���š���
		msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
		msg.AddMsgEvent(MsgID, 21,	4)			--ȡ����ť
		msg.AddMsgEvent(MsgID, 1,	12223)			--�����븱����
		msg.AddMsgEvent(MsgID, 21,	5)			--ȡ����ť
		msg.AddMsgEvent(MsgID, 1,	22)			--��ȡ����
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	elseif TalkIndex == 4 then
	    --[[if role.GetSoarValue(RoleID) ~= 1 then
		    local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20,	)		--��¬������\n    �㻹û�з������Ҳ���������������Ĺ����
			msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
			msg.AddMsgEvent(MsgID, 21,	5)			--ȡ����ť
			msg.AddMsgEvent(MsgID, 1,	21)			--�����븱����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else]]
		if role.GetRoleLevel(MapID, InstanceID, RoleID) < 80 then
		    local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20,	12225)		--��¬������\n    �㻹û�дﵽ100�����Ҳ���������������Ĺ����
			msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
			msg.AddMsgEvent(MsgID, 21,	5)			--ȡ����ť
			msg.AddMsgEvent(MsgID, 1,	21)			--�����븱����
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
			role.RoleGotoNewMap(MapID, InstanceID, RoleID, 3084409807, 804, 1, 111)
		end
	end
end

aux.RegisterCreatureEvent(4902002, 7, "N4902002_OnTalk")

--������ɫ��¯
function C4900712_OnInVest(MapID, InstanceID, CreatureID, CreatureTypeID, RoleID)
	local k = role.RemoveFromRole(MapID, InstanceID, RoleID, 6000219, 1, 420)
	--[[local HP = unit.GetUnitAttValue(MapID, InstanceID, CreatureID, 9)
	if k ~= nil and k == 0 then
	    if HP == 2 then
		    unit.ChangeHP(MapID, InstanceID, CreatureID, -1)
		end
	else
	    unit.ChangeHP(MapID, InstanceID, CreatureID, 1)
	end]]
end
--ע�������ɫ��¯�¼�
aux.RegisterCreatureEvent(4900712, 6, "C4900712_OnInVest")
