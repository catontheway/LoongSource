--��ɱ�� Ӣ������ 1008091 ˢ�� Ӣ�鱦�� 3060035 / ������Ч txcj47 ���꣺x 1154 y 14780 z 523
function c1008091_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	map.MapCreateCreature(MapID, InstanceID, 3060035, 1154, 14780, 523)
end
aux.RegisterCreatureEvent(1008091, 4, "c1008091_OnDie")

-- �������ս������ʾ��
function c1008091_OnEnterCombat(MapID, InstanceID, CreatureID)
    --�õ�����ĵ�ǰĿ��
    local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
    --ͬ����޸���ΧΪ10�����ӣ��߶�Ϊ20���ѷ�����
    g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID, 1, 10, 20)
end

aux.RegisterCreatureEvent(1008091, 2, "c1008091_OnEnterCombat")
