-- �������ս������ʾ��
function m42_xiaoguai_OnEnterCombat(MapID, InstanceID, CreatureID)
    --�õ�����ĵ�ǰĿ��
    local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
    --ͬ����޸���ΧΪ3�����ӣ��߶�Ϊ20���ѷ�����
    g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID, 1, 10, 20)
end

aux.RegisterCreatureEvent(1006701, 2, "m42_xiaoguai_OnEnterCombat")
aux.RegisterCreatureEvent(1006702, 2, "m42_xiaoguai_OnEnterCombat")
aux.RegisterCreatureEvent(1006703, 2, "m42_xiaoguai_OnEnterCombat")
aux.RegisterCreatureEvent(1006704, 2, "m42_xiaoguai_OnEnterCombat")
aux.RegisterCreatureEvent(1006705, 2, "m42_xiaoguai_OnEnterCombat")
aux.RegisterCreatureEvent(1006706, 2, "m42_xiaoguai_OnEnterCombat")
aux.RegisterCreatureEvent(1006707, 2, "m42_xiaoguai_OnEnterCombat")
aux.RegisterCreatureEvent(1006708, 2, "m42_xiaoguai_OnEnterCombat")
aux.RegisterCreatureEvent(1006709, 2, "m42_xiaoguai_OnEnterCombat")
aux.RegisterCreatureEvent(1006710, 2, "m42_xiaoguai_OnEnterCombat")
aux.RegisterCreatureEvent(1006711, 2, "m42_xiaoguai_OnEnterCombat")
aux.RegisterCreatureEvent(1006712, 2, "m42_xiaoguai_OnEnterCombat")
aux.RegisterCreatureEvent(1006713, 2, "m42_xiaoguai_OnEnterCombat")
aux.RegisterCreatureEvent(1006714, 2, "m42_xiaoguai_OnEnterCombat")
aux.RegisterCreatureEvent(1006715, 2, "m42_xiaoguai_OnEnterCombat")
aux.RegisterCreatureEvent(1006716, 2, "m42_xiaoguai_OnEnterCombat")
aux.RegisterCreatureEvent(1006717, 2, "m42_xiaoguai_OnEnterCombat")
aux.RegisterCreatureEvent(1006718, 2, "m42_xiaoguai_OnEnterCombat")
aux.RegisterCreatureEvent(1006723, 2, "m42_xiaoguai_OnEnterCombat")
aux.RegisterCreatureEvent(1006724, 2, "m42_xiaoguai_OnEnterCombat")
aux.RegisterCreatureEvent(1006725, 2, "m42_xiaoguai_OnEnterCombat")
aux.RegisterCreatureEvent(1006726, 2, "m42_xiaoguai_OnEnterCombat")
aux.RegisterCreatureEvent(1006727, 2, "m42_xiaoguai_OnEnterCombat")
aux.RegisterCreatureEvent(1006728, 2, "m42_xiaoguai_OnEnterCombat")



--[[����1 ��ɱ 1006717 ����
        x=785 y=3923 z=757��
        x=804 y=3923 z=749��
        x=795 y=3923 z=779��
        x=814 y=3923 z=768��
        �ĸ���ˢ�� 1006718
]]

function c1006717_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	map.MapCreateCreature(MapID, InstanceID, 1006718, 804, 3923, 749)
	map.MapCreateCreature(MapID, InstanceID, 1006718, 795, 3923, 779)
	map.MapCreateCreature(MapID, InstanceID, 1006718, 814, 3923, 768)
	map.MapCreateCreature(MapID, InstanceID, 1006718, 785, 3923, 757)

	local xiaoguai_num = map.GetMapScriptData(MapID, InstanceID, 1, 10)+1
	map.SetMapScriptData(MapID, InstanceID, 1, 10, xiaoguai_num)
	local Boss_num = map.GetMapScriptData(MapID, InstanceID, 1, 11)

	local Boss1 = Boss_num % 2
	local Boss2 = math.floor(Boss_num/2) % 2
	local Boss3 = math.floor(Boss_num/4) % 2
	local Boss4 = math.floor(Boss_num/8) % 2

	local temp = Boss1+Boss2+Boss3+Boss4

	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 26, 1757)	--�ѻ�ɱ����С��xx/xx���ѻ�ɱ����Bossxx/4��
	msg.AddMsgEvent(MsgID, 9, xiaoguai_num)
	msg.AddMsgEvent(MsgID, 9, temp)
	msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID)
end

--ע��
aux.RegisterCreatureEvent(1006717, 4, "c1006717_OnDie")

