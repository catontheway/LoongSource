--���ﺰ���ű�

-- �������ս������/ʾ��
function c1004605_OnEnterCombat(MapID, InstanceID, CreatureID)
	--�õ�����ĵ�ǰĿ��
    local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
    --ͬ����޸���ΧΪ70�����ӣ��߶�Ϊ20���ѷ�����
    g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID, 1, 70, 20)
    cre.MonsterSay(MapID, InstanceID, CreatureID, 10052)

end

aux.RegisterCreatureEvent(1004605, 2, "c1004605_OnEnterCombat")


--������������
function c1004605_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

             cre.MonsterSay(MapID, InstanceID, TargetID, 10053)

end

aux.RegisterCreatureEvent(1004605, 4, "c1004605_OnDie")

