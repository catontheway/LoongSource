-- �������ս������ʾ��
function c1006719_OnEnterCombat(MapID, InstanceID, CreatureID)
    --�õ�����ĵ�ǰĿ��
    local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
    --ͬ����޸���ΧΪ3�����ӣ��߶�Ϊ20���ѷ�����
    g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID, 1, 10, 20)
end

aux.RegisterCreatureEvent(1006719, 2, "c1006719_OnEnterCombat")

--����3  ��ɱ 1006719 �������� x=972 y= 2680 z=763 ˢ�� ͨڤ��ʯ 3040534

function c1006719_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	map.MapCreateCreature(MapID, InstanceID, 3040534, 953, 2680, 572)
	Increase_shili(MapID, InstanceID, RoleID, 1, 2, "jinjieshili")

	local xiaoguai_num = map.GetMapScriptData(MapID, InstanceID, 1, 10)
	local Boss_num = map.GetMapScriptData(MapID, InstanceID, 1, 11)+1
	map.SetMapScriptData(MapID, InstanceID, 1, 11, Boss_num)

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
aux.RegisterCreatureEvent(1006719,4,"c1006719_OnDie")


