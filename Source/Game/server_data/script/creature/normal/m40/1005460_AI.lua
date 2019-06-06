--���ﺰ���ű�/ʾ���ű�

-- �������ս������
function c1005460_OnEnterCombat(MapID, InstanceID, CreatureID)

	cre.MonsterSay(MapID, InstanceID, CreatureID, 20055)
	--�õ�����ĵ�ǰĿ��
    local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
    --ͬ����޸���ΧΪ8�����ӣ��߶�Ϊ20���ѷ�����
    g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID, 1, 10, 20)

end

aux.RegisterCreatureEvent(1005460, 2, "c1005460_OnEnterCombat")

--������������
function c1005460_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	Increase_shili(MapID, InstanceID, RoleID, 1, 2, "jinjieshili")


	local xiaoguai_num = map.GetMapScriptData(MapID, InstanceID, 1, 11)
	local Boss_num = map.GetMapScriptData(MapID, InstanceID, 1, 8)+4
	map.SetMapScriptData(MapID, InstanceID, 1, 8, Boss_num)

	local Boss1 = Boss_num % 2
	local Boss2 = math.floor(Boss_num/2) % 2
	local Boss3 = math.floor(Boss_num/4) % 2
	local Boss4 = math.floor(Boss_num/8) % 2

	local temp = Boss1+Boss2+Boss3+Boss4

	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 26, 1756)	--�ѻ�ɱ����С��xx/xx���ѻ�ɱ����Bossxx/4��
	msg.AddMsgEvent(MsgID, 9, xiaoguai_num)
	msg.AddMsgEvent(MsgID, 9, temp)
	msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID)
end

aux.RegisterCreatureEvent(1005460, 4, "c1005460_OnDie")
