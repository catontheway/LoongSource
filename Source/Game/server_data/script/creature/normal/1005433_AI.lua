--���ﺰ���ű�

-- �������ս������/ʾ���ű�
function c1005433_OnEnterCombat(MapID, InstanceID, CreatureID)

             cre.MonsterSay(MapID, InstanceID, CreatureID, 20067)
			     --�õ�����ĵ�ǰĿ��
    local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
    --ͬ����޸���ΧΪ8�����ӣ��߶�Ϊ20���ѷ�����
    g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID, 1, 10, 20)

end

aux.RegisterCreatureEvent(1005433, 2, "c1005433_OnEnterCombat")


--������������
function c1005433_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

    cre.MonsterSay(MapID, InstanceID, TargetID, 20069)
	Increase_shili(MapID, InstanceID, RoleID, 1, 2, "jinjieshili")

end

aux.RegisterCreatureEvent(1005433, 4, "c1005433_OnDie")



