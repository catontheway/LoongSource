--���ﺰ���ű�/ʾ���ű�

-- �������ս������
function c1006532_OnEnterCombat(MapID, InstanceID, CreatureID)

             cre.MonsterSay(MapID, InstanceID, CreatureID, 30103)
			     --�õ�����ĵ�ǰĿ��
    local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
    --ͬ����޸���ΧΪ8�����ӣ��߶�Ϊ20���ѷ�����
    g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID, 1, 10, 20)

end

aux.RegisterCreatureEvent(1006532, 2, "c1006532_OnEnterCombat")



--������������
function c1006532_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

	cre.MonsterSay(MapID, InstanceID, TargetID, 30104)
	Increase_shili(MapID, InstanceID, RoleID, 1, 2, "jinjieshili")

	local Boss_num=map.GetMapScriptData(MapID, InstanceID, 1, 4)+1000
	map.SetMapScriptData(MapID, InstanceID, 1, 4, Boss_num)

	if Boss_num==1111 then
		map.MapCreateCreature(MapID, InstanceID, 1006533, 1509, 10460, 1408)
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 101, 1746)	--���͵��Ѿ�����������[1509, 1408]��
		msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID)
	end

end

aux.RegisterCreatureEvent(1006532, 4, "c1006532_OnDie")






