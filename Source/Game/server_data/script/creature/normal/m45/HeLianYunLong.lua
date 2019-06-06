--��͸����ʱ��1������ʱ�䣬����ʱ��ʹ��
function c3075402_OnDisappear(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
    map.MapCreateColCreature(MapID, InstanceID, 1010503, 898, 7000, 146, 1, "")
    map.MapCreateColCreature(MapID, InstanceID, 1010503, 886, 7000, 170, 1, "")
    map.MapCreateColCreature(MapID, InstanceID, 1010503, 862, 7000, 146, 1, "")
    map.MapCreateColCreature(MapID, InstanceID, 1010505, 899, 7000, 158, 1, "")
    map.MapCreateColCreature(MapID, InstanceID, 1010505, 865, 7000, 166, 1, "")
    map.MapCreateColCreature(MapID, InstanceID, 1010505, 872, 7000, 137, 1, "")
    local wave = map.GetMapScriptData(MapID,InstanceID,1,3)
    map.SetMapScriptData(MapID,InstanceID,1,3,wave+1)
    if wave+1 == 2 then
		map.MapCreateColCreature(MapID, InstanceID, 3075402, 154, 4320, 657, 1, "")--͸����ʱ��1����ʱ��1�ְ�
   		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 101, 1775)	--60���ˢ����һ������������
		msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID)     --������Ϣ
	end
end
aux.RegisterCreatureEvent(3075402, 13, "c3075402_OnDisappear")

-- �������ս������
function c1010527_OnEnterCombat(MapID, InstanceID, CreatureID)
	cre.MonsterSay(MapID, InstanceID, CreatureID, 50071)
end

aux.RegisterCreatureEvent(1010527, 2, "c1010527_OnEnterCombat")


function c1010527_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
    cre.MonsterSay(MapID, InstanceID, TargetID, 50072)
	local Boss_num = map.GetMapScriptData(MapID,InstanceID,1,4)+1
	map.SetMapScriptData(MapID,InstanceID,1,4,Boss_num)
	if Boss_num%4 == 3 then
		map.MapCreateColCreature(MapID, InstanceID, 1010529, 418, 6101, 795, 1, "")
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 101, 1776)	--���Թ���Ѿ�����������[418, 795]��
		msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID)     --������Ϣ
	end
	Increase_shili(MapID, InstanceID, RoleID, 1, 4, "jinjieshili")

	local xiaoguai_num = map.GetMapScriptData(MapID, InstanceID, 1, 14)

	local Boss1 = Boss_num % 2
	local Boss2 = math.floor(Boss_num/2) % 2
	local Boss3 = math.floor(Boss_num/4) % 2
	local Boss4 = math.floor(Boss_num/8) % 2

	local temp = Boss1+Boss2+Boss3+Boss4

	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 26, 1777)	--�ѻ�ɱ����С��xx/xx���ѻ�ɱ����Bossxx/4��
	msg.AddMsgEvent(MsgID, 9, xiaoguai_num)
	msg.AddMsgEvent(MsgID, 9, temp)
	msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID)

end
aux.RegisterCreatureEvent(1010527, 4, "c1010527_OnDie")
