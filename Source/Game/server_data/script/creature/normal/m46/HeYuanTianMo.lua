
--BOSS��Ԫ��ħ��һ��̬��1010410��Ѫ��40%ʱ��ʧ��ͬʱˢ��
--�ڶ���̬��1010411��X=500 Y=5664 Z=1778
--���������1010417��
--X=495 Y=5664 Z=1795
--X=508 Y=5664 Z=1796
--а����ˣ�1010414��
--X=486 Y=5664 Z=1781
--X=515 Y=5664 Z=1780
--X=497 Y=5664 Z=1765
--X=506 Y=5664 Z=1765
--1524��1525���ų�ʼΪ�ر�״̬����һ��̬��1010410����ʧʱ�����ſ������ڶ���̬��1010411������ս���󣬴��Źرա���ɱ�ڶ���̬���ſ�����
--1526 ���ų�ʼΪ�ر�״̬��ʹ�ÿ��ŵ�����Ŵ򿪡�

--[[function s2423701_Cast(MapID, InstanceID, SkillID, OwnerID)
	map.SetMapScriptData(MapID, InstanceID, 1, 5, 1)
	local TypeID = cre.GetCreatureTypeID(MapID, InstanceID, OwnerID)
	if TypeID == 1010410 then
		cre.MonsterSay(MapID, InstanceID, OwnerID, 50064)
	end
	return 0
end

--ע��

aux.RegisterSkillEvent(2423701, 1, "s2423701_Cast")]]

-- �������ս������
function c1010410_OnEnterCombat(MapID, InstanceID, CreatureID)

	cre.MonsterSay(MapID, InstanceID, CreatureID, 50063)

end

aux.RegisterCreatureEvent(1010410, 2, "c1010410_OnEnterCombat")

function c1010410_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
--    local i = map.GetMapScriptData(MapID, InstanceID, 1, 5)
--	if i == 1 then
--	    map.MapDeleteCreature(MapID, InstanceID, TargetID)
        cre.MonsterSay(MapID, InstanceID, TargetID, 50064)
	    local BossID = map.MapCreateColCreature(MapID, InstanceID, 1010411, 500, 5664, 1778, 1, "")
	    map.MapCreateColCreature(MapID, InstanceID, 1010417, 495, 5664, 1796, 1, "")
	    map.MapCreateColCreature(MapID, InstanceID, 1010417, 508, 5664, 1796, 1, "")
	    map.MapCreateColCreature(MapID, InstanceID, 1010414, 486, 5664, 1781, 1, "")
	    map.MapCreateColCreature(MapID, InstanceID, 1010414, 515, 5664, 1780, 1, "")
	    map.MapCreateColCreature(MapID, InstanceID, 1010414, 497, 5664, 1765, 1, "")
	    map.MapCreateColCreature(MapID, InstanceID, 1010414, 506, 5664, 1765, 1, "")
	   -- map.OpenCloseDoor(MapID, InstanceID, 1524, 1)
	   -- map.OpenCloseDoor(MapID, InstanceID, 1525, 1)
	    map.SetMapScriptData(MapID, InstanceID, 1, 2, BossID)

	local Num = map.GetMapScriptData(MapID, InstanceID, 1, 13) + 1000
	map.SetMapScriptData(MapID, InstanceID, 1, 13, Num)
	if Num >= 1100 then
	    local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 101, 3351)	--ͨ����Ԫ��ħ�ڶ���̬����Ĵ����Ѿ��򿪣�
		msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID)
	    map.OpenCloseDoor(MapID, InstanceID, 1524, 1)
	    map.OpenCloseDoor(MapID, InstanceID, 1525, 1)
	elseif Num < 1100 then
	    local a = math.floor(Num/1000)
		local b = Num % 1000
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 3352)	--��ɱආ�XX/100,��ɱ��Ԫ��ħ��һ��̬X/1
		msg.AddMsgEvent(MsgID, 9, b)
		msg.AddMsgEvent(MsgID, 9, a)
		msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID)
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 3353)	--������ɺ󽫻Ὺ��ͨ���������췿��Ĵ��ţ�
		msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID)
	end
--	end
	Increase_shili(MapID, InstanceID, RoleID, 1, 4, "jinjieshili")

end
aux.RegisterCreatureEvent(1010410, 4, "c1010410_OnDie")


function c1010411_OnEnterCombat(MapID, InstanceID, CreatureID)
    cre.MonsterSay(MapID, InstanceID, CreatureID, 50065)
	map.OpenCloseDoor(MapID, InstanceID, 1524, nil)
	map.OpenCloseDoor(MapID, InstanceID, 1525, nil)

end

aux.RegisterCreatureEvent(1010411, 2, "c1010411_OnEnterCombat")

function c1010411_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

    local TeamID = role.IsRoleHaveTeam(MapID, InstanceID, RoleID)
	local Role = {}
	Role[1], Role[2], Role[3], Role[4], Role[5], Role[6] = role.GetRoleTeamMemID(TeamID)
	for i=1, 6 do
	    if Role[i] and Role[i]~= 4294967295 then
		    local Target_MapID, Target_InstanceID = role.GetRoleMapID(Role[i])
			if Target_MapID == MapID then
			    role.AddRoleItem(Target_MapID, Target_InstanceID, Role[i], 3305003, 5, -1, 8, 420)
			    if act.GetActIsStart(151) then
				    HuoDeYinYuanJie(Role[i])
					HuoDeYinYuanJie(Role[i])
					HuoDeYinYuanJie(Role[i])
					HuoDeYinYuanJie(Role[i])
					HuoDeYinYuanJie(Role[i])
				end
			end
	    end
	end

    cre.MonsterSay(MapID, InstanceID, TargetID, 50066)
	map.OpenCloseDoor(MapID, InstanceID, 1524, 1)
	map.OpenCloseDoor(MapID, InstanceID, 1525, 1)
	map.MapCreateCreature(MapID, InstanceID, 3075203, 500, 5664, 1778)
	Increase_shili(MapID, InstanceID, RoleID, 1, 4, "jinjieshili")

end
aux.RegisterCreatureEvent(1010411, 4, "c1010411_OnDie")

function c1010411_LeaveCombat(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	map.OpenCloseDoor(MapID, InstanceID, 1524, 1)
	map.OpenCloseDoor(MapID, InstanceID, 1525, 1)

end
aux.RegisterCreatureEvent(1010411, 3, "c1010411_LeaveCombat")



function c3075211_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	local BossID = map.GetMapScriptData(MapID, InstanceID, 1, 2)
	unit.CancelBuff(MapID, InstanceID, BossID, 9422602)
end
aux.RegisterCreatureEvent(3075211, 4, "c3075211_OnDie")