--����2  ����ˢ�����ĸ��� 1006718 ����ɱ���Ժ� �������� x=800 y=3923 z=757 ˢ�� 1006722
function c1006718_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

	local a = map.GetMapScriptData(MapID,InstanceID,1,1) --����Data����1
	a = a + 1
	map.SetMapScriptData(MapID,InstanceID,1,1,a)

	if a == 4 then
		map.MapCreateCreature(MapID, InstanceID, 1006722, 800, 3923, 757)
	end

	local xiaoguai_num = map.GetMapScriptData(MapID, InstanceID, 1, 10)+1
	map.SetMapScriptData(MapID, InstanceID, 1, 10, xiaoguai_num)
	local Boss_num = map.GetMapScriptData(MapID, InstanceID, 1, 11)

	local Boss1 = Boss_num % 2
	local Boss2 = math.floor(Boss_num/2) % 2
	local Boss3 = math.floor(Boss_num/4) % 2
	local Boss4 = math.floor(Boss_num/8) % 2

	local temp = Boss1+Boss2+Boss3+Boss4

	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 26, 1757)	--�ѻ�ɱ����С��xx/xx���ѻ�ɱ����Bossxx/4��
	msg.AddMsgEvent(MsgID, 9, xiaoguai_num)
	msg.AddMsgEvent(MsgID, 9, temp)
	msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID)

end

--ע��
aux.RegisterCreatureEvent(1006718,4,"c1006718_OnDie")

function m42_xiaoguai_OnDie(MapID, InstanceID, CreatureID)

	local xiaoguai_num = map.GetMapScriptData(MapID, InstanceID, 1, 10)+1
	map.SetMapScriptData(MapID, InstanceID, 1, 10, xiaoguai_num)
	local Boss_num = map.GetMapScriptData(MapID, InstanceID, 1, 11)

	local Boss1 = Boss_num % 2
	local Boss2 = math.floor(Boss_num/2) % 2
	local Boss3 = math.floor(Boss_num/4) % 2
	local Boss4 = math.floor(Boss_num/8) % 2

	local temp = Boss1+Boss2+Boss3+Boss4

	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 26, 1757)	--�ѻ�ɱ����С��xx/xx���ѻ�ɱ����Bossxx/4��
	msg.AddMsgEvent(MsgID, 9, xiaoguai_num)
	msg.AddMsgEvent(MsgID, 9, temp)
	msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID)

end

aux.RegisterCreatureEvent(1006701, 4, "m42_xiaoguai_OnDie")
aux.RegisterCreatureEvent(1006702, 4, "m42_xiaoguai_OnDie")
aux.RegisterCreatureEvent(1006703, 4, "m42_xiaoguai_OnDie")
aux.RegisterCreatureEvent(1006704, 4, "m42_xiaoguai_OnDie")
aux.RegisterCreatureEvent(1006705, 4, "m42_xiaoguai_OnDie")
aux.RegisterCreatureEvent(1006706, 4, "m42_xiaoguai_OnDie")
aux.RegisterCreatureEvent(1006707, 4, "m42_xiaoguai_OnDie")
aux.RegisterCreatureEvent(1006708, 4, "m42_xiaoguai_OnDie")
aux.RegisterCreatureEvent(1006709, 4, "m42_xiaoguai_OnDie")
aux.RegisterCreatureEvent(1006710, 4, "m42_xiaoguai_OnDie")
aux.RegisterCreatureEvent(1006711, 4, "m42_xiaoguai_OnDie")
aux.RegisterCreatureEvent(1006712, 4, "m42_xiaoguai_OnDie")
aux.RegisterCreatureEvent(1006713, 4, "m42_xiaoguai_OnDie")
aux.RegisterCreatureEvent(1006714, 4, "m42_xiaoguai_OnDie")
aux.RegisterCreatureEvent(1006715, 4, "m42_xiaoguai_OnDie")
aux.RegisterCreatureEvent(1006716, 4, "m42_xiaoguai_OnDie")

aux.RegisterCreatureEvent(1006723, 4, "m42_xiaoguai_OnDie")
aux.RegisterCreatureEvent(1006724, 4, "m42_xiaoguai_OnDie")
aux.RegisterCreatureEvent(1006725, 4, "m42_xiaoguai_OnDie")
aux.RegisterCreatureEvent(1006726, 4, "m42_xiaoguai_OnDie")
aux.RegisterCreatureEvent(1006727, 4, "m42_xiaoguai_OnDie")
aux.RegisterCreatureEvent(1006728, 4, "m42_xiaoguai_OnDie")
