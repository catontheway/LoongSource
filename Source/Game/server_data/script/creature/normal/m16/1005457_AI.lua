--���ﺰ���ű�

-- �������ս������/ʾ���ű�
function c1005457_OnEnterCombat(MapID, InstanceID, CreatureID)

	cre.MonsterSay(MapID, InstanceID, CreatureID, 20084)
	--�õ�����ĵ�ǰĿ��
    local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
    --ͬ����޸���ΧΪ8�����ӣ��߶�Ϊ20���ѷ�����
    g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID, 1, 10, 20)

end

aux.RegisterCreatureEvent(1005457, 2, "c1005457_OnEnterCombat")

--������������
function c1005457_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

	cre.MonsterSay(MapID, InstanceID, TargetID, 20086)
	Increase_shili(MapID, InstanceID, RoleID, 1, 2, "jinjieshili")

	map.MapCreateColCreature(MapID, InstanceID, 1005458, 701, 21173, 678, 1)
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 101, 1728)	--�����Ѿ�����������[701, 678]��
	msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID)

end

aux.RegisterCreatureEvent(1005457, 4, "c1005457_OnDie")




