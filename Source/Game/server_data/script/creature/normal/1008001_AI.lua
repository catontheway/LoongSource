--��ɱ�� ����ħ�� 1008001 ˢ�� ����ʯ 3060027 / ������Ч txcj09 ���꣺x 1464 y 15330 z 598
function c1008001_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	map.MapCreateCreature(MapID, InstanceID, 3060027, 1464, 15330, 598)
end
aux.RegisterCreatureEvent(1008001, 4, "c1008001_OnDie")

-- �������ս������ʾ��
function c1008001_OnEnterCombat(MapID, InstanceID, CreatureID)
    --�õ�����ĵ�ǰĿ��
    local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
    --ͬ����޸���ΧΪ10�����ӣ��߶�Ϊ20���ѷ�����
    g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID, 1, 10, 20)
end

aux.RegisterCreatureEvent(1008001, 2, "c1008001_OnEnterCombat")
