-- �������ս������
function c1010406_OnEnterCombat(MapID, InstanceID, CreatureID)
	cre.MonsterSay(MapID, InstanceID, CreatureID, 50062)
end

aux.RegisterCreatureEvent(1010406, 2, "c1010406_OnEnterCombat")

--ȫ����ɱ����ʦ��1010406���ͲƱ�������1010407����ˢ��
--����һ����3075207�� X=451 Y=3840 Z=1011

--������ؼ�buff��ɱ����ʦ��buff

function c1010406_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	unit.CancelBuff(MapID, InstanceID, RoleID, 5003301)
	local TeamID = role.IsRoleHaveTeam(MapID, InstanceID, RoleID)
	local Role = {}
	Role[1], Role[2], Role[3], Role[4], Role[5], Role[6] = role.GetRoleTeamMemID(TeamID)
	for i=1, 6 do
	    if Role[i] and Role[i]~= 4294967295 and Role[i] ~= RoleID then
		    unit.CancelBuff(MapID, InstanceID, Role[i], 5003301)
	    end
	end
	local i = map.GetMapScriptData(MapID, InstanceID, 1, 0)
	map.SetMapScriptData(MapID, InstanceID, 1, 0, i+1)
	if i+1 == 2 then
		map.MapCreateCreature(MapID, InstanceID, 3075207, 451, 3840, 1011)
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 101, 1749)	--�ƺ�ʲô�ط�������һ�����䣡
		msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID)
	end
	--���û��С��
	Increase_shili(MapID, InstanceID, RoleID, 1, 4, "jinjieshili")

end


aux.RegisterCreatureEvent(1010406, 4, "c1010406_OnDie")

function c1010407_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	local i = map.GetMapScriptData(MapID, InstanceID, 1, 0)
	map.SetMapScriptData(MapID, InstanceID, 1, 0, i+1)
	if i+1 == 2 then
		map.MapCreateCreature(MapID, InstanceID, 3075207, 451, 3840, 1011)
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 101, 1749)	--�ƺ�ʲô�ط�������һ�����䣡
		msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID)
	end
	Increase_shili(MapID, InstanceID, RoleID, 1, 4, "jinjieshili")

end

aux.RegisterCreatureEvent(1010407, 4, "c1010407_OnDie")

function c3075213_OnInvest(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

	map.SetMapScriptData(MapID, InstanceID, 1, 6, 1)
	unit.AddBuff(MapID, InstanceID, RoleID, 5003301, RoleID)

	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 101, 1750)	--���ܵ��˻��ص�ʧѪ״̬Ӱ�죬ֻ�л�ɱ�������ʦ���ɽ����
	msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID)

end
aux.RegisterCreatureEvent(3075213, 6, "c3075213_OnInvest")

function c1010418_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

	local Num = map.GetMapScriptData(MapID, InstanceID, 1, 13) + 1
	map.SetMapScriptData(MapID, InstanceID, 1, 13, Num)
	if Num == 1100 then
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

	local Youhun_num = map.GetMapScriptData(MapID, InstanceID, 1, 7)+1
	map.SetMapScriptData(MapID, InstanceID, 1, 7, Youhun_num)

	if Youhun_num<30 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 1751)	--���ѻ�ɱxx��Թ���Ļ꣬��ɱ30���������ʦ������֣�
		msg.AddMsgEvent(MsgID, 9, Youhun_num)
		msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID)
	elseif Youhun_num==30 then
		map.MapCreateCreature(MapID, InstanceID, 1010406, 242, 3840, 612)
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 101, 1752)	--�������ʦ�Ѿ�����������[242, 612]��
		msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID)
	end

	if Youhun_num%5 == 0 and Youhun_num~= 30 then
		local x, y, z = unit.GetPosition(MapID, InstanceID, TargetID)
		map.MapCreateCreature(MapID, InstanceID, 1010418, GetMiddle(x, 100, 193), y, GetMiddle(z, 565, 766))
		map.MapCreateCreature(MapID, InstanceID, 1010418, GetMiddle(x+8, 100, 193), y, GetMiddle(z+5, 565, 766))
		map.MapCreateCreature(MapID, InstanceID, 1010418, GetMiddle(x+5, 100, 193), y, GetMiddle(z-6, 565, 766))
		map.MapCreateCreature(MapID, InstanceID, 1010418, GetMiddle(x-6, 100, 193), y, GetMiddle(z-7, 565, 766))
		map.MapCreateCreature(MapID, InstanceID, 1010418, GetMiddle(x-7, 100, 193), y, GetMiddle(z+8, 565, 766))
	end
end


aux.RegisterCreatureEvent(1010418, 4, "c1010418_OnDie")

function GetMiddle(value, min_v, max_v)
	if value>= max_v then
		return max_v
	end

	if value<= min_v then
		return min_v
	end

	return value
end
